# Discovering How to Maximize Combined Array Strength by Selecting Exactly K Disjoint Subarrays [#3077]

Hello everyone and welcome back to my coding corner! Today I want to share a thorough walkthrough of a very interesting algorithmic puzzle. If you enjoy breaking down arrays and finding creative ways to group numbers to achieve a massive score, you are in for a real treat. We will explore how to select completely separate segments from a list of numbers to get the highest possible strength. 

I will try my best to explain the core concepts in a friendly and step by step manner so that whether you are a beginner or a seasoned coder, you will walk away with a solid understanding of how to tackle this logic. We will avoid complicated math symbols and keep things conversational and clear.



### Understanding the Problem Setup

Imagine you are given a long list of numbers. Some are positive, some are zero, and some are negative. You are also given a specific odd number, which we will call K. Your mission is to carefully pick exactly K different segments from this list. 

There are a few strict rules we must follow:
1. Every segment you pick must be a contiguous block of numbers from the original list. We call this a subarray.
2. The subarrays must be completely separate. They cannot overlap at all. The last number of the first segment must appear before the first number of the second segment, and so forth.
3. You do not have to use all the numbers in the array. You can skip numbers between your chosen segments, or at the very beginning, or at the very end.

Now, how do we score these selected segments? We calculate a total "strength" by adding the sums of the elements in each of our chosen segments, but with a twist. Each segment is multiplied by a specific weight, and the sign of that weight alternates!

Here is how the scoring works for our K chosen segments:
* The first segment's sum is multiplied by K.
* The second segment's sum is multiplied by K minus 1, and this value is subtracted! 
* The third segment's sum is multiplied by K minus 2, and this value is added.
* This pattern continues, alternating between adding and subtracting, while the multiplier decreases by 1 each time.
* Finally, the very last segment, the Kth segment, is simply added as its multiplier has reduced down to 1.

Our goal is to figure out the maximum possible strength we can achieve by making the smartest choices of where these K segments start and end.

### Why Simple Approaches Fall Short

When we first look at this, it might be tempting to use a greedy strategy. For example, maybe we just find the segments with the largest positive sums and assign them to the positive multipliers, and find segments with the most negative sums to assign to the negative multipliers. 

However, this greedy approach quickly falls apart. The array limits us with the order of elements. If we pick a huge positive number early on for our third segment, we might not have enough space left to form the remaining segments. Alternatively, skipping some numbers might allow us to form a much better combination later on. Because every choice affects the valid choices for all subsequent segments, trying to guess the best local choice will not guarantee the best global score.

We need a structured way to explore the possibilities without recalculating things over and over. This leads us directly to Dynamic Programming.

### The Dynamic Programming Strategy

Dynamic Programming is a wonderful technique where we break our massive problem into smaller, bite sized subproblems. Instead of looking at the whole array at once, we look at the array element by element, and at each step, we make decisions based on the best results we have seen so far.



To build our strategy, we need to track our progress. At any given point while scanning our list of numbers from left to right, we need to know two things:
1. How many numbers have we looked at so far?
2. How many segments have we formed so far?

Let us establish our memory tables to keep track of our maximum scores. We will use two separate arrays for this to ensure we capture all edge cases correctly.

#### Defining Our States

We will define two memory structures:
* `f` array: This will hold the maximum strength if we have formed a specific number of segments, and the current number we are looking at is strictly included at the very end of the last formed segment. 
* `g` array: This will hold the absolute maximum strength we have seen so far for a specific number of segments, regardless of whether the current number is included in the segment or skipped completely.

Since we are moving through our list of numbers one by one, we can update these `f` and `g` values on the fly. We just need to keep track of how many segments we are forming, ranging from 1 up to K.

#### Formulating the Transitions

Let us imagine we are looking at a single number in our list, and we want to update our maximum scores for forming `j` segments. 

First, let us figure out the weight for the `j`th segment. As we discussed earlier, the multiplier starts at K and goes down. So the multiplier is K minus `j` plus 1. 
Next, we determine the sign. If `j` is an odd number (like the 1st, 3rd, or 5th segment), the sign is positive. If `j` is an even number (like the 2nd or 4th segment), the sign is negative. 
We combine the multiplier and the sign to get our final weight for the `j`th segment.

Now, how do we update the `f` array for `j` segments? 
Since the `f` array represents the best score where the current number MUST be part of the `j`th segment, we have two distinct choices:
Choice A: We extend the existing `j`th segment. We simply take the previous best score from the `f` array for `j` segments and add our current number multiplied by the weight.
Choice B: We start a completely fresh `j`th segment with our current number. To do this, we look at the absolute best score we had for `j` minus 1 segments (which is stored in our `g` array from the previous step) and add our current number multiplied by the weight.

