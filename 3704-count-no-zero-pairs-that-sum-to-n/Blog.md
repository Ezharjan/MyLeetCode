# Mastering the Art of Counting No Zero Pairs Summing to N [#3704]

[Blog Post Link](https://leetcode.com/discuss/post/7553723/mastering-the-art-of-counting-no-zero-pa-azy6/)


Hello there fellow coding enthusiasts! It is wonderful to have you here reading this deep dive into an absolutely fascinating algorithmic challenge. Today I want to share a journey through a problem that looks deceptively simple on the surface but holds a treasure trove of logic puzzles underneath. We are going to explore how to count pairs of integers that sum up to a specific target  with a very peculiar twist regarding the digit zero.

If you have ever found yourself staring at a problem involving huge numbers and digit constraints, wondering how on earth to process it efficiently, this guide is for you. We will unravel a technique known as Digit DP (Dynamic Programming on Digits) and apply it to solve this puzzle efficiently.

Grab a cup of coffee and let us crack this code together!

## The Puzzle of No Zero Integers

Let us start by defining exactly what we are trying to solve. The problem asks us to find the number of pairs  that satisfy two main conditions.

First, the sum of  and  must equal a given integer. That is simply.

Second, and this is the tricky part, both  and  must be "no zero" integers. A no zero integer is a positive integer that does not contain the digit `0` anywhere in its decimal representation. For example, the number `123` is valid. The number `105` is invalid because it has a zero in the middle. The number `10` is also invalid.

Here are a few examples to help visualize this.

Suppose  is `11`. We need pairs that add up to `11` where neither number contains a zero.
The pair  sums to `11`, but `10` has a zero. So this pair is invalid.
The pair  sums to `11`, and neither `2` nor `9` has a zero. This is a valid pair.
The pair  is also valid.

Our goal is to return the total count of such valid pairs.

The constraints are what make this a "Hard" difficulty problem. The input  can be as large as. This is a massive number, meaning we cannot simply loop through all numbers from `1` to. A simple loop would take millions of years to finish! We need an approach that is much faster, something that works based on the number of digits rather than the magnitude of the number itself.

## Why Brute Force Fails

Imagine trying to check every single number. You would start with , calculate , check if  or  has a zero, and then move to.

For small inputs like , this works instantly. But when  has 15 digits, the search space is astronomical. The time complexity for a brute force solution is linear, or , which is unacceptable here. We need logarithmic time complexity, or. This specifically hints that we should process the number digit by digit.

## The Strategy Digit Dynamic Programming

The optimal way to solve this is using Digit Dynamic Programming. If you are new to this, do not worry! The concept is intuitive once you visualize how we do addition manually.

Think about how you add two numbers on paper. You write them one above the other and add the digits column by column, starting from the rightmost digit (the ones place) and moving to the left. If the sum in a column exceeds `9`, you carry the `1` over to the next column.

We can reverse engineer this process. We know the target sum. We can iterate through the digits of  from right to left (least significant to most significant) and try to determine which digits for  and  could possibly produce the current digit of.

### The Core Logic

At any specific digit position (let us call it `index`), we need to choose a digit for number  (let us call it `val_a`) and a digit for number  (let us call it `val_b`).

These two digits, plus any `carry` coming from the previous position (the position to the right), must result in the current digit of  plus potentially a new carry passed to the next position.

The equation for every column is essentially:


We will try every possible combination of `val_a` and `val_b` (from `0` to `9`) to see which ones satisfy this equation.

### The Problem with Zero

Wait a minute. The problem says "no zero integers." Does that mean `val_a` and `val_b` can never be `0`?

Not exactly! This is the subtle trap of the problem.

Consider the pair  summing to `11`.
Now consider. One valid pair might be.
If we treat these numbers as strings of equal length to , they look like this:




Notice the leading zero? The number `52` does not contain a zero in its meaningful representation, but if we align it with `105`, we mathematically pad it with a zero at the front.

This leads to a crucial rule for our DP state:
**We are allowed to place a `0` for  or  ONLY if that number has "stopped" or ended.**

If a number has already ended (meaning all subsequent digits to the left are just leading zeros), then placing another `0` is perfectly fine. It is just another leading zero. However, if the number is still "active" (meaning we have placed non zero digits or we are at the very start of the number generation), placing a `0` effectively terminates the number. You cannot place a `0` and then later place a `5` to the left of it, because that would mean the number had an internal zero (like `50...`), which is forbidden.

So, placing a `0` transitions a number from "active" to "stopped." Placing any digit from `1` to `9` keeps the number "active."

## Designing the DP State

To implement this, we need a function (let us call it `dp`) that remembers our progress. We need four pieces of information to define our state uniquely.

1. **Index:** Which digit position of  are we currently considering? We start from `0` (the ones place) and go up to the length of.
2. **Carry:** What is the carry value coming into this position from the right? This will be either `0` or `1`.
3. **Stopped A:** A boolean flag. Is number  finished? If `true`, then `val_a` must be `0` (leading zero). If `false`, `val_a` can be `1` through `9`, or it can be `0` (which would set this flag to `true` for the next position).
4. **Stopped B:** A boolean flag with the same logic for number.

The function signature looks something like this:
`long long dp(int index, int carry, bool stopped_a, bool stopped_b)`

### The Transitions

For each state, we iterate through possible values for the `next_carry`. Since we are adding two numbers, the carry can only be `0` or `1`.

Once we pick a `next_carry`, we calculate the `required_sum` needed from `val_a` and `val_b` using our algebra from earlier:


Now we iterate through all possible digits for `val_a`.

* If `stopped_a` is true, `val_a` MUST be `0`.
* If `stopped_a` is false, `val_a` can be anything from `0` to `9`. Note that at `index=0` (the ones place), `val_a` cannot be `0` because positive integers cannot end with a zero if they are single digits, and more importantly, if we put `0` at the ones place while the number is active, it violates the "no zero" rule. Actually, let us refine that: `0` is only allowed as a leading zero. A number like `10` is invalid. So if we are at the very first position we process (the ones place), we cannot pick `0` unless the number is just empty (which is not allowed as ). So effectively, at `index=0`, `val_a` is in range `[1, 9]`. At `index > 0`, if we pick `0`, the number becomes "stopped".

Once we pick `val_a`, we calculate `val_b = required_sum - val_a`.
We check if `val_b` is a valid digit (between `0` and `9`) and if it follows the same logic for `stopped_b`.

If everything is valid, we add the result of the recursive call `dp(index + 1, ...)` to our total count.

## Detailed Step by Step Walkthrough

Let us trace a very small example to cement the understanding.
Suppose.
We call `dp(0, 0, false, false)`.

**Index 0 (Ones place):**
Current digit of  is `2`. `carry` is `0`.
We try `next_carry = 0`.
Equation: `val_a + val_b + 0 = 2 + 10*0` implies `val_a + val_b = 2`.

Valid pairs  summing to `2`:

1. : Invalid because at index `0`, numbers cannot be `0` (must be positive integers).
2. : Valid! `val_a=1`, `val_b=1`. Both numbers remain active (not stopped).
3. : Invalid for same reason as number 1.

So we take the path. We move to `Index 1`.
`dp(1, 0, false, false)`

**Index 1:**
We have processed all digits of  ( has length 1).
However, we must ensure we have not left any hanging carries or unfinished numbers.
Usually, we loop up to `length of n`. If we reach the length and `carry` is `0`, we found a valid combination!
Wait, actually, since we are building numbers potentially longer than  (though unlikely for sum), we generally process until we cover all digits of.
Since  is exhausted and carry is 0, we return `1`.

Total count is 1. Correct!

## The Solution Code

Here is the complete C++ solution. I have written this to be as clear as possible, avoiding short variable names where descriptive ones help.

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

class Solution {
public:
    // We store the digits of n here for easy access
    string S;
    
    // Memoization table to store results of our subproblems
    // Dimensions: [Index][Carry][StoppedA][StoppedB]
    // Index: up to 18 (enough for 10^15)
    // Carry: 0 or 1
    // StoppedA: 0 (False) or 1 (True)
    // StoppedB: 0 (False) or 1 (True)
    long long memo[20][2][2][2];

    long long dp(int idx, int carry, bool sa, bool sb) {
        // Base Case: When we have processed all digits of n
        if (idx == S.size()) {
            // We found a valid pair ONLY if there is no remaining carry.
            // If carry is 1, it means a + b > n, which is wrong.
            // Also, we implicitly assume leading zeros for positions beyond S.size()
            // which is handled by the 'stopped' logic naturally.
            return carry == 0;
        }
        
        // Check if we have already computed this state
        if (memo[idx][carry][sa][sb] != -1) {
            return memo[idx][carry][sa][sb];
        }

        long long count = 0;
        int current_digit_n = S[idx] - '0';

        // We iterate over the possible carry we send to the NEXT position
        // The carry can only be 0 or 1
        for (int next_carry = 0; next_carry <= 1; ++next_carry) {
            
            // The Fundamental Equation of Addition:
            // val_a + val_b + current_carry = current_digit_n + (10 * next_carry)
            
            // We rearrange to find what val_a + val_b must equal:
            int current_sum_needed = current_digit_n + (10 * next_carry) - carry;

            // Simple check: Two single digits can sum to at most 9+9=18.
            // If the needed sum is outside [0, 18], this carry choice is impossible.
            if (current_sum_needed < 0 || current_sum_needed > 18) continue;

            // Now we need to find pairs (val_a, val_b) that sum to current_sum_needed.
            // We iterate through all valid digits for val_a.
            
            // Determine the range for val_a
            int start_a = 0;
            int end_a = 9;

            // Constraint: If 'a' has already stopped, it MUST be 0 (leading zero)
            if (sa) {
                start_a = 0;
                end_a = 0;
            } 
            // Constraint: At the very first position (ones place), 'a' cannot be 0
            // because a no zero integer cannot effectively end at the ones place 
            // without being just "0", which is not positive.
            else if (idx == 0) {
                start_a = 1;
            }

            for (int val_a = start_a; val_a <= end_a; ++val_a) {
                int val_b = current_sum_needed - val_a;

                // Check if the calculated val_b is a valid single digit
                if (val_b < 0 || val_b > 9) continue;

                // Constraint Check for b:
                // If 'b' is stopped, it MUST be 0. 
                if (sb && val_b != 0) continue;
                
                // If index is 0, 'b' cannot be 0 (same logic as 'a')
                if (idx == 0 && val_b == 0) continue;

                // Now we determine the new states for the next recursive step.
                // If we were not stopped, but we picked 0, we are NOW stopped.
                bool next_sa = sa || (val_a == 0);
                bool next_sb = sb || (val_b == 0);

                // Add the results from the next state
                count += dp(idx + 1, next_carry, next_sa, next_sb);
            }
        }

        // Store and return the result
        return memo[idx][carry][sa][sb] = count;
    }

    long long countNoZeroPairs(long long n) {
        S = to_string(n);
        // We reverse the string to process from Ones place (index 0) to MSB
        reverse(S.begin(), S.end());
        
        // Initialize the memoization table with -1
        memset(memo, -1, sizeof(memo));
        
        // Start the recursion
        return dp(0, 0, false, false);
    }
};

```

## Complexity Analysis

Let us break down why this is so much faster than the brute force approach.

**Time Complexity:**
The dynamic programming state is defined by four variables:

1. `index`: This goes from `0` to roughly `15` (the number of digits in ).
2. `carry`: This has `2` states (`0` or `1`).
3. `stopped_a`: This has `2` states.
4. `stopped_b`: This has `2` states.

The total number of unique states is approximately , which equals `128` states. This is incredibly small! Inside each state, we perform a constant amount of work (looping through carries and digits, at most roughly `20` iterations). Therefore, the total operations are effectively constant relative to the magnitude of. The complexity is logarithmic with respect to , written as.

**Space Complexity:**
We use a memoization table of size `128`, which is negligible. The recursion stack depth is equal to the number of digits (around `15`). So the space complexity is also.

## Final Thoughts

This problem is a beautiful example of how changing your perspective can make an impossible task easy. By looking at the numbers not as whole values but as strings of digits that interact column by column, we transformed a  iteration nightmare into a practically instant calculation.

The key takeaway here is the "stopped" flag. Whenever you deal with problems involving digit properties and variable lengths (like leading zeros), explicitly tracking whether a number is "active" or "finished" is a powerful technique.

I hope this explanation helps you understand the magic behind Digit DP! It is a tool that appears often in competitive programming and technical interviews, so mastering it is definitely worth the effort.

Happy coding and good luck with your algorithm adventures!
