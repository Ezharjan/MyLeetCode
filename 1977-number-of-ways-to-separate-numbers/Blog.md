# Mastering the String Partition Puzzle A Guide to Separating Numbers Strictly Increasing [#1977]

Hello everyone in the coding community! Today I want to share my thoughts on a fascinating string parsing puzzle. It is a problem that tests our ability to organize data, look for patterns, and build a solution step by step using dynamic programming. I recently spent some time studying this challenge, and I would love to walk you through a working approach. 

If you have ever stared at a massive string of numbers and wondered how many valid ways you can chop it up into smaller pieces, this is the exact scenario we will be tackling. Let us dive right in!

***

## Understanding the Rules of the Puzzle

Imagine you wrote down a sequence of positive integers in a continuous string called `num`. Unfortunately, you forgot to add spaces or commas to separate the individual numbers. 

You do remember two critical rules about the original sequence of numbers:
1.  **Monotonically Increasing** Each number in the sequence was greater than or equal to the number immediately preceding it.
2.  **No Leading Zeros** Not a single number in the sequence started with the digit '0'. 

Your goal is to figure out the total number of valid original sequences that could have produced the string `num`. Because this total count can grow to be extremely large, we need to return the final answer modulo 1000000007.

### Looking at Concrete Examples

To truly grasp what the problem asks of us, evaluating a few test cases is a great idea.

**Case 1 String "327"**
We have a few options for how we can split this string.
* Option A: We can keep it as one single number: "327". This is completely valid because a single number sequence technically does not violate the increasing rule, and there are no leading zeros.
* Option B: We can split it into "3" and "27". Is "3" less than or equal to "27"? Yes. Are there any leading zeros? No. So this is also a valid sequence!
* Option C: We can split it into "3", "2", and "7". Is "3" less than or equal to "2"? No. So we discard this option.
The total number of valid sequences here is 2.

**Case 2 String "094"**
Whenever a string starts with '0', we immediately hit a roadblock. The very first number we try to form would begin with a zero, which violates our strict no leading zero rule. Therefore, there are exactly 0 valid sequences for this string.



***

## The Core Concept and Initial Thoughts

When faced with a problem that asks for "the number of ways" to do something, recursion and dynamic programming should immediately come to mind. 

The naive way to solve this would be to try every single possible split using recursion. We could start at the beginning of the string, pick a length for the first number, and then recursively call a function to process the rest of the string while passing along the value of the number we just formed. 

However, looking at the constraints, the length of `num` can be up to 3500 characters long. If we try every combination, the number of possibilities grows exponentially. A recursive tree would expand into millions of branches, leading to a Time Limit Exceeded error. We need a way to store our intermediate results so we never calculate the same state twice.

### Defining Our Dynamic Programming State

To build a methodical solution, we need to define a state that captures all the necessary information to move forward. Let us create a two dimensional array called `ways`.

Let `ways[i][length]` represent the number of valid sequences starting at index `i` of the string `num`, where the very first number in our new sequence has exactly `length` digits.

If we want to find `ways[i][length]`, we need to look ahead to the next number. The next number will start at index `i + length`. Let us say the next number has a length called `next_length`. 

We have three distinct scenarios to consider for the next number:
1.  If `next_length` is greater than `length`, the next number will naturally have a larger numerical value (since there are no leading zeros, a longer string of digits always represents a larger number). This is a valid transition!
2.  If `next_length` is exactly equal to `length`, we must carefully compare the two strings of digits. We need to check if the first string is less than or equal to the second string. If it is, the transition is valid.
3.  If `next_length` is less than `length`, the next number will definitely be smaller. This violates our increasing rule, so we completely ignore this scenario.

***

## Overcoming Bottlenecks

While the state definition above is correct, implementing it directly with two nested loops and a string comparison would still be too slow. Comparing two strings of length `L` takes `O(L)` time. If we do this inside our nested loops, our time complexity pushes toward `O(N^3)`, which is too slow for 3500 characters.

We have two major bottlenecks to eliminate.

### Bottleneck 1 The String Comparison

We need a way to compare two substrings of the same length in constant `O(1)` time. We can achieve this using a widely known technique called the Longest Common Prefix matrix. 

Let `lcp[i][j]` represent the number of matching characters starting at index `i` and index `j`. We can build this matrix from bottom to top. If the characters at `num[i]` and `num[j]` are identical, then `lcp[i][j] = 1 + lcp[i + 1][j + 1]`. Otherwise, it is simply 0.



How does this help us? When we want to compare a substring starting at `i` and a substring starting at `j` (both of length `L`), we first check their longest common prefix. Let `common` be `lcp[i][j]`. 
* If `common` is greater than or equal to `L`, the two substrings are identical! They are perfectly equal.
* If `common` is less than `L`, the substrings differ exactly at the index `common`. We just need to compare the single character at `num[i + common]` with the single character at `num[j + common]` to know which substring is larger! 

This brilliant trick reduces our string comparison to a simple constant time check.

### Bottleneck 2 The Inner Loop Summation

To calculate `ways[i][length]`, we established that we need to add up `ways[i + length][next_length]` for all valid `next_length` values. Looping through all possible lengths every single time will drag down our performance.

Instead of a loop, we can maintain a running sum. Let us introduce another two dimensional array called `dp`. We will define `dp[i][length]` as the sum of `ways[i][k]` for all `k` greater than or equal to `length`. 

By maintaining this running total, whenever we need the sum of all ways where the next number is strictly longer than our current number, we can simply look up `dp[i + length][length + 1]` in one single step! 



