# Unlocking the Secrets of 2D Longest Increasing Paths [#3288]

Hello fellow coders and algorithm enthusiasts!

Today I want to share a fascinating journey into solving a challenging geometry and dynamic programming problem. If you have ever struggled with finding paths in a 2D grid or determining the Longest Increasing Subsequence (LIS) in complex data structures, this post is designed specifically for you. We will dissect the problem step by step, visualize the logic, and implement a robust solution in C++.

Let us dive right in together.

## Understanding the Challenge

Imagine you are given a scattering of points on a 2D plane. Each point has an X coordinate and a Y coordinate. You are also given a specific target point from this list, identified by an index `k`.

Your goal is to find the maximum number of points you can visit in a sequence, forming a path, subject to two very strict rules:

1.  **Strictly Increasing Coordinates**
    For every step you take from point A to point B, point B must be strictly to the right **and** strictly above point A. In other words, if you move from `(x1, y1)` to `(x2, y2)`, then `x2` must be greater than `x1` AND `y2` must be greater than `y1`.

2.  **Must Include the Target**
    The path you construct must pass through the specific point at index `k`.

This might sound like a graph problem or a Dynamic Programming challenge. It is actually a beautiful variation of the classic Longest Increasing Subsequence problem, but with a geometrical twist.

### Visualizing the Constraint

Let us picture the target point, which we will call `P_k`.
Because every point in our path must be strictly increasing, `P_k` acts as a bottleneck or a pivot.
* Any point coming *before* `P_k` in the path must have an X smaller than `P_k`'s X and a Y smaller than `P_k`'s Y. Geometrically, these points lie in the bottom left quadrant relative to `P_k`.
* Any point coming *after* `P_k` in the path must have an X larger than `P_k`'s X and a Y larger than `P_k`'s Y. These points lie in the top right quadrant relative to `P_k`.

This observation is the key to unlocking the solution. Instead of trying to find one long path through the whole cloud of points, we can split the problem into two smaller, independent tasks.

## The Divide and Conquer Strategy

Since the path is pinned at `P_k`, we can calculate two separate values:

1.  The length of the longest chain ending at `P_k`.
2.  The length of the longest chain starting at `P_k`.

The final answer will simply be the sum of these two lengths plus 1 (to account for `P_k` itself).

### Step 1 Filtering the Data

First, we iterate through our entire list of coordinates. We compare every point against our target `P_k`.
* If a point is strictly smaller in both X and Y, it belongs to the "Left Group".
* If a point is strictly larger in both X and Y, it belongs to the "Right Group".
* If a point is mixed (for example, larger X but smaller Y), it cannot possibly be part of a strictly increasing path passing through `P_k`, so we simply ignore it.

### Step 2 The 1D Reduction Trick

Now we have two sets of points, and we need to find the longest path within each set. This is a classic 2D LIS problem. The standard naive solution would be `O(N squared)`, which is too slow for large inputs. We need an `O(N log N)` approach.

Here is the clever trick to convert this 2D problem into a 1D problem.

If we sort our points based on their X coordinates, we guarantee that as we iterate through the list, the X requirement is automatically satisfied (since we are moving from left to right). This allows us to ignore X and focus solely on finding the Longest Increasing Subsequence of the Y coordinates.

However, there is a catch. What if two points have the same X coordinate?
If we have point A at `(2, 2)` and point B at `(2, 4)`, we cannot use both in the same path because the X values are not strictly increasing; they are equal.

To handle this, we use a custom sorting comparator:
**Sort by X in ascending order. If X values are tied, sort by Y in descending order.**

### Why Sort Y Descending?

This is the most critical logic in the solution. Let us illustrate with an example.
Suppose we have points `(3, 1)`, `(3, 2)`, and `(3, 3)`.
If we sort them normally (X asc, Y asc), we get `(3, 1), (3, 2), (3, 3)`.
If we feed the Y values `1, 2, 3` into a standard LIS algorithm, it will say the length is 3. But this is wrong! We cannot visit all three because their X values are identical.

Now, let us try the custom sort (X asc, Y desc).
The order becomes `(3, 3), (3, 2), (3, 1)`.
The Y sequence is `3, 2, 1`.
The Longest Increasing Subsequence of `3, 2, 1` has a length of 1. This is correct! We can only pick one point from this group.

