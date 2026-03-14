# Workflow Evolution Roadmap

## Purpose
This document tracks the strategic evolution of the AI decompilation workflow itself. `AGENT.md` defines how agents operate today; this document defines how we intend to improve that operation over time.

It is a living document. When a milestone is completed, a new gap is identified, or priorities shift, this file must be updated and the change logged in the revision history at the bottom.

## Guiding Principles
1. **The workflow is the product; decompilation is the test harness.** Every session should leave the *system* better, not just the *codebase*.
2. **Notes must become executable.** Narrative journals are step one. Structured data is step two. Automated scripts are step three. A pattern that only exists in prose hasn't finished evolving.
3. **Every improvement must be measurable.** "The workflow feels faster" is not evidence. Tracked metrics over time are.
4. **Minimize cold-start cost.** A new agent session should reach productive work within minutes, not after reading 20+ journal entries.

## Current Workflow Maturity Assessment

### What Works Well
- **Persistent memory triad**: The Knowledge Base (`knowledge_base.json`), Journals (`journals/`), and TOC Dashboard (`journals/TOC.md`) successfully preserve context across sessions. No critical discovery has been lost.
- **Micro-Build iteration loop**: Compiling individual `.o` files via `make build/src/.../file.c.o` reduces feedback to milliseconds, enabling rapid trial-and-error without linker interference.
- **Jigsaw Strategy + Call Graph targeting**: `generate_call_graph.py` provides data-driven target selection by reference count and module clustering, validated by the `libspu` 100% completion.
- **Rich compiler trick catalog**: 16 KB patterns covering delay slots, goto manipulation, register forcing, OR-chain control, linked list idioms, and more — all battle-tested against real functions.
- **Iterative human-AI readability review**: The GpuPackage struct (2026-03-14) was refined across 3 passes — basic names → descriptive names + docs → typed signatures + Doxygen — each triggered by human design review. The AI handles mechanical work (grep, rename, verify); the human drives design taste. This produced higher-quality code than either could alone.

### What's Missing
- **Knowledge is write-only.** Patterns are appended to the KB but there's no structured retrieval procedure. An agent must read all 16 entries and intuit which apply.
- **No mismatch-to-fix mapping.** Most iteration time is spent *after* the first attempt, diagnosing *why* the output doesn't match and choosing a fix. The ClearImage2 session (2026-03-14) burned 4 of 5 iterations at this stage — the agent could see "s0/s1 swapped, constant ORed with wrong operand" but had no structured path from symptom to solution. Pattern_003 (register asm) was tried and failed for a new reason before pattern_016 (temp variable ordering) was discovered. A "mismatch symptom → fix" lookup would have saved most of that time.
- **No pre-analysis classification.** Target selection relies on reference count and line count, but not structural signatures (spin loops, GTE routines, SN wrappers). Agents discover unmatchable functions only after wasting time on them.
- **The feedback loop isn't closed.** Outcome data (attempts, iterations, failure modes, winning patterns) is buried in journal prose, not structured records. There's no way to statistically analyze what works.
- **Skills directory is underutilized.** Only one skill exists (Docker setup). The core decompilation loop, symbol renaming, post-match reflection, and pattern classification are all manual, undocumented procedures.
- **No readability checkpoint in the workflow.** The current loop is "match → document → next target" but doesn't pause to ask whether matched code is actually human-readable. Cryptic auto-generated symbols (`D_800568C8`) and raw pointer casts persist until a human intervenes. The GpuPackage struct refactor (2026-03-14) was triggered by a human design review, not by any automated workflow step — yet it was arguably higher-impact than matching additional functions. Notably, each refinement round was driven by human feedback ("these are cryptic", "add docs", "type the fixed-arg ones") — the AI had the technical ability but didn't proactively identify the need. Fully autonomous readability passes may hit diminishing returns without human-in-the-loop review for design taste.
- **Journals don't scale.** 20 entries after one day. At this rate, the journal archive becomes unsearchable within a week. No topical index, no function-to-journal mapping.
- **No workflow metrics.** We cannot prove the workflow is improving because we don't measure it.

---

## Evolution Tiers

### Tier 1: Close the Feedback Loop
**Goal:** An agent can select and apply the right compiler pattern for a new function without reading any journal entries.

