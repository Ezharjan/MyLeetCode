# Crafting the Perfect String Transformation by Solving Minimum Cost Good Caption [#3441]

Hello there fellow coding enthusiasts! Today I want to share a fascinating journey I took while solving a challenging string manipulation problem. It was a rollercoaster of logic, optimization, and a very tricky edge case that taught me a valuable lesson about lexicographical comparisons.

If you enjoy dynamic programming or just love seeing how a complex problem breaks down into manageable pieces, pull up a chair. We are going to explore how to transform a messy string into a "good" one with the minimum cost.

## Understanding the Problem

Let us start by defining what we are actually trying to solve. We are given a string called `caption` consisting of lowercase English letters. The goal is to transform this string into a "good caption."

So what exactly makes a caption "good"?
A caption is considered good if every character in it appears in groups of at least 3 consecutive occurrences.

For example, `aaabbb` is good because `a` appears 3 times and `b` appears 3 times. `aaaaccc` is also good. However, `aabbb` is not good because the group of `a`s only has length 2. `abc` is definitely not good.

We can change any character in the string to another character, but there is a cost. The cost is calculated based on the alphabetical distance. Changing `a` to `b` costs 1. Changing `a` to `c` costs 2. Changing `a` to `z` costs 25. Note that the alphabet is not circular here; you cannot jump from `z` to `a` directly.

Our specific task is to find a good caption that requires the minimum total cost to create. If there are multiple good captions that yield the same minimum cost, we must return the one that is lexicographically smallest.

This second requirement about the lexicographical order is where the real challenge lies, and it is exactly where I stumbled during my first few attempts.

## Initial Thoughts and Strategy

When I first looked at this, my mind went straight to the constraints. The string length can go up to 50,000 characters. This immediately tells us that an exponential solution is out of the question. We need something linear or close to linear, like O(N).

A greedy approach seems tempting at first. You might think, "I will just look at the next 3 characters, see what is cheapest to turn them into, and move on." But greedy logic fails here because a choice you make now restricts what you can do later. If you cheaply turn the first 3 characters into `aaa`, you might force the 4th character to match them (to extend the group) even if that 4th character is very expensive to change to `a`.

This dependency on previous (or future) choices suggests Dynamic Programming.

### The Power of Block Decomposition

The key insight for this problem is understanding how a "good caption" is constructed. The definition says every group must be at least length 3. This means any valid string can be viewed as a concatenation of blocks, where each block has a length of 3, 4, or 5.

Wait, why just 3, 4, and 5?
Any integer greater than or equal to 3 can be represented as a sum of 3s, 4s, and 5s.
For instance:
Length 6 is 3 plus 3.
Length 7 is 3 plus 4.
Length 8 is 3 plus 5 (or 4 plus 4).

This simplifies our DP transitions immensely. Instead of worrying about a group of arbitrary length 100, we can just say: At any index `i`, we can form a block of length 3, 4, or 5 using some character `c` (from `a` to `z`), and then solve the subproblem for the remaining suffix.

## The First Attempt and Why I Failed

I want to be transparent about my process because failure is often the best teacher. My initial solution passed many test cases but failed on a specific, tricky input.

**Input:** `dlqlawbgd`
**My Output:** `llllllddd`
**Expected Output:** `llllddddd`

Let us analyze this. Both outputs are valid "good captions." Both likely have the same transformation cost (the problem guarantees we want the minimum cost, so let us assume my code got the cost right but the string wrong).

My code produced `llllllddd`.
The expected answer was `llllddddd`.

Alphabetically, `llllddddd` is smaller than `llllllddd` because at the fourth index (0 indexed), the expected string has `l` and my string has `l`. Wait, they are the same there. Let us look at the fifth index.
My string: `l`
Expected string: `d`

Since `d` comes before `l` in the alphabet, the expected string is lexicographically smaller.

**The Flaw in My Logic**

My initial DP state just stored the minimum cost. When reconstructing the path, if I found two options with the same cost, I used a simple tie breaker.

I thought: "If I have two options, one starting with `a` and one starting with `b`, I will pick `a` because it is smaller."

But what if both options start with the *same* character?
Option A: Pick a block of 3 `l`s, then recursively solve the rest.
Option B: Pick a block of 4 `l`s, then recursively solve the rest.

If both options yield the same total cost, how do you choose?
My code simply looked at the next character. It thought, "Okay, both start with `l`. Let us look at what comes *after* the block."
If Option A leads to a suffix starting with `d` and Option B leads to a suffix starting with `e`, I would pick Option A.

This seems correct, but it fails when the "next" character is *also* `l`.
In the failing case, we are comparing a run of `l`s.
Option A might give us `lllll...`
Option B might give us `llll...`

If the subsequent characters continue to be `l`, comparing the "next" character does not help because they are identical. You need to look deeply ahead to find the first character that *differs*.

Basically, I was not tracking the "run length" of the current character. I did not know if extending the current block of `l`s by one more character would eventually lead to a smaller character sooner, or if it would delay a transition to a smaller character.

