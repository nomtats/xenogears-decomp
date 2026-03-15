#!/usr/bin/env python3
"""Compare a target matching .s file against the compiled dump for a given function.

Usage:
    python3 tools/asmdiff.py <function_name> [source_file.c]
    python3 tools/asmdiff.py --build <function_name> [source_file.c]

Examples:
    python3 tools/asmdiff.py MoveImage
    python3 tools/asmdiff.py --build MoveImage
    python3 tools/asmdiff.py MoveImage src/slus_006.64/psyq/libgpu.c

Flags:
    --build     Run `make` to compile the source into an object file before
                diffing. The build target is inferred from the function name
                (or from the explicit source_file argument).
    --analyze   Perform heuristic cluster analysis on mismatches to suggest
                fixes based on KB patterns (e.g., branch polarity inversion).

How it works:
    1. Locates the target assembly in asm/<target>/matchings/<lib>/<module>/<Func>.s
    2. Infers the compiled dump at build/src/<target>/<lib>/<module>.c.dump.s
    3. Aligns target and generated instruction blocks using Python SequenceMatcher.
    4. Compares instruction-by-instruction, masking relocation fields (address
       immediates for lui/addiu/lw/sw, jump targets for j/jal). 
    5. Highlights line-level opcode and argument differences (e.g., register drift)
       in bold yellow format. 
    6. Prints a color-coded side-by-side diff with a match score.

Via the Makefile (recommended — builds then diffs in one step):
    make asmdiff FUNC=MoveImage

All commands run inside Docker:
    docker exec xenogears_decomp_env make asmdiff FUNC=MoveImage
"""

import re
import subprocess
import sys
from collections import Counter
from difflib import SequenceMatcher
from pathlib import Path

RESET = "\033[0m"
GREEN = "\033[32m"
RED = "\033[31m"
YELLOW = "\033[33m"
DIM = "\033[2m"
BOLD = "\033[1m"
CYAN = "\033[36m"


def find_target_asm(func_name: str) -> Path | None:
    """Walk asm/**/nonmatchings/ or asm/**/matchings/ to find <func_name>.s"""
    asm_root = Path("asm")
    for p in asm_root.rglob(f"{func_name}.s"):
        if "matchings" in p.parts or "nonmatchings" in p.parts:
            return p
    return None


def infer_dump_path(target_path: Path) -> Path | None:
    """Derive build/.../foo.c.dump.s from asm/.../matchings/.../FuncName.s
    
    The directory structure: asm/<target>/nonmatchings/<lib>/<module>/<Func>.s
    maps to source:          src/<target>/<lib>/<module>.c
    """
    parts = list(target_path.parts)
    
    # Find the index of either 'matchings' or 'nonmatchings'
    match_idx = -1
    for i, part in enumerate(parts):
        if part in ("matchings", "nonmatchings"):
            match_idx = i
            break
            
    if match_idx == -1:
        return None

    target_name = "/".join(parts[1:match_idx])
    after_match = parts[match_idx + 1:]

    if len(after_match) < 2:
        return None

    lib_module = "/".join(after_match[:-1])
    dump = Path(f"build/src/{target_name}/{lib_module}.c.dump.s")
    if dump.exists():
        return dump

    if len(after_match) >= 3:
        lib_module2 = "/".join(after_match[:-2])
        dump2 = Path(f"build/src/{target_name}/{lib_module2}.c.dump.s")
        if dump2.exists():
            return dump2

    return dump


def dump_path_to_obj_target(dump_path: Path) -> str:
    """build/src/.../libgpu.c.dump.s -> build/src/.../libgpu.c.o"""
    return str(dump_path).replace(".dump.s", ".o")


def resolve_paths(func_name: str, source_file: str | None) -> tuple[Path, Path]:
    """Resolve the target .s path and dump .dump.s path for a function."""
    target_path = find_target_asm(func_name)
    if target_path is None:
        print(f"Error: Could not find asm/**/matchings/**/{func_name}.s", file=sys.stderr)
        sys.exit(1)

    if source_file:
        dump_path = Path(f"build/{source_file}".replace(".c", ".c.dump.s"))
    else:
        dump_path = infer_dump_path(target_path)

    if dump_path is None:
        print(f"Error: Could not infer dump path from {target_path}", file=sys.stderr)
        sys.exit(1)

    return target_path, dump_path


