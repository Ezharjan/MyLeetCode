# A Friendly Guide To Exploring The Longest Increasing Subsequence With A Strict Value Gap Limit [#2407]

Hello everyone! Today I would love to share my thoughts and a detailed walkthrough on a very interesting array problem. The problem asks us to find a strictly increasing sequence of numbers from a given list, but it introduces a fascinating twist. The difference between any two adjacent numbers in our chosen sequence cannot exceed a specific limit, which we will call the gap limit. 

This type of problem is a wonderful opportunity to learn about combining classic dynamic programming concepts with clever data structures. Whether you are preparing for interviews or simply enjoy solving logical puzzles, I hope this guide will be a helpful and supportive resource for you. Let us explore the details together step by step!

### Understanding the Requirements

Before we dive into any coding, it is always a great idea to make sure we completely understand what the problem is asking. We are given an array of integers and an integer representing our maximum gap limit.

We need to extract a subsequence from the array. A subsequence is a new array formed by picking some elements from the original array while keeping them in their original relative order. You do not have to pick elements that are right next to each other in the original list.

However, our subsequence must satisfy two strict rules. First, it must be strictly increasing. This means every number we pick must be strictly greater than the number we picked right before it. Second, the difference between any two adjacent numbers in our newly formed subsequence must be at most our gap limit. 

For example, if our gap limit is 3, and we just picked the number 5, the next number we pick must be strictly greater than 5, but it cannot be greater than 8. If we pick 9, the difference would be 4, which breaks the rule. Our goal is to find the length of the longest possible subsequence that perfectly follows these rules.

### The Classic Approach and Its Limitations

If you have studied algorithms before, this problem might remind you of the classic Longest Increasing Subsequence problem. In the classic version, we usually create an array to keep track of the longest valid sequence ending at each specific index. 

For every new number we look at, we would look back at all the previous numbers we have processed. If a previous number is smaller than our current number, we could potentially add our current number to that sequence. To handle the gap limit in this new problem, we would just add one extra check. We would only consider previous numbers that are within our allowed gap range.

While this logical process is completely sound, we run into a bottleneck when the input array is very large. Looking back at previous elements for every single new element requires nested loops. In the worst case, the number of operations grows quadratically with the size of the input. For an array with a hundred thousand elements, this approach will take too long and will likely hit a time limit restriction. We need a way to look back and find the best previous sequence much faster.

### Introducing the Segment Tree

To speed up our search, we need a special tool. When we are looking at a new number, let us call it `current_value`, we want to find the longest sequence we have built so far that ends with a number between `current_value minus gap_limit` and `current_value minus 1`. 

Essentially, we are asking a question. What is the maximum length recorded in a specific range of values? 

A data structure called a Segment Tree is absolutely wonderful for this exact scenario. A segment tree allows us to query the maximum value in a specific range very quickly. It also allows us to update the value at a specific position very quickly. Both of these operations take logarithmic time, which is a massive improvement over scanning elements one by one.

Instead of building a complicated tree structure with objects and memory pointers, we can implement an iterative segment tree using a simple flat array. This approach is highly practical and avoids the overhead of recursive function calls.

### Deep Dive into the Iterative Segment Tree

Let us explore how an iterative segment tree works. Imagine we want to track values up to a certain maximum number, which we will call `M`. We will create an array of size `2 times M`. 

The beautiful part of this design is how we arrange the data. The actual values we want to update and query are stored in the second half of this array, starting from index `M`. The first half of the array, from index 1 to `M minus 1`, acts as the parent nodes that summarize the data. 

Every parent node at index `i` is responsible for storing the maximum value of its two children. The children of node `i` are always located at index `2 times i` and index `2 times i plus 1`. This mathematical relationship allows us to navigate up and down the tree using fast bitwise operations.

When we write our code, shifting bits to the left by one position is the same as multiplying by two. Shifting bits to the right by one position is the same as dividing by two. Using bitwise operations makes our code clean and fast.

### Building Our Tree Functions

We will need two primary functions for our segment tree. An update function and a query function.

The update function takes a position and a new value. Because our actual data sits in the second half of the tree array, we first add our maximum tree size `M` to the position. We update that specific spot with our new value. Then, we need to update all the parent nodes above it. We do this by continuously dividing our position by two to move up the tree. At each parent node, we look at its two children and store the larger of the two values. We keep doing this until we reach the very top of the tree.

The query function takes a left boundary and a right boundary. It finds the maximum value between these two boundaries inclusive. We adjust our boundaries to map to the second half of the tree array. Then, we look at our left and right pointers. If the left pointer is an odd number, it means it is a right child, and its parent includes data outside our queried range. So, we simply take the value at the left pointer, move the pointer one step to the right, and then divide by two to move up. We do a similar check for the right pointer. This process intelligently gathers the maximum value from the smallest necessary number of parent nodes.

### Connecting the Tree to Our Problem

Now we can bring everything together. In our specific problem, the positions in our segment tree will represent the actual numerical values from our input array. 

First, we need to find the absolute maximum value present in our input array. We will use this maximum value to determine the size of our segment tree. 

Next, we iterate through each number in our input array one by one. Let us say we are currently looking at the number `V`. We want to find the longest valid sequence we can attach `V` to. To do this, we use our segment tree query function. We ask the tree for the maximum value in the range from `V minus gap_limit` to `V minus 1`. We must also be careful to ensure our lower boundary does not drop below 1.

