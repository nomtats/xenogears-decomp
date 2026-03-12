# Journal Entry: 2026-03-12 23:48 - Caller Frequency Scripting and GCC Register Casting

## Objective
To update `find_target.py` to identify the most highly-referenced unmatched functions in the repository, and to solve an unexpected compilation discrepancy via manual compiler register manipulation. 

## Actions Taken
1.  **Script Enhancement**: I rewrote the analysis engine in `tools/scripts/find_target.py`. It now scans the entire `.s` and `.c` tree leveraging Python regex to trace how many times an un-matched target function is referenced over the global codebase. I bounded the metric to a new `--sort-callers` argument.
2.  **Target Selection**: The script flagged `FieldScriptMemoryWriteU16` in `src/field/scripts/virtual_machine.c` as the absolute highest-priority target with an immense **362 references**. It was a tiny 9-line leaf offset calculation.
3.  **Compilation Discrepancies**: While the raw C-code pointer logic was sound, the `objdiff` hash repeatedly failed. The PS1 GCC compiler insisted on reusing the input `$a0` register for internal shift mathematics instead of cleanly separating the operation into `$a2` and `$v0` as authored on the disc. 
4.  **Register Override**: To achieve 1-to-1 instruction matching with the arbitrary assembly, I forced the compiler's hand using GCC's Local Register Variable extensions: 
    ```c
    void FieldScriptMemoryWriteU16(int index, int value) {
        register int a2 asm("a2") = index >> 1;
        register int v0 asm("v0") = a2 << 1;
        *(short*)((char*)&g_FieldScriptMemory + v0) = value;
    }
    ```
5.  **Validation**: A fresh incremental `time make build` and subsequent `make check` proved **perfect 100% hash parity**, matching `FIELD.BIN: OK`.
6.  `make report` was executed, advancing the system's matched count to **677 functions**.

## Meta-Learnings & DKB Updates
- The `objdiff` hashes are fiercely rigid concerning mathematical register allocation. If the underlying logic is correct but the registers are slightly off, **GCC variable assignment `asm("reg")`** is a verified, powerful workaround to "lock in" byte-perfect PS1 code structure.
- The `ai_workflow/knowledge_base.json` has been updated with `pattern_example_003` outlining this explicit casting heuristic.

## Next Logical Steps
With both simplistic pointer arithmetic and complex compiler-level register overrides manually executed natively and logged, all prerequisites in Phase 3 are completely exhausted. 
We stand ready to compose Phase 4 (`scripts/auto_decomp.py`) where an AI worker can utilize this exact workflow seamlessly.
