# CS106B Style Guide: Complete Summary

> Source: [CS106B Programming Abstractions, Style Guide](https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1272/course/style_guide/) (Stanford, archive 1272)
> Scraped and summarized 2026-10-12.

---

## Philosophy and Why Style Matters

- **You are the main beneficiary.** Tidy, well-structured code is easier to test, has fewer bugs, and keeps the remaining bugs isolated and easier to track down. You finish faster and get better results.
- **There is no single "best" style** and no checklist covers every case, but some choices are better than others. The guide steers you toward the better ones.
- **Grading expectation:** you must make a *concerted effort* to follow these practices. Code that breaks the rules can still be well styled, but for grading purposes you should adopt the course's habits. Take theoretical disagreements to the staff in person.
- **Career skill:** most companies require you to follow their style standard. Following a style guide carefully, and keeping style consistent across a team, are valuable job skills.
- **Interactive grading** with your section leader (SL) gives you one-on-one feedback on *your own* style choices. The guide calls this even more valuable than the guide itself, so don't skip it.

---

## 1. Layout, Indentation, and Whitespace

| Rule | Details |
|---|---|
| **Consistent indentation, always** | Indentation shows program structure and makes errors easier to find. |
| **Indent per brace** | Increase the indent one level at each `{` and decrease it at each `}`. |
| **Indent size** | Pick 2–4 spaces per level and **use it consistently**. |
| **One statement per line** | Never put more than one statement on a line. |
| **Long lines** | Break any line **longer than 100 characters** into two. Align the overflow with the text above it. |
| **Blank lines** | Separate functions, and logical groups of statements within a function, with blank lines. |
| **Operator spacing** | Put spaces between operators and their operands. |
| **Clarifying parentheses** | Add parentheses to show grouping wherever precedence might be unclear to a reader. |

```cpp
// BAD: confusing, hard to follow
while (x < y) {
   if (x != 0) { binky(x);
   }
else { winky(y);
    y--; }}
  return x;

// GOOD: indentation follows structure
while (x < y) {
    if (x != 0) {
        binky(x);
    } else {
        winky(y);
        y--;
    }
}
return x;
```

```cpp
// BAD: line too long
result = longFunctionName(argument, 106 * expression * variable) + variable - longerFunctionName() + otherFunction(variable);

// GOOD: wrapped and aligned
result = longFunctionName(argument, 106 * expression * variable) + variable -
         longerFunctionName() + otherFunction(variable);
```

```cpp
// BAD
int root = (-b+sqrt(b*b-4*a*c))/2*a;
// GOOD
int root = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
```

---

## 2. Names

Meaningful identifiers lower the reader's cognitive load and document the purpose of each variable and function.

- **Variables are nouns.** Ask "What is it?" and use a noun (`name`, `scores`), adding a modifier to clarify (`courseName`, `maxScore`).
  - **Don't repeat the type in the name.** Write `title`, not `titleString`.
  - **Avoid one-letter names** like `a` or `p`. Exceptions: loop counters `i`, `j` and coordinates `x`, `y`.
  - **Never name a variable `l`.** It is too easily confused with the digit `1`.
- **Functions are verbs.** Ask "What does it do?"
  - Functions that perform actions get verbs: `findSmallest`, `stripPunctuation`, `drawTriangle`.
  - Functions used mainly for their return value are named after the property returned: `isPrime`, `getAge`.
- **Use named constants, not magic numbers.** Declare a named `const` and use it everywhere the value is needed. This improves readability and gives you one place to change the value.
  ```cpp
  const int VOTING_AGE = 18;
  ```
- **Capitalization conventions.** These let a reader tell an identifier's category at a glance.

  | Kind | Convention | Example |
  |---|---|---|
  | Functions and variables | camelCase | `countPixels` |
  | Classes and types | Capitalized (PascalCase) | `GridLocation` |
  | Constants | UPPER_SNAKE_CASE | `MAX_WIDTH` |

---

## 3. Variable Scope

- **Use the narrowest possible scope.** If a variable is used only inside a loop, declare it inside the loop body, not at the top of the function or file.
- **Don't reuse a name in an inner scope.** An inner variable with the same name as an outer one *shadows* it. This is confusing and often causes hard-to-find bugs.
- **No global variables.** Share information between function calls through **parameters and return values**, never by reaching out to global state.

---

## 4. Use of C++ Language Features

### Prefer C++ idioms over C idioms
Use `cout` instead of `printf`, and `string` instead of `char*`.
```cpp
// old school
char* str = "My message";
printf("%s\n", str);

// modern and hip
string str = "My message";
cout << str << endl;
```

