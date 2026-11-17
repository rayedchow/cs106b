/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 6
 */

#include "Labyrinth.h"
using namespace std;

/* The isPathToFreedom function takes a starting cell and
 * a string of moves and returns true if every move is legal
 * and the path picks up all three of the spellbook, potion,
 * and wand. On the other hand, if any of those conditions
 * are failed, then the function returns false.
 */
bool isPathToFreedom(MazeCell* start, const string& moves) {
    MazeCell* current = start;

    bool hasSpellBook = false;
    bool hasPotion = false;
    bool hasWand = false;

    if (current->whatsHere == Item::SPELLBOOK) {
        hasSpellBook = true;
    } else if (current->whatsHere == Item::POTION) {
        hasPotion = true;
    } else if (current->whatsHere == Item::WAND) {
        hasWand = true;
    }

    for (int i = 0; i < moves.length(); i++) {
        char step = moves[i];

        if (step == 'N') {
            current = current->north;
        } else if (step == 'S') {
            current = current->south;
        } else if (step == 'E') {
            current = current->east;
        } else if (step == 'W') {
            current = current->west;
        }

        if (current == nullptr) {
            return false;
        }

        if (current->whatsHere == Item::SPELLBOOK) {
            hasSpellBook = true;
        } else if (current->whatsHere == Item::POTION) {
            hasPotion = true;
        } else if (current->whatsHere == Item::WAND) {
            hasWand = true;
        }
    }


    if (hasSpellBook && hasPotion && hasWand) {
        return true;
    } else {
        return false;
    }
}


/* * * * * * Test Cases Below This Point * * * * * */
#include "GUI/SimpleTest.h"
#include "Demos/MazeGenerator.h"

/* Optional: Add your own custom tests here! */














/* * * * * Provided Tests Below This Point * * * * */

/* Utility function to free all memory allocated for a maze. */
void deleteMaze(const Grid<MazeCell*>& maze) {
    for (auto* elem: maze) {
        delete elem;
    }
    /* Good question to ponder: why don't we write 'delete maze;'
     * rather than what's shown above?
     */
}

PROVIDED_TEST("Checks paths in the sample maze.") {
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN"));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES"));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES"));

    /* These paths don't work, since they don't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW"));
    EXPECT(!isPathToFreedom(maze[2][2], "SWWN"));
    EXPECT(!isPathToFreedom(maze[2][2], "WNNE"));

    /* These paths don't work, since they aren't legal paths. */
    EXPECT(!isPathToFreedom(maze[2][2], "WW"));
    EXPECT(!isPathToFreedom(maze[2][2], "NN"));
    EXPECT(!isPathToFreedom(maze[2][2], "EE"));
    EXPECT(!isPathToFreedom(maze[2][2], "SS"));

    deleteMaze(maze);
}

PROVIDED_TEST("Can't walk through walls.") {
    auto maze = toMaze({"* S *",
                        "     ",
                        "W * P",
                        "     ",
                        "* * *"});

    EXPECT(!isPathToFreedom(maze[1][1], "WNEES"));
    EXPECT(!isPathToFreedom(maze[1][1], "NWSEE"));
    EXPECT(!isPathToFreedom(maze[1][1], "ENWWS"));
    EXPECT(!isPathToFreedom(maze[1][1], "SWNNEES"));

    deleteMaze(maze);
}

PROVIDED_TEST("Works when starting on an item.") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "EE"));
    EXPECT(isPathToFreedom(maze[0][1], "WEE"));
    EXPECT(isPathToFreedom(maze[0][2], "WW"));

    deleteMaze(maze);
}

/* Printer for items. */
ostream& operator<< (ostream& out, Item item) {
    if (item == Item::NOTHING) {
        return out << "Item::NOTHING";
    } else if (item == Item::WAND) {
        return out << "Item::WAND";
    } else if (item == Item::POTION) {
        return out << "Item::POTION";
    } else if (item == Item::SPELLBOOK) {
        return out << "Item::SPELLBOOK";
    } else {
        return out << "<unknown item type>";
    }
}
