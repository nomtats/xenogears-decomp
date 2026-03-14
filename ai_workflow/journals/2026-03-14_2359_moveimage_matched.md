# MoveImage Decompilation — Matched 100% (49/49)

**Date**: 2026-03-14  
**Status**: MATCHED — 49/49 instructions, byte-perfect  
**Function**: `MoveImage` in `src/slus_006.64/psyq/libgpu.c`  
**Target ASM**: `asm/slus_006.64/matchings/psyq/libgpu/MoveImage.s` (49 instructions)  
**Total attempts**: 25+  
**Previous journal**: [2026-03-14_2300_moveimage_investigation.md](./2026-03-14_2300_moveimage_investigation.md) (attempts 1–13)

## Resolution Summary

MoveImage was the hardest function matched to date. The core conflict (documented in the previous journal) was: `bnez` branch polarity requires convergence at a merge point, but convergence forces GCC to pick `a2` instead of `v0` for `dest`. After 13 attempts with goto-based approaches, the solution came in three phases:

### Phase 1: Inverted If-Body Pattern (Attempt 24) — 38/49
**Breakthrough**: `if (dest) { body; return dest; } return -1;` produces `bnez` WITHOUT goto. GCC lays out blocks as: test → error inline → body after. The `bnez` jumps over the error path to the body. Combined with `register int dest asm("$2")` for v0 pinning. This solved the fundamental branch polarity + register conflict cleanly. Documented as KB pattern_017.

### Phase 2: Barrier + Pkg Pinning (Attempt 25) — 45/49
Added `register GpuPackage *pkg asm("$3") = g_GpuPkg` to pin the vtable pointer to v1, plus one `__asm__("")` barrier to prevent GCC from reordering the g_GpuPkg load past the store sequence. This fixed scheduling for most of the function body.

### Phase 3: Explicit Inline Asm (Final — human-driven) — 49/49
The last 4 mismatches were a delay slot scheduling preference: GCC filled a load delay slot with `lw a0, dmaOTagCfg` while the original compiler used `move a3, zero`. No C-level technique could fix this. The human solved it with:
- `register int a2 asm("$6")` + `__asm__("addiu $6, $0, 0x14")` — pins and emits a2 setup
- `__asm__("move $7, $0")` — forces a3=0 into the exact delay slot position
- `__asm__("")` barrier after D_80056988 store — prevents dmaOTagCfg hoisting
- Pass a2/a3 as variables to `pkg->dmaTransfer(...)` to avoid conflicting arg setup

## Failed Approaches (Attempts 14–25+)

| # | Strategy | Match | Why it failed |
|---|---|---|---|
| 14 | Pin rect->h to v0 via temp | — | Generated `lhu` instead of `lh` (unsigned vs signed halfword) |
| 17–18 | __asm__("") barriers with temp vars | — | GCC -O3 optimized away temps, barriers had nothing to anchor |
| 19 | __asm__("") barrier without temps | ~42/49 | Moved g_GpuPkg load earlier (good) but shifted other regs |
| 21 | Second __asm__("") barrier | 40/49 | Over-constrained: broke interleaving of call arg setup with stores |
| CDK | Different GCC build (gcc-2.7.2-cdk) | — | Completely different register allocation, wrong universe |
| 23 | if-else without goto for height | ~40/49 | `if (dest == 0) return -1;` gives beqz (wrong polarity) |
| 24 | Inverted if-body + dest pinning | 38/49 | BREAKTHROUGH for branches, body scheduling still off |
| 25 | + barrier + pkg pin to $3 | 45/49 | Close, but 4-instruction delay slot rotation remains |
| 26 | + second barrier after D_80056988 | 40/49 | Regressed — barrier prevented a2/a3 interleaving |

## Key Failure Lessons

1. **Goto creates merge-point register conflicts** — GCC reconciles registers from different paths at merge points, consistently picking worse registers. Avoid goto when if/else can produce the same branch polarity.

2. **Global register pinning can backfire** — Pinning dest to v0 everywhere prevents GCC from using v0 as scratch for intermediate operations (width check comparison).

3. **The optimizer sees through temp variables** — At -O3, temp variables are transparent. Barriers anchored to temps have nothing to anchor to.

4. **Type signedness changes the instruction** — `short` → `lh`, `u_short` → `lhu`. One wrong cast = one wrong instruction.

5. **More barriers ≠ better** — One barrier: 45/49. Two barriers: 40/49. Over-fragmenting scheduling blocks prevents useful interleaving.

6. **Alternative compilers aren't a shortcut** — CDK GCC produced completely different code. Stick with the project's compiler.

7. **Load delay slot scheduling is the final boss** — After solving control flow and register allocation, the remaining mismatches are pure scheduling heuristic. Only inline asm works.

## Final Code

```c
int MoveImage(RECT *rect, int x, int y) {
    register int dest asm("$2");
    u_long *buf;

    func_8004463C(&D_800191A4, rect);

    dest = rect->w;
    if (dest == 0) return -1;

    dest = rect->h;
    if (dest) {
        dest = y << 16;
        dest |= (x & 0xFFFF);
        buf = &D_80056980;
        /* Equivalent clean C:
         *   D_80056984 = dest;
         *   *buf = *(u_long *)rect;
         *   dest = *((u_long *)rect + 1);
         *   D_80056988 = dest;
         *   dest = g_GpuPkg->dmaTransfer(g_GpuPkg->dmaOTagCfg, buf - 2, 0x14, 0);
         */
        {
            u_long rectData = *(u_long *)rect;
            register GpuPackage *pkg asm("$3") = g_GpuPkg;
            register int a2 asm("$6");
            register int a3 asm("$7");
            __asm__("addiu $6, $0, 0x14");
            D_80056984 = dest;
            *buf = rectData;
            dest = *((u_long *)rect + 1);
            __asm__("move $7, $0");
            D_80056988 = dest;
            __asm__("");
            dest = pkg->dmaTransfer(pkg->dmaOTagCfg, buf - 2, a2, a3);
        }
        return dest;
    }
    return -1;
}
```

## KB Patterns Added

- **pattern_017**: Inverted if-body produces bnez without goto
- **pattern_018**: Inline asm for delay slot scheduling control
- **pattern_019**: Barrier dosage — one helps, two can hurt
- **pattern_020**: Scheduling escalation ladder (clean C → pin → barrier → inline asm)
- **anti_pattern_003**: MoveImage failure catalog (6 failure classes)

## Tooling Built

- **tools/asmdiff.py**: Per-instruction diff tool with relocation masking. `make asmdiff FUNC=X` inside Docker for single-command build+diff. Dramatically improved iteration speed.
