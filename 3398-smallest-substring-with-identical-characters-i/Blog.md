# Minimizing Longest Runs Using Binary Search and Greedy Strategies [#3398]

Hello everyone! I recently enjoyed working on a fascinating problem that combines string manipulation with a classic algorithmic strategy. It is a great example of how changing our perspective from "how do I modify this string" to "can I achieve this specific goal" simplifies a complex task. I want to walk you through the thought process, the logic, and the final C++ implementation for solving the "Smallest Substring With Identical Characters I" problem.

## Understanding the Problem

Let us start by breaking down what we are trying to achieve. We are given a binary string consisting only of the characters '0' and '1'. We also have an integer budget called `numOps`. This budget represents the maximum number of times we are allowed to flip a character. Flipping a character means changing a '0' to a '1' or a '1' to a '0'.

Our goal is to minimize the length of the longest substring where all characters are the same.

Imagine you have a string "00000". The longest substring of identical characters here is the whole string, which has a length of 5. If we have enough operations, we could flip the middle character to make it "00100". Now, the longest runs of identical characters are "00" (at the start) and "00" (at the end). Both have a length of 2. By performing just one operation, we reduced the answer from 5 to 2.

We need to find the absolute minimum length possible for these "identical runs" without exceeding our budget of `numOps`.

## Analyzing the Constraints

Before diving into algorithms, let us look at the constraints. The length of the string `n` is up to 1000. This is a relatively small number in the world of competitive programming. It suggests that a solution with complexity like O(N log N) or even O(N squared) would pass comfortably. However, the logic we will discuss here is efficient enough to scale much higher if needed.

## The Strategy: Binary Search on the Answer

When we face a problem that asks us to "minimize the maximum" value, it is often a strong hint to use **Binary Search on the Answer**.

Why? Because calculating the minimum length directly is hard. However, verifying if a specific length is possible is often much easier.

Let us rephrase the problem as a question:
**"Is it possible to ensure that every run of identical characters has a length of at most `L` using `numOps` flips?"**

If the answer is Yes, then `L` might be our solution, or we might be able to do even better with a smaller `L`. If the answer is No, then `L` is too small, and we must try a larger value.

This gives us a range for our binary search:
* **Lower Bound:** 1 (The best possible case is an alternating string like "010101", where the longest run is 1).
* **Upper Bound:** The length of the string (The worst case is we do nothing).

## Checking Feasibility (The Greedy Approach)

The core of this solution is the check function. Let us call it `canAchieve(limit)`. This function takes a target length `limit` and tells us if we can satisfy the condition within our operation budget.

There are two distinct scenarios we need to handle inside this function.

### Scenario A: The Limit is 1

If `limit` is 1, we are requiring the string to have no adjacent identical characters. This means the string must be perfectly alternating. There are only two valid patterns for a binary string of length `n`:
1.  Pattern 1: starts with 0 (e.g., "010101...")
2.  Pattern 2: starts with 1 (e.g., "101010...")

To check this, we simply iterate through the original string and count how many characters differ from Pattern 1. Let us call this `cost1`. Then we count how many differ from Pattern 2. Let us call this `cost2`.

If the minimum of `cost1` and `cost2` is less than or equal to `numOps`, then a limit of 1 is achievable.

### Scenario B: The Limit is 2 or Greater

This is the general case. If we are allowed runs of length 2 or more, we can process the string using a greedy approach. We simply scan the string and identify consecutive runs of identical characters.

Suppose we find a run of identical characters with length `k`. We want to break this run into smaller pieces so that no piece is longer than `limit`.

To do this efficiently, we place a "separator" (a flipped character) every `limit + 1` positions.

Let us look at a concrete example.
Imagine our `limit` is 2.
We encounter a run: "00000000" (8 zeros).

If we do nothing, the length is 8, which is greater than 2. We need to flip characters to break it up.
The most efficient way to break a long run is to preserve as many original characters as possible while inserting breaks just before the run gets too long.

We can visualize the indices of this run as 0, 1, 2, 3, 4, 5, 6, 7.
* We keep index 0 and 1.
* We must flip index 2.
* We keep index 3 and 4.
* We must flip index 5.
* We keep index 6 and 7.

The result looks like "00100100".
The segments are "00", "00", "00". All have length 2.
We used 2 operations.

