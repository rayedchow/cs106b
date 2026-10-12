/*
 * File: soundex.cpp
 * Course: CS106B, Autumn 2026
 * Authors: Rayed Chowdhury, Jun Jang
 *
 * Assignment 1, Soundex.
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

// Length of every Soundex code: one letter followed by three digits.
const int CODE_LENGTH = 4;

// Soundex digit for each letter, in alphabet order: index 0 is A's digit,
// index 1 is B's digit, and so on through Z.
const string LETTER_DIGITS = "01230120022455012623010202";

/* The lettersOnly function returns a copy of `s` that keeps only the
 * letter characters, in their original order (digits, spaces, and
 * punctuation are removed).
 *
 * Bug fixed: the original code always copied s[0] into the result without
 * checking whether it was a letter, then started the loop at index 1. So a
 * string that began with a non-letter ("9abc") kept that character, and an
 * empty string returned a 1-character string instead of "". The fix starts
 * from an empty result and checks every character, including the first.
 */
string lettersOnly(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* The encodeAsDigits function replaces each letter of `letters` with its
 * Soundex digit (as a character), ignoring case. For example, "Curie"
 * becomes "20600".
 *
 * Precondition: `letters` contains only letter characters.
 */
string encodeAsDigits(string letters) {
    string digits = "";
    for (int i = 0; i < letters.length(); i++) {
        int alphabetIndex = toupper(letters[i]) - 'A';
        digits += LETTER_DIGITS[alphabetIndex];
    }
    return digits;
}

/* The removeAdjacentDuplicates function returns `digits` with each run of
 * repeated neighboring characters reduced to one copy. For example,
 * "222025" becomes "2025".
 */
string removeAdjacentDuplicates(string digits) {
    string result = "";
    for (int i = 0; i < digits.length(); i++) {
        if (i == 0 || digits[i] != digits[i - 1]) {
            result += digits[i];
        }
    }
    return result;
}

/* The removeZeros function returns `code` with every '0' character
 * removed. For example, "A0203" becomes "A23".
 */
string removeZeros(string code) {
    string result = "";
    for (int i = 0; i < code.length(); i++) {
        if (code[i] != '0') {
            result += code[i];
        }
    }
    return result;
}

/* The padOrTruncate function returns `code` made exactly CODE_LENGTH
 * characters long: short codes get '0's added to the end, and long codes
 * have the extra characters cut off. For example, "L" becomes "L000" and
 * "E2424" becomes "E242".
 */
string padOrTruncate(string code) {
    while (code.length() < CODE_LENGTH) {
        code += '0';
    }
    return code.substr(0, CODE_LENGTH);
}

/* The soundex function returns the Soundex code for the surname `s`
 * (case does not matter, and non-letters are ignored). Following the
 * algorithm, it keeps only the letters, encodes them as digits, merges
 * adjacent duplicate digits, replaces the first digit with the uppercase
 * first letter, removes zeros, and pads or truncates to length 4.
 *
 * Returns: the 4-character code, or "" if `s` contains no letters.
 */
string soundex(string s) {
    string code = "";
    string letters = lettersOnly(s);

    if (!letters.empty()) {
        string digits = removeAdjacentDuplicates(encodeAsDigits(letters));
        string withInitial = charToString(toupper(letters[0])) + digits.substr(1);
        code = padOrTruncate(removeZeros(withInitial));
    }
    return code;
}

/* The findMatches function returns, in sorted order, every name in
 * `names` whose Soundex code equals `searchCode`. The names vector is
 * passed by reference to avoid copying the whole database on each call.
 */
Vector<string> findMatches(const Vector<string>& names, string searchCode) {
    Vector<string> matches;
    for (const string& name : names) {
        if (soundex(name) == searchCode) {
            matches.add(name);
        }
    }
    matches.sort();
    return matches;
}

/* The soundexSearch function runs the console search program. It reads
 * a database of surnames (one per line) from the file at `filepath`, then
 * repeatedly asks the user for a surname, prints that name's Soundex code,
 * and prints all database names with the same code in sorted order. It
 * stops when the user presses RETURN on an empty line.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames
    while (true) {
        cout << endl;
        string searchName = getLine("Enter a surname (RETURN to quit): ");
        if (searchName == "") {
            cout << "All done!" << endl;
            break;
        }

        string searchCode = soundex(searchName);
        cout << "Soundex code is " << searchCode << endl;
        cout << "Matches from database: " << findMatches(allNames, searchCode) << endl;
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

STUDENT_TEST("lettersOnly on an empty string returns an empty string") {
    // the original lettersOnly returned a 1-character string here
    EXPECT_EQUAL(lettersOnly(""), "");
}

STUDENT_TEST("lettersOnly drops a non-letter first character") {
    // the original lettersOnly always kept s[0], letter or not
    EXPECT_EQUAL(lettersOnly("9abc"), "abc");
    EXPECT_EQUAL(lettersOnly("123"), "");
    EXPECT_EQUAL(lettersOnly("-"), "");
}

STUDENT_TEST("encodeAsDigits handles upper and lower case") {
    EXPECT_EQUAL(encodeAsDigits("Curie"), "20600");
    EXPECT_EQUAL(encodeAsDigits("DRELL"), "36044");
    EXPECT_EQUAL(encodeAsDigits("z"), "2");
}

STUDENT_TEST("removeAdjacentDuplicates merges runs but not separated repeats") {
    EXPECT_EQUAL(removeAdjacentDuplicates("222025"), "2025");
    EXPECT_EQUAL(removeAdjacentDuplicates("1111"), "1");
    EXPECT_EQUAL(removeAdjacentDuplicates("202"), "202");
    EXPECT_EQUAL(removeAdjacentDuplicates(""), "");
}

STUDENT_TEST("removeZeros removes every zero and nothing else") {
    EXPECT_EQUAL(removeZeros("A0203"), "A23");
    EXPECT_EQUAL(removeZeros("A000"), "A");
    EXPECT_EQUAL(removeZeros("B123"), "B123");
}

STUDENT_TEST("padOrTruncate pads short codes and cuts long ones") {
    EXPECT_EQUAL(padOrTruncate("L"), "L000");
    EXPECT_EQUAL(padOrTruncate("E2424"), "E242");
    EXPECT_EQUAL(padOrTruncate("C600"), "C600");
}

STUDENT_TEST("soundex on our own names and Angelou") {
    EXPECT_EQUAL(soundex("Chowdhury"), "C360");
    EXPECT_EQUAL(soundex("Angelou"), "A524");
}

STUDENT_TEST("Pfister: first letter's digit matches the next letter's digit") {
    EXPECT_EQUAL(soundex("Pfister"), "P236");
}

STUDENT_TEST("soundex returns empty string when there are no letters") {
    EXPECT_EQUAL(soundex("123"), "");
}

