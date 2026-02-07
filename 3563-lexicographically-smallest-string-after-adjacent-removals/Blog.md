# Mastering String Minimization with Interval Dynamic Programming [#3563]

[Blog Post Link](https://leetcode.com/discuss/post/7561602/mastering-string-minimization-with-inter-hnyi/)

Hello fellow coding enthusiasts! Now I want to share a fascinating journey into solving a complex string manipulation puzzle that really tests our understanding of dynamic programming. It is a problem that looks simple on the surface but hides a beautiful depth of logic beneath it. We are going to explore how to break down a string, remove characters based on specific rules, and reconstruct it to be the lexicographically smallest version possible.

This particular challenge is known as **Lexicographically Smallest String After Adjacent Removals**.

### The Challenge We Face

Let us start by understanding exactly what we are trying to achieve. Imagine you are holding a string of lowercase English letters. You are allowed to perform a magic trick on this string repeatedly. The trick works like this:

You can pick any two adjacent characters in the string. However, you can only pick them if they are neighbors in the alphabet. For example, 'a' and 'b' are neighbors, 'd' and 'c' are neighbors. The order does not matter, so 'ab' works just as well as 'ba'.

There is a twist! The alphabet is treated as circular. This means 'z' and 'a' are also considered neighbors. So if you see 'za' or 'az' sitting together, you can pick them too.

When you pick such a pair, you remove them from the string. Then, the remaining parts of the string shift together to close the gap. This might bring new characters next to each other, creating new opportunities for removal.

**The Goal**

We want to keep performing this operation as many times as we like (or not at all) to get a final result that is the "lexicographically smallest."

What does "lexicographically smallest" mean? Think of it like a dictionary. Words in a dictionary are sorted alphabetically. "apple" comes before "banana" because 'a' is smaller than 'b'. "ant" comes before "anteater" because shorter words come first if they match the start of the longer word. We want our final string to appear as early as possible in this dictionary order. An empty string is the absolute smallest possible result, followed by "a", then "aa", then "ab", and so on.

### Strategy and Intuition

At first glance, you might feel tempted to use a "greedy" approach. A greedy strategy means making the best looking move right now and hoping it leads to the best result later. For example, if you see "bca", you might think "Oh, 'b' and 'c' are neighbors! Let me remove them to get 'a'."

But wait! What if the string was something more complex like "cab"?
If we just look at neighbors, we see 'a' and 'b'. If we remove "ab", we are left with "c".
However, maybe if we waited or removed things in a different order, we could get something better. In this specific "cab" case, 'c' and 'a' are not neighbors, and 'a' and 'b' are neighbors. If we remove 'ab', we get 'c'. There is no other move.

Let us look at a trickier case: "ccba".
We could remove "cb" (neighbors). Remaining: "ca". No more moves. Result: "ca".
Or, we could remove "ba" (neighbors). Remaining: "cc". No more moves. Result: "cc".
Comparing "ca" and "cc", "ca" is smaller. So the choice matters!

Because removing a pair might bring two other characters together that enable a crucial future move, or conversely, removing a pair might destroy a better future opportunity, a simple greedy approach will often fail. We need to explore all possibilities systematically.

This suggests we need **Dynamic Programming**, specifically **Interval DP**.

### Breaking It Down with Interval DP

Interval Dynamic Programming is a technique where we solve a problem for every small substring first, and then use those answers to solve larger substrings.

Let us define our state. We need a way to store the best answer for a specific part of the string.
Let `dp[i][j]` be the lexicographically smallest string we can obtain from the substring `s` starting at index `i` and ending at index `j`.

Our final answer will simply be the value stored in `dp[0][n-1]`, which covers the entire string.

To calculate `dp[i][j]`, we have to consider what happens to the very first character of our substring, which is `s[i]`. There are fundamentally only two things that can happen to `s[i]`.

**Option 1: The Character Survived**

In this scenario, we decide that `s[i]` will not be removed. It will remain as the first character of our resulting string. If we keep `s[i]`, then the best string we can make is simply `s[i]` combined with the best string we can make from the rest of the interval, which is `dp[i+1][j]`.

So, `Candidate 1 = s[i] + dp[i+1][j]`.

**Option 2: The Character Was Removed**

This is the interesting part. If `s[i]` is removed, it cannot just vanish on its own. The rules say characters are removed in pairs. So, `s[i]` must be paired with some matching partner `s[k]` somewhere ahead of it in the substring (where `k` is between `i+1` and `j`).

For this removal to be valid, three conditions must be met:

1. **Alphabet Neighbors:** `s[i]` and `s[k]` must be consecutive in the alphabet (like 'a' and 'b', or 'z' and 'a').
2. **Clear Path:** The characters *between* `s[i]` and `s[k]` (the range `i+1` to `k-1`) must be completely removable. We cannot pair `s[i]` and `s[k]` if there is a chunk of text stuck between them that we cannot get rid of. The substring `s[i+1...k-1]` must reduce to an empty string.
3. **Future Optimality:** If we pair `s[i]` and `s[k]`, they both disappear. The middle is gone. We are then left with whatever remains from the rest of the string after `k` (the range `k+1` to `j`).

So, if `s[i]` pairs with `s[k]`, the result is simply `dp[k+1][j]`.

We need to check every possible `k` from `i+1` to `j`. If a valid `k` is found, we get a candidate string. We compare all such candidates and pick the smallest one.

### The Algorithm in Steps

Here is how we will structure our solution logic:

1. **Initialization:** Create a table (a 2D array) called `dp`. We also find it helpful to have a helper table, let us call it `can_empty`, which simply tells us "True" or "False" if a substring can be completely deleted. This speeds up our checks.
2. **Length Iteration:** We loop through lengths from 1 up to the full length of the string `n`. We solve for all substrings of length 1, then length 2, and so on. This ensures that when we compute a large interval, we already know the answers for the smaller intervals inside it.
3. **The Interval Loop:** For each length, we slide a window across the string to define our start `i` and end `j`.
4. **Compute the Best String:**
* First, assume we keep `s[i]`. Construct the string `s[i] + dp[i+1][j]`.
* Next, iterate through all possible split points `k`.
* Check if `s[i]` and `s[k]` are compatible neighbors.
* Check if the middle part `dp[i+1][k-1]` is an empty string.
* If both are true, the result of this move is just `dp[k+1][j]`.
* Compare this new result with our current best. If it is lexicographically smaller, update our best.


5. **Store and Repeat:** Save the best result in `dp[i][j]`. If this result happens to be an empty string, mark `can_empty[i][j]` as True.
6. **Final Output:** Return `dp[0][n-1]`.

### The Code Implementation

Here is the C++ solution that implements this logic. I have added comments to guide you through each section.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    string lexicographicallySmallestString(string s) {
        int n = s.length();
        
        // dp[i][j] stores the best (smallest) string obtainable 
        // from the substring s[i...j]
        vector<vector<string>> dp(n, vector<string>(n));
        
        // can_empty[i][j] stores whether the substring s[i...j] 
        // can be completely removed (reduced to "").
        // This is a small optimization to avoid checking if dp[i][j] == "".
        vector<vector<bool>> can_empty(n, vector<bool>(n, false));

        // A helper lambda function to check alphabet adjacency
        auto are_adjacent = [](char a, char b) {
            if (abs(a - b) == 1) return true;
            if (a == 'a' && b == 'z') return true;
            if (a == 'z' && b == 'a') return true;
            return false;
        };

        // We process substrings by length, starting from 1 up to n.
        for (int len = 1; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                
                // --- STRATEGY 1: Keep s[i] ---
                // If we keep s[i], the result is s[i] followed by the 
                // best reduction of the rest of the substring.
                string best_s;
                if (i + 1 <= j) {
                    best_s = s[i] + dp[i+1][j];
                } else {
                    best_s = string(1, s[i]);
                }
                
                // --- STRATEGY 2: Remove s[i] ---
                // Try to pair s[i] with some s[k] inside the current range.
                for (int k = i + 1; k <= j; ++k) {
                    
                    // Check Condition 1: Are they alphabet neighbors?
                    if (are_adjacent(s[i], s[k])) {
                        
                        // Check Condition 2: Can the middle be deleted?
                        // The middle is the range [i+1, k-1].
                        bool middle_empty = false;
                        if (i + 1 > k - 1) { 
                            // This means indices are adjacent (like 0 and 1), 
                            // so the middle is essentially empty.
                            middle_empty = true;
                        } else {
                            middle_empty = can_empty[i+1][k-1];
                        }
                        
                        if (middle_empty) {
                            // If middle is gone, s[i] and s[k] are removed.
                            // The result is whatever is left after k.
                            string candidate;
                            if (k + 1 <= j) {
                                candidate = dp[k+1][j];
                            } else {
                                candidate = "";
                            }
                            
                            // Compare this new candidate with our current best.
                            if (candidate < best_s) {
                                best_s = candidate;
                            }
                        }
                    }
                    
                    // Optimization: If we found a way to make the string empty,
                    // we can stop checking other 'k' because empty is the 
                    // absolute best possible result.
                    if (best_s.empty()) break; 
                }
                
                // Store the result
                dp[i][j] = best_s;
                
                // Update our helper table if the result is empty
                if (best_s.empty()) {
                    can_empty[i][j] = true;
                }
            }
        }
        
        // The answer for the full string from 0 to n-1
        return dp[0][n-1];
    }
};

