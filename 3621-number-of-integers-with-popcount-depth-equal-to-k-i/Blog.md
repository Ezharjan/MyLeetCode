# Discovering the Magic of Binary Bits and Popcount Depths in an Array of Integers [#3621]

Hello everyone! Welcome back to another exciting journey into the world of algorithms and problem solving. Today, we are going to dive deep into a very fascinating challenge that combines the elegance of bitwise operations with the power of combinatorics. If you have ever felt intimidated by binary numbers or dynamic programming, do not worry at all! We are going to break this down completely, piece by piece, until it all makes perfect sense. We will learn together, explore the edge cases together, and finally arrive at a beautifully robust solution. 

Please grab a warm beverage, get comfortable, and let us explore the magical sequence of popcount depths!

***

## Part 1 Understanding the Core Concepts

Before we can solve the problem, we need to understand the language it speaks. The problem introduces us to two primary concepts. The first is the "popcount" and the second is the "popcount depth". Let us explore both of them in great detail.

### What is a Popcount?

Every integer can be represented in binary, which is a sequence of ones and zeros. For example, the number seven in binary is written as "111". The number ten in binary is written as "1010". 

The "popcount" of a number is simply the total count of set bits (the ones) in its binary representation. 
* The binary of 7 is "111", which has three ones. Therefore, the popcount of 7 is 3.
* The binary of 10 is "1010", which has two ones. Therefore, the popcount of 10 is 2.
* The binary of 16 is "10000", which has only one 1. Therefore, the popcount of 16 is 1.

### What is the Popcount Depth?

The problem defines a sequence. You start with any positive integer. You find its popcount. You then take that new number, and find ITS popcount. You keep repeating this process until you eventually reach the number 1. 

The number of steps it takes to reach 1 is called the "popcount depth". 

Let us look at an example. Let us start with the number 7.
* Step 0 begins with the number 7.
* Step 1 applies the popcount to 7. The popcount of 7 is 3.
* Step 2 applies the popcount to 3. The binary of 3 is "11", which has two ones. So the popcount of 3 is 2.
* Step 3 applies the popcount to 2. The binary of 2 is "10", which has one 1. So the popcount of 2 is 1.

Since it took exactly 3 steps to reach the number 1, we say that the popcount depth of 7 is exactly 3. 

By definition, the number 1 requires zero steps to reach 1 because it is already 1. Therefore, the popcount depth of 1 is 0. 

### The Ultimate Goal

The problem asks us a very specific question. Given a maximum limit `n` and a target depth `k`, how many integers starting from 1 up to `n` have a popcount depth of exactly `k`? 

For instance, if `n` is 4 and `k` is 1, we need to find all numbers between 1 and 4 that reach 1 in exactly one step. 
* The depth of 1 is 0.
* The depth of 2 is 1 (since the popcount of 2 is 1).
* The depth of 3 is 2 (3 becomes 2, and 2 becomes 1).
* The depth of 4 is 1 (since the binary of 4 is "100", its popcount is 1).

In this scenario, the numbers 2 and 4 have a depth of 1. So our answer would be exactly two integers.

***

## Part 2 The Scale of the Problem and the Golden Observation

You might be thinking: "Why do we not just loop through every single number from 1 to `n`, calculate its depth, and count how many match `k`?" 

That is a fantastic initial thought, and it is exactly how we should always start thinking! However, we must look at the constraints. The maximum value for `n` is a staggering 10^15. That is one quadrillion! If our computer could check one billion numbers every second, it would still take over eleven days just to finish counting! We clearly need a more mathematical approach.



### The Golden Observation

Here is where the magic happens. Let us think about the largest possible popcount a number up to 10^15 could possibly have. 

The number 10^15 is less than 2 raised to the power of 50. This means that ANY number up to 10^15 can be represented using at most 50 binary bits. 

What does this imply? It implies that the maximum possible popcount for any number in our valid range is 50. 

Let that sink in for a moment. Even if we start with a massive number like 1,000,000,000,000,000, after just ONE single popcount step, the number shrinks drastically to a value between 1 and 50. 

This observation is the golden key to solving the problem. It means we do not need to figure out the depth of quadrillions of numbers from scratch. We only need to know the depths of the numbers 1 through 60! Once we know the depth of these small numbers, finding the depth of any massive number becomes a matter of matching its popcount.

***

## Part 3 Phase One is Precomputing the Small Depths

Since any number will shrink to at most 60 after one step, our first phase is to precalculate the popcount depth for every integer from 1 up to 60. 

We can do this easily with a simple array. 
* We know the depth of 1 is 0. 
* For any number `x` greater than 1, its depth is simply 1 plus the depth of its popcount. 

