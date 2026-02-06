# Unlocking the Alternating Sum Mystery Maximizing Products with Dynamic Programming [#3509]

[Blog Post Link](https://leetcode.com/discuss/post/7546934/unlocking-the-alternating-sum-mystery-ma-n4li/)



Hi!
Today we are going to dissect a fascinating problem that combines arithmetic properties, sequence manipulation, and optimization. If you have ever struggled with subsequences or found yourself stuck on Dynamic Programming problems involving negative numbers and state management, this post is for you.

We are going to tackle a problem where we must select a sequence of numbers to satisfy a specific alternating sum while simultaneously maximizing their product. It sounds simple on the surface, but as we will see, there are hidden traps waiting for the unwary. By the end of this guide, you will understand how to use bit manipulation sets to supercharge your Dynamic Programming solutions and how to handle tricky edge cases like multiplication by zero.

### The Challenge Before Us

Let us first clearly define what we are trying to solve.

We are given an array of integers called `nums`. We are also given two specific integers: `k` (our target alternating sum) and `limit` (the maximum allowed product).

Our goal is to find a subsequence of `nums`. A subsequence is just a selection of numbers from the original array kept in their original relative order. For this chosen subsequence, two conditions must be met. First, its **alternating sum** must equal exactly `k`. Second, the **product** of all numbers in the subsequence must be as large as possible without exceeding the `limit`.

If we find such a subsequence, we return that maximum product. If no subsequence satisfies the condition, we return negative one.

### Understanding Alternating Sums

Before we write a single line of code, we must understand the math. What exactly is an alternating sum?

Imagine you have a subsequence of numbers. The alternating sum is calculated by taking the element at index 0, subtracting the element at index 1, adding the element at index 2, subtracting the element at index 3, and so on.

For example, if our subsequence is `[a, b, c, d]`, the alternating sum is `a minus b plus c minus d`.

Notice a pattern here? The sign of the number depends on its position.

* Elements at even indices (0, 2, 4) are **added**.
* Elements at odd indices (1, 3, 5) are **subtracted**.

This parity (even or oddness) of the current length is crucial. If our current subsequence has length 2 (indices 0 and 1 filled), the next number we append will be at index 2. Index 2 is even, so we will **add** that number. If the length is 3, the next index is 3 (odd), so we will **subtract** that number.

### The Naive Approach and Why It Fails

You might initially think about trying every single possible subsequence. In computer science terms, this is the "Brute Force" method. For an array of length `N`, there are `2 to the power of N` possible subsequences.

Let us look at the constraints. The array length can be up to 150. `2 to the power of 150` is an astronomically large number. It is far larger than the number of atoms in the universe. Trying to check every subsequence would take billions of years. We clearly need a more efficient strategy.

### Enter Dynamic Programming

This problem has "Dynamic Programming" written all over it. We are making a sequence of decisions (include a number or skip it) and we need to track certain states to know if a future solution is valid.

What information do we need to remember as we iterate through the `nums` array?

1. **The Current Product:** We need to know the product of the numbers we have picked so far because it cannot exceed the `limit`.
2. **The Current Alternating Sum:** We need to know the running sum so we can check if it equals `k` at the end.
3. **The Current Parity:** We need to know if the next number should be added or subtracted. This depends on whether we have picked an even or odd number of elements so far.

This suggests a DP state structure like `dp[index][current_product][current_sum][parity]`. However, this is too large! The `index` goes up to 150, the `product` up to 5000, and the `sum` can range from negative 1800 to positive 1800. A 4D array of this size would consume too much memory and be too slow to update.

We need to optimize.

### Optimization Strategy The Bitset Revolution

Let us rethink our DP state. We iterate through the numbers one by one, so we can drop the `index` dimension by just keeping the "previous" state and calculating the "next" state.

That leaves us with `product`, `sum`, and `parity`.
The `product` is relatively small (up to 5000). The `parity` is just 0 or 1. The `sum` is the tricky part. It can vary widely.

Instead of storing a boolean `true` or `false` in a massive 3D array like `dp[product][sum][parity]`, let us compress the `sum` dimension.

We can use a `std::bitset` in C++. A bitset is a highly efficient way to store a collection of bits (0s and 1s).
We can define our DP table as:
`dp[product][parity]` equals a `Bitset` of all reachable sums.

If the bit at index `S` is 1, it means it is possible to achieve an alternating sum of `S` with the given `product` and `parity`.

Wait, bitsets cannot have negative indices! The alternating sum can be negative. To solve this, we use an **offset**. Since the maximum possible sum is around 1800 and the minimum is around negative 1800, we can add 2000 to every sum. A sum of 0 becomes index 2000. A sum of negative 5 becomes index 1995.

### The Transition Logic

Now we process each number `x` in the input array. For every existing subsequence that has `product p` and `parity`, what happens if we append `x`?

1. **Update Product:** The new product becomes `p multiplied by x`. If this exceeds `limit`, we usually stop (but wait, there is a catch we will discuss later).
2. **Update Parity:**
* If the old subsequence had **Even** length (parity 0), the next operation is addition. We append `x` (which is at an even index), so the new parity becomes **Odd** (parity 1).
* If the old subsequence had **Odd** length (parity 1), the next operation is subtraction. We append `x` (which is at an odd index), so the new parity becomes **Even** (parity 0).


3. **Update Sum (The Bitwise Magic):**
* **Adding x:** If we have a collection of sums in a bitset, adding `x` to all of them corresponds to a **Left Shift** operation (`<< x`). This moves every bit `x` positions to the left (higher value).
* **Subtracting x:** Subtracting `x` from all sums corresponds to a **Right Shift** operation (`>> x`). This moves every bit `x` positions to the right (lower value).



This allows us to update thousands of possible sums in a single CPU operation!

### The Hidden Trap The Zero Case

This is where many experienced coders fail.

Normally, if `current_product multiplied by x` is greater than `limit`, we would discard that subsequence. It seems logical. Why keep a "bad" subsequence?

But consider the input `nums = [10, 10, 9, 0]` and `limit = 20`.

* We pick 10. Product is 10. Valid.
* We pick the next 10. Product is 100. This is greater than 20. We discard it.
* We pick 9. Product 90. Discard it.
* We pick 0.

Wait! If we had kept the subsequence `[10, 10]` (Product 100), and then appended `0`, the new product would be `100 multiplied by 0`, which is **0**. Zero is less than 20. Suddenly, a previously "invalid" subsequence becomes valid!

If we aggressively prune branches where the product exceeds the limit, we lose the ability to recover them later using a zero.

**The Solution:**
We need two separate trackers:

1. **Valid DP:** Tracks subsequences where the product is strictly within the limit. We use this to find the final answer.
2. **Global Sums:** Tracks **all** reachable alternating sums regardless of the product. This basically assumes the product is "infinity" or "overflow". We only care about the sum and parity here.

When we encounter a `0` in the input array, we can take **any** reachable sum from our `Global Sums` tracker, multiply the subsequence by 0 (making the product 0), and inject it back into our `Valid DP` table at index `product = 0`.

### Step by Step Walkthrough

Let us dry run the algorithm mentally.

**Initialization:**
We create `dp[limit + 1][2]` containing bitsets. Initially empty.
We create `global_sums[2]` containing bitsets. Initially empty.
OFFSET is 2000.

**Processing Number `x`:**

**Case A: `x` is 0**

1. The product becomes 0.
2. We take all sums from `global_sums`.
3. If we were at parity 0 (even length), we append 0. We add 0. The sum stays the same. The parity flips to 1.
4. If we were at parity 1 (odd length), we append 0. We subtract 0. The sum stays the same. The parity flips to 0.
5. We merge these sums into `dp[0][0]` and `dp[0][1]`.
6. We also start a new subsequence `[0]`.

**Case B: `x` is not 0**

1. **Extend Valid Subsequences:** We loop through our `dp` table. For every `product p`, if `p multiplied by x` is less than or equal to `limit`:
* Take `dp[p][0]` (Even parity). Left shift by `x` (Add `x`). Save to `new_dp[new_prod][1]`.
* Take `dp[p][1]` (Odd parity). Right shift by `x` (Subtract `x`). Save to `new_dp[new_prod][0]`.


2. **Start New Subsequence:** If `x` itself is within the limit, we set the bit for `x` in `new_dp[x][1]`.
3. **Update Global Tracker:** We do similar shift operations on `global_sums` but we ignore the product limit entirely. This ensures that if a zero appears later, we have the history of all sums ready to be rescued.

### Complexity Analysis

Why is this optimal?

* **Time Complexity:** We iterate through `nums` (N times). Inside, we loop through `limit` (L times). The bitset operations take constant time roughly `SumRange divided by 64`.
* Total Operations: `N multiplied by L multiplied by (SumRange / 64)`.
* With N=150, L=5000, SumRange=4000, this is roughly `45 million` operations. This easily fits within the standard 1 to 2 second time limit for C++ (which can handle hundreds of millions of operations).


* **Space Complexity:** We store the `dp` table of size `5000 multiplied by 2` bitsets. Each bitset is roughly 500 bytes.
* Total Memory: `10,000 multiplied by 500 bytes` equals roughly `5 Megabytes`. This is very lightweight.



### The Complete Implementation

Here is the highly accurate C++ code implementing this logic. I have added detailed comments to help you follow along.

```cpp
#include <vector>
#include <bitset>
#include <array>
#include <cmath>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxProduct(vector<int>& nums, int k, int limit) {
        // We use an offset to handle negative alternating sums.
        // The maximum theoretical sum is 150 elements * 12 value = 1800.
        // So a range of 4001 bits is sufficient to cover -2000 to +2000.
        const int OFFSET = 2000;
        const int MAX_SUM = 4001; 

        // If k is impossibly large given the constraints, return -1 immediately.
        if (abs(k) > 1800) return -1;

        // dp[prod][parity] = Bitset of achievable sums for valid products <= limit
        // parity 0: Even length (next operation is addition)
        // parity 1: Odd length (next operation is subtraction)
        vector<array<bitset<MAX_SUM>, 2>> dp(limit + 1);

        // global_sums[parity] = Bitset of ALL achievable sums, ignoring product limit.
        // We need this to "rescue" sequences that exceeded the limit if we later find a 0.
        array<bitset<MAX_SUM>, 2> global_sums;

        for (int x : nums) {
            // Create copies for the next state to avoid using updated values 
            // from the current step within the same step.
            auto next_dp = dp;
            auto next_global_sums = global_sums;

            // --- Phase 1: Update the Valid DP Table (Products <= limit) ---
            if (x == 0) {
                // The "Rescue" Operation:
                // If x is 0, the product becomes 0.
                // We can append 0 to ANY existing subsequence (even those > limit).
                
                // 1. Even Length -> Becomes Odd Length. Sum + 0 (No change in value).
                // We take all global even-parity sums and move them to valid odd-parity.
                next_dp[0][1] |= global_sums[0];
                
                // 2. Odd Length -> Becomes Even Length. Sum - 0 (No change in value).
                // We take all global odd-parity sums and move them to valid even-parity.
                next_dp[0][0] |= global_sums[1];
                
                // 3. Start a brand new subsequence [0]
                // Length is 1 (Odd). Sum is 0. Product is 0.
                next_dp[0][1].set(0 + OFFSET);
            } else {
                // If x > 0, we extend existing valid products.
                for (int p = 0; p <= limit; ++p) {
                    long long new_prod = (long long)p * x;
                    
                    // Optimization: Since p increases, once new_prod exceeds limit,
                    // larger p will also exceed limit. We can break.
                    if (new_prod > limit) break; 
                    
                    // Skip if there are no valid sums for this product/parity
                    if (dp[p][0].none() && dp[p][1].none()) continue;

                    // Extend Odd Parity (currently length 1, 3...):
                    // Next operation is Subtract x. New Parity is Even (0).
                    // Bitwise Right Shift handles subtraction.
                    if (dp[p][1].any()) {
                        next_dp[new_prod][0] |= (dp[p][1] >> x);
                    }

                    // Extend Even Parity (currently length 2, 4...):
                    // Next operation is Add x. New Parity is Odd (1).
                    // Bitwise Left Shift handles addition.
                    if (dp[p][0].any()) {
                        next_dp[new_prod][1] |= (dp[p][0] << x);
                    }
                }

                // Start a brand new subsequence [x]
                if (x <= limit) {
                    next_dp[x][1].set(x + OFFSET);
                }
            }

            // --- Phase 2: Update Global Sums (Ignoring Limit) ---
            // This ensures we track sums even if the product blew past the limit.
            
            // Extend Odd Parity -> Subtract x -> Becomes Even
            if (global_sums[1].any()) {
                next_global_sums[0] |= (global_sums[1] >> x);
            }
            // Extend Even Parity -> Add x -> Becomes Odd
            if (global_sums[0].any()) {
                next_global_sums[1] |= (global_sums[0] << x);
            }
            
            // Start new subsequence [x] in global tracker
            next_global_sums[1].set(x + OFFSET);

            // Apply the updates for this iteration
            dp = move(next_dp);
            global_sums = move(next_global_sums);
        }

        // --- Phase 3: Retrieve the Result ---
        // We want the maximum product, so we iterate backwards from limit down to 0.
        int target_idx = k + OFFSET;
        for (int p = limit; p >= 0; --p) {
            // Check if the target sum k exists in either parity for product p
            if (dp[p][0].test(target_idx) || dp[p][1].test(target_idx)) {
                return p;
            }
        }

        // If no subsequence formed the sum k, return -1.
        return -1;
    }
};

```

### Final Thoughts

This problem is a beautiful example of how changing your perspective on state management can solve seemingly impossible constraints. By rotating the problem and storing sums in bits rather than array indices, we turned a slow solution into a lightning fast one.

Handling the edge case of zero multiplication is the cherry on top. It reminds us that in algorithm design, we must always consider how a "reset" value (like multiplying by zero) can invalidate our greedy assumptions about discarding bad states.

I hope this breakdown helps you in your coding journey. 

Dealing with bitsets and dynamic programming can be intimidating at first, but with practice, they become powerful tools in your arsenal. 
