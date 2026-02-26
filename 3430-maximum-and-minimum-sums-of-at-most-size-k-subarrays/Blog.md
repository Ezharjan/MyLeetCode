# Conquering Maximum and Minimum Sums in Bounded Array Segments [#3430]

Hello everyone! Welcome to another programming discussion. Today, I want to share a thorough and friendly deep dive into an incredibly fascinating array manipulation problem. We will be exploring the mechanics behind finding the maximum and minimum values across various array segments and summing them up. We will break this down step by step so that every member of our community, regardless of their current experience level, can grasp the underlying logic. 

If you have ever felt overwhelmed by problems involving segment boundaries and combinations, you are definitely not alone. I have been there many times myself! Grab a warm beverage, and let us explore this together. I have written this guide to be as detailed as possible, spanning the equivalent of several pages of learning material, to ensure absolutely no detail is left unexplained.



### Understanding the Core Challenge

Before we write any logic, we must clearly understand what the problem is asking us to achieve. 

Imagine you are given a list of integers, which we will call `nums`. You are also given a positive integer `K`. Your task is to look at every possible contiguous segment within this list (which we call a subarray) that has a length of at most `K`. 

For every single one of these valid segments, you need to identify two things:
1. The smallest number present in that segment.
2. The largest number present in that segment.

Once you have identified these two values for a particular segment, you add them together. You repeat this identification and addition for every single valid segment in the entire list, accumulating a massive total sum. Finally, you return this total sum.

Consider a small example where our list is `[1, 2, 3]` and `K` is 2.
The valid segments of length 1 or 2 are:
* The segment containing just `1`. Its minimum is 1, maximum is 1. Sum is 2.
* The segment containing just `2`. Its minimum is 2, maximum is 2. Sum is 4.
* The segment containing just `3`. Its minimum is 3, maximum is 3. Sum is 6.
* The segment `[1, 2]`. Its minimum is 1, maximum is 2. Sum is 3.
* The segment `[2, 3]`. Its minimum is 2, maximum is 3. Sum is 5.

Adding all these sums together gives us 20. 

While this sounds very simple on a small scale, the constraints of the problem tell us that our list can have up to 80000 elements! If we try to list out every single segment one by one, our program will take far too long to finish. We need a more practical approach that scales well with large amounts of data.

### The Naive Approach and Why We Need to Dig Deeper

When we first look at this challenge, the most intuitive method is to use nested loops. We could pick a starting point, then pick an ending point that is no further than `K` steps away. We could then scan the elements between the start and end to find the minimum and maximum, and add them to our total.

Why does this struggle with large lists? 
If you have 80000 elements, the number of possible segments is enormous. The time complexity for this naive method scales quadratically, which means the number of operations grows like a square of the input size. For an array of 80000 elements, this results in billions of operations. Our computers are incredibly fast, but running billions of operations for a single problem will exceed the time limits set by competitive programming platforms.

Therefore, we must completely flip our perspective. Instead of looking at the segments and finding their extremes, let us look at each individual number and ask: "In how many valid segments will this specific number be the absolute minimum? In how many will it be the absolute maximum?" 

This shift in perspective is known as the Contribution Technique. It is a highly effective way to solve these kinds of counting puzzles.

### The Contribution Technique

Let us focus purely on the minimums for a moment. The logic we build here will apply perfectly to the maximums later, just with the comparison signs reversed.

For any specific number at a specific position in our list, we want to expand a window to the left and to the right. We want to keep expanding this window as long as the numbers we encounter are larger than our specific number. The moment we hit a number that is smaller than our current number, we must stop. That smaller number dictates the boundary. If our segment includes that smaller number, our current number is no longer the minimum!

So, for every element, we need to find two boundaries:
* The Left Boundary: The closest index to the left that holds a smaller value.
* The Right Boundary: The closest index to the right that holds a smaller value.

