# AI Decompilation Journal: Sound Handle Error

## Target
`SoundHandleError` (Most called non-library function from the queue: 71 references)

## Context
When the game encounters severe audio failures (e.g. SPU buffers missing data, invalid callback flags), it triggers a hardware reset specifically for the SPU (Sony Processing Unit).

## Findings
The `SoundHandleError` function is short but dense with global variables:
```c
void SoundHandleError(s32 errorId) {
    if ((g_SoundControlFlags & 0x88) == 0) {
        g_SoundControlFlags |= 8;
        g_SoundSpuErrorId = errorId;
        SoundSpuMemoryFreeBlock(0x10000);
        SoundLoadWdsFile(D_80050940, 0);
        SoundAddSedsEntry(&D_80050910);
        func_8003BDFC(0x10);
        func_80039E60((D_80050924[0] << 16) | 1);
    }
}
```

- When an error triggers, it immediately throws `g_SoundControlFlags | 0x8` (preventing re-entry).
- It frees the absolute block of SPU dynamic memory layout (the high layout block `0x10000`).
- Reloads the default system `.wds` (Wave Data / Instrument bank) directly from default statically initialized pointers mapped into the data sections.
- `func_8003BDFC(0x10)` is structurally identified as a Spin-Wait that explicitly blocks execution while the SPU DMA transfer executes.

## Compilation Quirks
- We attempted to decompile the adjoining `func_8003BDFC` spin-loop:
```c
while (g_SoundControlFlags & 0x10) {}
```
- A strict MIPS disassembly diff revealed that GCC 2.7.2 aggressively hoists the `g_SoundControlFlags` pointer resolution outside of empty spin-loops unless using `asm("")` or explicit volatile casting.
- Even locally casting to `extern volatile` inside the function or utilizing `asm("")` generated subtle multi-instruction mismatches vs the original PsyQ `lui/lhu` loop evaluation style.
- We opted to revert `func_8003BDFC` to `INCLUDE_ASM` as it provides minimal human-readability over the actual underlying SPU mechanism.

## Conclusion
The main `SoundHandleError` target matched byte-for-byte and provides total clarity on what happens when Xenogears catches an SPU processing exception!