This specific type of lookahead is tricky in standard DP because usually, DP states are independent. Here, the "lexicographically smallest" requirement creates a dependency on *how long* the current character repeats.

## The Correct Approach: Tracking Run Lengths

To fix this, we need to enrich our DP. We cannot just store the cost. We need to store enough information to perform a proper lexicographical comparison between two valid paths that have the same cost.

We will iterate backwards from the end of the string to the beginning.
Let `dp[i]` be the minimum cost to transform the suffix starting at `i`.

For each index `i`, we try every character `c` (from `a` to `z`) and every length `L` (3, 4, 5).
Cost = (Cost to turn `caption[i...i+L-1]` into `c`) + `dp[i+L]`.

If we find a strictly smaller cost, we update our best choice.
If we find an equal cost, we must compare the resulting strings.

### The Tie Breaking Rules

Suppose we have two choices at index `i` that both cost the same amount.
Choice 1 starts with character `char1`.
Choice 2 starts with character `char2`.

**Case 1: `char1` is different from `char2`**
This is easy. We pick the choice with the smaller character. If `char1` is `a` and `char2` is `b`, pick Choice 1.

**Case 2: `char1` is the same as `char2` (Let us call it `c`)**
This is the hard part. Both strings start with the same character `c`. We need to know which string switches to a *different* character first, and what that different character is.

To handle this efficiently, we can store a value `run_len[i]` alongside our DP table. `run_len[i]` tells us: "For the optimal solution starting at index `i`, how many consecutive `c`s are there at the beginning?"

Let us compare Choice 1 and Choice 2 again, assuming both start with `c`.
Let `run1` be the total length of the continuous block of `c`s for Choice 1.
Let `run2` be the total length of the continuous block of `c`s for Choice 2.

If `run1` is not equal to `run2`, it means one string stays as `c` longer than the other.
Let us say `run1` is shorter than `run2`.
This means Choice 1 switches to a new character (let us call it `next_char1`) after `run1` characters.
Choice 2 is still `c` at that position.

So, effectively, we are comparing `next_char1` vs `c`.
If `next_char1` < `c`, then Choice 1 is better (it switches to a smaller letter sooner).
If `next_char1` > `c`, then Choice 2 is better (it stays `c` longer, avoiding the larger letter).

This logic handles the `llllllddd` vs `llllddddd` case perfectly.

## Detailed Solution Walkthrough

Now let us construct the solution step by step.

### Step 1: Precomputing Costs
Calculating the cost to change a substring to a specific character over and over again is slow. We can use a 2D prefix sum array (or just a regular 2D array precomputed) to do this in constant time.

Let `P[char][i]` be the cost to change the *prefix* of the caption (from index 0 to `i-1`) into the character `char`.
The cost to change a substring from `i` to `j` into `char` is simply:
`RangeCost = P[char][j+1] - P[char][i]`

### Step 2: The DP Arrays
We need a few arrays to store our state. Since we are going backwards (from `n` down to `0`):
1.  `dp[i]`: Minimum cost for suffix `i`.
2.  `op_len[i]`: The length of the block chosen at index `i` (3, 4, or 5).
3.  `min_char[i]`: The character chosen for the block at index `i`.
4.  `run_len[i]`: The length of the continuous run of `min_char[i]` starting at `i`.

### Step 3: The Main Loop
We iterate `i` from `n-1` down to `0`.
Inside, we iterate through every possible character `c` from `a` to `z`.
Inside that, we iterate through lengths `len` = 3, 4, 5.

We calculate the total cost for this combination.
`CurrentCost = RangeCost(i, i+len, c) + dp[i+len]`

**Handling the Run Length:**
When we consider a block of `c` with length `len`, the new run length will be at least `len`.
If the optimal solution at `i+len` *also* starts with `c`, we add its run length to ours.
`current_run = len + (min_char[i+len] == c ? run_len[i+len] : 0)`

### Step 4: Applying the Logic
For each option:
1.  If `CurrentCost < dp[i]`: Update `dp[i]` and all other records (`op_len`, `min_char`, `run_len`).
2.  If `CurrentCost == dp[i]`: Use the tie breaker logic we discussed.
    * Compare `c` with the current best `min_char[i]`.
    * If `c` is smaller, update.
    * If `c` is equal, compare `current_run` vs `run_len[i]`. determine which one reveals a smaller character first.

### Step 5: Reconstruction
Once we fill the table down to `i=0`, we can build the result string.
Start at `idx = 0`.
While `idx < n`:
   * Get the chosen length `L = op_len[idx]` and character `C = min_char[idx]`.
   * Append `C` repeated `L` times to our result string.
   * Move `idx` forward by `L`.

## The C++ Implementation

Here is the C++ code that implements this logic. I have kept it clean and added comments to help you follow along.

