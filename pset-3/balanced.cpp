/*
 * Rayed Chowdhury and Jun Jang
 * CS106B, Assignment 3
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "SimpleTest.h"

using namespace std;

/* The operatorsFrom function takes a string 'str' and returns
 * a string that contains only bracketing operator characters in
 * their original order. If the string is empty and has no brackets,
 * an empty string "" is returned.
 */
string operatorsFrom(string str) {

    string cleaned;

    if (str.empty()) {
        return "";
    }

    char first = str[0];

    if (first == '(' || first == ')' || first == '[' || first == ']' || first == '{' || first == '}') {
        cleaned += first;
    }

    cleaned += operatorsFrom(str.substr(1));

    return cleaned;
}

/* operatorsAreMatched function takes string 'ops,' which
 * is made up only of bracketing operators and returns true/false
 * if the string is balanced/unbalanced respectively. It removes
 * adjacent pairs of bracketing operators one at a time and checks
 * if the shorter string is balanced and goes until the string is empty,
 * which returns true in that case. If no such pair exists in a non-empty
 * string, the function returns false.
 */
bool operatorsAreMatched(string ops) {
    if (ops.empty()) {
        return true;
    }

    if (stringContains(ops, "()")) {
        int position = ops.find("()");
        ops.erase(position, 2);
        return operatorsAreMatched(ops);
    } else if (stringContains(ops, "{}")) {
        int position = ops.find("{}");
        ops.erase(position, 2);
        return operatorsAreMatched(ops);
    } else if (stringContains(ops, "[]")) {
        int position = ops.find("[]");
        ops.erase(position, 2);
        return operatorsAreMatched(ops);
    } else {
        return false;
    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

STUDENT_TEST("operatorsFrom, additional tests to verify it tests all cases") {
    EXPECT_EQUAL(operatorsFrom(""), "");
    EXPECT_EQUAL(operatorsFrom("a! d3?"), "");
    EXPECT_EQUAL(operatorsFrom("a(b[c]d)e"), "([])");
}

STUDENT_TEST("operatorsAreMatched on four different cases") {
    EXPECT(operatorsAreMatched("{[()]}"));
    EXPECT(operatorsAreMatched("[]{}()"));
    EXPECT(!operatorsAreMatched("(]"));
    EXPECT(!operatorsAreMatched(")("));
}
