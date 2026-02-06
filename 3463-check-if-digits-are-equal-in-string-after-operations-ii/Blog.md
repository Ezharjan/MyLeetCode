# Cracking the Code of Large Digit Strings and Modulo 10 Magic [#3463]

[Blog Post Link](https://leetcode.com/discuss/post/7553599/cracking-the-code-of-large-digit-strings-mj4o/)


Welcome fellow coding enthusiasts! Today I want to share a fascinating journey through a problem that looks deceptively simple but hides a beautiful mathematical treasure chest inside. If you have ever stared at a "Hard" problem involving string manipulations and wondered how to optimize it beyond the brute force approach, this guide is for you. We are going to tackle a problem involving digit strings and repeated summation operations.

I remember when I first saw this puzzle. It felt like a standard simulation task. However, as soon as I looked at the constraints, I realized that a simple simulation would crash and burn. We are dealing with strings of length up to 100,000. Simulating the process step by step would take billions of operations. We need something faster. We need the power of Number Theory.

Let us dive deep into the mathematics of Pascal’s Triangle, Lucas’ Theorem, and the Chinese Remainder Theorem to solve this elegantly.

## Understanding the Challenge

Imagine you are given a string consisting purely of digits, like "34789". The rule of the game is simple yet destructive. You look at every pair of adjacent digits, add them up, take the last digit of the sum (modulo 10), and write it down. This creates a new string that is one digit shorter than the previous one. You repeat this entire process until you are left with exactly two digits.

Your goal is to determine if those final two digits are identical.

Let us visualize this with the example "34789".
The first pass transforms the string.
We add 3 and 4 to get 7.
We add 4 and 7 to get 1 (since 11 modulo 10 is 1).
We add 7 and 8 to get 5 (15 modulo 10 is 5).
We add 8 and 9 to get 7 (17 modulo 10 is 7).
Our new string is "7157".

We do it again.
7 plus 1 is 8.
1 plus 5 is 6.
5 plus 7 is 2.
The new string is "862".

And again.
8 plus 6 is 4.
6 plus 2 is 8.
The string is now "48".

We stop here because we have two digits. Is 4 equal to 8? No. So we return false.

## The Trap of Brute Force

The straightforward solution is to write a loop that runs the simulation exactly as described. You create a new array, iterate through the old array, sum the pairs, and repeat.

This logic works perfectly for small strings. However, consider the computational cost. If the string has length , the first step performs  additions. The next step performs  minus 1 additions. The step after that performs  minus 2 additions. The total number of operations is roughly the sum of integers from 1 to , which scales quadratically ().

When  is 100,000,  is 10,000,000,000. Ten billion operations will definitely exceed the time limit of most coding platforms (usually around 1 second, or roughly 100 million operations). We need a linear  or near linear approach.

## The Mathematical Breakthrough

Let us look at how the digits combine.
Suppose our string is simply .
Round 1: 
Round 2:  which simplifies to 
Round 3:  which simplifies to 

Do these coefficients look familiar?
1, 2, 1
1, 3, 3, 1
These are the rows of Pascal’s Triangle!

If we want to reduce a string of length  down to a single digit, the value of that final digit is the sum of each original digit multiplied by a specific binomial coefficient. Specifically, for a string of length , the coefficient for the digit at index  (0 indexed) is "L minus 1 choose i".

In our specific problem, we do not stop at 1 digit. We stop at 2 digits.
Let the string length be . We stop when the length is 2.
This essentially means we are reducing the subarray from index 0 to  minus 2 into the **Left Final Digit**, and the subarray from index 1 to  minus 1 into the **Right Final Digit**.

Let . This  represents the "degree" of our binomial expansion.
The Left Final Digit is the sum of `Binomial(N, i) * s[i]` for all  from 0 to .
The Right Final Digit is the sum of `Binomial(N, i) * s[i+1]` for all  from 0 to .

We need to return true if Left equals Right.
Mathematically, this is equivalent to checking if `Left - Right` is congruent to 0 modulo 10.
Substitute the sums:
Sum of `Binomial(N, i) * s[i]` minus Sum of `Binomial(N, i) * s[i+1]` is congruent to 0.
We can factor out the binomial coefficient:
Sum of `Binomial(N, i) * (s[i] - s[i+1])` must be divisible by 10.

## The Hurdle of Modulo 10

We have a formula! But we have a big problem.
We need to calculate `Binomial(N, i)` modulo 10.
The number  can be up to 100,000. The value of  (factorial) is astronomically large and cannot be stored in any standard variable.
Usually, we compute combinations using factorials and modular inverse. But modular inverse only exists if the modulus is prime. The number 10 is not prime. It is .

This means we cannot directly calculate "N choose k" modulo 10 using standard division.
However, we can compute it modulo 2 and modulo 5 separately. Since 2 and 5 are prime (and coprime to each other), we can combine the results to find the answer modulo 10 using the **Chinese Remainder Theorem**.

### Part 1: Lucas' Theorem for Modulo 5

Lucas' Theorem is a powerful tool in number theory for finding `nCk % p` where  is a prime.
The theorem states that to find `Binomial(n, k) % p`, you should express  and  in base .
Let  and  be the base  expansions.
Then `Binomial(n, k) % p` is equal to the product of `Binomial(n_i, k_i) % p` for all positions .

Simply put:

1. Write  and  in base 5.
2. Look at each digit pair. Compute the small binomial coefficient for those digits.
3. Multiply all these small results together modulo 5.

If any digit of  is greater than the corresponding digit of  in base 5, the result is 0.

Since the base 5 digits are small (0 to 4), we can precompute a tiny 5x5 lookup table for these values.

### Part 2: Bitwise Logic for Modulo 2

We also need `Binomial(N, i) % 2`.
This basically asks: Is the binomial coefficient odd or even?
There is a fascinating property for this. `Binomial(N, i)` is odd if and only if every bit that is set (1) in the binary representation of  is also set in the binary representation of .
In bitwise logic terms: `(N & i) == i`.
If this condition holds, the result is 1. Otherwise, it is 0.

### Part 3: The Chinese Remainder Theorem (CRT)

Now we have two values:



We need a value  such that:



We can construct a formula for this.
We need a term that is a multiple of 5 but gives remainder 1 when divided by 2. That number is 5 (since 5 % 2 is 1).
We need a term that is a multiple of 2 but gives remainder 1 when divided by 5. That number is 6 (since 6 % 5 is 1).

So the formula is:


Let us verify.
If : . (5 is 1 mod 2 and 0 mod 5). Correct.
If : . (6 is 0 mod 2 and 1 mod 5). Correct.

## The Final Algorithm

We have all the pieces. Here is the master plan to solve the problem efficiently.

1. Identify  as the length of string .
2. Set . This is the effective row of Pascal's triangle we are using.
3. Initialize a variable `total_diff` to 0.
4. Loop  from 0 to :
* Calculate the modulo 2 coefficient using the bitwise check `(N & i) == i`.
* Calculate the modulo 5 coefficient using Lucas' Theorem (converting  and  to base 5).
* Combine them to get the modulo 10 coefficient.
* Calculate the difference between the current digit  and the next digit .
* Multiply this difference by the coefficient and add it to `total_diff`.
* Keep everything modulo 10.


5. If `total_diff` modulo 10 is 0, return true. Otherwise, return false.

## Implementation Details

We will implement this in C++. We need a helper function for Lucas' Theorem and a small lookup table for the combinations of numbers 0 through 4.

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
private:
    // Precomputed Binomial Coefficients for Modulo 5
    // This table stores nCk % 5 where row is n and col is k
    const int C[5][5] = {
        {1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 2, 1, 0, 0},
        {1, 3, 3, 1, 0},
        {1, 4, 1, 4, 1}
    };

    // Function to calculate nCk % 5 using Lucas' Theorem
    int lucasMod5(int n, int k) {
        if (k < 0 || k > n) return 0;
        int res = 1;
        
        // We process n and k in base 5 digit by digit
        while (n > 0 || k > 0) {
            int n_digit = n % 5;
            int k_digit = k % 5;
            
            // If a digit in k is larger than n, the term is 0
            if (k_digit > n_digit) return 0; 
            
            // Multiply by the precomputed small coefficient
            res = (res * C[n_digit][k_digit]) % 5;
            
            n /= 5;
            k /= 5;
        }
        return res;
    }

public:
    bool hasSameDigits(string s) {
        int n = s.length();
        int N = n - 2; // The degree of the binomial expansion
        
        int diffSum = 0;

        for (int i = 0; i <= N; ++i) {
            // 1. Calculate nCk % 2 using Bitwise property
            // If (N & i) == i, then nCk is odd (1), else even (0)
            int mod2 = ((N & i) == i) ? 1 : 0;

            // 2. Calculate nCk % 5 using Lucas' Theorem helper
            int mod5 = lucasMod5(N, i);

            // 3. Combine using Chinese Remainder Theorem for Modulo 10
            // Formula: (5 * mod2 + 6 * mod5) % 10
            int coeff = (5 * mod2 + 6 * mod5) % 10;

            // 4. Calculate contribution of the difference between adjacent digits
            int digitDiff = s[i] - s[i+1];
            
            // Add to the total sum modulo 10
            // We ensure positive modulo result by adding 100 before modding if needed
            // but standard (a + b) % 10 is fine for positive chains.
            // For differences, (coeff * digitDiff) might be negative.
            int term = (coeff * digitDiff) % 10;
            diffSum = (diffSum + term + 10) % 10;
        }

        return diffSum == 0;
    }
};

```

## Why This Works So Fast

Let us analyze the performance.
The loop runs  times where  is roughly 100,000.
Inside the loop we call `lucasMod5`.
This function divides the number by 5 repeatedly. Since  is , the number of base 5 digits is very small (roughly  or  iterations).
This is incredibly fast. The complexity is technically  but the log factor is so small it acts almost like .
The space complexity is  because we only store a few integers and a tiny constant table.

## Conclusion

We started with a problem that looked like a slow simulation and transformed it into a lightning fast number theory calculation. By recognizing the pattern of Pascal’s Triangle, we realized we were dealing with binomial coefficients. By understanding the properties of the number 10, we decomposed the problem into modulo 2 and modulo 5. And finally, by using Lucas' Theorem and the Chinese Remainder Theorem, we bridged the gap to find the exact solution.

This is the beauty of algorithmic problem solving. 
It is not just about writing code; it is about seeing the hidden structures beneath the surface. 
I hope this guide helps you understand not just the "how" but the deep "why" behind the solution. 

Happy coding!
