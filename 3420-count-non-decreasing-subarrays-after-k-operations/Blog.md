# Conquering Array Operations A Guide To Counting Non Decreasing Subarrays With K Increments [#3420]

Hello everyone! Welcome to another community sharing session. Today we are going to dive deeply into a fascinating array manipulation problem. I want to share a solution that combines several foundational computer science concepts. We will explore how to use a monotonic stack, a two pointer sliding window, and prefix sums to find our answer. My goal is to break this down into very detailed steps so that anyone, regardless of their current experience level, can grasp the underlying concepts and apply them.

Grab a warm beverage, get comfortable, and let us explore this logic step by step!

### Problem Introduction

Before we jump into the solution, let us clearly define the challenge we are facing. We are given an array of integers and a specific number of allowed operations called `k`. 

An operation consists of picking any element in a subarray and increasing its value by exactly 1. We have a maximum of `k` operations available for each subarray we evaluate.

Our objective is to count the total number of subarrays that can be transformed into a "non decreasing" sequence. A non decreasing sequence simply means that every number in the sequence is greater than or equal to the number that comes immediately before it. For example, the sequence 1, 3, 3, 5 is non decreasing. 

Each subarray is evaluated completely independently. This means if we use operations to fix one subarray, those changes do not carry over to the next subarray we check. We are simply asking the question "If we looked at only this specific piece of the array, could we fix it using at most `k` additions?"



### The Core Concept and Cost Calculation

To determine if a subarray can be made non decreasing, we first need to figure out the minimum number of additions required to fix it. 

Imagine we are walking through a subarray from left to right. To make the sequence non decreasing, the current number we are looking at must be at least as large as the largest number we have seen so far in this subarray. If the current number is smaller than that maximum, we have to increase it until it matches the maximum. 

Therefore, the "target" value for any element in the subarray is exactly the maximum value that appears before it, or its own value if it is already the largest.

Let us define a concept called "Cost". The cost to make a subarray starting at index `L` and ending at index `R` non decreasing is the sum of all the additions we need to make. 

Instead of simulating the additions one by one, we can calculate the cost mathematically. The total cost is equal to the sum of the "target" values for every element, subtracted by the sum of the original values of the elements. 

To say it in plain text
Total Cost equals (Sum of Prefix Maximums) minus (Sum of Original Elements)

If this calculated Total Cost is less than or equal to our allowed operations `k`, then the subarray is valid!

### Breaking Down The Tools We Need

To solve this problem systematically without running out of time, we need a few specialized tools. We cannot simply check every single subarray from scratch, because that would take far too long for large arrays. We need to be clever about how we store and reuse information.

#### Tool 1 Prefix Sums
To quickly find the "Sum of Original Elements" for any subarray from index `L` to `R`, we will use a prefix sum array. A prefix sum array keeps a running total of the elements. When we want to know the sum of elements between `L` and `R`, we simply take the running total up to `R` and subtract the running total up to `L`. This gives us the sum instantly, saving us from looping through the elements again and again.

#### Tool 2 The Monotonic Stack
Finding the "Sum of Prefix Maximums" is a bit more complex. As we evaluate different subarrays, the "maximum seen so far" changes. To keep track of these maximums efficiently, we will use a data structure called a monotonic stack. 



A monotonic stack is just a regular stack, but we enforce a strict rule about what is allowed inside it. In our case, we will maintain a strictly decreasing stack of array indices. We will iterate through the array backwards. By doing this, the stack will help us divide the array into "blocks" or "zones" where a specific number is the reigning maximum.

#### Tool 3 Binary Search
Because our stack maintains a sorted order of indices, we can use binary search to quickly find which "block" of maximums a specific index belongs to. This is crucial for rapidly calculating our Total Cost for any arbitrary window.

#### Tool 4 The Sliding Window
Finally, we will use a two pointer approach, also known as a sliding window. We will maintain a left boundary `L` and a right boundary `R`. We will move `L` backwards from the end of the array to the beginning. For each position of `L`, we will check if the subarray from `L` to `R` is valid. If the cost is too high (greater than `k`), we will shrink the window by moving `R` to the left until the cost becomes acceptable. 

Because the cost always increases as the window gets larger, we know that if the window from `L` to `R` is valid, all smaller windows starting at `L` are also valid. This allows us to count multiple valid subarrays at once!

### Step By Step Walkthrough Of The Logic

Let us put all these tools together into a cohesive strategy.

1.  **Preparation** First, we create our prefix sum array for the original numbers. This will handle the second half of our cost calculation formula.
2.  **Initialization** We set up our monotonic stack to hold indices. We also set up another prefix sum array specifically for the stack, which will keep track of the accumulated sums of the "maximum blocks". We initialize our right pointer `R` to the very last index of the array.
3.  **Iterating Backwards** We start a loop where our left pointer `L` moves from the last index down to 0. 
4.  **Maintaining The Stack** Inside the loop, we look at the number at the current left pointer `L`. If the stack has elements, and the number at `L` is greater than or equal to the number at the index stored at the top of the stack, it means the number at `L` is the new reigning maximum for that block. We pop elements off the stack until the stack is empty or the top of the stack points to a number strictly greater than the number at `L`.
5.  **Adding To The Stack** Once we have cleared out the smaller elements, we calculate the size of the new block that is dominated by the number at `L`. We add this sum to our stack prefix sum array, and then we push the index `L` onto the stack.
6.  **Calculating the Cost Function** We create a helper function to calculate the cost for the current window from `L` to `R`. This function uses binary search on the stack to find the exact sum of the prefix maximums for the range `L` to `R`. It subtracts the original array sum for the same range.
7.  **Adjusting the Window** We use a while loop to check if the cost of the window `L` to `R` is greater than `k`. If it is, we decrement `R` by 1. We keep shrinking the window until the cost is valid.
8.  **Counting Valid Subarrays** Once we have a valid window from `L` to `R`, we know that every subarray starting at `L` and ending anywhere up to `R` is valid. So, we add the size of the window to our running total of valid subarrays.



