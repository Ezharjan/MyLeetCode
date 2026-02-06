# Cracking the Code of Massive Multiplications Using Priority Queues and Modular Arithmetic [#3266]

[Blog Post Link](https://leetcode.com/discuss/post/7553886/cracking-the-code-of-massive-multiplicat-qsmt/)


Hello there, fellow coding enthusiasts! Welcome back to another deep dive into the world of algorithmic problem solving. Today I am absolutely thrilled to share a fascinating journey I took while tackling a particularly challenging "Hard" problem involving arrays, massive simulation counts, and modular arithmetic.

If you have ever felt intimidated by problems that ask you to perform billions of operations efficiently, this guide is specifically for you. We are going to break down a complex optimization puzzle into bite sized pieces, ensuring that by the end of this read, you will feel confident managing heaps, modular exponentiation, and simulation shortcuts.

Let us dive right in!

## Understanding the Challenge

Before we write a single line of code, we must fully grasp what we are up against. Imagine you are given an array of integers called `nums`, a multiplier value, and a number `k`.

The rules of the game are quite specific. You must perform exactly `k` operations on this array. For every single operation, you need to follow this precise sequence:

1. Scan the entire array to find the minimum value.
2. If multiple elements share this minimum value, you must pick the one that appears first (the one with the smallest index).
3. Multiply this chosen value by the `multiplier`.
4. Update the array with this new value.

After you have laboriously repeated this `k` times, there is one final twist. You must return the array where every single element is taken modulo .

### The Scale of the Problem

Here is where things get tricky. The constraints are massive.

* The array size can be up to 10,000 elements.
* The value `k` can be up to  (one billion).
* The multiplier is at least 1.

If `k` were small, say 100 or 1000, we could simply write a loop and simulate the process. However, because `k` can be a billion, a simple simulation would run for far too long and result in a Time Limit Exceeded error. We need something smarter. We need a strategy that combines simulation with pure mathematics.

## The Naive Approach and Why It Fails

Your first instinct might be to use a Min Heap (priority queue). You could push all the elements into the heap, pop the smallest one, multiply it, and push it back. This handles the "find minimum" requirement efficiently.

The complexity for one operation would be logarithmic relative to the array size. Doing this `k` times results in a total complexity of roughly . With `k` equal to a billion, this is simply too slow for modern competitive programming environments, which typically expect solutions to run within a second or two.

We need to optimize this. We need to find a pattern.

## The Two Phase Strategy

The "Aha!" moment for this problem comes when you realize the process behaves differently in two distinct stages.

### Phase 1: The Catch Up Game

Initially, the array is likely "rugged" or uneven. You might have some very small numbers (like 2) and some very large numbers (like 1,000,000). When you multiply the smallest number, it grows larger. You keep picking the smallest numbers and growing them.

Eventually, the smallest numbers "catch up" to the larger numbers. The array becomes relatively flat. Specifically, there comes a point where the smallest value in the array, even after being multiplied, becomes larger than the maximum value currently in the array.

Once this happens, the behavior changes. The array is now "stable." This means that in the future, we will pick every element exactly once in a strictly cyclic order before picking any element a second time.

### Phase 2: The Mathematical Fast Forward

Once the array is stable, we do not need to simulate one by one anymore. If we have `n` elements and we need to perform `k` more operations, we know that every element will be multiplied roughly `k / n` times.

We can use fast modular exponentiation to calculate the final values instantly!

## Detailed Algorithm Walkthrough

Let us design our solution step by step.

### Step 1: Handling Edge Cases

First things first. If the `multiplier` is 1, the values in the array never change regardless of how large `k` is. In this case, we simply return the original array. This is a small but crucial check that saves us unnecessary computation.

### Step 2: Setting Up the Priority Queue

We need a Min Heap to efficiently track the smallest element. Since the problem requires us to pick the first index in case of a tie, we cannot just store the values. We must store a pair: `{value, index}`.

By default, C++ pairs compare the first element first. If the first elements are equal, it compares the second element. This perfectly aligns with our requirement to pick the smallest value, and then the smallest index for tie resolution.

We also need to keep track of the current maximum value in the array (`current_max`). This will help us determine when to switch from Phase 1 to Phase 2.

### Step 3: The Simulation Loop (Phase 1)

We enter a loop that continues as long as we still have operations left (`k > 0`).

Inside this loop, we peek at the smallest element in our heap. Let us call it `min_val`. We perform a critical check:
**Is `min_val` multiplied by `multiplier` greater than `current_max`?**

If the answer is **Yes**, it means our array has stabilized. Even if we multiply the current smallest number, it will jump past the largest number. This implies we have reached the point where the operations become cyclic. We can break out of the loop and use math.

If the answer is **No**, we perform the simulation manually:

1. Pop the smallest element.
2. Update its value by multiplying it.
3. Push it back into the heap.
4. Update `current_max` if this new value is the new maximum.
5. Decrement `k`.

*Note: Be very careful with integer overflow here. We should use `long long` types for our values, as they can grow quite large before we apply the modulo.*

### Step 4: The Mathematical Calculation (Phase 2)

When we exit the loop, we might still have a massive amount of `k` remaining. But now we know the order is predictable.

Suppose the array size is `n`.

1. **Full Cycles:** Each element in the heap will undergo `k / n` more multiplications.
2. **Partial Cycles:** The remaining `k % n` operations will apply to the smallest `k % n` elements currently in the heap. These specific elements will receive exactly one extra multiplication.

We can compute the multiplier factor using Modular Exponentiation.

* Let `cycles = k / n`
* Base factor = `pow(multiplier, cycles) % MOD`
* Extra factor = `pow(multiplier, cycles + 1) % MOD`

We iterate through our priority queue one last time. For each element we pop, we determine if it is one of the "lucky" ones that gets an extra multiplication (based on the remainder `k % n`) or if it just gets the standard `cycles` amount. We apply the math, take the modulo, and place the result into our final answer vector at the correct index.

## The C++ Solution

Here is the complete, optimal code implementing the logic above. I have added comments to guide you through every line.

```cpp
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
    // We define our modulo constant as specified in the problem
    long long MOD = 1e9 + 7;

    // This helper function performs Modular Exponentiation.
    // It calculates (base^exp) % MOD efficiently in O(log exp) time.
    long long fastPow(long long base, long long exp) {
        long long res = 1;
        base %= MOD;
        while (exp > 0) {
            // If the current bit of exp is 1, multiply the result by base
            if (exp % 2 == 1) res = (res * base) % MOD;
            // Square the base for the next bit
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }

public:
    vector<int> getFinalState(vector<int>& nums, int k, int multiplier) {
        // Edge Case: If the multiplier is 1, the array remains unchanged.
        if (multiplier == 1) return nums;

        int n = nums.size();
        
        // We use a Min Heap to store pairs of {value, index}.
        // The 'greater' comparator ensures the smallest pair is at the top.
        // C++ pairs compare the first element (value) then the second (index),
        // which automatically handles the tie resolution rule.
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        
        long long current_max = 0;

        // Initialize the heap and find the initial maximum value
        for (int i = 0; i < n; ++i) {
            pq.push({(long long)nums[i], i});
            current_max = max(current_max, (long long)nums[i]);
        }

        // Phase 1: Simulation
        // We simulate operations one by one until the array becomes "stable".
        // Stability is reached when the smallest element multiplied by the multiplier
        // exceeds the current maximum element.
        while (k > 0) {
            pair<long long, int> top = pq.top();
            
            // Optimization Check:
            // We verify if multiplying the current minimum would make it exceed current_max.
            // We use division (current_max / multiplier) to safely check without overflow.
            if (top.first > current_max / multiplier) { 
                break; 
            }
            
            pq.pop();
            
            // Perform the multiplication
            top.first *= multiplier;
            
            // Push the updated value back into the heap
            pq.push(top);
            
            // Update the global maximum tracker
            current_max = max(current_max, top.first);
            
            // Decrement the operation count
            k--;
        }

        // Phase 2: Mathematical Application
        // At this stage, the array is stable. The operations will follow a cyclic pattern.
        // We calculate how many full cycles remain and how many partial steps exist.
        long long cycles = k / n;
        int remaining = k % n;
        
        // Calculate the large multipliers using modular exponentiation
        long long pow_cycles = fastPow(multiplier, cycles);
        long long pow_cycles_plus_1 = (pow_cycles * multiplier) % MOD;

        vector<int> result(n);
        
        // The heap currently holds elements in the exact order they would be picked next.
        // The first 'remaining' elements in the heap are the ones that would have
        // been picked in the partial cycle, so they get one extra multiplication.
        while (!pq.empty()) {
            pair<long long, int> top = pq.top();
            pq.pop();
            
            // We perform calculations modulo 10^9 + 7
            long long val = top.first % MOD; 
            int idx = top.second;
            
            if (remaining > 0) {
                // This element is part of the partial cycle (gets extra mult)
                val = (val * pow_cycles_plus_1) % MOD;
                remaining--;
            } else {
                // This element only gets the base number of multiplications
                val = (val * pow_cycles) % MOD;
            }
            
            result[idx] = (int)val;
        }
        
        return result;
    }
};

```

## Why This Solution Works Efficiently

You might be wondering about the Time Complexity of this approach.

**Phase 1** runs relatively quickly. Because the values are multiplied exponentially, they grow very fast. The smallest value will catch up to the largest value in approximately  steps. Even with the constraints, this phase executes in a tiny fraction of a second.

**Phase 2** is dominated by the heap operations and the modular exponentiation. Emptying the heap takes , and the exponentiation takes .

This puts our total Time Complexity well within the acceptable limits for huge inputs. We essentially skip the vast majority of the billion operations by realizing they are just mathematical cycles.

## Final Thoughts and Key Takeaways

Solving problems like this is incredibly satisfying because it forces us to look past the brute force solution. We learned that:

1. **Simulation has limits.** When  is huge, we cannot just loop.
2. **Order emerges from chaos.** The array eventually stabilizes into a predictable cycle.
3. **Math is a powerful tool.** Modular exponentiation allows us to "jump" time and apply billions of operations in milliseconds.

I hope this breakdown helps you tackle similar problems in the future. Remember to always look for the pattern when the constraints seem impossible!

Happy coding, everyone!
