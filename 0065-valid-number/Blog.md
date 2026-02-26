# Validating Numeric Strings A Comprehensive Guide to Parsing Digits Exponents and Decimals [#65]

Hello everyone! Welcome to my detailed guide. Today, I am very excited to share my thoughts and learnings with the community regarding a fascinating string parsing challenge. String manipulation and validation are core skills in computer science, deeply tied to how compilers, interpreters, and data validation systems understand the text we type. 

In this blog post, we will explore a practical and reliable way to determine if a given string of text correctly represents a mathematical number. We will look at the exact rules that define valid numeric strings, break down the logic required to evaluate them character by character, and build a clean C++ solution together. My hope is that by the end of this read, you will feel completely comfortable handling similar text parsing problems. Let us dive right in!

***

### Understanding the Problem

Before writing any code, we must clearly define what makes a string a valid number according to our specific rules. The computer only sees a sequence of characters, and it is our job to instruct it on which sequences are mathematically meaningful and which are just gibberish.

A valid number string can fall into a few categories:
* **Integer Numbers:** These are your standard whole numbers. They consist of a sequence of digits from 0 to 9. They may optionally begin with a plus character or a minus character (ASCII 45) to indicate their sign. Examples include "42", "0", and a plus sign followed by "123".
* **Decimal Numbers:** These numbers include a fractional component denoted by a dot character. A decimal number can start with digits followed by a dot, digits followed by a dot followed by more digits, or just a dot followed by digits. Just like integers, they can optionally start with a sign character. Examples include "3.14", ".9", and "4.".
* **Scientific Notation (Exponents):** Numbers can be expressed in scientific notation using the letter 'e' or 'E'. This notation represents a base number multiplied by ten raised to a given power. For our rules, an exponent consists of an integer or decimal number, followed by 'e' or 'E', followed by an integer. Examples include "2e10" or "53.5e93".

Strings that contain unexpected letters, multiple dots, misplaced sign characters, or missing digits are considered invalid. For instance, "abc" is entirely letters, "1a" contains an unexpected letter, and "e3" is missing a base number before the exponent.

Our goal is to write a function that takes a string as input and returns a boolean value true if the string perfectly adheres to the rules, and false otherwise.

***

### The Core Strategy State Tracking

When tackling a string parsing problem like this, a highly effective approach is to read the string exactly once from left to right, keeping track of what we have seen so far. By remembering our history, we can make informed decisions about whether the current character we are looking at is allowed.

We can accomplish this by maintaining a few simple boolean variables, often referred to as flags. These flags will act as our memory. Let us define the three core flags we need:

1.  **The Digit Flag (`seenDigit`):** This flag tells us if we have encountered at least one numerical digit so far. This is crucial because a completely empty string or a string containing only a dot or a sign is not a valid number. We must see digits for the number to be valid.
2.  **The Decimal Flag (`seenDot`):** This flag tells us if we have already encountered a decimal dot. Since a valid number cannot have more than one decimal point, seeing a second dot while this flag is true means the string is invalid. Furthermore, an exponent strictly requires an integer power, so we are not allowed to see a dot after we have seen an 'e' or 'E'.
3.  **The Exponent Flag (`seenExponent`):** This flag tells us if we have seen the 'e' or 'E' character. Just like the decimal point, we can only have at most one exponent character. Also, an exponent character is only valid if we have already seen at least one digit beforehand.

By continually updating these flags and checking them against the character we are currently evaluating, we can build a robust set of rules.

***

### Formulating the Rules Character by Character

Let us establish the exact rules for each type of character we might encounter in the string. We will process each character in a loop.

**Rule for Digits (0 to 9):**
Whenever we see a digit, we simply mark our `seenDigit` flag as true. Digits are generally always welcome, so there are no strict preconditions for encountering a digit.

**Rule for Sign Characters (Plus and ASCII 45):**
Sign characters are tricky. They cannot just appear anywhere. A sign character is strictly allowed in only two specific locations:
* It can be the very first character of the entire string.
* It can be the character immediately following an 'e' or 'E' character, because the exponent's power is an integer, which can be positive or negative.
If we see a sign character anywhere else, the string is invalid.

**Rule for Decimal Dots:**
When we encounter a dot, we must check our memory flags. If we have already seen a dot previously, the string is invalid because multiple decimal points are not allowed. Additionally, if we have already seen an exponent character, the string is invalid because the power in scientific notation must be a whole integer, not a decimal. If neither of these conditions is met, we mark `seenDot` as true and continue.

