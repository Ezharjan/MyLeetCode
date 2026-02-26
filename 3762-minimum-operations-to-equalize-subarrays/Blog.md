# Equalizing Subarrays with Minimum Operations: A Deep Dive into Range Queries [#3762]

Welcome fellow coders! Today I want to share a fascinating problem involving arrays, modular arithmetic, and efficient range queries. If you have ever wondered how to efficiently manipulate subarrays to make all elements equal, or if you are looking to deepen your understanding of Persistent Segment Trees, this post is for you.

We are going to explore **Minimum Operations to Equalize Subarrays**. This is a "Hard" difficulty problem, but do not let that scare you. We will break it down into small, manageable logic puzzles and solve it step by step.

## Understanding the Problem

Imagine you are given an array of integers called `nums` and a special integer `k`.

You are allowed to perform a specific operation any number of times: you can increase or decrease any element in the array by exactly `k`.

**The Goal:**
We have a list of queries. Each query gives us a start index `l` and an end index `r`. For each query, we need to answer two questions about the subarray from `l` to `r`:

1. Is it possible to make all the numbers in this subarray equal using our operation?
2. If it is possible, what is the **minimum** number of operations required?

If it is impossible, we return `-1`.

### A Quick Example

Let's look at `nums = [1, 4, 7]` with `k = 3`.
Suppose our query is the whole array `[0, 2]`.

* We can turn `1` into `4` by adding `3` once (1 operation).
* We can turn `7` into `4` by subtracting `3` once (1 operation).
* Now the array is `[4, 4, 4]`. All equal!
* Total operations: `1 + 1 = 2`.

## Step 1 The Possibility Check

Before we worry about the cost, we must determine if equalization is even possible.

The key constraint is that we can only add or subtract `k`. This brings us to **Modular Arithmetic**.

If you start with a number `A` and add `k` repeatedly, the result `B` will always satisfy this property:
`A % k == B % k` (A modulo k equals B modulo k).

In simpler terms, two numbers can only become equal via steps of size `k` if they share the **same remainder** when divided by `k`.

**The Condition:**
For a subarray `nums[l..r]` to be equalizable, every single number in that range must have the same value modulo `k`.

**Optimizing the Check:**
Checking every number in the range `[l, r]` for every query would be too slow (specifically `O(N)` per query). We need a faster way.

We can use a **Difference Array** combined with **Prefix Sums**.

1. Create a boolean array (or integer array) where index `i` is `1` if `nums[i] % k` is different from `nums[i-1] % k`, and `0` otherwise.
2. Build a prefix sum array from this.
3. For any query `[l, r]`, we check the sum of differences in the range `[l+1, r]`. If the sum is greater than 0, it means there is at least one "break" in the pattern, so the answer is impossible (`-1`). If the sum is 0, all remainders are the same.

*Note: Be careful with negative numbers in C++! The modulo operator can return negatives. A safe way to handle this is `((nums[i] % k) + k) % k`.*

## Step 2 Minimizing the Cost

If we know it is possible, we need to find the target value that minimizes the number of operations.

Since every valid number `x` in our subarray can be written as `x = q * k + rem` (where `rem` is the common remainder), we can simplify the problem. Instead of looking at `nums[i]`, let's look at their quotients `q = nums[i] / k`.

Our problem transforms into:
**Given a list of integers (the quotients), find a target integer T such that the sum of absolute differences `sum(|q - T|)` is minimized.**

### Why the Median?

This is a classic mathematical property. To minimize the sum of absolute differences (also known as the L1 norm), the optimal target `T` is the **median** of the numbers.

* If we choose the mean (average), we minimize the squared differences.
* If we choose the **median**, we minimize the absolute differences.

So, for each query `[l, r]`, our algorithm needs to:

1. Extract the quotients `nums[i] / k` for indices `l` to `r`.
2. Find the median of these quotients.
3. Calculate the total cost: `sum(|quotient - median|)` for all quotients in the range.

## Step 3 Efficient Range Queries

This is the hardest part. We need to find the median and the sum of distances for an arbitrary subarray. A simple sorting approach would be too slow (`O(N log N)` per query).

We need a data structure that acts like a "history" of the array, allowing us to query statistics about any range. The perfect tool for this is the **Persistent Segment Tree** (often implemented as a Merge Sort Tree or Chairmans Tree).

### How the Persistent Segment Tree Works

Imagine building a Segment Tree for every prefix of the array.

* Tree 0 contains data for `nums[0...0]`
* Tree 1 contains data for `nums[0...1]`
* ...
* Tree `i` contains data for `nums[0...i]`