- [ ] **Pattern Decision Tree Skill** (`ai_workflow/skills/pattern_classifier/SKILL.md`)
  - Extract the implicit flowchart from journals into a formal, versioned decision tree.
  - **Two layers** — both are needed to minimize wasted iterations:
    1. **Pre-attempt classification** — Input: assembly characteristics (instruction types, branch polarity, delay slot usage, loop structure). Output: ranked list of applicable KB patterns with confidence indicators.
    2. **Mismatch diagnosis** — Input: specific objdiff symptom (wrong register, wrong branch polarity, constant merged with wrong operand, extra/missing instruction). Output: ranked fix strategies with known failure modes.
  - Example mismatch→fix entries (from real sessions):
    - "s0/s1 register allocation swapped" → try `register asm()` forcing (KB pattern_003); if GCC copies to caller-saved accumulator instead, fall back to temp-variable ordering (KB pattern_016).
    - "OR constant merged with wrong operand" → GCC reassociates OR chains freely; split into separate assignment statements via temp variables (KB pattern_016).
    - "beqz instead of bnez (wrong branch polarity)" → first try inverted if-body pattern: `if (cond) { body; return ok; } return err;` (KB pattern_017). Only fall back to flat goto (KB pattern_010) if the inverted pattern doesn't fit the control flow.
    - "Instructions reordered in load delay slot" → follow the Scheduling Escalation Ladder (KB pattern_020): register pinning → one `__asm__("")` barrier → explicit `__asm__("instruction")` as last resort (KB pattern_018). Beware: adding multiple barriers can regress matching (KB pattern_019).
    - "lhu instead of lh (or vice versa)" → check C type signedness. `short` → `lh`, `u_short`/`u16` → `lhu`. One wrong cast changes the instruction (KB anti_pattern_003).
  - Must be updated whenever a new pattern is added to the KB. Current KB has 21 patterns (pattern_001–020 + anti_pattern_003).

- [x] **Structured Outcome Records** (`ai_workflow/outcome_log.jsonl`)
  - Every decompilation attempt (success or failure) produces a machine-readable entry: function name, module, ASM line count, number of attempts, first approach, failure mode, winning patterns, iteration count, difficulty rating.
  - Append-only JSONL format for easy parsing.
  - Enables statistical analysis of pattern effectiveness and difficulty prediction.
  - **Bootstrapped** with ~45 retroactive entries from journals (22 matched functions, 1 failed, 17 unmatchable BIOS wrappers + setjmp/longjmp).

- [x] **Post-Match Reflection Template** (`ai_workflow/skills/post_match_reflection/SKILL.md`)
  - A mandatory checklist executed after every successful byte-match:
    1. Write Doxygen documentation on the matched function.
    2. Attempt goto reduction (can structured C produce the same output?).
    3. Append structured outcome record to `outcome_log.jsonl`.
    4. If a new technique was used, add it to `knowledge_base.json` with cross-references.
    5. Update `journals/TOC.md` status and next steps.
    6. **Readability check:** Are there cryptic `D_`/`func_` symbols that now have enough context to rename? Can raw pointer arithmetic be replaced with a struct definition? Flag candidates for a readability pass.
  - Prevents the most commonly skipped steps (documentation, KB updates, reflection).
  - **Also covers failure path** (3 steps for abandoned/unmatchable functions) and **batch match shortcut** (full reflection on first, abbreviated for siblings).

- [x] **Symbol Rename & Struct Recovery Skill** (`ai_workflow/skills/symbol_rename/SKILL.md`)
  - Documented procedure for the global rename workflow:
    1. Identify candidate symbol (e.g. `D_800568C8`) and determine its semantic meaning from surrounding code.
    2. grep across all file categories (asm/, src/, include/, config/, linker/) to find all references.
    3. Add human-readable name to `config/symbol_addrs.*.txt`.
    4. Execute global search-and-replace across all assembly and data files.
    5. Update C source declarations and usages.
    6. For pointer-to-table symbols: recover struct layout by analyzing all offset accesses across callers, classify members as function pointers vs data, and define a typedef struct in the appropriate header.
    7. Micro-build + `make check` to verify byte-match is preserved.
  - **Struct recovery follows a quality ladder** — each layer is independently valuable and verifiable:
    1. **Named struct**: Replace raw `u_long[]` array with named members (eliminates magic index numbers).
    2. **Descriptive names**: Rename members to reflect their domain role (e.g. `transfer` → `dmaTransfer`, `clearParam` → `dmaClearCfg`).
    3. **Typed signatures**: For function pointers with consistent call signatures, add typed parameters. For polymorphic members (callers pass different types), keep K&R `()`.
    4. **Documentation**: Add Doxygen `@brief`/`@param` to each member. For polymorphic members, document all known call signatures explicitly.
  - **Typed vs polymorphic analysis**: For each function pointer in a recovered vtable, collect all call sites and compare argument types. If all callers agree on the signature → type it. If callers pass different types for the same positional arg → keep K&R `()` and document the variants.
  - **Key technique**: K&R-style `()` function pointers generate identical code to explicit casts on GCC 2.7.2 MIPS because all arg types are already ≥32-bit on MIPS32. Typed signatures also match — no codegen difference for `int`, `u_long`, or pointer args.

