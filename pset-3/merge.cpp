/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 3
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "SimpleTest.h"
using namespace std;

/*
 * Parameters: one and two are the queues to merge
 * Returns: sorted queue of all values from one and two, keeping inputs unchanged
 * Preconditions: one and two are sorted in increasing order
 * Errors: either input is out of order
 */
Queue<int> binaryMerge(Queue<int> one, Queue<int> two) {
    Queue<int> result;
    int previous = 0;

    while (!one.isEmpty() || !two.isEmpty()) {
        int next;
        if (one.isEmpty() || (!two.isEmpty() && one.peek() > two.peek())) {
            next = two.dequeue();
        } else {
            next = one.dequeue();
        }
        if (!result.isEmpty() && next < previous) {
            error("Input queues must be sorted in increasing order.");
        }
        result.enqueue(next);
        previous = next;
    }

    return result;
}

/*
 * Parameters: all contains the queues to merge
 * Returns: sorted queue of all values in all, keeping inputs unchanged
 * Preconditions: each queue in all is sorted in increasing order
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * Parameters: all contains the queues to merge
 * Returns: sorted queue of all values in all, keeping inputs unchanged
 * Preconditions: each queue in all is sorted in increasing order
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    if (all.isEmpty()) {
        return {};
    }
    if (all.size() == 1) {
        return all[0];
    }

    Vector<Queue<int>> left = all.subList(0, all.size() / 2);
    Vector<Queue<int>> right = all.subList(all.size() / 2);

    return binaryMerge(recMultiMerge(left), recMultiMerge(right));
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/*
 * Parameters: size is the number of values to put in the queue
 * Returns: queue of integers from 0 to size - 1
 * Preconditions: size is nonnegative
 */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/*
 * Parameters: input contains the values; all contains the destination queues
 * Does: randomly adds values from input to the queues in all
 * Preconditions: all is nonempty if input is nonempty
 */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}


STUDENT_TEST("binaryMerge handles empty, unequal, and duplicate queues") {
    Queue<int> empty;
    Queue<int> values = {-4, -4, 0, 2, 8};
    EXPECT_EQUAL(binaryMerge(empty, empty), empty);
    EXPECT_EQUAL(binaryMerge(empty, values), values);
    EXPECT_EQUAL(binaryMerge(values, empty), values);
    Queue<int> single = {2};
    Queue<int> expected = {-4, -4, 0, 2, 2, 8};
    EXPECT_EQUAL(binaryMerge(single, values), expected);
    EXPECT_EQUAL(binaryMerge(values, single), expected);
    EXPECT_EQUAL(values, (Queue<int>{-4, -4, 0, 2, 8}));
}

STUDENT_TEST("binaryMerge rejects out-of-order values, including in the tail") {
    Queue<int> invalid = {1, 4, 3};
    Queue<int> sorted = {0, 2};
    Queue<int> empty;
    EXPECT_ERROR(binaryMerge(invalid, sorted));
    EXPECT_ERROR(binaryMerge(sorted, invalid));
    EXPECT_ERROR(binaryMerge(invalid, empty));
    EXPECT_ERROR(binaryMerge(empty, invalid));
    EXPECT_ERROR(binaryMerge(Queue<int>{2, 1}, Queue<int>{}));
}

STUDENT_TEST("Multiway merges handle zero queues") {
    Vector<Queue<int>> all;
    EXPECT(naiveMultiMerge(all).isEmpty());
    EXPECT(recMultiMerge(all).isEmpty());
}

STUDENT_TEST("Multiway merges handle many empty queues") {
    Vector<Queue<int>> all(7);
    EXPECT(naiveMultiMerge(all).isEmpty());
    EXPECT(recMultiMerge(all).isEmpty());
}

STUDENT_TEST("Multiway merges handle one nonempty queue among empty queues") {
    Vector<Queue<int>> all = {{}, {-2, -2, 5}, {}, {}, {}};
    Queue<int> expected = {-2, -2, 5};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
    EXPECT_EQUAL(recMultiMerge(all), expected);
    EXPECT_EQUAL(all[1], expected);
}

STUDENT_TEST("Recursive merge matches naive merge for varied queue counts") {
    for (int k = 1; k <= 20; k++) {
        Vector<Queue<int>> all(k);
        distribute(createSequence(150), all);
        EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
    }
}

STUDENT_TEST("Time binaryMerge over doubling total sizes") {
    for (int n = 500000; n <= 4000000; n *= 2) {
        Queue<int> one = createSequence(n / 2);
        Queue<int> two = createSequence(n / 2);
        TIME_OPERATION(n, binaryMerge(one, two));
    }
}

STUDENT_TEST("Time naiveMultiMerge with fixed k = 32") {
    for (int n = 50000; n <= 800000; n *= 2) {
        Vector<Queue<int>> all(32);
        distribute(createSequence(n), all);
        TIME_OPERATION(n, naiveMultiMerge(all));
    }
}

STUDENT_TEST("Time naiveMultiMerge with fixed n = 200000") {
    for (int k = 8; k <= 128; k *= 2) {
        Vector<Queue<int>> all(k);
        distribute(createSequence(200000), all);
        TIME_OPERATION(k, naiveMultiMerge(all));
    }
}

STUDENT_TEST("Time recMultiMerge with fixed k = 32") {
    for (int n = 50000; n <= 800000; n *= 2) {
        Vector<Queue<int>> all(32);
        distribute(createSequence(n), all);
        TIME_OPERATION(n, recMultiMerge(all));
    }
}

STUDENT_TEST("Time recMultiMerge with fixed n = 200000") {
    for (int k = 2; k <= 512; k *= 4) {
        Vector<Queue<int>> all(k);
        distribute(createSequence(200000), all);
        TIME_OPERATION(k, recMultiMerge(all));
    }
}
