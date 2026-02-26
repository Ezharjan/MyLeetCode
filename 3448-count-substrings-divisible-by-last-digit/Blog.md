# Cracking the Puzzle of Counting Substrings Divisible by Their Last Digit A Comprehensive and Friendly Guide [#3448]

Hello everyone! Welcome to another deep dive into the fascinating world of string processing and algorithmic problem solving. Today, I would like to share my thoughts and a detailed walkthrough for a very interesting counting puzzle. I really enjoy exploring these types of questions because they teach us how to think about combinations, sequences, and large numbers without getting lost in the massive scale of the data. 

I hope this guide serves as a helpful resource for anyone trying to understand the underlying logic. Whether you are a beginner looking to grasp dynamic programming concepts or an experienced coder wanting a fresh perspective, I have structured this tutorial to be as accessible and thorough as possible. Grab a cup of coffee or tea, and let us dive right into the details!

***

## Understanding the Core Problem

Before we write any code, we must completely understand what the problem is asking us to achieve. We are given a string of characters, and every single character in this string is a numerical digit ranging from zero to nine. 

Our goal is to find the total number of valid substrings. A substring is simply a contiguous slice of the original string. However, for a substring to be considered valid and added to our final count, it must satisfy two specific conditions:

1. The substring must end with a nonzero digit. This means a substring ending in zero is immediately disqualified.
2. If we read that entire substring as a single integer number, it must be perfectly divisible by its own final digit. In other words, dividing the number by its last digit should leave a remainder of exactly zero.

Let us look at a tiny example. Suppose our string contains the characters "2936". 
If we take the substring "36", it ends with the digit 6. Six is a nonzero digit, so the first condition is met. Now, we read "36" as the integer thirty six. Is thirty six divisible by six? Yes, it is! Thirty six divided by six is exactly six, with a remainder of zero. Therefore, "36" is a valid substring.

On the other hand, consider the substring "29". It ends with 9, which is nonzero. However, twenty nine divided by nine is three with a remainder of two. Since the remainder is not zero, "29" is not a valid substring.

Our task is to look at all possible contiguous slices within the given string, check these rules, and return the total count of the ones that pass.

***

## Why the Brute Force Approach Falls Short

When we first encounter a problem like this, the most natural instinct is to try a direct, simulated approach. We might think about using two nested loops. The outer loop would pick the starting position of the substring, and the inner loop would pick the ending position. Then, we could extract the substring, convert it to a whole integer, and check the divisibility rule.

While this logic is completely correct in theory, it runs into two massive roadblocks when applied to larger inputs.

### The Integer Overflow Issue

The first major issue is the sheer size of the numbers. A standard integer in programming can hold numbers up to about two billion, which is roughly ten digits long. A long integer might hold up to nineteen digits. However, the problem specifies that the input string can have a length of up to one hundred thousand characters! 

Imagine a substring that is fifty characters long. There is absolutely no standard primitive data type that can hold a fifty digit number. If we try to convert such a long string into an integer, our program will crash or produce garbage values due to overflow.

### The Time Limit Exceeded Issue

The second issue is the amount of time it takes to check every single combination. If we have a string of one hundred thousand characters, the number of possible substrings is roughly five billion. Generating five billion substrings and processing each one would take a massive amount of time, far exceeding the typical time limits allowed on coding platforms.

We need a completely different strategy. We need a way to check divisibility without ever storing the full integer, and we need a way to count the valid substrings without explicitly checking every single one from scratch.

***

## The Magic of Modulo Arithmetic

To solve the integer overflow problem, we need to rely on the properties of remainders, formally known as modulo arithmetic. 

Think about how we build numbers digit by digit. Suppose we have a number like 14. If we divide 14 by 6, the remainder is 2. 
Now, suppose we append the digit 5 to the end of 14, making it 145. How does the new remainder relate to our old remainder of 2?

Notice that appending a digit is the exact same mathematical operation as multiplying the original number by ten and then adding the new digit. 
So, 145 is simply 14 multiplied by 10, plus 5.

According to the rules of modulo arithmetic, we do not need the full original number to find the new remainder! We only need the old remainder.
The new remainder when dividing by 6 will be equal to the old remainder multiplied by 10, plus the new digit, all evaluated modulo 6.

Let us verify this. 
Our old remainder was 2. 
We multiply 2 by 10, which gives 20. 
We add the new digit 5, which gives 25. 
Finally, 25 modulo 6 is exactly 1 (since 6 multiplied by 4 is 24, leaving 1).

Is the remainder of 145 divided by 6 really 1? 
Yes! 145 divided by 6 is 24 with a remainder of 1.

This discovery is phenomenal! It means we never need to store gigantic numbers. As we read the string character by character from left to right, we only need to keep track of the remainders. Since our possible divisors are the digits 1 through 9, our remainders will always be small numbers between 0 and 8.

