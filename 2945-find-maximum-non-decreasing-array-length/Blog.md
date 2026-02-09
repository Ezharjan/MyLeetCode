# Unlocking Array Transformations with Dynamic Programming and Monotonic Queues [#2945]


[Blog Post Link](https://leetcode.com/discuss/post/7564530/unlocking-array-transformations-with-dyn-uzrp/)


Welcome back coding friends! Today I want to share a fascinating challenge I stumbled upon that really tested my understanding of array manipulations and optimization techniques. It is one of those problems that looks simple on the surface but hides a beautiful depth requiring us to combine greedy strategies with dynamic programming.

If you have ever wondered how to efficiently manipulate arrays to satisfy specific ordering conditions while maximizing their length, this walkthrough is for you. We are going to break down the logic step by step, exploring how we can transform a standard O(N^2) approach into something much faster using a monotonic queue.

### The Challenge at Hand

Let us start by understanding what we are trying to solve. We are given a 0 indexed integer array called `nums`. We have a specific goal. We want to transform this array into a **non decreasing** array (where every element is greater than or equal to the previous one).

However, we are not just swapping elements. The only operation allowed is to select a contiguous subarray and replace it with the **sum** of its elements.

For example, if you have `[1, 3, 5, 6]` and you decide to merge the subarray `[3, 5]`, your array becomes `[1, 8, 6]`. Note that `3 + 5 = 8`.

Our objective is to perform these operations in such a way that the final array is sorted in non decreasing order, and crucially, we want this final array to have the **maximum possible length**.

Why is this tricky?
Merging elements reduces the length of the array. To get the maximum length, we want to perform as few merges as possible. However, to satisfy the non decreasing condition, we are often forced to merge smaller numbers into larger sums so they become big enough to stand behind the previous element. This creates a tension between keeping the array long (few merges) and making the elements large enough to be sorted (more merges).

### Building Intuition

Let us try to reason about this greedily. Suppose we are building our non decreasing sequence from left to right. We finish forming the i-th element of our new sequence, and it has a value V. Now we look at the remaining numbers in `nums`. We need to form the (i+1)-th element.

To maximize the total length of the array, we want to finish forming this next element as soon as possible. We do not want to keep adding more numbers to the current sum if we do not have to, because using more numbers now leaves fewer numbers for the rest of the array.

Therefore, our strategy should be **Make the current element as small as possible, provided it is still greater than or equal to the previous element.**

By keeping the current element small, we make it easier for the *next* element to be greater than or equal to it without needing to consume too many indices. This "keep it small" strategy propagates forward, giving us the best chance to extend the array length.

### The Dynamic Programming Approach

This intuition naturally leads us to Dynamic Programming. We need to keep track of the optimal state as we process the array from left to right.

Let us define `dp[i]` as the maximum length of a non decreasing array we can create using the prefix of the original array ending at index `i`.

But wait! Just knowing the length is not enough. To decide if we can form the next element, we need to know the value of the last element we just formed. If we have two ways to construct a sequence of length 3, we prefer the one where the last element is smaller, because that makes it easier to extend the sequence further.

So, let us introduce a second array, let us call it `last[i]`. This will store the **sum of the last element** in the optimal sequence of length `dp[i]` ending at index `i`.

#### The Transition Logic

For every index i, we want to find a previous index j (where j < i) such that the sum of the subarray `nums[j...i]` is greater than or equal to `last[j]`.

If this condition is met, it means we can append the subarray `nums[j...i]` (merged into a single value) to the sequence ending at j.

Mathematically, we look for a j that satisfies the condition `Sum(nums[j...i]) >= last[j]`.

Among all valid j indices that satisfy this, which one should we pick?
To maximize the count, we want `dp[j]` to be as large as possible. If there are multiple j values giving the same max `dp[j]`, we want the one that yields the smallest new sum `Sum(nums[j...i])`. Since `Sum(nums[j...i])` increases as j moves to the left (we are summing more elements), we want the **largest possible j** (the one closest to i) that satisfies the condition.

So the transition becomes:
1. Find the largest j < i such that `PrefixSum[i] - PrefixSum[j] >= last[j]`.
2. Set `dp[i] = dp[j] + 1`.
3. Set `last[i] = PrefixSum[i] - PrefixSum[j]`.

### The Bottleneck

If we implement the logic above directly, we would have a nested loop. For every i from 1 to N, we would iterate back through all j from i - 1 down to 0 to find the best j.

This results in a time complexity of roughly O(N^2). Given that N can be up to 100,000, a quadratic solution is effectively doing 10 billion operations, which is far too slow for standard limits. We need something better.

### Optimization with Monotonicity

Let us look closer at the inequality we need to satisfy.
`PrefixSum[i] - PrefixSum[j] >= last[j]`

We can rearrange this to group the j related terms together.
`PrefixSum[i] >= last[j] + PrefixSum[j]`

Let us call this value `last[j] + PrefixSum[j]` the **Constraint Value** for index j.
We are looking for a j such that its Constraint Value is less than or equal to `PrefixSum[i]`.

Here is the key observation.
As we iterate i from 0 to N, `PrefixSum[i]` is strictly increasing (assuming positive numbers). This means if a specific j is valid for a current i, it will also be valid for all future values of i.

Furthermore, we noticed earlier that we want the **largest valid j**.
Let us consider two candidate indices, A and B, where A < B.
If `ConstraintValue(A) >= ConstraintValue(B)`, is there ever a reason to keep A?
No. B is a better candidate because:
1. B is larger (closer to the current index), which generally implies a better DP state or a tighter last sum.
2. B is easier to match (smaller constraint value).
3. B allows us to jump from a later point.

This suggests we can maintain a collection of candidate indices where the Constraint Value is strictly increasing. If we encounter a new candidate with a smaller Constraint Value than the one before it, the previous one becomes obsolete.

This structure is a **Monotonic Queue** (or simply a vector we use as a stack or queue).

### The Optimized Algorithm

We will maintain a list `q` of indices j.
The property we maintain in `q` is that `last[j] + PrefixSum[j]` is strictly increasing.

For a new index i:
1. We want to choose the largest j from `q` such that `last[j] + PrefixSum[j] <= PrefixSum[i]`.
2. Since `q` is sorted by this constraint value, we can use **Binary Search** to find this j efficiently. Specifically, `std::upper_bound` in C++ can help us find the first element that exceeds our limit, and the element right before that is our best valid j.
3. Once we find the best j, we compute `dp[i]` and `last[i]`.
4. Now, i becomes a candidate for future steps. We calculate its Constraint Value: `last[i] + PrefixSum[i]`.
5. We add i to `q`. To maintain the sorted property of `q`, we remove candidates from the back of `q` that have a Constraint Value greater than or equal to the value for i. Why? Because i is a larger index (better) with a smaller or equal constraint (easier to satisfy), making those previous candidates useless.

By doing this, finding the optimal j takes logarithmic time O(log N), and adding i to the queue is amortized constant time O(1). The overall complexity becomes O(N log N).

### A Detailed C++ Solution

Here is the complete code implementing this logic. I have added detailed comments to help you follow along.

```cpp
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findMaximumLength(vector<int>& nums) {
        int n = nums.size();
        
        // We use a prefix sum array to calculate subarray sums in O(1).
        // Since the sum of elements can exceed the integer limit, we use long long.
        // pre[i] represents the sum of nums[0] through nums[i - 1].
        vector<long long> pre(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pre[i + 1] = pre[i] + nums[i];
        }

        // dp[i] stores the maximum length of a non decreasing array 
        // that can be formed using the prefix nums[0...i - 1].
        vector<int> dp(n + 1, 0);
        
        // last[i] stores the sum of the *last element* (which is a merged subarray)
        // of the optimal sequence ending at index i.
        // We want to minimize this value to allow future elements to be added easily.
        vector<long long> last(n + 1, 0);
        
        // q will store candidate indices 'j'.
        // We maintain the invariant that (pre[j] + last[j]) is strictly increasing in q.
        // This allows us to binary search for the best j.
        vector<int> q;
        
        // Initialize with index 0 (base case)
        q.push_back(0);

        for (int i = 1; i <= n; ++i) {
            // We need to find the largest index j < i such that:
            // last[j] <= Sum(nums[j...i - 1])
            // Which is: last[j] <= pre[i] - pre[j]
            // Rearranging: (pre[j] + last[j]) <= pre[i]
            
            // We want the largest 'j' that satisfies this. 
            // Since our queue 'q' is sorted by (pre[j] + last[j]), we can search.
            
            // upper_bound returns an iterator to the first element that is strictly 
            // GREATER than pre[i].
            auto it = upper_bound(q.begin(), q.end(), pre[i], [&](long long val, int index) {
                return val < (pre[index] + last[index]);
            });
            
            // The element immediately BEFORE 'it' is the largest one that is 
            // LESS THAN OR EQUAL TO pre[i]. This is our best candidate.
            int j = *prev(it);
            
            // Perform the transition
            dp[i] = dp[j] + 1;
            last[i] = pre[i] - pre[j];
            
            // Now we prepare to add 'i' into the queue of candidates.
            // We calculate the constraint value for the current index i.
            long long current_val = pre[i] + last[i];
            
            // Maintain the Monotonic property of the queue.
            // If the element at the back of the queue has a constraint value >= current_val,
            // then 'i' is a strictly better candidate (it is a later index with a smaller cost).
            // So we remove the worse candidate.
            while (!q.empty() && (pre[q.back()] + last[q.back()]) >= current_val) {
                q.pop_back();
            }
            
            q.push_back(i);
        }
        
        return dp[n];
    }
};

```

### Complexity Analysis

It is always good practice to verify that our solution fits within the limits.

* **Time Complexity:** We iterate through the array once. Inside the loop, we perform a binary search using `upper_bound`, which takes O(log N) time. The `while` loop that pops elements from `q` might look expensive, but each element is added to `q` once and removed at most once. Thus, the amortized cost of the queue operations is O(1). The dominant factor is the binary search, making the total time complexity O(N log N). Given N = 10^5, this is well within the time limit.
* **Space Complexity:** We use auxiliary arrays for prefix sums `pre`, dynamic programming `dp`, and `last` values, plus the queue `q`. All of these scale linearly with the input size. Therefore, the space complexity is O(N).

### Wrapping Up

This problem is a fantastic example of how we can optimize dynamic programming using data structures. We started with a clear greedy intuition to minimize the last element to maximize room for future growth. We translated that into a DP relation, identified the performance bottleneck, and solved it by noticing the monotonic nature of the transition condition.

I hope this explanation clears up the logic behind finding the maximum non decreasing array length. It is a tough problem, so do not feel discouraged if it takes a while to sink in. Walking through the code with a small paper example (like `[5, 2, 2]`) is a great way to solidify the concept.

Happy coding, and see you in the next challenge!
