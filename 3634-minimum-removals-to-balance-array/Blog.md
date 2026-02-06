# Balancing Arrays by Keeping the Maximum Within Limits of the Minimum [#3634]


[Blog Post Link](https://leetcode.com/discuss/post/7558263/balancing-arrays-by-keeping-the-maximum-z9l34/)


Hello, fellow coding enthusiasts! It is wonderful to have you here. Today I want to share a fascinating problem that I recently encountered. It involves balancing an array of numbers, a concept that sounds simple but requires a bit of clever thinking to solve efficiently. If you have ever wondered how to determine the minimum number of elements to remove from a list to satisfy a specific proportional condition, this post is exactly for you.

We will walk through this problem together, starting from the very basics and building up to a solution that is both clean and effective. My goal is to make this logic accessible to everyone, whether you are writing your first line of code or you are a seasoned engineer looking for a refresher on sliding window techniques.

### The Problem Statement

Let us first clarify what we are trying to achieve.

We are given an array of integers called `nums` and a separate integer value `k`. Our objective is to ensure the array becomes **balanced**.

But what does it mean for an array to be balanced? The problem defines it quite strictly. An array is balanced if the **maximum** element in that array is **at most** `k` times the **minimum** element. In mathematical terms, if we let `max` be the largest value and `min` be the smallest value in our current selection, the condition is:

`max <= min * k`

We are allowed to remove any number of elements from the original array to achieve this state. However, we want to be as efficient as possible. This means we want to delete as few elements as we can. Another way to view this is that we want to **keep** the largest possible number of elements such that the condition holds true for the remaining set.

The final answer we need to return is that minimum number of removals.

#### A Note on Constraints

Before we dive into logic, we must look at the size of the data we are handling.

1. The length of the array can be up to 100,000. This suggests that a solution involving nested loops that compare every element against every other element might be too slow. We need something faster, likely linear or near linear time.
2. The values in the array can be as large as 1,000,000,000 (1 billion). This is a crucial detail! When we multiply a number by `k`, the result might exceed the limit of a standard 32 bit integer. We will need to be careful with our data types.

### Initial Thoughts and Observations

When I first looked at this problem, my instinct was to think about the relationship between the minimum and maximum values. In an unsorted array, the minimum and maximum could be anywhere. They could be at the start, at the end, or right in the middle. This randomness makes it very difficult to decide which elements to remove.

For example, imagine we have `[100, 2, 50]`.
The min is 2. The max is 100.
If `k` is 3, then `2 * 3 = 6`.
Since 100 is clearly greater than 6, this is not balanced. We have to remove something. Should we remove 100? Or should we remove 2? If we remove 2, the new min becomes 50, and `50 * 3 = 150`, which is greater than 100. So removing 2 fixes it. But if we remove 100, the remaining `[2, 50]` has min 2 and max 50. `2 * 3` is still 6, which is less than 50. So removing just 100 is not enough.

This trial and error process is confusing on an unsorted list. This brings us to our first major strategy decision.

### The Power of Sorting

Since we are allowed to remove any elements we want, the **order** of elements in the original array does not actually matter for the final condition. The condition only cares about magnitudes.

If we sort the array in ascending order (from smallest to largest), the structure of the problem becomes much clearer.

In a sorted array, for any subarray (a contiguous slice of the array), the **minimum** is always the first element of that slice, and the **maximum** is always the last element of that slice.

Let us denote our sorted array as `A`. If we choose to keep elements from index `i` to index `j` (where `i <= j`), then:

* `min` value = `A[i]`
* `max` value = `A[j]`

The condition `max <= min * k` simplifies to:
`A[j] <= A[i] * k`

Our goal of "minimizing removals" is mathematically identical to "maximizing the length of the subarray we keep". So, we are looking for the largest distance between `i` and `j` such that the condition holds.

### Developing a Strategy

Now that our array is sorted, we can imagine strictly looking at contiguous chunks. Why contiguous? Because if we pick a start index `i` and an end index `j`, and the condition holds for `A[i]` and `A[j]`, it will automatically hold for every number **between** them.

Why?
Any number `x` between index `i` and `j` will satisfy `A[i] <= x <= A[j]`.
Since `A[j]` is already small enough (less than or equal to `A[i] * k`), `x` will definitely be small enough too.
And since `A[i]` is the minimum, `x` is definitely larger than or equal to it.
Therefore, picking a contiguous range in a sorted array is the correct approach to forming a valid subset.

#### The Brute Force Way (Too Slow)

We could try checking every possible pair of `i` and `j`.

1. Loop `i` from 0 to the end.
2. Loop `j` from `i` to the end.
3. Check if `A[j] <= A[i] * k`.
4. If it is, record the length `j - i + 1`.
5. Pick the max length found.

With 100,000 elements, this creates roughly 5 billion pairs to check. That is far too much work for a computer to finish in a reasonable time limit (usually 1 second). We need a better tool.

#### The Sliding Window Approach

This is where the "Sliding Window" technique shines. It allows us to solve this in a single pass after sorting.

Imagine a window defined by two pointers, `left` and `right`.

* `right` represents the end of our current selection (the maximum value).
* `left` represents the start of our current selection (the minimum value).

We want to expand `right` as much as possible. As we move `right` forward to include larger numbers, we might eventually hit a number that is "too big" for our current `left` (minimum).

When `A[right]` becomes greater than `A[left] * k`, our window is invalid. The maximum is essentially "too heavy" for the minimum to support. To fix this, we must increase the minimum. We do this by moving the `left` pointer forward. Moving `left` forward gives us a larger minimum value (since the array is sorted), which increases the allowable threshold (`min * k`).

We keep moving `left` forward until the window becomes valid again. At every valid step, we measure the size of the window and see if it is the largest we have found so far.

### The Algorithm Step by Step

Here is the precise recipe we will follow:

1. **Sort the Input**: Take the input array `nums` and sort it in non decreasing order.
2. **Initialize Variables**:
* `max_kept`: An integer to track the maximum number of elements we have found so far that satisfy the condition. Start it at 0.
* `left`: An integer index pointing to the start of our window. Start it at 0.
* `n`: The total size of the array.


3. **Iterate with a Right Pointer**:
* Create a loop where `right` goes from 0 up to `n minus 1`.
* Inside the loop, check the condition. Is `nums[right] > nums[left] * k`?
* **The Adjustment**: If the condition fails (meaning the value at `right` is too large), we increment `left` by 1. We repeat this check until the condition is true or `left` catches up to `right`.
* **Record Success**: Once the condition is met, the window from `left` to `right` is valid. The number of elements in this window is `right - left + 1`. We update `max_kept` if this new size is larger than what we had before.


4. **Calculate Removals**:
* After the loop finishes, we know the maximum number of elements we can keep is `max_kept`.
* The answer is the total elements `n` minus `max_kept`.


5. **Return the Answer**.

### Handling Large Numbers (Overflow Warning)

There is one technical trap to avoid. The problem states that elements can be up to 1,000,000,000.
If `nums[left]` is 1 billion and `k` is, say, 3, then `nums[left] * k` is 3 billion.
A standard signed 32 bit integer (often just `int` in C++) can only hold values up to roughly 2.14 billion.
If we calculate 3 billion in a standard `int`, it will "overflow" and wrap around to a negative number, causing our logic to fail completely.

To prevent this, we must cast our numbers to a larger type, such as `long long` in C++, before performing the multiplication. This ensures we have enough space to store the result of the multiplication safely.

### The C++ Solution

Here is the complete solution code. I have written it to be clean and easy to read, with comments explaining the logic.

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    int minRemoval(vector<int>& nums, int k) {
        // First, we sort the array. This is critical because it allows us
        // to treat a contiguous subarray as a set of numbers where
        // nums[i] is the minimum and nums[j] is the maximum.
        sort(nums.begin(), nums.end());
        
        int n = nums.size();
        int max_kept = 0;
        int left = 0;
        
        // We iterate with 'right' acting as the end of our current window.
        // This pointer represents the index of the maximum element in the 
        // current subarray we are considering.
        for (int right = 0; right < n; ++right) {
            
            // We need to check if the current window is valid.
            // The condition is: maximum <= minimum * k
            // Here, maximum is nums[right] and minimum is nums[left].
            // We cast to (long long) to prevent integer overflow during multiplication.
            long long limit = (long long)nums[left] * k;
            
            // If the current maximum exceeds the limit allowed by the current minimum,
            // we must shrink the window from the left. Moving 'left' forward increases
            // the minimum value, which raises the allowed limit.
            while ((long long)nums[right] > limit) {
                left++;
                // Re-calculate limit based on the new left
                if (left < n) {
                    limit = (long long)nums[left] * k;
                } else {
                    break;
                }
            }
            
            // At this point, the window from 'left' to 'right' satisfies the condition.
            // We calculate the size of this window.
            int current_window_size = right - left + 1;
            
            // We update our record of the largest valid window found so far.
            if (current_window_size > max_kept) {
                max_kept = current_window_size;
            }
        }
        
        // The problem asks for the minimum number of elements to REMOVE.
        // This is simply the total count minus the maximum number we can KEEP.
        return n - max_kept;
    }
};

