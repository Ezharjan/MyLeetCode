# Cracking the Lightning Bolt Pattern and Mastering the Trionic Array Challenge [#3637]

[Blog Post Link](https://leetcode.com/discuss/post/7550353/cracking-the-lightning-bolt-pattern-and-vq7e8/)




Hello there, fellow coding enthusiasts!

Welcome back to another deep dive into the fascinating world of algorithmic challenges. Today I am absolutely thrilled to share a specific problem that beautifully illustrates the power of linear scanning and state management in array manipulation. It is a problem that tests our ability to translate visual patterns into rigorous logic.

We often encounter problems asking if an array is sorted or if it contains a mountain peak. But what happens when the pattern gets more complex? What if the data needs to go up, then down, then back up again? This is the essence of the **Trionic Array**.

In this extensive guide I will walk you through every single logic gate, edge case, and line of code required to solve this. Whether you are a beginner just starting with C++ or a seasoned veteran looking to brush up on pattern recognition, this post is for you.

## Understanding the Landscape

Let us first define exactly what we are looking at. The problem asks us to determine if an integer array, which we will call **nums**, follows a very specific structural shape. This shape is not random. It mimics the letter **N** or a lightning bolt.

### The Definition

An array is considered **Trionic** if we can find two specific indices, let us call them **p** and **q**, that split the array into three distinct segments.

The rules for these segments are strict and precise.

1. **The First Ascent:** From the start of the array up to index **p**, the numbers must get larger and larger. This is a **strictly increasing** sequence.
2. **The Descent:** From index **p** down to index **q**, the numbers must get smaller and smaller. This is a **strictly decreasing** sequence.
3. **The Second Ascent:** From index **q** to the very end of the array, the numbers must get larger and larger again. This is another **strictly increasing** sequence.

The constraints tell us that **p** must come before **q**, and both must exist strictly within the bounds of the array (not at the very start or very end). Specifically, the indices must satisfy **0 < p < q < n minus 1**.

### Visualizing the Path

Imagine you are hiking.
You start at the trailhead at index 0.
You begin walking uphill. The path gets steeper and higher with every step.
Suddenly you reach a peak at index **p**.
From this peak you descend into a valley. The path goes down, down, down until you hit the lowest point at index **q**.
From this valley you look up and see the final destination. You start climbing again, going strictly uphill until you reach the end of the trail.

If you can complete this hike exactly as described, the array is Trionic. If you get stuck, if the path flattens out, or if you run out of trail before completing all three stages, the array is not Trionic.

## Analyzing the Requirements and Constraints

Before we write a single line of code, we must understand the constraints that will break our algorithm if we ignore them.

**1. The Length of the Array**
The problem requires three segments.

* The first segment needs at least two numbers to show an increase.
* The second segment needs at least two numbers to show a decrease (sharing the peak with the first segment).
* The third segment needs at least two numbers to show an increase (sharing the valley with the second segment).

Mathematically, to have indices **0 < p < q < n minus 1**, the array must have a minimum length.

* Index 0 is the start.
* Index **p** is at least 1.
* Index **q** is at least 2.
* The last index **n minus 1** must be greater than **q**, so at least 3.

Wait, let us look closer.
If indices are 0, 1, 2, 3.
We could have **p** at 1 and **q** at 2.
Then we have segments: 0 to 1 (Up), 1 to 2 (Down), 2 to 3 (Up).
So the minimum length **n** must be 4.
If the array has fewer than 4 elements, it is mathematically impossible to contain these three distinct directional shifts. This is our first base case.

**2. The "Strictly" Requirement**
The problem uses the phrase **strictly increasing** and **strictly decreasing**. This is a crucial detail.
Strictly means that **nums[i]** cannot equal **nums[i + 1]**.
If we see two consecutive numbers that are equal, like `[3, 3]`, the condition is violated immediately. We are looking for inequalities like 3 < 4 or 5 > 2. We are never looking for 3 <= 3. Plateaus are not allowed on this mountain!

## Developing the Algorithm

Now we will construct our solution. We could try to check every possible pair of **p** and **q** using nested loops, but that would be incredibly slow. That approach would take O(N squared) time or worse. We want something faster. We want an O(N) approach, meaning we look at each number only once.

We will use a **Linear Scan** strategy. We will simulate the hike step by step using a single pointer or index, let us call it **i**.

### Phase 1: The First Ascent

We start our pointer **i** at 0.
We move forward as long as the next number is greater than the current number.
`nums[i] < nums[i + 1]`

We keep incrementing **i** until this condition is no longer true. When the loop stops, **i** sits at the peak. This potential peak is our **p**.

**Checkpoint 1:**
Did we actually move? If **i** is still 0, it means the array started with a decrease or a flat line. That is invalid.
Did we go too far? If **i** reached the last index, it means the whole array is increasing. We never went down or back up. That is also invalid.
If either of these happens, we return `false`.

### Phase 2: The Descent

Now we are standing at our peak **i**. The next step must be downwards.
We continue moving forward as long as the next number is smaller than the current number.
`nums[i] > nums[i + 1]`

We keep incrementing **i** until this condition stops. When this loop stops, **i** sits at the bottom of the valley. This potential valley is our **q**.

**Checkpoint 2:**
Did we move from the peak? If **i** is still at the same position as the peak, it means we did not descend. We hit a plateau or an increase immediately. That is invalid.
Did we go too far? If **i** reached the last index, it means the rest of the array was all decreasing. We never climbed back up for the final segment. That is invalid.
If either of these happens, we return `false`.

### Phase 3: The Second Ascent

Now we are at the valley **i**. The final leg of the journey must be upwards.
We continue moving forward as long as the next number is greater than the current number.
`nums[i] < nums[i + 1]`

We increment **i** until we either stop increasing or run out of numbers.

**Final Validation:**
After attempting all three phases, we simply check one thing. Did our pointer **i** reach the very last index of the array?
If `i == n minus 1`, it means we successfully traversed the entire path following the Up Down Up pattern perfectly. We return `true`.
If **i** stopped short, it means we hit a violation (like a dip during the final ascent) or a plateau. We return `false`.

## The Solution Implementation

Let us translate this logic into clean and efficient C++ code. I will use `vector<int>` as given in the problem prompt.

```cpp
class Solution {
public:
    bool isTrionic(vector<int>& nums) {
        int n = nums.size();
        
        // Step 1: Check for minimum length
        // We need at least 4 elements to form Up-Down-Up
        // Indices involved: 0...p...q...n-1
        if (n < 4) {
            return false;
        }

        int i = 0;

        // Step 2: Traverse the first strictly increasing segment
        // We ensure we do not go out of bounds (i < n - 1)
        while (i < n - 1 && nums[i] < nums[i + 1]) {
            i++;
        }

        // Validation for Step 2
        // If i is 0, we never went up (array starts decreasing or flat)
        // If i is n-1, the whole array is increasing (no down part)
        if (i == 0 || i == n - 1) {
            return false;
        }

        // Step 3: Traverse the strictly decreasing segment
        // Store the current peak position to compare later
        int p = i;
        while (i < n - 1 && nums[i] > nums[i + 1]) {
            i++;
        }

        // Validation for Step 3
        // If i == p, we did not move down at all
        // If i == n-1, we went all the way down without a final up
        if (i == p || i == n - 1) {
            return false;
        }

        // Step 4: Traverse the final strictly increasing segment
        while (i < n - 1 && nums[i] < nums[i + 1]) {
            i++;
        }

        // Final Validation
        // If we made it to the last element, the pattern is valid
        return i == n - 1;
    }
};

```

## A Walkthrough with Examples

To truly understand the mechanics, let us simulate the code with some concrete examples.

### Case A: The Perfect Trionic Array

**Input:** `[1, 5, 4, 2, 6, 9]`
**Length n:** 6. This is greater than 4, so we proceed.

1. **First Ascent:**
* Compare 1 and 5. Increasing. Move **i** to 1 (value 5).
* Compare 5 and 4. Decreasing. Stop.
* Current **i** is 1. Peak value is 5.
* Check: Is **i** 0? No. Is **i** 5? No. Continue.


2. **The Descent:**
* Compare 5 and 4. Decreasing. Move **i** to 2 (value 4).
* Compare 4 and 2. Decreasing. Move **i** to 3 (value 2).
* Compare 2 and 6. Increasing. Stop.
* Current **i** is 3. Valley value is 2.
* Check: Is **i** equal to previous peak (index 1)? No. Is **i** 5? No. Continue.


3. **Second Ascent:**
* Compare 2 and 6. Increasing. Move **i** to 4 (value 6).
* Compare 6 and 9. Increasing. Move **i** to 5 (value 9).
* We reached the end of the array. Loop stops.


4. **Final Check:**
* Does `i == n minus 1`? Yes (5 == 5).
* **Result: True.**



### Case B: The False Start

**Input:** `[5, 4, 6, 7]`
**Length n:** 4. Safe.

1. **First Ascent:**
* Compare 5 and 4. Decreasing immediately.
* The loop condition `nums[i] < nums[i+1]` fails instantly.
* **i** remains 0.


2. **Check:**
* Is **i** 0? Yes.
* **Result: False.**
* *Explanation: A Trionic array must start by going up.*



### Case C: The Missing Third Leg

**Input:** `[1, 5, 3, 2]`
**Length n:** 4. Safe.

1. **First Ascent:**
* 1 to 5 (Up). **i** becomes 1. Stop at 5.
* Check passes.


2. **The Descent:**
* 5 to 3 (Down). **i** becomes 2.
* 3 to 2 (Down). **i** becomes 3.
* We are at the end of the array.


3. **Check:**
* Is **i** equal to peak? No.
* Is **i** equal to `n minus 1` (3)? Yes.
* **Result: False.**
* *Explanation: We ran out of numbers before we could establish the final upward trend.*



## Complexity Analysis

When we write code for production or technical interviews, efficiency is paramount. Let us verify why this solution is optimal.

### Time Complexity: O(N)

We define N as the number of elements in the input array.
Look closely at the pointer **i**. We initialize it at 0. Throughout the three `while` loops, we only ever increment **i** (`i++`). We never reset it to 0. We never move it backwards.
This means we visit each element in the array exactly once. Whether the array has 10 elements or 10 million elements, the number of operations grows strictly linearly with the input size. This is the best possible time complexity because we must inspect every number at least once to verify the pattern.

### Space Complexity: O(1)

We define Space Complexity by how much extra memory we use relative to the input.
In this solution we create the integer variables `n`, `i`, and `p`.
Regardless of how large the input array is, we only ever need these few integers to keep track of our position. We are not creating new arrays, hash maps, or recursion stacks. Thus, our memory usage is constant.

## Why This Matters

You might wonder why we spend so much time on a specific array shape. The beauty of the Trionic Array problem lies in **State Machine Logic**.

We can think of this problem as a machine with three valid states:

1. **State 1: Climbing.** We stay here as long as inputs increase.
2. **State 2: Falling.** We transition here after a peak. We stay here as long as inputs decrease.
3. **State 3: Climbing Again.** We transition here after a valley. We stay here as long as inputs increase.

If the input ends while we are in State 3, we win. If the input violates the current state rules (like seeing a decrease while in State 3) or if the input ends while we are in State 1 or 2, we lose.

Learning to break down a problem into these sequential states is a superpower in programming. It allows you to tackle much harder problems like parsing text, validating complex passwords, or analyzing financial trends.

## Summary

To recap our journey today:

1. **We defined the Trionic Array** as a Strictly Increasing, then Strictly Decreasing, then Strictly Increasing sequence.
2. **We visualized the "N" shape** to guide our logic.
3. **We identified critical constraints**, specifically the minimum length of 4 and the strict inequality requirements.
4. **We implemented a Linear Scan algorithm** that traverses the array in a single pass using one pointer.
5. **We validated each stage**, ensuring we do not get stuck at the start, skip a stage, or end too early.

I hope this guide helps you feel more confident in tackling array traversal problems. The next time you see a problem asking about "mountains", "valleys", or specific patterns, remember this approach. Break it down into segments, use a single pointer, and validate your transitions carefully.

Happy leet-coding, everyone! 

Feel free to leave a comment if you found a different way to solve this or if you have questions about the edge cases. 
I'm always happy to chat about code!