### `for` vs. `while`
- Use **`for`** when the number of repetitions is **known** (definite).
- Use **`while`** when it is **unknown** (indefinite).
```cpp
// loop exactly n times
for (int i = 0; i < n; i++) { ... }

// loop until there are no more lines
string str;
while (input >> str) { ... }
```

### `break` and `continue`
- Where possible, structure a loop the ordinary way, with a clear start, stop, and advance and no disruptive loop control.
- **A few uses of `break` are acceptable:** the *loop-and-a-half* (`while (true)` with a `break`), or when you need to exit mid-iteration.
- **`continue` is rare and often confusing. Avoid it.**

### `switch` fallthrough
- Almost every `case` should end with a `break` or `return`.
- In the very rare case where you *intend* to fall through, **say so in a comment**. Accidental fallthrough causes many difficult bugs.
```cpp
switch (val) {
    case 1:
        handleOne();
        break;
    case 2:
        handleTwo();
        // NOTE: fallthrough ***
    case 3:
        handleTwoOrThree();
```

### `return` statements
- Multiple returns are allowed, but it is usually better to **funnel through a single `return` at the end** of the function.
- **Acceptable early returns:** a recursive base case, an error handled at the start of a function, or a `return` used as a loop exit.
- **Don't scatter other returns through the function.** They cause a disproportionate share of bugs, because it is easy to overlook the early-return path and assume the function runs to the end.

### Always use `{}` on control statements
The body of every `if`/`else`/`for`/`while` etc. gets braces and proper line breaks, **even if it is one line**.
```cpp
// ugh
if (count == 0) error("not found");
for (int i = 0; i < n; i++) draw(i);
if (condition)
    doFirst();
    doSecond(); // inside? Indent looks so, but no braces!

// better
if (count == 0) {
    error("not found");
}
for (int i = 0; i < n; i++) {
    draw(i);
}
if (condition) {
    doFirst();
    doSecond();
}
```

### Unary / compound-assignment operators (`++`, `--`, `*=`, …)
- **Use them freely on standalone lines.**
- **Don't embed them in larger expressions** such as function-call arguments. The behavior there is unintuitive at best and **undefined** at worst.
```cpp
// BAD: which element? which value?
int i = 0;
fun(vec[i++], i++);

// GOOD
int i = 0;
fun(vec[i], i + 1);  // clear intended values
i += 2;              // clear moment of update
```

### Booleans
A boolean is already `true` or `false`, so never compare it to `true`/`false` or convert a boolean expression into `true`/`false` with an if/else.
```cpp
// BAD
if (isWall == true) { ... }
if (matches > 0) {
    return true;
} else {
    return false;
}

// GOOD
if (isWall) { ... }
return (matches > 0);
```

### Use `&&`, `||`, `!`, not `and`, `or`, `not`
C++ accepts both forms (mostly for international compatibility), but the symbolic operators are traditional and preferred. The word forms are unusual and jarring to C++ programmers.
```cpp
// non-standard
if ((even and positive) or not zero) { ... }
// preferred
if ((even && positive) || !zero) { ... }
```

### Use `error()` for fatal conditions
Report fatal errors with the **Stanford library's `error()`** function instead of throwing a raw C++ exception. It works well with the debugger and the **SimpleTest** framework.
```cpp
// raw exception (avoid)
if (arg < 0) { throw arg; }
// preferred
if (arg < 0) { error("arg must be positive!"); }
```

---

## 5. Efficiency

CS106B values efficient choices of data structures and algorithms **where the payoff is significant**. It does *not* want **micro-optimizations** that clutter the code for little gain.

- **Choose the better Big-O class.** O(N log N) beats O(N²). O(1) or O(log N) beats O(N).
- **Choose the best ADT for the job:**
  - For many lookups, use a **`Set`** rather than a **`Vector`**, because `contains` is efficient.
  - All **`Stack`/`Queue`** operations are **O(1)**. Use a `Stack` if you only add and remove at the top, and a `Queue` if you add at the tail and remove from the head.
  - **`HashSet`/`HashMap`** give a small win over `Set`/`Map` when you don't need **sorted order**.
- **Save expensive results and reuse them.** If you use the result of an expensive call more than once, store it in a variable. This matters most inside loops.
  ```cpp
  // computes search twice
  if (reallySlowSearch(term) >= 0) {
      remove(reallySlowSearch(term));
  }
  // avoid recompute
  int index = reallySlowSearch(term);
  if (index >= 0) {
      remove(index);
  }
  ```
- **Avoid copying large objects.** Passing or returning an object by value copies the whole thing, which is expensive for collection ADTs. **Pass by reference** so the caller and the function share one instance.
  ```cpp
  // slow because of copying
  void process(Set<string> data) { ... }
  Vector<int> fillVector() {
      Vector<int> v;
      // add data to v
      return v; // makes copy
  }

  // improved efficiency
  void process(Set<string>& data) { ... }
  void fillVector(Vector<int>& v) {   // shares vector without making copy
      // add data to v
  }
  ```

