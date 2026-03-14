# Symbol Rename & Struct Recovery

## When to Use

Use this skill when:
- A decompiled function references a cryptic auto-generated symbol (`D_XXXXXXXX`, `func_XXXXXXXX`) whose meaning is now clear from context
- Multiple decompiled functions access the same global pointer with array offsets, suggesting an underlying struct
- A human review flags code as unreadable due to raw hex symbols or cast-heavy pointer arithmetic

**Prerequisite:** At least one caller of the symbol must be decompiled so you have enough context to determine the symbol's semantic meaning.

## Phase 1: Simple Symbol Rename

### Step 1: Determine the Meaning

Read the decompiled C code and assembly callers to understand what the symbol represents. Consider:
- Is it a function? A global variable? A pointer to a struct/table?
- What subsystem does it belong to? (libgpu, sound, archive, etc.)
- Follow the project naming convention: `g_` prefix for globals, `CamelCase` for functions

### Step 2: Discover All References

**Critical:** The `asm/` directory is gitignored, so workspace search tools (Grep tool, ripgrep) will silently miss it. Always use `grep -r` directly via Shell:

```bash
grep -rl "D_XXXXXXXX" asm/ src/ include/ config/ linker/ 2>/dev/null
```

This gives you the complete list of files to update.

### Step 3: Add to Symbol Address Map

Add the new name to `config/symbol_addrs.slus_006.64.txt` in the appropriate section:

```
// PSY-Q Libgpu
g_GpuPkg = 0x800568C8;
```

### Step 4: Global Rename in Assembly

Use `sed -i` on all assembly files found in Step 2:

```bash
sed -i '' 's/D_XXXXXXXX/g_NewName/g' file1.s file2.s ...
```

This covers:
- `asm/slus_006.64/nonmatchings/` — per-function assembly stubs
- `asm/slus_006.64/psyq/*.s` — module-level assembly
- `asm/slus_006.64/data/*.s` — data label definitions (dlabel)

### Step 5: Update C Source

Change the `extern` declaration and all usages in the relevant `.c` file.

### Step 6: Verify

```bash
docker exec xenogears_decomp_env make check
```

Both `SLUS_006.64: OK` and `FIELD.BIN: OK` must pass. If not, a reference was missed — re-run Step 2.

---

## Phase 2: Struct Recovery (When Applicable)

Use this phase when the renamed symbol is a pointer to a table accessed at multiple offsets (e.g., a vtable, a DMA config block, a hardware register map).

### Step 7: Collect All Offset Accesses

Read every assembly file that references the symbol. For each, note:
- The offset used (`lw $v0, 0x3C($v0)` → offset 0x3C)
- Whether the loaded value is called as a function (`jalr`) or used as data
- What arguments are passed (for function pointers)
- Which high-level function the access belongs to

Build a complete offset map:

| Offset | Type | Used by |
|:-------|:-----|:--------|
| 0x08 | func ptr | LoadImage, StoreImage, DrawOTag |
| 0x0C | data | ClearImage, ClearImage2 |
| ... | ... | ... |

### Step 8: Define the Struct

Create a `typedef struct` in the appropriate header. Apply the **quality ladder** — each level is independently valuable and should be verified with `make check` before proceeding:

#### Level 1: Named Members
Replace raw `u_long[]` with named struct members. Even generic names (`fn_08`, `param_0C`) are better than magic indices.

#### Level 2: Descriptive Names
Rename members to reflect their domain role:
- `transfer` → `dmaTransfer` (it's a DMA operation)
- `clearParam` → `dmaClearCfg` (it's a DMA channel configuration)
- `command` → `sendGP1` (it writes to the GPU GP1 port)

#### Level 3: Typed Signatures
For each function pointer, perform the **typed-vs-polymorphic analysis**:

1. Collect all call sites for the function pointer
2. List the argument types each caller passes
3. **If all callers agree** on the signature → add typed parameters:
   ```c
   int (*drawSync)(int mode);
   ```
4. **If callers pass different types** for the same positional arg → keep K&R `()`:
   ```c
   int (*dmaTransfer)();  /* polymorphic — see Doxygen for variants */
   ```

**Why this is safe:** On GCC 2.7.2 MIPS, both K&R `()` and typed prototypes generate identical code for 32-bit arg types (`int`, `u_long`, pointers). No codegen risk.

#### Level 4: Documentation
Add Doxygen `@brief` / `@param` / `@return` to every non-trivial member. For polymorphic function pointers, **document all known call signatures explicitly**:

```c
/**
 * @brief Initiate a GPU DMA transfer.
 *
 * Polymorphic — known call signatures:
 *   LoadImage:  dmaTransfer(dmaLoadCfg,  RECT *rect, 8, u_long *src)
 *   StoreImage: dmaTransfer(dmaStoreCfg, RECT *rect, 8, u_long *dst)
 *   DrawOTag:   dmaTransfer(dmaOTagCfg,  u_long *ot, 0, 0)
 */
/* 0x08 */ int (*dmaTransfer)();
```

### Step 9: Update C Callers

Replace all array-index + cast patterns with struct member access:

```c
// Before:
((int (*)(int))D_800568C8[0xF])(mode);

// After:
g_GpuPkg->drawSync(mode);
```

### Step 10: Final Verification

```bash
docker exec xenogears_decomp_env make check
```

---

## Gotchas

- **asm/ is gitignored.** Always use `grep -r` via Shell, never the workspace Grep tool, for assembly file discovery.
- **Data labels exist in `asm/slus_006.64/data/*.sdata.s`.** Don't forget these — they define the actual storage (`dlabel`) for the symbol.
- **Linker files are auto-generated.** You typically don't need to edit them, but verify with `make check`.
- **Run `make check` (not just micro-build) after renames.** The full link step catches cross-module reference breakage that object-level builds miss.
- **This skill is iterative.** You don't have to reach Level 4 in one pass. Each level is a valid stopping point. Human review between levels often identifies improvements the AI misses.

## Reference

- **Example:** `D_800568C8` → `g_GpuPkg` (`GpuPackage` struct), 2026-03-14
- **KB Pattern:** `pattern_example_015` (PsyQ GPU Vtable Dispatch)
- **Journal:** [2026-03-14_1600_gpu_package_struct.md](../../journals/2026-03-14_1600_gpu_package_struct.md)
