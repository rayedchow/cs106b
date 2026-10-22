/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 3
 */
#include "recursion.h"
#include "SimpleTest.h"
using namespace std;

/*
 * Parameters: a and b are the ballot counts for A and B
 * Returns: # of orderings of a A ballots and b B ballots
 * Preconditions: nonnegative counts with a > b & counts at most 15
 */
int countAllOrderings(int a, int b) {
    if (a == 0 || b == 0) {
        return 1;
    }
    return countAllOrderings(a - 1, b) + countAllOrderings(a, b - 1);
}

/*
 * Parameters: a and b are remaining ballot counts; lead is A's current lead
 * Returns: # of good completions with a A ballots and b B ballots left
 * Preconditions: nonnegative counts & lead is A's current lead
 */
int countGoodCompletions(int a, int b, int lead) {
    if (lead <= 0) {
        return 0;
    }
    if (b == 0) {
        return 1;
    }
    if (a == 0) {
        return lead > b;
    }
    return countGoodCompletions(a - 1, b, lead + 1)
           + countGoodCompletions(a, b - 1, lead - 1);
}

/*
 * Parameters: a and b are the ballot counts for A and B
 * Returns: # of orderings of a A ballots and b B ballots where A always leads
 * Preconditions: nonnegative counts with a > b & counts at most 15
 */
int countGoodOrderings(int a, int b) {
    // A must receive the first ballot.
    return countGoodCompletions(a - 1, b, 1);
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("countAllOrderings, two A one B") {
   EXPECT_EQUAL(countAllOrderings(2, 1), 3);
}

PROVIDED_TEST("countGoodOrderings, two A one B") {
   EXPECT_EQUAL(countGoodOrderings(2, 1), 1);
}

STUDENT_TEST("Hand counts for small ballot collections") {
    // AAAB, AABA, ABAA, BAAA: only the first two are good.
    EXPECT_EQUAL(countAllOrderings(3, 1), 4);
    EXPECT_EQUAL(countGoodOrderings(3, 1), 2);
    // AAABB, AABAB, AABBA, ABAAB, ABABA, ABBAA,
    // BAAAB, BAABA, BABAA, BBAAA: only the first two are good.
    EXPECT_EQUAL(countAllOrderings(3, 2), 10);
    EXPECT_EQUAL(countGoodOrderings(3, 2), 2);
    EXPECT_EQUAL(countAllOrderings(1, 0), 1);
    EXPECT_EQUAL(countGoodOrderings(1, 0), 1);
    EXPECT_EQUAL(countAllOrderings(4, 0), 1);
    EXPECT_EQUAL(countGoodOrderings(4, 0), 1);
}

STUDENT_TEST("Ballot counts agree with Bertrand's theorem") {
    for (int a = 1; a < 11; a++) {
        for (int b = 0; b < a; b++) {
            double ratio = double(countGoodOrderings(a, b)) / countAllOrderings(a, b);
            EXPECT_EQUAL(ratio, double(a - b) / (a + b));
        }
    }
}
