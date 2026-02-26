# Mastering Binary String Substring Counting with Constraints and Multiple Queries [#3261]

Hello everyone in the programming community! Today I would like to share a comprehensive and friendly walkthrough of a very interesting string manipulation and counting problem. I recently spent some time studying this challenge, and I want to share my notes and findings with all of you. My goal is to break down the logic step by step so that anyone, regardless of their current experience level, can understand the mechanics behind the solution. 

In this post, we will explore how to efficiently count specific substrings within various query ranges of a binary string. We will build our understanding from the ground up, starting with the problem description and moving towards a solid, well thought out logic combining a few classic algorithmic concepts.

## Understanding the Problem 

Let us first clearly define what we are trying to solve. You are given a string made entirely of characters that are either zeros or ones. You are also given an integer representing a constraint, which we will refer to as the k constraint. 

A substring is simply a continuous sequence of characters taken from the original string. We consider a substring to be "valid" or to "satisfy the k constraint" if it meets at least one of these two conditions
* The total number of zeros in the substring is less than or equal to the constraint k.
* The total number of ones in the substring is less than or equal to the constraint k.

If a substring has too many zeros AND too many ones, it is invalid. If it passes at least one of those checks, it is perfectly valid.

Along with the string and the constraint k, you receive a list of queries. Each query provides a starting boundary L and an ending boundary R. For every single query, your task is to look strictly at the segment of the string from index L to index R, and count exactly how many valid substrings are entirely contained within that segment. 

At first glance, this might sound straightforward, but the challenge lies in the sheer volume of queries and the potentially massive length of the string. We need a logic that can answer each query quickly without repeatedly doing the same work.

## The Brute Force Idea and Why We Need to Improve It

When tackling a new problem, it is always helpful to think about the most direct approach first. If we were to use a brute force method, we would process each query one by one. For a query bounded by L and R, we could generate every possible starting position inside that boundary. For each starting position, we could generate every possible ending position. Then, we would count the zeros and ones in that specific substring to see if it satisfies the constraint.

While this approach is completely correct in its logic, we must consider the processing time. If the string has a length N, generating all substrings within a query takes an amount of time proportional to N squared. If we have Q queries, the total operations would be roughly proportional to Q multiplied by N squared. Given that both the string length and the number of queries can be up to one hundred thousand, this brute force method will take far too long and result in a Time Limit Exceeded error. We need a strategy to prepare some information ahead of time so we can answer each query almost instantly.

## Building the Foundation with a Sliding Window



To improve our strategy, we can first ignore the queries and just look at the entire string. We want to know the longest valid substring ending at every possible position in the string. 

Imagine a window placed over our string. The window has a left edge and a right edge. We will expand the window by moving the right edge one character at a time. As we include a new character, we update our count of zeros and ones. 

If adding a character causes our window to violate the k constraint (meaning we now have strictly more than k zeros AND strictly more than k ones), our window is no longer valid. To fix this, we must shrink the window by moving the left edge to the right. We keep moving the left edge, discarding characters and updating our counts, until the window becomes valid again.

Because both the left and right edges only ever move from left to right, this sliding window process is very efficient. We will create an array called `left` to store the results. For every index right edge, `left[right edge]` will store the smallest valid starting index for a substring ending at that right edge. 

Let us walk through a very detailed example to make this crystal clear. Suppose our string is "010101" and our constraint k is 1. We will track our left pointer, our right pointer, and our counts.

* Right pointer at index 0 (character '0'). Counts are zero: 1, one: 0. This is valid because ones count is less than or equal to 1. The left array at index 0 stores 0.
* Right pointer at index 1 (character '1'). Counts are zero: 1, one: 1. This is valid. The left array at index 1 stores 0.
* Right pointer at index 2 (character '0'). Counts are zero: 2, one: 1. This is valid because ones count is still exactly 1! The left array at index 2 stores 0.
* Right pointer at index 3 (character '1'). Counts are zero: 2, one: 2. This is INVALID! Both counts exceed k (which is 1). We must move the left pointer.
    * The character at left pointer (index 0) is '0'. We remove it. Counts become zero: 1, one: 2.
    * Now the zero count is 1, which satisfies the constraint! We can stop shrinking. The left pointer is now at index 1.
    * The left array at index 3 stores 1.
