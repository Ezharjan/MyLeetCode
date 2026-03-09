# Unraveling the Magic of Array Permutations and Inversion Counting A Friendly Guide [#3193]

Hello everyone! Welcome back to another coding adventure. Today I want to share my thoughts and a complete walkthrough on a beautiful algorithm challenge that revolves around permutations and inversion counting. We will be looking at how to count the number of valid permutations of an integer array based on very specific inversion requirements at various lengths. 

I really enjoyed working through this one, and I want to share a comprehensive, highly detailed guide with the community. Whether you are new to dynamic programming or looking to refine your array manipulation skills, I hope this breakdown helps you understand the underlying concepts clearly. Let us dive right into the details!

### Understanding the Problem

Imagine you are given an integer n. Your goal is to construct permutations of the numbers from zero up to n minus one. However, this is not just about finding any permutation. We are given a two dimensional array of requirements. Each requirement gives us an end index and a target inversion count. 

For a permutation to be considered valid, its prefix ending at that specific index must contain exactly the target number of inversions. Finally, because the total number of valid permutations can be incredibly large, we need to return our final answer modulo 1000000007.

Before we write any code or formulate an algorithm, we need to completely understand the two fundamental pillars of this problem: permutations and inversions.

### What Exactly is an Inversion?

Let us take a moment to define an inversion. In the context of an array of numbers, an inversion occurs when a larger number appears before a smaller number. 

Formally, a pair of indices (i, j) forms an inversion if:
* i is strictly less than j
* The number at index i is strictly greater than the number at index j

Let us look at a small example. Suppose we have the array [2, 0, 1].
* We look at the first element, which is 2. The numbers appearing after 2 are 0 and 1. Since 2 is greater than 0, that is one inversion. Since 2 is also greater than 1, that is a second inversion.
* We look at the second element, which is 0. The only number after it is 1. Since 0 is not greater than 1, this does not form an inversion.
* The total number of inversions for the array [2, 0, 1] is exactly two.

When the problem mentions that a prefix must have a certain number of inversions, it means we only consider the subarray starting from index zero up to that specific end index, and we count the inversions only within that isolated subarray.

### Building Permutations Step by Step

To solve this problem effectively, we need to think about how permutations are built and how adding a new number affects the total inversion count. 

Imagine we are building a permutation of the numbers 0, 1, 2, and 3. We can build it incrementally. 
First, we start with just the number 0. The array is [0]. It has zero inversions.
Next, we add the number 1. Because 1 is the largest number so far, its placement directly dictates how many new inversions we create!

Let us see what happens when we place the new largest number into an existing array. Suppose our current array is [0, 1]. It currently has zero inversions. Now we want to insert the number 2.
* If we place 2 at the very end, we get [0, 1, 2]. The number 2 is larger than everything before it, so it creates zero new inversions. The total remains zero.
* If we place 2 one spot from the end, we get [0, 2, 1]. The number 2 is placed before the number 1. Since 2 is greater than 1, this creates exactly one new inversion. The total becomes one.
* If we place 2 at the very front, we get [2, 0, 1]. The number 2 is placed before both 0 and 1. This creates exactly two new inversions. The total becomes two.

This is the golden rule for this problem. When we are building a permutation of length L, and we insert the largest number we have seen so far, the number of new inversions we add is strictly determined by how far away from the end we place it. We can add anywhere from 0 up to L minus one new inversions.

### Formulating the Dynamic Programming Strategy

Now that we know how inversions grow predictably, we can use Dynamic Programming to keep track of the number of valid permutations. We want to avoid generating every single permutation, as that would take far too much time and memory.

Let us define our state. We will use a two dimensional array named dp. 
The value at dp[length][inv] will represent the total number of valid permutations of the first 'length' elements that contain exactly 'inv' inversions.

How do we calculate dp[length][inv]? 
We know that to reach a state with 'inv' inversions at the current length, we must have transitioned from a state at the previous length that had fewer inversions. Specifically, if we place our new largest element in a way that creates 'k' new inversions, the previous state must have had 'inv' minus 'k' inversions.

So, the number of ways to form a prefix of size 'length' with 'inv' inversions is the sum of the ways to form a prefix of size 'length' minus one, having 'inv' minus 'k' inversions. We sum this up for all possible values of 'k', where 'k' can be anything from 0 up to 'length' minus one.

### Integrating the Requirements Array

This is where the unique twist of the problem comes into play. We are given a list of requirements. 

We can map these requirements to an easy lookup table. Let us call it req_array. If there is a requirement that the prefix of length L must have exactly C inversions, we store req_array[L] = C. If there is no requirement for a specific length, we can store negative one to signify that any number of inversions is acceptable.

While we are filling out our dynamic programming table, we add a simple check. If we are calculating the states for a specific length, we look at our req_array. If there is a requirement for this length, we simply force all dp states that do not match the required inversion count to become zero. 

By zeroing out the invalid states, we guarantee that only permutations meeting all the necessary prefix criteria continue to propagate forward in our calculations!

### A Complete Walkthrough

Let us dry run the logic with a small example to solidify our understanding.
Suppose our integer n is 3. 
Our requirements are that at index 2 (length 3), we need exactly 2 inversions. At index 0 (length 1), we need exactly 0 inversions.

