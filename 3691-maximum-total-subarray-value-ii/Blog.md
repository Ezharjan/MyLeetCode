# Mastering Array Selections A Step by Step Guide to Finding the Maximum Total Value [#3691]

Hello everyone! Welcome to this detailed guide. Today I want to share my thoughts and a practical approach for a very interesting array problem. Finding the maximum total value of chosen subarrays can seem quite daunting at first glance. I remember staring at this problem and feeling a bit overwhelmed by the sheer number of possible subarrays. However, by breaking it down piece by piece, we can build a workable and elegant solution together. Grab your favorite beverage, make yourself comfortable, and let us dive deep into the mechanics of array manipulation!

## Understanding the Problem

Let us clearly lay out what we are trying to achieve. You are handed an array of integers and a specific target number, k. Your mission is to select exactly k distinct nonempty subarrays. 

What is a subarray? It is simply a contiguous slice of the original array. Distinct means that if you choose a slice from index 0 to index 2, you cannot choose that exact same slice from index 0 to index 2 again. You can, however, choose an overlapping slice like index 1 to index 3.

The value of each subarray is calculated by a simple rule: find the largest number in that slice, find the smallest number in that slice, and subtract the smallest from the largest. Finally, you want to add up the values of all your k chosen subarrays to get the largest possible total sum.

### A Quick Example Walkthrough

Imagine your array is [1, 3, 2] and you need to pick exactly 2 subarrays. 
If we look at the slice [1, 3], the largest number is 3 and the smallest is 1. The value is 3 minus 1, which equals 2.
If we look at the slice [1, 3, 2], the largest is 3 and the smallest is 1. The value here is also 3 minus 1, which equals 2.
Adding these two values together gives us 4. This happens to be the best we can do for this small example.

## Why Brute Force Falls Short

When we first approach this, the most intuitive thought might be to just generate every single possible subarray, calculate the value for each one, sort them from highest to lowest, and pick the top k values. 

While this sounds perfectly logical, we run into a massive roadblock when we look at the constraints. The array can have up to 50000 elements. The number of possible subarrays for an array of size N is N multiplied by N plus 1, all divided by 2. For 50000 elements, this equates to over 1.25 billion subarrays! Generating, storing, and sorting over a billion values would require an enormous amount of time and memory. Our computers would simply freeze up or run out of memory. We definitely need a more thoughtful strategy.

## The First Big Idea Guessing the Answer

Since we cannot afford to look at all subarrays, what if we tried to guess the value of the k th largest subarray? Let us call our guessed value M. 

If we have a guess M, we can ask a much simpler question: "How many subarrays have a value that is greater than or equal to M?" 

If the answer to that question is greater than or equal to k, it means our guess M is either exactly right or a little bit too small. We can afford to guess a higher number.
If the answer is strictly less than k, it means our guess M was too ambitious. We need to guess a lower number.

This technique is known as binary searching the answer. We know the smallest possible subarray value is 0. We know the largest possible subarray value is the overall maximum element of the entire array minus the overall minimum element of the entire array. By repeatedly halving this search range, we can quickly zero in on the exact correct threshold M.

## Counting Valid Subarrays with a Sliding Window



Now we need a reliable way to answer the question: "How many subarrays have a value greater than or equal to M?"

We can use a sliding window approach. Imagine pointing to the end of a subarray with a right pointer, and pointing to the start with a left pointer. As we move our right pointer from the beginning to the end of the array, we want to find the tightest window where the maximum minus the minimum is still greater than or equal to M.

If the window from the left pointer to the right pointer has a valid value, then any larger window ending at the right pointer and starting before the left pointer will also be valid. Expanding a window can only introduce larger maximums or smaller minimums, which only increases the overall value. 

To keep track of the maximum and minimum numbers inside our sliding window efficiently, we can use two deques. One deque will maintain a decreasing sequence of numbers to help us quickly find the maximum. The other deque will maintain an increasing sequence to help us quickly find the minimum. This way, as we slide our pointers, we can always know the current maximum and minimum in constant time.