We compare Choice A and Choice B, and whichever gives us the larger number becomes our new value for the `f` array at position `j`.

How do we update the `g` array for `j` segments?
This is much simpler! The `g` array tracks the absolute best score. So we just look at the highest value between:
Choice C: The previous best score in the `g` array for `j` segments (which means we simply skip adding the current number to any new segment).
Choice D: The new value we just calculated for the `f` array at position `j` (which means the current number is the end of our `j`th segment).

We take the maximum of these two choices and update our `g` array.

### Memory Optimization Techniques



If you think about it, to calculate the answers for the current number we are looking at, we only ever need the answers from the immediately previous number. We do not need to keep the entire history of every single number in the array. 

This means instead of using a massive two dimensional grid of memory that could take up a huge amount of space, we can just use two simple one dimensional lists of size K plus 1. One list for `f` and one list for `g`. As we move through our main list of numbers, we just overwrite our `f` and `g` lists. 

We just have to be a little bit careful. When updating the `j`th segment, Choice B relies on the `g` value from the `j` minus 1 segment from the previous step. We need to make sure we save that older `g` value into a temporary variable before we overwrite it, so it is ready to be used when we move on to calculating the next segment.

### A Complete Step By Step Walkthrough

Let us really solidify our understanding by walking through an example together. 
Suppose our list of numbers is: 1, 2, 3, negative 1, 2.
And we need to form K equals 3 segments.

Let us determine our weights for the segments first:
For segment 1: The multiplier is 3. It is an odd segment, so it is positive 3.
For segment 2: The multiplier is 2. It is an even segment, so it is negative 2.
For segment 3: The multiplier is 1. It is an odd segment, so it is positive 1.

We initialize our `f` and `g` lists with very small numbers (representing negative infinity) because we want to maximize our score. However, forming 0 segments gives a score of 0.

Now, let us process the numbers one by one.

Processing the first number: 1
For 1 segment: 
Weight is 3.
Start fresh: Previous `g` for 0 segments is 0. 0 plus 3 times 1 equals 3.
Extend: Previous `f` is negative infinity. 
Max for `f` is 3.
Max for `g` is the larger of negative infinity and 3, which is 3.

Processing the second number: 2
For 1 segment:
Weight is 3.
Start fresh: Previous `g` for 0 segments is 0. 0 plus 3 times 2 equals 6.
Extend: Previous `f` is 3. 3 plus 3 times 2 equals 9.
Max for `f` is 9.
Max for `g` is the larger of 3 and 9, which is 9.

For 2 segments:
Weight is negative 2.
Start fresh: Previous `g` for 1 segment was 3. 3 plus negative 2 times 2 equals negative 1.
Extend: Previous `f` is negative infinity.
Max for `f` is negative 1.
Max for `g` is negative 1.

Processing the third number: 3
For 1 segment:
Weight is 3.
Start fresh: 0 plus 3 times 3 equals 9.
Extend: Previous `f` is 9. 9 plus 3 times 3 equals 18.
Max for `f` is 18.
Max for `g` is 18.

For 2 segments:
Weight is negative 2.
Start fresh: Previous `g` for 1 segment was 9. 9 plus negative 2 times 3 equals 3.
Extend: Previous `f` was negative 1. Negative 1 plus negative 2 times 3 equals negative 7.
Max for `f` is 3.
Max for `g` is 3.

For 3 segments:
Weight is 1.
Start fresh: Previous `g` for 2 segments was negative 1. Negative 1 plus 1 times 3 equals 2.
Extend: Previous `f` is negative infinity.
Max for `f` is 2.
Max for `g` is 2.

Processing the fourth number: negative 1
For 1 segment:
Weight is 3.
Start fresh: 0 plus 3 times negative 1 equals negative 3.
Extend: Previous `f` is 18. 18 plus 3 times negative 1 equals 15.
Max for `f` is 15.
Max for `g` is the larger of 18 (previous best) and 15, which remains 18.

For 2 segments:
Weight is negative 2.
Start fresh: Previous `g` for 1 segment was 18. 18 plus negative 2 times negative 1 equals 20.
Extend: Previous `f` was 3. 3 plus negative 2 times negative 1 equals 5.
Max for `f` is 20.
Max for `g` is the larger of 3 and 20, which is 20.

For 3 segments:
Weight is 1.
Start fresh: Previous `g` for 2 segments was 3. 3 plus 1 times negative 1 equals 2.
Extend: Previous `f` was 2. 2 plus 1 times negative 1 equals 1.
Max for `f` is 2.
Max for `g` is the larger of 2 and 2, which remains 2.

