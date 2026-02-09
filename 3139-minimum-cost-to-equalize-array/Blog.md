# Balancing the Scales A Guide to Equalizing Arrays with Minimum Cost [#3139]

[Blog Post Link](https://leetcode.com/discuss/post/7566527/balancing-the-scales-a-guide-to-equalizi-u85u/)


Hello to all my fellow coding enthusiasts! Now I want to share a fascinating journey through a problem that looks simple on the surface but hides some beautiful mathematical depth. We are going to explore how to make all elements in an array equal while spending the least amount of money possible.

This challenge is a wonderful exercise in logical thinking and edge case analysis. It requires us to move beyond simple greedy strategies and consider how changing the final goalpost can actually save us effort.

## The Challenge Before Us

Imagine you are playing a game where you manage a team of characters. Each character has a certain power level represented by an integer. Your goal is to make every character on the team have the exact same power level. To do this you can perform two types of magic spells.

The first spell allows you to choose one character and increase their power by 1. This costs you a specific amount we will call **cost1**.

The second spell is more complex. It allows you to choose two different characters and increase the power of both of them by 1. This costs you an amount we will call **cost2**.

You are given an array of integers representing the current power levels and the values for **cost1** and **cost2**. We need to find the minimum total cost to equalize the array. Because the answer might be huge we will return the result modulo 1,000,000,007.

## Initial Observations and The Economy of Magic

Before we write a single line of code we need to analyze our tools. We have two ways to add value to our array.

**Option A** is the Single Increment. We pay **cost1** to add 1 total value to the array.
**Option B** is the Pair Increment. We pay **cost2** to add 2 total value to the array (1 to two different people).

This leads to our first major decision point. Is the Pair Increment actually a good deal?

If **2 times cost1** is less than or equal to **cost2** then the Pair Increment is a trap. It costs more to raise two numbers together than it would to just raise them individually. In this specific scenario we should ignore the Pair Increment entirely. We would simply calculate the gap between every number and the maximum number in the array and multiply that total gap by **cost1**.

However things get interesting when **2 times cost1** is greater than **cost2**. This means we get a discount for raising numbers in pairs! To minimize cost we should prioritize using the Pair Increment as much as possible and only use the Single Increment when absolutely necessary.

## The Strategy for Discounted Pairs

Let us assume the Pair Increment is cheaper. Our goal is now to maximize the number of pairs we use. You might think the best target value for everyone to reach is simply the current maximum value in the array. While that is a logical starting point it is not always the cheapest destination.

We need to consider two main constraints when trying to use pairs.

### Constraint 1 The Parity of the Total Gap
A Pair Increment adds exactly 2 to the sum of the array. If the total difference we need to bridge is an odd number we will inevitably have a remainder of 1. That last 1 must be filled using a Single Increment.

### Constraint 2 The Bottleneck Principle
This is the trickiest part of the problem. Imagine you have an array `[1, 10, 10]` and you want to reach `10`.
The first element needs to gain 9. The others need to gain 0.
Even though pair operations might be free you cannot use them here because you need two **different** indices to increment. You have only one element that needs growing.

We can generalize this. Let **total difference** be the sum of increases all numbers need. Let **max difference** be the increase the smallest number needs (the one furthest behind).

If **max difference** is greater than half of the **total difference** then that single smallest number is a bottleneck. It needs more growth than all the other numbers combined. We can pair it with every other available slot but eventually we will run out of partners. The remaining growth for that bottleneck must be paid for with the expensive Single Increment.

If **max difference** is less than or equal to half of the **total difference** then we are in a safe zone. We can distribute the operations such that almost everything is covered by Pair Increments (except for a potential parity remainder).

## The Hidden Dimension Raising the Bar

Here is where the solution shines. If we find ourselves in a bottleneck situation where we are forced to use too many Single Increments we can actually change the rules.

Instead of aiming for the current maximum value let us try aiming for `maximum + 1` or `maximum + 2`.

Why would we do this?
When we raise the target value by 1 we increase the requirement for every single element by 1. If we have `n` elements the **total difference** increases by `n`. However the **max difference** (the gap for the smallest number) only increases by 1.

Because `n` (the array size) is usually much larger than 1 the **total difference** grows much faster than the **max difference**. This helps us break out of the bottleneck. By raising the target we might spend more on total operations but we shift those operations from the expensive Single type to the cheaper Pair type.

## The Algorithm Step by Step

We can now formulate a robust plan.

1.  **Basic Analysis**
    Find the minimum element, maximum element and the sum of the array.
    Check if the array size is very small (less than 3). If so we cannot effectively juggle pairs to balance differences so we treat it as a Single Increment case.

2.  **The Cheap Single Case**
    If `2 * cost1 <= cost2` simply calculate the cost to bring everyone to the current maximum using only Single Increments.

3.  **The Cheap Pair Case**
    If pairs are efficient we need to search for the best target. We will iterate through possible target values.
    **Start point:** The current maximum element.
    **End point:** Roughly `2 * maximum element`.
    *Why this limit?* Mathematically once we raise the target enough to break the bottleneck going higher just adds unnecessary cost. The crossover point is guaranteed to happen within reasonable bounds.

4.  **Cost Calculation Loop**
    For each target value `T` from `max` to `2 * max` perform the following:
    * Calculate `current total difference` equal to `T * n - sum`.
    * Calculate `current max difference` equal to `T - min_element`.
    * Check for the bottleneck.
    * **If Bottleneck:** We pair the "other" elements with the bottleneck element. The remaining part of the bottleneck is paid by `cost1`.
    * **If No Bottleneck:** We assume we can pair everything. We pay `cost2` for every pair. If there is a remainder of 1 we pay `cost1` for it.
    * Keep track of the minimum cost found during this search.

## Complexity Analysis

Let us look at how efficient this approach is.

**Time Complexity**
We iterate from `max_val` to `2 * max_val`. The range of values is determined by the input constraints where numbers are up to 1,000,000. This might seem large but the inner operations are simple arithmetic calculations taking O(1) time. Therefore the complexity is roughly proportional to the range of the values which fits well within the time limits (usually a few million operations). The finding of min and max takes O(N).

**Space Complexity**
We only store a few variables for sums and limits. This gives us O(1) space complexity making the solution very memory efficient.

## The Solution Code

Here is the C++ implementation. I have used `long long` to prevent integer overflow during the intermediate calculations.

```cpp
class Solution {
public:
    int minCostToEqualizeArray(vector<int>& nums, int cost1, int cost2) {
        long long n = nums.size();
        long long MOD = 1e9 + 7;
        
        long long min_val = nums[0];
        long long max_val = nums[0];
        long long total_sum = 0;
        
        // Step 1 Gather basic statistics
        for (int x : nums) {
            min_val = min(min_val, (long long)x);
            max_val = max(max_val, (long long)x);
            total_sum += x;
        }
        
        // Base cost calculation if we target the current max_val
        long long total_diff = max_val * n - total_sum;
        
        // Step 2 Check if single operations are cheaper OR if array is too small
        // If cost1 * 2 <= cost2 it is never optimal to use pair operations.
        // If n < 3 we cannot use pair operations to close relative gaps effectively.
        if (cost1 * 2 <= cost2 || n <= 2) {
            return (total_diff * cost1) % MOD;
        }
        
        // Step 3 Iterate through possible target values
        // We might need to raise the target to allow more pair operations
        long long ans = -1; // Initialize with a sentinel value
        
        // We search up to 2 * max_val which is a safe upper bound for the bottleneck
        for (long long target = max_val; target <= 2 * max_val; ++target) {
            long long current_diff = target * n - total_sum;
            long long max_element_diff = target - min_val;
            
            long long current_cost = 0;
            
            // Bottleneck Check
            // If the largest gap is more than half the total gap
            if (max_element_diff > current_diff / 2) {
                // We pair the rest of the numbers with the bottleneck number
                long long pairs = current_diff - max_element_diff;
                long long singles = max_element_diff - pairs;
                current_cost = pairs * cost2 + singles * cost1;
            } else {
                // No bottleneck we can pair freely
                long long pairs = current_diff / 2;
                long long singles = current_diff % 2;
                current_cost = pairs * cost2 + singles * cost1;
            }
            
            if (ans == -1 || current_cost < ans) {
                ans = current_cost;
            }
        }
        
        return ans % MOD;
    }
};

```

## Final Thoughts

This problem is a perfect example of why we should always question our initial greedy assumptions. The intuitive answer (aiming for the current maximum) is not always the correct one. By visualizing the differences as gaps that need filling we can derive a mathematical condition (the bottleneck) that guides us to the optimal solution.

It is also a great reminder to check constraints. The relationship between `cost1` and `cost2` completely changes the logic we need to apply.

I hope this walkthrough helps you understand the mechanics behind array equalization. It is a tough problem but breaking it down into these logical cases makes it much more manageable. 

Happy coding to everyone and good luck with your algorithmic adventures!
