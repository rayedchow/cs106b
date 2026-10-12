/*
 * File: perfect.cpp
 * Course: CS106B, Autumn 2026
 * Authors: Rayed Chowdhury, Jun Jang
 *
 * Assignment 1, Perfect Numbers
 *
 * Tidbit: We learned a lot about how different number theory algorithms
 * work! It's pretty interesting.
 *
 */
#include "console.h"
#include <iostream>
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* The smarterSum function returns the sum of the proper divisors of `n`
 * (all positive divisors except n itself), giving the same result as
 * divisorSum but much faster.
 *
 * Divisors come in pairs (d, n / d), where one is at most sqrt(n), so
 * the loop only checks candidates d with d * d <= n and adds both members
 * of each pair. Two special cases:
 *  - 1 is added up front for any n > 1, and the loop starts at 2. This
 *    way the partner of 1 (which is n itself) is never added.
 *  - For a perfect square, d == n / d, so that divisor is added only once.
 *
 * Parameter n: the number whose proper divisors are summed.
 * Returns: the sum, or 0 when n <= 1 (matching divisorSum).
 */
long smarterSum(long n) {
    long total = 0;
    if (n > 1) {
        total = 1;
        for (long divisor = 2; divisor * divisor <= n; divisor++) {
            if (n % divisor == 0) {
                total += divisor;
                long pairedDivisor = n / divisor;
                if (pairedDivisor != divisor) {
                    total += pairedDivisor;
                }
            }
        }
    }
    return total;
}

/* The isPerfectSmarter function returns true if `n` is a perfect number
 * (a positive number equal to the sum of its proper divisors) and false
 * otherwise. It works like isPerfect but uses the faster smarterSum.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* The findPerfectsSmarter function searches every number from 1 up to
 * (but not including) `stop` and prints each perfect number it finds.
 * It works like findPerfects but uses isPerfectSmarter, and prints a dot
 * every 10000 numbers to show progress.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush;
        }
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* The findNthPerfectEuclid function returns the nth perfect number using
 * Euclid's rule: if the Mersenne number 2^k - 1 is prime, then
 * 2^(k-1) * (2^k - 1) is perfect. Starting at k = 1, it tests each
 * Mersenne number in turn (a number is prime exactly when its only proper
 * divisor is 1, i.e. smarterSum returns 1) and stops at the nth prime.
 *
 * Parameter n: which perfect number to find (1 for the first, 6).
 * Returns: the nth perfect number.
 * Precondition: n > 0.
 * Limitation: results larger than a long can hold overflow and come out
 * wrong. That is past the 8th perfect number when long is 64 bits, or past
 * the 5th when long is 32 bits (as on Windows).
 */
long findNthPerfectEuclid(long n) {
    long perfectsFound = 0;
    long powerOfTwo = 2; // holds 2^k, starting at k = 1
    long perfect = 0;

    while (perfectsFound < n) {
        long mersenne = powerOfTwo - 1;
        if (smarterSum(mersenne) == 1) {
            perfectsFound++;
            perfect = (powerOfTwo / 2) * mersenne;
        }
        powerOfTwo *= 2;
    }
    return perfect;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}

STUDENT_TEST("Q2 findPerfects time trials: doubling, ending at ~60 seconds") {
    TIME_OPERATION(4000, findPerfects(4000));
    TIME_OPERATION(8000, findPerfects(8000));
    TIME_OPERATION(16000, findPerfects(16000));
    TIME_OPERATION(32000, findPerfects(32000));
    TIME_OPERATION(400000, findPerfects(400000));  // takes approx 60s
}

STUDENT_TEST("Q5 isPerfect returns false for negative inputs") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-100));
    EXPECT(!isPerfect(-1000));
}

STUDENT_TEST("Q6 smarterSum matches divisorSum on edge cases 0 and 1") {
    EXPECT_EQUAL(smarterSum(0), divisorSum(0));
    EXPECT_EQUAL(smarterSum(1), divisorSum(1));
}

STUDENT_TEST("Q6 smarterSum matches divisorSum on negative input") {
    EXPECT_EQUAL(smarterSum(-100), divisorSum(-100));
}

STUDENT_TEST("Q6 smarterSum matches divisorSum on perfect squares") {
    EXPECT_EQUAL(smarterSum(25), divisorSum(25));
    EXPECT_EQUAL(smarterSum(36), divisorSum(36));
}

STUDENT_TEST("Q6 smarterSum matches divisorSum on primes, perfects, and a large input") {
    EXPECT_EQUAL(smarterSum(13), divisorSum(13));
    EXPECT_EQUAL(smarterSum(28), divisorSum(28));
    EXPECT_EQUAL(smarterSum(1000), divisorSum(1000));
}

STUDENT_TEST("isPerfectSmarter agrees with isPerfect, including 1 and 0") {
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
    EXPECT(isPerfectSmarter(6));
    EXPECT(isPerfectSmarter(496));
    EXPECT(!isPerfectSmarter(12));
}

STUDENT_TEST("Q7 findPerfectsSmarter time trials: doubling, ending at ~60 seconds") {
    TIME_OPERATION(4000, findPerfectsSmarter(4000));
    TIME_OPERATION(8000, findPerfectsSmarter(8000));
    TIME_OPERATION(16000, findPerfectsSmarter(16000));
    TIME_OPERATION(32000, findPerfectsSmarter(32000));
    TIME_OPERATION(10580000, findPerfectsSmarter(10580000)); // takes approx 60s
}

STUDENT_TEST("Q9 findNthPerfectEuclid: first perfect number is 6") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
}

STUDENT_TEST("Q9 findNthPerfectEuclid: 2nd through 4th match known values") {
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
}

STUDENT_TEST("Q9 findNthPerfectEuclid: 5th perfect number is 33550336") {
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}

STUDENT_TEST("Q9 findNthPerfectEuclid results pass the independent isPerfect check") {
    EXPECT(isPerfect(findNthPerfectEuclid(1)));
    EXPECT(isPerfect(findNthPerfectEuclid(3)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(5)));
}
