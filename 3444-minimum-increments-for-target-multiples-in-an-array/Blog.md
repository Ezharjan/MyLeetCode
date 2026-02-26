# Mastering Array Increments to Satisfy Target Multiples [#3444]

Welcome everyone! Today I am very excited to share a comprehensive, highly detailed guide on a fascinating logic puzzle. We will walk through the concepts step by step, ensuring that learners of all levels can grasp the core ideas and build a solid foundation in algorithmic thinking. Let us explore a reliable method to approach this challenge, relying on fundamental computer science concepts.



### Introduction to the Challenge

In this puzzle, we are given two arrays of integers, named `nums` and `target`. Our primary goal is to ensure that every single element present in the `target` array has at least one multiple inside the `nums` array. 

To achieve this goal, we are allowed to perform a specific operation: we can increment any element in the `nums` array by exactly 1. We can perform this operation as many times as we need, on any element we choose. The ultimate objective is to figure out the absolute minimum total number of increments required across all elements to satisfy the condition for every target.

For instance, if our `nums` array has the value 8, and our `target` array requires multiples of 10 and 5, we can increment the 8 twice to reach 10. The number 10 is a multiple of both 5 and 10, completely satisfying our requirements with just 2 operations.

### Understanding the Core Constraints

Before we write any code or draft algorithms, we must carefully analyze the constraints provided by the problem. Often, the constraints contain massive hints about the expected solution. 

The most crucial detail provided is the size of the `target` array. The problem guarantees that the length of `target` will never exceed 4! This tiny number is our golden key. When a constraint is this small, it heavily implies that an exponential time complexity approach based on the size of that specific input is perfectly fine. Two raised to the power of four is only sixteen, and three raised to the power of four is just eighty one. This naturally leads us to think about a concept called bitmasking, which allows us to efficiently track combinations and subsets.

The `nums` array, on the other hand, can have up to fifty thousand elements. This means our algorithm needs to process the `nums` array somewhat linearly, doing a small amount of work for each element.

### Prerequisites: What is Bitmasking?

To track which targets we have satisfied, we can use a technique called bitmasking. A bitmask uses the binary representation of an integer to represent a subset of a larger collection. Since our target array has at most four elements, we only need four binary bits to represent any possible combination of those targets.

Let us map this out logically:
* The bit `1` at position 0 represents the first element in the target array.
* The bit `1` at position 1 represents the second element.
* The bit `1` at position 2 represents the third element.
* The bit `1` at position 3 represents the fourth element.



Using this system, the integer 0 (which is `0000` in binary) represents the empty set where no targets are satisfied. The integer 15 (which is `1111` in binary) represents the full set where all four targets are satisfied. The integer 3 (`0011` in binary) means we have satisfied the first and second targets. This elegant representation allows us to use fast bitwise operations to combine and check subsets.

### The Power of the Least Common Multiple

If we decide to increment a single number in our array to satisfy multiple targets at the same time, that newly incremented number must become a common multiple of all those targets. To keep our increments to an absolute minimum, we want to target the smallest possible common multiple. This is known mathematically as the Least Common Multiple, often abbreviated as LCM.

For example, imagine we want a single number to satisfy the targets 4 and 6. A number that is a multiple of both 4 and 6 must be a multiple of their LCM, which is 12. If our starting number is 8, the closest multiple of 12 is 12 itself. We would need to increment 8 four times to reach 12.

To calculate the cost of incrementing a number `x` to the next multiple of a given `LCM`, we can use modulo arithmetic. We first find the remainder by calculating `x % LCM`. 
* If the remainder is exactly zero, it means our number `x` is already a perfect multiple of the LCM. The cost is zero.
* If the remainder is greater than zero, we need to push our number `x` up to the next multiple. The number of increments required is the difference between the LCM and our remainder. 

In our code, we will need to calculate the difference without using the standard subtraction symbol to comply with strict formatting rules for this platform. We can creatively use bitwise inversion. Adding the bitwise NOT of a number and then adding one is mathematically identical to subtracting that number. So, adding `~rem + 1` is a brilliant workaround to find the difference!

### Building the Dynamic Programming Table