```

### A Walkthrough with an Example

Let us trace the code with a concrete example to solidify our understanding.

**Input:** `nums = [1, 6, 2, 9]`, `k = 3`

**Step 1: Sort**
The array becomes `[1, 2, 6, 9]`.
`n = 4`.

**Step 2: Loop starts**
`max_kept = 0`, `left = 0`.

**Iteration 1 (`right = 0`):**

* Value at `right` is `1`. Value at `left` is `1`.
* Condition: Is `1 > 1 * 3`? No.
* Window size: `0 - 0 + 1 = 1`.
* `max_kept` becomes `1`.

**Iteration 2 (`right = 1`):**

* Value at `right` is `2`. Value at `left` is `1`.
* Condition: Is `2 > 1 * 3`? No.
* Window size: `1 - 0 + 1 = 2`.
* `max_kept` becomes `2`.

**Iteration 3 (`right = 2`):**

* Value at `right` is `6`. Value at `left` is `1`.
* Condition: Is `6 > 1 * 3`? Yes (6 is greater than 3).
* **While loop triggers:**
* Increment `left` to `1`. New min is `nums[1]` which is `2`.
* New limit is `2 * 3 = 6`.
* Check again: Is `6 > 6`? No.


* Window size: `2 - 1 + 1 = 2`.
* `max_kept` stays `2`.

**Iteration 4 (`right = 3`):**

* Value at `right` is `9`. Value at `left` is `2`.
* Condition: Is `9 > 2 * 3`? Yes (9 is greater than 6).
* **While loop triggers:**
* Increment `left` to `2`. New min is `nums[2]` which is `6`.
* New limit is `6 * 3 = 18`.
* Check again: Is `9 > 18`? No.


* Window size: `3 - 2 + 1 = 2`.
* `max_kept` stays `2`.

**End of Loop:**
The loop finishes. `max_kept` is `2`.
Result: `4 - 2 = 2`.

We remove 2 elements. Looking at the sorted array `[1, 2, 6, 9]`, the valid subarrays of length 2 could be `[1, 2]` or `[6, 9]`. Both are valid. We keep 2, remove 2.

### Complexity Analysis

It is always good practice to discuss efficiency.

1. **Time Complexity**:
The most "expensive" part of our solution is the sorting step. Standard sorting algorithms take `O(N log N)` time, where `N` is the number of elements.
The sliding window part involves the `right` pointer moving from start to end exactly once. The `left` pointer also moves from start to end at most once (it never moves backward). This means the sliding window part is `O(N)`.
Combining these, the total time complexity is dominated by the sort: **O(N log N)**.
Given `N = 100,000`, `N log N` is roughly `1,700,000` operations, which is well within the limits of modern processors for a 1 second timeout.
2. **Space Complexity**:
We are not using any extra data structures like HashMaps or new Arrays. We only use a few integer variables (`left`, `right`, `max_kept`).
However, the sorting function itself might use some stack space (usually `O(log N)`).
So, generally, we consider this to be **O(1)** or **O(log N)** auxiliary space depending on how strictly you define the sorting internals. This is very memory efficient.

### Corner Cases to Consider

* **Array size 1**: The loop runs once. `right=0`, `left=0`. Condition `nums[0] > nums[0] * k` is never true (unless k is negative, but constraints say k >= 1). Size is 1. Removals = 0. Correct.
* **All elements equal**: `nums = [5, 5, 5]`, `k=1`. The condition `5 > 5 * 1` is false. The window grows to encompass the whole array. Removals = 0. Correct.
* **Strictly increasing large gaps**: `nums = [1, 100, 1000]`, `k=2`.
* Window `[1]`: ok.
* Window `[1, 100]`: `100 > 1*2`, left moves to 100. Window becomes `[100]`.
* Window `[100, 1000]`: `1000 > 100*2`, left moves to 1000. Window becomes `[1000]`.
* Max kept is 1. Removals = 2. Correct.



### Final Thoughts

This problem is a classic example of how sorting can transform a difficult "selection" problem into a simpler "contiguous subarray" problem. By organizing the data first, we enabled the use of the sliding window technique, which is a powerful tool in any programmer's kit.

I hope this explanation was clear and helpful! It is always satisfying to see a complex condition boil down to a neat loop with two pointers. If you have any questions or alternate ways you solved this, please feel free to share. Happy coding!
