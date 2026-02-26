# Demystifying the Stable Binary Arrays Puzzle [#3130]

Hello everyone! Welcome to another problem solving journey. Today we are diving into an incredibly interesting algorithmic challenge involving binary arrays. We will explore how to construct sequences of zeros and ones under some very specific and intriguing rules. 

I am excited to share a dynamic programming approach that helped me understand this logic clearly. This guide is built to be a friendly and thorough resource for anyone looking to grasp the underlying concepts without getting lost in overwhelming mathematical notation. We will take it step by step, or rather, piece by piece, to ensure everything makes perfect sense.

### Understanding the Problem Requirements

Let us first establish what we are trying to achieve. You are given three positive numbers. These are **zero**, **one**, and **limit**. 

Our goal is to figure out the total number of ways we can build a valid array. But what makes an array valid? A valid array, which the problem refers to as a stable binary array, must perfectly satisfy three conditions.

First, the array must contain exactly the amount of zeros specified by the **zero** variable. Not one more, not one less. 

Second, the array must contain exactly the amount of ones specified by the **one** variable. 

Third, and this is the core challenge of our puzzle, we cannot have too many identical numbers grouped together. Specifically, any contiguous block of identical numbers cannot exceed the **limit** variable. If our limit is two, we can have two zeros in a row, but three zeros in a row would immediately make the entire array invalid.



Because the total number of valid arrays can grow to be exceptionally large, the problem asks us to return our final answer modulo 1000000007. This keeps our numbers within standard integer bounds during calculation.

### Why Brute Force Will Not Work

When facing array construction problems, our first instinct might be to generate all possible permutations of zeros and ones and then check each one to see if it violates the limit rule. 

However, think about the constraints. The number of zeros and ones can each go up to 1000. Generating permutations for an array of size 2000 is computationally impossible within standard time limits. The number of combinations would be astronomical. We need a method that counts valid arrangements without actually building them one by one. This is exactly where Dynamic Programming shines.

### The Dynamic Programming Blueprint

Dynamic programming is essentially about breaking a large, complex problem down into smaller, manageable pieces. We solve the small pieces, store their answers, and use them to build up to the final solution.

To count our arrays, we need to track three crucial pieces of information as we build them.
1. How many zeros we have used so far.
2. How many ones we have used so far.
3. The very last number we placed (whether it was a zero or a one). 

Tracking the last placed number is essential because it tells us what contiguous block we are currently extending. 

We can set up a three dimensional grid, often called a DP table. Let us conceptualize it as **dp[i][j][last_digit]**. 
Here, **i** represents the number of zeros used. The **j** represents the number of ones used. The **last_digit** will be either 0 or 1.

The value stored inside **dp[i][j][0]** will tell us the total number of valid sequences containing exactly **i** zeros and **j** ones that end with the number 0. 
Similarly, **dp[i][j][1]** will tell us the total number of valid sequences containing exactly **i** zeros and **j** ones that end with the number 1.

### Establishing the Base Cases

Before we build complex arrays, we must define the simplest scenarios. What if we only use zeros? Or only use ones?

If we use purely zeros, we can only do so up to our **limit**. For any number of zeros from 1 up to the limit, there is exactly 1 valid way to build the sequence (just placing zeros). If we try to place more zeros than the limit, the sequence becomes invalid, yielding 0 ways.

So, for **i** starting at 1 and going up to the limit, the value of **dp[i][0][0]** is exactly 1. 

The exact mirrored logic applies to purely ones. For **j** starting at 1 and going up to the limit, the value of **dp[0][j][1]** is exactly 1.

### The Heart of the Logic The Transitions

Now comes the beautiful part. How do we fill the rest of our table? Suppose we want to calculate **dp[i][j][0]**. This means we are trying to place a zero at the very end of our sequence.

Where could this zero have come from? It could be appended to any valid sequence that is exactly one zero shorter. That previous sequence could have ended in either a 0 or a 1. 

So, our first thought is to say that **dp[i][j][0]** is equal to **dp[i minus 1][j][0]** plus **dp[i minus 1][j][1]**.

But wait! We have the limit rule to respect. By blindly adding a zero, we might accidentally create a block of zeros that exceeds the limit. We need to identify and subtract these invalid combinations.