Dynamic Programming is a method for solving complex problems by breaking them down into simpler subproblems. We store the results of these subproblems to avoid redundant calculations. 

For our challenge, we will create a memory table called `dp`. The size of this table will be 16 to accommodate all possible bitmasks from `0000` to `1111`.
* The value stored at `dp[mask]` will represent the minimum total increments we have spent so far to satisfy the subset of targets defined by that specific `mask`.
* We start by initializing our table. We set `dp[0]` to 0 because satisfying zero targets requires zero operations. We fill all other spots in the table with an immensely large number to symbolize that those states are currently unreachable.

### Step by Step Execution Strategy

Now we orchestrate the entire process. We will look at every single number in our `nums` array one by one. 

For every number `x` in `nums`, we follow a strict sequence of actions:
1.  We create a temporary duplicate of our `dp` table. Let us call it `next_dp`. This ensures that the updates we make using the current number `x` do not interfere with the states we are currently reading from.
2.  We calculate the cost to make our current number `x` a multiple of the LCM for every possible subset of targets. We store these 16 costs in a small array.
3.  We iterate through our existing `dp` table. If we find a state that is reachable (meaning its cost is not our incredibly large infinity value), we examine it. Let us call the mask of this valid state `pmask`.
4.  We need to figure out which targets are still missing. We do this by applying a bitwise XOR between a full mask (all ones) and our `pmask`. This gives us a new mask representing the unfulfilled targets. Let us call this `rem_mask`.
5.  We must iterate over every possible submask of `rem_mask`. These submasks represent disjoint sets of targets that we can choose to satisfy using our current number `x`. To safely loop through these subsets, we update our loop variable by adding the bitwise NOT of zero (which effectively steps the value down by one) and then applying a bitwise AND with `rem_mask`.
6.  For each valid submask, we calculate the combined mask by applying a bitwise OR between `pmask` and the submask. We then update `next_dp` at this newly combined mask position. The new value will be the minimum of whatever is currently there versus the sum of the cost from `dp[pmask]` and the cost to satisfy the new submask using `x`.
7.  After processing all subsets for the current number `x`, we permanently replace our old `dp` table with `next_dp`.

Once we have processed every single number in the `nums` array, our final answer will be waiting for us in the `dp` table at the index representing the full mask (where all target bits are set to 1).

### Walkthrough of an Example Scenario

Let us trace a simple scenario to solidify our understanding. Suppose our `nums` array is `[8, 4]` and our `target` array is `[10, 5]`.

We have two targets, so our bitmasks will range from 0 to 3.
* Mask 0: Empty set
* Mask 1: Target 10 (LCM is 10)
* Mask 2: Target 5 (LCM is 5)
* Mask 3: Targets 10 and 5 (LCM is 10)

Initially, `dp[0]` is 0, and the rest are infinity.

First, we process the number 8:
* Cost to satisfy Mask 1: The next multiple of 10 after 8 is 10. Cost is 2.
* Cost to satisfy Mask 2: The next multiple of 5 after 8 is 10. Cost is 2.
* Cost to satisfy Mask 3: The next multiple of 10 after 8 is 10. Cost is 2.
* We update our table. `dp[1]`, `dp[2]`, and `dp[3]` all become 2.

Next, we process the number 4:
* Cost to satisfy Mask 1: The next multiple of 10 after 4 is 10. Cost is 6.
* Cost to satisfy Mask 2: The next multiple of 5 after 4 is 5. Cost is 1.
* Cost to satisfy Mask 3: The next multiple of 10 after 4 is 10. Cost is 6.
* We try to combine states. We see that `dp[1]` has a cost of 2 (meaning target 10 is satisfied). We can satisfy Mask 2 (target 5) using our current number 4 at a cost of 1. The combined mask is 3. The combined cost is 2 plus 1, which equals 3. 
* However, `dp[3]` already holds a value of 2 from our previous step! Since 2 is smaller than 3, we keep the existing smaller value.

Our loop finishes, and we look at `dp[3]` for our answer, which is 2. This perfectly matches the logic that incrementing the 8 twice is the most optimal path.

### Analyzing Time and Space Complexity

Understanding the efficiency of our algorithm is a core engineering practice. 

