# Arranging Numbers to Find the Minimum Cost Array Permutation A Friendly Guide [#3149]

Hello everyone! Welcome to another deep dive into algorithm problem solving. Today I want to share my thoughts on a fascinating array permutation puzzle. Finding the right sequence of elements to minimize a specific cost can seem daunting at first glance. However, by breaking the logic down step by step and looking at the hidden patterns, we can arrive at a very elegant and satisfying logic. I am sharing this in hopes that it helps anyone struggling with similar sequence optimization challenges!



## Understanding the Core Problem

Let us start by clearly defining what we are trying to achieve. You are given an array of integers named `nums`. This array contains all the numbers from `0` up to `n minus 1` exactly once. Your goal is to construct a completely new array called `perm`, which must also contain all the numbers from `0` up to `n minus 1` exactly once.

The tricky part is the scoring system. The score of your new permutation is calculated by looking at adjacent pairs of numbers in your sequence. Specifically, for each number in your permutation, you look at the *next* number in your permutation, use that next number as an index to look up a value in the original `nums` array, and calculate the absolute difference. Finally, because the sequence wraps around, you pair the very last number in your permutation with the very first number.

The formula looks like this:
Absolute difference of `perm[0]` and `nums[perm[1]]`
Plus
Absolute difference of `perm[1]` and `nums[perm[2]]`
Plus
... all the way to ...
Plus
Absolute difference of `perm[n minus 1]` and `nums[perm[0]]`

We want to find the specific arrangement for `perm` that makes this total sum as small as possible. If there are multiple arrangements that give the exact same minimum score, we must return the one that is lexicographically smallest. Lexicographically smallest simply means it would appear first in a dictionary of numbers. For example, `[0, 1, 2]` is smaller than `[0, 2, 1]`.

## The Hidden Circle and the First Big Insight

Before we rush into writing code, let us look at the structure of the score formula. Notice how the first element links to the second, the second links to the third, and the final element links right back to the first? 

This creates a closed loop or a circle. 



Because it is a perfect circle, the "starting point" does not actually matter for the total score. Think of a necklace made of different colored beads. Whether you start looking at the necklace from a red bead or a blue bead, the total sequence of colors going around the loop remains exactly the same. 

If we have a permutation like `[1, 2, 0]`, the score checks:
1 links to 2
2 links to 0
0 links back to 1

If we shift that exact same permutation to `[0, 1, 2]`, the score checks:
0 links to 1
1 links to 2
2 links back to 0

The pairs being evaluated are completely identical! Therefore, any valid minimum score arrangement can simply be rotated so that the number `0` is at the very front. Since we want the lexicographically smallest answer, we *always* want our array to start with `0`. This is a fantastic realization because it instantly anchors our starting point. We no longer need to guess the first number; it will always be `0`.

## Why Guessing Everything is Too Slow

You might wonder if we can just generate every possible permutation, calculate the score for each, and pick the best one. 

The length of the array `n` can be up to 14. The number of ways to arrange 14 distinct items is 14 factorial. 
14 factorial is 87,178,291,200. 

Calculating the score for 87 billion different combinations would take a standard computer a very long time, far exceeding any reasonable time limit. We need a way to remember our previous work so we do not calculate the same scenarios repeatedly.

## Enter Dynamic Programming

Instead of generating full arrays all at once, let us build our permutation one number at a time from left to right. 

Imagine you are midway through building your permutation. To decide the best number to add next, what information do you actually need to know? 
You only need to know two things:
1. Which numbers have you already used? (We cannot reuse numbers).
2. What was the very last number you added? (Because the score formula pairs the last number with the next number).

The exact order of the previously placed numbers does not matter for the *future* choices. Only the set of used numbers and the most recently placed number matter. This realization is the cornerstone of Dynamic Programming for this puzzle.



## Managing Sets with Bitmasking

How do we efficiently keep track of "which numbers have been used"? We could use an array of boolean values, but since `n` is very small (up to 14), we can use a clever technique called Bitmasking.

A computer stores integers as binary numbers (a series of 1s and 0s). We can use a single integer to represent our set of used numbers. 
Let the bit at position `i` be `1` if the number `i` has been used, and `0` if it has not.

For example, if `n` is 4, and we have used the numbers `0` and `2`, our binary representation would be `0101` (which is the decimal number 5). 

To check if a number `i` is already used, we use the bitwise AND operation. 
To mark a number `i` as used, we use the bitwise OR operation. 

This makes our program incredibly fast and memory efficient!

## Designing the Memory Tables

We will create two tables (matrices) to store our computations:
1. `dpTable` : This will store the minimum possible future cost given our current state (the bitmask of used numbers and the last placed number).
2. `nextChoiceTable` : This will store the actual best choice for the next number so we can reconstruct our answer at the very end.

The size of these tables will be `2^n` rows (representing all possible combinations of used numbers) and `n` columns (representing the last placed number).

## The Step by Step Logic

We will write a recursive function to explore our choices. 

**The Goal of the Function:**
Given the `mask` of used numbers and the `last` number placed, return the minimum cost to finish the rest of the permutation.