1. Initialization: 
We start with length 1. The only element is 0. A single element array always has 0 inversions. 
So, dp[1][0] is 1. All other inversion counts for length 1 are 0.
We check our requirements for length 1. It requires 0 inversions. Our state matches this perfectly.

2. Moving to length 2:
We want to calculate dp[2][inv]. The new element can add 0 or 1 new inversions.
* For 0 total inversions: dp[2][0] comes from dp[1][0] (adding 0 new inversions). So dp[2][0] becomes 1.
* For 1 total inversion: dp[2][1] comes from dp[1][0] (adding 1 new inversion). So dp[2][1] becomes 1.
We check our requirements for length 2. There are no requirements, so we keep both values.

3. Moving to length 3:
We want to calculate dp[3][inv]. The new element can add 0, 1, or 2 new inversions.
* We compute the sums based on the previous states from length 2.
* However, our requirements state that for length 3, we MUST have exactly 2 inversions.
* Therefore, we only care about calculating dp[3][2]. Any other value like dp[3][0] or dp[3][1] is automatically set to zero.
* Where can dp[3][2] come from? 
  It can come from dp[2][2] plus 0 new inversions (but dp[2][2] is 0).
  It can come from dp[2][1] plus 1 new inversion (dp[2][1] is 1, so we add 1).
  It can come from dp[2][0] plus 2 new inversions (dp[2][0] is 1, so we add 1).
* Total for dp[3][2] becomes 2.

The final answer is 2, which matches the problem description perfectly!

### The Code Implementation

Below is the C++ implementation based purely on the logic we just discussed. I have included detailed comments throughout the code to explain each step. I hope you find the structure clean and easy to read.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int numberOfPermutations(int n, vector<vector<int>>& requirements) {
        // We need to return our answer modulo 1000000007
        const int MOD = 1e9 + 7;
        
        // Based on the constraints, the maximum possible inversions is 400
        const int MAX_INV = 400; 
        
        // Create an array to look up requirements quickly.
        // req[i] will store the exact number of inversions required at prefix index i.
        // We initialize it with a negative number to represent 'no requirement'.
        vector<int> req(n, -1);
        for (const auto& r : requirements) {
            req[r[0]] = r[1];
        }
        
        // If the very first number is required to have more than 0 inversions, 
        // it is impossible, because a single number cannot be inverted with anything.
        if (req[0] > 0) {
            return 0;
        }
        
        // This is our main two dimensional dynamic programming table.
        // dp[i][j] represents the number of valid arrangements of length i+1 having j inversions.
        vector<vector<long long>> dp(n, vector<long long>(MAX_INV + 1, 0));
        
        // Base case: A prefix of length 1 (index 0) has 0 inversions.
        dp[0][0] = 1;
        
        // We build our permutations length by length, starting from length 2 (index 1) up to length n.
        for (int i = 1; i < n; ++i) {
            
            // For the current length, we calculate the number of ways to achieve every possible inversion count.
            for (int j = 0; j <= MAX_INV; ++j) {
                
                // If the current prefix length has a requirement, and our current inversion count 'j' 
                // does not match it, we mark this state as invalid (0 ways) and skip further calculations.
                if (req[i] != -1 && req[i] != j) {
                    dp[i][j] = 0;
                    continue;
                }
                
                long long current_sum = 0;
                
                // The new number can be inserted to create 'k' new inversions.
                // 'k' can range from 0 up to 'i' (the number of existing elements).
                // We also must ensure we do not look for a negative inversion count, so k <= j.
                for (int k = 0; k <= min(i, j); ++k) {
                    current_sum = (current_sum + dp[i - 1][j - k]) % MOD;
                }
                
                // Store the calculated sum in our table
                dp[i][j] = current_sum;
            }
        }
        
        // The final answer is the number of valid permutations of the full array length 'n'
        // matching the specific requirement for the final index 'n minus one'.
        return dp[n - 1][req[n - 1]];
    }
};

```

### Analyzing the Complexity

When implementing algorithms, it is always a good practice to evaluate the time and space complexity to ensure our solution runs within the designated limits. Let us break it down clearly.

Time Complexity
We have three nested loops in our solution.
The outer loop runs n times, representing the length of the prefix we are currently building.
The middle loop runs for every possible inversion count. According to the problem constraints, this is at most 400.
The innermost loop represents the new inversions we can add. This runs at most n times.
Combining these together, the total number of operations is roughly proportional to n times max inversions times n. Given that n is at most 300, and max inversions is 400, the total operations are around 36 million. This is very efficient and will comfortably pass the time limits in modern competitive programming environments.

Space Complexity
We are storing the results of our subproblems in a two dimensional array.
The dimensions of this array are n by the maximum number of inversions.
Since n is up to 300 and the max inversions is 400, our memory footprint requires storing around 120,000 integers. This uses an incredibly small amount of memory, easily fitting within standard memory constraints.

### Closing Thoughts

I find problems involving permutation building incredibly rewarding. They teach us to look past the overwhelming number of possible combinations and focus entirely on the incremental transitions. By asking ourselves "What happens when I add just one more element?", we transform an impossible combinatorial explosion into an elegant dynamic programming sequence.

I hope you found this guide helpful and easy to follow! Please feel free to study the transitions and try modifying the code or drawing out the dynamic programming table on paper for a small test case. Visualization is often the best way to master these concepts.

Happy coding, and see you in the next challenge!
