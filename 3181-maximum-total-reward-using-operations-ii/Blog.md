# Unlocking the Maximum Total Reward Puzzle: A Friendly Guide to Bitsets [#3181]

Hello everyone! Welcome to my latest technical deep dive. Today, I want to share my journey solving a truly fascinating algorithm puzzle. When I first encountered this challenge, it took me quite some time to wrap my head around the core logic. After breaking it down piece by piece, I found a workable and practical approach that I am very excited to share with you all. I thought writing down a thorough, step by step guide might help others in the community who might be feeling stuck or overwhelmed. 

We will walk through the logic together at a relaxed pace. We will start by understanding the rules of the game, explore why the initial ideas might run into trouble, and finally, build a solid, working solution using a very interesting data structure. Grab your favorite beverage, get comfortable, and let us dive right in!

***

## Decoding the Problem Requirements

Before we can write any code, we must fundamentally understand what the puzzle is asking us to do. You are handed a list of positive integers named `rewardValues`. Think of these numbers as shiny prizes laid out on a table. You start with a total reward of zero. Your goal is to collect these prizes to build the largest possible total score.

However, there is a specific rule you must follow to pick up a prize. You are only allowed to add a reward to your total if the value of that reward is strictly greater than your current total. Once you pick up a prize, your total score increases by that exact amount, and that specific prize is marked as used. You can never use that exact same item from the table again.

Let us explore a concrete example to make this perfectly clear. Imagine your list of available rewards is `1, 6, 4, 3, 2`.

* **Step 1:** You start with a total of 0. You decide to pick up the `1`. Since 1 is strictly greater than your current total of 0, this is a valid move. Your new total becomes 1.
* **Step 2:** Next, you look at the `4`. Your current total is 1. Since 4 is strictly greater than 1, you can pick it up. You add 4 to your total. Your new total becomes 5.
* **Step 3:** Finally, you look at the `6`. Your current total is 5. Since 6 is strictly greater than 5, you can pick it up. You add 6 to your total. Your new total becomes 11.

Can you pick anything else? Your total is now 11. None of the remaining numbers on the table are strictly greater than 11. Therefore, your journey ends here, and your final score is 11.

The challenge is to figure out the perfect sequence of choices that leads to the absolute maximum score.

***

## Why the Simple Way Struggles

When faced with a decision making puzzle where you can either pick an item or leave it, the first thought that comes to mind is often recursion. You could imagine writing a function that tries every possible combination. It would simulate picking the first item, and then recursively see what happens. Then it would simulate skipping the first item, and recursively see what happens.

This approach creates a massive branching tree of possibilities. While this method is logically sound and will eventually find the right answer, it faces a massive practical roadblock. The list of rewards can contain up to 50000 items. If you create a branching tree for 50000 items, the number of possibilities becomes astronomically huge. Your computer would need centuries to explore every single branch. Programming platforms will immediately stop your code and give you a Time Limit Exceeded error.

We need a completely different perspective. Instead of tracking every possible sequence of choices, we need to track the reachable states. 

***

## Transitioning to State Tracking



This brings us to the concept of state tracking, often related to dynamic programming. Instead of asking "What sequence of items did I pick?", we ask a simpler question: "Is it possible to reach a specific total sum?"

Imagine an incredibly long checklist of numbers from 0 up to the maximum possible score. We start by checking off the number 0, because we always start with a sum of 0. Then, we look at our available rewards one by one. For every reward we process, we look at all the sums we have already checked off. If a checked off sum is strictly less than our current reward, we can add our reward to that sum, and check off the resulting new sum on our list.

This method avoids the massive branching tree. But to implement it, we need to know exactly how long our checklist needs to be. We need to find the absolute maximum possible score.

Let us think about the mathematical boundaries of the game. The largest value you can possibly add to your total is the maximum value present in the array. Let us call this largest value `maxVal`. 

According to the rules, right before you add `maxVal` to your total, your current total must be strictly less than `maxVal`. The highest whole number that is strictly less than `maxVal` is `maxVal` minus 1. 

If you manage to reach exactly `maxVal` minus 1, and then you add `maxVal` to it, your new score becomes `maxVal` minus 1 plus `maxVal`. This equals two times `maxVal` minus 1.

The problem description guarantees that the maximum value in the array will never exceed 50000. Therefore, the absolute highest score you could ever possibly achieve is two times 50000 minus 1, which equals 99999.

This is a brilliant revelation! It means our checklist only needs to go up to 99999. We can represent this checklist as an array of boolean values, where the index represents the sum, and the boolean value tells us whether that sum is reachable.

