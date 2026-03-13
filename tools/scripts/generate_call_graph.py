#!/usr/bin/env python3
import os
import glob
import re
import json
from collections import defaultdict

def main():
    dump_files = glob.glob('build/**/*.dump.s', recursive=True)
    if not dump_files:
        print("No .dump.s files found. Please ensure 'make report' has run to compile object files.")
        return

    # Call graph: caller -> set of callees
    call_graph = defaultdict(set)
    # Reverse call graph: callee -> set of callers
    reverse_graph = defaultdict(set)

    # Identifiers
    func_def_re = re.compile(r'^([0-9a-fA-F]+)\s+<([^>]+)>:')
    jal_re = re.compile(r'\bjal(?:r)?\s+[0-9a-fA-F]+\s+<([^>]+)>')
    jal_no_target_re = re.compile(r'\bjal(?:r)?\s+([A-Za-z0-9_]+)')
    reloc_re = re.compile(r'R_MIPS_26\s+(\S+)')

    # Sections to ignore (objdump artifacts)
    ignore_sections = {'.reginfo', '.MIPS.abiflags', '.gnu.attributes', '.pdr', '.data', '.bss'}

    print(f"Analyzing {len(dump_files)} object dump files...")

    for fpath in dump_files:
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

    # Resolve matched vs unmatched status
    unmatched_funcs = set()
    for root, _, files in os.walk("asm"):
        if "nonmatchings" in root:
            for file in files:
                if file.endswith(".s"):
                    unmatched_funcs.add(file[:-2])

    # Convert sets to lists for JSON
    json_graph = {
        "call_graph": {k: sorted(list(v)) for k, v in call_graph.items()},
        "reverse_graph": {k: sorted(list(v)) for k, v in reverse_graph.items()},
        "unmatched_functions": sorted(list(unmatched_funcs))
    }

    os.makedirs('ai_workflow/analysis', exist_ok=True)
    with open('ai_workflow/analysis/call_graph.json', 'w') as f:
        json.dump(json_graph, f, indent=2)

    # Compute targets for decompilation
    targets = []
    for func in unmatched_funcs:
        outbound = len(call_graph.get(func, []))
        inbound = len(reverse_graph.get(func, []))
        is_leaf = outbound == 0
        targets.append({
            "name": func,
            "callers": inbound,
            "is_leaf": is_leaf
        })

    # Sort primarily by `is_leaf` (True first), then by `callers` descending
    targets.sort(key=lambda x: (not x["is_leaf"], -x["callers"]))

    # Generate Markdown Report
    report = ["# Xenogears Call Graph Analysis", "", "## Top 20 Candidates for Decompilation", "These are unmatched functions, prioritized strictly by 'Leaf' status and then 'Incoming References'."]
    report.append("| Function | Callers | Is Leaf |")
    report.append("| --- | --- | --- |")
    for t in targets[:20]:
        report.append(f"| `{t['name']}` | {t['callers']} | {t['is_leaf']} |")

    report.append("")
    report.append("## Most Highly Referenced Functions Overall (Matched & Unmatched)")
    report.append("Functions that are called the most throughout the codebase.")
    report.append("| Function | Callers | Matched? |")
    report.append("| --- | --- | --- |")
    
    overall = []
    # Avoid counting dead ends as highly referenced if caller == 0
    for func, callers in reverse_graph.items():
        overall.append((func, len(callers), func not in unmatched_funcs))
            
    overall.sort(key=lambda x: -x[1])
    for f, c, m in overall[:20]:
         status = "✅" if m else "❌"
         report.append(f"| `{f}` | {c} | {status} |")

    with open('ai_workflow/analysis/call_graph_report.md', 'w') as f:
        f.write("\n".join(report))

    print("Analysis complete! (With Relocation Parsing Support)")
    print(f"- Wrote graph JSON to: ai_workflow/analysis/call_graph.json")
    print(f"- Wrote markdown report to: ai_workflow/analysis/call_graph_report.md")

if __name__ == "__main__":
    main()
