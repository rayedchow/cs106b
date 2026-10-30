/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 4
 */
#include "backtracking.h"
#include "grid.h"
#include "gridgraphics.h"
#include "SimpleTest.h"
using namespace std;

/*
 * Parameters: g is the population grid; range selects the cells to add
 * Returns: total population in range
 * Preconditions: range is inside g
 */
int subgridSum(Grid<int>& g, GridLocationRange range) {
    int total = 0;
    for (GridLocation location : range) {
        total += g[location];
    }
    return total;
}

/*
* Parameters: range is the area to split; row is the last row of topPart
*             topPart and bottomPart receive the two ranges
* Preconditions: range.startRow() <= row < range.endRow()
* 
* splits range horizontally without copying cells
 */
void divideHorizontal(GridLocationRange range, int row,
                      GridLocationRange& topPart, GridLocationRange& bottomPart) {
    topPart = GridLocationRange(range.startRow(), range.startCol(), row, range.endCol());
    bottomPart = GridLocationRange(row + 1, range.startCol(), range.endRow(), range.endCol());
}

/*
 * Parameters: range is the area to split; col is the last column of leftSide
 *             leftSide and rightSide receive the two ranges
 * Preconditions: range.startCol() <= col < range.endCol()
 * 
 * splits range vertically without copying cells
 */
void divideVertical(GridLocationRange range, int col,
                    GridLocationRange& leftSide, GridLocationRange& rightSide) {
    leftSide = GridLocationRange(range.startRow(), range.startCol(), range.endRow(), col);
    rightSide = GridLocationRange(range.startRow(), col + 1, range.endRow(), range.endCol());
}

/*
 * Parameters: grid holds populations; range is the current area; target is people per district
 * Returns: true if range can be split into districts of target people
 * Preconditions: range is inside grid, populations are nonnegative, and target > 0
 */
bool canRedistrictRange(Grid<int>& grid, GridLocationRange range, int target) {
    int population = subgridSum(grid, range);
    if (population == target) {
        return true;
    }
    if (population < target || population % target != 0) {
        return false;
    }
    GridLocationRange first, second;
    for (int row = range.startRow(); row < range.endRow(); row++) {
        divideHorizontal(range, row, first, second);
        if (canRedistrictRange(grid, first, target)
            && canRedistrictRange(grid, second, target)) {
            return true;
        }
    }
    for (int col = range.startCol(); col < range.endCol(); col++) {
        divideVertical(range, col, first, second);
        if (canRedistrictRange(grid, first, target)
            && canRedistrictRange(grid, second, target)) {
            return true;
        }
    }
    return false;
}

/*
 * Parameters: populationGrid holds populations; nDistricts is the number of districts
 * Returns: true if horizontal and vertical cuts can make equal-population districts
 * Preconditions: populations >= 0, total > 0, nDistricts > 0, and total % nDistricts == 0
 */
bool canRedistrict(Grid<int>& populationGrid, int nDistricts) {
    int target = subgridSum(populationGrid, populationGrid.locations()) / nDistricts;
    return canRedistrictRange(populationGrid, populationGrid.locations(), target);
}

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("subgridSum handles zero cells and an offset rectangle") {
    Grid<int> grid = {{0, 2, 3}, {4, 0, 6}, {7, 8, 9}};
    EXPECT_EQUAL(subgridSum(grid, GridLocationRange(0, 0, 0, 0)), 0);
    EXPECT_EQUAL(subgridSum(grid, GridLocationRange(1, 1, 2, 2)), 23);
    EXPECT_EQUAL(subgridSum(grid, grid.locations()), 39);
}

STUDENT_TEST("Horizontal cuts include the correct rows at both ends") {
    GridLocationRange range(2, 3, 5, 7), top, bottom;
    for (int row = 2; row < 5; row++) {
        divideHorizontal(range, row, top, bottom);
        EXPECT_EQUAL(top.startLocation(), range.startLocation());
        EXPECT_EQUAL(top.endLocation(), (GridLocation{row, 7}));
        EXPECT_EQUAL(bottom.startLocation(), (GridLocation{row + 1, 3}));
        EXPECT_EQUAL(bottom.endLocation(), range.endLocation());
        EXPECT_EQUAL(top.size() + bottom.size(), range.size());
    }
}

STUDENT_TEST("Vertical cuts include the correct columns at both ends") {
    GridLocationRange range(2, 3, 5, 7), left, right;
    for (int col = 3; col < 7; col++) {
        divideVertical(range, col, left, right);
        EXPECT_EQUAL(left.startLocation(), range.startLocation());
        EXPECT_EQUAL(left.endLocation(), (GridLocation{5, col}));
        EXPECT_EQUAL(right.startLocation(), (GridLocation{2, col + 1}));
        EXPECT_EQUAL(right.endLocation(), range.endLocation());
        EXPECT_EQUAL(left.size() + right.size(), range.size());
    }
}

