# Strategy for Maximum Value by Placing Three Rooks [#3256]

[Blog Post Link](https://leetcode.com/discuss/post/7560789/strategy-for-maximum-value-by-placing-th-uuyd/)

Welcome back to another deep dive into algorithmic problem solving! Today we are going to explore a fascinating challenge involving chessboards, strategic placement, and value maximization. If you enjoy grid problems or combinatorial puzzles, this one is going to be a delight.

We will be looking at a problem where we are given a grid of numbers representing a chessboard. Our goal is simple yet tricky: place three rooks on this board to maximize the sum of the values of the squares they occupy. However, there is a catch. In chess, rooks attack everything in their row and column. To succeed, we must place our three rooks so that no two rooks attack each other.

This means every rook must be in a unique row and a unique column.

Let us walk through this solution together. We will start by understanding the rules, move on to the intuition behind the efficient approach, and finally construct the code together.

## Understanding the Rules of the Game

Imagine you have a grid of size M by N. Every cell in this grid contains a number. This number could be positive, negative, or zero.

You have three chess rooks. You need to place them on three different cells , , and .

The placement is valid only if:

1. **Row Uniqueness**
The row indices , , and  are all different. No two rooks can share a horizontal line.
2. **Column Uniqueness**
The column indices , , and  are all different. No two rooks can share a vertical line.

Your objective is to find the configuration that gives you the largest possible total sum of the values found at those three coordinates.

### Why is this difficult?

If the board is small, say 3 by 3, you can easily check every possibility. But as the board grows to the constraint limit of 100 by 100, the number of combinations explodes.

A naive approach might try to pick every possible first square, then every possible second square, and then every possible third square.

* The first rook has about 10,000 choices.
* The second rook has about 10,000 choices.
* The third rook has about 10,000 choices.

This results in roughly one trillion operations, which is far too slow for standard computing limits. We need a smarter way to narrow down our choices without losing the correct answer.

## The Core Intuition: The Power of Three

The constraints tell us that M and N are up to 100. This is a small enough number that allows us to use an algorithm with cubic complexity, something that runs in roughly  or  time.

The key realization comes from looking at the rows. We know we must pick three distinct rows. Since the number of rows is small (up to 100), we can actually afford to iterate through every possible triplet of rows.

However, once we pick three rows, which columns do we choose?

For any specific row, we naturally want to pick the column with the largest value. But if we simply pick the largest value in Row A, and the largest value in Row B, they might happen to be in the same column. That would be invalid.

This leads us to the **Candidate Reduction Strategy**.

For any given row, do we really need to consider all 100 columns? No.
Consider a specific row. We want to place a rook here.

* Ideally, we pick the column with the absolute highest value.
* What if that column is taken by one of the other two rooks? Then we would want the column with the second highest value.
* What if that column is also taken by the other rook? Then we would want the column with the third highest value.

Is it possible that the third best column is also taken?
No!
We are only placing three rooks in total. Even in the worst case scenario where the best column and the second best column are blocked by the other two rooks, the third best column is guaranteed to be free.

Therefore, for every row on the board, we only ever need to care about the **three columns with the largest values**. Any column with a value smaller than the top three in that row will never be the optimal choice for a valid configuration, because one of the top three will always be available.

## The Algorithm Design

Based on the intuition above, we can structure our solution into two distinct phases.

### Phase 1: Preprocessing the Candidates

First, we will iterate through every row of the board. For each row, we want to identify the top three most valuable cells.

We can store this information in a simplified list. Instead of keeping the whole row, we will create a helper structure (like a 2D array or a vector of vectors) where `top_candidates[i]` contains just three pairs. Each pair holds the value and the column index of a top cell in row `i`.

For example, if a row looks like `[10, 50, 20, 40, 5]`, the sorted versions would be `50` (at index 1), `40` (at index 3), and `20` (at index 2). We discard the `10` and `5` because they will never be needed.

This step drastically reduces the amount of data we handle. We go from checking 100 columns per row to just 3 columns per row.

### Phase 2: Exhaustive Search on Reduced Data

Now that we have our reduced candidates, we can iterate through all combinations of three unique rows.

Let us use three loops:

1. **Loop i** from 0 to M.
2. **Loop j** from i + 1 to M.
3. **Loop k** from j + 1 to M.

We have now selected three distinct rows: , , and .

Inside these loops, we need to select the columns. We don't check the whole board. We only look at our precomputed `top_candidates` for these rows.

* Row  has 3 candidates.
* Row  has 3 candidates.
* Row  has 3 candidates.

We can use nested loops to try every combination of these candidates. That is , which equals 27 checks. This is a very small constant number!

For each combination of three candidates, we check if their column indices are distinct.

* Does `col_i` equal `col_j`?
* Does `col_i` equal `col_k`?
* Does `col_j` equal `col_k`?

If all columns are unique, we calculate the sum:
`Sum = Value(row i) + Value(row j) + Value(row k)`

We compare this sum to a global maximum variable and update it if the new sum is larger.

## Detailed Implementation Steps

Let us look at how we build this in C++.

### Step 1: Initialize

We start by grabbing the dimensions of the board,  and . We also need a place to store our "Top 3" lists. A `vector<vector<pair<int, int>>>` is perfect here. The outer vector represents rows. The inner vector holds pairs, where each pair is `{value, column_index}`.

### Step 2: The Precomputation Loop

We loop through each row index from 0 to .
Inside, we create a temporary list of all pairs `{value, column_index}` for that row.
We sort this temporary list in descending order based on the value.
After sorting, we take the first three elements and push them into our `top_candidates` storage.
(Note: If the board width  is less than 3, we take all of them. But the problem constraints say  is at least 3, so we will always take 3).

### Step 3: The Triple Row Loop

We set a variable `max_value` to a very small number (like `LLONG_MIN` from the limits library) to ensure any real sum will beat it.

We start our three nested loops to pick rows , , and .

* `i` goes from 0 to .
* `j` goes from `i + 1` to .
* `k` goes from `j + 1` to .

Using `j = i + 1` and `k = j + 1` ensures we never pick the same row twice and we don't repeat combinations (like picking rows 1, 2, 3 and then later 3, 2, 1). This is a standard combinatorial pattern.

### Step 4: The Candidate Combination Loop

Inside the row loops, we now have access to `top_candidates[i]`, `top_candidates[j]`, and `top_candidates[k]`.

We iterate through the 3 candidates for row  (let's call the current one `c1`).
We iterate through the 3 candidates for row  (let's call the current one `c2`).
We check: is `c1.column == c2.column`? If yes, this is invalid, so `continue`.

If they are valid so far, we iterate through the 3 candidates for row  (let's call the current one `c3`).
We check: is `c3.column == c1.column` or is `c3.column == c2.column`? If yes, invalid, so `continue`.

If we pass all checks, we have a valid set of 3 rooks!
We compute `current_sum = c1.value + c2.value + c3.value`.
We update `max_value = max(max_value, current_sum)`.

### Step 5: Return the Result

After all loops finish, `max_value` holds the highest possible sum achievable on the board. We simply return it.

## Complexity Analysis

It is important to understand why this solution works within the time limits.

**Time Complexity:**
Let  be the number of rows and  be the number of columns.

1. **Preprocessing:** For each of the  rows, we sort  elements. Sorting takes . So this part is .
2. **Main Loops:** We choose 3 rows out of . The number of ways to do this is roughly proportional to . Specifically, it is .
3. **Inner Work:** Inside the row loops, we perform a constant number of operations (checking  combinations). This is  relative to the input size.

So, the total complexity is dominated by the term . With ,  is 1,000,000. Computers can easily handle millions of operations per second (usually up to  is safe). Thus, this approach is very efficient.

**Space Complexity:**
We store 3 candidates for each of the  rows. This takes  space. This is very minimal memory usage.

## The Solution Code

Here is the C++ code that implements the logic described above. It is clean, uses standard libraries, and focuses on readability.

```cpp
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    long long maximumValueSum(vector<vector<int>>& board) {
        int m = board.size();
        int n = board[0].size();
        
        // Structure to hold our top 3 candidates per row.
        // Each element is a pair: {value, column_index}
        vector<vector<pair<int, int>>> top_candidates(m);
        
        // Phase 1: Preprocessing
        // Identify the best 3 columns for every row
        for(int r = 0; r < m; ++r) {
            vector<pair<int, int>> current_row_values;
            for(int c = 0; c < n; ++c) {
                current_row_values.push_back({board[r][c], c});
            }
            
            // Sort in descending order to put largest values first
            sort(current_row_values.rbegin(), current_row_values.rend());
            
            // Store only the top 3 (or fewer if n < 3, though constraints say n >= 3)
            for(int k = 0; k < 3 && k < n; ++k) {
                top_candidates[r].push_back(current_row_values[k]);
            }
        }
        
        long long max_sum = LLONG_MIN;
        
        // Phase 2: Iterate through all unique triplets of rows
        // We use indices i, j, k to represent the 3 rows we select
        for(int i = 0; i < m; ++i) {
            for(int j = i + 1; j < m; ++j) {
                for(int k = j + 1; k < m; ++k) {
                    
                    // Phase 3: Try all valid combinations of top columns
                    // Iterate through the precomputed candidates for row i
                    for(auto& rook1 : top_candidates[i]) {
                        
                        // Iterate through the precomputed candidates for row j
                        for(auto& rook2 : top_candidates[j]) {
                            // Check column conflict between rook 1 and rook 2
                            if(rook1.second == rook2.second) continue; 
                            
                            // Iterate through the precomputed candidates for row k
                            for(auto& rook3 : top_candidates[k]) {
                                // Check column conflict with rook 1 or rook 2
                                if(rook3.second == rook1.second || rook3.second == rook2.second) continue;
                                
                                // If we reach here, we have 3 rooks in different rows and columns
                                long long current_sum = (long long)rook1.first + rook2.first + rook3.first;
                                
                                // Update our global maximum
                                if(current_sum > max_sum) {
                                    max_sum = current_sum;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return max_sum;
    }
};

```

## Summary and Tips

This problem is a classic example of how "brute force" can be refined into an elegant solution by identifying the limits of interaction.

1. **Analyze Constraints:** Seeing  was the hint that we could use  logic.
2. **Reduce the Search Space:** Realizing that for any row, only the top 3 values matter was the breakthrough. It turned a problem of checking 100 columns into checking just 3.
3. **Structured Iteration:** Fixing the rows first () and then checking columns is much easier to manage than trying to pick random cells all at once.

I hope this explanation helps you understand not just how to solve this specific problem, but how to look for optimization opportunities in grid challenges. Happy coding and good luck with your algorithm practice!
