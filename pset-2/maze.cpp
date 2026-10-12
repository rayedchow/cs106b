/*
 * CS106B Assignment 2 — Maze helpers and solvers
 * Rayed Chowdhury, Jun Jang
 */
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


/* Returns in-bounds, open orthogonal neighbors of cur (N, S, E, W).
 * Assumes cur is a valid maze location; never returns diagonal moves.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    Vector<GridLocation> candidates = {
        {cur.row - 1, cur.col}, {cur.row + 1, cur.col},
        {cur.row, cur.col + 1}, {cur.row, cur.col - 1}
    };

    for (GridLocation candidate : candidates) {
        if (maze.inBounds(candidate) && maze[candidate]) {
            neighbors.add(candidate);
        }
    }
    return neighbors;
}

/* Checks that path is a nonempty, non-repeating route from the
 * upper-left entrance to the lower-right exit through open corridors.
 * Reports malformed paths using error(); otherwise returns normally.
 */
void validatePath(Grid<bool>& maze, Vector<GridLocation>& path) {
    if (path.isEmpty()) {
        error("Path is empty!");
    }

    GridLocation entrance = {0, 0};
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    if (path[0] != entrance) {
        error("Path does not start at the entrance!");
    }
    if (path[path.size() - 1] != exit) {
        error("Path does not end at the exit!");
    }
    if (!maze[entrance]) {
        error("The entrance is a wall!");
    }

    Set<GridLocation> visited = {entrance};
    for (int i = 1; i < path.size(); i++) {
        if (!generateValidMoves(maze, path[i - 1]).contains(path[i])) {
            error("Path takes an invalid step!");
        }
        if (visited.contains(path[i])) {
            error("Path repeats a location!");
        }
        visited.add(path[i]);
    }
}

/* Uses breadth-first search to find a shortest entrance-to-exit path.
 * Assumes a rectangular maze with open corners and an existing solution.
 * Tracks visited cells when enqueuing to avoid duplicate/cyclic paths.
 */
Vector<GridLocation> solveMazeBFS(Grid<bool>& maze) {
    GridLocation entrance = {0, 0};
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    Queue<Vector<GridLocation>> allPaths;
    Set<GridLocation> visited = {entrance};
    Vector<GridLocation> initialPath = {entrance};
    allPaths.enqueue(initialPath);

    while (!allPaths.isEmpty()) {
        Vector<GridLocation> path = allPaths.dequeue();
        GridLocation tail = path[path.size() - 1];
        if (tail == exit) {
            return path;
        }

        for (GridLocation neighbor : generateValidMoves(maze, tail)) {
            if (!visited.contains(neighbor)) {
                visited.add(neighbor);
                Vector<GridLocation> extended = path;
                extended.add(neighbor);
                allPaths.enqueue(extended);
            }
        }
    }
    error("No path exists from the maze entrance to its exit.");
    return {};
}

/* Uses a stack to find a valid entrance-to-exit path by DFS.
 * Assumes a rectangular maze with open corners and an existing solution.
 * Tracks visited cells when pushing to avoid duplicate/cyclic paths.
 */
Vector<GridLocation> solveMazeDFS(Grid<bool>& maze) {
    GridLocation entrance = {0, 0};
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    Stack<Vector<GridLocation>> allPaths;
    Set<GridLocation> visited = {entrance};
    Vector<GridLocation> initialPath = {entrance};
    allPaths.push(initialPath);

    while (!allPaths.isEmpty()) {
        Vector<GridLocation> path = allPaths.pop();
        GridLocation tail = path[path.size() - 1];
        if (tail == exit) {
            return path;
        }

        for (GridLocation neighbor : generateValidMoves(maze, tail)) {
            if (!visited.contains(neighbor)) {
                visited.add(neighbor);
                Vector<GridLocation> extended = path;
                extended.add(neighbor);
                allPaths.push(extended);
            }
        }
    }
    error("No path exists from the maze entrance to its exit.");
    return {};
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines = stringSplit(readEntire(in), '\n');

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Vector<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution, hand-constructed maze") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Vector<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution read from file, medium maze") {
    Grid<bool> maze;
    Vector<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true},
                       {true, true}};
    Vector<GridLocation> notBeginAtEntry = { {1, 1}, {2, 1} };
    Vector<GridLocation> notEndAtExit = { {0, 0}, {1, 0}, {2, 0} };
    Vector<GridLocation> moveThroughWall = { {0 ,0}, {0, 1}, {1, 1}, {2, 1} };

    EXPECT_ERROR(validatePath(maze, notBeginAtEntry));
    EXPECT_ERROR(validatePath(maze, notEndAtExit));
    EXPECT_ERROR(validatePath(maze, moveThroughWall));
}

PROVIDED_TEST("solveMazeBFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}



STUDENT_TEST("generateValidMoves excludes walls at a boundary") {
    Grid<bool> maze = {{true, false, true},
                       {false, true, true},
                       {true, true, true}};
    EXPECT(generateValidMoves(maze, {0, 0}).isEmpty());
}

STUDENT_TEST("generateValidMoves does not include diagonal cells") {
    Grid<bool> maze = {{true, false}, {false, true}};
    EXPECT(generateValidMoves(maze, {0, 0}).isEmpty());
}

STUDENT_TEST("generateValidMoves returns all open neighbors") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};
    EXPECT_EQUAL(generateValidMoves(maze, {1, 1}), expected);
}

STUDENT_TEST("validatePath rejects empty and wrong endpoints") {
    Grid<bool> maze = {{true, true}, {true, true}};
    Vector<GridLocation> empty;
    Vector<GridLocation> wrongStart = {{0, 1}, {1, 1}};
    Vector<GridLocation> wrongEnd = {{0, 0}, {1, 0}};
    EXPECT_ERROR(validatePath(maze, empty));
    EXPECT_ERROR(validatePath(maze, wrongStart));
    EXPECT_ERROR(validatePath(maze, wrongEnd));
}

STUDENT_TEST("validatePath rejects repeated start and nonadjacent moves") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    Vector<GridLocation> loop = {{0, 0}, {0, 1}, {0, 0},
                                 {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    Vector<GridLocation> jump = {{0, 0}, {1, 1}, {2, 2}};
    EXPECT_ERROR(validatePath(maze, loop));
    EXPECT_ERROR(validatePath(maze, jump));
}

STUDENT_TEST("validatePath accepts a valid winding route") {
    Grid<bool> maze = {{true, true, true},
                       {false, false, true},
                       {true, true, true}};
    Vector<GridLocation> path = {{0, 0}, {0, 1}, {0, 2},
                                 {1, 2}, {2, 2}};
    EXPECT_NO_ERROR(validatePath(maze, path));
}

STUDENT_TEST("solveMazeBFS finds the shortest path on an open grid") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    Vector<GridLocation> path = solveMazeBFS(maze);
    EXPECT_NO_ERROR(validatePath(maze, path));
    EXPECT_EQUAL(path.size(), 5);
}

STUDENT_TEST("solveMazeBFS solves a winding route") {
    Grid<bool> maze = {{true, true, true, false},
                       {false, false, true, false},
                       {true, true, true, true},
                       {true, false, false, true}};
    Vector<GridLocation> path = solveMazeBFS(maze);
    EXPECT_NO_ERROR(validatePath(maze, path));
    EXPECT_EQUAL(path.size(), 7);
}

STUDENT_TEST("solveMazeDFS produces a loop-free valid route") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    Vector<GridLocation> path = solveMazeDFS(maze);
    EXPECT_NO_ERROR(validatePath(maze, path));
}
