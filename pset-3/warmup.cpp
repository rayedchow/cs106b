/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 3
 */

#include <iostream>    // for cout, endl
#include "error.h"
#include "random.h"
#include "SimpleTest.h"
using namespace std;


/*
 * Parameters: n is the integer to take the factorial of
 * Returns: n factorial
 * Preconditions: n is at most 12
 * Errors: n is negative
 */
int factorial(int n) {
    if (n < 0) {
        error("Factorial requires a nonnegative argument.");
    }
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

/*
 * Parameters: base is the number to raise; exp is the exponent
 * Returns: base raised to exp using a loop
 * Preconditions: exp is nonnegative
 */
double iterativePower(double base, int exp) {
    double result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

/*
 * Parameters: base is the number to raise; exp is the exponent
 * Returns: base raised to exp using recursion
 * Preconditions: exp is nonnegative
 */
double recPower(double base, int exp) {
    if (exp == 0) {
        return 1;
    } else {
        double half = recPower(base, exp/2);
        if (exp % 2 == 0) {
            return half * half;
        } else {
            return half * half * base;
        }
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Confirm factorial(3) is 6 ") {
    EXPECT_EQUAL(factorial(3), 6);
}

PROVIDED_TEST("Test recursive power against iterative power") {
     EXPECT_EQUAL(recPower(7, 3), iterativePower(7, 3));
}

PROVIDED_TEST("Test recursive power against iterative power, generated inputs") {
    for (int base = -8; base < 35; base++) {
        for (int exp = 0; exp < 10; exp++) {
            EXPECT_EQUAL(recPower(base, exp), iterativePower(base, exp));
        }
    }
}


/*
 * Parameters: base and exp are the inputs passed to recPower
 * Does: calls recPower repeatedly with base and exp for timing
 * Preconditions: exp is nonnegative
 */
void manyPowerCalls(double base, int exp) {
    const int NUM_ITERATIONS = 24000000;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        recPower(base, exp);
    }
}

PROVIDED_TEST("Time trial recPower, double base, keep exp constant") {
    for (int size = 4; size <= 128; size *= 2) {
        TIME_OPERATION(size, manyPowerCalls(size, 5));
    }
}

PROVIDED_TEST("Time trial recPower, keep base constant, double exp") {
    for (int size = 4; size <= 128; size *= 2) {
        TIME_OPERATION(size, manyPowerCalls(5, size));
    }
}


STUDENT_TEST("Factorial rejects negatives and handles zero and twelve") {
    EXPECT_ERROR(factorial(-3));
    EXPECT_EQUAL(factorial(0), 1);
    EXPECT_EQUAL(factorial(12), 479001600);
}
