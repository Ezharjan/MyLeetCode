# Mastering the Peaks and Valleys of the Trionic Array Challenge [#3640]

[Blog Post Link](https://leetcode.com/discuss/post/7553103/mastering-the-peaks-and-valleys-of-the-t-6yo8/)


Hello there coding enthusiasts! Welcome back to my another deep dive into the fascinating world of algorithmic problem solving. Today we are tackling a particularly beautiful challenge that tests our ability to manage state and visualize data flow. It is a problem that looks intimidating at first glance but actually unravels elegantly once we apply the right perspective.

If you have ever felt overwhelmed by array problems that ask for complex subarray structures, this guide is for you. We are going to explore a problem involving a "Trionic" array. This is a fancy term for a specific shape: a sequence that goes up, then down, then up again.

Grab your favorite beverage and let us break this down together.

## Understanding the Problem

Let us start by clearly defining what we are looking for. We are given an integer array called `nums`. We need to find the **maximum sum** of a specific type of contiguous subarray.

This subarray is not just any slice of the array. It must satisfy a specific structural pattern defined by four indices .

### The Shape of the Data

The structure is defined as follows:

1. **First Leg ( to ):** The numbers must be **strictly increasing**. This is our first ascent up the mountain.
2. **Second Leg ( to ):** The numbers must be **strictly decreasing**. This is our descent into the valley.
3. **Third Leg ( to ):** The numbers must be **strictly increasing** again. This is our final climb.

So, visually, we are looking for an **N** shape (or a lightning bolt shape). We want to find the specific "N" shape inside our array that yields the largest total sum of elements.

### The Constraints

* The indices must strictly follow the order .
* Strictly increasing means every number is larger than the one before it. Equal numbers break the chain.
* Strictly decreasing means every number is smaller than the one before it.
* The array size can be up to  elements.
* The values can be negative.

The length constraint implies that a valid trionic subarray must contain at least four elements, although usually it requires more to form the slopes (at least two numbers for the first up, sharing the peak for the down, sharing the valley for the second up).

## Visualizing the Logic

Imagine you are hiking across a terrain represented by the numbers in the array. You want to plan a hike that consists of exactly three phases:

1. **Climb**
2. **Descend**
3. **Climb**

You cannot skip a phase, and you cannot change the order. Your goal is to maximize the "points" (values) you collect along this specific path.

Because the array length is large, checking every possible start and end point would take forever. We need a smarter way. We need to process the terrain in a single pass. This brings us to **Dynamic Programming**, or more specifically, a **State Machine** approach.

## The State Machine Approach

When we iterate through the array one number at a time, we need to decide what "state" we are in at that specific moment. Are we currently building the first upward leg? Are we in the middle of the downward slope? Or are we finishing the final upward climb?

Let us define three variables to track the maximum sum ending at the current position for each of these three phases.

### 1. The First Ascent (`up1`)

This variable tracks the maximum sum of a strictly increasing subarray ending exactly at our current position.

* If the current number is greater than the previous number, we can extend the climb.
* Alternatively, we could start a brand new climb using just the previous number and the current number.

### 2. The Descent (`down`)

This variable tracks the maximum sum of a valid prefix (Up then Down) ending exactly at the current position.

* To be in this state, we must be going down. So the current number must be smaller than the previous one.
* We can arrive here by extending an existing descent.
* Or, we can arrive here by transitioning from the `up1` state (crossing the peak).

### 3. The Second Ascent (`up2`)

This variable tracks the maximum sum of the complete Trionic structure (Up then Down then Up) ending exactly at the current position.

* To be in this state, we must be going up.
* We can arrive here by extending an existing second climb.
* Or, we can arrive here by transitioning from the `down` state (crossing the valley).

## Designing the Transitions

This is the most critical part of our algorithm. We will iterate from index 1 to the end. Let `cur` be the current number and `prev` be the number immediately before it.

The relationship between `cur` and `prev` dictates everything.

### Scenario A: We are Climbing (`cur > prev`)

In this scenario, the terrain is going up. This allows us to update our two climbing states (`up1` and `up2`).

* **Updating `up1`:** We can extend the previous `up1` chain. But wait! What if the previous `up1` chain was terrible (very negative)? We might prefer to start fresh. A fresh start for the first leg isn't just `cur`; it requires two numbers to define a slope. So a fresh start is `prev + cur`.
* *Logic:* `new_up1 = max(up1, prev) + cur`


* **Updating `up2`:** We can extend an existing `up2` chain. Or, we can just now be entering the third leg from a valley. This means we could transition from the `down` state.
* *Logic:* `new_up2 = max(up2, down) + cur`


* **What about `down`?** We cannot extend a downward slope because we just went up. So `down` becomes invalid for this specific index. However, in code, we usually set this to a very small number to signify "impossible".

### Scenario B: We are Descending (`cur < prev`)

In this scenario, the terrain is going down. This allows us to update our descent state (`down`).

* **Updating `down`:** We can extend an existing `down` chain. Or, we can just now be crossing the peak from the first ascent (`up1`).
* *Logic:* `new_down = max(down, up1) + cur`


* **What about `up1` and `up2`?** We cannot extend upward slopes because we just went down. These states become invalid for this index.

### Scenario C: Flat Terrain (`cur == prev`)

The problem demands **strictly** increasing or decreasing sequences. If two adjacent numbers are equal, the chain snaps. All current formations become invalid. We must reset everything to our "impossible" value.

## Implementation Details

We need to be careful with initialization. Since we want to find the maximum sum, and that sum could be negative, we should initialize our states to a very small number (like negative infinity). However, be careful not to use the absolute smallest integer possible because adding to it might cause an overflow error. A safe large negative number works best.

We also need a global variable, let us call it `ans`, to keep track of the highest value `up2` ever reaches throughout the entire loop.

## The C++ Solution

Here is the clean, efficient code implementing the logic we discussed.

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    long long maxSumTrionic(vector<int>& nums) {
        int n = nums.size();
        
        // We use a large negative constant to represent an invalid state.
        // We do not use LLONG_MIN to ensure we can add to it without overflow.
        const long long INF = -1e15; 
        
        // These variables hold the max sum ending at index i-1
        // for the three respective phases.
        long long currentUp1 = INF;
        long long currentDown = INF;
        long long currentUp2 = INF;
        
        long long maxTrionicSum = INF;
        
        // Iterate through the array starting from the second element
        for (int i = 1; i < n; ++i) {
            long long cur = nums[i];
            long long prev = nums[i-1];
            
            // We calculate the potential new states for index i
            long long nextUp1 = INF;
            long long nextDown = INF;
            long long nextUp2 = INF;
            
            if (cur > prev) {
                // The slope is increasing.
                
                // 1. Handle the First Ascent (up1)
                // We can extend an existing ascent: currentUp1 + cur
                // OR start a new ascent: prev + cur
                // We take the better option.
                nextUp1 = max(currentUp1, prev) + cur;
                
                // 2. Handle the Second Ascent (up2)
                // We can extend an existing second ascent: currentUp2 + cur
                // OR transition from a valley (down state): currentDown + cur
                // Note: We only transition if currentDown is valid (> INF)
                long long bestPrevious = max(currentUp2, currentDown);
                if (bestPrevious > INF) {
                    nextUp2 = bestPrevious + cur;
                }
                
            } else if (cur < prev) {
                // The slope is decreasing.
                
                // Handle the Descent (down)
                // We can extend an existing descent: currentDown + cur
                // OR transition from a peak (up1 state): currentUp1 + cur
                long long bestPrevious = max(currentDown, currentUp1);
                if (bestPrevious > INF) {
                    nextDown = bestPrevious + cur;
                }
            }
            
            // If cur == prev, strict monotonicity is broken.
            // All next variables remain at INF, effectively resetting our chains.
            
            // Update our state variables for the next iteration
            currentUp1 = nextUp1;
            currentDown = nextDown;
            currentUp2 = nextUp2;
            
            // If we have formed a valid full trionic array, update the global answer
            if (currentUp2 > INF) {
                maxTrionicSum = max(maxTrionicSum, currentUp2);
            }
        }
        
        return maxTrionicSum;
    }
};

