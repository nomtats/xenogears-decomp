#!/usr/bin/env python3
import os
import glob
import sys
import argparse

def analyze_asm_files(base_dirs):
    targets = []
    
    for base_dir in base_dirs:
        if not os.path.exists(base_dir):
            continue
            
        for root, dirs, files in os.walk(base_dir):
            for file in files:
                if file.endswith(".s"):
                    filepath = os.path.join(root, file)
                    
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
                    
                    targets.append({
                        "path": filepath,
                        "size_lines": size,
                        "is_leaf": is_leaf
                    })
                    
    return targets

def main():
    parser = argparse.ArgumentParser(description="Find good decompilation targets")
    parser.add_argument("--limit", type=int, default=20, help="Number of targets to show")
    parser.add_argument("--path-filter", type=str, default="", help="Filter targets by path substring (e.g., 'system')")
    parser.add_argument("--non-leaf", action="store_true", help="Include non-leaf functions in top results")
    args = parser.parse_args()

    # We want to look inside the nonmatchings folders for both the main executable and field overlays
    search_dirs = [
        "asm/slus_006.64/nonmatchings",
        "asm/field/nonmatchings"
    ]
    
    print(f"Scanning target directories...")
    targets = analyze_asm_files(search_dirs)
    
    if args.path_filter:
        targets = [t for t in targets if args.path_filter in t["path"]]
        
    print(f"Found {len(targets)} un-matched functions total.\n")
    
    # Sorting Strategy:
    # 1. By default, Leaf functions are prioritized because they are self-contained and don't require knowing external function addresses/signatures.
    # 2. Then, sort by size (smallest first). We want simple functions to test the workflow.
    
    if not args.non_leaf:
        # Filter to only show leaf functions, or prioritize them heavily
        primary_sort = lambda x: (not x["is_leaf"], x["size_lines"])
    else:
        primary_sort = lambda x: x["size_lines"]
        
    targets.sort(key=primary_sort)

    print(f"Top {args.limit} suggested targets:")
    print(f"{'Path':<70} | {'Lines':<6} | {'Type'}")
    print("-" * 90)
    for t in targets[:args.limit]:
        func_type = "Leaf" if t["is_leaf"] else "Non-Leaf"
        print(f"{t['path']:<70} | {t['size_lines']:<6} | {func_type}")

if __name__ == "__main__":
    main()
