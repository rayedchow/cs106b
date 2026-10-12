/*
 * CS106B Assignment 2 — ADT warmup
 * Rayed Chowdhury, Jun Jang
 */
#include "queue.h"
#include "stack.h"
#include "SimpleTest.h"
using namespace std;

/* Reverses q in place by transferring its contents through a stack.
 */
void reverse(Queue<int>& q) {
    Stack<int> s;
    int num;

    while (!q.isEmpty()) {
        num = q.dequeue();
        s.push(num);
    }
    while (!s.isEmpty()) {
        num = s.pop();
        q.enqueue(num);
    }
}

/* Duplicates each negative integer in its original position in the queue.
 * q is modified in place; the original queue size bounds the loop.
 */
void duplicateNegatives(Queue<int>& q) {
    int n = q.size();
    for (int i = 0; i < n; i++) {
        int val = q.dequeue();
        q.enqueue(val);
        if (val < 0) {
            q.enqueue(val);   // double up on negative numbers
        }
    }
}

/* Returns the sum of all integers in s, or zero if s is empty.
 * The stack is passed by value, so the caller's stack is unchanged.
 */
int sumStack(Stack<int> s) {
    int total = 0;
    while (!s.isEmpty()) {
        total += s.pop();
    }
    return total;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("reverse queue") {
    Queue<int> q = {1, 2, 3, 4, 5};
    Queue<int> expected = {5, 4, 3, 2, 1};

    reverse(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has no negatives") {
    Queue<int> q = {2, 10};
    Queue<int> expected = q;

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has one negative, one positive") {
    Queue<int> q = {-7, 9};
    Queue<int> expected = {-7, -7, 9};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has negatives interspersed with positives") {
    Queue<int> q = {-3, 4, -5, 10};
    Queue<int> expected = {-3, -3, 4, -5, -5, 10};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("sumStack, 3 elements") {
    Stack<int> s = {1, 8, -5};

    EXPECT_EQUAL(sumStack(s), 4);
}

PROVIDED_TEST("sumStack, empty stack") {
    Stack<int> empty;

    EXPECT_EQUAL(sumStack(empty), 0);
}

STUDENT_TEST("Single negative number") {
    Queue<int> q = {-10};
    Queue<int> expected = {-10, -10};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

STUDENT_TEST("duplicateNegatives handles negatives at both ends") {
    Queue<int> q = {-3, 4, -5, 10, -2};
    duplicateNegatives(q);
    EXPECT_EQUAL(q, (Queue<int>{-3, -3, 4, -5, -5, 10, -2, -2}));
}

STUDENT_TEST("duplicateNegatives handles an empty queue and zero") {
    Queue<int> empty;
    Queue<int> q = {0, 2, 0};
    duplicateNegatives(empty);
    duplicateNegatives(q);
    EXPECT(empty.isEmpty());
    EXPECT_EQUAL(q, (Queue<int>{0, 2, 0}));
}
