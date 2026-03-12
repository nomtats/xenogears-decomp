#!/usr/bin/env python3
import os
import glob
import sys
import argparse

"""
find_target.py - Assembly Target Analyzer for AI Decompilation

This script is designed to survey the raw PlayStation MIPS assembly output
split by 'splat' (the `asm/*/nonmatchings/` directories) and rank functions 
by their decompilation suitability.

Metrics used for ranking:
1. Size (Lines of code): Smaller functions (e.g., 4-8 lines) are prioritized
   since they are easier to guess C-code for.
2. Leaf Functions: Assembly routines that do not jump to or branch to other
   sub-routines (e.g., lacking 'jal' instructions). Leaf functions are
   prioritized because they do not require knowing the C signature or return
   types of external functions.

Usage:
  python3 tools/scripts/find_target.py [--limit 20] [--path-filter "system"] [--non-leaf]
"""

import re

def build_caller_counts():
    func_counts = {}
    func_pattern = re.compile(r'\b(func_[A-Fa-f0-9_]+|[A-Za-z0-9_]+)\b')
    
    scan_dirs = ["asm", "src"]
    for base_dir in scan_dirs:
        if not os.path.exists(base_dir):
            continue
        for root, dirs, files in os.walk(base_dir):
            for file in files:
                if file.endswith(".s") or file.endswith(".c"):
                    try:
                        with open(os.path.join(root, file), "r") as f:
                            content = f.read()
                            # We look for strings like "jal func_800..." or "jal SpriteSet" etc
                            # But a simpler heuristic is just counting the name occurrences.
                            matches = func_pattern.findall(content)
                            for match in matches:
                                func_counts[match] = func_counts.get(match, 0) + 1
                    except Exception:
                        pass
    return func_counts

def analyze_asm_files(base_dirs, ref_counts):
    targets = []
    
    for base_dir in base_dirs:
        if not os.path.exists(base_dir):
            continue
            
        for root, dirs, files in os.walk(base_dir):
            for file in files:
                if file.endswith(".s"):
                    filepath = os.path.join(root, file)
                    func_name = file.replace(".s", "")
                    
                    try:
                        with open(filepath, "r") as f:
                            content = f.readlines()
                    except Exception as e:
                        continue
                    
                    # Rough size metric: number of assembly instructions (lines)
                    size = len(content)
                    
                    # Check if it's a leaf function
                    # Leaf functions don't call other functions, so they lack 'jal' or 'jalr'
                    is_leaf = True
                    for line in content:
                        clean_line = line.strip()
                        if clean_line.startswith("jal\t") or clean_line.startswith("jal ") or \
                           clean_line.startswith("jalr\t") or clean_line.startswith("jalr "):
                            is_leaf = False
                            break
                    
                    # Subtract some references like definition glabel, .size, and INCLUDE_ASM itself
                    # Usually around 3 definition occurrences. Bounding to >= 0
                    raw_count = ref_counts.get(func_name, 0)
                    callers = max(0, raw_count - 3)
                    
                    targets.append({
                        "path": filepath,
                        "name": func_name,
                        "size_lines": size,
                        "is_leaf": is_leaf,
                        "callers": callers
                    })
                    
    return targets

def main():
    parser = argparse.ArgumentParser(description="Find good decompilation targets")
    parser.add_argument("--limit", type=int, default=20, help="Number of targets to show")
    parser.add_argument("--path-filter", type=str, default="", help="Filter targets by path substring (e.g., 'system')")
    parser.add_argument("--exclude-filter", type=str, default="psyq", help="Exclude targets by path substring (default: 'psyq')")
    parser.add_argument("--non-leaf", action="store_true", help="Include non-leaf functions in top results")
    parser.add_argument("--sort-callers", action="store_true", help="Sort strictly by caller count (descending) instead of size")
    args = parser.parse_args()

    # We want to look inside the nonmatchings folders for both the main executable and field overlays
    search_dirs = [
        "asm/slus_006.64/nonmatchings",
        "asm/field/nonmatchings"
    ]
    
    print(f"Scanning the codebase for function references (this may take a few seconds)...")
    ref_counts = build_caller_counts()
    
    print(f"Scanning target directories...")
    targets = analyze_asm_files(search_dirs, ref_counts)
    
    if args.path_filter:
        targets = [t for t in targets if args.path_filter in t["path"]]
    if args.exclude_filter:
        targets = [t for t in targets if args.exclude_filter not in t["path"]]
        
        
    print(f"Found {len(targets)} un-matched functions total.\n")
    
    # Sorting Strategy:
    if args.sort_callers:
        primary_sort = lambda x: (-x["callers"], x["size_lines"])
    elif not args.non_leaf:
        # Prioritize leaves, then callers (descending), then size
        primary_sort = lambda x: (not x["is_leaf"], -x["callers"], x["size_lines"])
    else:
        primary_sort = lambda x: (-x["callers"], x["size_lines"])
        
    targets.sort(key=primary_sort)

    print(f"Top {args.limit} suggested targets:")
    print(f"{'Path':<70} | {'Lines':<5} | {'Refs':<4} | {'Type'}")
    print("-" * 94)
    for t in targets[:args.limit]:
        func_type = "Leaf" if t["is_leaf"] else "Non-Leaf"
        print(f"{t['path']:<70} | {t['size_lines']:<5} | {t['callers']:<4} | {func_type}")

if __name__ == "__main__":
    main()