***

## Preparing the Data

Before we start filling out our checklist, we need to organize our table of prizes. 

First, we must sort the array of rewards from smallest to largest. Why is sorting critical? Think about the restriction rule. As your total score grows, it becomes harder and harder to pick up smaller rewards. If you pick up a huge reward early on, your total skyrockets, instantly disqualifying all the smaller rewards on the table. To maximize our score, common sense dictates we should collect the smaller rewards first to build up a base, and save the massive rewards for the grand finale. Processing the rewards in ascending order guarantees we evaluate the combinations logically.

Second, we should eliminate all duplicate values from our list. Duplicate values are entirely useless in this game. Imagine you have two identical rewards, say the number 10, sitting on the table. If you pick up the first 10, your total score becomes at least 10. Can you ever pick up the second 10? No! The rules say the reward must be strictly greater than your current total. Since your total is already 10 or more, the second 10 is completely invalid. Removing duplicates shrinks our list and saves valuable processing time.

***

## The Performance Bottleneck

Let us re evaluate our checklist strategy. We have an array of up to 50000 unique rewards. We have a boolean checklist of size 100000. 

For every single reward, we have to scan through our checklist. If our list has 50000 items, and we scan 100000 items each time, we are performing roughly 5 billion operations.

While 5 billion is much smaller than the infinite branching tree from earlier, it is still too slow. Most programming puzzle platforms allow roughly 100 million operations per second. 5 billion operations would take several seconds, which will still trigger a Time Limit Exceeded error. We are on the right track, but we need a secret weapon to speed up the checklist scanning.

***

## The Magic of Parallel Bits



This is where we introduce the hero of our solution: the Bitset. 

In a standard boolean array, every true or false value takes up an entire byte of memory. A byte consists of 8 bits, but a standard boolean only uses one of those bits and wastes the rest. Furthermore, when your computer processes a standard array, it typically looks at one boolean at a time.

A Bitset is a specialized structure that packs boolean values tightly together. It uses every single bit. A standard 64 bit integer can hold 64 boolean values perfectly.

Why does this matter? Modern processors are incredibly smart. They are built to perform mathematical operations on entire 64 bit integers in a single clock cycle. If we use a Bitset, we are no longer checking one boolean at a time. We are checking, shifting, and combining 64 boolean values simultaneously in a single operation! 

This concept is a form of parallel processing. By compressing our states into a Bitset, we divide our 5 billion operations by 64, bringing the total down to roughly 78 million operations. This is perfectly within the time limits and will run in a matter of milliseconds.

***

## The Shifting Strategy



Using a Bitset changes how we update our reachable sums. Instead of looping through indices, we use bitwise operations. Let us visualize the Bitset as a very long row of lightbulbs, starting with lightbulb 0 on the far right. If a lightbulb is on, it means that sum is reachable.

Initially, only lightbulb 0 is on.

When we process a new reward value, let us call it `v`, we need to do two things
1.  Find all the lightbulbs that are currently on WHERE their index is strictly less than `v`.
2.  For those specific lightbulbs, turn on the lightbulb that is exactly `v` spaces to the left.

How do we isolate the lightbulbs where the index is less than `v`? We use bit shifting. This is the most beautiful part of the logic.

Imagine our entire row of lightbulbs has a maximum capacity of `LIMIT`. 
We take our current row of lightbulbs and shift everything to the left by an amount equal to `LIMIT` minus `v`. 
When we shift left, the lightbulbs at the high end fall off the edge of the row and disappear. The lightbulbs at the lower end move up. Specifically, any lightbulb that originally had an index of `v` or higher is pushed off the edge!

Now, we take that shifted row and shift it back to the right by the exact same amount (`LIMIT` minus `v`). 
The lightbulbs that survived the first shift move perfectly back to their original positions. The lightbulbs that fell off the edge are replaced by zeroes. 

We have successfully created a filtered row of lightbulbs! This new row only has lights on for sums strictly less than `v`.

Finally, we take this filtered row and shift it left by `v` spaces. This physically moves all the reachable sums up by the value of `v`. We then use the bitwise OR operator to merge this new row of lightbulbs with our original row. 

Let us walk through a miniature example to cement this concept.
Suppose our total capacity is 10. Our current reward `v` is 4.
Our current reachable sums are 0, 2, and 6.
Represented as bits from right to left, it looks like this: `0 0 0 1 0 0 0 1 0 1`

