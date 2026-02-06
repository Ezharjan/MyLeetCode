# Unlocking the Maximum Score in Cyclic Partitions [#3743]

[Blog Post Link](https://leetcode.com/discuss/post/7543752/unlocking-the-maximum-score-in-cyclic-pa-2k2p/)



Partitioning a cyclic array to maximize the sum of range scores stands as a formidable algorithmic challenge. This problem requires us to divide a circular sequence of numbers into at most  subarrays. We calculate the range for each subarray by finding the difference between its maximum and minimum elements. The goal is to maximize the total sum of these ranges.

This task is difficult because the array wraps around. A subarray can start at the end of the array and finish at the beginning. Furthermore, the number of subarrays is flexible, provided it does not exceed . We need a robust strategy that moves beyond simple greedy approaches. We must employ Dynamic Programming to explore all valid states efficiently.

## The Core Concept The Contribution Technique

The naive approach involves iterating through all possible partition points. This is far too slow. We need to change our perspective. Instead of visualizing complete subarrays, let us look at the individual contribution of each number.

The score of a single subarray is defined as **Maximum Value minus Minimum Value**.

Consequently, the total score of the entire partition is the **Sum of Maximums minus Sum of Minimums**.

This transformation simplifies the problem immensely. We no longer need to track boundaries of intervals. We only need to decide the role of each element in the array. For every number `nums[i]`, we have three choices:

1. **Ignore it.** The element is neither a minimum nor a maximum for its interval.
2. **Pick it as a Maximum.** This adds `+nums[i]` to our total score.
3. **Pick it as a Minimum.** This adds `-nums[i]` (negative `nums[i]`) to our total score.

We are limited to at most  subarrays. Since each subarray has exactly one minimum and one maximum, we can select at most  pairs of extremum values. This means the total number of picked elements (both maximums and minimums combined) cannot exceed .

## Handling Valid Intervals with Balance

We cannot simply pick maximums and minimums arbitrarily. They must form valid intervals. In a linear array, we typically alternate. We pick a start (minimum), then an end (maximum), or vice versa. We can track this using a variable called `balance`.

* **Balance increases** when we pick a Maximum (opening a positive contribution).
* **Balance decreases** when we pick a Minimum (opening a negative contribution).

For a standard linear partition, the balance usually toggles between 0 and 1. However, this problem involves a cyclic array. An interval might wrap around the edge, meaning we could start with a "pending" interval. To accommodate any valid configuration of disjoint or adjacent cyclic intervals, we use a small range of states for our balance:

* **State 0:** We have one excess Minimum (net count is negative one). We "owe" a Maximum to close the pair.
* **State 1:** The Neutral State (net count is zero). All picked Minimums have matching Maximums.
* **State 2:** We have one excess Maximum (net count is positive one). We "owe" a Minimum to close the pair.

## The Cyclic Strategy

The circular nature of the array creates a dependency between the start and the end. If we begin the array with a balance of 1 (neutral), we must end with a balance of 1. If we begin with a balance of 2 (wrapping a maximum from the end), we must finish the array in that same state to complete the cycle.

To solve this, we run our Dynamic Programming algorithm three separate times. We fix the starting balance at 0, then 1, and finally 2. For each run, we only consider the final answer valid if the ending balance matches the starting balance.

## Designing the Dynamic Programming Algorithm

We define our DP state as `dp[picks][balance]`.

* `picks`: The total count of elements selected as either minimum or maximum so far. This goes from 0 up to .
* `balance`: The current open status (0, 1, or 2).

### The Transitions

For each element `x` in the array, we update our table based on the previous values:

1. **Skip the Element:** The state remains the same.
`dp[picks][bal]` transitions to `dp[picks][bal]`.
2. **Pick as Max:** We add `x` to the score. We transition from `balance` to `balance + 1`. This uses one pick.
3. **Pick as Min:** We subtract `x` from the score. We transition from `balance` to `balance - 1`. This uses one pick.

### Space Optimization

A standard 2D table of size  might be too large or slow to allocate repeatedly. We can optimize this. Notice that computing the current state only requires values from the previous step. We can use a single array and iterate backwards through the `picks` dimension. This allows us to update the DP table in place without overwriting the data we need for the current calculation. This reduces the space complexity to .

## The Optimized C++ Solution

Below is the highly efficient C++ implementation. It avoids expensive memory allocations inside the loop and uses the logic described above.

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    long long maximumScore(vector<int>& nums, int k) {
        int n = nums.size();
        long long ans = 0;
        
        // We use a static sized array to ensure fast access and minimal overhead.
        // The dimensions represent [number of picks][current balance].
        // Max picks is 2*k (at most 2000). We use 2005 for safety.
        long long dp[2005][3];
        
        // A sufficiently large number to represent invalid states.
        const long long INF = 1e16; 

        // We iterate through all valid starting balance states (0, 1, 2).
        // This accounts for intervals that might wrap around the cyclic array.
        for (int start_bal = 0; start_bal <= 2; ++start_bal) {
            
            // Initialize the DP table with negative infinity
            for(int j = 0; j <= 2 * k; ++j) {
                dp[j][0] = -INF;
                dp[j][1] = -INF;
                dp[j][2] = -INF;
            }
            
            // The initial state must match our chosen start_bal with 0 picks.
            dp[0][start_bal] = 0;
            
            // Process every number in the cyclic array
            for (int i = 0; i < n; ++i) {
                long long x = nums[i];
                
                // We iterate backwards to allow inplace updates.
                // We can make at most i+1 picks so far, capped at 2*k.
                int limit = min(2 * k, i + 1);
                
                for (int j = limit; j >= 1; --j) {
                    // Retrieve values from the previous state (before processing x)
                    long long prev_bal0 = dp[j-1][0];
                    long long prev_bal1 = dp[j-1][1];
                    long long prev_bal2 = dp[j-1][2];
                    
                    // Current values represent the "Skip" option
                    long long cur0 = dp[j][0];
                    long long cur1 = dp[j][1];
                    long long cur2 = dp[j][2];

                    // Option: Become a Minimum (subtract x)
                    // Transition from bal 1 to 0
                    if (prev_bal1 > -INF) cur0 = max(cur0, prev_bal1 - x);
                    dp[j][0] = cur0;

                    // Option: Become a Minimum (subtract x)
                    // Transition from bal 2 to 1
                    if (prev_bal2 > -INF) cur1 = max(cur1, prev_bal2 - x);
                    
                    // Option: Become a Maximum (add x)
                    // Transition from bal 0 to 1
                    if (prev_bal0 > -INF) cur1 = max(cur1, prev_bal0 + x);
                    dp[j][1] = cur1;

                    // Option: Become a Maximum (add x)
                    // Transition from bal 1 to 2
                    if (prev_bal1 > -INF) cur2 = max(cur2, prev_bal1 + x);
                    dp[j][2] = cur2;
                }
            }
            
            // Final Validation
            // 1. The final balance must match the start_bal to close the cycle.
            // 2. We must have made an even number of picks (pairs of min and max).
            for (int j = 2; j <= 2 * k; j += 2) {
                ans = max(ans, dp[j][start_bal]);
            }
        }
        
        return ans;
    }
};

```

## Complexity Analysis

This solution is designed for performance.

* **Time Complexity:** The outer loop runs 3 times (constant). The main loop iterates  times. The inner loop iterates up to  times. This results in a total complexity of . Given that both  and  are up to 1000, the operations are roughly , which is well within the time limit.
* **Space Complexity:** We use a fixed size array of approximately . This results in  space complexity. This is extremely memory efficient compared to allocating full  tables.

By focusing on individual element contributions and maintaining a balanced state, we transform a complex geometric problem on a circle into a straightforward linear optimization task.

What do you think about my solution?

Share you solutions or insights in the comments below~
