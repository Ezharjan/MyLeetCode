# Unlocking the Mystery of Divisible Digit Products and Sums A Friendly Guide [#3490]

Hello fellow developers and programming enthusiasts! Today I want to share my journey and thoughts on a truly fascinating algorithmic puzzle. We are going to look into a challenge where we need to count specific numbers based on the relationship between their digits. 

I really enjoyed working through this one, and I hope this detailed, step by step guide helps you understand the underlying concepts. I will do my best to explain everything clearly so that everyone, regardless of their background, can follow along and add this technique to their tool belt. We will break this down thoroughly, so grab a cup of coffee and let us dive right in!

## Understanding the Core Problem

The premise of the challenge is straightforward but hides some wonderful complexity. We are given two positive integers, L and R. Our goal is to count how many numbers within this inclusive range are considered "beautiful." 

What makes a number beautiful? A number earns this title if the product of its digits is perfectly divisible by the sum of its digits. 

For example, let us look at the number 15. 
* The digits are 1 and 5. 
* The sum of the digits is 1 plus 5, which equals 6. 
* The product of the digits is 1 times 5, which equals 5. 
* Since 5 is not perfectly divisible by 6, the number 15 is not beautiful.

Now let us look at the number 12. 
* The digits are 1 and 2. 
* The sum is 1 plus 2, which equals 3. 
* The product is 1 times 2, which equals 2. 
* Since 2 is not divisible by 3, 12 is not beautiful either.

But what about the number 20?
* The digits are 2 and 0.
* The sum is 2 plus 0, which equals 2.
* The product is 2 times 0, which equals 0.
* Is 0 divisible by 2? Yes! Zero divided by any positive number is zero with no remainder. Therefore, 20 is a beautiful number.

The constraints tell us that L and R can be as large as one billion (10^9). Because this upper limit is quite large, checking every single number one by one would take far too long. We need a more systematic approach to count these numbers efficiently.

## The Magic of Zero

The observation we just made about the number 20 is the key to unlocking the first major part of our solution. 

If any number contains the digit '0', the product of all its digits will immediately become zero. Because we are dealing with positive integers, the sum of the digits will always be a strictly positive number (greater than zero). 

Since zero modulo any positive integer is always zero, we can confidently state a universal rule for this puzzle: Every single positive integer that contains at least one '0' is guaranteed to be a beautiful number!

This simplifies our work immensely. Instead of calculating the product and sum for numbers with zeros, we can treat them as a separate group. Our logic then splits the numbers into two distinct buckets:
1. Numbers that contain at least one zero.
2. Numbers that consist entirely of nonzero digits (digits 1 through 9).

## Breaking Down the Logic

To find the total number of beautiful numbers up to a limit X, we can combine our two buckets. 

First, we can easily find how many numbers contain at least one zero. We do this by taking the total count of numbers from 1 to X, and subtracting the count of numbers that only use the digits 1 through 9.

Second, we need to find how many numbers made strictly of nonzero digits are beautiful. This is the trickier part, and we will use a technique called Digit Dynamic Programming to solve it.

Once we have a function that can calculate the total beautiful numbers from 1 up to any number X, we can find the answer for our specific range from L to R by calculating the total for R, and then subtracting the total for L minus 1. This is a common and highly effective prefix sum technique.

## Tackling the Nonzero Digits

For numbers that do not contain any zeros, we must ensure that the product of the digits modulo the sum of the digits equals zero. 

Since the maximum limit is just under one billion, our numbers will have at most 9 digits. The largest possible sum of digits for a 9 digit number occurs when all digits are 9. 
9 times 9 equals 81. 

This means the sum of the digits for any valid number in our range can only be an integer from 1 up to 81. Because the target sum is so small, we can iterate through every possible target sum. For each target sum, we will count how many valid numbers have exactly that digit sum and a digit product that is a multiple of that sum.

### Setting Up the Dynamic Programming Table

To avoid recalculating the same combinations repeatedly, we will precompute our values using a multidimensional array. Let us define our state. We will use an array with four dimensions:
`dp[target_sum][length][current_sum][product_modulo]`

