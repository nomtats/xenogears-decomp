# memset Match & BIOS Tail-Call Anti-Pattern Discovery

**Date:** 2026-03-14
**Session Focus:** Continue Jigsaw Strategy on libc leaf functions; assess PsyQ BIOS wrappers

## What Happened

### memset Matched (First Attempt)
- Recognized that `memset`'s assembly is structurally identical to `bzero` — same flat goto pattern with delay slot masking, just storing `$a1` (value) instead of zero.
- Applied the proven template from `bzero` directly. Compiled with micro-build. Instruction-for-instruction match confirmed on first attempt.
- **Key insight:** Once a pattern is proven on one function, scanning the call graph report for structurally similar functions and applying the same template yields instant matches. Pattern reuse is the highest-leverage activity.

### libc Progress
- libc is now **8/12 matched (66.7%)**, up from 58.3%.
- Remaining 4 unmatched: `func_8003FA78` (49 lines), `func_8003FB20` (33 lines), `func_8003FB84` (22 lines), `Sprintf` (604 lines).
- `Sprintf` is by far the largest remaining function in libc and may require significant effort.

### BIOS Tail-Call Wrappers: Permanently Unmatchable
Assessed all 6-line functions across libapi, libapi_2, libapi_3, and libcard. Discovered that most are 3-instruction PS1 BIOS dispatch trampolines:
```
addiu $t2, $zero, 0xB0    # BIOS B-table entry vector
jr $t2                     # tail-call to BIOS
addiu $t1, $zero, <num>   # function number (in delay slot)
```

These are **fundamentally unmatchable from C** because:
1. They are tail calls — `jr $t2` diverts to the BIOS, which returns directly to the original caller.
2. GCC always emits `jr $ra; nop` epilogue, making any C function at least 5 instructions (20 bytes) instead of the required 3 (12 bytes).
3. GCC 2.7.2 has no `__attribute__((naked))` for MIPS to suppress the epilogue.
4. These were hand-written assembly in the original PsyQ SDK, never compiled from C.

Similarly, `setjmp`/`longjmp` directly manipulate `$ra` and all callee-saved registers — also fundamentally not expressible in C.

**Affected functions (17 total):**
- libapi: StartPAD2, InitPAD2, StopPAD2, SysEnqIntRP, SysDeqIntRP, func_80040ABC
- libapi_2: HookEntryInt, ResetEntryInt, ReturnFromException, func_8004BED8, setjmp, longjmp
- libapi_3: ChangeClearRCnt
- libcard: StartCARD2, InitCARD2, StopCARD2, _card_info

**Decision:** These must remain as `INCLUDE_ASM` permanently. Added as anti-pattern entry (antipattern_001) to the KB.

### Matchable Small Functions Identified
Two trivial getter/setter functions in libapi ARE matchable:
- `func_8004076C`: stores `$a0` to global `D_80056414` (4 instructions, has normal epilogue)
- `func_8004077C`: loads and returns global `D_80056414` (4 instructions, has normal epilogue)
- `func_8004BEC0`: empty function — just `jr $ra; nop`

These are low-priority but easy wins for a future session.

## What Worked
- **Pattern reuse from bzero → memset** was instant. The KB template eliminated all trial-and-error.
- **Systematic assessment of BIOS wrappers** before attempting them saved significant time. Recognizing the 3-instruction tail-call signature early let us skip 17 functions that would have been dead ends.

## What to Do Next
1. **libgpu functions** — `DrawSync` (13 callers, 30 lines), `LoadImage` (8 callers, 27 lines), `StoreImage` (3 callers, 27 lines), `DrawOTag` (4 callers, 32 lines). These all use a GPU vtable dispatch through `D_800568C8` and have normal function prologues/epilogues. They should be matchable.
2. **Remaining libc functions** — `func_8003FA78` (49 lines), `func_8003FB20` (33 lines), `func_8003FB84` (22 lines) are medium-priority.
3. **libapi getter/setter pair** — `func_8004076C`/`func_8004077C` are trivial picks.

## Patterns Applied
- `pattern_example_013` (Flat Goto + Delay Slot Masking) — reused directly from bzero
- New: `antipattern_001` (BIOS Tail-Call Wrappers)
- New: `pattern_example_014` (Flat Goto Pattern Reuse Across libc)