Once we know these boundaries, we know that any segment starting after the left boundary and ending before the right boundary, which also includes our specific element, will have our element as its absolute minimum.

#### Handling Duplicates Gracefully

There is a subtle trap here. What if our list has duplicate numbers? For example, in the list `[2, 2, 2]`, which `2` is the minimum of the entire segment? 

If we say every `2` is the minimum, we might accidentally count the same segment multiple times, inflating our final sum. To fix this, we establish a strict rule: the leftmost occurrence of a duplicate value takes the crown. 

To implement this rule technically:
* When searching for the left boundary, we stop at numbers that are strictly smaller. We allow our segment to include equal numbers to the left.
* When searching for the right boundary, we stop at numbers that are smaller OR equal. This prevents the right side from including another identical number, ensuring only the first instance claims the segment.



### Introducing the Monotonic Stack

Finding the left and right boundaries for every single element could still be slow if we scan outward one by one. To do this efficiently, we use a wonderful data structure called a Monotonic Stack.

A stack is like a stack of plates; you can only add or remove from the top. A monotonic stack is simply a stack that we force to stay sorted. 

If we want to find the previous smaller elements (our left boundaries), we iterate through our list from left to right. Before we push the current index onto our stack, we look at the top of the stack. If the value at the top of the stack is greater than or equal to our current value, we pop it off. We keep popping until we find a smaller value or the stack becomes empty. 

The index that remains at the top of the stack is our exact left boundary! We then push our current index onto the stack and move to the next element. We repeat this process from right to left to find the right boundaries. This allows us to find all boundaries for all elements in a single pass, which is remarkably fast.

### The Length Constraint Challenge

If there was no length limit `K`, counting the segments would be easy. If an element has `X` possible starting positions to its left and `Y` possible ending positions to its right, the total number of segments is simply `X` multiplied by `Y`.

However, we have a strict rule: the total length of the segment cannot exceed `K`.

Let us define this mathematically without using complex equations. 
Let the number of valid items in the left portion (including the element itself) be `X`.
Let the number of valid items in the right portion (excluding the element itself) be `Y`.
Let `u` represent how far left we stretch. `u` can range from `0` up to `X minus 1`.
Let `v` represent how far right we stretch. `v` can range from `0` up to `Y minus 1`.

The length of our segment is `u plus v plus 1`. 
The problem states the length must be less than or equal to `K`. 
Therefore, `u plus v plus 1` must be less than or equal to `K`.
This means `u plus v` must be less than or equal to `K minus 1`. Let us call `K minus 1` our `Limit`.

We now have a fascinating discrete geometry puzzle. We need to count how many pairs of `u` and `v` exist such that:
* `u` is between `0` and `X minus 1`
* `v` is between `0` and `Y minus 1`
* `u plus v` is less than or equal to `Limit`.



### Breaking Down the Geometric Combinations

To count these valid pairs without using another slow loop, we can use simple area calculations. Imagine a grid where the horizontal axis represents our choices for `u` and the vertical axis represents our choices for `v`. We are drawing a bounding box of width `X` and height `Y`. We are also drawing a diagonal line representing `u plus v equals Limit`. We want to count all the grid points that fall inside the box AND below or on the diagonal line.

To make the logic simpler, we can always ensure that `X` is less than or equal to `Y`. If it is not, we just swap their values. The total number of valid pairs remains exactly the same.

There are four distinct scenarios we must handle in our calculation logic:

**Scenario 1: The Limit is incredibly small.**
If the `Limit` is less than zero, it means we cannot even form a segment of length 1. The number of valid combinations is completely zero.

**Scenario 2: The Limit forms a small triangle.**
If the `Limit` is strictly less than `X`, the diagonal line cuts off a small triangle right at the origin of our grid. It never hits the top or the right side of our bounding box. 
The number of valid pairs is simply the sum of numbers from 1 up to `Limit plus 1`.
The formula for the sum of an arithmetic progression gives us: `(Limit plus 1) multiplied by (Limit plus 2), all divided by 2`.