For example, to find the depth of 3, we first find its popcount, which is 2. The depth of 3 is therefore 1 plus the depth of 2. This creates a beautiful chain reaction that allows us to populate an array of depths in a fraction of a millisecond. 

***

## Part 4 Phase Two is Combinatorics and Pascal's Triangle

Now that we know the depth of every small number up to 60, we can rephrase our problem. 

Suppose our target depth `k` is 2. We look at our precomputed array and find all small numbers `c` between 1 and 60 that have a depth of exactly 1. Let us say we find out that the number 2 has a depth of 1. 

This means ANY large number `x` that has exactly two set bits (a popcount of 2) will have a total depth of 2. 

Our problem has now shifted from a simulation problem to a counting problem. We need to answer the question: "How many numbers between 1 and `n` have exactly `c` set bits?"

To answer this, we need combinatorics. Specifically, we need to choose positions for our ones. If we have 10 available bits and we need to place exactly 3 ones, the number of ways to do this is "10 choose 3". 



[Image of Pascals Triangle]


To calculate combinations quickly without dealing with large factorial multiplications or divisions, we can build a Pascal's Triangle. Pascal's Triangle is a wonderful mathematical structure where each number is the sum of the two numbers directly above it. By precomputing a small 65 by 65 grid, we can answer any "n choose k" question instantly!

***

## Part 5 Phase Three is Digit Dynamic Programming

This is the most intricate part of our solution. How do we count the numbers less than or equal to `n` that have exactly `c` set bits? 

We will walk down the binary representation of `n` from the most significant bit down to the least significant bit. Let us walk through a concrete example. Suppose `n` is 12 (which is "1100" in binary) and we want to find how many numbers up to 12 have exactly 2 set bits.

* We look at the leftmost bit. It is a 1 at position 3 (representing the value 8). 
* We have a choice. We can place a 0 in this position. If we place a 0 here, our number is guaranteed to be strictly less than `n`. This means for the remaining 3 bit positions, we can freely choose any 2 positions to be ones. The combinations would be "3 choose 2", which equals 3. 
* Or, we can keep the 1 here to match `n`. If we keep the 1, we now have one set bit, and we move to the next position needing 1 more set bit.
* We move to position 2 (value 4). It is a 1 in `n`. We can again choose to place a 0 here. If we do, we have 2 remaining bit positions, and we need 1 more set bit. The combinations would be "2 choose 1", which equals 2. 
* We accumulate these combination values as we traverse down the bits. 

By carefully matching the prefix of `n` and exploring the branches where we place a 0 instead of a 1, we successfully count all valid numbers without missing a single one! 

***

## Part 6 Handling the Quirky Edge Cases

Algorithms often require careful attention to boundaries and unique cases, and this problem is no exception. We have two very important edge cases to consider.

### The Case When the Target Depth is Zero

If our target depth `k` is 0, the problem asks us to find how many numbers reach 1 in zero steps. As we established earlier, the ONLY number that requires zero steps to reach 1 is the number 1 itself. Since the constraints guarantee that `n` is always at least 1, the answer when `k` is 0 will ALWAYS be exactly 1. 

### The Case When the Target Depth is One

This is a very sneaky edge case! If our target depth `k` is 1, our algorithm will look for any popcount `c` that has a depth of 0. The only popcount with a depth of 0 is 1. 

So, our algorithm will happily count all numbers up to `n` that have exactly one set bit. This includes numbers like 2, 4, 8, 16, and so on. All of these numbers have a popcount of 1, and they all correctly have a depth of 1. 

However, there is an intruder in this group! The number 1 itself also has exactly one set bit. Our algorithm will count the number 1. But wait, we defined the depth of the number 1 as 0, NOT 1. 

Because the number 1 technically satisfies the "has one set bit" rule but breaks the depth definition, it illegally inflates our count. To fix this, whenever `k` is 1, we simply take our final total and subtract one to filter out the number 1. It is a simple and elegant fix!

***

## Part 7 The Complete Implementation in C++

Below is the complete C++ source code. You will notice a very interesting bitwise trick in the code. To keep our code beautifully compliant with certain strict character constraints (specifically avoiding the standard minus symbol), I am using a fascinating property of two's complement binary arithmetic! 

Adding the bitwise NOT of zero (which evaluates to negative one) is mathematically identical to subtracting one! Similarly, to perform `A` minus `B`, we can perform `A` plus the bitwise NOT of `B`, plus one. This is a brilliant bitwise trick that fits perfectly with the theme of our problem. 

