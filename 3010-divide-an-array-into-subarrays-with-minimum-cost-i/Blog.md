# The Entry Fee Secret to Minimizing Array Cost [#3010]


[Blog Post Link](https://leetcode.com/discuss/post/7543401/the-entry-fee-secret-to-minimizing-array-aovk/)


Often in array problems, the constraints and definitions hide a very simple truth. The problem "Divide an Array Into Subarrays With Minimum Cost I" is a perfect example of this. We are asked to divide an array into three contiguous subarrays to minimize a specific "cost."

At first glance, it might seem like a dynamic programming problem or something involving complex sliding windows. But once we look closer at the definition of "cost," the optimal strategy becomes immediately clear.

Let's break down how to arrive at the **optimal solution**.

---

## 🧐 Understanding the Problem Definitions

Before jumping into code, we must fully understand the rules of the game.

1. **The Goal**
Divide integer array `nums` into **exactly 3 disjoint contiguous subarrays**.
* *Contiguous* means the elements must be next to each other in the original array.
* *Disjoint* means they don't overlap.
* Since they must cover the whole array, the split visually looks like this:
`[Subarray 1] [Subarray 2] [Subarray 3]`


2. **The Cost**
The cost of a subarray is strictly defined as the value of its **first element**.
* Cost of `[1, 2, 3]` is **1**.
* Cost of `[12]` is **12**.



We need to minimize the **sum** of the costs of these three subarrays.

## 💡 The Key Insight

Let's look at how these subarrays must be formed. Because the subarrays are contiguous and must cover the entire array starting from the beginning, the **first subarray () MUST start at index 0**.

There is no way around this. You cannot start the first subarray at index 1, because then index 0 wouldn't be included in any subarray.

**Therefore, the element `nums[0]` is ALWAYS part of our total cost.** It is the mandatory "entry fee" to the problem.

If the total cost is defined as:


We know for a fact that:


### The Strategy

Since `nums[0]` is fixed, to minimize the total sum, we only need to worry about minimizing the remaining parts: .

 and  must start somewhere *after* index 0.

* Let's say  starts at index . Its cost is `nums[i]`.
* Let's say  starts at index  (where ). Its cost is `nums[j]`.

To minimize the sum `nums[i] + nums[j]`, we simply need to find the **two smallest numbers** existing in the array from index `1` to the end ().

## 📝 The Algorithm Step by Step

We don't need complex sorting (). We can find the two smallest numbers in a single pass ().

1. **Initialize Total Cost**
Start a variable representing our final answer. Initialize it to `nums[0]`, as we know this is always included.
2. **Prepare Trackers**
We need to find the smallest and second-smallest numbers in the rest of the array. Let's create two variables, `min1` and `min2`, and initialize them to the largest possible integer value (`INT_MAX`). This ensures the first numbers we check will replace them.
3. **The Single Pass Loop**
Iterate through the array starting from index **1** (skipping index 0) up to the end. For every number `current_num`:
* **Check if it's a new minimum:** If `current_num` is smaller than our current `min1`:
* The old `min1` gets demoted to become the new `min2`.
* The `current_num` becomes the new `min1`.


* **Check if it's a new second minimum:** Otherwise, if `current_num` is not smaller than `min1`, but it *is* smaller than `min2`:
* Update `min2` to be `current_num`.




4. **Final Calculation**
Add the discovered `min1` and `min2` to our initial cost. Return the result.

---

## 💻 The C++ Solution

Here is the clean, optimal implementation of the strategy described above.

```cpp
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    int minimumCost(vector<int>& nums) {
        // 1. The fixed cost: The first subarray MUST start at index 0.
        // So, nums[0] is always part of the sum.
        int totalCost = nums[0];
        
        // We need to find the two smallest elements in the remaining array
        // (from index 1 to the end).
        
        // Initialize trackers for the smallest and second smallest values.
        // Use INT_MAX so any numbers in the array will be smaller initially.
        int min1 = INT_MAX;
        int min2 = INT_MAX;
        
        // 2. The Single Pass Loop: Iterate starting from index 1.
        for (int i = 1; i < nums.size(); ++i) {
            int currentNum = nums[i];
            
            if (currentNum < min1) {
                // Found a new smallest number.
                // Push the previous smallest down to second place.
                min2 = min1;
                min1 = currentNum;
            } else if (currentNum < min2) {
                // Found a number smaller than the second place,
                // but not smaller than the first place. Update second place.
                min2 = currentNum;
            }
        }
        
        // 3. Final Calculation: Add the fixed cost and the two minimal starting points found.
        return totalCost + min1 + min2;
    }
};

```

## 📊 Complexity Analysis

* **Time Complexity**
We iterate through the array exactly once (specifically,  times). Finding the minimums in a single pass is linear time. This is faster than sorting the array, which would take .
* **Space Complexity**
We only use a constant amount of extra space to store our variables (`totalCost`, `min1`, `min2`, `i`, `currentNum`), regardless of how large the input array becomes.

Thinking about the constraints leads us directly to the optimal greedy approach! 

Happy coding!
