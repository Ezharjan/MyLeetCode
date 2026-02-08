# Mastering Dynamic Range Queries and Prime Splits with Segment Trees [#3569]

[Blod Post Link](https://leetcode.com/discuss/post/7563634/mastering-dynamic-range-queries-and-prim-xpad/)


Hello to the coding community! Today we are going to dive deep into a fascinating algorithmic challenge that combines number theory with advanced data structures. If you enjoy optimizing array operations and managing dynamic updates, this problem is a perfect case study for you. We will explore how to handle range updates efficiently to solve the "Maximize Count of Distinct Primes After Split" problem.

We will break this down step by step to ensure everyone can follow along. We will start with the logic, move to the data structures, and finally implement the solution in C++.

## Understanding the Problem

Let us first clarify what the problem asks us to do. We are given an array of integers called `nums`. We are also given a series of `queries`.

The process involves two main actions for every query:

1. **Update:** We change the value at a specific index in the array to a new value.
2. **Split and Evaluate:** After the update, we need to find a split point `k`. A split point divides the array into a left part (prefix) and a right part (suffix). We need to choose `k` such that the "total score" is maximized.

**What is the score?**
The score is the sum of two counts:

* The count of **distinct** prime numbers in the left part.
* The count of **distinct** prime numbers in the right part.

We need to return the maximum possible score for each query.

### A Concrete Example

Imagine we have the array `nums` equal to `[2, 1, 3, 1, 2]`.

Let us look at the prime numbers here. The primes are 2 and 3. The number 1 is not prime.
The indices of the primes are:

* Prime 2 is at indices 0 and 4.
* Prime 3 is at index 2.

Now consider a split at index `k = 3`.
The left part is `nums[0]` through `nums[2]`, which is `[2, 1, 3]`.
The right part is `nums[3]` through `nums[4]`, which is `[1, 2]`.

* **Left Part Primes:** Contains 2 and 3. That is 2 distinct primes.
* **Right Part Primes:** Contains 2. That is 1 distinct prime.
* **Total Score:** 2 + 1 = 3.

Our goal is to find the `k` that makes this total score as large as possible after every single update.

## The Core Intuition

A naive solution would be to recompute the score for every possible split point after every update. However, with the array length `n` and the number of queries both reaching up to 50,000, an O(N squared) approach will be too slow. We need something much faster.

Let us analyze how a single prime number contributes to the score.

Suppose we have a prime number `p` that appears multiple times in the array. Let `first` be the index of its very first appearance, and `last` be the index of its very last appearance.

When we split the array at index `k`, we create a left part `[0, k minus 1]` and a right part `[k, n minus 1]`.

There are three scenarios for our prime `p`:

1. **The split is before the prime appears (`k` is less than or equal to `first`)**
The prime `p` is entirely in the right part.
Contribution to score: +1 (only to the suffix count).
2. **The split is after the prime finishes (`k` is greater than `last`)**
The prime `p` is entirely in the left part.
Contribution to score: +1 (only to the prefix count).
3. **The split is strictly between the first and last appearance (`first` is less than `k` which is less than or equal to `last`)**
The prime `p` has at least one instance in the left part (at `first`) and at least one instance in the right part (at `last`).
Contribution to score: +2 (+1 for prefix, +1 for suffix).

### Simplifying the Logic

Notice that in all three cases, the prime `p` contributes at least +1 to the total score for any valid split, as long as `p` exists somewhere in the array.

The "bonus" point comes only in Case 3. The prime `p` contributes an **extra** +1 if the split `k` falls inside the interval `(first, last]`.

So, the problem transforms into this:

1. Every distinct prime present in the array gives a **base score** of 1 to all split points.
2. Every distinct prime gives a **bonus score** of 1 to the split points `k` where `first < k <= last`.

We need to maintain these scores for all possible `k` (from 1 to `n` minus 1) and query the maximum value.

## Data Structure Strategy

To handle range updates (adding +1 to a range of split points) and range maximum queries efficiently, we need a specific data structure.

### 1. The Segment Tree

A Segment Tree is the ideal candidate here. It allows us to:

* Add a value to a range of indices in O(log N) time.
* Query the maximum value in the entire range in O(1) time (since we always query the root).

We will build a Segment Tree where the leaves represent the possible split points `k`. The range of valid split points is from 1 to `n` minus 1.

### 2. Managing Prime Positions

To determine the range `(first, last]`, we need to know the first and last indices of every prime number quickly.
Since the values in `nums` are up to 100,000, we can use an array of sets (or similar structures) to store the indices for each number.

* `pos[p]` will store a sorted list of indices where prime `p` appears.
* `first` is `pos[p].begin()`.
* `last` is `pos[p].rbegin()`.

Using a balanced BST (like `std::set` in C++) allows us to insert and remove indices in O(log N) time while keeping them sorted.

### 3. Sieve of Eratosthenes

We need a fast way to check if a number is prime. Since the values are relatively small (up to 105), we can precompute a boolean array `isPrime` using the Sieve of Eratosthenes.

## The Algorithm in Detail

Here is the lifecycle of our solution:

### Step 1: Initialization

We start by running the Sieve to identify all primes up to the maximum possible value.
Then, we scan the initial `nums` array. For each number, if it is a prime, we add its index to our position tracker `pos`.

After populating `pos`, we populate our Segment Tree. For every prime `p` found in the array:

1. It adds +1 to the global range `[1, n minus 1]` because it exists.
2. If it appears more than once (meaning `first < last`), it adds an extra +1 to the range `[first + 1, last]`.

### Step 2: Processing Queries

For each query `[idx, val]`, we perform the following logic. Let `oldVal` be the number currently at `nums[idx]`.

**If `oldVal` is a prime:**
We must remove its contribution because its position `idx` is being removed.

1. Check the current `first` and `last` indices for `oldVal`.
2. Remove the "Base Score" (+1) from the global range `[1, n minus 1]`.
3. If `first < last`, remove the "Bonus Score" (+1) from the range `[first + 1, last]`.
4. Remove `idx` from the set `pos[oldVal]`.
5. **Re-add contribution:** If `oldVal` still exists in the array (the set is not empty), we recalculate its *new* `first` and `last` indices and add its contribution back to the Segment Tree. This ensures the Segment Tree accurately reflects the state of `oldVal` without the index `idx`.

**If `val` (the new value) is a prime:**
We must add its contribution.

1. First, if `val` already exists in the array, we remove its *current* contribution from the Segment Tree (just like above) because the `first` and `last` indices might change when we add the new index.
2. Insert `idx` into `pos[val]`.
3. Calculate the new `first` and `last` indices.
4. Add the "Base Score" (+1) to the global range.
5. If `first < last`, add the "Bonus Score" (+1) to the range `[first + 1, last]`.

**If the values are not prime:**
We simply update `nums[idx]` and ignore any Segment Tree operations for that specific value, as non prime numbers do not affect the score.

### Step 3: Getting the Answer

After updating the Segment Tree, the root node will contain the maximum value across all split points. We simply append this value to our result list.

## Implementation Details

Below is the implementation of the strategy discussed. We use a Segment Tree with Lazy Propagation to ensure our range updates are efficient.

### The Code

```cpp
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

// Maximum value for numbers in nums as per constraints
const int MAX_VAL = 100005;

// Global array to store primality
bool is_prime[MAX_VAL];
bool sieve_initialized = false;

// Function to precompute primes
void init_sieve() {
    if (sieve_initialized) return;
    fill(is_prime, is_prime + MAX_VAL, true);
    is_prime[0] = is_prime[1] = false; // 0 and 1 are not prime
    for (int p = 2; p * p < MAX_VAL; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i < MAX_VAL; i += p)
                is_prime[i] = false;
        }
    }
    sieve_initialized = true;
}

// Segment Tree Class to handle range updates and max queries
class SegmentTree {
public:
    int n;
    vector<int> tree;
    vector<int> lazy;

    SegmentTree(int n) : n(n) {
        // Size is 4 times n to accommodate the tree structure
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }

    // Helper to push lazy updates down to children
    void push(int node) {
        if (lazy[node] != 0) {
            tree[2 * node] += lazy[node];
            lazy[2 * node] += lazy[node];
            tree[2 * node + 1] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
            lazy[node] = 0;
        }
    }

    // Update function: adds val to range [l, r]
    // node covers range [start, end]
    void update(int node, int start, int end, int l, int r, int val) {
        if (l > end || r < start) return; // Out of range
        if (l <= start && end <= r) {
            // Node is fully within update range
            tree[node] += val;
            lazy[node] += val;
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        // Current node value is max of children
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    // Helper wrapper for update
    void range_add(int l, int r, int val) {
        if (l > r) return;
        update(1, 1, n, l, r, val);
    }

    // Query function returns the max value in the tree (root)
    int query_max() {
        return tree[1];
    }
};

class Solution {
    vector<set<int>> pos; // Map prime value to a set of its indices
public:
    vector<int> maximumCount(vector<int>& nums, vector<vector<int>>& queries) {
        init_sieve();
        int n = nums.size();
        
        // Initialize position sets
        pos.assign(MAX_VAL, set<int>());
        
        // Segment Tree will cover split indices from 1 to n-1
        SegmentTree st(n - 1); 

        // Lambda function to handle the complex update logic for a single prime
        // p: the prime number
        // idx: the index being added or removed
        // adding: boolean flag (true if adding idx, false if removing idx)
        auto update_contribution = [&](int p, int idx, bool adding) {
            
            // Step A: Remove CURRENT contribution of p from Segment Tree
            if (!pos[p].empty()) {
                // Remove Base Score
                st.range_add(1, n - 1, -1);
                
                int first = *pos[p].begin();
                int last = *pos[p].rbegin();
                
                // Remove Bonus Score if it exists
                if (first < last) {
                    st.range_add(first + 1, last, -1);
                }
            }
            
            // Step B: Update the index set
            if (adding) pos[p].insert(idx);
            else pos[p].erase(idx);
            
            // Step C: Add NEW contribution of p to Segment Tree
            if (!pos[p].empty()) {
                // Add Base Score
                st.range_add(1, n - 1, 1);
                
                int first = *pos[p].begin();
                int last = *pos[p].rbegin();
                
                // Add Bonus Score if it exists
                if (first < last) {
                    st.range_add(first + 1, last, 1);
                }
            }
        };

        // Initial population of the system
        for (int i = 0; i < n; ++i) {
            if (is_prime[nums[i]]) {
                update_contribution(nums[i], i, true);
            }
        }

        vector<int> results;
        results.reserve(queries.size());

        // Process each query
        for (auto& q : queries) {
            int idx = q[0];
            int val = q[1];
            int old_val = nums[idx];
            
            if (old_val != val) {
                // Handle removal of old value
                if (is_prime[old_val]) {
                    update_contribution(old_val, idx, false);
                }
                
                // Handle addition of new value
                if (is_prime[val]) {
                    update_contribution(val, idx, true);
                }
                
                // Update the array
                nums[idx] = val;
            }
            results.push_back(st.query_max());
        }
        return results;
    }
};

```

## Complexity Analysis

It is important to understand why this solution is efficient enough for the problem constraints.

### Time Complexity

* **Preprocessing:** The Sieve runs in effectively linear time relative to `MAX_VAL`.
* **Initialization:** We iterate through `nums` once. Each insertion into the set takes `O(log N)` and the Segment Tree update takes `O(log N)`. Total initialization is `O(N log N)`.
* **Query Processing:** For each query, we perform a constant number of set operations (`O(log N)`) and Segment Tree updates (`O(log N)`).
* **Total:** `O((N + Q) * log N)`. Given `N` and `Q` are up to 50,000, this comfortably fits within typical time limits (usually 1 or 2 seconds).

### Space Complexity

* **Segment Tree:** Uses `4 * N` integers.
* **Position Sets:** Stores at most `N` indices across all sets.
* **Total:** `O(N)`. This is very memory efficient.

## Key Takeaways

1. **Decompose the Problem:** Instead of trying to calculate the split maximum from scratch, we looked at the contribution of individual elements (primes).
2. **Transformation:** We transformed the condition "prime exists in prefix and suffix" into a geometric range problem.
3. **Correct Tooling:** We identified that the Segment Tree is the perfect tool for handling "add to range" and "query max" operations dynamically.
4. **Lazy Propagation:** This technique is essential for making range updates efficient. Without it, updating a range would be too slow.

This problem is a beautiful example of how changing your perspective from "global calculation" to "incremental updates" can unlock massive performance gains.

I hope this explanation helps you understand the mechanics behind solving hard array problems involving range queries. Feel free to use this code as a template for similar problems in the future. 

Happy coding!
