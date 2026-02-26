# Gathering Ones to Win the Game A Friendly Guide to Minimizing Moves [#3086]

Welcome everyone! Today, I am really excited to share my thoughts on a fascinating array puzzle that I recently spent some time studying. It is a wonderful exercise in logical deduction, careful observation, and applying sliding window techniques. We will walk through it together, breaking down the logic step by step so that everyone, regardless of their programming experience level, can understand the thought process and learn how to solve it. 

I love sharing my findings with the community, and I hope this detailed guide helps you grasp the underlying concepts. Grab a cup of coffee or tea, and let us dive into the wonderful world of arrays and logical algorithms!

## Understanding the Rules of the Game

Before we can solve any puzzle, we must thoroughly understand its rules. In this game, Alice is interacting with a binary array called `nums`, which means the array contains only zeros and ones. 



She has a specific goal. She wants to collect exactly `k` ones in the absolute minimum number of moves. At the very beginning of the game, Alice gets to choose her starting position. She can pick any index in the array to stand on. If she happens to stand on an index that already contains a one, she picks it up instantly, and this initial pickup costs absolutely zero moves.

After choosing her starting spot, Alice can perform any number of moves. Every move she makes must be one of the following two specific actions:

* **Action Type One (Creating a One):** Alice can magically select any index in the array that currently contains a zero and change it into a one. However, she does not have infinite magic. She can only perform this creation action up to a specific limit, which is given to us as the variable `maxChanges`. 
* **Action Type Two (Swapping a One):** Alice can select any two adjacent positions in the array where one position contains a one and the neighboring position contains a zero. She can swap their values. Essentially, this means she can slide an existing one over by one spot. If she slides a one onto the exact spot where she is currently standing, she immediately picks it up.

Our mission is to figure out the minimum number of moves Alice needs to perform to gather exactly `k` ones.

## Analyzing the True Cost of Collecting Ones

To find the minimum moves, we need to understand the "price tag" associated with gathering ones from different places. Let us imagine Alice has decided to stand at a specific index. How much does it cost to get a one from various distances into her hands?

* **Cost of a One at Distance Zero:** If there is already a one exactly where Alice is standing, the cost is zero moves. She picks it up for free before the game even officially starts counting moves.
* **Cost of a One at Distance One:** If there is a one immediately to her left or immediately to her right, she can use Action Type Two to swap it onto her position. This takes exactly one move.
* **Cost of a One at Distance Two or More:** If there is an existing one located further down the array, she will have to slide it over one spot at a time. Therefore, the cost in moves is exactly equal to the distance between that one and Alice. For example, a one that is three spots away will cost three moves to pull in.
* **Cost of a Created One:** What if Alice uses Action Type One to create a new one? She wants to minimize moves, so the smartest place to create a new one is directly adjacent to her. Creating it costs one move. Then, sliding it onto her position costs a second move. Thus, a created one will always cost exactly two moves.

## The Core Insight Comparing Created Ones vs Existing Ones

This cost analysis brings us to the most beautiful and critical realization of the entire problem. We need to compare the cost of creating a one versus the cost of fetching an existing one.

As we just established, creating a one and picking it up costs exactly two moves. 

Now, look at the cost of fetching existing ones. If an existing one is three steps away, it costs three moves. If it is four steps away, it costs four moves. 

Do you see the catch? It is actually cheaper to create a brand new one (which only costs two moves) than to drag an existing one from three or more steps away! 

Because of this, Alice should highly prefer using her `maxChanges` to create ones rather than walking far away to fetch existing ones. The only time she should ever prefer an existing one over a created one is if the existing one is incredibly close. Specifically, if it is at distance zero (costs zero moves) or distance one (costs one move). Even an existing one at distance two costs two moves, which simply ties the cost of creating a brand new one.

This completely changes how we look at the array. We no longer need to consider gathering ones from far away unless we absolutely have to.

## Determining How Many Existing Ones We Actually Need

Since we want to rely on creating ones as much as possible, let us figure out exactly how many existing ones we are actually forced to pick up.

Alice needs a total of `k` ones. She has `maxChanges` available to her. 

If `k` is less than or equal to `maxChanges`, she has enough magic to create almost all the ones she needs. However, she still might want to pick up ones that are directly next to her because they are cheaper than creating new ones. 

If `k` is greater than `maxChanges`, she simply does not have enough magic to create everything. She will use all her `maxChanges`, and for the remaining ones, she will be forced to fetch them from the existing ones in the array, no matter how far away they are.

