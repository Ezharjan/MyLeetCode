# Unlocking the Power of Bitwise Partitions and Linear Basis in C++ [#3630]

[Blog Post Link](https://leetcode.com/discuss/post/7561675/unlocking-the-power-of-bitwise-partition-dusq/)


Hello everyone! Welcome back to another deep dive into algorithmic problem solving. Today I want to share a fascinating problem that beautifully combines combinatorial search with linear algebra concepts. It is a perfect example of how breaking a complex problem into smaller, manageable pieces can lead us to a clean solution.

We are going to look at a problem where we need to split an array of numbers into three groups to maximize a specific score based on bitwise operations. It sounds simple on the surface, but the interplay between XOR and AND operations makes it a delightful puzzle to crack.

### Understanding the Puzzle

Let us start by looking at what the problem asks us to do.

We are given an array of integers called `nums`. We need to divide every element in this array into one of three buckets. Let us call these buckets Sequence A, Sequence B, and Sequence C. Every number from the original array must go into exactly one of these buckets.

Our goal is to organize these numbers to make the following value as large as possible

**Total Score = XOR sum of A + AND sum of B + XOR sum of C**

Here is a quick refresher on the terms

1. **XOR sum** This is the result of applying the bitwise XOR operation (the `^` operator) to all elements in the group. If the group is empty, this value is 0.
2. **AND sum** This is the result of applying the bitwise AND operation (the `&` operator) to all elements in the group. If the group is empty, this value is 0.

We need to return that maximum possible Total Score.

**A Quick Example**
Imagine our input array is `[1, 3, 2]`.
One way we could split this is

* **A** gets `[1]`. The XOR sum is 1.
* **B** gets `[2]`. The AND sum is 2.
* **C** gets `[3]`. The XOR sum is 3.

The total score would be `1 + 2 + 3 = 6`. It turns out 6 is the maximum we can get for this input.

### The Initial Challenge

When we first look at the constraints, we see that the length of the array `nums` is small, specifically up to 19 elements.

In competitive programming, a size of 19 usually hints at an exponential time complexity. We might instinctively think about trying every possible way to distribute the numbers. Since each number has 3 choices (A, B, or C), the total number of combinations is roughly 3 to the power of 19.

If we calculate 3 to the power of 19, we get roughly 1.1 billion. That is unfortunately too large for a standard time limit, which usually handles around 100 million operations per second. We need something smarter than a pure brute force approach.

### Breaking Down the Logic

To optimize this, we should look at the equation again.

`Score = XOR(A) + AND(B) + XOR(C)`

The terms `XOR(A)` and `XOR(C)` look very similar, while `AND(B)` behaves quite differently. The AND operation is reductive; adding more elements to a set usually makes the AND sum smaller or keeps it the same. The XOR operation is more chaotic; values can flip back and forth.

This observation leads us to a crucial strategy. What if we fix one of the sets first?

Since the array is small (N=19), iterating through all subsets for just **one** group is actually very fast. There are only 2 to the power of 19 possibilities, which is about 500,000. This is very manageable!

Let us iterate through every possible subset for **Sequence B**.
Once we pick a specific set of numbers for B, the value `AND(B)` is fixed and known.
The numbers that are NOT in B must now be distributed between **Sequence A** and **Sequence C**. Let us call this remaining pool of numbers `RemainingNums`.

Now our problem simplifies to this
Given a fixed `RemainingNums` pool, split it into A and C to maximize `XOR(A) + XOR(C)`.

### The Mathematical Insight

This is the part where things get interesting. We need to maximize the sum of two XOR values. Let us use some math to rewrite this sum.

We know a fundamental property of addition and XOR
**X + Y = (X XOR Y) + 2 * (X AND Y)**

Let us apply this to our problem variables
`XOR(A) + XOR(C) = (XOR(A) XOR XOR(C)) + 2 * (XOR(A) AND XOR(C))`

Notice the first term: `XOR(A) XOR XOR(C)`.
Since A and C together make up the `RemainingNums` pool, the XOR sum of all those numbers is just `XOR(A) ^ XOR(C)`. Let us call this value **M**.
**M** is a constant for the current iteration because it is simply the XOR sum of everything in `RemainingNums`. No matter how we shuffle elements between A and C, `XOR(A) ^ XOR(C)` will always equal **M**.

So, our equation becomes
`Score = AND(B) + M + 2 * (XOR(A) AND XOR(C))`

To maximize the total score, we need to maximize the term `XOR(A) AND XOR(C)`.

Recall that `XOR(C) = M ^ XOR(A)`.
So we are trying to maximize
**Val = XOR(A) AND (M XOR XOR(A))**

Let us look at this bit by bit. Suppose we are looking at the kth bit.

* **Case 1: The kth bit of M is 1.**
This means the total XOR sum has a 1 at this position. Consequently, the kth bit of `XOR(A)` and the kth bit of `XOR(C)` must be different (one is 0, one is 1).
Therefore, `XOR(A) AND XOR(C)` at this bit will be `0 AND 1` (or `1 AND 0`), which is **0**.
We cannot get any value from this bit position in the AND term.
* **Case 2: The kth bit of M is 0.**
This means the total XOR sum has a 0 at this position. Consequently, the kth bit of `XOR(A)` and the kth bit of `XOR(C)` must be the same (both 0 or both 1).
If we can force `XOR(A)` to have a 1 at this position, then `XOR(C)` will also have a 1.
Then `XOR(A) AND XOR(C)` will be `1 AND 1`, which is **1**.
This contributes to our score!

**Conclusion of the Math**
We want to choose a subset **A** from `RemainingNums` such that `XOR(A)` has 1s in as many positions as possible, **but only where M has 0s**.
Essentially, we want to maximize `XOR(A)` subject to a mask. The mask is the bitwise negation of M (`~M`).

### The Tool: Linear Basis

We have reduced the problem to finding a subset of numbers whose XOR sum is maximized under a specific mask. This is a classic scenario for a technique called **Linear Basis** (or Gaussian Elimination over GF(2)).

A Linear Basis is a small set of numbers (at most 30 to 60 for standard integer types) that can represent all possible XOR combinations of a larger array. By inserting our numbers into a basis, we can greedily ask "Can I make the current bit 1?" starting from the most significant bit.

In our specific case, we do not just insert the raw numbers. We care about the bits where M is zero. So, before inserting a number into our basis, we apply the mask `~M`. This focuses the Linear Basis on maximizing only the bits that actually contribute to the `2 * (XOR(A) AND XOR(C))` term.

### The Algorithm Summary

Here is the complete game plan

1. **Iterate through B**: Use a loop from 0 to `2^N` to represent every possible subset for sequence B. The loop variable acts as a bitmask.
2. **Calculate Constants**:
* Compute `AND(B)` for the current selection.
* Identify the `RemainingNums` (elements not in B).
* Compute `M`, which is the XOR sum of `RemainingNums`.
* Compute the `TargetMask`, which is `~M`.


3. **Build the Linear Basis**:
* For every number in `RemainingNums`, calculate `masked_value = number & TargetMask`.
* Insert this `masked_value` into a Linear Basis.


4. **Maximize**:
* Use the constructed basis to find the largest possible XOR value. Let us call this `MaxMaskedXor`.


5. **Compute Score**:
* The score for this specific B is `AND(B) + M + (MaxMaskedXor * 2)`.


6. **Update Global Maximum**: Keep track of the highest score found across all iterations.

### The Code Implementation

Here is how we can translate this logic into clean C++ code.

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    long long maximizeXorAndXor(vector<int>& nums) {
        int n = nums.size();
        long long total_xor_of_all = 0;
        
        // Precalculate XOR of the entire array
        for (int x : nums) {
            total_xor_of_all ^= x;
        }

        long long max_score = 0;
        
        // There are 2^N combinations for set B.
        // limit is 1 shifted left by N positions.
        int limit = 1 << n; 

        // We use a fixed size array for the Linear Basis
        // 32 is enough because numbers are up to 10^9
        int basis[32]; 

        // Iterate through every possible mask for B
        for (int mask = 0; mask < limit; ++mask) {
            long long current_and_b = -1; // -1 is all 1s in binary
            long long current_xor_b = 0;
            
            // 1. Determine the content of B and the Remaining set
            // calculating AND(B) and XOR(B)
            bool b_is_empty = true;
            
            for (int i = 0; i < n; ++i) {
                // If the ith bit is set in mask, nums[i] belongs to B
                if ((mask >> i) & 1) { 
                    int val = nums[i];
                    if (b_is_empty) {
                        current_and_b = val;
                        b_is_empty = false;
                    } else {
                        current_and_b &= val;
                    }
                    current_xor_b ^= val;
                }
            }
            
            if (b_is_empty) {
                current_and_b = 0;
            }

            // M is the XOR sum of elements in A and C.
            // XOR(A) ^ XOR(C) is equivalent to Total_XOR ^ XOR(B)
            long long M = total_xor_of_all ^ current_xor_b;
            
            // We want to maximize bits where M has 0s.
            // TargetMask helps us ignore bits where M has 1s.
            long long target_mask = ~M;

            // 2. Build the Linear Basis
            // Reset basis for the current iteration
            for(int k = 0; k < 32; ++k) basis[k] = 0;

            for (int i = 0; i < n; ++i) {
                // If the ith bit is NOT set in mask, nums[i] is in A or C
                if (!((mask >> i) & 1)) {
                    // Filter the value through the target mask
                    int val = nums[i] & target_mask;
                    
                    // Insert into Linear Basis
                    for (int k = 30; k >= 0; --k) {
                        if ((val >> k) & 1) {
                            if (!basis[k]) {
                                basis[k] = val;
                                break;
                            }
                            val ^= basis[k];
                        }
                    }
                }
            }

            // 3. Greedy maximization using the Basis
            long long best_xor_a = 0;
            for (int k = 30; k >= 0; --k) {
                if (!basis[k]) continue;
                // If XORing with the basis vector increases the value, do it
                if ((best_xor_a ^ basis[k]) > best_xor_a) {
                    best_xor_a ^= basis[k];
                }
            }

            // 4. Calculate total score
            // The term (best_xor_a * 2) represents 2 * (XOR(A) AND XOR(C))
            // The math: Score = AND(B) + M + 2 * (XOR(A) & XOR(C))
            // best_xor_a is effectively (XOR(A) & ~M) which is exactly (XOR(A) & XOR(C))
            long long current_score = current_and_b + M + (best_xor_a * 2);
            
            if (current_score > max_score) {
                max_score = current_score;
            }
        }
        
        return max_score;
    }
};

