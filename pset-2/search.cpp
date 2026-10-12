/*
 * CS106B Assignment 2 — Inverted-index search engine
 * Rayed Chowdhury, Jun Jang
 */
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


/* Removes non-alphanumeric characters and lowercases ASCII letters.
 * Returns an empty string when token has no letters or digits.
 */
string cleanToken(string token) {
    string cleaned;
    for (char ch : token) {
        if (isalnum(static_cast<unsigned char>(ch))) {
            cleaned += tolower(static_cast<unsigned char>(ch));
        }
    }
    return cleaned;
}

/* Returns the unique, nonempty normalized words in one page's body.
 * Tokens are separated by spaces and normalized with cleanToken.
 */
Set<string> gatherTokens(string bodytext) {
    Set<string> tokens;
    for (string word : stringSplit(bodytext, " ")) {
        string token = cleanToken(word);
        if (!token.empty()) {
            tokens.add(token);
        }
    }
    return tokens;
}

/* Reads alternating URL/body lines from dbfile and adds URL postings
 * to the inverted index. Returns the number of pages processed.
 * Reports an error when the database cannot be opened.
 * Assumes the input file consists of complete URL/body pairs.
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index) {
    ifstream input;
    if (!openFile(input, dbfile)) {
        error("Cannot open file named " + dbfile);
    }

    int pageCount = 0;
    string url;
    string bodytext;
    while (getline(input, url) && getline(input, bodytext)) {
        for (string token : gatherTokens(bodytext)) {
            index[token].add(url);
        }
        pageCount++;
    }
    return pageCount;
}

/* Evaluates a well-formed query left-to-right using set operations.
 * Plain words mean OR, +word means AND, and -word means exclusion.
 * Terms are case-insensitive and punctuation is ignored.
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Vector<string> terms = stringSplit(query, " ");
    Set<string> matches;

    for (int i = 0; i < terms.size(); i++) {
        string term = terms[i];
        char modifier = ' ';
        if (term[0] == '+' || term[0] == '-') {
            modifier = term[0];
            term = term.substr(1);
        }

        string token = cleanToken(term);
        Set<string> current;
        if (index.containsKey(token)) {
            current = index[token];
        }
        if (i == 0) {
            matches = current;
        } else if (modifier == '+') {
            matches *= current;
        } else if (modifier == '-') {
            matches -= current;
        } else {
            matches += current;
        }
    }
    return matches;
}

/* Builds an index from dbfile, reports its size, and repeatedly
 * searches entered queries until the user enters an empty line.
 * File-open errors are reported by buildIndex.
 */
void searchEngine(string dbfile) {
    Map<string, Set<string>> index;
    cout << "Stand by while building index..." << endl;
    int pageCount = buildIndex(dbfile, index);
    cout << "Indexed " << pageCount << " pages containing " << index.size()
         << " unique terms." << endl;

    string query;
    do {
        cout << endl;
        query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if (!query.empty()) {
            Set<string> matches = findQueryMatches(index, query);
            cout << "Found " << matches.size() << " matching pages" << endl;
            cout << matches << endl;
        }
    } while (!query.empty());
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings of letters and digits") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS106B"), "cs106b");
}

PROVIDED_TEST("cleanToken on strings containing punctuation") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
    EXPECT_EQUAL(cleanToken("they're"), "theyre");
}

PROVIDED_TEST("cleanToken on string of only punctuation") {
    EXPECT_EQUAL(cleanToken("#$^@@.;"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 6 unique tokens, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 12 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 12);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}



STUDENT_TEST("cleanToken preserves digits and strips mixed punctuation") {
    EXPECT_EQUAL(cleanToken("...MiD-quarter!?"), "midquarter");
    EXPECT_EQUAL(cleanToken("4.CS106B_"), "4cs106b");
    EXPECT_EQUAL(cleanToken("!!!"), "");
}

STUDENT_TEST("gatherTokens excludes empty tokens and duplicates") {
    Set<string> expected = {"fish", "42", "red"};
    EXPECT_EQUAL(gatherTokens("Fish fish !!! RED red 42"), expected);
}

STUDENT_TEST("buildIndex records exact URL membership for tiny.txt") {
    Map<string, Set<string>> index;
    EXPECT_EQUAL(buildIndex("res/tiny.txt", index), 4);
    EXPECT_EQUAL(index["fish"],
                 (Set<string>{"www.shoppinglist.com", "www.dr.seuss.net",
                              "www.bigbadwolf.com"}));
    EXPECT_EQUAL(index["green"], (Set<string>{"www.rainbow.org"}));
}

STUDENT_TEST("query processing supports case-insensitive plus and minus") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expected = {"www.dr.seuss.net"};
    EXPECT_EQUAL(findQueryMatches(index, "RED +fIsh"), expected);
    EXPECT_EQUAL(findQueryMatches(index, "RED -FISH"),
                 (Set<string>{"www.rainbow.org"}));
}

STUDENT_TEST("query processing is left-to-right and handles unknown terms") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expected = {"www.shoppinglist.com", "www.dr.seuss.net",
                            "www.bigbadwolf.com"};
    EXPECT_EQUAL(findQueryMatches(index, "red -fish fish"), expected);
    EXPECT(findQueryMatches(index, "red +nosuchword").isEmpty());
}

STUDENT_TEST("query modifiers apply only at the beginning of a term") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(findQueryMatches(index, "milk +5LB_m&ms"),
                 (Set<string>{"www.shoppinglist.com"}));
}
