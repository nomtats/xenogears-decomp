# Static Call Graph Analysis

## Context
When decompiling a large binary like Xenogears, picking the right target is crucial. Attempting to decompile a "parent" function without knowing the signatures of the "leaf" functions it calls leads to massive cascading register allocation errors. Previously, we used a rudimentary `tools/scripts/find_target.py` regex search to identify targets, but it often misidentified "True Leaves" because it only looked at un-matched assembly files, ignoring the already matched codebase.

## Action
We deleted `find_target.py` and built a comprehensive `tools/scripts/generate_call_graph.py` pipeline.

1. **R_MIPS_26 Resolving:** Instead of parsing raw `.s` files, the new script parses the disassembled `.dump.s` artifacts generated during `make report`. This leverages the actual `R_MIPS_26` relocation tags from `objdump`, guaranteeing 100% precision on what a `jal` call correlates to.
2. **Module Clustering:** The script maps functions back to logical `src/` modules using a custom heuristic. This aggregates cohesive libraries (like `psyq/libspu`) into single modules, while atomizing massive catch-all folders (like `field/main/misc.c`) into file-specific modules for better granularity.
3. **Line Count Metrics:** The script scans the original `.s` matching sizes to sort ties by size (lines of ASM), prioritizing smaller functions.
4. **Git Tracking:** The massive `call_graph.json` artifact was added to `.gitignore`, but the resulting markdown `call_graph_report.md` is tracked in git.

## Result
We now have a dedicated command (`make analyze_graph`) that takes <1 second to run. It correctly flagged `SoundWaitSpuTransfer` as highly referenced but successfully decompiled, while exposing functions like `func_8003E5BC` as critical 6-caller True Leaves in the sound engine. The overall progress of the decompilation is tracked accurately per subsystem.
