# Mastering Grid Selection with Maximum Score and Unique Constraints [#3276]

[Blog Post Link](https://leetcode.com/discuss/post/7559244/mastering-grid-selection-with-maximum-sc-qith/)


Hello, fellow coders! Welcome back to another deep dive into an interesting algorithmic challenge. Today, we are going to tackle a problem that looks deceptively simple on the surface but quickly reveals a beautiful layer of complexity. It is a fantastic exercise for practicing dynamic programming and bitmasking techniques.

If you have ever stared at a grid of numbers and wondered how to pick the best ones without breaking a set of strict rules, this post is for you. We are going to explore how to select cells from a matrix to maximize a total score while ensuring that every selected number is unique and no two cells come from the same row.

Let’s jump right in!

---

## Understanding the Challenge

Before we write a single line of code, let’s make sure we truly understand what we are being asked to do.

Imagine you are given a grid (a 2D matrix) filled with positive integers. Your goal is to select a set of cells from this grid to get the highest possible sum. However, there are two strict rules you must follow:

1. **Row Constraint** You cannot pick more than one cell from the same row.
2. **Value Constraint** You cannot pick the same number value more than once. All selected values must be unique.

### A Visual Example

Let’s look at a small example to visualize this. Suppose we have the following 3x3 grid:

|  | Column 0 | Column 1 | Column 2 |
| --- | --- | --- | --- |
| **Row 0** | 1 | 2 | 3 |
| **Row 1** | 4 | 3 | 2 |
| **Row 2** | 1 | 1 | 1 |

We want to pick numbers to make the sum as large as possible.

* **Attempt 1** You might be tempted to just pick the biggest numbers available. The biggest number is `4` (Row 1). The next biggest is `3` (Row 0 or Row 1).
* If we pick `4` from Row 1, we cannot pick anything else from Row 1.
* Now we look at Row 0. The available numbers are `1`, `2`, `3`. The largest is `3`. Since `3` is different from `4`, we can pick it.
* Now we look at Row 2. The only option is `1`. We haven't picked `1` yet, and we haven't touched Row 2 yet. So we pick it.
* **Total Score** 4 + 3 + 1 = 8.

This is a valid selection! Can we do better? Let's check. If we picked the `2` from Row 1, we would be leaving the `4` behind, which seems suboptimal. In this specific case, 8 happens to be the maximum score.

### Why Greedy Fails

A "Greedy" approach means always taking the best immediate option (like the biggest number) right now. While it worked in the example above, it often fails in harder cases.

Consider this grid:

|  | Column 0 | Column 1 |
| --- | --- | --- |
| **Row 0** | 10 | 5 |
| **Row 1** | 10 | 100 |

* **Greedy Approach** We see `100` in Row 1. We take it!
* Now Row 1 is used. We look at Row 0. We have `10` and `5`.
* We want to take `10`, but wait! We already took `100`. If we assume the values must be unique, `10` is fine (10 is not 100).
* Wait, let's look closer. What if the values were distinct? Actually, let's look at a case where the values conflict.

Consider:
Row 0: `[5, 9]`
Row 1: `[9, 10]`

* If we greedily pick the largest number `10` from Row 1, Row 1 is used.
* Now we look at Row 0. We have `5` and `9`. We pick `9`.
* Total: 19.
* **Alternative** What if we picked `9` from Row 0 and `9` from Row 1?
* This is **illegal** because the values must be unique! We cannot pick `9` twice.
* **Correct Strategy** Pick `9` from Row 0. Then pick `10` from Row 1? Yes, 19.
* What if we picked `5` from Row 0? Then `10` from Row 1. Total 15.

The conflict comes when the **same value** appears in multiple rows, or when taking a value from a specific row prevents you from taking a much better value later because that row is now "locked."

We need a strategy that explores different combinations efficiently.

---

## Flipping the Perspective

The most intuitive way to solve grid problems is usually to iterate through the grid cell by cell (Row 0, then Row 1, etc.). However, the constraints here give us a hint to try something different.

The constraints say: **Values must be unique.**

This suggests that instead of iterating through **Rows**, we should iterate through **Values**.

The range of values in the grid is relatively small (1 to 100). The number of rows is also very small (1 to 10). This is a huge clue!

Let's rephrase the decision process. Instead of saying "For Row 0, which cell do I pick?", let's say:
**"For the value 100, which Row should I pick it from?"**

We can iterate through all possible values from largest to smallest (100 down to 1). For each value, we have a few choices:

1. **Skip it.** We don't include this number in our sum.
2. **Pick it.** If we pick it, we must choose a row that contains this number. Crucially, that row must not have been used yet.

This perspective shift is powerful because it handles the "unique values" constraint automatically (we only process each number value once) and simplifies the "unique rows" constraint into a tracking problem.

---

## The Power of Bitmasks

We need a way to keep track of which rows have been used. Since there are at most 10 rows, we can use an integer as a **Bitmask**.

A bitmask is just a binary number where each bit represents a boolean flag.

* If the -th bit is `0`, Row  is free.
* If the -th bit is `1`, Row  is already taken.

For example, if we have 3 rows:

* `000` (0 in decimal) means no rows are used.
* `001` (1 in decimal) means Row 0 is used.
* `101` (5 in decimal) means Row 0 and Row 2 are used.

Using a bitmask allows us to pass the state of all rows around in a single integer, which is very fast and efficient for memory.

---

## Designing the Algorithm

We will use **Recursion with Memoization** (also known as Top-Down Dynamic Programming).

### 1. Data Preprocessing

First, we need to know where every value lives. We can build a lookup table.
Let's call it `rowsByValue`.

* `rowsByValue[5]` will be a list of row indices where the number `5` appears.
* `rowsByValue[10]` will be a list of row indices where `10` appears.

We also make a list of all unique values present in the grid and sort them in descending order. We want to try picking the biggest numbers first to potentially reach a higher score faster (though for DP, the order strictly serves as our iteration index).

### 2. The Recursive Function `solve(index, mask)`

Our function will look like this:

* **Input:**
* `index`: The current position in our sorted list of unique values.
* `mask`: An integer representing which rows are currently occupied.


* **Output:** The maximum score achievable from this state.

### 3. The Transitions

Inside the function, for the current value (let's call it `val`), we have two main branches of logic:

**Option A: Ignore this value.**
Maybe taking this value forces us to use a row that is needed for an even better number later. So, we recursively call the function for the next value without changing the mask.

* `res = solve(index + 1, mask)`

**Option B: Take this value.**
We look at `rowsByValue[val]`. For every row `r` where this value appears:

* Check if Row `r` is currently free in our `mask`.
* If it is free, we can "take" it.
* We calculate the score: `val + solve(index + 1, mask | (1 << r))`
* We update our result `res` if this option yields a higher total.

### 4. Memoization

Because we might arrive at the same state (same `index` and same `mask`) through different paths, we store the result in a table `memo[index][mask]`. If we see this state again, we return the stored answer instantly.

---

## Implementation Details

Now, let's write this out in C++. I will guide you through the specific structure.

### The Class Structure

We will use a `Solution` class. We need member variables to store our preprocessed data and our memoization table.

### Preprocessing Logic

We iterate through the entire grid. If `grid[i][j]` is `50`, we add row index `i` to our list for value `50`. After scanning the grid, we clean up these lists (remove duplicates) and create our master list of values to iterate over.

### The Bit Operations

* **Check if row `i` is used:** `(mask >> i) & 1`
* **Mark row `i` as used:** `mask | (1 << i)`

---

## The Complete Solution

Here is the full, clean C++ code. I have added comments to explain every block.

```cpp
#include <vector>
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

class Solution {
private:
    // This array will store which rows contain a specific value.
    // Since values are up to 100, we need size 101.
    vector<int> rowsByValue[101];
    
    // A list of all the unique values found in the grid.
    vector<int> allValues;
    
    // Memoization table. 
    // Dimensions: [Index of value we are considering][Current Mask of used rows]
    // Index goes up to 100. Mask goes up to 2^10 = 1024.
    int memo[101][1024]; 

    // The core recursive function
    int solve(int valIdx, int mask) {
        // Base Case: If we have gone through all unique values, we can't add more score.
        if (valIdx == allValues.size()) {
            return 0;
        }

        // Check if we have already solved this exact scenario
        if (memo[valIdx][mask] != -1) {
            return memo[valIdx][mask];
        }

        int currentVal = allValues[valIdx];

        // OPTION 1: Skip the current value.
        // We decide not to pick 'currentVal' at all and move to the next value.
        int maxScore = solve(valIdx + 1, mask);

        // OPTION 2: Try to pick 'currentVal'.
        // We can only pick it from a row that contains it AND is not yet used.
        for (int row : rowsByValue[currentVal]) {
            // Check if the bit at 'row' is 0 (meaning the row is free)
            if (!(mask & (1 << row))) {
                // Determine the new mask if we use this row
                int newMask = mask | (1 << row);
                
                // Calculate score: value + result of remaining recursion
                int currentOptionScore = currentVal + solve(valIdx + 1, newMask);
                
                // Keep the maximum result
                maxScore = max(maxScore, currentOptionScore);
            }
        }

        // Store the result before returning
        return memo[valIdx][mask] = maxScore;
    }

public:
    int maxScore(vector<vector<int>>& grid) {
        // 1. Reset data structures for the new test case
        // This is important because the class instance might be reused.
        for (int i = 0; i < 101; ++i) {
            rowsByValue[i].clear();
        }
        allValues.clear();
        
        // Initialize memo table with -1 (indicating "not computed yet")
        memset(memo, -1, sizeof(memo));

        int m = grid.size();     // Number of rows
        int n = grid[0].size();  // Number of columns

        // 2. Preprocess the grid
        // We map every value to the list of rows it appears in.
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                int val = grid[r][c];
                rowsByValue[val].push_back(r);
            }
        }

        // 3. Build the sorted unique values list
        // Iterating from 100 down to 1 ensures we handle larger numbers first 
        // (though order doesn't strictly change correctness for this DP, 
        // it helps with organizing the recursion).
        for (int v = 100; v >= 1; --v) {
            if (!rowsByValue[v].empty()) {
                // We remove duplicate row indices. 
                // If a value appears twice in Row 0, we only need to know it's in Row 0 once.
                sort(rowsByValue[v].begin(), rowsByValue[v].end());
                rowsByValue[v].erase(unique(rowsByValue[v].begin(), rowsByValue[v].end()), rowsByValue[v].end());
                
                allValues.push_back(v);
            }
        }

        // 4. Start the recursion from the first value (index 0) and an empty mask (0)
        return solve(0, 0);
    }
};

```

---

## Why This Works: Complexity Analysis

When analyzing algorithms, especially recursive ones, it is helpful to look at the "State Space"—how many distinct inputs the recursive function can receive.

### Time Complexity

* **The State Variables:**
* `valIdx`: This ranges from 0 to the number of unique values. In the worst case, every number from 1 to 100 is present. So, this is approx **100**.
* `mask`: This ranges from 0 to `2^m - 1`, where `m` is the number of rows. Since `m <= 10`, `2^10` is **1024**.


Total number of states = `100 * 1024` ≈ `100,000`. This is very small for a computer!
* **The Transitions:**
Inside each function call, we iterate through the rows where the current value appears. In the worst case, a value appears in all 10 rows.
Total operations ≈ `Number of States * Transitions per State`
Total operations ≈ `100,000 * 10` = `1,000,000`.
One million operations is well within the typical time limit for competitive programming (which usually allows for ~100 million operations per second).

### Space Complexity

* **Memoization Table:** We store `100 * 1024` integers. This takes up less than 1 MB of memory.
* **Recursion Stack:** The recursion depth is at most 100 (one level for each unique value).
* **Auxiliary Storage:** The `rowsByValue` array stores row indices, which is negligible.

Overall, this solution is extremely efficient in terms of both time and memory.

---

## Key Takeaways

1. **Constraint Analysis is Key:** The small number of rows (10) screamed "Bitmasking". The small range of values (100) screamed "Iterate by Value". Always look at the constraints first!
2. **Change Your Perspective:** We are used to processing grids by coordinates (x, y). Sometimes, processing by value allows you to bypass complex checking logic.
3. **Bitmasks are Friends:** Whenever you need to track a small set of boolean states (like "is this row used?"), an integer bitmask is usually the cleanest and fastest tool for the job.

I hope this walkthrough helps you understand not just how to solve this specific problem, but how to approach similar constraint-satisfaction problems in the future.

Happy Coding!
