# Code Smells Audit: main

1. **Large Class** (Confidence: C4 - Certain, Severity: HIGH)
   - **Location**: `part52.cpp:114` (`class CyberSnake`)
   - **Reason**: The class is over 150 lines and violates the Single Responsibility Principle. It handles game state, rendering (`drawSmooth`), input parsing (`handleInput`), and random generation all in one giant class.

2. **Primitive Obsession** (Confidence: C3 - High, Severity: MEDIUM)
   - **Location**: `part52.cpp:152-156` (State variables)
   - **Reason**: The concept of a "Snake" or "Player" is scattered across raw primitives: `deque<Pt> snake`, `Dir dir`, `int score`, `bool alive`. They should be grouped into a `Snake` class or struct.

3. **Long Method** (Confidence: C4 - Certain, Severity: HIGH)
   - **Location**: `part52.cpp:209` (`drawSmooth()`)
   - **Reason**: The method is 49 lines long and deeply nested (5 levels of indentation). It mixes UI layout logic, terminal escape codes, and game state querying.

4. **Duplicate Code** (Confidence: C4 - Certain, Severity: HIGH)
   - **Location**: `part52.cpp:165` and `part52.cpp:174`
   - **Reason**: The exact same 3 lines of code used to calculate empty grid cells `vector<Pt>e; vector<vector<char>>u(...)` are duplicated identically in both `placeFruit()` and `maybeAddObstacle()`.

5. **Data Clumps** (Confidence: C2 - Medium, Severity: LOW)
   - **Location**: `part52.cpp:112` (`struct Fruit`)
   - **Reason**: Fruit only groups a `Pt` and `type`. This is fine for a simple game, but as it grows, `type` and `Pt` often travel together and could benefit from methods like `getScoreValue()`.
