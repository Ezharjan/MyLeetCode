# Deconstructing the Logic Behind the Strong Password Checker [#420]

[Blog Post Link](https://leetcode.com/discuss/post/7558808/deconstructing-the-logic-behind-the-stro-dd2g/)



Welcome fellow coding enthusiasts! Today we are going to dive deep into a problem that looks simple on the surface but hides a beautiful labyrinth of logic underneath. We are talking about the Strong Password Checker. If you have ever tried to sign up for a website and been frustrated by those complex password requirements, you already know the vibe of this problem.

This challenge is categorized as Hard for a reason. It is not because it requires an obscure algorithm or advanced data structure, but because it demands a very careful handling of edge cases. It is the ultimate test of your ability to break a large problem down into manageable, logical pieces.

In this guide, I will walk you through the thought process step by step. We will move from understanding the basic rules to mastering the tricky optimization logic required for long passwords. By the end, you will have a clear mental model of how to solve this efficiently.

Let us get started!

### The Problem at a Glance

Imagine you are writing the backend logic for a user registration system. You need to validate a password string based on three specific rules.

**The Rules of Strength**

1. **Length Requirement**
The password must contain between 6 and 20 characters inclusive.
2. **Variety Requirement**
It must contain at least one lowercase letter, one uppercase letter, and one digit.
3. **Repetition Requirement**
It must not contain three repeating characters in a row. For example, "aaa" is invalid, but "a" or "aa" is acceptable.

Your goal is to calculate the **minimum number of steps** required to make a given string strong. A single step can be any of the following:

* Insert one character.
* Delete one character.
* Replace one character with another.

### Why Is This Tricky?

The complexity comes from how these operations interact. An insertion can fix a length issue and break a repetition sequence at the same time. A deletion might fix a length issue but change how many replacements you need for repetitions. A replacement is the most versatile tool for fixing variety and repetition issues without changing the length.

To solve this without getting a headache, we need to divide the problem based on the length of the input string. The strategy changes completely depending on whether the password is too short, just right, or too long.

We will analyze three distinct scenarios.

* **Scenario A** where the length is less than 6.
* **Scenario B** where the length is between 6 and 20.
* **Scenario C** where the length is greater than 20.

Let us explore each one in detail.

### Analyzing the Variables

Before jumping into the scenarios, we need to gather some data about our current password.

First, we check for **Missing Types**. We scan the string to see if we are missing a lowercase letter, an uppercase letter, or a digit. Let us call this count `missing_types`. This value will be between 0 (we have everything) and 3 (we have nothing but symbols or we have an empty string).

Second, we check for **Repeating Sequences**. We scan the string for any run of 3 or more identical characters. For every sequence of length `L`, we theoretically need `L / 3` replacements to break it up if we are not deleting or inserting.

* "aaa" (length 3) needs 1 replacement to become "aXa".
* "aaaaa" (length 5) needs 1 replacement to become "aaXaa".
* "aaaaaa" (length 6) needs 2 replacements to become "aaXaaX".

We sum up all these required replacements into a variable we will call `total_replace_count`.

Now, let us apply this to our scenarios.

### Scenario A: The Password is Too Short

If the length of the password is less than 6, our primary problem is the length. We are forced to insert characters to reach the minimum length of 6.

Let `n` be the current length. We need `6 - n` insertions.

Here is the good news: Insertions are incredibly powerful. When you insert a character, you can choose exactly what character to insert and where to put it. This means a single insertion can:

1. Increase the length by 1.
2. Provide a missing type (e.g., insert a Digit if you are missing one).
3. Break a repeating sequence (e.g., insert a generic char inside "aaa" to make "aaXa").

Because the maximum repetition length in a password shorter than 6 is 5 (e.g., "aaaaa"), the number of insertions required to fix the length is always sufficient to fix the repetition issues as well.

Therefore, for this scenario, the answer is simply the maximum of either the missing types or the required length increase.

**Formula for Scenario A**
`steps = max(missing_types, 6 - n)`

### Scenario B: The Length is Just Right

If the password length is between 6 and 20, we do not need to insert or delete anything to satisfy the length rule. We only care about the variety requirement and the repetition requirement.

In this range, we prefer **Replacement**. Why? Because replacement keeps the length constant.

We have two pressures here:

1. We need to fix `missing_types`.
2. We need to fix `total_replace_count` to break sequences.

Fortunately, one replacement action can solve both problems at once. If you have "aaa" (needs 1 replacement) and you are missing a Digit, you can replace the middle 'a' with '1'. Now you have "a1a". You fixed the sequence and the missing type in one move.

So, we simply need to do enough replacements to cover the repetition problems. If we still have missing types after that, we count those too. Mathematically, it is just the larger of the two numbers.

**Formula for Scenario B**
`steps = max(missing_types, total_replace_count)`

### Scenario C: The Password is Too Long

This is the most challenging part of the problem. If the length `n` is greater than 20, we **must** delete `n - 20` characters. There is no way around this.

The tricky part is that deletions can also help us reduce the number of replacements needed for repeating sequences, but not all deletions are created equal. We want to delete characters from repeating sequences in a way that minimizes the work we have to do later.

Let us look at how deletions affect sequences based on their length modulo 3.

**Category 0: Length implies remainder 0**
Consider a sequence like "aaaaaa" (6 chars).

* Current replacements needed: 2 (for "aaXaaX").
* If we delete 1 character, it becomes "aaaaa" (5 chars).
* New replacements needed: 1.
* **Benefit**: 1 Deletion saves 1 Replacement. This is highly efficient.

**Category 1: Length implies remainder 1**
Consider a sequence like "aaaa" (4 chars).

* Current replacements needed: 1.
* If we delete 1 character, it becomes "aaa" (3 chars).
* New replacements needed: 1.
* **Benefit**: 1 Deletion changed nothing regarding replacements.
* However, if we delete 2 characters, it becomes "aa" (2 chars).
* New replacements needed: 0.
* **Benefit**: 2 Deletions save 1 Replacement.

**Category 2: Length implies remainder 2**
Consider a sequence like "aaaaa" (5 chars).

* Current replacements needed: 1.
* If we delete 1 character, it becomes "aaaa" (4 chars). Still needs 1 replacement.
* If we delete 2 characters, it becomes "aaa" (3 chars). Still needs 1 replacement.
* If we delete 3 characters, it becomes "aa". Needs 0 replacements.
* **Benefit**: 3 Deletions save 1 Replacement.

**The Strategy for Deletion**
Since we are forced to delete characters anyway to get the length down to 20, we should use those forced deletions greedily to reduce our future replacement burden.

1. **Priority 1**: Use deletions on **Category 0** sequences. Every 1 char we delete here reduces the replacement count by 1. We do this until we run out of such sequences or run out of required deletions.
2. **Priority 2**: Use deletions on **Category 1** sequences. We need to delete 2 chars to save 1 replacement here.
3. **Priority 3**: Use deletions on **Category 2** sequences (or the leftovers from the previous steps). We need to delete 3 chars to save 1 replacement here.

After we have used up our mandatory deletions (`n - 20`), we recalculate the required replacements for whatever sequences remain. The final answer will be the mandatory deletions plus the remaining replacements (or missing types, whichever is higher).

**Formula for Scenario C**
`steps = deletions + max(remaining_replace_count, missing_types)`

### The Code Implementation

Now that we have the logic mapped out, let us look at the C++ solution. I have added detailed comments to help you connect the code back to the logic we just discussed.

```cpp
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    int strongPasswordChecker(string password) {
        int n = password.length();
        
        // Step 1: Check for missing types (Lower, Upper, Digit)
        bool hasLower = false, hasUpper = false, hasDigit = false;
        for (char c : password) {
            if (islower(c)) hasLower = true;
            else if (isupper(c)) hasUpper = true;
            else if (isdigit(c)) hasDigit = true;
        }
        
        // This variable holds how many types we are missing (0 to 3)
        int missing_types = (!hasLower) + (!hasUpper) + (!hasDigit);

        // Scenario A: Too Short
        if (n < 6) {
            // We need at least (6 - n) insertions.
            // These insertions can also fix missing types.
            // Repetition issues in short strings are automatically covered 
            // by the insertions needed for length.
            return max(6 - n, missing_types);
        }

        // Step 2: Analyze Repeating Sequences
        // We will count how many replacements are theoretically needed.
        // We also track sequences by their length % 3 to optimize deletions later.
        int replace_count = 0;
        int one_seqs = 0; // Sequences with length % 3 == 0
        int two_seqs = 0; // Sequences with length % 3 == 1

        for (int i = 0; i < n; ) {
            int len = 1;
            while (i + len < n && password[i + len] == password[i]) {
                len++;
            }
            
            if (len >= 3) {
                replace_count += len / 3;
                if (len % 3 == 0) one_seqs++;
                else if (len % 3 == 1) two_seqs++;
            }
            
            i += len;
        }

        // Scenario B: Good Length
        if (n <= 20) {
            // We just need to fix repetitions and missing types.
            // One replacement can fix both a repetition and a missing type.
            return max(replace_count, missing_types);
        }

        // Scenario C: Too Long
        // We must delete exactly (n - 20) characters.
        int delete_count = n - 20;
        
        // We try to use these deletions to reduce 'replace_count' efficiently.
        
        // Priority 1: Sequences with length % 3 == 0
        // Deleting 1 char reduces needed replacements by 1.
        if (delete_count > 0) {
             int len_reduced = min(delete_count, one_seqs);
             replace_count -= len_reduced;
             delete_count -= len_reduced;
        }
        
        // Priority 2: Sequences with length % 3 == 1
        // Deleting 2 chars reduces needed replacements by 1.
        if (delete_count > 0) {
            // Each sequence needs 2 deletes to save a replacement.
            // We can treat remaining deletes as pairs.
            int len_reduced = min(delete_count / 2, two_seqs);
            replace_count -= len_reduced;
            delete_count -= len_reduced * 2;
        }
        
        // Priority 3: General Sequences
        // Deleting 3 chars reduces needed replacements by 1.
        if (delete_count > 0) {
            replace_count -= delete_count / 3;
        }
        
        // Finally, add the mandatory deletions to the remaining replacements needed
        return (n - 20) + max(replace_count, missing_types);
    }
};

```

### Complexity Analysis

It is always important to understand the performance of our solution.

* **Time Complexity**: The solution runs in **Linear Time**, or O(N). We iterate through the password string a constant number of times. First to check for character types, and second to identify repeating sequences. The mathematical operations for the deletion logic are constant time relative to the number of sequences.
* **Space Complexity**: The solution uses **Constant Space**, or O(1). We store a few integer variables to keep track of counts, flags, and sequence types. We do not create any large arrays or copy the string, making this solution very memory efficient.

### Summary and Key Takeaways

Solving the Strong Password Checker is about recognizing that not all operations are equal.

1. **Context Matters**: The value of an "Insertion" or "Deletion" changes completely depending on the total length of the string.
2. **Greedy Optimization**: When the string is too long, we do not just delete random characters. We delete the ones that give us the highest "Return on Investment" for reducing future replacement work.
3. **Unified Fixes**: Always remember that one well placed change can fix multiple problems (like length, type, and repetition) simultaneously.

I hope this walkthrough helped clarify the logic behind this complex problem. It is a fantastic exercise in conditional logic and optimization. If you encounter this problem again, you will be ready to tackle it with confidence.

Happy Coding!
