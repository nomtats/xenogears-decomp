# AI Decompilation Journal: Uncovering Fallback Sound Structs

## Target Context
While decompiling the SPU (Sound Processing Unit) handler `SoundHandleError`, we discovered it referenced five completely unknown global elements when an audio wipe was triggered.

The previous decompilation read like arbitrary memory references:
```c
extern void* SoundLoadWdsFile(void*, int);
extern int func_8003BDFC(u32);
extern void func_80039E60(u32);

extern u32 D_80050940[];
extern SoundFile D_80050910;
extern u16 D_80050924[];

void SoundHandleError(s32 errorId) {
    if ((g_SoundControlFlags & 0x88) == 0) {
        // ... SPU memory release code ...
        SoundLoadWdsFile(D_80050940, 0);
        SoundAddSedsEntry(&D_80050910);
        func_8003BDFC(0x10);
        func_80039E60((D_80050924[0] << 16) | 1);
    }
}
```

## Decompilation and Data Mapping
By navigating into the raw `asm/slus_006.64/data/` dumps representing the binary `.sdata` sections, we explicitly analyzed the memory structures at these pointers:

1. **`D_80050940` (Instrument Bank Data)**
   The word `0x20736477` mathematically reverses (due to little-endian string conversion) to `"wds "`. This acts as Xenogears' custom magic header for Wave Data / Instrument mappings on the disc. We renamed this global pointer to `g_SoundDefaultWdsData`.
2. **`D_80050910` (Sound Sequence Data)**
   The word `0x73646573` reverses to `"seds"`. This acts as Xenogears' custom magic header for Sound Effect Data Sequence formatting. We renamed this pointer to `g_SoundDefaultSedsData`.
3. **`func_8003BDFC` -> `SoundWaitSpuTransfer`**
   This function operates as a spin-wait loop halting CPU execution while SPU hardware transfer flag `0x10` completes on the `g_SoundControlFlags`. We encountered GCC 2.7.2 infinite-loop hoisting issues here and preserved the `INCLUDE_ASM`, but gracefully renamed and documented its objective.
4. **`func_80039E60` -> `SoundPlayDefaultSeds`**
   The final execution sequence casts the first index of `D_80050924` (renamed to `g_SoundDefaultSedsVoices`) out of 16 bits and fires heavily bitwise-manipulated config flags down to core `func_8003B644` API channels for playback.

## Results
The decompiled code was globally replaced out of the `INCLUDE_ASM` blocks and formatted neatly. 

```c
void SoundPlayDefaultSeds(u32 voiceConfigFlags) {
    if (g_SoundControlFlags & 0x800) {
        s16 voiceBaseParam = func_8003A65C(voiceConfigFlags, 2);
        D_80059404 = 2;
        func_8003B644(voiceBaseParam | 0x2000, voiceConfigFlags, 0x6000, 0x4000);
    }
}
```

By resolving the `.sdata` payload semantics (the WDS/SEDS signatures), we gained full architecture visibility into the Xenogears sound recovery structure!