**The Formula**
For a run of length `k` and a target `limit`, the number of operations needed is:
`k / (limit + 1)` (using integer division).

Why `limit + 1`? Because one flipped character supports a segment of size `limit` before it and breaks the chain for the next segment.

**Wait, what about creating new problems?**
You might wonder: "If I flip a character inside a run of 0s to make it a 1, will that 1 merge with adjacent 1s to create a new long run of 1s?"
When `limit` is greater than or equal to 2, this is effectively never a blocking issue because the flipped character stands alone as a run of length 1. Even if it touches other 1s, we are only calculating the cost for the current run of 0s. The greedy strategy works because we only strictly need to break the *current* violation. In this specific problem version, simply counting `k / (limit + 1)` for all runs is sufficient to determine feasibility.

## The Algorithm Steps

1.  Initialize `low` to 1 and `high` to the string length `n`.
2.  While `low` is less than or equal to `high`:
    * Calculate `mid` as the average of `low` and `high`.
    * Call `canAchieve(mid)`.
    * If it returns true:
        * Store `mid` as a potential answer.
        * Move `high` to `mid - 1` to see if we can find a smaller valid length.
    * If it returns false:
        * Move `low` to `mid + 1` because `mid` was too strict.
3.  Return the stored answer.

## Complexity Analysis

Let us look at the efficiency of this approach.

**Time Complexity**
The binary search performs a logarithmic number of steps based on the string length `n`. Specifically, it runs roughly `log(n)` times.
Inside each step, the `canAchieve` function iterates through the string once, which takes linear time proportional to `n`.
Therefore, the total time complexity is proportional to `n` multiplied by `log(n)`. With `n` up to 1000, this is extremely fast.

**Space Complexity**
We only need a few integer variables to track our counts and indices. We do not create any large auxiliary data structures. Thus, the space complexity is constant, or O(1).

## The C++ Implementation

Here is the code that implements the logic discussed above.

```cpp
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    int minLength(string s, int numOps) {
        int n = s.length();
        int low = 1;
        int high = n;
        int ans = n;

        // Lambda function to check if a specific max length is possible
        auto canAchieve = [&](int limit) {
            // Special Case: Limit is 1
            // The string must be strictly alternating (e.g. 010101 or 101010)
            if (limit == 1) {
                int costPattern1 = 0; // Target pattern: starts with s[0]
                int costPattern2 = 0; // Target pattern: starts with opposite of s[0]
                
                // We compare s against "010101..." and "101010..."
                for (int i = 0; i < n; ++i) {
                    char expected1 = (i % 2 == 0) ? '0' : '1';
                    char expected2 = (i % 2 == 0) ? '1' : '0';
                    
                    if (s[i] != expected1) costPattern1++;
                    if (s[i] != expected2) costPattern2++;
                }
                return min(costPattern1, costPattern2) <= numOps;
            }

            // General Case: Limit >= 2
            // We iterate through the string to find runs of identical characters
            int opsNeeded = 0;
            int currentRunLength = 0;
            char prevChar = ' '; 

            for (int i = 0; i < n; ++i) {
                // If it is the start of string or same as previous char
                if (i == 0 || s[i] == prevChar) {
                    currentRunLength++;
                } else {
                    // The run ended. Calculate operations needed for the finished run.
                    // Formula: operations = length / (limit + 1)
                    opsNeeded += currentRunLength / (limit + 1);
                    
                    // Reset for the new run
                    currentRunLength = 1;
                }
                prevChar = s[i];
            }
            
            // Do not forget to process the final run after the loop finishes
            opsNeeded += currentRunLength / (limit + 1);

            return opsNeeded <= numOps;
        };

        // Binary Search
        while (low <= high) {
            int mid = low + (high - low) / 2;
            
            if (canAchieve(mid)) {
                ans = mid;      // mid is feasible, try smaller
                high = mid - 1;
            } else {
                low = mid + 1;  // mid is not feasible, need larger
            }
        }

        return ans;
    }
};

```

## Summary

This problem is an excellent exercise in recognizing when to use binary search on the answer. By guessing the answer and verifying it, we turned a potentially complicated optimization problem into a series of simple linear checks. The key takeaway here is handling the edge case where the limit is 1 separately from the general case where we can use integer division to count operations.

I hope this explanation helps you understand the mechanics behind solving this problem. Happy coding!