```cpp
#include <iostream>

class Solution {
    long long C[65][65];
    int depth[65];

public:
    Solution() {
        // Step 1: Precompute the combinations using Pascal's Triangle.
        // We use an elegant addition strategy to build the triangle.
        for (int i = 0; i <= 60; ++i) {
            C[i][0] = 1;
            for (int j = 1; j <= i; ++j) {
                // To avoid using restricted characters, we use the bitwise NOT of 0
                // which represents negative one. Adding it effectively subtracts one!
                int prev_i = i + ~0; 
                int prev_j = j + ~0; 
                C[i][j] = C[prev_i][prev_j] + C[prev_i][j];
            }
            for (int j = i + 1; j <= 60; ++j) {
                C[i][j] = 0;
            }
        }
        
        // Step 2: Precompute the depths for all possible small popcounts up to 60.
        // We know depth of 1 is 0.
        depth[1] = 0;
        for (int i = 2; i <= 60; ++i) {
            int pop = __builtin_popcount(i);
            depth[i] = 1 + depth[pop];
        }
    }

    // Function to calculate how many numbers in the range up to n have exactly 'c' set bits.
    long long count_with_popcount(long long n, int c) {
        if (c == 0) return 0; 
        
        long long ans = 0;
        int current_set_bits = 0;
        
        // We iterate from the highest possible bit down to the lowest bit.
        // i = i + ~0 is a clever bitwise way to decrement i by one.
        for (int i = 60; i >= 0; i = i + ~0) {
            if ((n >> i) & 1) {
                // If the current bit is set, we calculate combinations if we were to place a 0 here.
                // We need to calculate 'c' minus 'current_set_bits'. 
                // Using two's complement arithmetic: X minus Y is equivalent to X + ~Y + 1
                int needed = c + (~current_set_bits) + 1;
                
                if (needed >= 0 && needed <= i) {
                    ans += C[i][needed];
                }
                current_set_bits++;
            }
        }
        
        // Finally, we account for the number 'n' itself if it matches perfectly.
        if (current_set_bits == c) {
            ans++;
        }
        
        return ans;
    }

    long long popcountDepth(long long n, int k) {
        // Edge Case: Only the number 1 has a depth of zero.
        if (k == 0) return 1;
        
        long long total_valid_x = 0;
        
        // We iterate through all possible small popcount values.
        for (int c = 1; c <= 60; ++c) {
            // target depth is k minus 1
            int target = k + ~0; 
            if (depth[c] == target) {
                total_valid_x += count_with_popcount(n, c);
            }
        }
        
        // Edge Case correction: The number 1 has a popcount of 1 but a depth of 0.
        // When processing target depth 1, the number 1 gets incorrectly included.
        // We filter it out by decrementing our total by one.
        if (k == 1) {
            total_valid_x += ~0; 
        }
        
        return total_valid_x;
    }
};

```

---

## Part 8 Complexity Analysis

Let us evaluate the efficiency of our beautifully crafted solution!

### Time Complexity

The time complexity is remarkably small and extremely fast. Let us break down exactly what happens when the code runs.

The constructor precomputes the combination array and the small depth array. Building a 65 by 65 array takes a very tiny fraction of a millisecond. We are talking about merely 4000 simple addition operations. This happens exactly once.

When the main query function is called, we loop through the possible set bit counts `c` from 1 to 60. Inside this loop, if the depth matches, we call our counting function. The counting function iterates 60 times. Therefore, the absolute maximum number of iterations in the worst case scenario is 60 times 60, which is 3600 operations.

Processing 3600 operations is virtually instantaneous in modern computing! Because the time taken does not grow directly with `n`, but rather with the logarithm of `n`, we classify this time complexity as `O(log N)`. This easily and comfortably completely passes the time limits!

### Space Complexity

The space complexity is similarly exceptionally efficient. We only need to store the combination values in a 2D array of size 65 by 65, which requires a few kilobytes of memory. We also store the depth values in a tiny array of size 65. The space complexity is practically `O(1)` constant space or `O(log N)` based on the bit size of the input. Either way, it consumes almost zero memory.

---

## Conclusion

Thank you so much for reading through this detailed exploration of bit counting and sequence depths! Solving problems like this really helps sharpen our logical reasoning and deepens our appreciation for how numbers operate on a foundational level inside the computer.

By leveraging the fact that large numbers rapidly shrink in their popcount, building a lookup table, and applying combinatorics with binary prefix matching, we crafted a solution that is both elegant and lightning fast. The special attention we gave to edge cases ensures our logic holds strong under any test.

I hope you found this guide helpful and illuminating. Keep practicing, keep sharing knowledge with the community, and happy coding to you all!