* Right pointer at index 4 (character '0'). Counts are zero: 2, one: 2. INVALID! We move the left pointer.
    * The character at left pointer (index 1) is '1'. We remove it. Counts become zero: 2, one: 1. 
    * Valid again! The left pointer is now at index 2.
    * The left array at index 4 stores 2.

By the end of this single pass, we have perfectly mapped out the leftmost valid starting point for every ending position in the entire string!

## Handling Queries Efficiently

Now that we know the earliest valid starting point for every character, how does this help us answer the queries? 



[Image of binary search algorithm diagram]


A query gives us a strict boundary from index L to index R. We need to count valid substrings strictly within this boundary. Let us consider any ending position `j` that falls between L and R. How many valid substrings end exactly at position `j`?

Usually, the valid starting positions for ending position `j` range from `left[j]` all the way up to `j`. The total number of these substrings is `j minus left[j] plus 1`. 

However, we must be careful! The query explicitly restricts us to not look at any index before L. This means if `left[j]` is a number smaller than L, we cannot actually use it. The earliest starting point we are allowed to use is L itself. 

This creates a beautiful split in our logic for any given query bounded by L and R. As `j` moves from L to R, the value of `left[j]` will gradually increase (or stay the same). This means there are two distinct parts within our query range.

### Part One The Constrained Boundary
For the first few values of `j` in our query, `left[j]` might be smaller than or equal to L. Because we are trapped by the query boundary L, the valid starting points for these substrings will range from L up to `j`. 
The number of valid substrings ending at `j` in this part is simply `j minus L plus 1`. 
Let us write out the first few counts
* When j is L, the count is 1.
* When j is L plus 1, the count is 2.
* When j is L plus 2, the count is 3.

This forms a perfect arithmetic progression! If this part of the query has a total length `len`, the sum of all valid substrings in this section is calculated by the classic formula `len multiplied by (len plus 1) divided by 2`. This means we can count all substrings in this first section using a single mathematical operation!

### Part Two The Natural Boundary
Eventually, as `j` continues to increase towards R, `left[j]` will become strictly greater than L. Once this happens, the query boundary L no longer restricts us. The earliest valid starting point is naturally dictated by `left[j]`. 
The number of valid substrings ending at `j` in this part is exactly `j minus left[j] plus 1`.

To quickly sum these values for all remaining `j` positions up to R, we can use a Prefix Sum array. A prefix sum array is a technique where we precalculate running totals. We will create a prefix array where each position stores the sum of all `j minus left[j] plus 1` values up to that point. When we need the total for a specific range, we simply take the value at the end of the range and subtract the value just before the start of the range. This allows us to find the sum of any section in constant time!

## Locating the Split Point with Binary Search

We know our query range has two parts, but how do we find exactly where Part One ends and Part Two begins? 

Since `left[j]` is a non decreasing array (the values never go down as you move right), we can use Binary Search. Specifically, we can search for the very first index within our query range where the value in the `left` array becomes strictly greater than L. 

In standard programming libraries, this is often called an "Upper Bound" search. By using binary search, we can locate this split point extremely fast, taking only logarithmic time relative to the length of the string.

Once we find this split point, we calculate the arithmetic sum for Part One, calculate the prefix difference for Part Two, and add them together. That sum is the final answer for the query!

## Summary of the Algorithm Steps

Let us summarize our entire master plan before looking at the code
1. Set up a sliding window to traverse the string once. Fill an array `left` where `left[i]` is the smallest index such that the substring from `left[i]` to `i` is valid under the k constraint.
2. Build a prefix sum array. This array will store the running totals of `i minus left[i] plus 1` for every index `i`.
3. Loop through each query provided.
4. For a query from L to R, use binary search on the `left` array to find the index `split` where `left[split]` is strictly greater than L.
5. Calculate the total valid substrings from L up to `split minus 1` using the arithmetic progression formula.
6. Calculate the total valid substrings from `split` up to R using the prefix sum array.
7. Add the two totals together and store the result for that query.