```cpp
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

class Solution {
public:
    string minCostGoodCaption(string caption) {
        int n = caption.length();
        // If the string is too short to form even a single group of 3, return empty.
        if (n < 3) return "";

        // Step 1: Precompute prefix costs
        // P[c][i] stores the cost to change caption[0...i-1] into character ('a' + c).
        // This allows O(1) calculation for any substring cost.
        vector<vector<int>> P(26, vector<int>(n + 1, 0));
        for (int c = 0; c < 26; ++c) {
            char target = 'a' + c;
            for (int i = 0; i < n; ++i) {
                P[c][i + 1] = P[c][i] + abs(caption[i] - target);
            }
        }

        // Step 2: Initialize DP arrays
        // dp[i] stores the minimum cost to fix the suffix starting at index i
        // We use a large number (2e9) to represent infinity.
        vector<int> dp(n + 1, 2000000000);
        dp[n] = 0;
        
        // Arrays to reconstruct the solution and handle tie breaking
        vector<int> op_len(n + 1, 0);      // The length of the block chosen at i
        vector<int> min_char(n + 1, 26);   // The char chosen at i (26 is a sentinel > 'z')
        vector<int> run_len(n + 1, 0);     // The continuous run length of min_char starting at i

        // Step 3: Iterate backwards
        for (int i = n - 1; i >= 0; --i) {
            // Iterate through all possible characters 'a' through 'z'
            for (int c = 0; c < 26; ++c) {
                // Try block lengths of 3, 4, and 5
                for (int len = 3; len <= 5; ++len) {
                    if (i + len > n) break;
                    
                    // If the suffix starting at i+len is invalid (infinite cost), skip
                    if (dp[i + len] == 2000000000) continue;

                    // Calculate cost for this specific block + cost of the rest
                    int range_cost = P[c][i + len] - P[c][i];
                    int total_cost = range_cost + dp[i + len];
                    
                    // Calculate the continuous run length for this choice
                    int next_idx = i + len;
                    int current_run = len;
                    // If the next block uses the same character, we extend the run
                    if (next_idx < n && min_char[next_idx] == c) {
                        current_run += run_len[next_idx];
                    }

                    bool update = false;
                    
                    // Logic to decide if we should update our current best at dp[i]
                    if (total_cost < dp[i]) {
                        update = true; // Found a cheaper cost
                    } else if (total_cost == dp[i]) {
                        // Same cost, need to check lexicographical order
                        
                        if (c < min_char[i]) {
                            update = true; // Found a smaller starting character
                        } else if (c == min_char[i]) {
                            // Same starting character. This is the tricky case!
                            // We compare the run lengths.
                            int best_run = run_len[i];
                            
                            if (current_run != best_run) {
                                // Find the first character that differs between the two options
                                int shorter = min(current_run, best_run);
                                int break_point = i + shorter;
                                
                                // What char breaks the run? If we hit end of string, use sentinel 26
                                int X = (break_point < n) ? min_char[break_point] : 26;
                                
                                if (current_run < best_run) {
                                    // The current candidate run is shorter.
                                    // It means it switches to 'X' at break_point, while best stays 'c'.
                                    // If X < c, we want to switch! So update.
                                    if (X < c) update = true;
                                } else {
                                    // The current candidate run is longer.
                                    // It stays 'c', while best switches to 'X'.
                                    // We only want to stay 'c' if c < X.
                                    if (c < X) update = true;
                                }
                            }
                        }
                    }

                    // Apply the update if valid
                    if (update) {
                        dp[i] = total_cost;
                        op_len[i] = len;
                        min_char[i] = c;
                        run_len[i] = current_run;
                    }
                }
            }
        }

        // Step 4: Reconstruct the string
        if (dp[0] == 2000000000) return ""; // Impossible case

        string result = "";
        int idx = 0;
        while (idx < n) {
            int len = op_len[idx];
            int c = min_char[idx];
            result += string(len, (char)('a' + c));
            idx += len;
        }

        return result;
    }
};

```

## Complexity Analysis

Let us break down the efficiency to ensure this solution is robust enough for the limits.

**Time Complexity**
We iterate through the string of length `N` once.
For each index `i`, we iterate through 26 characters.
For each character, we iterate through 3 lengths (3, 4, 5).
All operations inside the loops (cost calculation, comparisons) are constant time `O(1)`.
So the total operations are roughly `N * 26 * 3`.
With `N = 50,000`, this is approximately 3.9 million operations, which is well within the 1 second time limit usually given for such problems.

**Space Complexity**
We use a few arrays of size `N`: `dp`, `op_len`, `min_char`, `run_len`.
We also use a 2D array `P` of size `26 * N` for prefix sums.
This results in linear space complexity relative to `N`. Given the memory limits, this is perfectly acceptable.

## Final Thoughts

This problem is a beautiful example of how detail oriented one must be with Dynamic Programming. The core DP is standard, but the lexicographical requirement adds a layer of depth that punishes superficial tie breaking.

The most important takeaway for me was the concept of "Run Length" in string comparisons. When you are comparing two strings that start with the same repeating character, you cannot just look at the next block. You have to look at where the repetition *stops*.

I hope this breakdown helps you tackle similar problems in the future. Happy coding!
