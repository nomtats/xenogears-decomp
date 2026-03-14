# Post-Match Reflection

## When to Use

Execute this skill **immediately** after one of these events:

- A function achieves a byte-perfect match (`make check` passes)
- A function is being **abandoned** as failed or unmatchable

This is not optional. It replaces AGENT.md steps 4–6 with a more comprehensive checklist. Do not move to the next target until all applicable steps are complete.

## Success Path

Follow all 6 steps in order after a byte-match.

### Step 1: Doxygen Documentation

Add a `/** ... */` block comment above the matched function with `@brief`, `@param`, and `@return` tags.

If the function uses non-obvious GCC tricks (goto blocks, register asm, delay slot placement, temp variable ordering), add an inline comment **before** the trick explaining *why* it exists:

```c
/**
 * @brief Clear a rectangular area on the VRAM with a specified color.
 * @param rect Pointer to the RECT defining the clear area.
 * @param r Red component (0–255).
 * @param g Green component (0–255).
 * @param b Blue component (0–255).
 * @return 0 on success, -1 on invalid rect dimensions.
 */
int ClearImage2(RECT *rect, int r, int g, int b) {
    /* Separate temps control both register allocation (s0=g, s1=b) and
       evaluation order. A single expression lets GCC reassociate the OR
       chain freely, merging 0x80000000 with the wrong operand. */
    int bv = b << 16;
    int color = (g << 8) | 0x80000000;
    ...
}
```

### Step 2: Goto Reduction (time-boxed: 5 minutes)

If the matched code contains `goto` statements, attempt to replace them with structured C (`do-while`, `if/else`, `for`).

**Procedure:**
1. Try `do-while` first — it produces the closest structured result for PS1 loop patterns.
2. Micro-build after each attempt. If the match breaks, revert immediately.
3. If structured C cannot match, document **which gotos are load-bearing and why** in an inline comment. Use this table format from the memchr analysis as a reference:

| Goto | Purpose | What GCC does without it |
|------|---------|--------------------------|
| `goto loop_check` | Forces jump-into-middle with size-- in j's delay slot | Hoists size-- into blez delay slot |
| `goto match` | Forces beq true-branch polarity | Inverts to bne, adds extra j;nop |

**Time limit:** If you haven't found a reduction in 5 minutes, stop. Record the result and move on.

### Step 3: Append Outcome Record

Append one JSON line to `ai_workflow/outcome_log.jsonl` for the matched function.

**Schema — Required fields** (must always be filled):

| Field | Type | Description |
|-------|------|-------------|
| `function` | string | Function name (e.g. `"ClearImage2"`) |
| `module` | string | Subsystem path (e.g. `"psyq/libgpu"`) |
| `source_file` | string | C source path (e.g. `"src/slus_006.64/psyq/libgpu.c"`) |
| `outcome` | string | `"matched"` |
| `date` | string | ISO date `"YYYY-MM-DD"` |
| `attempts` | int | Total compilation attempts before match |
| `winning_patterns` | array | KB pattern IDs that produced the match (e.g. `["pattern_example_015"]`). Empty `[]` if none. |

**Schema — Optional fields** (fill when known, omit when not):

| Field | Type | Description |
|-------|------|-------------|
| `asm_lines` | int | Instruction count in the original assembly |
| `patterns_attempted` | array | Ordered list of all patterns tried, including failed ones |
| `failure_modes` | array | Mismatch symptoms encountered (e.g. `["register_swap_s0_s1"]`) |
| `difficulty` | string | `"trivial"`, `"easy"`, `"medium"`, or `"hard"` |
| `goto_required` | bool | Whether goto was needed for the final match |
| `goto_reduction_result` | string | `"not_attempted"`, `"fully_reduced"`, `"partially_reduced"`, or `"all_load_bearing"` |
| `readability_candidates` | array | Cryptic symbols flagged in Step 6 (e.g. `["D_80056980", "func_8004463C"]`) |
| `batch_parent` | string | For batch matches: name of the first function in the batch |
| `journal` | string | Journal filename (e.g. `"2026-03-14_2100_clearimage_pair_match.md"`) |
| `notes` | string | Free-text for anything not captured above |

**Example entry (hard match):**
```json
{"function":"ClearImage2","module":"psyq/libgpu","source_file":"src/slus_006.64/psyq/libgpu.c","outcome":"matched","date":"2026-03-14","attempts":5,"winning_patterns":["pattern_example_015","pattern_example_016"],"asm_lines":28,"patterns_attempted":["pattern_example_015","pattern_example_003","pattern_example_016"],"failure_modes":["register_swap_s0_s1","or_chain_reassociation"],"difficulty":"hard","goto_required":true,"goto_reduction_result":"not_attempted","journal":"2026-03-14_2100_clearimage_pair_match.md"}
```

**Example entry (trivial match):**
```json
{"function":"memset","module":"psyq/libc","source_file":"src/slus_006.64/psyq/libc.c","outcome":"matched","date":"2026-03-14","attempts":1,"winning_patterns":["pattern_example_013"],"difficulty":"trivial","goto_required":true,"goto_reduction_result":"all_load_bearing","journal":"2026-03-14_1000_memset_and_bios_antipattern.md","notes":"Direct reuse of bzero flat goto template"}
```

