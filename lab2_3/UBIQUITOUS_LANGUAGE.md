# Ubiquitous Language

## Core Entities

| Term | Definition | Aliases to avoid | In code |
| :--- | :--- | :--- | :--- |
| **Snake** | The player-controlled entity that grows and moves around the grid. | Player | `snake` — `part5.cpp:115` |
| **Fruit** | The item the snake eats to gain points and grow. | Food, apple, star | `fruit` — `part5.cpp:125` |
| **Obstacle** | A fixed block that causes a collision and ends the game if hit. | Wall, block | `obstacles` — `part5.cpp:126` |
| **Board** | The bounded 2D grid area where the game takes place. | Grid, map | `rows`, `cols` — `part5.cpp:110` ⚠ |

## Game State & Lifecycle

| Term | Definition | Aliases to avoid | In code |
| :--- | :--- | :--- | :--- |
| **Tick** | A single unit of time update in the game loop. | Step, frame | `step()` — `part5.cpp:211` ⚠ |
| **Score** | Points earned by the player by eating fruits. | Points | `score` — `part5.cpp:118` |
| **Level** | The current difficulty stage, increasing after eating enough fruits. | Stage, difficulty | `level` — `part5.cpp:119` |
| **Collision** | When the snake hits an obstacle, the board edge, or itself, ending the game. | Crash, game over | `alive=false` — `part5.cpp:214` ⚠ |

## Example dialogue

> **Dev:** "When a **Snake** eats a **Fruit**, does the **Level** always increase?"
> **Domain expert:** "No, the **Score** increases immediately, but the **Level** only increases after every 4 **Fruits** eaten."
> **Dev:** "And if the **Snake** hits an **Obstacle**?"
> **Domain expert:** "That's a **Collision** — the game ends immediately."

## Flagged ambiguities

- The code uses raw `rows` and `cols` variables instead of a unified **Board** or **Grid** concept.
- The game's update cycle is called `step()` in the code, but it represents a game **Tick**.
- The code uses `alive = false` to indicate a **Collision** state, which is implicit rather than explicit.

## Code drift

| Canonical term | Called in code | Location | Note |
| :--- | :--- | :--- | :--- |
| **Board** | `rows`, `cols` | `part5.cpp:110` | Represented as disconnected primitive integers rather than a domain object. |
| **Tick** | `step()` | `part5.cpp:211` | Method name describes the action rather than the domain time unit. |
| **Collision** | `alive = false` | `part5.cpp:214` | The event of a collision is not explicitly modeled, only its effect (death). |
