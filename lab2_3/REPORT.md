# Lab 2.3 Report

## 1. Setup

**Tool:** Gemini 3.1 Pro (via Antigravity IDE)
**Install Route:** Paste fallback / direct URL read (The agent autonomously read the raw content of the SKILL.md files from the IT643 GitHub repository URL, since the local skill engine paths were abstracted).

## 2. Ubiquitous Language

**What we changed and why:**
We edited the AI-generated glossary to remove generic programmatic terms (like `Pt`, `Dir`, and `deque`) that the AI mistakenly flagged as domain terminology. We enforced that "Board" is the domain term (even though the code only uses `rows` and `cols`), and flagged `alive = false` as an implicit "Collision", which is the true domain event.

## 3. Smell Delta Table

| Smell | Introduced | Untouched | Removed |
| :--- | :--- | :--- | :--- |
| **Large Class** |  |  |  |
| **Primitive Obsession** |  |  |  |
| **Data Clumps** |  |  |  |
| **Duplicate Code** |  |  |  |
| **Feature Envy** |  |  |  |

## 4. Rejected Candidates

| Smell | File:Line | Why we rejected it |
| :--- | :--- | :--- |
| (To be filled) |  |  |
| (To be filled) |  |  |
| (To be filled) |  |  |

## 5. Commits

*(To be pasted from the checker script output)*

## 6. Measurement

| Metric | Run 1 (Lab 1) | Run 2 (Design-first) |
| :--- | :--- | :--- |
| **Feature: files changed** | 1 | (To be filled) |
| **Feature: lines added/deleted** | 96+ 23- | (To be filled) |
| **Feature: functions reached** | 7 | (To be filled) |
| **Refactor: lines added/deleted** | N/A | (To be filled) |

## 7. Analysis: The Refactor

**Which smell did commit 3 actually fix?**
(To be filled after refactoring)

**Compare commit 4 to your Lab-1 diff. What changed in the cost, and what did not?**
(To be filled after refactoring)

## 8. Analysis: The Process

**Did the assistant suggest restructuring before adding the feature in Lab 1?**
(To be filled after refactoring)

**How do you know commit 3 did not change behaviour?**
(To be filled after refactoring)
