/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 4
 */
#include "backtracking.h"
#include "hashset.h"
#include "SimpleTest.h"
using namespace std;

/*
 * Parameters: board is the current board; moveHistory holds the chosen moves
 *             visited stores board encodings already tried in this search
 * Returns: true if a winning path exists, adding its moves to moveHistory
 * Preconditions: moveHistory matches the path to board; visited belongs to this search
 */
bool solvePuzzleHelper(MarbleBoard& board, Vector<Move>& moveHistory,
                       HashSet<BoardEncoding>& visited) {
    if (board.isSolved()) {
        return true;
    }
    BoardEncoding encoding = board.encode();
    if (visited.contains(encoding)) {
        return false;
    }
    visited.add(encoding);
    for (Move move : board.findPossibleMoves()) {
        board.makeMove(move);
        moveHistory.add(move);
        bool solved = solvePuzzleHelper(board, moveHistory, visited);
        board.undoMove(move);
        if (solved) {
            return true;
        }
        moveHistory.remove(moveHistory.size() - 1);
    }
    return false;
}

/*
 * Parameters: board is the puzzle to solve; moveHistory receives the winning moves
 * Returns: true if the puzzle can be solved, keeping board unchanged
 * Preconditions: moveHistory is empty
 */
bool solvePuzzle(MarbleBoard& board, Vector<Move>& moveHistory) {
    HashSet<BoardEncoding> visited;
    return solvePuzzleHelper(board, moveHistory, visited);
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("solvePuzzle: one marble in the center is already solved") {
    MarbleBoard board(loadBoard("marbles/boards/solved.txt"));
    Vector<Move> moves;
    EXPECT(solvePuzzle(board, moves));
    EXPECT_EQUAL(moves.size(), 0);
}

PROVIDED_TEST("solvePuzzle: two marbles are solved with one jump") {
    Grid<Marble> original = loadBoard("marbles/boards/one-jump.txt");
    MarbleBoard board(original);
    Vector<Move> moves;
    EXPECT(solvePuzzle(board, moves));
    EXPECT_EQUAL(moves.size(), 1);
    EXPECT_EQUAL(checkSolution(original, moves), "");
}


PROVIDED_TEST("solvePuzzle: an unsolvable board returns false and is left unchanged") {
    MarbleBoard board(loadBoard("marbles/boards/five-plus.txt"));
    MarbleBoard original = board;
    Vector<Move> moves;
    EXPECT(!solvePuzzle(board, moves));
    EXPECT_EQUAL(board, original);
    EXPECT_EQUAL(board.numMarbles(), original.numMarbles());
}

PROVIDED_TEST("solvePuzzle: solvable mid-size boards") {
    for (string name : { "cross", "seven-step", "plus", "double-cross"}) {
        Grid<Marble> original = loadBoard("marbles/boards/" + name + ".txt");
        MarbleBoard board(original);
        Vector<Move> moves;
        EXPECT(solvePuzzle(board, moves));
        EXPECT_EQUAL(checkSolution(original, moves), "");
    }
}

PROVIDED_TEST("solvePuzzle: mid-size unsolvable board") {
    MarbleBoard board(loadBoard("marbles/boards/tangle.txt"));
    Vector<Move> moves;
    EXPECT(!solvePuzzle(board, moves));
}

PROVIDED_TEST("solvePuzzle: the full 32-marble board") {
    MarbleBoard board;
    Vector<Move> moves;
    bool solved = false;
    TIME_OPERATION(32, solved = solvePuzzle(board, moves));
    EXPECT(solved);
    EXPECT_EQUAL(checkSolution(fullBoard(), moves), "");
}

PROVIDED_TEST("solvePuzzle: unsolvable 15-marble board (needs the explored-boards optimization)") {
    // A solver that does not skip boards it has already explored will take
    // a very, very long time on this test.
    MarbleBoard board(loadBoard("marbles/boards/letter-l.txt"));
    Vector<Move> moves;
    bool solved = true;
    TIME_OPERATION(15, solved = solvePuzzle(board, moves));
    EXPECT(!solved);
}


STUDENT_TEST("A lone marble away from the center is also solved") {
    MarbleBoard board(loadBoard("marbles/boards/off-center.txt"));
    Vector<Move> moves;
    EXPECT(solvePuzzle(board, moves));
    EXPECT(moves.isEmpty());
}

STUDENT_TEST("Solver restores the board after success and can solve it again") {
    MarbleBoard board(loadBoard("marbles/boards/three-step.txt"));
    MarbleBoard original = board;
    for (int i = 0; i < 2; i++) {
        Vector<Move> moves;
        EXPECT(solvePuzzle(board, moves));
        EXPECT_EQUAL(checkSolution(original.toGrid(), moves), "");
        EXPECT_EQUAL(moves.size(), original.numMarbles() - 1);
        EXPECT_EQUAL(board, original);
        EXPECT_EQUAL(board.numMarbles(), original.numMarbles());
    }
}