Let us explain what each dimension represents:
* `target_sum`: The final sum we are aiming for. This ranges from 1 to 81.
* `length`: The number of digits we have placed so far. This ranges from 0 to 9.
* `current_sum`: The sum of the digits we have placed so far. This goes up to 81.
* `product_modulo`: The product of the digits placed so far, modulo the `target_sum`. This ranges from 0 to 80 (since modulo S is strictly less than S).

The value stored inside `dp[S][L][Sum][P]` will tell us the number of ways to form a sequence of digits of length L, such that their sum is exactly Sum, and their product modulo S is exactly P.

### Precomputing the States

We only need to populate this dynamic programming table once. 

We start by initializing our base cases. An empty sequence of length 0 has a sum of 0 and a product of 1. So, for every possible target sum S, `dp[S][0][0][1 modulo S]` is exactly 1. 

From there, we build our table bottom up. We loop through the length from 0 to 8 (since we want to build up to 9 digits). We loop through every possible current sum. We loop through every possible product modulo. 

If we find that `dp[S][len][sum][p]` has a value greater than zero, it means we have valid sequences here. We can then try appending a new digit to these sequences. The new digit can be anything from 1 to 9 (remember, we are only looking at nonzero digits now).

When we append a digit D:
* The new length becomes `len plus 1`.
* The new sum becomes `sum plus D`.
* The new product modulo becomes `(p times D) modulo S`.

We add the number of ways from our current state to the new state. If `sum plus D` exceeds our target sum S, we can stop checking larger digits because the digits are strictly increasing and we would overshoot our target. This is a nice little optimization that saves operations!

## Counting Numbers Below a Limit

Now that we have our precomputed table, we need to count the valid numbers that are less than or equal to our upper bound string X.

### Counting Numbers Without Zeros

We will write a helper function to count total numbers up to X that do not contain the digit 0. 

First, we count all numbers that have fewer digits than X. If X has L digits, then for any length less than L, we can place any of the 9 nonzero digits in each position. So a length of 2 contributes 9 times 9 ways, a length of 3 contributes 9 times 9 times 9 ways, and so on.

Next, we count numbers of the exact same length as X, but which are strictly smaller than X. We process X digit by digit from left to right. For each position, we can pick a digit D that is strictly less than the corresponding digit in X. Of course, D must start from 1. Once we place a smaller digit, all remaining positions to the right can be filled freely with any of the 9 nonzero digits. 

If we encounter a '0' in X during this process, we must stop and break out of our loop, because we cannot match a '0' using our restricted set of digits 1 through 9.

### Counting Beautiful Numbers Without Zeros

This function follows a similar prefix matching logic, but it uses our dynamic programming table to ensure the beautiful condition is met.

We will iterate through every possible target sum S from 1 to 81.

For each S, we first gather all valid numbers that have fewer digits than X. We look at our table for sequences of length less than L where the `current_sum` is exactly S, and the `product_modulo` is exactly 0. We add these to our result.

Next, we match numbers of the same length as X. We traverse X from left to right. We keep track of the `current_sum` of the digits we have matched so far, and the `current_prod` modulo S. 

For the current position, we try placing a digit D strictly less than the digit in X (from 1 up to the limit minus 1). We check if adding D keeps our total sum within S. If it does, we calculate the remaining length we need to fill, the remaining sum we need to reach S, and the new product modulo. 

We then need to find how many ways the remaining digits can result in a final product modulo S that equals zero. To do this, we look through our table for the remaining length and required sum, and find any starting product P such that when combined with our current product, it results in zero modulo S. The provided code includes a clever Greatest Common Divisor logic check to skip unnecessary multiplications and directly identify the valid P values!

Finally, we update our running sum and product with the exact digit from X and move to the next position. If the running sum ever exactly equals S and the running product modulo S is 0 after processing all digits, we count the boundary number X itself as one additional beautiful number.

## Bringing Everything Together

Our final wrapper function simply organizes the steps:
1. It converts the integer limit into a string for easy digit processing.
2. It calculates the total number of valid integers up to X that contain zero.
3. It calculates the total number of beautiful integers up to X that do not contain zero.
4. It adds them together to get the grand total of beautiful numbers.

## The C++ Implementation

Here is a clean and fully working implementation of the strategy we just discussed. I have included comments to help map the code directly to the concepts we covered above.

