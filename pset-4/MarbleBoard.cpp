/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 4
 */
#include "MarbleBoard.h"
#include "set.h"
#include "SimpleTest.h"
using namespace std;

/*
 * Returns: all legal moves on this board, checking the four hop directions
 * Preconditions: this board has a valid marble grid
 */
Vector<Move> MarbleBoard::findPossibleMoves() const {
    Vector<Move> moves;
    const Vector<GridLocation> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (GridLocation start : _grid.locations()) {
        if (_grid[start] == MARBLE_OCCUPIED) {
            for (GridLocation direction : directions) {
                GridLocation middle = {start.row + direction.row, start.col + direction.col};
                GridLocation end = {middle.row + direction.row, middle.col + direction.col};
                if (_grid.inBounds(middle) && _grid.inBounds(end)
                    && _grid[middle] == MARBLE_OCCUPIED && _grid[end] == MARBLE_EMPTY) {
                    moves.add({start, end});
                }
            }
        }
    }
    return moves;
}

/*
 * Parameters: move is the jump to make
 * Preconditions: move is legal on this board
 * 
 * moves the marble, removes the jumped marble, and updates the count
 */
void MarbleBoard::makeMove(Move move) {
    GridLocation middle = {(move.start.row + move.end.row) / 2,
                           (move.start.col + move.end.col) / 2};
    _grid[move.start] = MARBLE_EMPTY;
    _grid[middle] = MARBLE_EMPTY;
    _grid[move.end] = MARBLE_OCCUPIED;
    _numMarbles--;
}

/*
 * Parameters: move is the jump to undo
 * Preconditions: move was the most recent jump on this board
 * 
 * puts both marbles back and updates the count
 */
void MarbleBoard::undoMove(Move move) {
    GridLocation middle = {(move.start.row + move.end.row) / 2,
                           (move.start.col + move.end.col) / 2};
    _grid[move.start] = MARBLE_OCCUPIED;
    _grid[middle] = MARBLE_OCCUPIED;
    _grid[move.end] = MARBLE_EMPTY;
    _numMarbles++;
}

/* The functions below have already been written for you! */

MarbleBoard::MarbleBoard() : MarbleBoard(fullBoard()) {}   // default constructor creates the starting board

MarbleBoard::MarbleBoard(const Grid<Marble>& grid) {
    _grid = grid;
    _numMarbles = countMarbles(grid);
}

int MarbleBoard::numMarbles() const {
    return _numMarbles;
}

bool MarbleBoard::isSolved() const {
    return _numMarbles == 1;
}

Marble MarbleBoard::at(GridLocation loc) const {
    return _grid[loc];
}

BoardEncoding MarbleBoard::encode() const {
    return compressBoard(_grid);
}

Grid<Marble> MarbleBoard::toGrid() const {
    return _grid;
}

string MarbleBoard::toString() const {
    return boardToString(_grid);
}


/* * * * * * Test Cases * * * * * */

/*
 * Helper for the tests below: returns the moves as a Set, so that the tests
 * do not depend on the order in which findPossibleMoves returns its moves.
 */
static Set<Move> toSet(const Vector<Move>& moves) {
    Set<Move> result;
    for (const Move& move : moves) {
        result.add(move);
    }
    return result;
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: a lone marble has no moves") {
    MarbleBoard board(loadBoard("marbles/boards/solved.txt"));
    EXPECT_EQUAL(board.findPossibleMoves().size(), 0);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: two marbles side by side can jump either way") {
    MarbleBoard board(boardFromStrings({
        "XX---XX",
        "XX---XX",
        "-------",
        "-OO----",
        "-------",
        "XX---XX",
        "XX---XX"
    }));
    Vector<Move> moves = board.findPossibleMoves();
    Set<Move> expected = { { {3, 1}, {3, 3} },     // left marble jumps right
                           { {3, 2}, {3, 0} } };   // right marble jumps left
    EXPECT_EQUAL(moves.size(), 2);
    EXPECT_EQUAL(toSet(moves), expected);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: no jumping over empty holes, diagonally, off the board, or into corners") {
    // marbles with empty holes between them
    MarbleBoard spaced(boardFromStrings({
        "XXO-OXX",
        "XX---XX",
        "O-O-O-O",
        "-------",
        "O-O-O-O",
        "XX---XX",
        "XXO-OXX"
    }));
    EXPECT_EQUAL(spaced.findPossibleMoves().size(), 0);

    // marbles touching only diagonally
    MarbleBoard diagonals(loadBoard("marbles/boards/no-diagonals.txt"));
    EXPECT_EQUAL(diagonals.findPossibleMoves().size(), 0);

    // the only jumps in this line of marbles would land off the board or in a corner
    MarbleBoard edge(loadBoard("marbles/boards/off-the-edge.txt"));
    EXPECT_EQUAL(edge.findPossibleMoves().size(), 0);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: the full board has exactly four moves, all into the center") {
    MarbleBoard board;
    Vector<Move> moves = board.findPossibleMoves();
    Set<Move> expected = { { {1, 3}, {3, 3} }, { {5, 3}, {3, 3} },
                           { {3, 1}, {3, 3} }, { {3, 5}, {3, 3} } };
    EXPECT_EQUAL(moves.size(), 4);
    EXPECT_EQUAL(toSet(moves), expected);
}

PROVIDED_TEST("MarbleBoard::makeMove and undoMove: jump down into the center, then undo") {
    MarbleBoard board;
    Move move = { {1, 3}, {3, 3} };
    board.makeMove(move);
    EXPECT_EQUAL(board.at({1, 3}), MARBLE_EMPTY);       // jumping marble left its start
    EXPECT_EQUAL(board.at({2, 3}), MARBLE_EMPTY);       // jumped-over marble is gone
    EXPECT_EQUAL(board.at({3, 3}), MARBLE_OCCUPIED);    // jumping marble landed
    EXPECT_EQUAL(board.numMarbles(), 31);

    board.undoMove(move);
    EXPECT_EQUAL(board, MarbleBoard());
    EXPECT_EQUAL(board.numMarbles(), 32);
}

PROVIDED_TEST("MarbleBoard::makeMove and undoMove: jump to the left, then undo") {
    MarbleBoard board(loadBoard("marbles/boards/one-jump.txt"));
    MarbleBoard original = board;
    Move move = { {3, 2}, {3, 0} };
    board.makeMove(move);
    EXPECT_EQUAL(board, MarbleBoard(boardFromStrings({
        "XX---XX",
        "XX---XX",
        "-------",
        "O------",
        "-------",
        "XX---XX",
        "XX---XX"
    })));
    EXPECT(board.isSolved());
    board.undoMove(move);
    EXPECT_EQUAL(board, original);
    EXPECT_EQUAL(board.numMarbles(), 2);
}


STUDENT_TEST("Every starting move can be made and undone") {
    MarbleBoard board;
    MarbleBoard original = board;
    for (Move move : board.findPossibleMoves()) {
        board.makeMove(move);
        EXPECT_EQUAL(board.numMarbles(), 31);
        board.undoMove(move);
        EXPECT_EQUAL(board, original);
        EXPECT_EQUAL(board.numMarbles(), 32);
    }
}

STUDENT_TEST("Vertical pair has jumps up and down") {
    MarbleBoard board(boardFromStrings({
        "XX---XX", "XX---XX", "---O---", "---O---",
        "-------", "XX---XX", "XX---XX"
    }));
    Set<Move> expected = {{{2, 3}, {4, 3}}, {{3, 3}, {1, 3}}};
    EXPECT_EQUAL(toSet(board.findPossibleMoves()), expected);
}