Let us define a variable called `M`. `M` will represent the absolute minimum number of existing ones Alice is forced to collect. We can calculate this by subtracting `maxChanges` from `k`. If `maxChanges` is larger, then `M` is simply zero.

So, Alice is forced to collect `M` existing ones. 

Could she want to collect more than `M` existing ones? Yes, but only if they are super close to her (distance zero or one). How many ones can possibly be at distance zero or one? At most three! She can have one directly underneath her, one immediately to her left, and one immediately to her right. 

Therefore, she might voluntarily pick up to three additional existing ones.

This leads us to a massive breakthrough. The total number of existing ones Alice will pick up, let us call this number `L`, will always be within a very tight range. `L` will be at least `M` and at most `M + 3`. It cannot exceed `k`, and it cannot exceed the total number of ones currently in the array. 

Instead of guessing how many ones to create and how many to fetch, we only have to test a maximum of four possible values for `L`!

## The Magic of the Spatial Median

Now that we know we only need to test a few specific values for `L`, we face the next challenge. If we decide we want to collect exactly `L` existing ones from the array, which `L` ones should we pick, and where should Alice stand to minimize the walking distance?

Imagine we have a list of the positions of all the ones in the array. We want to select a continuous block of `L` ones from this list. 

Where should Alice stand to gather this block of `L` ones with the fewest moves? She should stand exactly in the middle of them. In mathematics and statistics, this middle point is known as the spatial median. 



There is a widely known property about the median. If you want to find a meeting point that minimizes the total travel distance for a group of people spread out along a straight line, you should always choose the median location. If you choose the average, extreme outliers can pull the meeting point away and increase the total distance. The median perfectly balances the number of items on the left and the right, ensuring the total absolute distance is as small as possible.

So, for any block of `L` ones, Alice should stand at the position of the one that is exactly in the middle of that block.

## Formulating a Sliding Window Strategy

We have all the pieces of the puzzle. We know we need to test a few sizes for `L`. For each size `L`, we need to find the best block of `L` ones in the array. 

We can achieve this using a sliding window technique. We will slide a window of size `L` across our list of one positions.



To make calculating the distance incredibly fast, we can use a technique called prefix sums. A prefix sum array keeps a running total of our values. By doing this, if we want to know the sum of any section of our array, we do not have to add them up one by one every time. We can just take the total sum up to the end of the section and subtract the total sum up to the beginning of the section. This gives us the answer instantly.



For a window of size `L` starting at index `i`, the median will be at index `m = i + L / 2`.
The total moves required to pull all the ones in this window to the median is composed of two parts:
* The distance for all the ones on the right side of the median.
* The distance for all the ones on the left side of the median.

Using our prefix sums, we can calculate these distances in a flash.

## A Detailed Walkthrough of the Strategy

Let us solidify this with a concrete, manual walkthrough using an example. 

Imagine our array is `[1, 1, 0, 0, 0, 1, 1, 0, 0, 1]`. 
We need to collect `k = 3` ones.
We are allowed `maxChanges = 1`.

First, let us find the positions of all existing ones. They are at indices 0, 1, 5, 6, and 9. 

Next, let us calculate `M`, the minimum existing ones we must take. 
`M` equals `k` minus `maxChanges`. That is 3 minus 1, which equals 2.
So, we must pick at least 2 existing ones.
We could potentially pick up to `2 + 3 = 5` existing ones. However, since we only need 3 ones in total, we will only test picking 2 existing ones or 3 existing ones.

**Testing L = 2 (Picking 2 existing ones and creating 1):**
We need to find the best window of size 2. Our positions are 0, 1, 5, 6, 9.
* Window `[0, 1]`: Distance between them is 1. Cost is 1 move.
* Window `[1, 5]`: Distance is 4 moves.
* Window `[5, 6]`: Distance is 1 move.
* Window `[6, 9]`: Distance is 3 moves.

The best cost to gather 2 existing ones is 1 move. We also need to create 1 one. Creating a one costs 2 moves.
Total cost for this plan: 1 (for existing) + 2 (for created) = 3 moves.

**Testing L = 3 (Picking 3 existing ones and creating 0):**
We need to find the best window of size 3.
* Window `[0, 1, 5]`: Median is 1. Distance from 0 to 1 is 1. Distance from 5 to 1 is 4. Total is 5 moves.
* Window `[1, 5, 6]`: Median is 5. Distance from 1 to 5 is 4. Distance from 6 to 5 is 1. Total is 5 moves.
* Window `[5, 6, 9]`: Median is 6. Distance from 5 to 6 is 1. Distance from 9 to 6 is 3. Total is 4 moves.

