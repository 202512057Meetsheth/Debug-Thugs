# Code Smells Audit: lab1-head

1. **Primitive Obsession & Data Clumps** (Confidence: C4 - Certain, Severity: HIGH)
   - **Location**: `part52.cpp:160-164` (State variables)
   - **Reason**: The primitive obsession from `main` exploded. We now have `snake2`, `dir2`, `pending_dir2`, `dir_changed2`, and `score2` floating loosely in the class alongside Player 1's variables. This is a severe failure of encapsulation.

2. **Duplicate Code** (Confidence: C4 - Certain, Severity: HIGH)
   - **Location**: `part52.cpp:200` (`step()`) and `part52.cpp:300` (`drawSmooth()`)
   - **Reason**: The logic for moving the snake head, checking wall collisions, checking obstacle collisions, and growing is copy-pasted almost identically for `snake` and `snake2`. The rendering loop to draw the body segments is also duplicated.

3. **Long Method** (Confidence: C4 - Certain, Severity: HIGH)
   - **Location**: `part52.cpp:200` (`step()`)
   - **Reason**: `step()` has grown massively to accommodate two separate sets of collision detection, movement logic, and tie-breaking, making it difficult to read and maintain.

4. **Shotgun Surgery** (Confidence: C3 - High, Severity: MEDIUM)
   - **Location**: Entire `CyberSnake` class
   - **Reason**: Adding a third player would require touching the class fields, `init()`, `step()`, `handleInput()`, and `drawSmooth()`, manually copying variables and if-statements in 5 different places.

5. **Large Class** (Confidence: C4 - Certain, Severity: HIGH)
   - **Location**: `part52.cpp:114` (`class CyberSnake`)
   - **Reason**: The class is now a massive blob containing the entire game simulation for two players, input parsing for two players, and UI rendering.