```

### Analysis and Constraints

Let us double check if this will run within the time limits.

* **Outer Loop**: Runs `2^N` times. With N=19, this is 524,288 iterations.
* **Inner Operations**: Inside the loop, we iterate through the array (size 19) to build the basis and perform constant time bitwise operations (looping 30 times for bits).
* **Total Complexity**: Roughly `O(2^N * N * 30)`.
`524,288 * 19 * 30` is approximately 300 million operations.

While 300 million is on the higher side, bitwise operations are extremely fast on modern CPUs, and the constant factor for the Linear Basis insertion is usually much smaller than 30 on average. This approach fits comfortably within standard C++ time limits (usually 1 to 2 seconds).

**Space Complexity** is minimal. We only use a small array for the basis and a few variables, so it is `O(1)` auxiliary space ignoring the input storage.

### Final Thoughts

This problem is a wonderful exercise in "fixing" one variable to simplify the rest. By fixing Set B, we transformed a confusing 3 partition problem into a simpler 2 partition problem that could be solved using XOR properties and Linear Basis.

Whenever you see a problem asking you to maximize XOR sums across subsets, always keep the Linear Basis technique in your mental toolbox. It is incredibly powerful for these types of bit manipulation challenges.

I hope this explanation helps you understand the mechanics behind the solution. Happy coding!