**Scenario 3: The Limit cuts off the right side but not the top.**
If the `Limit` is strictly less than `Y`, the diagonal line hits the right edge of our bounding box (width `X`) but stays below the top edge. 
To find the valid points here, we take a rectangle of width `X` and height `Limit plus 1`, and then subtract the small empty triangle that gets cut off at the bottom right.
The total valid pairs are calculated as: `X multiplied by (Limit plus 1)` minus `X multiplied by (X minus 1) divided by 2`.

**Scenario 4: The Limit cuts off both boundaries.**
If the `Limit` is large enough to hit both the top and the right side of the bounding box, but small enough that it does not cover the entire box, we calculate the total area of the box and subtract the small empty triangle in the top right corner.
The total area of the box is `X multiplied by Y`. 
The empty region at the top right is determined by a reverse limit. Let us define the reverse limit as `X plus Y minus 3 minus Limit`.
We subtract the combinations of the reverse limit: `(Reverse limit plus 1) multiplied by (Reverse limit plus 2) divided by 2`.

**Scenario 5: The Limit is massive.**
If the `Limit` is extremely large, our diagonal line is completely outside our bounding box. Every single combination of `u` and `v` is perfectly valid. The answer is simply the total area: `X multiplied by Y`.

By isolating this counting logic into a dedicated helper function, we keep our main logic clean and highly readable.

### Putting It All Together in C++

Now that we have thoroughly explored the theory, let us look at how this translates into practical, clean C++ code. I have structured this to be as clear as possible, avoiding overly condensed tricks in favor of readable steps.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    // This helper function counts the number of valid (u, v) pairs
    // It implements the discrete geometry logic we discussed in detail
    long long count_pairs(long long x, long long y, long long k_limit) {
        if (k_limit < 0) return 0;
        
        // Ensure x is always the smaller dimension to simplify our cases
        if (x > y) swap(x, y);
        
        // Scenario 2: Small triangle at the origin
        if (k_limit < x) {
            return (k_limit + 1) * (k_limit + 2) / 2;
        }
        // Scenario 3: Triangle hits the x boundary but stays below y
        if (k_limit < y) {
            return x * (k_limit + 1) - x * (x - 1) / 2;
        }
        // Scenario 4: Triangle hits both boundaries, subtract the empty corner
        if (k_limit < x + y - 2) {
            long long reverse_limit = x + y - 3 - k_limit;
            return x * y - (reverse_limit + 1) * (reverse_limit + 2) / 2;
        }
        // Scenario 5: The limit encompasses all possible pairs
        return x * y;
    }

