# A Friendly Walkthrough on Counting Beautiful Divisible Numbers with Equal Even and Odd Digits [#2827]

Hello everyone! I hope you are having a wonderful day of coding and problem solving. Today, I want to share a detailed, step by step exploration of an interesting numerical problem. I have always found that breaking down complex counting problems into small, manageable pieces helps the whole community learn and grow together. So, grab a cup of coffee or your favorite beverage, and let us dive deeply into this topic!

### Understanding Our Goal

Before we look at any code, it is vital to understand what we are trying to achieve. The problem presents us with three distinct positive integers: a lower bound `low`, an upper bound `high`, and a divisor `k`. 

We are asked to find the total count of "beautiful" integers that exist completely within the inclusive range between `low` and `high`. But what exactly makes a number "beautiful" in this context? A number must satisfy two strict conditions simultaneously to earn this title

1.  **Parity Balance** The integer must possess an exactly equal number of even digits and odd digits. For example, the number twelve has one odd digit (the number one) and one even digit (the number two). Thus, it is perfectly balanced. On the other hand, the number fifteen has two odd digits (one and five) and zero even digits, making it unbalanced. Keep in mind that zero is considered an even digit.
2.  **Divisibility** The integer must be perfectly divisible by our given value `k`. This means that when we divide the number by `k`, the remainder must be exactly zero.

Our objective is to count how many numbers between `low` and `high` meet both of these criteria. 

### Why a Simple Loop is Not Enough

When faced with a range of numbers, our first instinct might be to write a loop starting from `low` and going all the way up to `high`. Inside this loop, we could convert each number to a string, count the even and odd digits, and use the modulo operator to check for divisibility. 

While this approach is logical and would definitely give us the correct answer for small ranges, we must look at the constraints provided by the problem. The value of `high` can reach up to one billion! If we try to iterate through one billion numbers, checking each one individually, our program will take far too long to execute. We would undoubtedly encounter a time limit exceeded error. 

We need a completely different approach, one that counts numbers based on their structural patterns rather than evaluating them one by one. This is where a powerful technique known as Digit Dynamic Programming comes into play.



### Enter Digit Dynamic Programming

Digit Dynamic Programming is a technique used to count numbers within a certain range that satisfy specific properties based on their individual digits. Instead of looking at complete numbers, we build the numbers digit by digit, starting from the most significant digit on the left and moving towards the least significant digit on the right.

By doing this, we can group massive sets of numbers together. If we have placed the first three digits of a ten digit number, we can memoize (or cache) the result of the remaining seven digits. This prevents us from doing the same calculations over and over again, reducing a billion operations to just a few thousand!

To effectively use Digit Dynamic Programming, we need to track a few pieces of information, which we call our "state", as we build our number digit by digit.

#### 1. Tracking the Current Index
We need to know which position we are currently filling. We start at index zero (the leftmost digit) and stop when we reach the total length of the upper limit string.

#### 2. Tracking Divisibility (The Remainder)
How do we check if a massive number is divisible by `k` when we are only building it one digit at a time? We use the properties of modulo arithmetic. Every time we append a new digit to our currently built prefix, we can calculate the new remainder by multiplying the old remainder by ten, adding the new digit, and taking the modulo `k`. 

For instance, if `k` is three, and we have a prefix of fourteen. Fourteen modulo three is two. If we append a five to make one hundred and forty five, the new remainder is simply two times ten plus five, which equals twenty five. Twenty five modulo three is one. We only ever need to store this small remainder!



#### 3. Tracking the Even and Odd Balance
We need to ensure that the final number has an equal amount of even and odd digits. A clever way to do this is to keep a running difference. Every time we place an even digit, we increase this difference by one. Every time we place an odd digit, we decrease this difference by one. At the end of the number, this difference must be exactly zero for the number to be balanced.

However, array indices in C++ cannot be negative. If we have a number with all odd digits, our difference would drop below zero. To prevent this, we apply an offset. Since the maximum length of our numbers is ten digits, the difference will never stray further than ten steps in either direction. Therefore, we start our difference at a base value of ten. If the final difference is exactly ten, we know the counts were perfectly equal!