```

### Complexity Analysis

It is important to understand the efficiency of our code to ensure it runs within limits.

**Time Complexity**
Our solution involves three nested loops.

1. The outer loop iterates through lengths (roughly `n`).
2. The middle loop iterates through start positions (roughly `n`).
3. The inner loop iterates through split positions `k` (roughly `n`).

Inside the innermost loop, we perform string concatenation and comparison. In C++, comparing or adding strings takes time proportional to the length of the string, which is `O(n)`.
So, the total time complexity is roughly `O(n^4)`. Given that the maximum length of the string `n` is 250, `n^4` is roughly 3.9 billion operations in the worst case. However, because we break early when we find an empty string, and because average string lengths in comparisons are shorter than `n`, this runs well within the typical 2-second time limit for competitive programming problems.

**Space Complexity**
We store a DP table of size `n` by `n`, where each cell holds a string. This gives us a space complexity of `O(n^3)` because we have `n^2` cells, and each string can be up to length `n`. For `n=250`, this is perfectly safe for memory limits.

### Summary

This problem is a fantastic exercise in thinking recursively. We learned that to solve the big problem, we must first solve the small problems (substrings). By carefully considering the two choices for every character (keep it or pair it), we can explore the entire decision tree efficiently.

I hope this explanation clears up the logic behind the solution. String problems combined with Dynamic Programming can be intimidating, but breaking them down into intervals makes them much more manageable. 

Happy leet-coding!