The segment tree will return the length of the longest sequence ending with a valid number. We simply add 1 to this length to account for our current number `V`. 

Finally, we update our segment tree. We tell the tree that a sequence ending with the value `V` now has this new, longer length. We repeat this process for every number in the array, keeping track of the largest length we ever calculate.

### Step by Step Example Walkthrough

To make things perfectly clear, let us trace through a small example together. Imagine our input array is `[1, 5]` and our gap limit is 1.

First, we find the maximum value in our array, which is 5. We will build a segment tree capable of holding values up to 5. 

We start processing the array with the first number, which is 1. We want to query the tree for sequences ending in values between `1 minus 1` and `1 minus 1`. That means we are looking for values between 0 and 0. Since our sequence numbers must be at least 1, this range is basically empty. The tree returns 0. We add 1 to get a new length of 1. We update the segment tree, telling it that sequences ending in 1 have a maximum length of 1.

Next, we look at the second number, which is 5. We query the tree for the range from `5 minus 1` to `5 minus 1`, which is exactly the range from 4 to 4. We ask the tree what the longest sequence ending in 4 is. Since we have not processed a 4 yet, the tree returns 0. We add 1 to get a new length of 1. We update the segment tree, telling it that sequences ending in 5 have a maximum length of 1.

We have finished the array. The longest sequence length we found is 1. This makes perfect sense because the difference between 5 and 1 is 4, which is much larger than our gap limit of 1. We cannot connect them together. The longest valid sequences are just the individual numbers themselves.

### The C++ Code Implementation

Below is the C++ code that implements all the ideas we have discussed. I have included comments throughout the code to help map the logic directly to the concepts we explored above.

```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums, int k) {
        // First, we find the maximum value in the array.
        // This helps us determine the size of our segment tree.
        int max_val = 0;
        for (int x : nums) {
            max_val = max(max_val, x);
        }
        
        // We set our tree base size. 
        // We add 1 so we can easily use the numbers directly as indices.
        int n = max_val + 1;
        
        // We initialize our flat array for the segment tree with zeros.
        // The size is twice the base size.
        vector<int> tree(2 * n, 0);
        
        // This is our helper function to update a value in the tree.
        auto update = [&](int pos, int value) {
            // We move to the leaf node in the second half of the array.
            pos += n;
            // We update the value if it is larger than what is already there.
            tree[pos] = max(tree[pos], value);
            // We move up the tree by dividing by 2 (shifting right).
            for (pos >>= 1; pos > 0; pos >>= 1) {
                // The parent node becomes the maximum of its two children.
                // Left child is pos << 1, right child is pos << 1 | 1.
                tree[pos] = max(tree[pos << 1], tree[pos << 1 | 1]);
            }
        };
        
        // This is our helper function to query the maximum in a range.
        auto query = [&](int l, int r) {
            int res = 0;
            // We increment right boundary to make the logic inclusive.
            r++; 
            // We shift both boundaries into the leaf node section.
            for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
                // If left boundary is a right child, process it and move right.
                if (l & 1) res = max(res, tree[l++]);
                // If right boundary is a right child, move left and process it.
                if (r & 1) res = max(res, tree[--r]);
            }
            return res;
        };
        
        int ans = 0;
        
        // We process each number in the array.
        for (int v : nums) {
            // We calculate the acceptable range of previous values.
            int left_bound = max(1, v - k);
            int right_bound = v - 1;
            
            int max_len = 0;
            // If the range is valid, we query our segment tree.
            if (left_bound <= right_bound) {
                max_len = query(left_bound, right_bound);
            }
            
            // We calculate the new length by adding 1.
            int new_len = max_len + 1;
            
            // We update the tree with our new length for the current value.
            update(v, new_len);
            
            // We keep track of the absolute longest length found so far.
            ans = max(ans, new_len);
        }
        
        return ans;
    }
};

```

### Performance Analysis

Whenever we write an algorithm, it is important to understand how much time and memory it will consume. Let us break down the performance of this approach.

For the time complexity, we iterate through our input array exactly one time. Let us call the length of our input array `N`. Inside our loop, we call our query function and our update function. Both of these functions climb the segment tree. The height of our tree is based on the maximum value in our array, which we can call `M`. Because the tree is binary, the height is the logarithm of `M`. Therefore, the time taken for each element is proportional to the logarithm of `M`. Multiplying this by the `N` elements in our array gives us a total time complexity of Big O of N multiplied by the logarithm of M. This is wonderfully efficient and will easily pass the problem constraints without any time limit issues.

For the space complexity, the primary memory we use is the array representing our segment tree. The size of this array is exactly twice the maximum value in our input list. Therefore, our space complexity is Big O of M. According to the problem constraints, the maximum value is one hundred thousand. Creating an integer array of size two hundred thousand requires less than one megabyte of memory, which is exceptionally light and completely safe.

### Final Thoughts

I hope this detailed walkthrough has been helpful to you! Combining array values as segment tree indices is a very powerful technique to keep in your problem solving toolkit. It transforms a slow lookup process into a rapid tree traversal.

Please take your time to read through the tree update and query logic. Drawing out a small tree on a piece of paper and manually tracing the bitwise shifts is one of the best ways to solidify your understanding. Keep practicing, stay curious, and happy coding!