def build_obj(dump_path: Path):
    """Run make to compile the object file that produces this dump."""
    obj_target = dump_path_to_obj_target(dump_path)
    result = subprocess.run(["make", obj_target], capture_output=False)
    if result.returncode != 0:
        print(f"Error: build failed for {obj_target}", file=sys.stderr)
        sys.exit(1)


def parse_target_asm(path: Path) -> list[tuple[str, str]]:
    """Parse a matching .s file into [(hex_word, mnemonic), ...].

    Lines look like:
        /* 3515C 8004495C E0FFBD27 */  addiu      $sp, $sp, -0x20
    The hex word (E0FFBD27) is little-endian byte order as stored on disc.
    We byte-swap to native instruction word format to match objdump output.
    """
    pattern = re.compile(
        r'/\*\s+[0-9a-fA-F]+\s+[0-9a-fA-F]+\s+([0-9a-fA-F]{8})\s+\*/\s+(.*)'
    )
    result = []
    for line in path.read_text().splitlines():
        m = pattern.search(line)
        if m:
            raw = m.group(1).lower()
            word = raw[6:8] + raw[4:6] + raw[2:4] + raw[0:2]
            mnemonic = re.sub(r'\$', '', m.group(2).strip())
            result.append((word, mnemonic))
    return result


def parse_dump_func(path: Path, func_name: str) -> list[tuple[str, str, bool, list[str]]]:
    """Extract [(hex_word, mnemonic, has_reloc, source_lines), ...] for func_name from objdump."""
    lines = path.read_text().splitlines()
    in_func = False
    result = []
    func_header = re.compile(r'^[0-9a-f]+\s+<' + re.escape(func_name) + r'>:')
    # Match next real function but ignore local debug labels like <LM3>
    next_func = re.compile(r'^[0-9a-f]+\s+<(?![L.]).*?>:')
    instr_pat = re.compile(r'^\s+[0-9a-f]+:\s+([0-9a-f]{8})\s+(.*)')
    reloc_pat = re.compile(r'^\s+[0-9a-f]+:\s+R_MIPS_')

    pending_source = []

    for line in lines:
        if not in_func:
            if func_header.match(line):
                in_func = True
            continue
        if next_func.match(line):
            break
            
        m = instr_pat.match(line)
        if m:
            result.append((m.group(1).lower(), m.group(2).strip(), False, pending_source))
            pending_source = []
            continue
            
        if reloc_pat.match(line) and result:
            last = result[-1]
            result[-1] = (last[0], last[1], True, last[3])
            continue
            
        # Not an instruction, reloc, or label header. Treat as source if not noise
        if not re.match(r'^[0-9a-f]+\s+<', line):
            text = line.strip()
            if text and not text.startswith("Disassembly of") and "file format" not in text:
                pending_source.append(line.replace('\t', '    '))

    return result


def reloc_mask(hex_str: str) -> int:
    """Bitmask for comparing a relocated instruction.

    J-type (j/jal): upper 6 bits stable.
    I-type (everything else): upper 16 bits stable.
    """
    opcode = (int(hex_str, 16) >> 26) & 0x3F
    if opcode in (2, 3):
        return 0xFC000000
    return 0xFFFF0000


def format_mnemonic(s: str, width: int = 38) -> str:
    s = s.replace('\t', '    ')
    if len(s) > width:
        return s[:width - 1] + "…"
    return s.ljust(width)


def decode_mips_word(hex_str: str) -> dict | None:
    """Decode a MIPS instruction word into basic fields."""
    if hex_str == "--------":
        return None
    word = int(hex_str, 16)
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    target = word & 0x03FFFFFF
    return {
        "word": word,
        "opcode": opcode,
        "rs": rs,
        "rt": rt,
        "rd": rd,
        "shamt": shamt,
        "funct": funct,
        "imm": imm,
        "target": target,
    }


def mnemonic_op(asm: str) -> str:
    if not asm or asm == "<missing>":
        return ""
    return asm.split()[0].strip().lower()


def align_key(hex_str: str, asm: str) -> str:
    """Key used for robust sequence alignment."""
    d = decode_mips_word(hex_str)
    if d is None:
        return "missing"
    opcode = d["opcode"]
    if opcode == 0:
        return f"r:{d['funct']}"
    return f"i:{opcode}"