**Example entry (batch sibling):**
```json
{"function":"StoreImage","module":"psyq/libgpu","source_file":"src/slus_006.64/psyq/libgpu.c","outcome":"matched","date":"2026-03-14","attempts":1,"winning_patterns":["pattern_example_015"],"asm_lines":27,"difficulty":"easy","batch_parent":"DrawSync","journal":"2026-03-14_1400_gpu_vtable_dispatch.md"}
```

### Step 4: Knowledge Base Update

Check whether the match revealed anything new:

- **New technique discovered?** Add a new entry to `ai_workflow/knowledge_base.json` with `id`, `concept`, `description`, `tags`, and `date_added`. Include a `used_in` field listing the function name.
- **Existing pattern reused?** No KB change needed — the outcome record already tracks which patterns were applied.
- **Existing pattern failed in a new way?** Update that pattern's `description` to document the new failure mode and workaround.
- **Nothing new?** Skip this step. Don't add noise to the KB.

### Step 5: Update TOC Dashboard

Open `ai_workflow/journals/TOC.md` and:

1. Revise the **Current Status** section to reflect the newly matched function(s) and updated module completion percentages.
2. Update the **Next Steps** section so the next agent session knows exactly where to resume.
3. If you wrote a journal entry for this session, append it to the log table at the bottom.

### Step 6: Readability Scan

Scan the matched function and its immediate callers (the functions that `jal` to it) for:

- **Cryptic symbols:** `D_XXXXXXXX` or `func_XXXXXXXX` references where the meaning is now clear from context.
- **Pointer arithmetic:** Raw `((type*)ptr)[offset]` patterns that suggest an underlying struct.
- **Cast-heavy calls:** `((int (*)(int, int))table[N])(a, b)` patterns that suggest a vtable.

Record any candidates in the outcome record's `readability_candidates` field. **Do not start renaming here** — that's a separate invocation of the Symbol Rename skill, potentially with human review for design taste.

---

## Failure Path

Follow these 3 steps when abandoning a function (too many failed attempts, or classified as unmatchable).

### Step F1: Append Failure Record

Append one JSON line to `ai_workflow/outcome_log.jsonl`.

**Required fields for failures:**

| Field | Type | Description |
|-------|------|-------------|
| `function` | string | Function name |
| `module` | string | Subsystem path |
| `source_file` | string | C source path |
| `outcome` | string | `"failed"` or `"unmatchable"` |
| `date` | string | ISO date |
| `attempts` | int | Total attempts before giving up (0 for unmatchable-by-inspection) |
| `blocking_reason` | string | Specific, actionable reason (e.g. `"3-instruction BIOS tail-call trampoline, no naked attribute in GCC 2.7.2"`) |
| `patterns_attempted` | array | What was tried |

**Example (unmatchable):**
```json
{"function":"StartPAD2","module":"psyq/libapi","source_file":"src/slus_006.64/psyq/libapi.c","outcome":"unmatchable","date":"2026-03-14","attempts":0,"blocking_reason":"3-instruction BIOS tail-call trampoline via jr $t2; no __attribute__((naked)) in GCC 2.7.2 MIPS","patterns_attempted":[],"batch_parent":"antipattern_001_bios_wrappers","journal":"2026-03-14_1000_memset_and_bios_antipattern.md"}
```

### Step F2: KB Update (if applicable)

- **New anti-pattern?** Add an `antipattern_NNN` entry to the KB documenting why the function is unmatchable and how to recognize similar cases.
- **Known technique failed in a new way?** Update the pattern's description.

### Step F3: Update TOC Dashboard

Mark the function's status as failed/unmatchable in the Current Status section. Note the blocking reason in Next Steps so future agents don't re-attempt it.

---

## Batch Match Shortcut

When multiple functions match in the same session using the same technique (e.g. 5 GPU vtable dispatchers):

1. **Full reflection (all 6 steps)** on the **first** function.
2. **Steps 3 + 5 only** for each subsequent sibling — append an outcome record with `batch_parent` set to the first function's name, and update the TOC once at the end covering all siblings.
3. Skip Steps 1, 2, 4, 6 for siblings unless a sibling required a meaningfully different technique.

---

## Gotchas

- **Don't skip Step 3 for trivial functions.** The outcome log's value is statistical completeness. A 1-attempt trivial match is still a data point.
- **Step 2 is time-boxed.** If structured C doesn't match within 5 minutes, revert and move on. The goto is load-bearing.
- **Step 6 is observation-only.** Do not start a rename mid-reflection. Flag candidates, finish reflection, then decide whether to invoke the Symbol Rename skill.
- **One outcome record per function.** Even in batch matches, each function gets its own JSONL line. The `batch_parent` field links them.
- **This skill replaces AGENT.md steps 4–6.** Follow this checklist instead of the older, less specific instructions in AGENT.md section 2.

## Reference

- **Roadmap item:** Tier 1 — Post-Match Reflection Template
- **Related skill:** [Symbol Rename & Struct Recovery](../symbol_rename/SKILL.md)
- **Outcome log:** `ai_workflow/outcome_log.jsonl`
- **Knowledge base:** `ai_workflow/knowledge_base.json`
