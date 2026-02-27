# Unraveling The Mystery Of Counting Stepping Numbers With Adjacent Digits Differing By Exactly One [#2801]

Hello everyone! Welcome to another algorithms discussion. Today I want to share a walkthrough for a really fascinating string and counting problem that you might encounter on your coding journey. It is a fantastic exercise in breaking down large constraints into manageable states. I am excited to share my learnings with all of you! 

### Understanding The Core Question

Imagine a number where every single digit differs from the digit right next to it by exactly one. We call these numbers "stepping numbers". For example, 123 is a stepping number because the difference between 1 and 2 is one, and the difference between 2 and 3 is one. The number 454 is also valid. However, 124 fails because the gap between 2 and 4 is two. 

The task requires us to find the total count of these valid stepping numbers within an inclusive range bounded by two numbers, Low and High. 

Here is the catch. The upper bound, High, can have up to one hundred digits! That means the number could literally be 10 to the power of 100. This massive constraint immediately tells us that we cannot use standard loops. Counting one by one would take longer than the lifespan of the universe. 

We need a structured way to build these numbers digit by digit. This leads us directly into a technique called Digit Dynamic Programming.

### The Foundation Of Digit Dynamic Programming



Digit Dynamic Programming is a specialized technique used whenever we need to count how many numbers in a specific range satisfy a certain property, especially when those numbers are too massive to fit into standard integer data types. 

Instead of looking at the whole number, we construct the number from the leftmost digit to the rightmost digit. As we place each digit, we just make sure we are not breaking the rules. 

For our stepping numbers, the rule is simple. The absolute difference between the digit we just placed and the digit we are about to place must be exactly one. 

To solve the range query for limits Low and High, we use a classic prefix sum approach. We can write a helper function that counts all valid numbers from 1 up to a given boundary string. If we find the count from 1 up to High, and then subtract the count from 1 up to Low, we get the count between the two bounds. Because the subtraction removes the lower bound entirely, we simply check if Low itself is a valid stepping number and add one back to our total if it is. 

### Deep Dive Into The State Variables

To implement our construction process without doing redundant work, we need to memorize the states we have already computed. We call this memoization. But what defines a unique state in our process? We need four pieces of information.

* **The Current Index** We need to know which position in the string we are currently filling. If the string has a length of ten, our index will start at zero and travel all the way to nine. Reaching the end means we successfully built a valid number.

* **The Previous Digit** To satisfy the stepping condition, we must remember the digit we placed in the step just before the current one. If we placed a 4 previously, our current choices are restricted strictly to 3 or 5. When we are placing the very first digit, we can use a placeholder value like negative one to indicate that no digit has been placed yet.

* **The Tight Bound Flag** This boolean variable is crucial. It tells us if we are currently hugging the upper limit of our maximum number. For example, if our maximum boundary is 532 and we have already placed 5 as the first digit, our next digit cannot exceed 3. The tight flag ensures we never build a number larger than our given boundary. If we place a digit smaller than the allowed limit, the tight flag turns false for all subsequent steps, meaning we are free to place any valid digit up to 9.

* **The Leading Zero Flag** Numbers can have varying lengths. If our maximum boundary has five digits, we still need to count the valid numbers that only have three digits. We handle this by allowing leading zeros. However, leading zeros do not count towards the stepping condition. If we have 0045, the difference between 0 and 4 does not matter because 4 is the actual starting digit. This flag tells us if we are still placing leading zeros or if the actual number has begun.

### Designing The Transitions



Let us trace how we move from one state to the next. 

When we stand at a specific index, we must decide which digits from 0 to 9 we are allowed to place. We determine the upper limit based on our tight bound flag. If the tight flag is true, the highest digit we can place is the corresponding digit from our boundary string. Otherwise, we can go all the way up to 9.

We use a loop to iterate through all allowed digits. For each candidate digit, we check a few conditions before placing it.

First, we check the leading zero flag. If we are currently holding a leading zero and we try to place another zero, we simply move to the next index keeping the leading zero flag set to true. The tight flag updates naturally. 

If we are holding a leading zero and we place a nonzero digit, we have officially started our number! We move to the next index, pass our newly placed digit as the previous digit, and turn the leading zero flag to false.