***

## Building a Smarter State Tracking System

Now that we know we only need to track remainders, we can build a tracking system. As we iterate through the string, we want to remember how many active substrings have a specific remainder for a specific divisor.

Since the last digit of our future valid substrings could be anything from 1 to 9, we must track the remainders for all nine possible divisors simultaneously!

We can set up a tracking table. Let us conceptualize a grid where the rows represent the divisors (from 1 to 9), and the columns represent the possible remainders (from 0 to 8). Inside each cell of this grid, we will store a count. This count tells us exactly how many contiguous sequences ending at our current position leave that specific remainder when divided by that specific divisor.

### Updating the Table Step by Step

Let us imagine we are reading a new digit from our string. We will call this new digit `C`. When we encounter `C`, two distinct things happen.

First, this new digit `C` extends all the existing substrings that ended at the previous character. For every single count we tracked in our previous table, we apply our modulo math rule. If a certain group of substrings had a remainder `R` for a divisor `D`, extending them with `C` changes their remainder to `(R * 10 + C) modulo D`. We simply take the old count and add it to the new cell in our updated table.

Second, the new digit `C` can stand entirely on its own as a brand new substring of length one! A single digit `C` divided by a divisor `D` leaves a remainder of `C modulo D`. So, we must add an extra count of 1 to that specific cell in our new table.

### Finding the Answer

After we finish calculating the new table for the current character `C`, we can easily check how many valid substrings end right here. 
We just look at the character `C`. If `C` is a nonzero digit, we go to the row in our table where the divisor is `C`. We look at the column where the remainder is exactly zero. The number inside that cell tells us exactly how many valid substrings end at this current position! We add this number to our running total.

We repeat this process for every character in the string, accumulating the valid counts into our grand total.

***

## Managing Memory with Toggling Arrays

If we create a brand new two dimensional grid for every single character in a string of length one hundred thousand, we might end up using too much memory and spending too much time allocating it. 

However, notice a crucial detail in our logic. To build the grid for the current character, we only ever look at the grid from the immediately previous character. All the older grids from earlier characters are completely useless to us. 

This means we only ever need two grids in memory at any given time. One grid holds the results from the previous step, and the other grid is used to build the new results for the current step. Once we finish the current step, the new grid becomes the old grid for the next iteration. 

This technique is often called rolling arrays or toggling states. We can represent this with an array of size two. We can use a variable `cur` to represent the index of the current grid (which will be either 0 or 1), and a variable `nxt` to represent the index of the next grid. After processing a character, we swap them.

***

## Squeezing Out Maximum Performance

To make our code run extremely fast, we can eliminate redundant mathematical operations. Inside our nested loops, we constantly calculate the new remainder using the formula `(R * 10 + C) modulo D`. 

Division and modulo operators are relatively slow operations for a computer processor compared to simple addition or array lookups. Since our values for `D`, `R`, and `C` are strictly bounded (they are all single digits), there are only a tiny handful of possible combinations. 

We can calculate all these combinations before we even start looking at the string! We can create a static lookup table called `trans`. We populate `trans[D][R][C]` with the result of the modulo math. Then, inside our main processing loops, we replace the math operations with a lightning fast array lookup. This precomputation strategy drastically reduces the runtime.

***

## The Complete Implementation

Below is a carefully constructed C++ implementation of our logic. Notice how we avoid using any minus signs in the code by using the bitwise XOR operator (`^`) to convert characters to integers. The ASCII value for the character '0' is 48. Doing a bitwise XOR with 48 perfectly transforms the character representations into their true integer values! 

```cpp
class Solution {
public:
    long long countSubstrings(string s) {
        // Prepare a lookup table to avoid calculating remainders inside the main loops
        int trans[10][10][10];
        
        for (int d = 1; d < 10; ++d) {
            for (int r = 0; r < d; ++r) {
                for (int c = 0; c < 10; ++c) {
                    trans[d][r][c] = (r * 10 + c) % d;
                }
            }
        }
        
        long long totalValidSubstrings = 0;
        
        // We only need two tables to toggle between previous and current states
        // dp[toggleIndex][divisor][remainder]
        long long dp[2][10][10] = {0};
        int cur = 0;
        
        for (char ch : s) {
            // Using bitwise XOR with 48 safely converts the ASCII character to an integer
            int c = ch ^ 48; 
            
            // The bitwise XOR with 1 toggles between 0 and 1
            int nxt = cur ^ 1;
            
            // Completely clear out the next state array before populating it
            for (int d = 1; d < 10; ++d) {
                for (int r = 0; r < d; ++r) {
                    dp[nxt][d][r] = 0;
                }
            }
            
            // Transfer and update all previous combinations using the new digit
            for (int d = 1; d < 10; ++d) {
                for (int r = 0; r < d; ++r) {
                    long long cnt = dp[cur][d][r];
                    if (cnt > 0) {
                        int nextRemainder = trans[d][r][c];
                        dp[nxt][d][nextRemainder] += cnt;
                    }
                }
                
                // Introduce the brand new single digit substring
                int soloRemainder = trans[d][0][c];
                dp[nxt][d][soloRemainder]++;
            }
            
            // If the current digit is not zero, count the substrings that leave a zero remainder
            if (c != 0) {
                totalValidSubstrings += dp[nxt][c][0];
            }
            
            // Swap the active array for the next character in the string
            cur = nxt;
        }
        
        return totalValidSubstrings;
    }
};

```