Processing the fifth number: 2
For 1 segment:
Weight is 3.
Start fresh: 0 plus 3 times 2 equals 6.
Extend: Previous `f` is 15. 15 plus 3 times 2 equals 21.
Max for `f` is 21.
Max for `g` is the larger of 18 and 21, which is 21.

For 2 segments:
Weight is negative 2.
Start fresh: Previous `g` for 1 segment was 18. 18 plus negative 2 times 2 equals 14.
Extend: Previous `f` was 20. 20 plus negative 2 times 2 equals 16.
Max for `f` is 16.
Max for `g` is the larger of 20 (previous best) and 16, which remains 20.

For 3 segments:
Weight is 1.
Start fresh: Previous `g` for 2 segments was 20. 20 plus 1 times 2 equals 22.
Extend: Previous `f` was 2. 2 plus 1 times 2 equals 4.
Max for `f` is 22.
Max for `g` is the larger of 2 and 22, which is 22.

After looking at all the numbers, our final answer is safely sitting in our `g` list for 3 segments. The value is 22! This perfectly matches the logic. We pick the segments [1, 2, 3], then [negative 1], and finally [2]. 

### The C Plus Plus Implementation

Here is the code block implementing the logic we just discussed. I have included comments throughout the code to help map our discussion to the actual syntax. Note that inside the C Plus Plus code block, standard mathematical subtraction signs are used as required by the compiler.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maximumStrength(vector<int>& nums, int k) {
        int n = nums.size();
        
        // We use a very small number to represent negative infinity.
        // This prevents integer overflow issues when calculating our max values.
        const long long MIN_INF = -1000000000000000000LL; 
        
        // f list tracks max strength of j segments where the jth segment
        // ends exactly at the current number we are evaluating.
        vector<long long> f(k + 1, MIN_INF);
        
        // g list tracks max overall strength of j segments up to the current number.
        vector<long long> g(k + 1, MIN_INF);
        
        // Base setup: 0 segments yield a score of 0.
        f[0] = 0;
        g[0] = 0;
        
        // Loop through each number in our input list.
        for (int i = 1; i <= n; ++i) {
            
            // prev_g will help us hold the previous step's g value.
            // For the first inner loop where j is 1, we need g[0] which is 0.
            long long prev_g = g[0]; 
            
            // We only need to check up to the minimum of i and k segments.
            // You cannot form 3 segments if you only have 2 numbers!
            int limit = min(i, k);
            for (int j = 1; j <= limit; ++j) {
                
                // Calculate the multiplier weight for the jth segment.
                // It alternates signs based on whether j is odd or even.
                long long w = (long long)(k - j + 1) * (j % 2 == 1 ? 1 : -1);
                
                // Save the current g[j] before we overwrite it.
                // It will become the prev_g for the next iteration where we look at j plus 1.
                long long temp_g = g[j];
                
                // Maximize whether we extend the current jth segment or start a brand new jth segment.
                long long next_f = max(f[j], prev_g) + w * nums[i - 1];
                
                // Update our f and g lists for the current number.
                f[j] = next_f;
                g[j] = max(g[j], f[j]);
                
                // Carry over the old g[j] to act as prev_g in the next loop.
                prev_g = temp_g;
            }
        }
        
        // The last element of g will contain the optimal max strength combining exactly k separate segments.
        return g[k];
    }
};

```

### Complexity Breakdown

When looking at algorithmic solutions, it is always a good idea to understand the resource requirements.

Time Requirements: Let us denote the total number of elements in our array as N. For every single element N in the array, we run an inner loop that iterates up to K times. Inside the inner loop, we are only performing basic arithmetic operations like addition, multiplication, and finding the maximum of two numbers. These basic operations take a constant amount of time. Therefore, our total time spent scales by multiplying N and K. This is a highly effective method that easily passes the problem constraints within the time limits.

Space Requirements: In a naive dynamic programming approach, we might have created a massive table with N rows and K columns. However, we utilized a memory optimization technique. Notice how our `f` and `g` lists only have a size of K plus 1. We simply overwrite these small lists as we move forward through the N elements. Thus, the extra memory we need only scales with K. This is an incredibly light footprint and makes our solution both fast and very memory conscious.

### Final Thoughts

This array partitioning challenge is an excellent example of how breaking down a complex problem into smaller subproblems reveals an elegant solution. By carefully defining our memory states and mapping out the choices we have at each step, we turned a seemingly impossible combinatorial nightmare into a smooth, step by step calculation.

I hope this thorough walkthrough has demystified the logic for you! Dynamic programming can be tricky to wrap your head around at first, but tracing through examples step by step like we did today is one of the best ways to build your intuition.

Keep practicing, keep exploring different patterns, and most importantly, keep enjoying the process of problem solving. Happy coding to all of you, and I look forward to sharing more insights with the community next time!