A query for range `[l, r]` can be answered by looking at `Tree r` and subtracting the influence of `Tree l-1`.

Since building `N` separate trees is expensive, we use **Persistence**. When we update the tree to add index `i`, we only create new nodes for the path that changed, reusing the nodes from `Tree i-1` for the rest. This keeps memory usage to `O(N log N)`.

### What We Store in the Tree

We operate on the **values** (the quotients). Since the values can be very large (up to `10^9`), we first compress them using **Coordinate Compression** (mapping the distinct values to ranks 0, 1, 2...).

Each node in our Segment Tree will store:

1. **Count:** How many numbers fall into this value range.
2. **Sum:** The total sum of the numbers in this value range.

### Answering a Query

To process a query `[l, r]`:

1. **Find the Median:** We look for the `k-th` smallest number in the range, where `k = (length / 2) + 1`. We traverse the segment tree (using `Root[r]` and `Root[l-1]`) to find the value that splits the data in half. Let's call this `median_val`.
2. **Calculate Cost:**
The total cost is the sum of differences. We can split this into two parts: numbers smaller than the median and numbers larger than the median.
* For numbers `x <= median_val`: Cost is `median_val - x`.
* For numbers `x > median_val`: Cost is `x - median_val`.


Using our Segment Tree, we can query the `Count` and `Sum` of all numbers less than or equal to the median in `O(log N)` time.
* `LeftCost = (Count_Left * median_val) - Sum_Left`
* `RightCost = Sum_Right - (Count_Right * median_val)`
* `TotalCost = LeftCost + RightCost`



## Complexity Analysis

Let `N` be the number of elements and `Q` be the number of queries.

* **Time Complexity:**
* Coordinate Compression: `O(N log N)`.
* Building the Persistent Tree: `O(N log N)`.
* Processing Queries: Each query takes `O(log N)` to find the median and calculate sums. Total is `O(Q log N)`.
* Overall: **`O((N + Q) log N)`**. This is highly efficient and fits well within the standard limits for `N = 40,000`.


* **Space Complexity:**
* The Persistent Segment Tree creates `log N` new nodes per element.
* Overall: **`O(N log N)`**.



## The Solution Code

Here is the complete C++ implementation. I have used a clear structure to separate the logic for the Segment Tree from the main solver.

