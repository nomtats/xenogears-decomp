# Decompiling ClearOTag and ClearOTagR

## Goal
Decompile `ClearOTag` and `ClearOTagR` in `libgpu.c` to reach a 100% byte-for-byte match, continuing the phase 4 libgpu pipeline.

## Process
1. Analyzed the assembly for `ClearOTag` which uses variables mapped to `0xFFFFFF` and `0xFF000000`.
2. Checked the standard PsyQ headers and identified that `ClearOTag` implementation is basically a loop setting the ordering table links using the `setlen(0)` and `setaddr(p, p+1)` macros.
3. The original code does `sb $zero, 3($s0)` for `setlen` and then standard `lw/and/or/sw` logical sequence for `setaddr`.
4. Recognized that `ClearOTagR` is simply a wrapper function dispatching through `g_GpuPkg->clearOTagR()`.
5. Both functions append the `D_8005698C` label (the GPU terminator limit/scratch address) to the end of the ordering table. Since no length mask is combined with this final address block, it's just `*ot = (u_long)&D_8005698C & 0x00FFFFFF;`, which naturally generates an explicit absolute `sw` assignment block.
6. The `asmdiff.py` script was slightly enhanced to also search in the `nonmatchings` folder, saving the need to manually move files to `matchings/` for initial testing.
7. Used `make asmdiff` locally inside docker: both functions matched 100% byte-for-byte on the first compilation pass.

## Outcome
- Matched `ClearOTag` and `ClearOTagR` perfectly.
- Recorded `D_8005698C` as a readability candidate for future renaming since it acts as the GPU scratchpad end marker.

## Next Steps
- Continue picking off remaining `libgpu` INCLUDE_ASM stubs like `SetDrawMove` or standard environment getters.
- Complete the Pattern Classifier skill.
