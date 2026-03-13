# AI Decompilation Journal: memchr Decompilation

**Date:** 2026-03-13
**Time:** 22:20
**Target:** `memchr` (`src/slus_006.64/psyq/libc.c`)

## Context & Objective
Following the success of decompiling `bzero` and `memcpy` in the PsyQ `libc.c` SDK module by explicitly mapping delay slots using the micro-build block strategy, we moved onto another highly-referenced leaf utility: `memchr`.

The objective was to perfectly match the original `memchr` assembly, which exhibited a highly particular initial block checking state followed by jumping directly into a specific sequence within the looping logic:
```mips
  .L8003F938:
    bltz       $a2, .L8003F960
    addu      $v0, $zero, $zero
    andi       $a1, $a1, 0xFF
  .L8003F944:
    lbu        $v0, 0x0($a0)
    nop
    beq        $v0, $a1, .L8003F930
    addiu     $a0, $a0, 0x1
    addiu      $a2, $a2, -0x1
    bgez       $a2, .L8003F944
```

## Problem

A standard `while (size-- > 0)` or `do-while` implementation inside GCC 2.7.2 hoisted the `size--` variable allocation improperly around the loop boundaries. It evaluated `size` cleanly and ignored explicitly seeding the branch delay slot before returning to the loop check iteration block.

When evaluating:
```c
while (size-- > 0) {
    if (*pDst++ == value) return pDst - 1;
}
```
GCC's optimizer failed to generate the specific state machine the PsyQ authors originally wrote where the `size--` mathematical operation was hard-assigned immediately following the label entry jump before evaluating `size < 0`.

## Solution

To achieve a 100% byte-match, we constructed a flattened pseudo-unrolled loop using `goto` statements that perfectly mirrors the source assembly logic. This tricks GCC's optimizer into laying out the basic blocks natively without interceding with loop optimization patterns.

```c
void* memchr(u_char* pDst, int value, int size) {
    if (pDst == NULL) return NULL;
    if (size <= 0) return NULL;
    
    goto loop_check;

match:
    return pDst - 1;

loop_check:
    size--;
    if (size < 0) return NULL;
    value &= 0xFF;

loop:
    if (*pDst++ == value) goto match;
    size--;
    if (size >= 0) goto loop;

    return NULL;
}
```

## Outcomes

- `memchr` in `libc.c` is fully completed and documented.
- The compiled `.o` object is identically byte-matched.
- The use case of flat `goto` state-machine blocks has been definitively proven as the required methodology for matching deep PS1 SDK array manipulation functions that utilize manual assembly looping mechanisms under the hood.

## Post-Match Analysis: Goto Reduction Experiments

After achieving the byte-match, we investigated whether the 3 `goto` statements could be reduced to produce more idiomatic C while preserving the exact binary output.

### Attempt 1: Clean `do-while` (0 gotos)
```c
size--;
if (size < 0) return NULL;
value &= 0xFF;
do {
    if (*pDst++ == value) return pDst - 1;
    size--;
} while (size >= 0);
return NULL;
```
**Result:** The core loop body (lbu → beq → addiu → addiu → bgez) was **instruction-identical** to the target. However, two differences in the prologue:
1. GCC hoisted `size--` into the `blez` delay slot (target has `nop` there, with `size--` in a separate `j` instruction's delay slot).
2. GCC placed the match-return block at the bottom with an extra `j return; nop` (2 extra instructions), while the target places it between the prologue and loop check.

### Attempt 2: Hybrid `goto` + `for(;;)` (1 goto)
```c
goto loop_check;
for (;;) {
    if (*pDst++ == value) return pDst - 1;
loop_check:
    size--;
    if (size < 0) return NULL;
    value &= 0xFF;
}
```
**Result:** GCC aggressively restructured the control flow, inverting branch polarities (`bne` instead of `beq`, `bgtz` instead of `blez`) and moving `andi` into the `bgez` delay slot. Completely different output.

### Conclusion

Each of the 3 gotos controls a distinct aspect of GCC 2.7.2's code generation:

| Goto | Purpose | What GCC does without it |
|------|---------|--------------------------|
| `goto loop_check` | Forces "jump into middle of loop" with `size--` in `j`'s delay slot | Hoists `size--` into `blez` delay slot instead |
| `goto match` | Forces `beq` (True Branch) polarity and places the match block between prologue and loop | Inverts to `bne` (False Branch) and adds extra `j; nop` to skip match block at bottom |
| `goto loop` | Forces `bgez` back-edge without loop restructuring | May combine exit conditions or invert loop test |

**Key takeaway for future agents:** Always try `do-while` first — it produces the closest structured result. The specific failure mode is **prologue block layout** and **delay slot filling in non-loop branches**, not the loop body itself. Fall back to `goto` only when the prologue/epilogue block ordering or branch polarity cannot be matched.

## Next Steps

1. Continue clearing remaining functions in `libc.c` (`memset` is logical next target).
2. Look at smaller remaining submodules such as `libcard` and `libsn` since `libc.c` only has a few targets left and the core delay-slot mechanisms are heavily documented.