## The Code Implementation

Below is the logic implemented in C++. The code utilizes standard vectors and algorithmic functions. Please observe how the sliding window and prefix sums are constructed exactly as we discussed. 

```cpp
class Solution {
public:
    vector<long long> countKConstraintSubstrings(string s, int k, vector<vector<int>>& queries) {
        int n = s.length();
        vector<int> left(n);
        vector<long long> prefix(n + 1, 0);

        // Step 1: Sliding window to find the earliest valid start for every end position
        int l = 0;
        int count0 = 0;
        int count1 = 0;
        
        for (int r = 0; r < n; ++r) {
            if (s[r] == '0') {
                count0++;
            } else {
                count1++;
            }

            // Shrink window from the left if the k constraint is violated
            while (count0 > k && count1 > k) {
                if (s[l] == '0') {
                    count0--;
                } else {
                    count1--;
                }
                l++;
            }
            // Record the furthest left starting point for the current right end
            left[r] = l;
        }

        // Step 2: Build a prefix sum array of valid substring counts bounded by left[i]
        for (int i = 0; i < n; ++i) {
            // (i - left[i] + 1) represents the number of valid substrings ending exactly at i
            prefix[i + 1] = prefix[i] + (i - left[i] + 1);
        }

        // Step 3: Answer each query rapidly
        vector<long long> ans;
        ans.reserve(queries.size());
        
        for (const auto& q : queries) {
            int L = q[0];
            int R = q[1];

            // Binary search to find the split point where the left constraint is greater than L
            // std::upper_bound finds the first element strictly greater than L
            int p = std::upper_bound(left.begin() + L, left.begin() + R + 1, L) - left.begin();
            
            // Part 1: Elements where left[j] is less than or equal to L
            // In this section, every substring must start at L or later
            long long len = p - L;
            long long contrib1 = len * (len + 1) / 2;
            
            // Part 2: Elements where left[j] is strictly greater than L
            // We use our precalculated prefix sums to get this total in one step
            long long contrib2 = prefix[R + 1] - prefix[p];
            
            ans.push_back(contrib1 + contrib2);
        }

        return ans;
    }
};

```

## Complexity Breakdown

It is always good practice to evaluate the time and memory resources our solution requires.

**Time Complexity** * The sliding window pass looks at each character in the string. The right pointer moves exactly N times, and the left pointer moves at most N times. This entire process takes O(N) time, where N is the length of the string.

* Building the prefix sum array is a single loop over the string, which also takes O(N) time.
* For answering the queries, we process Q queries. Inside the query loop, the only non constant operation is the binary search function. Searching within an array of size N takes logarithmic time, specifically O(log N). Thus, all queries together take O(Q multiplied by log N) time.
* Combining these phases, the overall time complexity is O(N + Q log N). This is incredibly fast and will comfortably pass all performance tests for this problem.

**Space Complexity**

* We create a `left` array of size N to store the sliding window results.
* We create a `prefix` array of size N plus 1 to store the running totals.
* We create an `ans` array to store the result for each of the Q queries.
* The overall auxiliary space complexity is O(N), which is perfectly reasonable and scales beautifully with the constraints provided.

## Conclusion

Thank you so much for reading through this detailed breakdown! We successfully transitioned from a Time Limit Exceeded brute force concept to a robust, combined algorithm. By understanding how to precalculate limits with a sliding window, how to aggregate totals with prefix sums, and how to query segments dynamically using binary search, we have crafted a solution that handles huge datasets gracefully.

I hope you found this explanation helpful for your learning journey. Please feel free to test the logic out by writing it out on paper with your own binary strings and k values. It is a fantastic way to solidify these concepts in your mind! Happy coding to you all!
