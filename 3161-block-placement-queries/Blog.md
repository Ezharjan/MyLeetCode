# Navigating the Number Line Efficiently with Segment Trees and Sets [#3161]

[Blog Post Link](https://leetcode.com/discuss/post/7564362/navigating-the-number-line-efficiently-w-6b9x/)

Welcome fellow coders! Today we are going to dive deep into a fascinating problem that combines spatial reasoning with efficient data structure management. If you have ever enjoyed problems involving intervals, dynamic updates, or resource allocation on a timeline, this challenge is right up your alley. We will explore how to manage obstacles on a number line and determine if we can fit blocks of a certain size into specific regions.

This approach relies on two powerful tools in algorithmic programming which are the Segment Tree and the Ordered Set. By the end of this post, you will have a clear mental model of how to handle dynamic gap queries and a solid C++ solution to add to your toolkit.

## Understanding the Landscape

Let us start by clearly defining the problem. Imagine you have an infinite number line starting at zero and extending infinitely to the right. Initially, the line is empty. You are given a series of commands, or "queries," that modify this line or ask questions about it.

There are two types of interactions you need to handle.

### Type 1 Interaction The Obstacle
You are given a coordinate `x`. You must build an obstacle at this exact distance `x` from the origin. The problem guarantees that no obstacle currently exists at that location. You can think of this as placing a permanent divider at that point on the line.

### Type 2 Interaction The Feasibility Check
You are given a coordinate `x` and a size value `sz`. The question is simple yet tricky. Is it possible to place a block of width `sz` somewhere in the range `[0, x]`?

There are strict rules for this placement.
1. The block must lie entirely within `[0, x]`.
2. The block cannot overlap with any obstacles.
3. The block can touch an obstacle, but it cannot cross it.

Essentially, we are looking for a continuous empty gap of length at least `sz` that exists completely before the coordinate `x`.

## The Scale of the Problem

Before jumping into solutions, we must look at the constraints to understand what computational resources we have.
* The coordinate values `x` are up to 50,000.
* The number of queries is up to 50,000 (based on the coordinate limit) or up to 150,000 total operations.
* This suggests that an algorithm with O(N squared) complexity will be too slow. We need something closer to O(N log N) or O(N sqrt N).

## The Naive Approach and Its Limitations

When we first look at this, it is tempting to simulate the number line directly. Since the maximum coordinate is around 50,000, one might create a boolean array of size 50,000 where `true` means an obstacle exists and `false` means it is empty.

For a Type 1 query (placing an obstacle), we just update the array index to `true`. This is very fast, taking O(1) time.

However, the Type 2 query (checking for space) becomes difficult. To find if a gap of size `sz` exists before `x`, we would have to iterate through the array from 0 to `x`, counting consecutive `false` values. In the worst case, this linear scan takes O(x) time. If we have many Type 2 queries with large `x` values, our total time complexity skyrockets, leading to a Time Limit Exceeded verdict.

We need a way to answer "what is the largest gap in this range?" much faster than checking every single point.

## A Better Strategy Modeling Gaps

Let us rethink the problem. We do not care about every single point on the line. We really only care about the **intervals** or **gaps** formed by the obstacles.

Imagine the number line as a long string.
* Initially, we have one giant interval starting at 0.
* When we place an obstacle at `x`, we cut the interval containing `x` into two smaller intervals.
* When we want to place a block of size `sz` before `x`, we need to know if the maximum interval length in the range `[0, x]` is at least `sz`.

This gives us the hint for our solution. We need a system that can **dynamically split intervals** and **efficiently query the maximum value** over a range.

## The Two Data Structures

To achieve this, we will combine two data structures.

1.  **std::set (Ordered Set)**
    We need to know where the obstacles are to calculate gap sizes. An ordered set is perfect for this. It keeps all obstacle coordinates sorted. This allows us to find the nearest obstacle to the left and right of any point in logarithmic time.

2.  **Segment Tree (Range Maximum Query)**
    We need to know the largest gap size in a specific range. A Segment Tree is a binary tree structure that allows us to update values and query the maximum value in a range, both in O(log N) time.

### Designing the Segment Tree

This is the most critical part of the logic. What exactly does the Segment Tree store?

We will build a Segment Tree over the coordinates from 0 to 50,000.
At every index `i` in the Segment Tree, we will store the **size of the gap that ends at `i`**.

* If there is no obstacle at `i`, we store 0.
* If there is an obstacle at `i`, and the previous obstacle was at `prev`, then the gap ending at `i` has size `i` + `prev`. We store this value at index `i`.

Why do we do this?
By storing the gap size at the coordinate where the gap *ends*, a Range Maximum Query from 0 to `x` tells us the size of the largest **completed** gap that fits entirely within the prefix.

## The Algorithm Step by Step

Let us walk through the lifecycle of the queries.

### Initialization
We start with a Segment Tree filled with zeros.
We also initialize our ordered set. It is helpful to treat the origin 0 as an implicit obstacle because no block can go before 0. So, we insert `0` into our set.

### Handling Query Type 1 Placing an Obstacle
Suppose we receive a query to place an obstacle at `x`.

1.  **Find Neighbors**
    Using our set, we find the existing obstacles immediately surrounding `x`. Let us call the obstacle to the left `L` and the obstacle to the right `R`.
    (Note that `R` might not exist if `x` is larger than all current obstacles).

2.  **Update the Old Gap**
    Before placing `x`, there was a single gap between `L` and `R`. The size of this gap was stored in the Segment Tree at index `R`.
    Since we are placing an obstacle at `x`, the gap ending at `R` is no longer `R` minus `L`. It has shrunk! It is now the gap between `x` and `R`.
    So, we update the Segment Tree at index `R` with the new value `R` minus `x`.

3.  **Create the New Gap**
    Now we have a new gap formed between `L` and `x`. The size is `x + L`.
    We update the Segment Tree at index `x` with this value.

4.  **Update Set**
    Finally, we insert `x` into our set of obstacles.

### Handling Query Type 2 Checking Feasibility
Suppose we receive a query to check if a block of size `sz` fits in `[0, x]`.

This check consists of two parts.

**Part A Completed Gaps**
We need to check all the gaps that are fully enclosed by obstacles within the range `[0, x]`.
Using our set, we find the largest obstacle that is less than or equal to `x`. Let us call this `max_obs`.
We perform a query on our Segment Tree for the range `[0, max_obs]`. This returns the largest gap size among all gaps that end at or before `max_obs`.

**Part B The Trailing Gap**
There is one gap that the Segment Tree might miss or not fully capture relative to `x`. That is the space between the last obstacle `max_obs` and the query boundary `x`.
Even though there is no obstacle at `x`, the space from `max_obs` to `x` is valid free space. We calculate this size as `x` minus `max_obs`.

**Final Decision**
The maximum available space is the larger of the result from Part A and the result from Part B.
If this maximum space is greater than or equal to `sz`, the answer is `true`. Otherwise, it is `false`.

## Detailed C++ Implementation

Below is the C++ code that implements this logic. I have avoided using standard library macros or confusing shorthand to ensure it is readable for everyone.

```cpp
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

// We define a maximum coordinate value based on the problem constraints.
// The problem states coordinates go up to 50,000.
// We add a small buffer to be safe.
const int MAX_COORDINATE = 50005;

class SegmentTree {
    // The tree array stores the maximum value for a given range.
    vector<int> tree;
    int n;

public:
    // Constructor initializes the tree with zeros.
    SegmentTree(int size) {
        n = size;
        tree.assign(4 * n, 0);
    }

    // Helper function to update a value at a specific position.
    // node is the current index in the tree array.
    // start and end define the range covered by this node.
    // idx is the target coordinate we want to update.
    // val is the new value to store.
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        
        int mid = start + (end - start) / 2;
        
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        
        // After updating the child, recalculate the max for the current node.
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    // Helper function to query the maximum value in a range [l, r].
    int query(int node, int start, int end, int l, int r) {
        // If the current range is completely outside the query range, return 0.
        if (r < start || end < l) {
            return 0;
        }
        
        // If the current range is completely inside the query range, return the stored value.
        if (l <= start && end <= r) {
            return tree[node];
        }
        
        int mid = start + (end - start) / 2;
        
        // Query both children and take the maximum.
        int left_max = query(2 * node, start, mid, l, r);
        int right_max = query(2 * node + 1, mid + 1, end, l, r);
        
        return max(left_max, right_max);
    }

    // Public wrapper for update
    void updateVal(int idx, int val) {
        update(1, 0, n, idx, val);
    }

    // Public wrapper for query
    int queryMax(int l, int r) {
        return query(1, 0, n, l, r);
    }
};

class Solution {
public:
    vector<bool> getResults(vector<vector<int>>& queries) {
        // This set will store the positions of all obstacles.
        // It allows us to quickly find the previous and next obstacles.
        set<int> obstacles;
        
        // Initialize with 0 as the starting boundary.
        obstacles.insert(0);
        
        // Create the Segment Tree.
        SegmentTree st(MAX_COORDINATE);
        
        vector<bool> results;

        for (const auto& q : queries) {
            int type = q[0];
            int x = q[1];

            if (type == 1) {
                // OPERATION Build an obstacle at x
                
                // Find the existing obstacle just before or at x.
                // We use upper_bound to find the first element strictly greater than x,
                // then move the iterator back one step.
                auto it = obstacles.upper_bound(x);
                
                // Get the obstacle immediately to the left of x.
                int prev_obs = *prev(it);
                
                // Check if there is an obstacle to the right.
                if (it != obstacles.end()) {
                    int next_obs = *it;
                    // The existing gap was between prev_obs and next_obs.
                    // This gap is now split. The gap ending at next_obs is now smaller.
                    // Its new size is (next_obs minus x).
                    st.updateVal(next_obs, next_obs - x);
                }

                // Now we register the new gap created by x itself.
                // This gap ends at x and starts at prev_obs.
                // Size is (x minus prev_obs).
                st.updateVal(x, x - prev_obs);
                
                // Finally, add x to our set of obstacles.
                obstacles.insert(x);

            } else {
                // OPERATION Check if a block of size sz fits in [0, x]
                int sz = q[2];
                
                // We need to find the obstacle closest to x (but <= x)
                auto it = obstacles.upper_bound(x);
                int max_obs = *prev(it);
                
                // Check 1 Query the Segment Tree for the max gap size among
                // all gaps that end at or before max_obs.
                int max_internal_gap = st.queryMax(0, max_obs);
                
                // Check 2 Calculate the gap between the last obstacle and x.
                // This is the "trailing" empty space.
                int trailing_gap = x - max_obs;
                
                // The available space is the maximum of these two values.
                if (max(max_internal_gap, trailing_gap) >= sz) {
                    results.push_back(true);
                } else {
                    results.push_back(false);
                }
            }
        }
        
        return results;
    }
};

```

## Why This Works Efficiently

Let us evaluate the performance of our solution to ensure it meets the requirements.

### Time Complexity

1. **Preprocessing**
Building the initial empty Segment Tree takes linear time relative to the coordinate range, which is roughly O(M) where M is 50,000. This is negligible.
2. **Type 1 Query (Update)**
* Set operations `insert` and `upper_bound` take O(log Q) time, where Q is the number of obstacles.
* Segment Tree updates take O(log M) time.
* Total per query O(log Q + log M).


3. **Type 2 Query (Query)**
* Set operations take O(log Q).
* Segment Tree query takes O(log M).
* Total per query O(log Q + log M).



Given that both Q and M are relatively small (in the tens of thousands), this solution runs extremely fast. It avoids the heavy O(N) scan of the naive approach.

### Space Complexity

We use a Segment Tree array of size 4 x 50,000, which is roughly 200,000 integers. This consumes less than 1 MB of memory. The set stores up to 50,000 integers. This is extremely memory efficient and well within standard limits.

## Common Pitfalls and Edge Cases

When implementing this, keep an eye out for these tricky situations.

* **Coordinate Zero**
Always remember that the number line starts at 0. Treating 0 as an initial obstacle simplifies the logic because you never have to check for "undefined" left boundaries.
* **The Rightmost Gap**
It is easy to forget the space between the last obstacle and the query coordinate `x`. The Segment Tree only knows about intervals that *end* at an obstacle. The space after the last obstacle is virtual until an obstacle is placed there. Do not forget to calculate `x` minus `max_obs`.
* **Strict Inequality**
The problem allows the block to *touch* obstacles. So, a gap of size exactly `sz` is sufficient. Ensure your comparison is `>= sz` and not `> sz`.

## Conclusion

This problem is a classic example of how changing your perspective can simplify a solution. By viewing the number line not as a collection of boolean points but as a collection of dynamic intervals, we transformed a slow linear scan into a snappy logarithmic lookup.

The combination of a **Set** for order and connectivity and a **Segment Tree** for aggregate statistics is a pattern that appears frequently in competitive programming. Mastering this pattern gives you a powerful template for solving many other hard problems involving dynamic ranges.

I hope this walkthrough clarifies the logic behind Block Placement Queries! Feel free to adapt this code and experiment with different inputs to see the logic in action. Happy coding!
