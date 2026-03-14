# ClearImage & ClearImage2 Decompilation — OR Chain Register Control

**Date:** 2026-03-14
**Functions:** `ClearImage`, `ClearImage2`
**Module:** `src/slus_006.64/psyq/libgpu.c`
**Result:** Both matched byte-for-byte. `make check` passes.

## ClearImage — Trivial Match

Standard `g_GpuPkg` vtable dispatch (KB pattern_example_015). Packs RGB as `(b << 16) | (g << 8) | r` and dispatches through `dmaTransfer(dmaClearCfg, rect, 8, color)`. Matched on first attempt — identical structure to `LoadImage`/`StoreImage`.

## ClearImage2 — 5-Iteration Struggle

Identical to ClearImage but ORs `0x80000000` (STP bit) into the color word. The extra constant caused two interrelated problems:

### Problem 1: Register Allocation Swap

ClearImage naturally allocates `s0=b(a3), s1=g(a2)`. The target ClearImage2 has `s0=g(a2), s1=b(a3)` — swapped! No amount of expression reordering or parenthesization changed this allocation.

### Problem 2: OR Chain Reassociation

GCC 2.7.2 treats OR as commutative+associative and freely reorders operands, ignoring explicit parentheses. With `(b << 16) | ((g << 8) | 0x80000000) | r`, GCC would ORed `0x80000000` with whichever register it preferred, not necessarily the `(g<<8)` operand.

### Attempts and Outcomes

| # | Expression / Technique | Result |
|---|---|---|
| 1 | `(b<<16) \| (g<<8) \| 0x80000000 \| r` | 0x80000000 ORed with r (s2) instead of g |
| 2 | `((g<<8) \| 0x80000000) \| (b<<16) \| r` | Correct grouping, wrong register alloc (s0=b, s1=g) |
| 3 | `register u_char asm("s0")=g, asm("s1")=b` | Right registers, but GCC used a3 as accumulator instead of computing in-place |
| 4 | `int color = (g<<8)\|0x80000000; return ...(b<<16)\|color\|r;` | Right registers (s0=g, s1=b)! But g processed before b (wrong eval order) |
| 5 | `int bv=b<<16; int color=(g<<8)\|0x80000000; return ...bv\|color\|r;` | **PERFECT MATCH** |

### Key Insight (KB pattern_example_016)

Two separate assignment statements control **both** register allocation **and** evaluation order:
- The variable assigned to the **first** temp gets the lower s-register (s0 or s1 depending on allocation context).
- Statements are evaluated in **declaration order**, preventing GCC from reassociating across statement boundaries.
- Single temp only fixes one dimension; two temps are needed when both matter.

### Why ClearImage2 Differs From ClearImage

ClearImage's symmetric expression `(b<<16)|(g<<8)|r` lets GCC use its default allocation. ClearImage2's asymmetric subexpression `(g<<8)|0x80000000` creates a deeper expression tree that changes the register allocator's Sethi-Ullman evaluation strategy, flipping s0/s1.

## MoveImage Analysis (Deferred)

Assembly fully analyzed for next session:
- `func_8004463C(&D_800191A4, rect)` debug call
- Early return -1 if `rect->w == 0` or `rect->h == 0`
- `bnez` true-branch polarity on h-check → needs goto
- Copies rect into static buffer `D_80056980`/`D_80056984`/`D_80056988`
- Dispatches `dmaTransfer(dmaOTagCfg, &D_80056980 - 2, 0x14, 0)`
- Static symbols need `extern u_long` declarations in C source

## Patterns Applied
- KB pattern_example_015 (GPU vtable dispatch)
- KB pattern_example_016 (NEW: temp variable ordering for OR chains)