The best cost to gather 3 existing ones is 4 moves. We create 0 ones.
Total cost for this plan: 4 moves.

Comparing our options, the plan where we pick 2 existing ones and create 1 results in 3 moves, which is less than 4 moves. 
Therefore, the minimum moves required is 3! 

## Writing the Code Together

Now, let us translate all of this beautiful logic into C++ code. I have added friendly comments to explain what each section is doing. 

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long minimumMoves(vector<int>& nums, int k, int maxChanges) {
        // First, we collect the exact positions of all existing ones in the array.
        vector<int> P;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] == 1) {
                P.push_back(i);
            }
        }
        
        // We create a prefix sum array of these positions. 
        // This allows us to instantly calculate the sum of any segment.
        vector<long long> pref(P.size() + 1, 0);
        for (size_t i = 0; i < P.size(); ++i) {
            pref[i + 1] = pref[i] + P[i];
        }
        
        // Calculate the absolute minimum number of existing ones we are forced to use.
        int M = max(0, k - maxChanges);
        
        // Calculate the maximum number of existing ones we should ever consider.
        // It cannot exceed k, it cannot exceed the total ones available, 
        // and we never need to check more than M plus 3.
        int max_L = min({k, (int)P.size(), M + 3});
        
        long long ans = -1;
        
        // We test every sensible amount of existing ones to collect, from M up to max_L.
        for (int L = M; L <= max_L; ++L) {
            long long current_cost = 0;
            
            if (L > 0) {
                long long min_cost_L = -1;
                
                // We slide a window of size L across our list of positions.
                for (int i = 0; i <= (int)P.size() - L; ++i) {
                    // Find the median index for this specific window.
                    int m = i + L / 2;
                    
                    long long left_count = m - i;
                    long long right_count = (i + L - 1) - m;
                    
                    // Use our prefix sums to get the sum of positions on the left and right.
                    long long left_sum = pref[m] - pref[i];
                    long long right_sum = pref[i + L] - pref[m + 1];
                    
                    // Calculate the total moves to bring all ones in this window to the median.
                    long long cost = left_count * P[m] - left_sum + right_sum - right_count * P[m];
                    
                    // Keep track of the lowest cost we find for a window of size L.
                    if (min_cost_L == -1 || cost < min_cost_L) {
                        min_cost_L = cost;
                    }
                }
                
                // Add the cost of any additional ones we need to create.
                // Each created one costs exactly 2 moves.
                current_cost = min_cost_L + 2LL * (k - L);
            } else {
                // If L is zero, it means we are creating every single one we need.
                current_cost = 2LL * k;
            }
            
            // Keep track of the absolute best answer across all tested sizes of L.
            if (ans == -1 || current_cost < ans) {
                ans = current_cost;
            }
        }
        
        return ans;
    }
};

```

## Understanding Time and Space Complexity

It is always good practice to understand how our solution scales as the input gets larger.

**Time Complexity:** First, we scan through the original array to find all the ones. This takes O(N) time, where N is the length of the array. Building the prefix sum array also takes O(N) time.
Then, we enter our testing loop. How many times does this loop run? Because `L` is bounded between `M` and `M + 3`, the loop runs a maximum of four times, no matter how huge `k` or the array is!
Inside the loop, we slide a window across our positions array, which takes O(N) time. The math inside the window is instant due to our prefix sums.
Since we do an O(N) operation at most four times, the overall Time Complexity is solidly O(N). This is a highly effective approach!

**Space Complexity:**
We create an array `P` to store the positions of the ones. In the situation where every item in the array is a one, this takes O(N) space. We also create a prefix sum array of the same size, taking another O(N) space.
Therefore, the overall Space Complexity is O(N).

## Final Thoughts and Encouragement

I truly hope this deep dive was helpful and illuminated the clever logic behind this puzzle. By breaking down the true cost of actions and realizing that we only need to search a tiny, restricted space, a seemingly overwhelming problem becomes remarkably straightforward.

Remember, when faced with a complex puzzle, try to identify the fundamental costs of your options. Often, you will find a shortcut that eliminates huge portions of the search space. Keep practicing, keep exploring, and most importantly, keep having fun with these logic games.

Happy coding, friends!