When does a block of zeros become invalid? It happens exactly when the block length reaches **limit plus 1**. 
If we just placed a zero, and it caused the block to become too long, it means the previous **limit** numbers were also zeros, and the number right before those was a one.

Let us trace that back. If our current zero is at the end, and the block is too long, we need to subtract the scenarios where we transitioned from a one exactly **limit plus 1** steps ago. The number of such scenarios is recorded in **dp[i minus limit minus 1][j][1]**. 

Therefore, our updated logic for placing a zero is as follows. We take the sum of shorter sequences. Then, if we have enough zeros to potentially exceed the limit, we subtract the invalid historical states.

We apply the exact same logic when trying to place a one to calculate **dp[i][j][1]**. We add the shorter valid sequences ending in 0 and 1, and if we have enough ones, we subtract the historical states where we transitioned from a zero exactly **limit plus 1** steps ago.

### Handling Large Numbers Modulo Arithmetic

Because we are doing additions and subtractions, our totals can get massive. We must take the result modulo 1000000007 at each step. 
A special trick is needed for subtraction. When we subtract one modulo number from another, the result might become negative in programming languages like C++. To fix this, when calculating **A minus B**, we do **(A minus B plus MOD) modulo MOD**. This ensures our counts remain positive and strictly within bounds.

### Code Implementation

Here is the C++ implementation reflecting the exact concepts we just discussed. I added comments throughout to map our logic directly to the code.

```cpp
class Solution {
public:
    int numberOfStableArrays(int zero, int one, int limit) {
        long long MOD = 1000000007;
        
        vector<vector<vector<long long>>> dp(zero + 1, vector<vector<long long>>(one + 1, vector<long long>(2, 0)));

        for (int i = 1; i <= min(zero, limit); ++i) {
            dp[i][0][0] = 1;
        }
        
        for (int j = 1; j <= min(one, limit); ++j) {
            dp[0][j][1] = 1;
        }

        for (int i = 1; i <= zero; ++i) {
            for (int j = 1; j <= one; ++j) {
                
                long long totalWaysEndingInZero = dp[i - 1][j][0] + dp[i - 1][j][1];
                
                if (i > limit) {
                    totalWaysEndingInZero = totalWaysEndingInZero - dp[i - limit - 1][j][1];
                }
                
                dp[i][j][0] = (totalWaysEndingInZero % MOD + MOD) % MOD;

                long long totalWaysEndingInOne = dp[i][j - 1][0] + dp[i][j - 1][1];
                
                if (j > limit) {
                    totalWaysEndingInOne = totalWaysEndingInOne - dp[i][j - limit - 1][0];
                }
                
                dp[i][j][1] = (totalWaysEndingInOne % MOD + MOD) % MOD;
            }
        }

        long long finalAnswer = (dp[zero][one][0] + dp[zero][one][1]) % MOD;
        return finalAnswer;
    }
};

```

### Analyzing the Efficiency

Let us evaluate how well this solution performs.

**Time Complexity**
The algorithm uses nested loops to traverse through every combination of zero and one counts. Inside the deepest loop, we perform a few basic arithmetic operations that take a constant amount of time. Therefore, the time complexity scales directly with the product of the zero variable and the one variable. In big O terms, this is proportional to **O(zero multiplied by one)**. Given the constraints top out at 1000, we perform roughly 1000000 operations, which runs blazing fast and easily passes all test cases.

**Space Complexity**
We allocate a three dimensional vector to store our precomputed states. The dimensions are based on the zero variable, the one variable, and a constant size of two for the last digit tracker. The space requirement is also proportional to **O(zero multiplied by one)**. For limits of 1000, this requires a few megabytes of memory, which is comfortably within standard competitive programming limits.

### Final Thoughts

I hope this breakdown clarifies the mechanics of building arrays with contiguous block limits. Thinking about transitions by adding everything and then subtracting the exact moment of failure is a powerful mental model that you can apply to many string and array building puzzles.

Thank you so much for reading through this walkthrough. I hope you found it insightful and helpful for your own learning journey. Keep practicing, stay curious, and happy coding!
