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
| **Data Clumps / Primitive Obsession** | 1 (`snake2`, `dir2`, etc.) | 1 (`snake`, `dir`) | 0 |
| **Duplicate Code** | 2 (`step()`, `drawSmooth()`) | 1 (`placeFruit()`) | 0 |
| **Long Method** | 1 (`step()`) | 1 (`drawSmooth()`) | 0 |
| **Shotgun Surgery** | 1 (Adding players) | 0 | 0 |
| **Large Class** | 0 | 1 (`CyberSnake`) | 0 |

## 4. Rejected Candidates

| Smell | File:Line | Why we rejected it |
| :--- | :--- | :--- |
| **Large Class** | `part52.cpp:114` | At only ~200 lines, splitting `CyberSnake` into separate Input, Render, and Engine classes is severe over-engineering and Speculative Generality. |
| **Long Method** | `part52.cpp:209` | `drawSmooth` is 49 lines, but it's a single, highly cohesive string-buffering pipeline; splitting it would break the top-to-bottom mental model of the screen rendering. |
| **Data Clumps** | `part52.cpp:112` | The `Fruit` struct groups `Pt` and `type`. While it's a clump, upgrading it to a full Class with methods is unnecessary since it has no internal behavior. |

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