**The Base Case (When we are done):**
If the `mask` shows that all numbers have been used (the binary number is all 1s), we just need to close the circle. We calculate the cost of linking the `last` number back to our starting number, which is `0`. The cost added is the absolute difference between `last` and `nums[0]`.

**The Exploring Phase:**
If we are not done, we look at every possible number `i` from `0` up to `n minus 1`.
If number `i` is not in our `mask`, it is a valid candidate for the next position!
The cost of choosing `i` is the absolute difference between `last` and `nums[i]`, plus whatever the minimum cost is to finish the rest of the sequence (which we get by calling our function again with the updated `mask` and `i` as the new last number).

We keep track of the absolute lowest cost we can achieve. Also, by iterating `i` starting from `0` and going upwards, the very first candidate that yields the lowest cost will naturally be the lexicographically smallest! We record this candidate in our `nextChoiceTable`.

## The C++ Implementation

Here is a clean and fully documented C++ code implementation based on our logic. 
Please note the standard C++ syntax requires basic subtraction symbols for mathematical evaluation and array length calculations.

```cpp
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> findPermutation(vector<int>& nums) {
        int n = nums.size();
        int totalStates = 1 << n; // 2 to the power of n
        
        // Initialize our memoization table with negative one to represent uncalculated states
        vector<vector<int>> dpTable(totalStates, vector<int>(n, -1));
        
        // Table to remember our optimal choices for reconstruction
        vector<vector<int>> nextChoiceTable(totalStates, vector<int>(n, -1));
        
        // We define our recursive logic using a lambda function
        auto evaluateCost = [&](auto& self, int mask, int last) -> int {
            // Base condition: all n bits are set to 1
            if (mask == totalStates - 1) {
                // Return the cost of closing the cycle back to element 0
                return abs(last - nums[0]);
            }
            
            // If we have already solved this exact state, return the saved answer
            if (dpTable[mask][last] != -1) {
                return dpTable[mask][last];
            }
            
            int lowestCost = 1000000000; // Start with a very large dummy value
            int bestNextElement = -1;
            
            // Try all possible next numbers from 0 up to n minus 1
            for (int i = 0; i < n; ++i) {
                // Check if the i-th bit is NOT set in the mask (meaning it is unused)
                if ((mask & (1 << i)) == 0) {
                    
                    // Calculate the cost of picking i right now
                    int immediateCost = abs(last - nums[i]);
                    
                    // Recursively calculate the cost for the remainder of the array
                    int futureCost = self(self, mask | (1 << i), i);
                    int totalCost = immediateCost + futureCost;
                    
                    // If we find a strictly smaller total cost, we update our records.
                    // Using strictly less than guarantees we favor the earliest 
                    // loop index, maintaining lexicographical order.
                    if (totalCost < lowestCost) {
                        lowestCost = totalCost;
                        bestNextElement = i;
                    }
                }
            }
            
            // Save the best decision and the lowest cost before returning
            nextChoiceTable[mask][last] = bestNextElement;
            dpTable[mask][last] = lowestCost;
            return lowestCost;
        };
        
        // We know the optimal sequence can start at 0.
        // So we start our recursive search with the 0-th bit set to 1, and the last number as 0.
        evaluateCost(evaluateCost, 1, 0);
        
        // Now we rebuild the final answer array using our choice table
        vector<int> finalPermutation;
        int currentMask = 1;
        int currentLast = 0;
        
        finalPermutation.push_back(0); // Add our fixed starting element
        
        for (int step = 1; step < n; ++step) {
            // Look up the best next choice for our current state
            int nextElement = nextChoiceTable[currentMask][currentLast];
            finalPermutation.push_back(nextElement);
            
            // Update our state variables to move forward
            currentMask = currentMask | (1 << nextElement);
            currentLast = nextElement;
        }
        
        return finalPermutation;
    }
};

```

## Evaluating Efficiency (Time and Space Complexity)

It is always important to evaluate how much time and memory an algorithm consumes.

**Time Complexity:**
We have `2^n` possible masks and `n` possible last elements. This gives us `n * 2^n` unique states in our DP table.
Inside our recursive function, for each state, we use a loop that runs `n` times to find the next best number.
Therefore, the maximum number of operations is roughly `n * n * 2^n`.
With `n` at a maximum of 14, `14 * 14 * 16384` evaluates to just over 3.2 million operations. This is incredibly small for modern processors and will easily execute in just a few milliseconds! The time complexity is exactly O(N * N * 2^N).

**Space Complexity:**
We allocated two two dimensional arrays: `dpTable` and `nextChoiceTable`.
Each table has `2^n` rows and `n` columns.
Therefore, the amount of memory we need scales with `n * 2^n`. This easily fits within standard memory limits. The space complexity is O(N * 2^N).

## Closing Thoughts

By observing the cyclic property of the score, we elegantly eliminated a massive amount of guesswork by fixing the starting position. From there, treating the sequence building as a series of decisions allowed us to use Dynamic Programming. Finally, bitmasking provided a highly compact way to remember our unused numbers.

Algorithm puzzles like this are wonderful exercises in breaking complex problems into manageable sub problems. I hope this detailed breakdown makes the concepts clearer and gives you a solid foundation for tackling similar sequence optimization challenges in the future. Happy coding and keep exploring!