## Moving from Counting to Summing

Once our binary search finishes, we will have found our magic threshold M. We know that picking all subarrays with a value strictly greater than M will get us very close to our target k. 

However, we need the actual sum of these subarray values, not just the count. How do we quickly sum the maximums and minimums of thousands of overlapping subarrays without calculating them one by one?



This is where monotonic stacks come into play. A monotonic stack is a fantastic data structure that helps us group elements into blocks. Let us focus just on the maximums for a moment. As our right pointer moves along the array, the maximum element for various left pointer starting positions will change. 

We can store these maximums in a stack as blocks. Each block will record:
* The starting index for this block
* How many subarrays fall into this block
* The actual maximum value for these subarrays
* A running prefix sum of the values up to this block

When a new number comes in at the right pointer, we compare it to the blocks at the top of our stack. If the new number is larger, it "absorbs" the older, smaller blocks because this new larger number will now be the maximum for all those previous starting positions. We pop the smaller blocks off, combine their lengths, and push a single new block onto the stack with the updated value and updated prefix sum.

We maintain two completely separate stack structures: one for the maximum blocks and one for the minimum blocks.

## Extracting the Sum Using Binary Search

With our stack structures in place, as we iterate through the array with our right pointer, we also maintain our left pointer limit L. This limit L represents the boundary where all subarrays starting before L have a value strictly greater than our threshold M.

Since we need the sum of all maximums and minimums for subarrays starting from index 0 up to index L minus 1, we can search within our monotonic stacks. Because our blocks represent continuous ranges of starting indices, we can use a quick binary search within the stack itself to locate exactly which block contains our boundary L. Once we find the block, we use the precalculated prefix sums to get the total sum in a fraction of a second.

## Padding to the Target Count

There is one final detail. By summing all subarrays strictly greater than M, we might end up with a count that is slightly less than our target k. Why? Because there could be many subarrays that have a value of exactly M, and we need just enough of them to reach exactly k subarrays.

This is an easy fix. We take our target k, subtract the number of subarrays we already summed, and multiply the difference by M. We then add this padding to our total sum. This perfectly accounts for the remaining subarrays needed to complete our selection.

## The Complete Implementation

Below is the complete C++ implementation combining all these ideas. I have added comments to help guide you through the logic.

