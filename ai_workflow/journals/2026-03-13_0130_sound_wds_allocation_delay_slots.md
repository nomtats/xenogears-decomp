# 2026-03-13: Sound WDS Allocation and Delay Slots

## Objective
Decompile `SoundLoadWdsFile`, `SoundSpuMemoryAllocateWDS`, and `SoundElementInit`.

## Actions Taken
1. Restructured `AudioElement` struct in `include/system/sound.h` by exploding a 35-byte black box `s8 unknown_data[0x23];` into explicit `u16` and `u8` variables based on assembly access patterns.
2. Successfully decompiled `SoundElementInit` (formerly `func_8003E3E0`) which initializes these newly discovered `AudioElement` fields.
3. Fixed backward conditional block in `SoundSpuMemoryAllocateWDS` to match original binary footprint.
4. Resolved the final "1-mile" puzzle in `SoundLoadWdsFile`:
   - Memory offsets for `spuMemoryAddress` and `pNext` were already perfectly mapped in the struct at `0x28` and `0x2C`.
   - The mismatch was caused by GCC 2.6.0 instruction reordering.
   - Using explicit assignments directly before `DisableEvent` and `EnableEvent` caused the compiler to lift the memory stores natively into the MIPS branch delay slots.
   - Rewrote the linked list traversal using a `do-while` loop combined with an initial `if (*curr != NULL)` check to flawlessly match the original MIPS branch layout.

## Valuable Findings (knowledge_base.json updates)
- **Delay Slot Hoisting:** GCC 2.6.0 will happily lift memory assignments (like `newEntry->pNext = NULL;`) into the delay slots of subsequent function calls (like `EnableEvent()`). If a decompiled function logically matches but order is slightly "off", try placing assignments directly *before* the function call to see if the compiler hoists them.
- **Linked List Traversal:** A standard modern `while(curr->next)` loop evaluates differently in PsyQ. To match original PS1 linked list traversals exactly, use a pre-condition `if (head != NULL)` followed by a `do { ... } while (curr != NULL);` loop.
- **Struct Black Boxes:** When faced with `unk_data array[...]` inside a struct, trace the assembly that initializes it (like `SoundElementInit`). It is often a quick way to uncover the true `u16` and `u8` layout of the struct.
