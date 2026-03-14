# MoveImage Decompilation — Deep Investigation (13 Attempts)

**Date**: 2026-03-14  
**Status**: BLOCKED — register allocation conflict, not yet matched  
**Function**: `MoveImage` in `src/slus_006.64/psyq/libgpu.c`  
**Target ASM**: `asm/slus_006.64/nonmatchings/psyq/libgpu/MoveImage.s` (49 instructions)

## Function Analysis

MoveImage transfers a VRAM rectangle to a new (x, y) position via the GPU DMA vtable dispatch:
1. Debug call: `func_8004463C(&D_800191A4, rect)`
2. Early return -1 if `rect->w == 0` or `rect->h == 0`
3. Pack destination: `(y << 16) | (x & 0xFFFF)`
4. Copy rect into static buffers `D_80056980`/`D_80056984`/`D_80056988`
5. Dispatch: `g_GpuPkg->dmaTransfer(g_GpuPkg->dmaOTagCfg, &D_80056980 - 2, 0x14, 0)`

## The Core Conflict

The target assembly requires TWO properties simultaneously:

### 1. `bnez` polarity on the height check
Target: `bnez v0, main_body` (branch to main body when h != 0, fall through to error `j + li -1`).

To get `bnez`, both error and success paths must **converge** at a shared `done:` label via `dest = -1; goto done; ... done: return dest;`. Without convergence, GCC inverts to `beqz`.

### 2. `v0` register for `dest` (the packed destination value)
Target uses v0 for `y<<16`, then `(y<<16) | (x & 0xFFFF)`, then stores to D_80056984. No extra move instructions.

**The conflict**: Convergence at `done:` forces GCC's register allocator to pick `a2` for `dest` (not `v0`), because `dest` and `rect->h` have overlapping live ranges at the branch point, and GCC gives v0 to the shorter-lived `rect->h` load.

## Attempts Summary

| # | Strategy | bnez? | v0? | Result |
|---|---|---|---|---|
| 1 | `goto do_move; return -1;` | no (beqz) | — | GCC inverts explicit goto |
| 2 | + buf pointer variable | no (beqz) | — | Same as 1 |
| 3 | `dest=-1; goto done; return dest;` | **yes** | no (a2) | +2 extra `move` instructions (51 vs 49) |
| 4 | + `register int dest asm("$2")` | **yes** | **yes** | Width check broken: nop delay slot, rect->h in v1, shared error block |
| 5 | if/else with shared return | **yes** | no (a2) | Identical output to attempt 3 |
| 6 | `if (h==0) { return -1; }` block | no (beqz) | — | GCC sees early exit, uses beqz |
| 7 | Scoped `register asm` in block | **yes** | **yes** | Identical to attempt 4 — GCC ignores block scope for asm |
| 8 | Hybrid: goto done + direct return | no (beqz) | — | Convergence lost → beqz |
| 9 | No dest variable, inline expression | no (beqz) | v0 (natural) | Body reordered, 2 insns shorter (47 vs 49) |
| 10 | dest computed after branch + goto | no (beqz) | — | GCC inverts when success uses return |
| 11 | goto done + dest after branch | **yes** | no (a2) | Convergence still forces a2 |
| 12 | register asm + width via dest | **yes** | **yes** | Same as attempt 4 — width still broken |
| 13 | `register int dest;` (no asm) | **yes** | no (a2) | register hint ignored at -O3 |

## Key Discoveries

### Why GCC chooses a2 instead of v0
Register allocation happens BEFORE delay slot filling. At allocation time:
- `dest = y << 16` is assigned before the branch
- `lh v0, 6(s0)` loads rect->h for the branch test
- These two live ranges overlap → they can't share v0
- GCC gives v0 to the load (shorter-lived, "expression result" preference), pushing dest to a2

In the TARGET binary, v0 is reused because the `sll` (y<<16) is in the delay slot AFTER the branch — the live ranges DON'T overlap in the scheduled code. But GCC allocates registers before scheduling.

### Why register asm("$2") breaks the width check
Pinning v0 for `dest` prevents GCC from:
1. Putting `li v0, -1` in the width beqz delay slot (v0 is "reserved")
2. Using v0 for `rect->h` load (forced to v1)
3. Having separate width/height error paths (merges them into shared block)

### External research
- SOTN decomp has a matched `MoveImage` but for a completely different PsyQ version (uses `addque2`/`cwc` dispatch, local stack array, no early returns)
- `SetDrawMove` in our own binary has the same bnez pattern + v0 for dest, but it's a leaf function (no jal) — the function call is what creates the register pressure
- No other PSX decomp project has a matching implementation for this PsyQ version's `MoveImage`

## Current State of Code

The code currently in `libgpu.c` is attempt 13 (same output as attempt 3):
```c
int MoveImage(RECT *rect, int x, int y) {
    register int dest;
    u_long *buf;
    func_8004463C(&D_800191A4, rect);
    if (rect->w == 0) return -1;
    dest = y << 16;
    if (rect->h != 0) goto do_move;
    dest = -1;
    goto done;
do_move:
    dest |= (x & 0xFFFF);
    buf = &D_80056980;
    *buf = *(u_long *)rect;
    D_80056984 = dest;
    D_80056988 = *((u_long *)rect + 1);
    dest = g_GpuPkg->dmaTransfer(g_GpuPkg->dmaOTagCfg, buf - 2, 0x14, 0);
done:
    return dest;
}
```

## Recommended Next Steps

1. **Keep iterating** — unexplored directions include:
   - Temp variable ordering (pattern_016) applied to the dest computation
   - Different expression decomposition (e.g., `(u16)y << 16 | (u16)x` like SOTN)
   - Loading g_GpuPkg into a local variable earlier to change register pressure
   - Reordering stores to match target instruction schedule
2. **If still stuck**, revert to `INCLUDE_ASM` and move to simpler functions (SetDrawMove is a leaf function, might match easily)
3. **Document as new KB pattern** when resolved — "register allocation at branch merge points"
