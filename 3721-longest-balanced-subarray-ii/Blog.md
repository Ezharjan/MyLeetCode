# Cracking the Hardest Subarray Problems with Segment Trees and Sweeping Lines [#3721]

[Blog Post Link](https://leetcode.com/discuss/post/7544235/cracking-the-hardest-subarray-problems-w-v0az/)

Welcome to a deep dive into one of the most fascinating algorithmic challenges involving arrays. If you have ever felt intimidated by problems labeled "Hard" that involve subarrays, distinct elements, and parity constraints, you are in the right place. We are going to deconstruct a complex problem involving the "Longest Balanced Subarray" using a powerful combination of the Sweep Line technique and Segment Trees.

By the end of this guide, you will not only understand how to solve this specific challenge but also master a generalizable framework for handling "distinct element" queries in contiguous arrays.

## The Challenge at Hand

Let us begin by clearly defining the problem we intend to solve. We are dealing with an array of integers. Our goal is to identify a specific type of contiguous part within this array, known as a subarray.

A subarray is considered **balanced** if it satisfies a strict condition:

> The count of **distinct even numbers** must equal the count of **distinct odd numbers**.

Our objective is to return the length of the **longest** balanced subarray possible.

### Illustrative Examples

To ensure we are on the same page, let us look at a few scenarios.

**Scenario A**
Input: `[2, 5, 4, 3]`

* Look at the whole array `[2, 5, 4, 3]`.
* Distinct Evens: `2, 4` (Count is 2)
* Distinct Odds: `5, 3` (Count is 2)
* The counts match. The length is 4.

**Scenario B**
Input: `[1, 2, 3, 2]`

* Consider the subarray `[2, 3, 2]`.
* Distinct Evens: `2` (Count is 1). Note that we ignore the duplicate `2`.
* Distinct Odds: `3` (Count is 1).
* The counts match. The length is 3.

This "distinct" constraint is what makes the problem significantly harder than standard subarray problems.

## The Mathematical Transformation

The first step in solving any advanced algorithmic problem is often transforming the condition into a mathematical equation.

We are looking for a subarray where:


We can rewrite this equation as:


Let us define a value called the **Balance Score**. For any given subarray, the Balance Score is the number of distinct even integers minus the number of distinct odd integers. We are strictly looking for the longest subarray where this Balance Score equals **zero**.

## The Naive Approach and Why It Fails

Your initial instinct might be to check every possible subarray. Let us see why this is not feasible.

An array of size  has roughly  subarrays. If  is up to  (or ), the number of subarrays is approximately .

For each subarray, you would need to iterate through it to count distinct elements. Even with a hash set to speed up counting, this approach leads to a time complexity of roughly  or . With , this will exceed the time limit of any standard coding environment immediately. We need a solution that operates closer to .

## The Strategic Breakthrough: Fixing the Right Endpoint

To optimize this, we use a standard technique in subarray problems: **Iterate through the right endpoint.**

Let us iterate through the array with an index  representing the **end** of our subarray. As we move  from  to , we want to answer the following question efficiently:

> For the current end index , what is the earliest start index  such that the subarray `nums[L...i]` has a Balance Score of 0?

If we can find this optimal  quickly, the length of the candidate subarray is simply . We maximize this value over all steps.

### The Challenge of "Distinct" Updates

This is where the problem gets tricky. When we move our right pointer from  to  and include a new number `x`, how does the Balance Score change for all the various subarrays ending at ?

Consider the array: `[2, 1, 2]`
Let us analyze what happens when we reach the second `2` at index 2.

1. **Subarray `[2]` (index 2 to 2):** This new `2` is distinct.
2. **Subarray `[1, 2]` (index 1 to 2):** This new `2` is distinct.
3. **Subarray `[2, 1, 2]` (index 0 to 2):** The new `2` is **not** distinct because a `2` already exists at index 0.

This observation leads to a crucial rule:

> A number `nums[i]` only contributes to the distinct count for subarrays that start **after** the previous occurrence of `nums[i]`.

Let `prev` be the index where `nums[i]` last appeared. If `nums[i]` has never appeared, `prev` is effectively `-1`.
When we encounter `nums[i]`, it changes the Balance Score for all subarrays starting at index  where:


* If `nums[i]` is **Even**, the Balance Score increases by 1 for all starts  in the range `(prev, i]`.
* If `nums[i]` is **Odd**, the Balance Score decreases by 1 for all starts  in the range `(prev, i]`.

## The Data Structure: A Segment Tree

We now have our requirements for a data structure. We need to maintain a list of Balance Scores for every possible start index . As we iterate  through the array, we need to:

1. **Range Add:** Add  or  to a range of start indices.
2. **Point Activate:** When we reach index , the start index  becomes valid (a subarray of length 1). We initialize its score.
3. **Global Search:** Find the smallest index  in the structure that currently holds a value of 0.

This is a textbook use case for a **Segment Tree**.

A Segment Tree is a binary tree structure that allows us to perform range updates and range queries in logarithmic time, specifically .

### Designing the Segment Tree Node

Each node in our Segment Tree will represent a range of **start indices**. For the root, this range is .

To efficiently find if a "0" exists in a range, each node in our tree will store two values:

1. **Min:** The minimum Balance Score in this range of start indices.
2. **Max:** The maximum Balance Score in this range of start indices.

Why both?
We are looking for the value 0.

* If `Min > 0`, then all values in this range are positive. 0 cannot exist here.
* If `Max < 0`, then all values in this range are negative. 0 cannot exist here.
* If `Min <= 0 <= Max`, then it is **possible** that a 0 exists in this range. We must search deeper.

We also need a **Lazy** value for each node to handle the "Range Add" operations efficiently without updating every single leaf node immediately. This is known as Lazy Propagation.

## The Algorithm Step by Step

Let us visualize the algorithm in action.

**Initialization:**
We build a Segment Tree covering indices  to . Initially, all start indices are "inactive." We simulate this by setting their values to Infinity. This ensures they are not selected as valid 0 locations.

We also need a map or array called `last_pos` to track the last index where each number appeared. Initialize this with `-1`.

**The Loop:**
Iterate  from  to :

1. **Activate Start Index :**
Before processing `nums[i]`, the subarray starting at  and ending at  represents a new potential window. Actually, it is easier to think of the start index  as initially representing an empty subarray just before we add the element. We set the value at index  in our Segment Tree to 0.
2. **Identify Contribution:**
Let `val = nums[i]`.
Check `last_pos[val]` to find `prev`.
Determine the contribution `diff`. If `val` is even, `diff = 1`. If `val` is odd, `diff = -1`.
3. **Range Update:**
We apply the update `diff` to the range of start indices .
This effectively tells the Segment Tree: "For any subarray starting in this range, the number at  is a new distinct even or odd number, so adjust the balance."
4. **Update Tracker:**
Update `last_pos[val] = i`.
5. **Query for Longest Subarray:**
Ask the Segment Tree: "What is the smallest index  such that the value at  is 0?"
If such an  exists, the length is . Update the global maximum length.

## Detailed Walkthrough

Let us trace `nums = [2, 5, 4, 3]`.
`last_pos` is empty. `tree` values are effectively infinite. `max_len = 0`.

**Step 0: i = 0, num = 2 (Even)**

* **Activate:** Set `tree[0] = 0`.
* **Prev:** `last_pos[2]` is -1.
* **Update:** Range `( -1 + 1, 0 ]` which is `[0, 0]`. Add +1 (Even).
* `tree[0]` becomes 1.


* **Tracker:** `last_pos[2] = 0`.
* **Query:** Is there a 0? No. `tree[0]` is 1.

**Step 1: i = 1, num = 5 (Odd)**

* **Activate:** Set `tree[1] = 0`.
* **Prev:** `last_pos[5]` is -1.
* **Update:** Range `[0, 1]`. Add -1 (Odd).
* `tree[0]` becomes .
* `tree[1]` becomes .


* **Tracker:** `last_pos[5] = 1`.
* **Query:** Is there a 0? Yes. The leftmost 0 is at index 0.
* Current subarray `nums[0...1]` is `[2, 5]`. Valid.
* Length: . `max_len = 2`.



**Step 2: i = 2, num = 4 (Even)**

* **Activate:** Set `tree[2] = 0`.
* **Prev:** `last_pos[4]` is -1.
* **Update:** Range `[0, 2]`. Add +1.
* `tree[0]` becomes .
* `tree[1]` becomes .
* `tree[2]` becomes .


* **Tracker:** `last_pos[4] = 2`.
* **Query:** Is there a 0? Yes. The leftmost 0 is at index 1.
* Current subarray `nums[1...2]` is `[5, 4]`. Valid.
* Length: . `max_len` remains 2.



**Step 3: i = 3, num = 3 (Odd)**

* **Activate:** Set `tree[3] = 0`.
* **Prev:** `last_pos[3]` is -1.
* **Update:** Range `[0, 3]`. Add -1.
* `tree[0]` becomes .
* `tree[1]` becomes .
* `tree[2]` becomes .
* `tree[3]` becomes .


* **Tracker:** `last_pos[3] = 3`.
* **Query:** Is there a 0? Yes. The leftmost 0 is at index 0.
* Current subarray `nums[0...3]` is `[2, 5, 4, 3]`.
* Length: . `max_len = 4`.



Final Answer: 4.

## Implementation Details

We will implement this in C++. We use a fixed-size array for the segment tree to avoid pointer overhead, as the constraints () fit comfortably in memory.

### Handling Lazy Propagation

Lazy propagation is vital here. Without it, updating a range like `[0, i]` would take  time, making our total time . With lazy propagation, we store pending updates in a `lazy` array. When we visit a node, we "push" this lazy value down to its children before processing the node.

### Finding the First Zero

The function `queryFirstZero` is a specialized search.

1. Check the current node. If `min > 0` or `max < 0`, return -1 immediately. The target 0 cannot exist here.
2. If this is a leaf node, return its index.
3. Push lazy tags.
4. Recursively check the **Left Child**. If it returns a valid index (not -1), that is our answer.
5. If the Left Child fails, recursively check the **Right Child**.

This logic ensures we always find the **smallest** (leftmost) index, maximizing the subarray length.

## The Full Code Solution

Below is the complete C++ implementation. It uses `std::vector` for memory management and clear class based organization.

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
    int n;
    // Segment Tree arrays for storing Min and Max of the range
    vector<int> tree_min, tree_max, lazy;
    
    // Constants to represent inactive nodes. 
    // INF ensures min > 0, NEG_INF ensures max < 0
    // This effectively hides inactive nodes from our query for 0.
    const int INF = 1e9;
    const int NEG_INF = -1e9;

    // Propagate lazy values down to children
    void push(int node) {
        if (lazy[node] != 0) {
            // Apply lazy value to left child
            tree_min[2 * node] += lazy[node];
            tree_max[2 * node] += lazy[node];
            lazy[2 * node] += lazy[node];
            
            // Apply lazy value to right child
            tree_min[2 * node + 1] += lazy[node];
            tree_max[2 * node + 1] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
            
            // Reset current node's lazy value
            lazy[node] = 0;
        }
    }

    // Range update: add val to indices [l, r]
    void update(int node, int start, int end, int l, int r, int val) {
        if (l > end || r < start) return; // Out of range
        
        if (l <= start && end <= r) {
            // Totally within range
            tree_min[node] += val;
            tree_max[node] += val;
            lazy[node] += val;
            return;
        }
        
        push(node); // Push pending updates before processing children
        int mid = (start + end) / 2;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        
        // Recalculate current node values
        tree_min[node] = min(tree_min[2 * node], tree_min[2 * node + 1]);
        tree_max[node] = max(tree_max[2 * node], tree_max[2 * node + 1]);
    }

    // Point update: Activate index idx by setting it to 0
    void set_zero(int node, int start, int end, int idx) {
        if (start == end) {
            tree_min[node] = 0;
            tree_max[node] = 0;
            lazy[node] = 0;
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        if (idx <= mid) set_zero(2 * node, start, mid, idx);
        else set_zero(2 * node + 1, mid + 1, end, idx);
        
        tree_min[node] = min(tree_min[2 * node], tree_min[2 * node + 1]);
        tree_max[node] = max(tree_max[2 * node], tree_max[2 * node + 1]);
    }

    // Query: Find the smallest index with value 0
    int queryFirstZero(int node, int start, int end) {
        // Pruning: If 0 is not in the range [min, max], stop.
        if (tree_min[node] > 0 || tree_max[node] < 0) return -1;
        
        if (start == end) return start; // Found a leaf
        
        push(node);
        int mid = (start + end) / 2;
        
        // Priority to left child to find the SMALLEST index
        int res = queryFirstZero(2 * node, start, mid);
        if (res != -1) return res;
        
        // If not found in left, try right
        return queryFirstZero(2 * node + 1, mid + 1, end);
    }
    
    // Initialize tree with inactive values
    void build(int node, int start, int end) {
        if (start == end) {
            tree_min[node] = INF;
            tree_max[node] = NEG_INF;
            lazy[node] = 0;
            return;
        }
        int mid = (start + end) / 2;
        build(2 * node, start, mid);
        build(2 * node + 1, mid + 1, end);
        tree_min[node] = min(tree_min[2 * node], tree_min[2 * node + 1]);
        tree_max[node] = max(tree_max[2 * node], tree_max[2 * node + 1]);
    }

public:
    int longestBalanced(vector<int>& nums) {
        n = nums.size();
        // Allocate memory (4x size is standard for Segment Trees)
        tree_min.assign(4 * n, 0);
        tree_max.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
        
        build(1, 0, n - 1);
        
        // Use a vector or hash map to store last positions. 
        // Since values are <= 10^5, a direct access table is faster.
        vector<int> last_pos(100001, -1);
        int max_len = 0;
        
        for (int i = 0; i < n; ++i) {
            // 1. Activate the current position as a valid start index
            set_zero(1, 0, n - 1, i);
            
            // 2. Identify value and parity
            int val = nums[i];
            int prev = last_pos[val];
            int diff = (val % 2 == 0) ? 1 : -1; 
            
            // 3. Update range (prev + 1 to i)
            update(1, 0, n - 1, prev + 1, i, diff);
            
            // 4. Update last seen position
            last_pos[val] = i;
            
            // 5. Query for the longest valid subarray ending at i
            int l = queryFirstZero(1, 0, n - 1);
            if (l != -1) {
                max_len = max(max_len, i - l + 1);
            }
        }
        
        return max_len;
    }
};

```

## Complexity Analysis

It is crucial to understand the efficiency of this solution.

**Time Complexity**
We perform one main loop that iterates  times. Inside this loop, we perform:

* `set_zero`: Point update on the Segment Tree. Cost is .
* `update`: Range add on the Segment Tree. Cost is .
* `queryFirstZero`: Tree traversal. In the worst case, we might visit nodes down to depth , but the pruning (checking min/max) keeps this efficient on average. The strict bound is .

Thus, the total time complexity is . Given , this performs roughly  operations, which is well within the typical limit of  operations per second allowed by judges.

**Space Complexity**
We use vectors for the Segment Tree of size  and a `last_pos` array of size . This results in a linear space complexity of .

## Summary

This problem is a beautiful example of how changing your perspective can simplify a task. By transforming the condition "Equal Distinct Counts" into a "Zero Balance" problem and utilizing a Segment Tree to manage the history of all start indices, we turned an impossible  task into an efficient  solution.

The key takeaways for your future algorithmic endeavors are:

1. **Transform Equalities:** Convert "A = B" into "A - B = 0".
2. **Handle Distinctness with Prevs:** Use the previous occurrence index to define the valid update range.
3. **Segment Trees for History:** Use Segment Trees not just for static arrays, but to maintain the state of all possible "start points" as you sweep through the data.

<br>

What do you think about this solution?

Share your insights in the comments below!

Happy Coding!