```cpp
#include <string>
#include <vector>
#include <numeric>

using namespace std;

// We define our dynamic programming table globally to prevent stack overflow
// Dimensions correspond to target sum, length, current sum, and product modulo
int dp[82][10][82][81];
bool initialized = false;

class Solution {
public:
    // This function populates the table strictly once
    void init() {
        if (initialized) return;
        initialized = true;
        
        for (int S = 1; S <= 81; ++S) {
            dp[S][0][0][1 % S] = 1; 
            for (int len = 0; len < 9; ++len) {
                for (int sum = 0; sum <= S && sum <= len * 9; ++sum) {
                    for (int p = 0; p < S; ++p) {
                        int ways = dp[S][len][sum][p];
                        if (ways > 0) {
                            for (int d = 1; d <= 9; ++d) {
                                if (sum + d <= S) {
                                    dp[S][len + 1][sum + d][(p * d) % S] += ways;
                                } else {
                                    break; 
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Returns the total integers up to X lacking the digit zero
    long long countNoZero(const string& X) {
        long long res = 0;
        int L = X.length();
        long long p9 = 1;
        
        for (int i = 1; i < L; ++i) {
            p9 *= 9;
            res += p9;
        }
        
        for (int i = 0; i < L; ++i) {
            int d = X[i] - '0';
            for (int j = 1; j < d; ++j) {
                long long ways = 1;
                for (int k = i + 1; k < L; ++k) ways *= 9;
                res += ways;
            }
            if (d == 0) break; 
            if (i == L - 1) res++; 
        }
        return res;
    }

    // Returns the beautiful integers up to X lacking the digit zero
    long long countBeautifulNoZero(const string& X) {
        long long res = 0;
        int L = X.length();
        
        for (int S = 1; S <= 81; ++S) {
            for (int len = 1; len < L; ++len) {
                if (S <= len * 9) {
                    res += dp[S][len][S][0];
                }
            }
            
            int current_sum = 0;
            int current_prod = 1 % S;
            bool valid = true;
            
            for (int i = 0; i < L; ++i) {
                int limit = X[i] - '0';
                for (int d = 1; d < limit; ++d) {
                    if (current_sum + d <= S) {
                        int rem_len = L - 1 - i;
                        int req_sum = S - current_sum - d;
                        int V = (current_prod * d) % S;
                        
                        int g = std::gcd(V, S);
                        int step = S / g;
                        
                        for (int P = 0; P < S; P += step) {
                            res += dp[S][rem_len][req_sum][P];
                        }
                    }
                }
                if (limit == 0) { 
                    valid = false;
                    break;
                }
                current_sum += limit;
                current_prod = (current_prod * limit) % S;
            }
            if (valid && current_sum == S && current_prod == 0) {
                res++;
            }
        }
        return res;
    }

    // Orchestrates the logic to count all valid instances up to X
    long long countAll(long long X) {
        if (X == 0) return 0;
        string sX = to_string(X);
        long long total_with_zero = X - countNoZero(sX);
        long long beautiful_no_zero = countBeautifulNoZero(sX);
        
        return total_with_zero + beautiful_no_zero;
    }

    int beautifulNumbers(int l, int r) {
        init();
        return countAll(r) - countAll(l - 1);
    }
};

```

## Wrapping Up with Complexity

To close things out, let us talk about the resources this solution uses.

For the Space Complexity, we are storing an array of dimensions 82 by 10 by 82 by 81. Multiplying these together gives about 5.4 million integer entries. In C++, an integer typically takes 4 bytes, so our table requires approximately 21 megabytes of memory. This is well within standard competitive programming and platform limits.

For the Time Complexity, the initialization phase takes a fixed number of operations. Specifically, 81 target sums times 9 lengths times 82 current sums times 81 modulo states times 9 digits. This resolves to roughly 40 million basic loop iterations, which execute in a small fraction of a second. After this single setup phase, processing any query is extremely fast. The query evaluation involves iterating over the 81 possible sums and checking configurations up to 9 digits in length, scaling proportionally to the number of digits in our upper bounds. It guarantees highly responsive execution even for limits near one billion.

I hope you found this guide illuminating! By breaking the problem down into logical categories and systematically applying dynamic programming, we transformed a daunting puzzle into a manageable and understandable process. Happy coding to all of you, and keep exploring those fascinating algorithms!