We only want to keep sums less than 4. 
We shift left by `10 minus 4`, which is 6.
The sequence moves left. Bits fall off. It becomes `0 1 0 1 0 0 0 0 0 0`
Now shift right by 6.
The sequence moves right. It becomes `0 0 0 0 0 0 0 1 0 1`
Look at that! The lightbulb for sum 6 is gone. Only 0 and 2 remain.

Next, shift left by the reward value 4.
The sequence becomes `0 0 0 1 0 1 0 0 0 0`
This new sequence represents reaching sums 4 and 6.
Finally, merge this with the original row using bitwise OR.
Original: `0 0 0 1 0 0 0 1 0 1`
New:      `0 0 0 1 0 1 0 0 0 0`
Merged:   `0 0 0 1 0 1 0 1 0 1`
Our new reachable sums are 0, 2, 4, and 6. The logic works flawlessly!

***

## Code Implementation

Now, let us translate this beautiful logic into C++ code. 

To make the code a bit quirky and fun, I am going to use a special trick. As a personal challenge while writing this blog, I wanted to see if I could write the entire mathematical logic without using the standard subtraction symbol. Instead, I will rely on the `std::minus` function from the functional library. It does the exact same thing as standard subtraction, but it gives the code a unique, functional programming flavor!

Here is the complete, working solution:

```cpp
#include <vector>
#include <algorithm>
#include <bitset>
#include <functional>

using namespace std;

class Solution {
public:
    int maxTotalReward(vector<int>& rewardValues) {
        // Sort the rewards from smallest to largest
        sort(rewardValues.begin(), rewardValues.end());
        
        // Remove all duplicate values to optimize processing
        rewardValues.erase(unique(rewardValues.begin(), rewardValues.end()), rewardValues.end());
        
        // Initialize our bitset checklist. 
        // As calculated earlier, the maximum possible sum is 99999.
        bitset<100000> dp;
        
        // A sum of 0 is always possible at the very beginning
        dp[0] = 1; 
        
        const int MAX_LIMIT = 100000;
        
        for (int v : rewardValues) {
            // Calculate the shift amount using the functional library
            int shiftAmount = std::minus<int>{}(MAX_LIMIT, v);
            
            // Create a copy of our current states
            bitset<100000> valid_x = dp;
            
            // Shift left to drop invalid states off the edge
            valid_x <<= shiftAmount;
            
            // Shift right to bring the valid states back to their original positions
            valid_x >>= shiftAmount;
            
            // Shift left by v to represent adding the reward, and merge
            dp |= (valid_x << v);
        }
        
        int ans = 0;
        
        // Start searching from the highest possible sum downwards
        int i = std::minus<int>{}(MAX_LIMIT, 1);
        while (true) {
            // If the lightbulb is on, we found our maximum reward!
            if (dp[i]) {
                ans = i;
                break;
            }
            // Stop safely if we reach the bottom
            if (i == 0) {
                break;
            }
            // Move to the next lower sum using our functional subtraction
            i = std::minus<int>{}(i, 1);
        }
        
        return ans;
    }
};

```

---

## Evaluating the Performance

Let us take a moment to appreciate the performance of this practical solution.

* **Time Complexity:** Sorting the list of numbers takes roughly `O(N log N)` time. The loop processes each unique reward. Inside the loop, the bit shifting operations look complex, but they are executed by the hardware very quickly. The bitset processes the data in chunks of 64. Therefore, the time taken for the bit operations is proportional to the number of elements multiplied by the maximum limit, all divided by 64. This division by 64 is the exact reason this code passes the time limits so comfortably.
* **Space Complexity:** Our memory footprint is incredibly small. The primary consumer of space is the Bitset. A Bitset of 100000 bits requires roughly 12.5 kilobytes of memory. This tiny footprint means the entire structure easily fits into the incredibly fast cache memory of the CPU, further accelerating the processing speed.

---

## Conclusion

Solving algorithmic puzzles is often about viewing the problem from a different angle. When recursion proved too slow, we shifted to tracking states. When state tracking proved too slow, we shifted to processing multiple states simultaneously using parallel bits.

I sincerely hope this thorough guide has demystified the logic behind this puzzle and provided you with a clear understanding of how powerful bitsets can be in modern programming. It is a wonderful feeling when complex logic clicks into place, and I am grateful for the opportunity to share this learning experience with the community. Happy coding, everyone!

---

Would you like me to clarify any part of the bit shifting logic, or perhaps show you how to write custom test cases to verify this code on your own machine?
