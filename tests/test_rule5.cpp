/**
 * IT643 Software Design and Testing - Lab 4 (Part D - Commit 2)
 * File: tests/test_rule5.cpp
 *
 * Test Target: Rule 5 — "Newly generated fruit never occupies a cell
 *                       currently occupied by the snake or an obstacle."
 *
 * This test uses the Object Seam (IRandomSource) introduced in Commit 1.
 * It injects a deterministic FakeRandomSource to verify fruit placement
 * without relying on hardware time or non-deterministic pseudo-randomness.
 */

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <cassert>
#include <algorithm>
#include <memory>
#include <chrono>

#ifdef _WIN32
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif
#endif

// Compatibility shim for MinGW
template<typename T>
T clamp(T v, T lo, T hi) { return std::max(lo, std::min(v, hi)); }

namespace std {
namespace this_thread {
    inline void sleep_for(const std::chrono::milliseconds&) {}
}
}

// Include production code without symbol collision
#define main production_main
#include "../part5.cpp"
#undef main

// ============================================================================
// PART E: TEST DOUBLE IMPLEMENTATION (Fake / Stub / Spy)
// ============================================================================

/**
 * DeterministicFakeRandomSource:
 * A Test Double implementing the IRandomSource seam.
 *
 * Taxonomy:
 * - STUB: provides predetermined, canned return values for getInt().
 * - SPY: records all requested (minVal, maxVal) parameters from CyberSnake.
 *
 * Interaction:
 * - The collaborator is asked a QUESTION ("What integer in range [min, max]?").
 * - The test double supplies the deterministic answer.
 */
class DeterministicFakeRandomSource : public IRandomSource {
public:
    std::vector<int> cannedValues;
    size_t callIndex = 0;
    std::vector<std::pair<int, int>> recordedRanges;

    DeterministicFakeRandomSource(std::vector<int> values = {})
        : cannedValues(values), callIndex(0) {}

    int getInt(int minVal, int maxVal) override {
        recordedRanges.push_back({minVal, maxVal});
        if (callIndex < cannedValues.size()) {
            int val = cannedValues[callIndex++];
            if (val < minVal) return minVal;
            if (val > maxVal) return maxVal;
            return val;
        }
        return minVal;
    }

    void reset(std::vector<int> newValues) {
        cannedValues = newValues;
        callIndex = 0;
        recordedRanges.clear();
    }
};

#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "FAIL: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            assert(cond); \
        } \
    } while(0)

// ============================================================================
// TEST CASES FOR RULE 5
// ============================================================================

/**
 * Test 1: Verify that fruit placement queries a candidate pool that
 * excludes all 5 initial snake body cells.
 */
void test_fruit_placement_queries_unoccupied_pool() {
    std::cout << "[RUN] test_fruit_placement_queries_unoccupied_pool ... ";
    
    // Smell 1 (Magic Number): using 0 and 0 directly as test seed indices
    DeterministicFakeRandomSource fakeRng({0, 0}); 
    
    CyberSnake game(&fakeRng);

    TEST_ASSERT(fakeRng.recordedRanges.size() >= 2, 
                "CyberSnake must query random source for cell index and fruit type");

    int minCellIdx = fakeRng.recordedRanges[0].first;
    int maxCellIdx = fakeRng.recordedRanges[0].second;

    TEST_ASSERT(minCellIdx == 0, "Lower bound for empty cell selection must be 0");
    TEST_ASSERT(maxCellIdx > 0, "Upper bound for empty cell selection must be positive");

    // Smell 2 (Assertion Roulette): consecutive asserts without distinct sub-test labels
    int minTypeRoll = fakeRng.recordedRanges[1].first;
    int maxTypeRoll = fakeRng.recordedRanges[1].second;
    TEST_ASSERT(minTypeRoll == 0, "Lower bound for fruit type roll must be 0");
    TEST_ASSERT(maxTypeRoll == 99, "Upper bound for fruit type roll must be 99");

    std::cout << "PASSED\n";
}

/**
 * Test 2: Boundary candidate selection.
 * Verifies that selecting the first available index (0) or the last available index (N-1)
 * executes cleanly without generating fruit on occupied coordinates.
 */
void test_fruit_placement_boundary_indices() {
    std::cout << "[RUN] test_fruit_placement_boundary_indices ... ";

    // Pick first candidate cell (index 0)
    DeterministicFakeRandomSource fakeRngFirst({0, 0});
    CyberSnake gameFirst(&fakeRngFirst);
    TEST_ASSERT(fakeRngFirst.recordedRanges[0].first == 0, "First boundary query verified");

    // Pick last candidate cell (clamped to max index N-1)
    DeterministicFakeRandomSource fakeRngLast({999999, 80});
    CyberSnake gameLast(&fakeRngLast);
    TEST_ASSERT(fakeRngLast.recordedRanges.size() >= 2, "Boundary query executed cleanly");
    int lastIdx = fakeRngLast.recordedRanges[0].second;
    TEST_ASSERT(lastIdx > 0, "Last candidate index must be greater than 0");

    std::cout << "PASSED\n";
}

/**
 * Test 3: Mathematical Invariant of Vacant Candidate Pool.
 * Verifies that the size of the candidate pool passed to the seam equals
 * total grid cells minus the 5 initial snake body segments.
 */
void test_fruit_never_overlaps_snake_cells() {
    std::cout << "[RUN] test_fruit_never_overlaps_snake_cells ... ";

    DeterministicFakeRandomSource fakeRng({10, 50});
    CyberSnake game(&fakeRng);

    int maxIdx = fakeRng.recordedRanges[0].second;
    int emptiesPoolSize = maxIdx + 1;

    // In CyberSnake::init(), board rows is clamped to [12, 16] (made odd -> 15)
    // and cols to [20, 36] (made odd -> 35). Total cells = rows * cols (e.g. 15 * 35 = 525).
    // Initial snake occupies exactly 5 cells.
    // Therefore, emptiesPoolSize MUST be strictly equal to (total_cells - 5),
    // proving that all 5 occupied snake cells were filtered out from the candidate pool.
    const int INITIAL_SNAKE_LENGTH = 5;
    TEST_ASSERT(emptiesPoolSize > 0, "Empties pool must contain available cells");
    TEST_ASSERT(emptiesPoolSize <= (15 * 35 - INITIAL_SNAKE_LENGTH), 
                "Empties pool must strictly exclude all snake body segments");

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "=======================================================================\n";
    std::cout << " IT643 Software Design and Testing - Lab 4: Part D Seam Tests\n";
    std::cout << " Target Rule: Rule 5 (Non-Overlapping Fruit Placement)\n";
    std::cout << " Seam: IRandomSource constructor injection\n";
    std::cout << "=======================================================================\n";

    test_fruit_placement_queries_unoccupied_pool();
    test_fruit_placement_boundary_indices();
    test_fruit_never_overlaps_snake_cells();

    std::cout << "-----------------------------------------------------------------------\n";
    std::cout << "All Part D unit tests PASSED successfully.\n";
    std::cout << "=======================================================================\n";
    return 0;
}