**Success Criteria:** A new agent session can pick its first target and identify the right decompilation approach within 5 minutes, using only the decision tree and KB — without reading any journals.

---

### Tier 2: Automate Target Selection
**Goal:** Target selection requires zero human judgment — a script outputs a ranked, tagged, difficulty-estimated list.

- [ ] **Assembly Classifier Script** (`tools/scripts/classify_targets.py`)
  - Scans all unmatched `.s` files and tags each with structural signatures:
    - `break` instruction → SN Systems wrapper (trivial)
    - `ctc2`/`mtc2`/`mfc2` → GTE coprocessor routine (inline asm)
    - Tight `lui`/`lhu` loop with no `jal` → hardware spin loop (likely unmatchable)
    - `jal` to unmatched symbols → blocked by dependencies (defer)
    - Line count < 10, no `jal`, no loops → trivial leaf (high priority)
    - Contains `goto`-signature branch polarity → needs flat goto treatment
  - Outputs a scored priority list combining: reference count × inverse difficulty × module completion %.

- [ ] **Function Status Registry** (`ai_workflow/function_registry.json`)
  - Single source of truth mapping every function to its status: `untouched`, `in_progress`, `matched`, `attempted_failed`, `unmatchable`.
  - Includes metadata: attempt count, blocking reason, relevant KB pattern IDs, assigned difficulty.
  - Replaces the need to cross-reference call graph reports, `progress.json`, and journal entries.
  - Updated automatically by the post-match reflection skill and manually for failed/unmatchable functions.

**Success Criteria:** Running one command produces a ranked target list with difficulty tags. An agent never picks a blocked or previously-failed target by accident.

---

### Tier 3: Scale the Knowledge System
**Goal:** Any question about "what do we know about X?" is answerable from the KB alone, without searching journals.

- [ ] **Cross-Reference Links**
  - KB patterns gain a `"used_in"` field listing functions where they were applied.
  - KB patterns gain a `"discovered_in"` field linking to the journal entry where they were first identified.
  - Journals gain a `"patterns_applied"` front-matter field listing KB pattern IDs.
  - The function registry links each matched function to the patterns that solved it.

- [ ] **Promote Buried Journal Insights to KB**
  - The `memchr` goto-reduction table (which goto controls what) → new KB entry.
  - The `SoundWaitSpuTransfer` impossibility finding → new "anti-pattern" KB category.
  - The `maspsx` decimal-only offset constraint → new toolchain heuristic.
  - Any other actionable findings currently only in journal narrative.

- [ ] **Anti-Pattern / Graveyard Registry**
  - A dedicated section (or file) documenting functions that were attempted and abandoned, with the specific reason.
  - Prevents future agents from re-attempting known-impossible targets.
  - Feeds into the classifier's "unmatchable" tag.

- [ ] **Topical Journal Index**
  - Group journal entries by subject (delay slots, goto manipulation, SDK wrappers, toolchain issues, sound system, etc.) in addition to chronological order.
  - Could be a section in TOC.md or a separate index file.

**Success Criteria:** A new agent can answer "what techniques exist for delay slot matching?" or "has anyone tried to decompile func_X before?" from structured data in under 30 seconds.

---

### Tier 4: Prove Self-Improvement
**Goal:** We can demonstrate with data that the workflow is objectively getting faster and more effective over time.

- [ ] **Workflow Metrics Tracking** (`ai_workflow/metrics.csv` or `.jsonl`)
  - Per-session metrics: functions matched, functions attempted, success rate, total iterations, KB entries added, skills created/updated.
  - Per-function metrics (from outcome_log): iterations to match, time estimate, difficulty rating, patterns used.
  - Append-only, timestamped for trend analysis.

- [ ] **Metrics Summary in TOC Dashboard**
  - Add a "Workflow Health" section to `TOC.md` showing:
    - Cumulative functions matched vs. total.
    - Rolling average iterations-to-match (trending down = workflow improving).
    - Most-used KB patterns (validates which knowledge is highest-value).
    - Functions in "attempted_failed" or "unmatchable" status (backlog visibility).

- [ ] **Periodic Retrospectives**
  - After every ~10 matched functions or at natural breakpoints, write a brief retrospective journal entry evaluating: Did the decision tree help? Were outcome records useful? What's the biggest remaining bottleneck?
  - Feed retrospective findings back into this roadmap as new tier items or priority adjustments.

