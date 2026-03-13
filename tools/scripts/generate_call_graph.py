#!/usr/bin/env python3
import os
import glob
import re
import json
from collections import defaultdict
def get_module_name(path):
    """Groups related C files into cohesive subsystems/modules."""
    squash_dirs = [
        "psyq/libspu", "psyq/libcd", "psyq/libapi", 
        "psyq/libc2", "psyq/libetc", "field/effects", 
        "field/scripts", "slus_006.64/main"
    ]
    for d in squash_dirs:
        idx = path.find(f"/{d}")
        if idx != -1:
            if path.startswith(f"/{d}/", idx) or path[idx:] == f"/{d}.c":
                return path[:idx + len(f"/{d}")]
    return path.replace(".c", "")

def main():
    dump_files = glob.glob('build/**/*.dump.s', recursive=True)
    if not dump_files:
        print("No .dump.s files found. Please ensure 'make report' has run to compile object files.")
        return

    # Call graph: caller -> set of callees
    call_graph = defaultdict(set)
    # Reverse call graph: callee -> set of callers
    reverse_graph = defaultdict(set)
    
    # Store mapping: func_name -> subsystem_path
    func_locations = {}

    # Identifiers
    func_def_re = re.compile(r'^([0-9a-fA-F]+)\s+<([^>]+)>:')
    jal_re = re.compile(r'\bjal(?:r)?\s+[0-9a-fA-F]+\s+<([^>]+)>')
    jal_no_target_re = re.compile(r'\bjal(?:r)?\s+([A-Za-z0-9_]+)')
    reloc_re = re.compile(r'R_MIPS_26\s+(\S+)')

    # Sections to ignore (objdump artifacts)
    ignore_sections = {'.reginfo', '.MIPS.abiflags', '.gnu.attributes', '.pdr', '.data', '.bss'}

    print(f"Analyzing {len(dump_files)} object dump files...")

    # First, track matched functions and their subsystem files from C files
    # We map func_name -> "src/subsystem/file.c"
    src_funcs = set()
    for fpath in glob.glob('src/**/*.c', recursive=True):
        try:
            with open(fpath, 'r', errors='ignore') as f:
                content = f.read()
                # Very rough heuristic to find function implementations in C
                # A proper regex or ctags would be better, but we mostly just need
                # it for reporting. The linker output (dump files) is safer.
        except Exception:
            pass

    # Actually, we can just assign paths based on the .dump.s file path!
    for fpath in dump_files:
        # e.g. build/src/slus_006.64/system/sound.c.dump.s
        rel_path = fpath.replace('build/', '').replace('.dump.s', '')
        
        with open(fpath, 'r', errors='ignore') as f:
            current_func = None
            pending_jal_target = None
            
            for line in f:
                # Detect function boundary
                m_def = func_def_re.match(line)
                if m_def:
                    func_name = m_def.group(2)
                    if func_name in ignore_sections or func_name.startswith('.'):
                        current_func = None
                        continue
                    if '+' in func_name:
                         func_name = func_name.split('+')[0]
                    current_func = func_name
                    pending_jal_target = None
                    
                    # Store where this function lives
                    func_locations[func_name] = rel_path
                    continue
                
                if current_func:
                    # Check for pending jal relocation
                    if pending_jal_target is not None:
                        m_rel = reloc_re.search(line)
                        if m_rel:
                            # Use relocation symbol instead of local section offset
                            target = m_rel.group(1)
                            call_graph[current_func].add(target)
                            reverse_graph[target].add(current_func)
                            pending_jal_target = None
                            continue
                        else:
                            # It was a local call, commit the pending target if valid
                            target = pending_jal_target
                            call_graph[current_func].add(target)
                            reverse_graph[target].add(current_func)
                            pending_jal_target = None

                    # Look for jal
                    if '\tjal' in line or ' jal' in line:
                        m_jal = jal_re.search(line)
                        if m_jal:
                            target = m_jal.group(1)
                            if '+' in target:
                                target = target.split('+')[0]
                            pending_jal_target = target
                        else:
                            m_alt = jal_no_target_re.search(line)
                            if m_alt:
                                target = m_alt.group(1)
                                if '+' in target:
                                    target = target.split('+')[0]
                                pending_jal_target = target

            # End of file, commit any dangling pending target
            if pending_jal_target is not None and current_func:
                call_graph[current_func].add(pending_jal_target)
                reverse_graph[pending_jal_target].add(current_func)

    # Clean up graph (remove self-loops or bad symbols parsed as callers)
    for k in list(call_graph.keys()):
        call_graph[k] = {v for v in call_graph[k] if v != k and not v.startswith('.')}
    for k in list(reverse_graph.keys()):
        reverse_graph[k] = {v for v in reverse_graph[k] if v != k and not v.startswith('.')}

    # Resolve matched vs unmatched status by reading the asm directory
    # If a function has a .s file in nonmatchings, it is unmatched.
    unmatched_funcs = set()
    unmatched_sizes = {}
    for root, _, files in os.walk("asm"):
        if "nonmatchings" in root:
            # Reconstruct the file path for subsystem grouping
            # e.g. asm/slus_006.64/nonmatchings/system/sound/...
            # We want to map it back to: src/slus_006.64/system/sound.c
            base_rel = root.split("nonmatchings/")[-1] 
            header_prefix = root.split("/nonmatchings")[0].replace("asm/", "src/")
            
            # The destination C file is usually the folder name.
            c_file_target = f"{header_prefix}/{base_rel}.c"
            
            for file in files:
                if file.endswith(".s"):
                    func = file[:-2]
                    unmatched_funcs.add(func)
                    
                    filepath = os.path.join(root, file)
                    try:
                        with open(filepath, 'r', errors='ignore') as f:
                            unmatched_sizes[func] = sum(1 for _ in f)
                    except Exception:
                        unmatched_sizes[func] = 0

                    # If it wasn't captured in the dump file loop gracefully, hardcode it here
                    if func not in func_locations:
                        func_locations[func] = c_file_target

    # Determine all functions (matched + unmatched)
    all_funcs = set(func_locations.keys())
    matched_funcs = all_funcs - unmatched_funcs

    # Build Global Stats
    total_funcs = len(all_funcs)
    total_matched = len(matched_funcs)
    global_pct = (total_matched / total_funcs * 100) if total_funcs > 0 else 0

    # Build Module Stats
    module_stats = defaultdict(lambda: {"matched": 0, "unmatched": 0, "total": 0})
    for func, path in func_locations.items():
        module = get_module_name(path)
        module_stats[module]["total"] += 1
        if func in matched_funcs:
            module_stats[module]["matched"] += 1
        else:
            module_stats[module]["unmatched"] += 1

    global_targets = []
    # Group unmatched targets by module
    grouped_targets = defaultdict(list)
    for func in unmatched_funcs:
        outbound = len(call_graph.get(func, []))
        inbound = len(reverse_graph.get(func, []))
        is_leaf = outbound == 0
        path = func_locations.get(func, "Unknown Module")
        module = get_module_name(path)
        size = unmatched_sizes.get(func, 0)
        
        target_info = {
            "name": func,
            "callers": inbound,
            "callees": outbound,
            "is_leaf": is_leaf,
            "size": size,
            "module": module,
            "file": path
        }
        grouped_targets[module].append(target_info)
        global_targets.append(target_info)

    # Generate Markdown Report
    report = ["# Xenogears Decompilation & Call Graph Analysis\n"]
    
    report.append("## 📈 Global Progress")
    report.append(f"- **Total Functions:** {total_funcs}")
    report.append(f"- **Decompiled (Matched):** {total_matched}")
    report.append(f"- **Remaining (Unmatched):** {total_funcs - total_matched}")
    report.append(f"- **Overall Completion:** {global_pct:.2f}%")
    report.append("")
    
    report.append("## 📁 Module Progress")
    report.append("| Module | Matched | Unmatched | Total | Progress |")
    report.append("| :--- | :--- | :--- | :--- | :--- |")
    
    # Sort modules by path alphabetically
    sorted_modules = sorted(module_stats.keys())
    for path in sorted_modules:
        stats = module_stats[path]
        m = stats["matched"]
        u = stats["unmatched"]
        t = stats["total"]
        pct = (m / t * 100) if t > 0 else 0
        report.append(f"| `{path}` | {m} | {u} | {t} | {pct:.1f}% |")
    report.append("")

    report.append("## 🏆 Top 20 Candidates for Decompilation (Global)")
    report.append("The best un-matched targets across the entire codebase, prioritizing 'True Leaf' status, then caller count, then lowest ASM lines.")
    report.append("| Function | Module | Callers | Callees | ASM Lines |")
    report.append("| :--- | :--- | :--- | :--- | :--- |")
    global_targets.sort(key=lambda x: (not x["is_leaf"], -x["callers"], x["size"]))
    for t in global_targets[:20]:
         report.append(f"| `{t['name']}` | `{t['module']}` | {t['callers']} | {t['callees']} | {t['size']} |")
    report.append("")

    report.append("## 🎯 Top Targets by Module")
    report.append("Unmatched functions prioritized by 'True Leaf' status, then by mostly heavily referenced.\n")
    
    # Sort and print targets per module
    for sub in sorted(list(grouped_targets.keys())):
        targets = grouped_targets[sub]
        # Sort primarily by `is_leaf` (True first), then by `callers` descending, then `size` ascending
        targets.sort(key=lambda x: (not x["is_leaf"], -x["callers"], x["size"]))
        
        report.append(f"### `{sub}`")
        report.append("| Function | File | Callers | Callees | ASM Lines |")
        report.append("| :--- | :--- | :--- | :--- | :--- |")
        # Only show top 10 per module
        for t in targets[:10]:
             filename = os.path.basename(t['file'])
             report.append(f"| `{t['name']}` | `{filename}` | {t['callers']} | {t['callees']} | {t['size']} |")
        report.append("")

    report.append("## 👑 Most Highly Referenced Functions Overall")
    report.append("Functions called by the most other functions throughout the entire codebase.")
    report.append("| Function | Callers | Module | Matched? |")
    report.append("| :--- | :--- | :--- | :--- |")
    
    overall = []
    for func, callers in reverse_graph.items():
        if func not in ignore_sections and not func.startswith('.'):
            callees = len(call_graph.get(func, []))
            val = (func, len(callers), callees, func not in unmatched_funcs, func_locations.get(func, "Unknown"))
            overall.append(val)
            
    overall.sort(key=lambda x: -x[1])
    for f, c, out, m, p in overall[:20]:
         status = "✅" if m else "❌"
         report.append(f"| `{f}` | {c} | `{p}` | {status} |")

    with open('ai_workflow/analysis/call_graph_report.md', 'w') as f:
        f.write("\n".join(report))

    # Convert sets to lists for JSON
    json_graph = {
        "call_graph": {k: sorted(list(v)) for k, v in call_graph.items()},
        "reverse_graph": {k: sorted(list(v)) for k, v in reverse_graph.items()},
        "unmatched_functions": sorted(list(unmatched_funcs)),
        "func_locations": func_locations
    }
    with open('ai_workflow/analysis/call_graph.json', 'w') as f:
        json.dump(json_graph, f, indent=2)


    print("Analysis complete!")
    print(f"- Wrote graph JSON to: ai_workflow/analysis/call_graph.json")
    print(f"- Wrote markdown report to: ai_workflow/analysis/call_graph_report.md")

if __name__ == "__main__":
    main()
