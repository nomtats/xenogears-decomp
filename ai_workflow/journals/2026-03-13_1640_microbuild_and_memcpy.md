# Journal Entry: 2026-03-13 16:40 - Micro-Build Workflow Strategy and Memcpy Optimization

## Objective
The goal was twofold: to pioneer a faster, "Micro-Build" strategy that dramatically speeds up decompilation turnaround loops, and to test this workflow by matching the `memcpy` routine inside `src/slus_006.64/psyq/libc.c`.

## Actions Taken
1.  **Workflow Investigation (Micro-Build Strategy)**:
    - Analyzed the Makefile structure and discovered we could bypass full `make build` and linker overhead entirely by targeting specific object files, for example: `docker exec -t xenogears_decomp_env make build/src/slus_006.64/psyq/libc.c.o`.
    - This approach reduces compilation feedback from seconds down to negligible milliseconds and inherently avoids cross-project linker errors that stall isolated function decompilation.

2.  **Evaluating `memcpy` with Objective Diffing**:
    - Translated the logic for `memcpy` manually by following the disassembled instructions byte-by-byte.
    - Used the new Micro-Build setup to dump `.o` instructions directly to the `build/` cache, allowing host-machine `objdiff` GUI instances to auto-update line mismatch outputs silently without global project validation failure.

3.  **Branch Delay Slot Null-Check Mismatch**:
    - My initial C code (`u_char* start = pDst;`) caused `v1` to load before checking whether the input pointer `pDst` was `NULL` (the `beqz` instruction). 
    - The original compiler instead postponed storing `pDst` to `start` until *after* verifying `pDst` was not `NULL`, and ingeniously slid the assignment into the branch delay slot of the subsequent size check (`blez a2`).

4.  **The Delayed-Assignment Trick**:
    - By decoupling variable initialization and re-sequencing the `start = pDst;` operation to immediately precede `if (size > 0)` and follow `pDst == NULL`, the ancient GCC compiler automatically tucked the assignment into the preceding branch delay slot natively.
    - This resulted in a **100% byte-for-byte MIPS assembly match**.
    - Replaced the `INCLUDE_ASM` stub in `libc.c` and enforced mandatory Doxygen header comments.

## Meta-Learnings & DKB Updates
- The Micro-Build workflow utilizing raw objects heavily optimizes rapid-iteration loops and has been proven empirically sound.
- A new heuristic was added to `knowledge_base.json` (`pattern_example_012`) detailing how to strategically separate assignment operators to coax GCC into exploiting delay slots correctly when memory/register availability is tight.
- Updated `ai_workflow/journals/TOC.md`.

## Next Logical Steps
We should persist with the current Micro-Build strategy to tackle the next targets within the `libc.c` file since we are already primed in this module. 
- Target 1: `bzero` (16 Lines)
- Target 2: `memset` (16 Lines)

After clearing those, returning to the `generate_call_graph.py` priority list is recommended.