**Rule for Exponent Characters ('e' or 'E'):**
When we find an 'e' or 'E', we again consult our flags. If we have already seen an exponent, it is invalid. If we have not seen any digits yet, it is invalid because an exponent needs a base number. If it is valid, we mark `seenExponent` as true. 
Here is a very important detail: when we see a valid exponent character, we must immediately reset our `seenDigit` flag to false. Why? Because a valid scientific notation strictly requires digits *after* the exponent. By resetting the flag, we force the program to find more digits before finishing. If the string ends immediately after the 'e', our final check will notice that `seenDigit` is false, correctly identifying the string as invalid.

**Rule for Anything Else:**
If the character does not fit into any of the above categories (for example, it is a letter like 'a' or a special symbol), the string is completely invalid, and we can immediately halt our checks.

***

### Step by Step Walkthroughs

To ensure total clarity, let us trace through our logic with a few detailed examples. We will simulate our program's memory at each step.

#### Example A Examining a simple decimal string "+3.14"

* **Initialization:** `seenDigit` is false, `seenDot` is false, `seenExponent` is false. We will also track the previous character to help validate signs, let us call it `prevChar` and initialize it to a null character.
* **Character 1 (Plus sign):** This is a sign character. It is at the very beginning of the string (the first position), so it is completely valid. We update `prevChar` to the plus sign.
* **Character 2 ('3'):** This is a digit. We set `seenDigit` to true. `prevChar` becomes '3'.
* **Character 3 (Dot):** This is a decimal. We check our flags: `seenDot` is false, and `seenExponent` is false. This is perfectly valid. We set `seenDot` to true. `prevChar` becomes a dot.
* **Character 4 ('1'):** This is a digit. `seenDigit` remains true. `prevChar` becomes '1'.
* **Character 5 ('4'):** This is a digit. `seenDigit` remains true. `prevChar` becomes '4'.
* **End of String:** We reach the end. Our final check ensures `seenDigit` is true. Since it is true, the string is a valid number.

#### Example B Examining scientific notation "2e10"

* **Initialization:** `seenDigit` is false, `seenDot` is false, `seenExponent` is false. `prevChar` is a null character.
* **Character 1 ('2'):** This is a digit. `seenDigit` becomes true. `prevChar` becomes '2'.
* **Character 2 ('e'):** This is an exponent. We check flags: `seenExponent` is false, and `seenDigit` is true. This is valid. We set `seenExponent` to true. Crucially, we reset `seenDigit` to false. `prevChar` becomes 'e'.
* **Character 3 ('1'):** This is a digit. `seenDigit` becomes true again. `prevChar` becomes '1'.
* **Character 4 ('0'):** This is a digit. `seenDigit` remains true. `prevChar` becomes '0'.
* **End of String:** We reach the end. `seenDigit` is true. The string is valid.

#### Example C Examining an invalid string "99e2.5"

* **Initialization:** `seenDigit` is false, `seenDot` is false, `seenExponent` is false. `prevChar` is null.
* **Character 1 ('9'):** Digit. `seenDigit` is true. `prevChar` is '9'.
* **Character 2 ('9'):** Digit. `seenDigit` is true. `prevChar` is '9'.
* **Character 3 ('e'):** Exponent. Valid since `seenDigit` is true and `seenExponent` is false. Set `seenExponent` to true. Reset `seenDigit` to false. `prevChar` is 'e'.
* **Character 4 ('2'):** Digit. `seenDigit` becomes true. `prevChar` is '2'.
* **Character 5 (Dot):** Decimal. We check flags: `seenDot` is false, BUT `seenExponent` is currently true! Our rules dictate that decimals cannot appear after exponents. The logic immediately flags this as invalid and stops.

#### Example D Examining an invalid trailing exponent "5e"

* **Initialization:** `seenDigit` is false, `seenDot` is false, `seenExponent` is false. `prevChar` is null.
* **Character 1 ('5'):** Digit. `seenDigit` is true. `prevChar` is '5'.
* **Character 2 ('e'):** Exponent. Valid. Set `seenExponent` to true. Reset `seenDigit` to false. `prevChar` is 'e'.
* **End of String:** The loop finishes. We check our final condition: is `seenDigit` true? Because we reset it at the 'e' character and never found more digits, `seenDigit` is false. The string is correctly identified as invalid.

***

### The C++ Implementation

