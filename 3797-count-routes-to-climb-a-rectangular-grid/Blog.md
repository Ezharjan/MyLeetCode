# Navigating the Grid A Journey into Dynamic Programming and Euclidean Distances [#3797]

Welcome fellow coding enthusiasts! Today we are going to tackle a fascinating challenge that combines grid navigation with some interesting geometric constraints. It is a problem that tests our ability to manage states in Dynamic Programming and optimize range queries. If you have ever felt stuck trying to optimize a grid problem from cubic complexity down to quadratic time, this walkthrough is for you.

We will be exploring the problem "Count Routes to Climb a Rectangular Grid". It is a hard difficulty problem that requires careful thought about how we define "valid moves" and how we handle spatial constraints efficiently.

Let us dive in together!

---

## 1. Understanding the Mission

Imagine you are standing at the bottom of a tall rectangular grid. Your goal is simple on the surface. You need to climb from any valid cell in the bottom row all the way up to the top row. The grid is filled with open spaces (represented by dots `.`) and obstacles (represented by hashes `#`).

However, the rules of movement are quite unique.

### The Movement Rules

You are not limited to just moving Up, Down, Left, or Right. Instead, you can jump between any two cells as long as the **Euclidean distance** between them is within a certain limit, let us call it `d`.

Here is the twist. You have a specific restriction on how you move between rows.

* **Upward Drift**
    Every move you make must either keep you on the same row or move you exactly one row up. You cannot move down, and you cannot jump more than one row at a time.
    
* **The Horizontal Limit**
    You are not allowed to stay on the same row for two consecutive turns. This is the crucial constraint. If you make a move that lands you on the same row you started on (a horizontal move), your very next move **must** take you to the row above. You cannot slide sideways twice in a row.

We need to count the total number of valid routes that start at the bottom (row `n minus 1`) and end at the top (row `0`). Because the number of paths can be astronomical, we will return the result modulo `1,000,000,007`.

---

## 2. Breaking Down the Logic

When we see a problem involving counting paths on a grid, Dynamic Programming (DP) is usually the first tool we should reach for.

In a standard grid problem, `dp[row][col]` might simply store "the number of ways to reach this cell." But here, that is not enough. Why? Because of the **Horizontal Limit**.

To know if we are allowed to move horizontally from our current cell, we need to know how we got there.
* Did we arrive here from the row below? If yes, we are fresh! We can choose to move horizontally or vertically next.
* Did we arrive here from the same row? If yes, our "horizontal stamina" is depleted. We *must* move vertically next.

This suggests we need two distinct states for every cell `(row, col)`.

### Defining Our States

Let us define two arrays (or vectors) for the current row we are processing. Let us assume we are processing the grid from the bottom (row `n minus 1`) up to the top (row `0`).

**State 1. Vertical Arrivals (v)**
`v[col]` will store the number of valid paths ending at the current cell `(row, col)` where the last move came from the **row below** (`row plus 1`).
* *Significance* If we are in this state, we are free to make a horizontal move next if we wish.

**State 2. Horizontal Arrivals (h)**
`h[col]` will store the number of valid paths ending at the current cell `(row, col)` where the last move came from the **same row** (`row`).
* *Significance* If we are in this state, we cannot make another horizontal move. Our next step must be vertical.

### The Transition Strategy

We will process the grid one row at a time. Since we move from the bottom up, we can calculate the values for row `i` based entirely on the values from row `i plus 1` (and other values within row `i`).

Let us analyze the transitions for a specific row `i`.

#### Part A. Calculating Vertical Arrivals
To calculate `v[col]` for the current row `i`, we look at the row below, `i plus 1`. We can jump from any column `k` in row `i plus 1` to column `col` in row `i`, provided the Euclidean distance is valid.

The Euclidean distance formula is `sqrt((r1 minus r2)^2 + (c1 minus c2)^2)`.
Since the row difference is exactly 1, this simplifies to `sqrt(1^2 + (col minus k)^2)`.
We need this to be less than or equal to `d`.
Squaring both sides removes the square root. `1 + (col minus k)^2 <= d^2`.
Solving for the column difference. `(col minus k)^2 <= d^2 minus 1`.
Finally, `abs(col minus k) <= sqrt(d^2 minus 1)`.

So, for a target cell at `col`, we can sum up all paths from the row below where the column index `k` falls within this range.
The source paths can be **anything** (both `v` and `h` from the previous row), because moving vertically resets the horizontal restriction.