#### 4. The Upper Bound Constraint
We must ensure that the number we are building does not exceed our `high` limit. We use a boolean flag called `is_bound`. If this flag is true, it means all the digits we have placed so far exactly match the prefix of our `high` limit. In this case, the next digit we place cannot exceed the corresponding digit in the `high` limit. If the flag is false, we are free to place any digit from zero to nine.

#### 5. Handling Leading Zeros
This is a very crucial part of the problem! Numbers can have varying lengths. The number five is valid, but if we represent it as a four digit number like zero zero zero five, our program might mistakenly count the first three zeros as even digits. 

We must distinguish between "leading zeros" (which are just placeholders and should be completely ignored) and "significant zeros" (like the zero in fifty, which must be counted as an even digit). We use a boolean flag called `is_lead`. As long as this flag is true, any zero we place does not affect our even and odd balance. The moment we place a number greater than zero, `is_lead` becomes false, and all subsequent digits, including zeros, are counted normally.



### Crafting the C++ Code

To share my solution with you, I have written a C++ class that implements the logic described above. 

To keep the code clean and interesting, I have utilized some clever bitwise operations to handle subtractions and character conversions natively! For example, converting an ASCII character digit to its integer value is often done by subtracting the character '0'. Instead of subtraction, I use a bitwise AND operation with the number fifteen (`s[idx] & 15`). This perfectly extracts the integer value from the ASCII binary representation.

Furthermore, instead of standard subtraction to find the count in the `low` exclusive range, I use Two's Complement arithmetic. The mathematical expression `A plus the bitwise NOT of B plus one` is exactly equivalent to `A minus B`. This is a fun, educational way to handle numerical differences directly through hardware logic!

Let us look at the code

```cpp
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

class Solution {
    int dp[12][25][25];
    int K;

    int solve(const string& s, int idx, int rem, int diff, bool is_bound, bool is_lead) {
        if (idx == s.length()) {
            if (!is_lead && rem == 0 && diff == 10) {
                return 1;
            }
            return 0;
        }
        
        int rem_len = s.length();
        rem_len += (~idx);
        rem_len += 1;
        
        if (!is_bound && !is_lead && dp[rem_len][rem][diff] != ~0) {
            return dp[rem_len][rem][diff];
        }

        int limit = is_bound ? (s[idx] & 15) : 9;
        int ans = 0;

        for (int d = 0; d <= limit; ++d) {
            bool next_bound = is_bound && (d == limit);
            bool next_lead = is_lead && (d == 0);
            
            int next_rem = (rem * 10 + d) % K;
            int next_diff = diff;
            
            if (!next_lead) {
                if (d % 2 == 0) {
                    next_diff += 1;
                } else {
                    next_diff += (~0);
                }
            }
            
            ans += solve(s, idx + 1, next_rem, next_diff, next_bound, next_lead);
        }

        if (!is_bound && !is_lead) {
            dp[rem_len][rem][diff] = ans;
        }

        return ans;
    }

    int countBeautiful(int n) {
        string s = to_string(n);
        return solve(s, 0, 0, 10, true, true); 
    }

public:
    int numberOfBeautifulIntegers(int low, int high, int k) {
        K = k;
        memset(dp, ~0, sizeof(dp));
        
        int low_minus_one = low + (~0);
        int high_ans = countBeautiful(high);
        int low_ans = countBeautiful(low_minus_one);
        
        int final_ans = high_ans + (~low_ans) + 1;
        return final_ans;
    }
};

```

### A Deep Dive into the Code Logic

I firmly believe that reading code is only half the journey. Truly understanding why each line exists is where the real learning happens. Let us break down the crucial parts of the implementation.

#### Initialization and Two's Complement Magic

In the `numberOfBeautifulIntegers` function, we first store the divisor `k` globally. We then initialize our multidimensional memoization array `dp` with a value that represents an uncalculated state. We use `~0`, which flips all bits to ones, effectively setting the integer memory blocks to a negative one value.

