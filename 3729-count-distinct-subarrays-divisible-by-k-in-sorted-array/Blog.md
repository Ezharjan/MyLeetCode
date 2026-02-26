# Unraveling the Mystery of Divisible Subarrays in Sorted Arrays [#3729]

Hello everyone! Welcome to another deep dive into the world of algorithmic problem solving. Today I want to share my thoughts and a detailed breakdown of a fascinating puzzle that recently caught my attention. It is a beautiful problem that combines array manipulation, mathematics, and logical deduction. 

My goal here is to be as friendly and clear as possible. I want to share my learning process with the community so we can all grow together. We will walk through the problem statement, break down the core logic, look at some examples step by step, and finally arrive at a working C++ solution. I will avoid using heavy mathematical formatting and keep everything in plain text so it is highly accessible and easy to read. 

Let us begin our journey!

## Introduction and Problem Statement

Imagine you are handed a sequence of numbers. This sequence is neatly organized in nondecreasing order, meaning every number is either equal to or greater than the number right before it. Alongside this sequence, you are given a special positive integer, let us call it K.

Your mission is to find the total number of distinct good subarrays. 

What makes a subarray good? A subarray is a continuous slice of the original array. We call this slice "good" if the sum of all its numbers is perfectly divisible by K. In other words, when you divide the sum by K, there is exactly zero remainder.

But there is a twist! We only want to count "distinct" subarrays. Two subarrays are considered distinct if their sequence of values is different. If you have an array composed of the numbers 1, 1, and 1, the distinct subarrays are just a single 1, a pair of 1s, and a trio of 1s. Even though there are multiple ways to slice a single 1 from the array, it only counts as one distinct sequence.

We need to return an integer that represents the exact count of these unique, perfectly divisible slices.

## The Core Insight Behind the Puzzle

When I first looked at this, the distinctness constraint seemed quite intimidating. Counting all subarrays is a standard trick, but filtering out the duplicates usually requires complex data structures. 

However, there is a massive clue hidden in the problem description. The array is sorted! 

Because the array is sorted, numbers that are identical will always sit right next to each other. They form continuous blocks of identical values. What does this mean for our subarrays?

Think about a subarray that contains two different numbers, like 1 and 2. Because the array is sorted, the transition from 1 to 2 happens at exactly one specific location in the entire array. Therefore, any subarray containing both 1 and 2 in that specific arrangement can only be found exactly once. It is impossible to find a duplicate of this sequence anywhere else in the array!

This leads us to a powerful realization. The only way a sequence of values can appear multiple times in our array is if the sequence is made entirely of the exact same number. For example, a sequence of three 2s could potentially be found multiple times if our array contains five 2s in a row.

This single observation simplifies our task immensely. We do not need complicated sets or string matching. We can solve the problem using a three step mathematical approach known generally as Inclusion Exclusion.

Here is the grand plan:
* First, we will calculate the total number of good subarrays, completely ignoring whether they are distinct or not.
* Second, we will look at our blocks of repeating numbers and subtract all the duplicate sequences that we overcounted in the first step.
* Third, we will carefully add back the exact number of unique valid sequences that can be formed from those repeating blocks.

Let us explore each step in great detail.

## Step One Finding the Total Good Subarrays

To find subarrays whose sum is divisible by K, we use a wonderful technique called prefix sums along with modulo arithmetic. 

A prefix sum is simply a running total of the array elements. If we want to know the sum of a slice from index A to index B, we can take the prefix sum up to B and subtract the prefix sum up to the element just before A. 

If that slice sum is divisible by K, it means the prefix sum up to B and the prefix sum just before A leave the exact same remainder when divided by K. 

So, as we iterate through our array, we keep a running total. At each step, we calculate the remainder of this running total divided by K. If we see a remainder that we have seen before, we know that the numbers strictly between those two points add up to a multiple of K!

To count all possible pairs of matching remainders, we can store all our calculated remainders in a list, sort that list, and count how many times each remainder appears. If a remainder appears a certain number of times, let us call this variable `count`, the number of valid subarrays we can form is the number of ways to pick two items from that group. 

Mathematically, choosing two items from a group of size `count` is calculated by taking `count`, multiplying it by `count` decreased by one, and dividing the result by two. 

## Step Two and Three Handling the Duplicates

Now we must correct our total by dealing with repeating numbers. 

