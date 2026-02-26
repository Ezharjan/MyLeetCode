# Counting Collections of Array Elements Where the Total Sum Falls Between Two Given Limits [#2902]

Hello there! Welcome to this friendly discussion where we will explore a fascinating programming puzzle together. I am very happy to share my thoughts and a complete walkthrough for this interesting challenge. We will take our time, break down every single piece of logic, and build up a clear understanding step by step. Whether you are studying for your next big interview or just solving puzzles for fun, I hope you find this guide helpful and easy to follow. 

In this article, we are going to look at a problem that asks us to find the number of specific collections we can make from a list of numbers. The goal is to ensure the sum of the elements in each collection falls securely within a specific range.

Grab a comfortable seat, maybe a warm beverage, and let us dive in!

***

## Understanding the Core Problem

Let us start by clearly defining what we are trying to achieve. 

Imagine you have a large bucket filled with numbers. These numbers are all integers zero and above. You are given a lower limit and an upper limit. Your task is to reach into the bucket, pull out a handful of numbers, and add them all together. If the total sum of the numbers in your hand is equal to or greater than the lower limit, and simultaneously equal to or less than the upper limit, then you have found a valid collection. 

The question asks us to count exactly how many distinct valid collections we can form. 

There are a few special rules we need to keep in mind

* **Order Does Not Matter** A collection of numbers like 1, 2, and 3 is considered exactly the same as 3, 2, and 1. We only care about how many of each number we have selected, not the sequence in which we picked them.
* **Duplicates Are Allowed but Limited** If the number 2 appears three times in our original bucket, we can choose to include the number 2 zero times, one time, two times, or a maximum of three times in our collection. We cannot include it four times because we only have three copies available. 
* **Empty Collections** If we pick absolutely nothing from the bucket, our total sum is exactly 0. 

Because the final count of these valid collections can be astronomically large, the problem asks us to return our final answer modulo 1000000007. This simply means we divide our final answer by 1000000007 and only return the remainder. This keeps the numbers within standard integer limits and prevents overflow issues in our computer memory.



***

## Why a Simple Approach Struggles

When faced with a problem like this, our first instinct might be to look at every single number in the array and make a choice we either include it in our current collection, or we leave it behind. We could write a recursive function to explore all these branches.

However, let us think about the scale of the problem. The array could contain up to twenty thousand numbers. If we branch two ways for every single number, we would create a decision tree with two to the power of twenty thousand branches. That number is so large it surpasses the number of atoms in the observable universe! It would take our computers billions of years to calculate.

Clearly, we need a smarter plan. Instead of looking at each individual element, we can group identical elements together.

***

## A Better Way Forward Grouping by Frequency

Think about how you count loose change. If you have a pile of identical coins, you do not count them one by one saying "here is a penny, here is another penny, here is a third penny". Instead, you group them. You say "I have ten pennies". 

We can apply this exact same logic to our array of numbers. We can create a frequency map or an array that tallies up how many times each unique number appears.

For example, if our array is 1, 2, 2, 3, we simply record that
* The number 1 appears exactly 1 time.
* The number 2 appears exactly 2 times.
* The number 3 appears exactly 1 time.

Now, instead of making a yes or no decision for every single item, we make a "how many" decision for every unique value. For the number 2, we ask "should we take zero of them, one of them, or two of them?"

This groups our redundant calculations and drastically reduces the amount of work we have to do.

***

## Handling the Magic of Zero

Before we get into the heavy lifting of calculating sums, we need to talk about the number zero. Zero is very special in this puzzle.

If you have a collection of numbers that sums to 5, and you add a zero to it, the sum remains exactly 5. Adding zero changes the composition of your collection, making it a new distinct sub multiset, but it does absolutely nothing to the total sum.

Because zero does not affect the sums, we can completely ignore all the zeros in our array while we do our main calculations. We simply count how many zeros we have in total. Let us say we have exactly five zeros in our array. When we finally build a valid collection of non zero numbers, we have several choices for the zeros
* We can include 0 zeros.
* We can include 1 zero.
* We can include 2 zeros.
* We can include 3 zeros.
* We can include 4 zeros.
* We can include 5 zeros.

That gives us exactly six choices. Therefore, the rule for zeros is simple count them up, add one to that count, and multiply our final result by this number right at the very end.

***

## The Dynamic Programming Blueprint

To solve this problem within the time limits, we will use a technique called Dynamic Programming. Dynamic programming is a method of solving complex problems by breaking them down into simpler overlapping subproblems, and storing the results so we do not have to compute them again.



We will create an array called `dp`. Let us define what this array represents. The value at `dp[j]` will store the total number of valid ways we can form the exact sum `j` using the numbers we have processed so far.