By sorting Y in descending order for ties, we ensure that a standard LIS algorithm will never pick more than one point with the same X coordinate, because the Y values will be decreasing, preventing the "increasing" condition from triggering.

## The Algorithm in Action

Let us walk through the algorithm logic with the "Right Group" as an example.

1.  **Collect Points**
    Gather all points where `x > P_k.x` and `y > P_k.y`.

2.  **Custom Sort**
    Sort these points such that X increases. For equal X, Y decreases.

3.  **Find LIS on Y**
    We iterate through the sorted points and extract the Y values. We maintain a helper list called `tails`.
    * For each Y value, use binary search to find the first element in `tails` that is greater than or equal to Y.
    * If found, replace that element with Y. (This keeps the potential end of the sequence small, allowing for longer future sequences).
    * If not found (meaning Y is larger than everything in `tails`), append Y to `tails`.
    * The size of `tails` at the end is the length of the longest increasing path.

## C++ Implementation Details

Below is the complete C++ code. I have kept the style clean and used standard library features for readability.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxPathLength(vector<vector<int>>& coordinates, int k) {
        // Extract the coordinates of the pivot point k
        int kx = coordinates[k][0];
        int ky = coordinates[k][1];
        
        // We will separate the points into two buckets
        // left: points that can come strictly BEFORE k
        // right: points that can come strictly AFTER k
        vector<vector<int>> leftGroup;
        vector<vector<int>> rightGroup;
        
        for (const auto& point : coordinates) {
            int x = point[0];
            int y = point[1];
            
            if (x < kx && y < ky) {
                leftGroup.push_back(point);
            } else if (x > kx && y > ky) {
                rightGroup.push_back(point);
            }
        }
        
        // Calculate LIS for both sides
        // The total length is (left side) + (point k itself) + (right side)
        return solveLIS(leftGroup) + 1 + solveLIS(rightGroup);
    }

private:
    // Helper function to solve 2D LIS
    int solveLIS(vector<vector<int>>& points) {
        if (points.empty()) {
            return 0;
        }
        
        // Custom Sort Comparator
        // 1. Sort by X in ascending order
        // 2. If X is equal, sort by Y in descending order
        sort(points.begin(), points.end(), 
             [](const vector<int>& a, const vector<int>& b) {
            if (a[0] == b[0]) {
                return a[1] > b[1]; 
            }
            return a[0] < b[0];
        });
        
        // Standard LIS algorithm on Y coordinates using the 'tails' list
        vector<int> tails;
        
        for (const auto& point : points) {
            int y = point[1];
            
            // Binary search for the first element >= y
            // lower_bound returns an iterator to the first element that does not compare less than y
            auto it = lower_bound(tails.begin(), tails.end(), y);
            
            if (it == tails.end()) {
                // If y is larger than all tails, we extend the longest sequence
                tails.push_back(y);
            } else {
                // If we found a larger or equal value, we replace it
                // This maintains the 'smallest possible end element' property
                *it = y;
            }
        }
        
        return tails.size();
    }
};

```

## Complexity Analysis

It is important to understand how efficient this solution is.

### Time Complexity

Let N be the number of coordinates.

1. **Filtering:** We iterate through the list once to split into groups. This takes `O(N)`.
2. **Sorting:** We sort the filtered lists. In the worst case, sorting takes `O(N log N)`.
3. **LIS Calculation:** For each point in the sorted list, we perform a binary search on the `tails` vector. The binary search takes `O(log N)`. Doing this for N points results in `O(N log N)`.

Combining these steps, the overall time complexity is **`O(N log N)`**. This is highly efficient and suitable for constraints where N is up to 100,000.

### Space Complexity

We create two auxiliary vectors (`leftGroup` and `rightGroup`) and the `tails` vector. In the worst case, these store all N points. Therefore, the space complexity is **`O(N)`**.

## Closing Thoughts

This problem is a fantastic example of how changing your perspective can simplify a difficult task. By realizing that the point `k` splits the problem into two independent subproblems, we reduce the complexity significantly. Furthermore, the "sort X ascending, Y descending" technique is a powerful pattern to keep in your toolkit for any 2D interval or pathing problems.

I hope this explanation helps you understand not just the *how* but the *why* behind the solution. Happy coding and keep learning!