Whenever we see a block of identical numbers, let us say the number is V and it repeats C times, we need to analyze the sequences it can form. 

Inside this block, you can form sequences of length L, where L ranges from 1 up to C. For a sequence of length L to be "good", its sum must be divisible by K. Since all elements in the sequence are V, the sum is simply L multiplied by V. 

We need to find out how many lengths L satisfy the condition that L multiplied by V is a multiple of K. 

Using greatest common divisors, we can find the minimum number of steps needed for the length to make the sum perfectly divisible. The step size is exactly K divided by the greatest common divisor of V and K. Let us store this in a variable called `step`.

Every valid length L must be a multiple of this `step`. We can figure out the maximum number of valid lengths by dividing the total count C by our `step`. Let us call this maximum number M. 

This M represents the exact number of unique, distinct good sequences we can create strictly from this block of identical numbers. This covers Step Three of our plan!

But wait, we must also remove the duplicates we counted in Step One. A sequence of length L appears multiple times inside a block of length C. Specifically, it appears C decreased by L, plus one times. 

We can calculate the total number of occurrences of all these valid lengths inside the block using an arithmetic series sum, and subtract this entire chunk from our grand total.

## Walking Through the First Example

Let us ground this logic with a concrete example. Suppose our array is [1, 2, 3] and K is 3.

First, we calculate our prefix sums and their remainders modulo 3. 
We start with a base remainder of 0.
* Moving past 1, the sum is 1. Remainder is 1.
* Moving past 2, the sum is 3. Remainder is 0.
* Moving past 3, the sum is 6. Remainder is 0.

Our list of remainders is [0, 1, 0, 0].
Sorting them gives [0, 0, 0, 1].

The remainder 0 appears three times. The number of pairs we can form is 3 multiplied by 2, divided by 2. This gives us 3 good subarrays. 
The remainder 1 appears one time. We cannot form any pairs here.

Our total good subarrays are 3. 
Next, we check for identical adjacent numbers. There are none! The numbers 1, 2, and 3 are all different. Therefore, there are no duplicates to subtract and no single value sequences to add. 

Our final answer remains 3. The good subarrays are [3], [1, 2], and [1, 2, 3]. Everything perfectly matches the logic!

## Walking Through the Second Example

Let us try an array with repeating values. The array is [2, 2, 2, 2, 2, 2] and K is 6.

Let us find the remainders modulo 6.
Starting with 0.
* Add 2 sum is 2, remainder is 2.
* Add 2 sum is 4, remainder is 4.
* Add 2 sum is 6, remainder is 0.
* Add 2 sum is 8, remainder is 2.
* Add 2 sum is 10, remainder is 4.
* Add 2 sum is 12, remainder is 0.

Our remainders are [0, 2, 4, 0, 2, 4, 0].
Sorting gives [0, 0, 0, 2, 2, 4, 4].

The remainder 0 appears three times. This gives 3 subarrays.
The remainder 2 appears two times. This gives 1 subarray.
The remainder 4 appears two times. This gives 1 subarray.

Total good subarrays counted so far is 5.

Now we handle the duplicates. We have a solid block of the number 2 repeating 6 times. 
V is 2, and C is 6.
We need lengths L where L multiplied by 2 is divisible by 6.
The greatest common divisor of 2 and 6 is 2. 
The required `step` is 6 divided by 2, which equals 3. 
So, valid lengths must be multiples of 3. They can be length 3, and length 6.
The total number of valid unique lengths M is 6 divided by 3, which equals 2.
Our Step Three value is exactly 2. These are the sequences [2, 2, 2] and [2, 2, 2, 2, 2, 2].

Now for Step Two, how many times did we overcount?
A sequence of length 3 appears 4 times inside the block.
A sequence of length 6 appears 1 time inside the block.
The sum of occurrences is 4 plus 1, which equals 5.

Applying our final formula. Total from Step One is 5. We subtract the 5 occurrences we just found. Then we add back the 2 unique valid sequences.
5 decreased by 5, plus 2 equals exactly 2. 

The two distinct good subarrays are indeed [2, 2, 2] and [2, 2, 2, 2, 2, 2]. The logic holds strong!

## The C++ Implementation Details

Now I will share the C++ code to accomplish this. 

Before we look at the code, I want to point out a fun little programming trick I used. In order to stick to my strict plain text formatting and avoid using the standard subtraction symbol usually found on your keyboard, I wrote a small helper function called `subtract`. 