At the very beginning, we have processed zero numbers. There is exactly one way to make a sum of 0 taking nothing. So, we set `dp[0]` equal to 1. All other sums are currently impossible, so they start at 0.

We also need to know how big to make our `dp` array. The problem states we want sums up to a maximum upper limit. However, if the sum of all elements in our array is smaller than the upper limit, we only need to calculate up to the total sum of the array. Thus, the size of our `dp` array will be the minimum between the requested upper limit and the total sum of all numbers. Let us call this maximum target `S`.

***

## Processing the Numbers The Bounded Knapsack Trick

Now comes the beautiful part of the logic. We will look at each unique non zero number one by one. Let us say we are currently looking at a value called `v`, and we have `c` copies of it available. 

We need to update our `dp` array to include the new ways we can form sums by adding copies of `v`. A straightforward loop would look at every possible sum, and try adding 1 copy, 2 copies, all the way up to `c` copies. While this works, it is slightly too slow for the maximum constraints of this puzzle.

Instead, we can achieve the exact same result in two sweeping passes across the array. This is a brilliant optimization for what is known as the Bounded Knapsack problem.

### Step One The Forward Pass

First, we do a forward sweep. We go from left to right, starting at the value `v` and going all the way up to our maximum sum `S`.

For each position `j`, we say the number of ways to make sum `j` increases by the number of ways we can make the sum `j minus v`. 

Why? Because if we have a valid collection that adds up to `j minus v`, we can simply drop one more copy of `v` into it, and suddenly we have a collection that adds up to exactly `j`. 

This forward sweep implicitly assumes we have an infinite supply of the number `v`. As we sweep forward, we keep adding `v` over and over again.

### Step Two The Backward Pass

Of course, we do not have an infinite supply. We only have `c` copies. If we try to use `c` plus 1 copies, we have broken the rules. 

We need to fix the overcounting caused by the forward pass. We do this by doing a backward sweep. 

We start from the maximum sum `S` and walk backwards. The value that represents using one too many copies is `(c plus 1) times v`. Let us call this the `limit`.

For any sum `j` that is greater than or equal to this `limit`, we need to deduct the invalid combinations. The invalid combinations are exactly the ways to make the sum `j minus limit`. By subtracting these out, we cleanly slice off the invalid scenarios where we used more copies of `v` than we actually possess.

This two pass method is mathematically solid and runs much faster than testing every single count individually. 



***

## Managing the Math and Modulo

Whenever we do our forward pass, we are adding values together. Because the problem asks for the answer modulo 1000000007, we must ensure our sums do not grow endlessly. After we add a value, we check if it is greater than or equal to 1000000007. If it is, we simply deduct 1000000007 from it. This keeps the number wrapped properly.

During the backward pass, we are deducting values. This means our number could accidentally drop below zero! In computer arithmetic with modulo, negative numbers can cause unexpected bugs. To keep things safe, whenever we do a deduction, we check if the result is less than zero. If it is negative, we immediately add 1000000007 back to it to make it a positive equivalent.

This careful handling of numbers ensures our final counts remain absolutely perfectly accurate.

***

## Putting It All Together A Walkthrough

Let us solidify our understanding by walking through Example 1 from the problem description.

The array is 1, 2, 2, 3. The lower limit is 6, and the upper limit is 6.

First, let us find our total sum
1 plus 2 plus 2 plus 3 equals 8.

Our target maximum sum `S` is the minimum of our upper limit 6 and our total sum 8. So, our `dp` array needs to go up to 6.

We count our frequencies
* Value 1 count 1
* Value 2 count 2
* Value 3 count 1

We start our `dp` array with `dp[0]` equal to 1. All other values are 0.

**Processing Value 1 (count 1)**
* Forward pass We add `dp[j minus 1]` to `dp[j]` for all valid `j`.
* Backward pass Our limit is `(1 plus 1) times 1` which equals 2. We deduct `dp[j minus 2]` from `dp[j]` walking backwards.
This correctly updates our table so that sums 0 and 1 have exactly 1 way.

**Processing Value 2 (count 2)**
* Forward pass We add `dp[j minus 2]` to `dp[j]`. This assumes infinite 2s.
* Backward pass Our limit is `(2 plus 1) times 2` which equals 6. We deduct `dp[j minus 6]` from `dp[j]`. This corrects for having only two 2s.

**Processing Value 3 (count 1)**
* Forward pass We add `dp[j minus 3]` to `dp[j]`.
* Backward pass Our limit is `(1 plus 1) times 3` which equals 6. We deduct `dp[j minus 6]`.

After all these loops finish entirely, we look at our final `dp` array. We want to know how many ways to make sums between our lower limit 6 and upper limit 6. We simply look at `dp[6]`.