---

## 6. Unify Common Code, Avoid Redundancy

Repeated or copy-pasted code should be merged into one passage. Then there is only one piece of code to write, test, debug, update, and comment.

- **Decompose into a helper function.** Pull the common code out into a helper.
  ```cpp
  // repeated code
  if (g.inBounds(left) && g[left] && left != g[0][0]) {
      return true;
  } else if (g.inBounds(right) && g[right] && right != g[0][0]) {
      return true;
  }

  // unify common into helper
  bool isViable(GridLocation loc, Grid<bool>& g) {
      return g.inBounds(loc) && g[loc] && loc != g[0][0];
  }
  ...
  return isViable(left, g) || isViable(right, g);
  ```
- **Factor common code out of branches.** If every branch of a chained `if`/`else` or `switch` does the same thing, move it before or after the branch.
  ```cpp
  // repeated code
  if (tool == CIRCLE) {
      setColor("black"); drawCircle(); waitForClick();
  } else if (tool == SQUARE) {
      setColor("black"); drawSquare(); waitForClick();
  } else if (tool == LINE) {
      setColor("black"); drawLine(); waitForClick();
  }

  // factor out common
  setColor("black");
  if (tool == CIRCLE) {
      drawCircle();
  } else if (tool == SQUARE) {
      drawSquare();
  } else if (tool == LINE) {
      drawLine();
  }
  waitForClick();
  ```
  *(The original shows the "repeated" version with one statement per line. It is compressed here for space.)*

---

## 7. Function Design

A well-designed function:
1. Performs a **single, independent, coherent task**.
2. Doesn't do **too large a share** of the work.
3. Isn't **unnecessarily entangled** with other functions.
4. Uses **parameters** for flexibility and reuse, rather than being a one-task tool.
5. Has a **clear relationship between input** (parameters) **and output** (return value).

**Length:** a function longer than about **20–30 lines** is unwieldy. Break it into smaller sub-functions. **The "and" test:** if describing the function's purpose takes "and" a lot, it does too much and should be split.

**Value vs. reference parameters:**
- Use a **reference** parameter only when you need to modify the caller's value or send information out of the function.
- Don't use references where they aren't needed or helpful.
```cpp
/*
 * Solves a quadratic equation ax^2 + bx + c = 0,
 * storing the results in output parameters root1 and root2.
 * Assumes that the given equation has two real roots.
 */
void quadratic(double a, double b, double c,     // a, b, c: by value (read-only)
               double& root1, double& root2) {   // outputs: by reference
    double discr = sqrt((b * b) - (4 * a * c));
    root1 = (-b + discr) / (2 * a);
    root2 = (-b - discr) / (2 * a);
}
```

**For a single result, use a return value, not a reference "out" parameter:**
```cpp
// harder to follow
void max(int a, int b, int& result) {
    if (a > b) { result = a; } else { result = b; }
}
// better
int max(int a, int b) {
    if (a > b) { return a; } else { return b; }
}
```

**Avoid "chaining" calls**, where functions call each other in a long chain without ever returning to `main`. Make `main` a high-level outline that calls the major steps in turn.
```
// chained control flow (BAD)     // better structured as (GOOD)
main                              main
 └─ doGame                         ├─ welcome
     └─ initAndPlay                ├─ initializeGame
         └─ configureAndPlay       │   └─ configureBoard
             ├─ readCubes          │       └─ readCubes
             └─ playGame           └─ playGame
                 └─ doOneTurn          └─ doOneTurn
```

---

## 8. Class Design

> ⚠️ **Note:** In the page's HTML source, this section sits inside a template `{% comment %}` block, so it may be hidden or inactive for this quarter. The `const`-members item is marked *"Optional, not covered this quarter."* It is included here for completeness.

- **Encapsulation:** make all data fields **`private`**.
- **`.h` vs. `.cpp`:** put the class declaration and its members in `ClassName.h` and the member implementations in `ClassName.cpp`. Use **`#pragma once`** in the header to prevent multiple declarations.
  ```cpp
  // Point.h
  #pragma once
  class Point {
    public:
      Point(int x, int y);
      int getX() const;
      int getY() const;
      void translate(int dx, int dy);
    private:
      int _x;
      int _y;
  };

  // Point.cpp
  #include "Point.h"
  Point::Point(int x, int y) { _x = x; _y = y; }
  void Point::translate(int dx, int dy) { _x += dx; _y += dy; }
  ```