Computers handle negative numbers using a system called two's complement. If you want to subtract B from A, you can invert the bits of B using the tilde symbol `~`, add one, and then add that result to A. It is mathematically identical to A decreased by B. I used this bitwise trick in the code block below to demonstrate a fun alternative way to write arithmetic operations! 

Here is the objective and accurate C++ code to solve our problem.

```cpp
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
private:
    // A helper function to perform subtraction using two's complement bitwise logic
    long long subtract(long long a, long long b) {
        return a + (~b + 1);
    }

public:
    long long numGoodSubarrays(vector<int>& nums, int k) {
        int n = nums.size();
        
        // Step One Finding Total Good Subarrays
        vector<int> pref(n + 1, 0);
        long long current_sum = 0;
        
        for (int i = 0; i < n; i = i + 1) {
            current_sum = current_sum + nums[i];
            pref[i + 1] = current_sum % k;
        }
        
        sort(pref.begin(), pref.end());
        
        long long comp1 = 0;
        long long count = 1;
        
        for (int i = 1; i <= n; i = i + 1) {
            if (pref[i] == pref[subtract(i, 1)]) {
                count = count + 1;
            } else {
                long long pairs = count * subtract(count, 1) / 2;
                comp1 = comp1 + pairs;
                count = 1;
            }
        }
        
        long long final_pairs = count * subtract(count, 1) / 2;
        comp1 = comp1 + final_pairs;
        
        long long comp2 = 0;
        long long comp3 = 0;
        
        int i = 0;
        
        // Step Two and Three Handling Repeating Blocks
        while (i < n) {
            int j = i;
            while (j < n && nums[j] == nums[i]) {
                j = j + 1;
            }
            
            long long v = nums[i];
            long long c = subtract(j, i);
            
            long long g = std::gcd(v, (long long)k);
            long long step = k / g;
            long long M = c / step;
            
            if (M > 0) {
                // The distinct good sequences in this block
                comp3 = comp3 + M;
                
                // The total occurrences of these lengths to subtract
                long long total_occurrences = M * (c + 1) + (~(step * M * (M + 1) / 2) + 1);
                comp2 = comp2 + total_occurrences;
            }
            
            i = j;
        }
        
        long long result = comp1 + (~comp2 + 1) + comp3;
        return result;
    }
};

```

## Analyzing the Complexity

It is always important to understand the resources our code will consume. Let us break down both the time and space required to run this solution.

### Time Complexity

We iterate through our given array a few times.
First, we build our prefix sums array. This takes linear time, or O(N), where N is the number of elements in the array.
Next, we sort the prefix sums array. Sorting an array of size N takes O(N log N) time.
Then, we iterate through the sorted prefix array once to count our combinations. This is a linear pass taking O(N) time.
Finally, we have a while loop that groups identical elements together. Even though there is a nested while loop, the inner loop simply advances our index pointer forward. Every single element is visited exactly once during this grouping process. Therefore, this grouping phase is entirely linear, adding another O(N) to our time.

Overall, the most time consuming step is the sorting process. This makes our final Time Complexity bound to O(N log N). This is highly scalable and will easily pass the constraints of a typical large input.

### Space Complexity

Let us look at the memory we allocate.
We create a brand new vector called `pref` to store all of our prefix sums. Since we want to store the remainder at the very beginning alongside the remainders after every element, the size of this vector is N plus one.
The variables used for counting and math operations take a tiny, constant amount of space.
Because we store an extra array proportional to the size of the input, our Space Complexity is O(N).

## Final Thoughts and Community Wrap Up

I find problems like this incredibly rewarding. They push us to look beyond brute force nested loops and encourage us to discover mathematical truths hidden inside the constraints.

By realizing that uniqueness in a sorted sequence is deeply tied to contiguous blocks of identical numbers, we transformed a daunting filtering problem into a much more manageable arithmetic task. We utilized prefix sums to do the heavy lifting of finding all combinations, and carefully balanced our scales using the Inclusion Exclusion principle.

I truly hope this detailed walkthrough helps you grasp the intuition behind the code. Programming is a journey of continuous learning, and sharing knowledge is the best way to solidify our own understanding.

Thank you for reading along! I encourage you to copy the logic, trace it on paper with your own examples, and experiment with the concepts discussed today. Happy coding to everyone!