If we run through the logic, we find `dp[6]` holds the value 1. The only valid collection is the numbers 1, 2, and 3.

Since there are no zeros in our original array, we multiply our answer by 1. The final answer is perfectly 1.

***

## Complexity Breakdown

It is always a good idea to understand how much memory and time an approach requires. 

**Time Requirement** Finding the frequencies of the numbers requires looking at each number once. This is very fast.
The main work happens in the dynamic programming updates. We only process unique non zero values. In the worst case, the maximum sum is 20000. The number of unique values whose sum fits into 20000 is surprisingly small roughly around 200 distinct values at maximum. 
For each unique value, we sweep over our `dp` array twice. The length of the array is at most 20000. 
Therefore, the maximum operations are roughly 200 distinct values multiplied by 20000 array cells multiplied by 2 passes. This takes only a few million simple operations, which takes a tiny fraction of a second in modern computer languages. This is a very solid and completely safe approach for the limits given.

**Space Requirement**
We need an array to store the frequencies of numbers, which takes space proportional to the maximum number possible roughly 20005.
We also need our `dp` array, which takes space up to the maximum target sum `S`, also roughly 20005. 
The space requirements are very small and comfortably fit within memory limits.

***

## The Source Code Implementation

Below is the complete, nicely formatted code in C++ based on all the logic we just discussed. I have included detailed comments inside the code block to help you match the logic to the syntax. 

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int countSubMultisets(vector<int>& nums, int l, int r) {
        // This is our modulo base to keep numbers from overflowing
        const int MOD = 1000000007;
        
        // This array counts how many times each specific number appears.
        // The problem states numbers go up to 20000, so we size it accordingly.
        vector<int> count(20005, 0);
        int total_sum = 0;
        
        // First, we tally the frequencies of every number and find the total sum.
        for (int num : nums) {
            count[num]++;
            total_sum += num;
        }
        
        // S is the maximum sum we actually need to calculate.
        // It is the smaller of the requested upper limit or the total sum possible.
        int S = min(r, total_sum);
        
        // If the requested lower limit is physically larger than all our numbers combined,
        // it is impossible to reach. We can safely return 0 ways.
        if (l > total_sum) {
            return 0;
        }
        
        // This is our main dynamic programming table.
        // dp[j] stores the number of ways to exactly reach the sum j.
        vector<int> dp(S + 1, 0);
        
        // There is exactly 1 way to make a sum of 0 using zero elements.
        dp[0] = 1; 
        
        // We now iterate through every possible unique positive integer value.
        // We stop at the minimum of 20000 or our target sum S.
        for (int v = 1; v <= min(20000, S); ++v) {
            
            // We only process this value if it actually exists in our original array.
            if (count[v] > 0) {
                int c = count[v];
                
                // The Forward Pass (Adding ways assuming infinite supply)
                for (int j = v; j <= S; ++j) {
                    dp[j] += dp[j - v];
                    if (dp[j] >= MOD) {
                        dp[j] -= MOD;
                    }
                }
                
                // The Backward Pass (Removing the invalid overcounted ways)
                int limit = (c + 1) * v;
                for (int j = S; j >= limit; --j) {
                    dp[j] -= dp[j - limit];
                    if (dp[j] < 0) {
                        dp[j] += MOD;
                    }
                }
            }
        }
        
        // We will accumulate our valid answers in a larger variable to prevent bounds issues.
        long long ans = 0;
        
        // We add up all the ways to make valid sums that fall within our target range.
        for (int j = l; j <= S; ++j) {
            ans += dp[j];
        }
        
        // We apply our modulo one last time to the accumulated sum.
        ans %= MOD; 
        
        // Finally, we account for the zeros. 
        // If we have Z zeros, we have (Z plus 1) choices for including zeros.
        // We multiply our answer by this choice count.
        ans = (ans * (count[0] + 1)) % MOD;
        
        return ans;
    }
};
```

***

## Final Thoughts

This problem is a fantastic exercise in recognizing patterns. Initially, it looks like a standard subset generation problem which would run far too slowly. By recognizing that we are dealing with multiple identical items, we transform it into a bounded knapsack problem. Then, by applying the clever two pass correction method, we squeeze the performance into a highly acceptable timeframe.

It is perfectly normal if the forward and backward passes feel a bit like magic at first glance. I highly recommend tracing it out on a piece of paper with a very small example, like one value with a count of two. Seeing the numbers add up and then perfectly subtract the excess is a genuinely satisfying "aha!" moment.

I hope this detailed explanation has shed some bright light on this logic. Thank you so much for reading through this walkthrough. Keep practicing, keep analyzing, and most importantly, keep enjoying the process of learning and solving these puzzles. 

Happy coding to all of you in the community!