```cpp
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

class Solution {
    // Structure to represent a block of identical maximums or minimums
    struct Block {
        int start_idx;
        int len;
        long long val;
        long long pref_sum;
    };

    // Helper function to find the sum of elements up to a specific count
    long long query_st(int count, const vector<Block>& st) {
        if (count <= 0) return 0;
        int low = 0, high = st.size() - 1;
        int idx = 0;
        
        // Binary searching the correct block in our stack
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (st[mid].start_idx <= count - 1) {
                idx = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        long long prev_sum = (idx == 0) ? 0 : st[idx - 1].pref_sum;
        return prev_sum + (long long)(count - st[idx].start_idx) * st[idx].val;
    }

public:
    long long maxTotalValue(vector<int>& nums, int k) {
        int n = nums.size();
        if (n == 0 || k == 0) return 0;

        int max_val = nums[0], min_val = nums[0];
        for (int x : nums) {
            if (x > max_val) max_val = x;
            if (x < min_val) min_val = x;
        }

        vector<int> max_dq(n), min_dq(n);
        
        // Lambda function to count subarrays with value greater than or equal to M
        auto count_ge = [&](long long M) {
            long long cnt = 0;
            int L = 0;
            int max_head = 0, max_tail = 0;
            int min_head = 0, min_tail = 0;
            
            for (int r = 0; r < n; ++r) {
                while (max_tail > max_head && nums[max_dq[max_tail - 1]] <= nums[r]) max_tail--;
                max_dq[max_tail++] = r;
                
                while (min_tail > min_head && nums[min_dq[min_tail - 1]] >= nums[r]) min_tail--;
                min_dq[min_tail++] = r;
                
                while (L <= r && nums[max_dq[max_head]] - nums[min_dq[min_head]] >= M) {
                    if (max_dq[max_head] == L) max_head++;
                    if (min_dq[min_head] == L) min_head++;
                    L++;
                }
                cnt += L;
            }
            return cnt;
        };

        // Binary search for the threshold value M
        long long low = 0, high = max_val - min_val, M = 0;
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (count_ge(mid) >= k) {
                M = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        long long total_sum = 0;
        long long count_greater = 0;
        int L = 0;
        int max_head = 0, max_tail = 0;
        int min_head = 0, min_tail = 0;
        vector<Block> max_st; max_st.reserve(n);
        vector<Block> min_st; min_st.reserve(n);

        // Summing the exact values of all valid subarrays
        for (int r = 0; r < n; ++r) {
            while (max_tail > max_head && nums[max_dq[max_tail - 1]] <= nums[r]) max_tail--;
            max_dq[max_tail++] = r;
            
            while (min_tail > min_head && nums[min_dq[min_tail - 1]] >= nums[r]) min_tail--;
            min_dq[min_tail++] = r;
            
            // Updating the maximum monotonic stack
            int start = r;
            while (!max_st.empty() && max_st.back().val <= nums[r]) {
                start = max_st.back().start_idx;
                max_st.pop_back();
            }
            long long prev_max_sum = max_st.empty() ? 0 : max_st.back().pref_sum;
            long long length_max = r - start + 1;
            max_st.push_back({start, (int)length_max, nums[r], prev_max_sum + length_max * nums[r]});
            
            // Updating the minimum monotonic stack
            start = r;
            while (!min_st.empty() && min_st.back().val >= nums[r]) {
                start = min_st.back().start_idx;
                min_st.pop_back();
            }
            long long prev_min_sum = min_st.empty() ? 0 : min_st.back().pref_sum;
            long long length_min = r - start + 1;
            min_st.push_back({start, (int)length_min, nums[r], prev_min_sum + length_min * nums[r]});
            
            // Shift left pointer for values strictly greater than M
            while (L <= r && nums[max_dq[max_head]] - nums[min_dq[min_head]] >= M + 1) {
                if (max_dq[max_head] == L) max_head++;
                if (min_dq[min_head] == L) min_head++;
                L++;
            }
            
            count_greater += L;
            total_sum += query_st(L, max_st) - query_st(L, min_st);
        }

        // Final step padding up to exactly k elements using the threshold M
        return total_sum + (long long)(k - count_greater) * M;
    }
};
```

## Analyzing the Performance

It is always good practice to understand how our code performs.

### Time Complexity
Finding the threshold takes logarithmic time based on the maximum possible difference, which is essentially a constant factor of around 30 operations for standard integer ranges. Inside this search, we run a sliding window which takes linear time O(N). This makes the first part O(N log D) where D is the maximum difference. 
The second part iterates through the array once. During this iteration, we push and pop from our stacks, and we do a binary search inside the stack blocks. The stack operations average out to constant time per element, while the binary search takes O(log N) time. Therefore, the second part takes O(N log N) time. 
Overall, this is a very solid and practical time complexity that passes the problem constraints gracefully.

### Space Complexity
We use several vectors for our deques and stacks. Each vector can hold up to N elements in the worst case scenario. This gives us a space complexity of O(N), which is well within standard memory limits.

## Wrapping Up

I hope this walkthrough brought some clarity to a rather complex topic. Problems that require combining binary search on an answer with advanced data structures like monotonic stacks are incredibly rewarding to solve. By taking it step by step, identifying the bottlenecks, and systematically applying our toolkit, we turn an impossible billion operations problem into a neat, manageable algorithm. 

Thank you for reading along! I highly encourage you to try writing out the code yourself and experimenting with some small test arrays on paper to see the stacks grow and shrink. Happy coding!