- **`class` vs. `struct`:** prefer `class`. Use `struct` only for a very small, simple type with a few public fields, such as `Point` or `LinkedListNode`.
- **Underscore prefix for private members:** for example `_elements`, `_x`.
- **Avoid unnecessary fields:** fields hold the object's important data, not temporary values used within a single call to one function.
- **Private helpers:** any member function you add that isn't in the assignment spec should be **`private`** so outside code can't call it.
  ```cpp
  class Student {
      ...
    private:
      double computeTuitionHelper();
  };
  ```
- **`const` member functions** *(optional, not covered this quarter)*: declare a member function `const` if it doesn't modify the object.
  ```cpp
  int getID() const;
  double getGPA(int year) const;
  void payTuition(Course& course);   // modifies state → not const
  string toString() const;
  ```

---

## 9. Commenting

### General principles
- **Good names and straightforward algorithms are the best documentation.** Let the code speak for itself where it can.
- Comment where things get complex, but **don't write lots of low-content comments** explaining self-evident code.
- **Audience:** a C++-literate programmer. **Don't explain how C++ works or basic programming techniques.**
- **When to write comments is up to you** (before, during, or after coding). Whenever you write them, **the final comments must match the final code.** A comment that says one thing while the code does another is especially harmful. **Review all comments at the end** to make sure they're still accurate.

### Required and recommended comments

| Comment type | Requirement |
|---|---|
| **File/class header** | Every file gets an overview comment describing its purpose. For assignments, include **your name, course/section, and a brief description of how the file relates to the assignment**. |
| **Citing sources** | If an external resource (web page, book, another person, etc.) **materially influenced** your code, you **must cite it** in a comment **at the top of the file**. Say explicitly what help you received and how and where it influenced your code. |
| **Function header** | Every function gets a header describing its behavior at a high level, plus: **(a) Parameters/return:** type and purpose of each parameter and of the return value; **(b) Preconditions/assumptions:** constraints the client should know about (e.g. "expects the file to be open for reading"); **(c) Errors:** special cases and error conditions handled (e.g. "raises error if divisor is 0", "returns NOT_FOUND if the word doesn't exist"). |
| **Inline comments** | Use **sparingly**, only where code is complex or unusual. **Explain what the code accomplishes, not what it says.** If the purpose is obvious, skip the comment. |

```cpp
// inline babbling just repeats what code already says, don't!
int counter;                // declare a counter variable
counter++;                  // increment counter
while (index < length)      // while index less than length
```

### Before you submit
- **Remove all `// TODO:` comments.**
- **Remove commented-out code.** Commenting code out while you work is fine, but don't submit large chunks of it. Delete code you no longer need.
- **Doc comments (`/** ... */`)** are allowed but **not required**. `//` and `/* ... */` are fine.

---

## Quick Pre-Submission Checklist

- [ ] Consistent indentation (2–4 spaces, one level per brace), one statement per line
- [ ] No line over 100 characters; wrapped lines aligned
- [ ] Spaces around operators; parentheses where precedence is unclear
- [ ] Blank lines between functions and logical blocks
- [ ] Nouns for variables, verbs for functions, `is`/`get` for property getters
- [ ] No type in variable names, no one-letter names (except `i`/`j`/`x`/`y`), never `l`
- [ ] No magic numbers; `UPPER_CASE` named constants
- [ ] camelCase variables/functions, PascalCase types
- [ ] Narrowest scope, no shadowing, **no globals**
- [ ] C++ idioms (`cout`, `string`), not C (`printf`, `char*`)
- [ ] `for` for definite loops, `while` for indefinite
- [ ] No `continue`; `break` only for loop-and-a-half or mid-iteration exit
- [ ] Every `switch` case ends in `break`/`return`, or has a fallthrough comment
- [ ] Single exit point, except base cases, upfront error checks, or loop exits
- [ ] Braces on **every** control-statement body
- [ ] No `++`/`--`/`+=` embedded in larger expressions
- [ ] No `== true`, no `if (x) return true; else return false;`
- [ ] `&&`, `||`, `!`, not `and`, `or`, `not`
- [ ] `error("...")`, not `throw`
- [ ] Best Big-O and the right ADT (Set for lookups, Hash* when order doesn't matter)
- [ ] Expensive call results cached; large objects passed by reference
- [ ] No duplicated code: helpers extracted, common branch code factored out
- [ ] Functions do one task, are ≤ 20–30 lines, and pass the "and" test
- [ ] References only when needed; single values returned, not passed out by reference
- [ ] `main` is a high-level outline with no call chaining
- [ ] File header (name, course/section, purpose) and source citations at the top
- [ ] Function headers (params/return, preconditions, errors)
- [ ] Inline comments explain *why/what*, not *how*; comments match the final code
- [ ] No `TODO`s, no commented-out code