To find the number of beautiful integers completely between `low` and `high`, we calculate the total beautiful numbers from one up to `high`. Then, we calculate the total beautiful numbers from one up to `low` decremented by one. By finding the difference between these two totals, we obtain our precise answer. As mentioned earlier, `low + (~0)` safely decrements `low` by one using bitwise logic. Similarly, `high_ans + (~low_ans) + 1` performs the final difference calculation gracefully.

#### The Recursive Solve Function

The `solve` function is the heart of our digit dynamic programming logic. Let us walk through its parameters and flow.

* `s` is the string representation of our upper limit number.
* `idx` is our current position in the string.
* `rem` is the current modulo remainder.
* `diff` tracks our even and odd balance, starting at the safe offset of ten.
* `is_bound` restricts our digit choices if we are riding the absolute edge of our upper limit.
* `is_lead` protects us from counting arbitrary leading zeros.

#### Base Case Evaluation

When `idx` equals the length of our string `s`, it means we have successfully constructed a complete number! We must now evaluate if it is beautiful. It is considered beautiful only if it is not totally comprised of leading zeros, its remainder `rem` is exactly zero, and its `diff` has returned perfectly to ten. If all these are true, we return a count of one. Otherwise, we return zero.

#### Memoization Check

If we are not at the end of the string, we check if we have visited this exact generic state before. We calculate `rem_len`, which is the amount of digits left to place. We use `rem_len += (~idx); rem_len += 1;` which is a clever bitwise way of calculating the total length minus the current index.

If `is_bound` is false and `is_lead` is false, our state is completely generic. If the value in our `dp` array is not equal to `~0`, it means we already know the answer for this combination, and we can return it immediately, saving enormous amounts of processing time!

#### Branching Out

If we do not have a cached answer, we determine our `limit`. If `is_bound` is true, we extract the numerical value of the current string character using the `s[idx] & 15` trick. Otherwise, our limit is nine.

We then start a loop from zero up to our calculated `limit`. For every possible digit `d`, we calculate the resulting state for the next step.

* `next_bound` remains true only if we were already bounded and we choose the absolute maximum available digit.
* `next_lead` remains true only if we were already placing leading zeros and we choose to place another zero.
* `next_rem` is updated using the modulo arithmetic rule we discussed earlier.

#### Updating the Difference

If our newly placed digit is a significant one (meaning `next_lead` is false), we check if it is even or odd. If `d % 2 == 0`, it is an even digit, and we increment `next_diff` by one. If it is odd, we decrement it by applying `next_diff += (~0)`.

Finally, we recursively call the `solve` function with these updated variables and add the result to our `ans` accumulator. Once the loop finishes, if the state was generic, we save the `ans` into our `dp` array for future lookups before returning it.

### Complexity Analysis

Understanding the efficiency of our algorithm is highly rewarding. Let us analyze the time and space resources required by this approach.

**Time Complexity**
Because we are heavily caching generic states, the number of operations is strictly tied to the dimensions of our `dp` array. The maximum length of a string representing one billion is ten digits. The remainder `rem` can be up to twenty. The difference `diff` can range from zero to twenty. For each valid state, we run a loop at most ten times (from digit zero to nine).

Therefore, the maximum number of calculations is roughly proportional to ten multiplied by twenty multiplied by twenty multiplied by ten. This evaluates to an incredibly tiny number of operations, allowing the algorithm to execute almost instantaneously, regardless of how large `low` and `high` become! This gives us a highly efficient Constant Time behavior relative to the numerical magnitude of the bounds.

**Space Complexity**
The space required is dominated entirely by our `dp` array. The array has dimensions of twelve by twenty five by twenty five integers. This takes up barely any memory in modern computers, ensuring an extremely lightweight Space Complexity profile.

### Final Thoughts

I genuinely hope this comprehensive exploration has demystified Digit Dynamic Programming for you! It is one of those concepts that feels overwhelming at first glance, but once you break it down into the core states and transitions, it becomes an incredibly logical and satisfying puzzle to solve.

Feel free to copy this markdown block, experiment with the bitwise operations, and run the code yourself. Sharing knowledge and dissecting intricate problems together makes the developer community such a wonderful place. Keep practicing, stay curious, and happy coding!