def is_row_match(t_hex: str, g_hex: str, g_reloc: bool) -> bool:
    if t_hex == "--------" or g_hex == "--------":
        return False
    if g_reloc:
        mask = reloc_mask(g_hex)
        return (int(t_hex, 16) & mask) == (int(g_hex, 16) & mask)
    return t_hex == g_hex


def build_aligned_rows(target, generated):
    """Align target and generated instructions before mismatch analysis."""
    t_keys = [align_key(t_hex, t_asm) for (t_hex, t_asm) in target]
    g_keys = [align_key(g_hex, g_asm) for (g_hex, g_asm, _r, _s) in generated]
    sm = SequenceMatcher(a=t_keys, b=g_keys, autojunk=False)

    rows = []
    t_i = 0
    g_i = 0

    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            while t_i < i2 and g_i < j2:
                t_hex, t_asm = target[t_i]
                g_hex, g_asm, g_reloc, g_src = generated[g_i]
                rows.append({
                    "t_idx": t_i + 1,
                    "g_idx": g_i + 1,
                    "t_hex": t_hex,
                    "t_asm": t_asm,
                    "g_hex": g_hex,
                    "g_asm": g_asm,
                    "g_reloc": g_reloc,
                    "g_src": g_src,
                    "match": is_row_match(t_hex, g_hex, g_reloc),
                })
                t_i += 1
                g_i += 1
        elif tag == "replace":
            t_count = i2 - i1
            g_count = j2 - j1
            count = max(t_count, g_count)
            for k in range(count):
                has_t = t_i < i2
                has_g = g_i < j2
                if has_t:
                    t_hex, t_asm = target[t_i]
                    t_idx = t_i + 1
                    t_i += 1
                else:
                    t_hex, t_asm, t_idx = "--------", "<missing>", None
                if has_g:
                    g_hex, g_asm, g_reloc, g_src = generated[g_i]
                    g_idx = g_i + 1
                    g_i += 1
                else:
                    g_hex, g_asm, g_reloc, g_src, g_idx = "--------", "<missing>", False, [], None

                rows.append({
                    "t_idx": t_idx,
                    "g_idx": g_idx,
                    "t_hex": t_hex,
                    "t_asm": t_asm,
                    "g_hex": g_hex,
                    "g_asm": g_asm,
                    "g_reloc": g_reloc,
                    "g_src": g_src,
                    "match": is_row_match(t_hex, g_hex, g_reloc),
                })
        elif tag == "delete":
            while t_i < i2:
                t_hex, t_asm = target[t_i]
                rows.append({
                    "t_idx": t_i + 1,
                    "g_idx": None,
                    "t_hex": t_hex,
                    "t_asm": t_asm,
                    "g_hex": "--------",
                    "g_asm": "<missing>",
                    "g_reloc": False,
                    "g_src": [],
                    "match": False,
                })
                t_i += 1
        elif tag == "insert":
            while g_i < j2:
                g_hex, g_asm, g_reloc, g_src = generated[g_i]
                rows.append({
                    "t_idx": None,
                    "g_idx": g_i + 1,
                    "t_hex": "--------",
                    "t_asm": "<missing>",
                    "g_hex": g_hex,
                    "g_asm": g_asm,
                    "g_reloc": g_reloc,
                    "g_src": g_src,
                    "match": False,
                })
                g_i += 1

    return rows