What if the leading zero flag is already false? That means we are in the middle of building a solid number. We must strictly enforce the stepping rule. We calculate the absolute difference between our candidate digit and the previous digit. If the difference is exactly one, the placement is valid, and we proceed to the next index. If not, we discard the candidate digit and try the next one.

At each step, we accumulate the valid paths. Since the problem mentions the answer could be enormous, we must apply a modulo operation of 1000000007 (which is 10 to the power of 9 plus 7) to our running sum at every addition. This keeps our numbers well within standard integer limits and prevents overflow issues.

### Complete C++ Implementation

Below is the C++ code utilizing the logic we discussed. It uses a recursive top down approach with a multidimensional array for memoization.

```cpp
#include <string>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

class Solution {
    int MOD = 1e9 + 7;
    int memo[105][11][2][2];
    string s;

    int dp(int idx, int prev_digit, bool is_tight, bool is_leading_zero) {
        if (idx == s.length()) {
            if (is_leading_zero) {
                return 0;
            } else {
                return 1;
            }
        }

        if (memo[idx][prev_digit + 1][is_tight][is_leading_zero] != -1) {
            return memo[idx][prev_digit + 1][is_tight][is_leading_zero];
        }

        int limit = is_tight ? s[idx] - '0' : 9;
        long long ans = 0;

        for (int d = 0; d <= limit; d++) {
            bool next_tight = is_tight && (d == limit);
            
            if (is_leading_zero) {
                if (d == 0) {
                    ans = (ans + dp(idx + 1, -1, next_tight, true)) % MOD;
                } else {
                    ans = (ans + dp(idx + 1, d, next_tight, false)) % MOD;
                }
            } else {
                if (abs(d - prev_digit) == 1) {
                    ans = (ans + dp(idx + 1, d, next_tight, false)) % MOD;
                }
            }
        }

        return memo[idx][prev_digit + 1][is_tight][is_leading_zero] = ans;
    }

    int solve(string num_str) {
        s = num_str;
        memset(memo, -1, sizeof(memo));
        return dp(0, -1, true, true);
    }

    bool isStepping(string num) {
        for (int i = 1; i < num.length(); i++) {
            if (abs(num[i] - num[i - 1]) != 1) return false;
        }
        return true;
    }

public:
    int countSteppingNumbers(string low, string high) {
        int right = solve(high);
        int left = solve(low);
        
        int ans = (right - left + MOD) % MOD;
        
        if (isStepping(low)) {
            ans = (ans + 1) % MOD;
        }
        
        return ans;
    }
};

```

### Unpacking The Complexity Attributes

It is always important to evaluate how our code will perform under maximum stress. Let us review the time and space requirements for this solution.

**Time Execution Elements**
Our recursive function uses states defined by four variables.

* The index can range from 0 to 100.
* The previous digit ranges from negative one to 9, giving us 11 possibilities.
* The tight bound is a boolean, yielding 2 possibilities.
* The leading zero is also a boolean, yielding another 2 possibilities.

Multiplying these together gives us a total of roughly 4400 unique states. Inside each state, our loop runs a maximum of 10 times (checking digits 0 through 9). Evaluating these loops is extremely fast. We run this entire process twice, once for the lower bound and once for the upper bound. The final result is a time complexity of `Big O of N`, where N is the length of the string. In practical terms, this executes in a fraction of a millisecond and easily passes the strict evaluation timers.

**Memory Overhead Elements**
The memory profile is similarly lightweight. We declare a four dimensional integer array to hold our cached results. An array of size 105 by 11 by 2 by 2 consumes a negligible amount of memory. In addition, the maximum depth of our recursion tree will exactly match the length of the string, which caps out at 100 frames on the call stack. Therefore, the space complexity rests comfortably at `Big O of N`, making it incredibly safe against stack overflow errors.

### Community Wrap Up

I hope this detailed breakdown demystifies the magic of building numbers digit by digit! Understanding how to separate boundaries, manage leading zeros, and cache visited paths are skills that will serve you well across numerous similar puzzles. Please feel free to share your thoughts, questions, or alternative approaches in the comments section below. Happy coding and keep building those skills step by step!