#### Part B. Calculating Horizontal Arrivals
To calculate `h[col]` for the current row `i`, we look at other cells in the **same row** `i`. We can jump from column `k` to column `col` if the distance is valid.

Since the row difference is 0, the distance is just `abs(col minus k)`.
So, `abs(col minus k) <= d`.

However, we must be careful about the source. We cannot chain horizontal moves. Therefore, we can only arrive at `h[col]` from a cell `k` that was a **Vertical Arrival** (`v[k]`). We cannot come from `h[k]`.

So, `h[col]` is the sum of `v[k]` for all `k` within distance `d` (excluding `k` equal to `col`).

---

## 3. The Efficiency Bottleneck

If we implement the logic above using simple loops, let us look at the complexity.
We have `n` rows and `m` columns. For every cell, we might iterate through neighbors up to distance `d`.
The complexity would be roughly `O(n * m * d)`.

Given the constraints where `n`, `m`, and `d` can all be up to 750, the total operations could reach around 400 million. This is on the edge of being too slow for typical time limits (usually around 1 second). We need something faster.

### The Optimization Prefix Sums

Notice that for both transitions, we are essentially asking for the **sum of a range of values** in an array.

1.  For Vertical Arrivals, we need the sum of `(prev_v[k] + prev_h[k])` for `k` in range `[col minus limit, col plus limit]`.
2.  For Horizontal Arrivals, we need the sum of `curr_v[k]` for `k` in range `[col minus d, col plus d]`.

We can use **Prefix Sum Arrays** to answer these queries in `O(1)` constant time!

By building a prefix sum array for the previous row's totals and the current row's vertical arrivals, we reduce the inner loop from `O(d)` to `O(1)`.
This brings our total complexity down to `O(n * m)`, which is roughly 750 * 750 = 562,500 operations per test case. This is extremely fast and well within limits.

---

## 4. Step by Step Algorithm

Here is the complete recipe for our solution.

1.  **Initialize**
    Create two arrays `v` and `h` of size `m` (number of columns).
    Set `MOD` to `1,000,000,007`.

2.  **Base Case (The Bottom Row)**
    For the bottom row (index `n minus 1`), any cell that is not an obstacle (`.`) is a valid starting point.
    Set `v[col] = 1` for these valid cells.
    Why `v`? Because starting at the bottom is conceptually like arriving from "the ground" (a vertical move).
    
    After setting `v`, we calculate `h` for the bottom row. Even on the first row, we can make a horizontal jump immediately after starting.
    Calculate prefix sums of `v`.
    For each `col`, `h[col]` is the sum of `v` in the range `[col minus d, col plus d]` excluding `v[col]` itself.

3.  **Iterate Upwards**
    Loop from row `i = n minus 2` down to `0`.

4.  **Process Vertical Moves (Into Row i)**
    Compute a combined source array from the row below `total_prev[k] = (v[k] + h[k])`.
    Build a prefix sum for `total_prev`.
    Calculate the vertical jump radius `limit = floor(sqrt(d^2 minus 1))`.
    For each column `j`, calculate `next_v[j]` by querying the prefix sum in range `[j minus limit, j plus limit]`.
    *Note* If the cell `grid[i][j]` is an obstacle `#`, `next_v[j]` stays 0.

5.  **Process Horizontal Moves (Within Row i)**
    Now that we have `next_v` (paths that just arrived at row `i`), we can calculate `next_h`.
    Build a prefix sum for `next_v`.
    For each column `j`, calculate `next_h[j]` by querying the prefix sum in range `[j minus d, j plus d]`. Remember to subtract the value at `j` itself (distance must be greater than 0).
    *Note* If `grid[i][j]` is an obstacle, `next_h[j]` stays 0.

6.  **Update States**
    Replace our old `v` and `h` arrays with `next_v` and `next_h`.

7.  **Final Count**
    After the loop finishes processing the top row (row 0), sum up all values in `v` and `h`. This is our answer.

---

## 5. The Solution Code

Here is the C++ implementation. I have kept it clean and modular. We use a helper lambda function for the prefix sum logic to keep the main loop tidy.