### The Code Implementation

Here is the C++ code that implements the logic we just discussed. I have added many comments to help you connect the code to the concepts above. Please note that in programming, the minus symbol is used for subtraction and decrementing variables.

```cpp
#include <vector>

using namespace std;

class Solution {
public:
    long long countNonDecreasingSubarrays(vector<int>& nums, int k) {
        int n = nums.size();
        
        // This array holds the running totals of the original numbers.
        // It allows us to get the sum of any range in constant time.
        vector<long long> prefSums(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            prefSums[i + 1] = prefSums[i] + nums[i];
        }

        // The monotonic stack stores indices to help us track the maximums.
        vector<int> stackIndices;               
        
        // This array tracks the accumulated sums of the maximum blocks.
        vector<long long> blockSums;     
        
        long long totalValidSubarrays = 0;
        int rightBoundary = n - 1;

        // A helper function to calculate the additions needed for the range L to R
        auto calculateCost = [&](int leftBoundary, int currentRight) {
            int searchLow = 0;
            int searchHigh = (int)stackIndices.size() - 1;
            int foundIndex = 0;
            
            // We use binary search to locate the block containing our currentRight
            while (searchLow <= searchHigh) {
                int midPoint = searchLow + (searchHigh - searchLow) / 2;
                if (stackIndices[midPoint] <= currentRight) {
                    foundIndex = midPoint;
                    searchHigh = midPoint - 1;
                } else {
                    searchLow = midPoint + 1;
                }
            }
            
            long long totalMaxSum = blockSums.back();
            long long endOfCurrentBlock = (foundIndex == 0) ? n : stackIndices[foundIndex - 1];
            
            // We must deduct the parts of the block that extend beyond our currentRight
            long long excessElements = (endOfCurrentBlock - 1 - currentRight);
            long long excessValue = excessElements * 1LL * nums[stackIndices[foundIndex]];
            
            long long blocksToTheRight = (foundIndex == 0) ? 0 : blockSums[foundIndex - 1];
            
            long long maxAreaForRange = totalMaxSum - excessValue - blocksToTheRight;
            long long originalAreaForRange = prefSums[currentRight + 1] - prefSums[leftBoundary];
            
            // The cost is the difference between the target maximums and the original numbers
            return maxAreaForRange - originalAreaForRange;
        };

        // We slide our left boundary backwards through the array
        for (int leftBoundary = n - 1; leftBoundary >= 0; --leftBoundary) {
            
            // We maintain the strictly decreasing property of our stack
            while (!stackIndices.empty() && nums[stackIndices.back()] <= nums[leftBoundary]) {
                stackIndices.pop_back();
                blockSums.pop_back();
            }
            
            // Calculate the reach of the new reigning maximum
            long long currentBlockLength = (stackIndices.empty() ? n : stackIndices.back()) - leftBoundary;
            long long currentBlockValue = currentBlockLength * 1LL * nums[leftBoundary];
            
            long long previousAccumulatedSum = blockSums.empty() ? 0 : blockSums.back();
            
            stackIndices.push_back(leftBoundary);
            blockSums.push_back(previousAccumulatedSum + currentBlockValue);

            // If the cost is too high, we shrink the window from the right
            while (calculateCost(leftBoundary, rightBoundary) > k) {
                rightBoundary--;
            }
            
            // All subarrays starting at leftBoundary and ending up to rightBoundary are valid
            totalValidSubarrays += (rightBoundary - leftBoundary + 1);
        }

        return totalValidSubarrays;
    }
};

```

### Understanding The Complexity

When evaluating algorithms, we look at Time Complexity (how long it takes to run) and Space Complexity (how much memory it uses). Let us break these down.

**Time Complexity**
The outer loop runs `N` times, where `N` is the length of the array. Inside this loop, we do a few things. Pushing and popping elements from the stack happens at most once per element overall, so the stack operations average out to constant time per element. However, we also call our cost calculation function. This function uses binary search on the stack, which takes logarithmic time based on the number of elements in the stack. In the worst case, this search takes `O(log N)` time. Therefore, the overall Time Complexity is bounded by `O(N log N)`. This is highly efficient and will comfortably pass time limits for arrays with hundreds of thousands of elements.

**Space Complexity**
We create a few extra arrays to manage our data. The prefix sums array takes `N + 1` space. The stack can potentially hold up to `N` elements in the worst case (if the array is already sorted in increasing order). The block sums array mirrors the stack size, taking up to `N` space. Therefore, the overall Space Complexity scales directly with the size of the input array, resulting in `O(N)` auxiliary space.

### Wrapping Up

I hope this thorough walkthrough helps demystify the logic behind counting non decreasing subarrays with a limited budget of increments. Combining sliding windows with monotonic stacks can seem overwhelming at first glance, but when you break the problem down into isolated concepts, it becomes a very elegant puzzle to solve.

The key takeaways are recognizing that the cost to fix a subarray depends purely on the running maximums, and utilizing efficient data structures to track those maximums dynamically as we slide our window across the data.

Thank you for reading this community guide! Keep practicing, keep breaking problems down into their core components, and happy coding!
