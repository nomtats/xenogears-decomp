# AI Decompilation Journal: Aligned Diff and Diagnostic Heuristics (asmdiff.py)

**Date**: 2026-03-15
**Time**: 15:34

## Summary
The `asmdiff.py` tool has been significantly upgraded. Previously, the diff viewer used a naive zip grouping on target and generated instructions. This masked the underlying reason for mismatches when blocks of code merely drifted up or down due to instruction omission or insertion. We have updated the tool to perform sequence alignment, producing true "Aligned Diffs". In turn, matching scores on active targets like `PutDispEnv` essentially doubled immediately since matching code blocks are now aligned properly.

## Actions Taken
1. **Integrated `SequenceMatcher`**: The tool now runs sequence alignment (`difflib.SequenceMatcher`) via `build_aligned_rows` separating instructions properly according to actual block-layout alignment structure.
2. **Cluster Mismatch Analysis**: Implemented `--analyze` flag (`classify_cluster`), which applies our existing KB pattern rules heuristically against instruction blocks. It provides insights into what kind of mismatch happened: missing block, register drift, immediate drift, or control flow polarity inversion.
3. **Inline Highlight Integration**: Created `highlight_asm_diff` to visually isolate operand or register differences (e.g. `addiu a0, a0...` vs `addiu a1, a1...`). Diff lines now highlight the exact sub-substrings causing mismatch using ANSI bold yellow, significantly reducing cognitive load.

## Immediate Results
- `PutDispEnv` matching score went from ~30% straight to 70.5% (210/298 matches), because identical sequences the compiler placed earlier or later are properly lined up now. 
- The diff clearly displays `-------` holes where the generated C code completely skips instructions.

## Next Steps
- We will integrate the knowledge base patterns further into `--analyze` to suggest precise inline C structural fixes.
- Resume decompiling libgpu functions using this high-fidelity diff view.