```cpp
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
    // Node structure for the Persistent Segment Tree
    struct Node {
        int l, r;       // Indices of left and right children
        int count;      // Count of numbers in this range
        long long sum;  // Sum of numbers in this range
    };
    
    vector<Node> tree;
    int node_cnt;

    // Build the initial empty tree
    void build(int &u, int l, int r) {
        u = ++node_cnt;
        tree[u].count = 0;
        tree[u].sum = 0;
        if (l == r) return;
        int mid = l + (r - l) / 2;
        build(tree[u].l, l, mid);
        build(tree[u].r, mid + 1, r);
    }
    
    // Update function: returns a new version of the tree
    void update(int prev, int &u, int l, int r, int val_idx, long long real_val) {
        u = ++node_cnt;
        tree[u] = tree[prev]; // Copy data from the previous version
        tree[u].count++;
        tree[u].sum += real_val;
        
        if (l == r) return;
        
        int mid = l + (r - l) / 2;
        if (val_idx <= mid) {
            update(tree[prev].l, tree[u].l, l, mid, val_idx, real_val);
        } else {
            update(tree[prev].r, tree[u].r, mid + 1, r, val_idx, real_val);
        }
    }
    
    // Query to find the index of the k-th smallest value
    int query_kth(int u_l, int u_r, int l, int r, int k) {
        if (l == r) return l;
        int mid = l + (r - l) / 2;
        // How many numbers are in the left child across the range [u_l, u_r]
        int left_count = tree[tree[u_r].l].count - tree[tree[u_l].l].count;
        
        if (k <= left_count) {
            return query_kth(tree[u_l].l, tree[u_r].l, l, mid, k);
        } else {
            return query_kth(tree[u_l].r, tree[u_r].r, mid + 1, r, k - left_count);
        }
    }
    
    // Query to get {count, sum} of numbers with rank <= target_idx
    pair<int, long long> query_prefix(int u_l, int u_r, int l, int r, int target_idx) {
        if (target_idx < l) return {0, 0LL};
        if (r <= target_idx) {
            return {
                tree[u_r].count - tree[u_l].count, 
                tree[u_r].sum - tree[u_l].sum
            };
        }
        int mid = l + (r - l) / 2;
        pair<int, long long> left_res = query_prefix(tree[u_l].l, tree[u_r].l, l, mid, target_idx);
        pair<int, long long> right_res = query_prefix(tree[u_l].r, tree[u_r].r, mid + 1, r, target_idx);
        return {left_res.first + right_res.first, left_res.second + right_res.second};
    }

public:
    vector<long long> minOperations(vector<int>& nums, int k, vector<vector<int>>& queries) {
        int n = nums.size();
        
        // --- Step 1: Precompute Possibility Checks ---
        // We create a prefix sum array to detect modulo mismatches
        vector<int> mod_diff_pref(n, 0);
        for (int i = 1; i < n; ++i) {
            bool diff = false;
            if (k == 0) {
                if (nums[i] != nums[i-1]) diff = true;
            } else {
                // Check if remainders are different
                // Use safe modulo logic if negative numbers were possible (though problem constraints usually imply positive)
                if ((nums[i] % k) != (nums[i-1] % k)) diff = true;
            }
            mod_diff_pref[i] = mod_diff_pref[i-1] + (diff ? 1 : 0);
        }
        
        // --- Step 2: Prepare Values for the Tree ---
        // We work with quotients: val = nums[i] / k
        vector<long long> vals;
        vals.reserve(n);
        if (k == 0) {
            // If k is 0, operations don't change values, so we just use 0 as a placeholder
            vals.assign(n, 0); 
        } else {
            for (int x : nums) vals.push_back((long long)x / k);
        }
        
        // Coordinate Compression: Map large quotients to small ranks 0..M
        vector<long long> sorted_vals = vals;
        sort(sorted_vals.begin(), sorted_vals.end());
        sorted_vals.erase(unique(sorted_vals.begin(), sorted_vals.end()), sorted_vals.end());
        int m = sorted_vals.size();
        
        // Lambda to get rank of a value
        auto get_rank = [&](long long val) {
            return lower_bound(sorted_vals.begin(), sorted_vals.end(), val) - sorted_vals.begin();
        };
        
        // --- Step 3: Build Persistent Segment Tree ---
        // Max nodes estimation: N * log(M) roughly. 
        // 25 is safe for 10^5 range, plus some buffer.
        int max_nodes = n * 30 + 50000;
        tree.resize(max_nodes);
        node_cnt = 0;
        
        vector<int> roots(n + 1);
        build(roots[0], 0, m - 1);
        
        for (int i = 0; i < n; ++i) {
            update(roots[i], roots[i+1], 0, m - 1, get_rank(vals[i]), vals[i]);
        }
        
        // --- Step 4: Answer Queries ---
        vector<long long> ans;
        ans.reserve(queries.size());
        
        for (const auto& q : queries) {
            int l = q[0];
            int r = q[1];
            
            // Check 1: Is equalization possible?
            // If there is any modulo mismatch in the range [l+1, r], it's impossible.
            if (l < r) {
                if (mod_diff_pref[r] - mod_diff_pref[l] > 0) {
                    ans.push_back(-1);
                    continue;
                }
            }
            
            // Edge case: k=0. If possible, cost is 0 (elements already equal).
            if (k == 0) {
                ans.push_back(0);
                continue;
            }
            
            // Find Median Logic
            int cnt = r - l + 1;
            int rank_k = cnt / 2 + 1; // The rank of the median
            
            // Retrieve the rank index of the median value in the compressed array
            int median_idx = query_kth(roots[l], roots[r+1], 0, m - 1, rank_k);
            long long median_val = sorted_vals[median_idx];
            
            // Calculate Cost
            // Cost = Sum of (median - x) for x <= median
            //      + Sum of (x - median) for x > median
            
            // Get stats for elements <= median
            pair<int, long long> prefix = query_prefix(roots[l], roots[r+1], 0, m - 1, median_idx);
            long long cnt_le = prefix.first;    // Count of nums <= median
            long long sum_le = prefix.second;   // Sum of nums <= median
            
            // Get stats for all elements in range
            long long total_sum = tree[roots[r+1]].sum - tree[roots[l]].sum;
            
            // Apply formula
            long long left_cost = (cnt_le * median_val - sum_le);
            long long right_cost = (total_sum - sum_le) - ((cnt - cnt_le) * median_val);
            
            ans.push_back(left_cost + right_cost);
        }
        
        return ans;
    }
};

```

### Wrapping Up

This problem is a fantastic exercise in combining number theory constraints (modular arithmetic) with advanced data structures (Persistent Segment Trees). While the solution requires a fair bit of code, each part serves a distinct and logical purpose.

I hope this walkthrough clarifies the intuition behind the solution. If you encounter similar range query problems in the future, remember the power of "Persistence" to look back into the history of your array updates!

Happy Coding!