public:
    long long minMaxSubarraySum(vector<int>& nums, int k) {
        int length_of_array = nums.size();
        long long grand_total = 0;
        long long constraint_limit = k - 1;
        
        // Arrays to hold the left and right boundaries for each element
        vector<int> left_bounds(length_of_array, -1);
        vector<int> right_bounds(length_of_array, length_of_array);
        
        // We use a vector as our stack for fast memory access
        vector<int> index_stack; 
        index_stack.reserve(length_of_array);
        
        // ==========================================
        // STEP 1: Process all the Minimum Contributions
        // ==========================================
        
        // Find the Left boundaries for minimums (strictly less to avoid duplicate counting)
        for (int i = 0; i < length_of_array; ++i) {
            while (!index_stack.empty() && nums[index_stack.back()] > nums[i]) {
                index_stack.pop_back();
            }
            if (!index_stack.empty()) {
                left_bounds[i] = index_stack.back();
            } else {
                left_bounds[i] = -1;
            }
            index_stack.push_back(i);
        }
        
        index_stack.clear();
        
        // Find the Right boundaries for minimums (less than or equal to)
        for (int i = length_of_array - 1; i >= 0; --i) {
            while (!index_stack.empty() && nums[index_stack.back()] >= nums[i]) {
                index_stack.pop_back();
            }
            if (!index_stack.empty()) {
                right_bounds[i] = index_stack.back();
            } else {
                right_bounds[i] = length_of_array;
            }
            index_stack.push_back(i);
        }
        
        // Calculate the contributions of the minimums
        for (int i = 0; i < length_of_array; ++i) {
            long long left_choices = i - left_bounds[i];
            long long right_choices = right_bounds[i] - i;
            long long valid_combinations = count_pairs(left_choices, right_choices, constraint_limit);
            grand_total += valid_combinations * nums[i];
        }
        
        // ==========================================
        // STEP 2: Process all the Maximum Contributions
        // ==========================================
        
        // Reset boundaries for the next phase
        fill(left_bounds.begin(), left_bounds.end(), -1);
        fill(right_bounds.begin(), right_bounds.end(), length_of_array);
        index_stack.clear();
        
        // Find the Left boundaries for maximums (strictly greater to avoid duplicate counting)
        for (int i = 0; i < length_of_array; ++i) {
            while (!index_stack.empty() && nums[index_stack.back()] < nums[i]) {
                index_stack.pop_back();
            }
            if (!index_stack.empty()) {
                left_bounds[i] = index_stack.back();
            } else {
                left_bounds[i] = -1;
            }
            index_stack.push_back(i);
        }
        
        index_stack.clear();
        
        // Find the Right boundaries for maximums (greater than or equal to)
        for (int i = length_of_array - 1; i >= 0; --i) {
            while (!index_stack.empty() && nums[index_stack.back()] <= nums[i]) {
                index_stack.pop_back();
            }
            if (!index_stack.empty()) {
                right_bounds[i] = index_stack.back();
            } else {
                right_bounds[i] = length_of_array;
            }
            index_stack.push_back(i);
        }
        
        // Calculate the contributions of the maximums
        for (int i = 0; i < length_of_array; ++i) {
            long long left_choices = i - left_bounds[i];
            long long right_choices = right_bounds[i] - i;
            long long valid_combinations = count_pairs(left_choices, right_choices, constraint_limit);
            grand_total += valid_combinations * nums[i];
        }
        
        return grand_total;
    }
};

```

### Understanding the Resource Usage

It is always important to evaluate how our solution performs in terms of time and memory. This ensures our method is practical for large scale inputs.

**Time Complexity Considerations:**
If we analyze the loops in our code, we see several passes over the data. We have separate loops to find the left bounds for minimums, the right bounds for minimums, the sum of minimums, and then we repeat these exact four passes for the maximums.
Inside the boundary finding loops, we use a stack. While there is a inner loop `while` statement, every single index is pushed onto the stack exactly one time and popped off the stack at most one time. Therefore, the operations inside the `while` loop run a total number of times directly proportional to the size of the array across the entire execution.
Because we do a fixed number of linear passes, the execution time grows directly in proportion to the number of elements in the input list. In academic terms, this is expressed as Big O of N time complexity, where N is the length of the array. This is fantastic and will easily process 80000 elements in a fraction of a second.

**Space Complexity Considerations:**
Our code creates a few extra storage vectors. We have one vector for the left boundaries, one for the right boundaries, and one vector acting as our stack. Each of these vectors can grow to store a number of elements equal to the input array size.
Since the memory we allocate scales directly with the input size, our space complexity is also Big O of N. This is a very reasonable trade off for achieving such a fast execution time.

### Final Thoughts

This problem is a beautiful illustration of how changing your perspective from "evaluate every segment" to "evaluate every item's contribution" can turn an impossible task into an elegant logic puzzle. By combining the Contribution Technique with Monotonic Stacks and a touch of discrete geometry, we established a highly effective method to solve the challenge.

I hope this thorough walkthrough has demystified the process for you. Take your time reading through the math logic, perhaps draw out the grid on a piece of paper for a small array, and see how the bounding boxes form.

Happy coding, and please feel free to ask any questions or share your own observations down below!
