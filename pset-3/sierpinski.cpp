/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 3
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "SimpleTest.h"
using namespace std;

/*
 * Parameters: window is the drawing window; one, two, and three are the corners
 * Does: fills the triangle with corners one, two, and three in window
 * Preconditions: window is a valid graphics window
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * Parameters: window is the drawing window; one, two, and three are the corners
 *             order is the fractal order
 * Does: draws the fractal with corners one, two, and three in window
 * Returns: # of filled black triangles
 * Preconditions: window is a valid graphics window
 * Errors: order is negative
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order < 0) {
        error("Triangle order must be nonnegative.");
    }
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    GPoint oneTwo = {(one.x + two.x) / 2, (one.y + two.y) / 2};
    GPoint oneThree = {(one.x + three.x) / 2, (one.y + three.y) / 2};
    GPoint twoThree = {(two.x + three.x) / 2, (two.y + three.y) / 2};
    return drawSierpinskiTriangle(window, one, oneTwo, oneThree, order - 1)
           + drawSierpinskiTriangle(window, oneTwo, two, twoThree, order - 1)
           + drawSierpinskiTriangle(window, oneThree, twoThree, three, order - 1);
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all your testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