STUDENT_TEST("Single district and one district per cell") {
    Grid<int> single = {{12}};
    EXPECT(canRedistrict(single, 1));
    EXPECT(!canRedistrict(single, 2));
    Grid<int> uniform = {{2, 2, 2}, {2, 2, 2}};
    EXPECT(canRedistrict(uniform, 1));
    EXPECT(canRedistrict(uniform, 6));
}

STUDENT_TEST("Single rows and columns, including zero-population cells") {
    Grid<int> row = {{0, 2, 0, 2, 0}};
    Grid<int> col = {{0}, {2}, {0}, {2}, {0}};
    EXPECT(canRedistrict(row, 2));
    EXPECT(canRedistrict(col, 2));
    Grid<int> blocked = {{1, 4, 1}};
    EXPECT(!canRedistrict(blocked, 2));
}

STUDENT_TEST("Search tries later cuts and leaves populations unchanged") {
    Grid<int> grid = {{1, 1, 2}, {1, 1, 2}};
    Grid<int> original = grid;
    EXPECT(canRedistrict(grid, 4));
    EXPECT_EQUAL(grid, original);
    Grid<int> zeroEdge = {{0, 0}, {2, 2}};
    EXPECT(canRedistrict(zeroEdge, 2));
}

/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */


PROVIDED_TEST("subgridSum sums values in grid range") {
    // This is an example test of helper function subgridSum
    Grid<int> grid = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 10, 10} };
    GridLocationRange all = grid.locations();
    GridLocationRange firstColumn = GridLocationRange(0, 0, 3, 0);
    GridLocationRange bottomCorner = GridLocationRange(3, 2, 3, 2);

    EXPECT_EQUAL(subgridSum(grid, firstColumn), 22);
    EXPECT_EQUAL(subgridSum(grid, bottomCorner), 10);
    EXPECT_EQUAL(subgridSum(grid, all), 75);
}

PROVIDED_TEST("nevada example from writeup can be divided into 4 districts") {
    Grid<int> nevada = { {20, 31, 20},
                         { 5, 19, 30},
                         {25, 25, 25} };

    initForPopulationGrid(nevada); // you can remove if you don't wish to animate this test case
    EXPECT(canRedistrict(nevada, 4));
    waitForCloseWindow();
}

PROVIDED_TEST("wyoming example from writeup cannot be divided into 2 districts") {
    Grid<int> wyoming = { {10, 15},
                          { 5, 10} };

    initForPopulationGrid(wyoming); // you can remove if you don't wish to animate this test case
    EXPECT(!canRedistrict(wyoming, 2));
    waitForCloseWindow();
}

PROVIDED_TEST("small example, 5 districts") {
    Grid<int> small = { {3, 7, 10},
                        {6, 3, 1},
                        {1, 10, 9} };
    initForPopulationGrid(small);  // you can remove if you don't wish to animate this test case
    EXPECT(canRedistrict(small, 5));
    waitForCloseWindow();
}

PROVIDED_TEST("medium example, 12 districts") {
    Grid<int> medium = { {10, 1,  2, 5, 1, 4},
                          {1, 1,  8, 2, 1, 7},
                          {9, 8, 10, 3, 3, 4},
                          {5, 2,  2, 8, 7, 3},
                          {2, 1,  1, 7, 1, 1} };
    initForPopulationGrid(medium);  // you can remove if you don't wish to animate this test case
    EXPECT(canRedistrict(medium, 12));
    waitForCloseWindow();
}

PROVIDED_TEST("large example can be divided into 25 districts") {
    Grid<int> large = { { 5,  2, 2,  1,  6, 4, 8, 2},
                        {10, 10, 2,  1,  3, 4, 2, 3},
                        { 1,  1, 4,  5,  1, 9, 1, 4},
                        { 9,  5, 5, 10, 10, 3, 3, 4},
                        { 3,  1, 1,  1,  2, 8, 6, 2},
                        { 2,  1, 1,  7,  3, 5, 1, 1},
                        { 1,  1, 2,  2,  7, 5, 1, 9},
                        { 4,  1, 2, 10,  1, 9, 5, 5} };
    initForPopulationGrid(large); // you can remove if you don't wish to animate this test case
    EXPECT(canRedistrict(large, 25));
    waitForCloseWindow();
}