```cpp
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

class Solution {
public:
    int numberOfRoutes(vector<string>& grid, int d) {
        int rows = grid.size();
        int cols = grid[0].size();
        long long MOD = 1e9 + 7;

        // DP arrays for the current row being processed
        // v[j] stores paths ending at col j via a vertical move from below
        // h[j] stores paths ending at col j via a horizontal move
        vector<long long> v(cols, 0);
        vector<long long> h(cols, 0);

        // --- Helper Function for Range Sum Queries ---
        // Creates a prefix sum array P where P[i] is sum of arr[0...i-1]
        auto compute_prefix = [&](const vector<long long>& arr) {
            vector<long long> P(cols + 1, 0);
            for (int k = 0; k < cols; ++k) {
                P[k + 1] = (P[k] + arr[k]) % MOD;
            }
            return P;
        };

        // Gets sum of arr[L...R] using the prefix array P
        auto get_sum = [&](const vector<long long>& P, int L, int R) {
            if (L < 0) L = 0;
            if (R >= cols) R = cols - 1;
            if (L > R) return 0LL;
            // P[R+1] contains sum up to R
            // P[L] contains sum up to L-1
            return (P[R + 1] - P[L] + MOD) % MOD;
        };

        // --- Base Case: The Bottom Row (rows - 1) ---
        // We start by filling 'v' as if we arrived from "below" the grid
        for (int j = 0; j < cols; ++j) {
            if (grid[rows - 1][j] == '.') {
                v[j] = 1;
            }
        }

        // Calculate 'h' for the bottom row (horizontal moves from start points)
        vector<long long> P_v = compute_prefix(v);
        for (int j = 0; j < cols; ++j) {
            if (grid[rows - 1][j] == '#') continue;
            
            // On same row, distance simply needs to be <= d
            // We subtract v[j] because we must move to a DIFFERENT cell
            long long sum_val = get_sum(P_v, j - d, j + d);
            long long self = v[j];
            h[j] = (sum_val - self + MOD) % MOD;
        }

        // --- Main Loop: Move upwards from second-to-last row ---
        for (int i = rows - 2; i >= 0; --i) {
            vector<long long> next_v(cols, 0);
            vector<long long> next_h(cols, 0);

            // 1. Calculate next_v (Vertical Arrivals)
            // We can come from ANY path in the row below (v or h)
            vector<long long> total_prev(cols);
            for(int k = 0; k < cols; ++k) {
                total_prev[k] = (v[k] + h[k]) % MOD;
            }
            vector<long long> P_total = compute_prefix(total_prev);

            // Calculate max horizontal offset allowed for a vertical step
            // sqrt(1^2 + dx^2) <= d  =>  dx <= sqrt(d^2 - 1)
            long long d_squared = (long long)d * d;
            int v_radius = 0;
            if (d_squared >= 1) {
                v_radius = floor(sqrt(d_squared - 1));
            } else {
                // If d=1, d^2-1=0, so radius is 0 (straight up only)
                v_radius = 0; 
            }

            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == '#') continue;
                next_v[j] = get_sum(P_total, j - v_radius, j + v_radius);
            }

            // 2. Calculate next_h (Horizontal Arrivals)
            // We can only come from cells in the SAME row that were Vertical Arrivals (next_v)
            vector<long long> P_next_v = compute_prefix(next_v);
            
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == '#') continue;
                
                // Horizontal move radius is simply d
                long long sum_val = get_sum(P_next_v, j - d, j + d);
                long long self = next_v[j];
                next_h[j] = (sum_val - self + MOD) % MOD;
            }

            // Move to the next state
            v = next_v;
            h = next_h;
        }

        // --- Final Answer ---
        // Sum all valid paths ending at the top row (row 0)
        long long ans = 0;
        for (int j = 0; j < cols; ++j) {
            ans = (ans + v[j] + h[j]) % MOD;
        }

        return ans;
    }
};

```

---

## 6. Complexity Analysis

It is important to understand why this solution works efficiently.

### Time Complexity

* **Preprocessing** Calculating the prefix sums takes `O(m)` time, where `m` is the number of columns.
* **Processing each row** We iterate through each of the `n` rows. Inside the row loop, we perform constant time operations for each of the `m` columns thanks to the prefix sums.
* **Total Time** The complexity is `O(n * m)`. With `n` and `m` up to 750, this is well within the safety margin for execution time.

### Space Complexity

* We only store the DP arrays for the current row and the previous row (`v` and `h`).
* This means our space usage is `O(m)`. We do not need to store the entire `n * m` grid in memory for the DP states, just the active frontier. This is very memory efficient.

---

## Final Thoughts

This problem is a beautiful example of how constraints (like "no two horizontal moves") translate directly into DP states. It also demonstrates the power of the **Prefix Sum** technique. Whenever you see a DP transition that involves summing a contiguous range of values, always think of Prefix Sums to shave off a factor of complexity.

I hope this guide helps you understand the mechanics of grid navigation with Euclidean constraints. It is a tough problem, but breaking it down state by state makes it manageable.

Happy Coding!