```

## Step by Step Tracing

Let us trace Example 1 to see this magic in action.
**Input:** `[0, -2, -1, -3, 0, 2, -1]`

**Initialization:** `up1`, `down`, `up2`, `ans` are all `INF`.

1. **Index 1 (Val: -2, Prev: 0):**
* `-2 < 0` (Decreasing).
* Update `down`: Max of (`down`, `up1`) + `-2`. Both are `INF`, so `down` stays `INF`.
* *Result:* No valid chains yet.


2. **Index 2 (Val: -1, Prev: -2):**
* `-1 > -2` (Increasing).
* Update `up1`: Max(`up1`, `prev`) + `-1`. Max(`INF`, `-2`) is `-2`. So `up1` = `-2 + (-1)` = `-3`.
* Update `up2`: `down` is `INF`, so no transition.
* *State:* `up1 = -3`. We have a valid first leg `[-2, -1]`.


3. **Index 3 (Val: -3, Prev: -1):**
* `-3 < -1` (Decreasing).
* Update `down`: Max(`down`, `up1`) + `-3`. Max(`INF`, `-3`) is `-3`. So `down` = `-3 + (-3)` = `-6`.
* *State:* `down = -6`. We have `[-2, -1, -3]`.


4. **Index 4 (Val: 0, Prev: -3):**
* `0 > -3` (Increasing).
* Update `up1`: Max(`up1`, `prev`) + `0`. Max(`INF`, `-3`) is `-3`. So `up1` = `-3`. (New leg `[-3, 0]`).
* Update `up2`: Max(`up2`, `down`) + `0`. Max(`INF`, `-6`) is `-6`. So `up2` = `-6 + 0` = `-6`.
* *State:* `up2 = -6`. We found a full Trionic array! `[-2, -1, -3, 0]`. Sum is -6. `ans` updates to -6.


5. **Index 5 (Val: 2, Prev: 0):**
* `2 > 0` (Increasing).
* Update `up1`: Max(`up1`, `prev`) + `2`. `up1` becomes `-3 + 2` = `-1`.
* Update `up2`: Max(`up2`, `down`) + `2`. `down` was invalid (`INF`) from previous step because Index 4 was increasing. But wait, in the code we update states *after* calculation. Let's look at the variable `currentDown` passed from index 4.
* At Index 4, `currentDown` became `INF` because Index 4 was increasing. So at Index 5, we cannot transition from `down`. We can only extend `up2`.
* `up2` extends: `-6 + 2` = `-4`.
* *State:* `ans` updates to -4. This corresponds to `[-2, -1, -3, 0, 2]`.


6. **Index 6 (Val: -1, Prev: 2):**
* `-1 < 2` (Decreasing).
* Update `down`.
* `up1`, `up2` become `INF`.



**Final Answer:** -4.

## Complexity Analysis

This solution is highly efficient.

* **Time Complexity:** Linear Time. We loop through the array exactly once. Inside the loop, we perform a constant number of comparisons and additions. Thus, the complexity is proportional to .
* **Space Complexity:** Constant Space. We only use a few long long variables (`currentUp1`, `currentDown`, etc.) to store our state. We do not need any extra arrays or hash maps.

## Summary

The key to solving the Trionic Array problem lies in identifying the three distinct phases of the pattern and using a state machine to track the best possible sum for each phase as we iterate.

1. **Identify the states:** First Ascent, Descent, Second Ascent.
2. **Define transitions:** Increasing values allow extending Ascents or starting the First Ascent. Decreasing values allow extending the Descent or transitioning from First Ascent to Descent.
3. **Handle invalidity:** Use a sentry value (like a large negative number) to handle impossible states.

I hope this walkthrough helps you see the pattern behind the chaos. It is really satisfying when you can reduce a complex structural requirement into a few simple `if-else` transitions.

Happy leet-coding and may your arrays always be sorted (or trionic, if that is what you need)!
