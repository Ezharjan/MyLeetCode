# Cracking the Code of Dynamic Updates and Non Adjacent Sums [#3165]

[Blog Post Link](https://leetcode.com/discuss/post/7558918/cracking-the-code-of-dynamic-updates-and-irsl/)

Welcome fellow coders! Today we are going to explore a fascinating challenge that combines dynamic programming concepts with efficient data structures. If you have ever struggled with problems requiring frequent updates to an array while maintaining complex queries, this post is for you. We will break down a hard level problem into bite sized pieces, ensuring that by the end, you not only understand the solution but also the underlying logic that makes it work.

### Understanding the Challenge

Let us look at the problem statement. We are working with an array of integers called `nums`. We are also given a series of `queries`. Each query asks us to perform two steps:

1. Update a specific value in the `nums` array.
2. Calculate the **maximum sum of a subsequence** where **no two selected elements are adjacent**.

Finally, we need to return the sum of the answers to all these queries, modulo 1,000,000,007.

**What is a Subsequence?**
A subsequence is derived by deleting some or no elements from the array without changing the order of the remaining elements. The key constraint here is that if you pick an element at index `i`, you cannot pick the elements at `i - 1` or `i + 1`.

**Example Walkthrough**
Imagine `nums = [3, 5, 9]`.

* We can pick `3` and `9` (sum = 12).
* We can pick just `5` (sum = 5).
* We cannot pick `3` and `5` because they are side by side.
* The maximum here is 12.

Now, imagine a query comes in: `[1, -2]`. This means "change the value at index 1 to -2".
Our array becomes `[3, -2, 9]`.

* We can pick `3` and `9` (sum = 12).
* We can pick `-2` (but since we want the maximum sum, we can choose to pick nothing from that spot if it reduces our total, or simply treat the empty subsequence as 0 sum if all numbers are negative).
* The answer remains 12.

This sounds like the classic "House Robber" problem, but with a twist: the values in the houses keep changing!

### The Naive Approach and Why We Need More

If the array were static (never changing), we could solve this using standard Dynamic Programming (DP) in linear time, or O(N).
We would define `dp[i]` as the max sum ending at or before index `i`.
The transition looks like this:
`dp[i] = max(dp[i - 1], nums[i] + dp[i - 2])`

However, we have `Q` queries. If we recompute the DP table from scratch for every single query, our total time complexity becomes **O(N * Q)**.
Given the constraints where N and Q can both be up to 50,000, calculating 50,000 * 50,000 operations results in 2.5 billion operations. This will definitely result in a Time Limit Exceeded (TLE) error on most platforms.

We need a way to update the array and query the answer much faster. Ideally, we want the update and query to happen in **logarithmic time**, or O(log N). This is the perfect use case for a **Segment Tree**.

### The Segment Tree Strategy

A Segment Tree is a binary tree used for storing intervals or segments. It allows querying which of the segments contain a given point and processing the data in those segments efficiently.

In a standard Segment Tree (like for a range sum query), each node stores the sum of values in its range. When we update a value, we only update the path from the leaf to the root, which takes O(log N).

But our problem is trickier. We cannot just store a single "max sum" value in each node. Why? Because simply knowing the max sum of a left child and a right child is not enough to combine them. We need to know **boundary conditions**.

**The Boundary Problem**
Imagine we split our array into two halves: Left and Right.

* Left Half: `[3, 5]`
* Right Half: `[9]`
If we pick `5` from the Left Half, we **cannot** pick `9` from the Right Half because they are adjacent in the original array `[3, 5, 9]`.
However, if we did not pick `5`, we **could** pick `9`.

To correctly merge two segments, we need to know if the **boundary elements** (the last element of the left segment and the first element of the right segment) were selected.

### Designing the Node Structure

To handle the adjacency constraint across the split, each node in our Segment Tree will store **four** values instead of just one. These four values represent the maximum non adjacent sum for that range, but with specific constraints on the **first** (leftmost) and **last** (rightmost) elements of that range.

Let us denote the range covered by a node as `[L ... R]`. We will maintain four variables:

1. **v00**: The max sum of the range `[L ... R]` where we **exclude** `nums[L]` AND **exclude** `nums[R]`.
2. **v01**: The max sum where we **exclude** `nums[L]` but `nums[R]` is **allowed** to be picked.
3. **v10**: The max sum where `nums[L]` is **allowed** to be picked but we **exclude** `nums[R]`.
4. **v11**: The max sum where both `nums[L]` and `nums[R]` are **allowed** to be picked.

**Clarification on "Allowed" vs "Picked"**
When we say `nums[L]` is "allowed", it does not mean we *must* pick it. It means we have the *option* to pick it. If `nums[L]` is negative, we might choose not to include it in the sum even if allowed. However, if we say "excluded", we are strictly forbidden from picking it, usually because a neighbor was picked.

### The Merging Logic

This is the most critical part of the solution. How do we compute these four values for a parent node by looking at its two children?

Let `LeftChild` cover the range `[L ... Mid]`.
Let `RightChild` cover the range `[Mid+1 ... R]`.

The boundary interaction happens between `nums[Mid]` (the end of the left child) and `nums[Mid+1]` (the start of the right child).
**Constraint:** We cannot pick both `nums[Mid]` and `nums[Mid+1]`.

Let us derive the formula for the parent's **v11** (Allow L, Allow R).
To form `v11` for the parent `[L ... R]`, we need to combine a path through `LeftChild` and `RightChild`.

There are two main scenarios for the split point `Mid` and `Mid+1`:

* **Scenario A: We do NOT pick `nums[Mid]` from the Left.**
If we don't pick `nums[Mid]`, then `nums[Mid+1]` in the Right child is free to be picked (allowed).
* From `LeftChild`, we need a state that allows `L` but excludes `Mid`. This is `LeftChild.v10`.
* From `RightChild`, we need a state that allows `Mid+1` and allows `R`. This is `RightChild.v11`.
* Combined Sum: `LeftChild.v10 + RightChild.v11`.


* **Scenario B: We do NOT pick `nums[Mid+1]` from the Right.**
If we don't pick `nums[Mid+1]`, then `nums[Mid]` in the Left child was free to be picked (allowed).
* From `LeftChild`, we need a state that allows `L` and allows `Mid`. This is `LeftChild.v11`.
* From `RightChild`, we need a state that excludes `Mid+1` and allows `R`. This is `RightChild.v01`.
* Combined Sum: `LeftChild.v11 + RightChild.v01`.



The parent's `v11` will be the maximum of these two scenarios.
`Parent.v11 = max(LeftChild.v10 + RightChild.v11, LeftChild.v11 + RightChild.v01)`

We apply this same logic to derive all four variables.

**Deriving Parent.v00 (Exclude L, Exclude R)**

* Option 1 (Exclude `Mid`): `LeftChild` excludes `L` and excludes `Mid` (`v00`) + `RightChild` allows `Mid+1` but excludes `R` (`v10`).
* Option 2 (Exclude `Mid+1`): `LeftChild` excludes `L` and allows `Mid` (`v01`) + `RightChild` excludes `Mid+1` and excludes `R` (`v00`).
* `Parent.v00 = max(LeftChild.v00 + RightChild.v10, LeftChild.v01 + RightChild.v00)`

**Deriving Parent.v01 (Exclude L, Allow R)**

* Option 1 (Exclude `Mid`): `LeftChild` excludes `L` and excludes `Mid` (`v00`) + `RightChild` allows `Mid+1` and allows `R` (`v11`).
* Option 2 (Exclude `Mid+1`): `LeftChild` excludes `L` and allows `Mid` (`v01`) + `RightChild` excludes `Mid+1` and allows `R` (`v01`).
* `Parent.v01 = max(LeftChild.v00 + RightChild.v11, LeftChild.v01 + RightChild.v01)`

**Deriving Parent.v10 (Allow L, Exclude R)**

* Option 1 (Exclude `Mid`): `LeftChild` allows `L` and excludes `Mid` (`v10`) + `RightChild` allows `Mid+1` and excludes `R` (`v10`).
* Option 2 (Exclude `Mid+1`): `LeftChild` allows `L` and allows `Mid` (`v11`) + `RightChild` excludes `Mid+1` and excludes `R` (`v00`).
* `Parent.v10 = max(LeftChild.v10 + RightChild.v10, LeftChild.v11 + RightChild.v00)`

### The Base Case (Leaf Node)

When we reach a leaf node, the range is just a single element `[i ... i]`.
Here, `L` is the same as `R`.

* **v11 (Allow L, Allow R):** Since `L=R`, we are allowed to pick `nums[i]`. We pick it if it is positive. If `nums[i]` is negative, the max sum is 0 (empty subsequence).
`Value = max(0, nums[i])`
* **v00 (Exclude L, Exclude R):** We are forced to not pick `nums[i]`. Sum is 0.
* **v01 (Exclude L, Allow R):** Since `L=R`, if we exclude L, we exclude the only element. Sum is 0.
* **v10 (Allow L, Exclude R):** Same logic. If we exclude R, we exclude the only element. Sum is 0.

So for a leaf node:
`v11 = max(0, nums[i])`
`v00 = v01 = v10 = 0`

### Step by Step Implementation Guide

Now that we have the theory, let us structure our C++ solution.

**1. The Node Struct**
We will create a simple `struct` or `class` to hold our four long long integers. Using `long long` is important because the sum can exceed the range of a standard 32 bit integer.

**2. Building the Tree**
We need a `build` function. This function initializes the tree. It recursively splits the range until it hits a leaf, sets the base case values, and then merges the results back up as the recursion unwinds.

**3. The Update Function**
This is very similar to `build`. We travel down the tree to find the leaf node corresponding to the index we want to update. We change its `v11` value based on the new input `x`. Then, as we return from the recursion, we re-merge the paths that were affected. This ensures the root of the tree always holds the correct answer for the full array.

**4. Handling the Queries**
We loop through the `queries` array. For each query `[pos, x]`:

* Call `update(1, 0, n-1, pos, x)`.
* The answer for the current state of the array is stored in the root of the tree. specifically, we want `root.v11` because for the full array, we are allowed to pick the first element and allowed to pick the last element (though we are not forced to).
* Add this result to our running total.
* Apply the modulo operator.

### The C++ Solution

Here is the complete code. I have added comments to explain every part of the process.

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
private:
    // Define the structure for our Segment Tree Node
    struct Node {
        // v00: Max sum excluding both left and right boundaries
        // v01: Max sum excluding left, allowing right boundary
        // v10: Max sum allowing left, excluding right boundary
        // v11: Max sum allowing both left and right boundaries
        long long v00, v01, v10, v11;
    };

    // The tree array
    vector<Node> tree;
    int n; // Size of the input array

    // Function to merge two nodes (Left Child and Right Child)
    // This implements the logic discussed in the "Merging Logic" section
    Node merge(const Node& l, const Node& r) {
        Node res;
        
        // Calculate v00 for the parent
        // We can either:
        // 1. Exclude Mid (l.v00) -> which allows us to pick or skip Mid+1, 
        //    but since we need R excluded, we take r.v10.
        // 2. Allow Mid (l.v01) -> which forces us to Exclude Mid+1,
        //    and we still need R excluded (r.v00).
        res.v00 = max(l.v00 + r.v10, l.v01 + r.v00);

        // Calculate v01 for the parent (Exclude L, Allow R)
        res.v01 = max(l.v00 + r.v11, l.v01 + r.v01);

        // Calculate v10 for the parent (Allow L, Exclude R)
        res.v10 = max(l.v10 + r.v10, l.v11 + r.v00);

        // Calculate v11 for the parent (Allow L, Allow R)
        res.v11 = max(l.v10 + r.v11, l.v11 + r.v01);

        return res;
    }

    // Recursive function to build the tree initially
    void build(const vector<int>& nums, int node, int start, int end) {
        if (start == end) {
            // Leaf Node Case
            // If we pick the element, the value is nums[start] (or 0 if negative)
            long long val = max(0, nums[start]);
            
            // v11 is the only state where we actually pick the element
            // In all other states (v00, v01, v10), one or both boundaries 
            // of this single-element range are excluded. Since Start==End,
            // excluding a boundary means excluding the element itself.
            tree[node] = {0, 0, 0, val};
            return;
        }
        
        int mid = start + (end - start) / 2;
        build(nums, 2 * node, start, mid);       // Build left child
        build(nums, 2 * node + 1, mid + 1, end); // Build right child
        
        // Combine the results
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    // Recursive function to update a specific index
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            // Re-evaluate the leaf node with the new value
            long long v = max(0, val);
            tree[node] = {0, 0, 0, v};
            return;
        }
        
        int mid = start + (end - start) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        
        // Merge the updated children
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

public:
    int maximumSumSubsequence(vector<int>& nums, vector<vector<int>>& queries) {
        n = nums.size();
        tree.resize(4 * n); // Segment tree size is roughly 4*N
        
        // Step 1: Build the initial tree
        build(nums, 1, 0, n - 1);

        long long totalSum = 0;
        int MOD = 1e9 + 7;

        // Step 2: Process each query
        for (const auto& q : queries) {
            int pos = q[0];
            int x = q[1];
            
            // Update the Segment Tree
            update(1, 0, n - 1, pos, x);
            
            // The answer is always at the root (node 1).
            // We want v11 because for the whole array, we have the option
            // to pick the first and last elements.
            long long currentMax = tree[1].v11;
            
            // Add to total sum and modulo
            totalSum = (totalSum + currentMax) % MOD;
        }

        return totalSum;
    }
};

```

### Complexity Analysis

It is important to understand why this solution is efficient enough to pass the test cases.

* **Time Complexity:**
* **Build:** The `build` function visits every node in the tree once. A Segment Tree has roughly `2 * N` nodes. So building takes **O(N)**.
* **Update:** For each query, we traverse from the root to a leaf. The height of the tree is `log N`. At each level, we perform constant time operations (merging 4 values). So each update takes **O(log N)**.
* **Total:** With `Q` queries, the total time complexity is **O(N + Q log N)**. Given `N = 50,000` and `Q = 50,000`, this is well within the limits.


* **Space Complexity:**
* We use a vector `tree` of size `4 * N` to store the nodes. This results in **O(N)** space complexity.



### Conclusion

And there you have it! We have successfully tackled a Hard problem by transforming a linear Dynamic Programming approach into a log time Segment Tree solution.

The key takeaway here is the **state representation**. When standard Divide and Conquer does not work immediately because of dependencies between sub problems (like the adjacency constraint here), try to expand the state information you return from each sub problem. By explicitly tracking the status of the boundary elements, we were able to stitch the sub problems together perfectly.

I hope this explanation was helpful and sheds light on how powerful Segment Trees can be for dynamic optimization problems. 

Have a great weekend everyone!