def classify_cluster(rows) -> tuple[str, str, str]:
    """Return (type, confidence, hint) for a mismatch cluster."""
    paired = [r for r in rows if r["t_hex"] != "--------" and r["g_hex"] != "--------"]
    t_only = [r for r in rows if r["g_hex"] == "--------"]
    g_only = [r for r in rows if r["t_hex"] == "--------"]

    if t_only and not paired and not g_only:
        return ("deletion/omission", "high", "Generated block is missing target instructions.")
    if g_only and not paired and not t_only:
        return ("insertion/extra", "high", "Generated block contains extra instructions.")

    if paired:
        t_ops = [mnemonic_op(r["t_asm"]) for r in paired]
        g_ops = [mnemonic_op(r["g_asm"]) for r in paired]

        # Branch polarity / inversion
        polarity_pairs = {("beqz", "bnez"), ("bnez", "beqz"), ("beq", "bne"), ("bne", "beq")}
        if paired and all((mnemonic_op(r["t_asm"]), mnemonic_op(r["g_asm"])) in polarity_pairs for r in paired):
            return ("condition polarity", "medium", "Try inverting condition/body layout for this branch block.")

        same_op = sum(1 for t, g in zip(t_ops, g_ops) if t == g)
        same_op_ratio = same_op / len(paired) if paired else 0.0

        # Register allocation drift
        reg_like = 0
        reg_diff = 0
        imm_diff = 0
        for r in paired:
            t_dec = decode_mips_word(r["t_hex"])
            g_dec = decode_mips_word(r["g_hex"])
            if not t_dec or not g_dec:
                continue
            if t_dec["opcode"] == g_dec["opcode"]:
                if t_dec["opcode"] == 0 and t_dec["funct"] == g_dec["funct"] and t_dec["shamt"] == g_dec["shamt"]:
                    reg_like += 1
                    if (t_dec["rs"], t_dec["rt"], t_dec["rd"]) != (g_dec["rs"], g_dec["rt"], g_dec["rd"]):
                        reg_diff += 1
                elif t_dec["opcode"] in (2, 3):
                    if t_dec["target"] != g_dec["target"]:
                        imm_diff += 1
                else:
                    if t_dec["imm"] == g_dec["imm"]:
                        reg_like += 1
                        if (t_dec["rs"], t_dec["rt"]) != (g_dec["rs"], g_dec["rt"]):
                            reg_diff += 1
                    else:
                        imm_diff += 1

        if reg_like >= 2 and reg_diff >= max(1, reg_like // 2):
            return ("register allocation", "high", "Preserve operand temps/order or use narrow register pinning.")

        if same_op_ratio >= 0.7 and imm_diff >= max(1, len(paired) // 3):
            return ("immediate/value formation", "medium", "Expression regrouping may be changing constant assembly.")

        # Reordering / scheduling
        t_counter = Counter(t_ops)
        g_counter = Counter(g_ops)
        if t_counter == g_counter and t_ops != g_ops:
            return ("instruction reordering", "medium", "Likely scheduler/block-layout drift; try one barrier or block split.")

        if "j" in t_ops + g_ops and ("beqz" in t_ops + g_ops or "bnez" in t_ops + g_ops):
            return ("control-flow layout", "medium", "Try branch-body inversion or small goto split for exact jump shape.")

    return ("mixed mismatch", "low", "Multiple mismatch types overlap; inspect this region manually.")


def print_analysis(target, generated, func_name: str):
    rows = build_aligned_rows(target, generated)
    clusters = []
    current = []
    for r in rows:
        if not r["match"]:
            current.append(r)
        elif current:
            clusters.append(current)
            current = []
    if current:
        clusters.append(current)

    if not clusters:
        print(f"  {BOLD}{GREEN}Analysis: no mismatch clusters detected.{RESET}")
        print()
        return

    print(f"{BOLD}{CYAN}  Mismatch analysis for {func_name}{RESET}")
    for idx, cl in enumerate(clusters[:12], 1):
        kind, confidence, hint = classify_cluster(cl)
        t_positions = [r["t_idx"] for r in cl if r["t_idx"] is not None]
        g_positions = [r["g_idx"] for r in cl if r["g_idx"] is not None]
        t_span = f"{min(t_positions)}-{max(t_positions)}" if t_positions else "none"
        g_span = f"{min(g_positions)}-{max(g_positions)}" if g_positions else "none"
        color = GREEN if confidence == "high" else (YELLOW if confidence == "medium" else RED)
        print(
            f"  {idx:>2}. {kind:<24} "
            f"{color}[{confidence}]{RESET} "
            f"target:{t_span} gen:{g_span} len:{len(cl)}"
        )
        print(f"      hint: {hint}")
    if len(clusters) > 12:
        print(f"  ... {len(clusters) - 12} more clusters omitted")
    print()


def highlight_asm_diff(t_asm: str, g_asm: str, width: int = 38) -> str:
    t_asm = t_asm.replace('\t', '    ')
    g_asm = g_asm.replace('\t', '    ')
    if len(t_asm) > width: t_asm = t_asm[:width-1] + "…"
    if len(g_asm) > width: g_asm = g_asm[:width-1] + "…"
    
    if t_asm == "<missing>" or g_asm == "<missing>":
        return g_asm.ljust(width)
        
    t_tokens = re.split(r'([ \t,()]+)', t_asm)
    g_tokens = re.split(r'([ \t,()]+)', g_asm)
    
    sm = SequenceMatcher(None, t_tokens, g_tokens)
    res = []
    vis_len = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        for j in range(j1, j2):
            tok = g_tokens[j]
            if not tok: continue
            if tag in ('replace', 'insert') and tok.strip():
                # Highlight differing text with YELLOW background or BOLD YELLOW text
                res.append(f"{YELLOW}{BOLD}{tok}{RESET}{RED}") 
            else:
                res.append(tok)
            vis_len += len(tok)
            
    padding = max(0, width - vis_len)
    return "".join(res) + (" " * padding)


def print_diff(target, generated, func_name: str) -> bool:
    rows = build_aligned_rows(target, generated)
    max_len = max(len(target), len(generated))
    matches = sum(1 for r in rows if r["match"])

    print()
    print(f"{BOLD}{CYAN}  {'Pos':>3}  {'Target hex':10} {'Target asm':<38}  {'Gen hex':10} {'Generated asm':<38}  {'Match'}{RESET}")
    print(f"  {'─' * 3}  {'─' * 10} {'─' * 38}  {'─' * 10} {'─' * 38}  {'─' * 5}")

    for r in rows:
        t_idx_str = str(r["t_idx"]) if r["t_idx"] is not None else "---"
        t_hex = r["t_hex"]
        t_asm = r["t_asm"]
        g_hex = r["g_hex"]
        g_asm = r["g_asm"]
        g_reloc = r["g_reloc"]
        g_src = r["g_src"]
        is_match = r["match"]
        
        # Print interlaced source code comments
        for src_line in g_src:
            print(f"{CYAN}{' ' * 69}{src_line.strip()}{RESET}")

        if is_match:
            marker = f"{GREEN}  {'≈' if g_reloc else '✓'}{RESET}"
            line_color = DIM
            formatted_g_asm = format_mnemonic(g_asm)
            t_asm_colored = format_mnemonic(t_asm)
        else:
            marker = f"{RED}  ✗{RESET}"
            line_color = RED
            formatted_g_asm = highlight_asm_diff(t_asm, g_asm)
            t_asm_colored = format_mnemonic(t_asm)

        reloc_tag = f" {DIM}(reloc){RESET}" if g_reloc else ""
        print(
            f"  {line_color}{t_idx_str:>3}{RESET}  "
            f"{line_color}{t_hex:<10}{RESET} {line_color}{t_asm_colored}{RESET}  "
            f"{line_color}{g_hex:<10}{RESET} {line_color}{formatted_g_asm}{RESET}"
            f"{marker}{reloc_tag}"
        )

    print()
    pct = (matches / max_len * 100) if max_len > 0 else 0
    if matches == max_len:
        color, status = GREEN, "PERFECT MATCH"
    elif pct >= 90:
        color, status = YELLOW, "CLOSE"
    else:
        color, status = RED, "MISMATCH"

    print(f"  {BOLD}{color}{func_name}: {matches}/{max_len} instructions match ({pct:.1f}%) — {status}{RESET}")
    print()
    return matches == max_len


def main():
    args = sys.argv[1:]
    do_build = False
    do_analyze = False

    if "--build" in args:
        do_build = True
        args.remove("--build")
    if "--analyze" in args:
        do_analyze = True
        args.remove("--analyze")

    if not args:
        print("Usage: asmdiff.py [--build] [--analyze] <function_name> [source_file.c]", file=sys.stderr)
        sys.exit(1)

    func_name = args[0]
    source_file = args[1] if len(args) > 1 else None

    target_path, dump_path = resolve_paths(func_name, source_file)

    if do_build:
        build_obj(dump_path)

    if not dump_path.exists():
        obj_target = dump_path_to_obj_target(dump_path)
        print(f"Error: Dump file not found: {dump_path}", file=sys.stderr)
        print(f"  Run: make {obj_target}", file=sys.stderr)
        print(f"  Or:  asmdiff.py --build {func_name}", file=sys.stderr)
        sys.exit(1)

    target = parse_target_asm(target_path)
    generated = parse_dump_func(dump_path, func_name)

    if not target:
        print(f"Error: No instructions parsed from {target_path}", file=sys.stderr)
        sys.exit(1)

    if not generated:
        print(f"Error: Function '{func_name}' not found in {dump_path}", file=sys.stderr)
        sys.exit(1)

    is_match = print_diff(target, generated, func_name)
    if do_analyze:
        print_analysis(target, generated, func_name)
    sys.exit(0 if is_match else 1)


if __name__ == "__main__":
    main()
