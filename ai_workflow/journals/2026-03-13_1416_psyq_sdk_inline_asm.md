# Journal Entry: 2026-03-13 - PsyQ SDK Functions and Inline Assembly

## Objective
Following the "Jigsaw Strategy," we targeted the highly referenced but extremely small ("True Leaf") SDK functions: `SetTransMatrix`, `SetRotMatrix` from `src/slus_006.64/psyq/libgte.c`, and `PCclose` from `src/slus_006.64/psyq/libsn.c`. These three functions had 63 aggregate callers but 0 callees, making them the perfect isolated edges to solve.

## Learnings & Observations
1. **Psycho SN Systems Break Instructions**:
   - `PCclose` was implemented using the MIPS `break` instruction. Examining the original assembly (`break 0, 260`), `maspsx.py` threw an error parsing the instruction argument. 
   - I learned that in the `break V` pseudo-instruction syntax for MIPS gas, `break 260` correctly shifts the constant to the upper 20-bit field `0x00041`, dropping into the final assembled word as `0x0000410D` (little endian `0D 41 00 00`), matching the original game exactly.

2. **Coprocessor 2 (GTE) and GNU C Extensions**:
   - `SetRotMatrix` and `SetTransMatrix` were purely handwritten routines to pump registers sequentially into GTE control registers `ctc2 $tN, $R`.
   - Instead of trying to coerce GCC, the cleanest and most accurate way to match was by injecting the routine using `__asm__ volatile`, binding the arguments correctly, and mapping the input pointer `%0` to the GCC clobber list.
   - For `maspsx.py` stability, I had to ensure that memory offset values for `lw` inside the inline asm were decimal (e.g. `lw $t1, 4(%0)`) instead of hex `0x4(%0)`.

## Next Steps
1. Having resolved `SetRotMatrix`, `SetTransMatrix`, and `PCclose`, we've effectively decompiled the puzzle pieces connecting to 63 other functions!
2. Continue parsing down the list in `ai_workflow/analysis/call_graph_report.md` for the next heavily referenced functions (like `DrawSync`, `LoadImage`, etc.).