---

## A Detailed Trace of a Small Example

To ensure the concepts are crystal clear, let us manually trace the logic using a tiny string: "12".

### Processing Character '1'

The character is '1', so our digit `C` is 1.
We clear the `nxt` table.
Since it is the very first character, the `cur` table is completely empty. We do not have any existing substrings to extend.
We add the solo digit as a new substring. For every divisor `D` from 1 to 9, we calculate the remainder of 1 modulo `D`.
For instance, for `D` equal to 2, the remainder is 1. So `dp[nxt][2][1]` becomes 1.
For `D` equal to 1, the remainder is 0. So `dp[nxt][1][0]` becomes 1.
After checking all divisors, we look at `C`. Our `C` is 1, which is not zero. We look at `dp[nxt][1][0]`. The value is 1. We add 1 to our running total.
We swap `cur` and `nxt`.

### Processing Character '2'

The character is '2', so our digit `C` is 2.
We clear the `nxt` table.
Now we have old substrings to extend!
Let us look at `D` equal to 1. The old remainder was 0. The new remainder is `(0 * 10 + 2) modulo 1`, which is 0. We carry over the count. `dp[nxt][1][0]` gets 1.
Let us look at `D` equal to 2. The old remainder was 1. The new remainder is `(1 * 10 + 2) modulo 2`, which is 12 modulo 2, giving 0. We carry over the count. `dp[nxt][2][0]` gets 1.
After extending all old states, we add the solo digit '2'.
For `D` equal to 2, the solo remainder is 2 modulo 2, which is 0. We increment `dp[nxt][2][0]`. It was 1, so now it becomes 2.
After checking all divisors, we look at our `C`. Our `C` is 2, which is not zero. We look at `dp[nxt][2][0]`. The value is 2. (These correspond to the valid substrings "12" and "2"). We add 2 to our running total.

The final total is 3. The valid substrings are "1", "12", and "2". Our logic worked perfectly!

---

## Evaluating the Algorithmic Complexity

It is always important to evaluate how our solution scales with larger inputs. Let us break down the complexity of our approach.

### Time Complexity Evaluation

Let us define N as the total number of characters in the input string.
For every single character in the string, we perform a fixed number of operations. We loop through 9 possible divisors. For each divisor `D`, we loop through its possible remainders, which go from 0 up to `D` minus 1.

The total number of inner loop iterations for a single character is exactly the sum of numbers from 1 to 9. One plus two plus three plus four plus five plus six plus seven plus eight plus nine equals exactly 45.

Because 45 is a constant number, the time it takes to process each character does not grow. We do exactly 45 constant time operations for every character. Therefore, the overall runtime scales directly and linearly with the length of the string. We denote this as an Order of N time complexity. This is an incredibly practical and workable solution that easily passes the stringent time limits!

### Memory Complexity Evaluation

Let us define D as the maximum number of divisors, which is a constant 9.
Our lookup table `trans` has dimensions 10 by 10 by 10. This takes an extremely small, constant amount of memory.
Our toggling tracking arrays have dimensions 2 by 10 by 10. This also takes a tiny, constant amount of memory.

Since the memory we allocate does not grow at all regardless of how long the input string gets, our memory usage is completely constant. We denote this as an Order of 1 memory complexity. It is highly optimized and leaves a minimal footprint on the system resources.

---

## Final Thoughts and Community Encouragement

Tackling string problems that involve numerical divisibility can be incredibly intimidating at first glance. The sheer number of permutations and the massive integer values can easily lead a programmer down the wrong path of brute force simulation.

However, by taking a step back and applying the beautiful rules of modulo arithmetic, we discovered a way to transform a seemingly impossible simulation into a smooth, rolling wave of states. Keeping track of remainders instead of full numbers is a powerful pattern that appears in many advanced programming puzzles. Once you internalize this pattern, you will find yourself equipped to solve a wide variety of similar challenges!

I sincerely hope this thorough breakdown has been helpful to you on your coding journey. Writing out the logic and explaining the math in plain English is a wonderful way to solidify understanding, and I am thrilled to share these insights with the community. Keep practicing, stay curious, and never be afraid to break a complex problem down into manageable, logical pieces.

Happy coding, and see you in the next puzzle discussion!