The Time Complexity is extremely favorable. We iterate through every element in the `nums` array exactly once. For each element, we loop through all possible subsets and their submasks. Mathematically, the total number of operations per element is bounded by three raised to the power of the size of the target array. Let N be the size of `nums` and M be the size of `target`. The time complexity is exactly O(N multiplied by 3 to the power of M). Since M is at most 4, three to the power of four is 81. Therefore, our algorithm performs at most 81 operations per number in the array. This translates to an incredibly fast execution well within any competitive programming time limits.

The Space Complexity is also remarkably small. We only need memory to store the LCM values and our Dynamic Programming table. The size of these arrays is exactly two raised to the power of M. With M being at most 4, the arrays only hold 16 elements. This requires an essentially constant amount of memory, resulting in a space complexity of O(2 to the power of M).

### Clean and Documented Code Implementation

Below is the complete C++ implementation. The code has been meticulously crafted to be highly accurate and easy to read. Note the implementation of the greatest common divisor and least common multiple functions directly within the class to ensure broad compatibility without relying heavily on newer standard libraries.

```cpp
#include <vector>
#include <algorithm>
#include <utility>

using std::vector;

class Solution {
private:
    // Helper function to find the greatest common divisor
    long long calculateGCD(long long a, long long b) {
        while (b > 0) {
            long long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Helper function to find the least common multiple
    long long calculateLCM(long long a, long long b) {
        if (a == 0 || b == 0) return 0;
        return (a / calculateGCD(a, b)) * b;
    }

public:
    int minimumIncrements(vector<int>& nums, vector<int>& target) {
        int m = target.size();
        
        // maximum possible state size
        int max_mask = 1 << m; 
        
        // This generates a mask with all target bits set to 1
        // We use bitwise shifts and inversions to build the mask
        int full_mask = ~(~0 << m); 
        
        // Precompute the LCM for every subset mask of the target array
        vector<long long> L(max_mask, 1);
        for (int mask = 1; mask < max_mask; ++mask) {
            long long current_lcm = 1;
            for (int i = 0; i < m; ++i) {
                if ((mask >> i) & 1) {
                    current_lcm = calculateLCM(current_lcm, (long long)target[i]);
                }
            }
            L[mask] = current_lcm;
        }
        
        // Use a safe maximum value for infinity to prevent overflow issues
        const long long INF = 1000000000000000000LL;
        vector<long long> dp(max_mask, INF);
        
        // Satisfying zero targets costs zero operations
        dp[0] = 0; 
        
        long long cost[16];
        
        // Iterate over every number in the given array
        for (int x : nums) {
            vector<long long> next_dp = dp;
            
            // Calculate the cost to satisfy each subset with the current number
            for (int mask = 1; mask < max_mask; ++mask) {
                long long rem = x % L[mask];
                
                // If remainder is zero, cost is zero.
                // Otherwise, calculate difference using bitwise inversion
                cost[mask] = (rem == 0) ? 0 : L[mask] + (~rem + 1);
            }
            
            // Iterate through existing DP states to map out transitions
            for (int pmask = 0; pmask < max_mask; ++pmask) {
                if (dp[pmask] == INF) continue;
                
                // Get the bitmask of targets we still need to satisfy
                int rem_mask = full_mask ^ pmask;
                
                // Iterate exclusively over all disjoint subsets of remaining targets
                // We update the submask by adding the bitwise NOT of zero
                for (int sub = rem_mask; sub > 0; sub = (sub + ~0) & rem_mask) {
                    
                    // Check if combining the current state with the new subset is cheaper
                    if (dp[pmask] + cost[sub] < next_dp[pmask | sub]) {
                        next_dp[pmask | sub] = dp[pmask] + cost[sub];
                    }
                }
            }
            
            // Move the updated states back to our main memory table
            dp = std::move(next_dp);
        }
        
        // Return the minimum increments needed for all elements in the target array
        return static_cast<int>(dp[full_mask]);
    }
};

```

I hope this thorough walkthrough brings clarity to your learning journey! Breaking down problems into their fundamental components, like bitmasking and dynamic programming states, is a highly rewarding experience that builds robust problem solving skills. Keep practicing, and happy coding to you all!