Now that we have thoroughly mapped out the logic, translating it into C++ is a smooth process. To ensure our code compiles flawlessly and avoids certain syntax complexities, we will use the ASCII value 45 to represent the minus character. We will also track the previous character in a dedicated variable rather than trying to look backwards into the string array, which helps prevent accidental out of bounds memory access.

Here is a clean and straightforward implementation you can study and use:

```cpp
class Solution {
public:
    bool isNumber(string s) {
        bool seenDigit = false;
        bool seenDot = false;
        bool seenExponent = false;
        
        // We use prevChar to track the character from the previous iteration
        // This helps us validate sign characters safely
        char prevChar = 0; 

        for (int i = 0; i < s.length(); i++) {
            char c = s[i];
            
            if (c >= '0' && c <= '9') {
                // If it is a digit, we record that we have seen one
                seenDigit = true;
            } 
            else if (c == '+' || c == 45) { // Note: 45 is the ASCII value for the minus character
                // A sign is only valid at the very beginning of the string
                // OR immediately following an exponent character
                if (i > 0 && prevChar != 'e' && prevChar != 'E') {
                    return false;
                }
            } 
            else if (c == '.') {
                // A decimal is completely invalid if we have already seen a decimal
                // or if we have already seen an exponent
                if (seenDot || seenExponent) {
                    return false;
                }
                seenDot = true;
            } 
            else if (c == 'e' || c == 'E') {
                // An exponent is invalid if we already saw one
                // or if we have not seen any base digits yet
                if (seenExponent || !seenDigit) {
                    return false;
                }
                seenExponent = true;
                
                // Crucial step: reset seenDigit because an exponent 
                // strictly requires an integer to follow it
                seenDigit = false; 
            } 
            else {
                // Any other character (letters, spaces, special symbols) 
                // makes the entire string invalid
                return false;
            }
            
            // Update prevChar for the next loop iteration
            prevChar = c;
        }
        
        // If we end the string with a dangling 'e' or a dangling sign, 
        // seenDigit will be false, accurately failing the validation.
        return seenDigit;
    }
};

```

---

### Understanding the Efficiency and Complexity

When designing solutions, it is essential to discuss how the program behaves as the size of the input grows. We measure this in terms of Time Complexity and Space Complexity.

**Time Complexity Analysis**
The Time Complexity of this solution is Big O of N, where N represents the total number of characters in the input string.
This is because our code utilizes a single loop that reads through the string exactly one time, moving steadily from the first character to the last. Inside the loop, we only perform simple character comparisons and boolean assignments. These operations take a constant amount of time regardless of the string size. Therefore, the total time taken scales perfectly linearly with the length of the string.

**Space Complexity Analysis**
The Space Complexity of this solution is Big O of 1, which signifies constant space.
Regardless of whether the input string has five characters or five million characters, our algorithm only allocates memory for exactly four small variables: `seenDigit`, `seenDot`, `seenExponent`, and `prevChar`. We do not create any new arrays, vectors, or strings. Because the memory required never grows alongside the input size, the space efficiency is fantastic.

---

### Alternative Thoughts Deterministic Finite Automatons

While our boolean flag approach is remarkably clear and highly suited for coding interviews, there is another academic concept often applied to this exact problem known as a Deterministic Finite Automaton.

Imagine a board game with multiple distinct zones or "states". You start at the beginning state. Every time you read a character from the string, you follow a specific path to move to a new state based on that character. For example, reading a digit might keep you in the "Integer State", while reading a dot moves you to a "Decimal State".

If you read a character that has no valid path from your current state (like reading a letter while in the Integer State), the machine breaks, and the string is invalid. If you successfully process all characters and end up on a state that is marked as an "Accepting State" (meaning the rules are satisfied), the string is valid.

Building a State Machine involves creating a large mapping table of states and transitions. While it is theoretically elegant and powers industrial text processing tools, writing out the entire transition table by hand can be tedious and prone to typing errors during a time constrained scenario. The boolean flag method we built together achieves the exact same underlying logic but uses simple readable variables instead of complex transition tables. It is a highly practical translation of State Machine theory into everyday programming.

---

### Concluding Thoughts

Thank you so much for reading through this extensive guide! String parsing problems can initially feel overwhelming due to the sheer number of edge cases and tiny details to track. However, by taking a step back, organizing our rules clearly, and using a few simple flags to remember our history, we can conquer even the most intricate validation requirements.

I hope this breakdown brings you confidence in your problem solving journey. Keep practicing, keep breaking down complex problems into small logical rules, and most importantly, keep sharing your knowledge with the community. Happy coding everyone!