**Success Criteria:** A chart (or table) exists showing iteration counts and success rates improving over time. The workflow can point to concrete evidence that it is learning.

---

### Tier 5: Autonomous Orchestration (Future)
**Goal:** The AI can run extended decompilation sessions with minimal human oversight.

- [ ] **`scripts/auto_decomp.py`** — Central orchestration script that:
  1. Runs the classifier to get a ranked target list.
  2. Picks the highest-priority unblocked target.
  3. Reads the function's assembly and applies the pattern decision tree.
  4. Generates candidate C code.
  5. Runs the micro-build loop, evaluating objdiff output.
  6. On match: executes the post-match reflection checklist automatically.
  7. On failure after N iterations: logs the attempt in the outcome log, marks the function as `attempted_failed`, and moves to the next target.

- [ ] **Diff Interpretation Engine** *(foundation built: `tools/asmdiff.py`)*
  - `asmdiff.py` provides structured, per-instruction diffs with relocation masking, match scoring, and color output. Run via `python3 tools/asmdiff.py <Func>` or `make asmdiff FUNC=<Func>`.
  - Next step: extend `asmdiff.py` to classify mismatch types programmatically (wrong register, wrong branch polarity, instruction rotation, extra/missing instruction) and suggest KB patterns.
  - Maps mismatch types to KB patterns ("wrong branch polarity" → suggest goto manipulation).

- [ ] **Session Planner**
  - At session start, reads the function registry + classifier output + outcome log.
  - Produces a session plan: "Today, attempt these 5 functions in this order, using these patterns. Estimated difficulty: 3 easy, 1 medium, 1 hard."

**Success Criteria:** The orchestrator can match trivial leaf functions (< 10 ASM lines, no loops, no branches) fully autonomously, end-to-end, without human intervention.

---

## Revision History

| Date | Change | Rationale |
| :--- | :--- | :--- |
| 2026-03-13 | Initial creation | Formalized workflow evolution plan from comprehensive review of journals, KB, and meta-workflow gaps. Established 5-tier improvement roadmap with measurable success criteria. |
| 2026-03-14 | Tier 1: Added readability pass + symbol rename skill | GpuPackage struct session revealed that readability refactoring is a distinct high-value workflow phase not captured in the post-match checklist. Added step 6 to reflection template, new Symbol Rename & Struct Recovery skill item, and identified the missing "readability checkpoint" gap in the maturity assessment. |
| 2026-03-14 | Expanded struct recovery skill + human-AI insight | Three iterative refinement passes on GpuPackage proved that readability benefits from human-AI dialogue. Expanded Symbol Rename skill with quality ladder (named → descriptive → typed → documented), typed-vs-polymorphic analysis step, and noted that autonomous readability passes may need human-in-the-loop for design taste. |
| 2026-03-14 | Tier 1: Added mismatch diagnosis layer to Pattern Decision Tree | ClearImage2 session (5 iterations, 4 wasted) proved that the biggest time sink is post-attempt mismatch diagnosis, not pre-attempt classification. Added two-layer decision tree spec (pre-attempt + mismatch diagnosis) with concrete symptom→fix examples. Added "no mismatch-to-fix mapping" to the gaps assessment. Updated KB pattern count to 16. |
| 2026-03-14 | Tier 1: Completed Post-Match Reflection + Outcome Records | Created post_match_reflection skill (6-step success path, 3-step failure path, batch shortcut). Created outcome_log.jsonl with ~45 retroactive entries from all journals. Updated AGENT.md to point to skill (replaces old steps 4–6). 3 of 4 Tier 1 items now complete. |
| 2026-03-14 | Tier 5: Built asmdiff.py — foundation for Diff Interpretation Engine | Created `tools/asmdiff.py`: CLI tool for per-instruction diff between target .s and compiled dump. Auto-discovers files by function name, handles MIPS relocations (masks address fields for I-type/J-type), color-coded output with match scoring. Added `make asmdiff FUNC=X` Makefile target. Updated AGENT.md Micro-Build loop to reference asmdiff as the primary diff workflow. |
| 2026-03-14 | MoveImage matched + KB enriched with failure analysis | MoveImage 100% matched after 25+ attempts. Added 4 new KB patterns: inline asm scheduling control (018), barrier dosage (019), scheduling escalation ladder (020), comprehensive failure catalog (anti_pattern_003). Added Scheduling Escalation Ladder directive to AGENT.md. Enriched Pattern Decision Tree mismatch→fix examples with delay slot scheduling, branch polarity (inverted if-body), and type signedness entries. KB now has 21 patterns. |