***

## Putting the Pieces Together

Now we have a clear blueprint for our algorithm.

1.  **Preparation** Create the `lcp` matrix to speed up comparisons. Create the `ways` matrix and the `dp` running sum matrix.
2.  **Direction** We will process the string backwards, starting from the last index and moving towards the first index. This is necessary because finding the number of ways at index `i` requires us to already know the answers for indices coming after `i`.
3.  **Iteration** For each starting index `i`, we will test all possible lengths for the first number. If the character at `num[i]` is a zero, we immediately skip it because leading zeros are forbidden.
4.  **Transitions** For a valid length, if the number stretches exactly to the end of the string, there is exactly 1 way to form the sequence. If it does not stretch to the end, we check if a sequence of the exact same length is valid using our `lcp` helper. We then add our running total from the `dp` array for all longer lengths.
5.  **Modulo Arithmetic** At every addition step, we must keep our numbers within the 1000000007 limit to prevent data overflow. 

### A Quick Note on the Code Implementation

To strictly adhere to formatting rules where we want to avoid specific punctuation marks in our logic flow, you will notice an interesting helper function in the code below called `sub`. Instead of using standard subtraction syntax which relies on the minus symbol, I am utilizing a neat bitwise arithmetic trick `a + (~b) + 1`. In the world of binary numbers, applying a bitwise NOT and adding one is equivalent to finding the negative value of a number. This allows us to perform all necessary index decrements and modulo reductions using only addition operations! It is a fun little exercise in computer architecture applied directly to competitive programming.

***

## The C++ Implementation

Here is a working implementation mapping out everything we just discussed.

```cpp
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    int numberOfCombinations(string num) {
        // Handle empty strings or strings starting with an invalid zero
        if (num.empty() || num[0] == '0') return 0;
        
        int N = num.length();
        int MOD = 1000000007;
        
        // A helper lambda to perform subtraction without using the minus operator
        // This works via two's complement binary logic
        auto sub = [](int a, int b) {
            return a + (~b) + 1;
        };
        
        // Build the Longest Common Prefix matrix to optimize string comparisons
        vector<vector<short>> lcp(N + 1, vector<short>(N + 1, 0));
        for (int i = sub(N, 1); i >= 0; i = sub(i, 1)) {
            for (int j = sub(N, 1); j >= 0; j = sub(j, 1)) {
                if (num[i] == num[j]) {
                    lcp[i][j] = 1 + lcp[i + 1][j + 1];
                }
            }
        }
        
        // Helper lambda to check if num starting at i is <= num starting at j
        // both having the exact same length l
        auto isValid = [&](int i, int j, int l) {
            int common = lcp[i][j];
            if (common >= l) return true; 
            return num[i + common] < num[j + common];
        };
        
        vector<vector<int>> ways(N + 1, vector<int>(N + 2, 0));
        vector<vector<int>> dp(N + 1, vector<int>(N + 2, 0));
        
        // Iterate backwards from the end of the string down to the beginning
        for (int i = sub(N, 1); i >= 0; i = sub(i, 1)) {
            
            // Skip paths that attempt to start a number with a leading zero
            if (num[i] == '0') continue;
            
            // Test all possible lengths for the current number
            for (int l = sub(N, i); l >= 1; l = sub(l, 1)) {
                
                // If the number spans exactly to the end of our string
                if (i + l == N) {
                    ways[i][l] = 1;
                } else {
                    int w = 0;
                    
                    // Check if we can chain to a next number of the exact same length
                    if (i + 2 * l <= N && isValid(i, i + l, l)) {
                        w = ways[i + l][l];
                    }
                    
                    // Add all combinations where the next number is strictly longer
                    w += dp[i + l][l + 1];
                    
                    // Keep the total within the required bounds
                    if (w >= MOD) w = sub(w, MOD);
                    
                    ways[i][l] = w;
                }
                
                // Update our running total array for the next iterations to use
                int current_dp = dp[i][l + 1] + ways[i][l];
                if (current_dp >= MOD) current_dp = sub(current_dp, MOD);
                dp[i][l] = current_dp;
            }
        }
        
        // Our final answer is the sum of all valid sequences starting at index 0
        // with any length of 1 or greater
        return dp[0][1];
    }
};

```

---

## Evaluating Complexity

A great way to finish up any algorithmic study is to check how much memory and processing power our code requires.

**Time Complexity** Our preparation step builds the longest common prefix matrix. This requires two nested loops, each running `N` times. Inside the loops, we do a basic addition operation. This takes `O(N^2)` time.
Our main logic also uses two nested loops traversing the string and the possible lengths. Inside this loop, due to our running sum trick and our prefix matching trick, all operations are evaluated in constant time. Thus, the main logic also takes `O(N^2)` time.
Overall, the time complexity stands securely at `O(N^2)`, which is perfectly suited to handle a string length of 3500 characters within standard execution limits.

**Space Complexity** We allocate three matrices to store our states. The `lcp` matrix uses short integers to save memory, taking up `O(N^2)` space. The `ways` and `dp` matrices use standard integers, also taking `O(N^2)` space. The overall space complexity is `O(N^2)`.

I hope this thorough breakdown helps clarify the inner workings of string parsing dynamically. By carefully tracking states and avoiding repetitive work, we can transform an overwhelmingly large problem into a highly manageable one.

Would you like me to walk through another string parsing problem to see how these dynamic programming concepts apply to different sets of rules?
