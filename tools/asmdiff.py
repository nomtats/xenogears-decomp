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

How it works:
    1. Locates the target assembly in asm/<target>/matchings/<lib>/<module>/<Func>.s
    2. Infers the compiled dump at build/src/<target>/<lib>/<module>.c.dump.s
    3. Compares instruction-by-instruction, masking relocation fields (address
       immediates for lui/addiu/lw/sw, jump targets for j/jal) since the
       unlinked object file has zeroed relocation slots.
    4. Prints a color-coded side-by-side diff with a match score.

Via the Makefile (recommended — builds then diffs in one step):
    make asmdiff FUNC=MoveImage

All commands run inside Docker:
    docker exec xenogears_decomp_env make asmdiff FUNC=MoveImage
"""

import re
import subprocess
import sys
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


def parse_dump_func(path: Path, func_name: str) -> list[tuple[str, str, bool]]:
    """Extract [(hex_word, mnemonic, has_reloc), ...] for func_name from objdump."""
    lines = path.read_text().splitlines()
    in_func = False
    result = []
    func_header = re.compile(r'^[0-9a-f]+\s+<' + re.escape(func_name) + r'>:')
    next_func = re.compile(r'^[0-9a-f]+\s+<\w+>:')
    instr_pat = re.compile(r'^\s+[0-9a-f]+:\s+([0-9a-f]{8})\s+(.*)')
    reloc_pat = re.compile(r'^\s+[0-9a-f]+:\s+R_MIPS_')

    for line in lines:
        if not in_func:
            if func_header.match(line):
                in_func = True
            continue
        if next_func.match(line):
            break
        m = instr_pat.match(line)
        if m:
            result.append((m.group(1).lower(), m.group(2).strip(), False))
            continue
        if reloc_pat.match(line) and result:
            last = result[-1]
            result[-1] = (last[0], last[1], True)

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
    if len(s) > width:
        return s[:width - 1] + "…"
    return s.ljust(width)


def print_diff(target, generated, func_name: str) -> bool:
    max_len = max(len(target), len(generated))
    matches = 0

    print()
    print(f"{BOLD}{CYAN}  {'Pos':>3}  {'Target hex':10} {'Target asm':<38}  {'Gen hex':10} {'Generated asm':<38}  {'Match'}{RESET}")
    print(f"  {'─' * 3}  {'─' * 10} {'─' * 38}  {'─' * 10} {'─' * 38}  {'─' * 5}")

    for i in range(max_len):
        pos = i + 1
        t_hex, t_asm = target[i] if i < len(target) else ("--------", "<missing>")
        if i < len(generated):
            g_hex, g_asm, g_reloc = generated[i]
        else:
            g_hex, g_asm, g_reloc = "--------", "<missing>", False

        if g_reloc:
            mask = reloc_mask(g_hex)
            is_match = (int(t_hex, 16) & mask) == (int(g_hex, 16) & mask)
        else:
            is_match = (t_hex == g_hex)

        if is_match:
            matches += 1
            marker = f"{GREEN}  {'≈' if g_reloc else '✓'}{RESET}"
            line_color = DIM
        else:
            marker = f"{RED}  ✗{RESET}"
            line_color = RED

        reloc_tag = f" {DIM}(reloc){RESET}" if g_reloc else ""
        print(
            f"  {line_color}{pos:>3}{RESET}  "
            f"{line_color}{t_hex}{RESET} {line_color}{format_mnemonic(t_asm)}{RESET}  "
            f"{line_color}{g_hex}{RESET} {line_color}{format_mnemonic(g_asm)}{RESET}"
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

    if "--build" in args:
        do_build = True
        args.remove("--build")

    if not args:
        print("Usage: asmdiff.py [--build] <function_name> [source_file.c]", file=sys.stderr)
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
    sys.exit(0 if is_match else 1)


if __name__ == "__main__":
    main()
