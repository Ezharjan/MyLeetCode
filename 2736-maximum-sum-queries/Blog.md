# Unlocking Maximum Sums with Two Arrays and Multiple Constraints [#2736]

Welcome fellow coders! Today I am incredibly excited to share a deep dive into an absolutely fascinating algorithm challenge. Whether you are preparing for technical interviews or simply love stretching your problem solving muscles, this specific puzzle is a treasure trove of clever techniques. We are going to explore a capable solution that relies on organizing our data intelligently and keeping track of only the most valuable information. 

Grab a cup of coffee, get comfortable, and let us break down this entire process step by step!

## Understanding the Problem

Imagine you are handed two zero indexed arrays of integers, named `nums1` and `nums2`. Both of these arrays have the exact same length, which we will call `n`. 

Along with these two arrays, you are given a set of queries. Each individual query provides you with two specific threshold values, let us call them `x` and `y`. 

For every single query, your mission is to search through all valid indices `j` from `0` up to `n` (excluding `n` itself) and find a specific index `j` that satisfies two mandatory conditions simultaneously:
* The value at `nums1[j]` must be greater than or equal to `x`.
* The value at `nums2[j]` must be greater than or equal to `y`.

If you can find one or more indices that satisfy both of these conditions, you need to calculate the sum of `nums1[j]` and `nums2[j]`. Your ultimate goal for that query is to return the absolute maximum sum possible among all the valid indices. If absolutely no index satisfies both conditions for a given query, you must return negative one for that query.

### A Quick Example

Let us look at a simple scenario to make sure we are all on the same page. 
Suppose our arrays are:
* `nums1` contains [4, 3, 1, 2]
* `nums2` contains [2, 4, 9, 5]

And we have a query where `x` is 4 and `y` is 1.

We check index 0: `nums1[0]` is 4, which is greater than or equal to 4. `nums2[0]` is 2, which is greater than or equal to 1. Both conditions are met! The sum here is 4 plus 2, which equals 6.
We check index 1: `nums1[1]` is 3. This is strictly less than 4, so this index fails immediately.
We check index 2: `nums1[2]` is 1. This also fails the first condition.
We check index 3: `nums1[3]` is 2. This fails the first condition as well.

So for this specific query, the maximum sum we can obtain is 6.

## Why the Brute Force Approach Falls Short

When we first look at this puzzle, the most intuitive approach is to use a nested loop structure. We could iterate through every single query one by one. For each query, we could then iterate through every single index `j` from 0 to the end of the arrays. 

During this inner loop, we would check if the conditions are met. If they are, we calculate the sum and update our maximum sum tracker for that query. 

While this brute force method is logical and easy to implement, it completely falls apart when we look at the constraints. The size of the arrays can be up to 100,000. The number of queries can also be up to 100,000. 

If we multiply these together, in the worst case scenario, our program would have to perform 10 billion operations! Modern judging environments typically allow around 100 million operations per second. A brute force approach would trigger a Time Limit Exceeded error almost instantly. We need a strategy that is significantly more refined.

## The Strategy of Processing Queries Offline



When you realize that evaluating every query independently takes too much time, a powerful technique to reach for is offline query processing. 

Notice that the problem does not require us to answer the queries in the exact order they were given. We just need to return an array of answers that corresponds to the original query order. This gives us the freedom to rearrange the queries in a way that makes our calculations much easier.

Let us bundle everything together. We will pair each `nums1[j]` with its corresponding `nums2[j]` and their pre calculated sum. We will consider each bundle as a single "item". 

Next, let us sort all of these items based on their `nums1` values in descending order. The item with the largest `nums1` value comes first.

We will do the same thing with our queries! We will sort all the queries based on their `x` threshold values in descending order. By doing this, the first query we process will be the one with the strictest, highest requirement for `nums1`.

Why is this sorting step so magical? Because as we move from the first sorted query to the second, and then to the third, the `x` threshold only ever decreases. This guarantees an incredible property: any item that had a large enough `nums1` value to satisfy the previous query will automatically have a large enough `nums1` value to satisfy the current query and all future queries!

Instead of searching through the entire array for every query, we can maintain a growing "active set" of valid items. As the `x` requirement drops, we simply add new valid items into our active set. We never have to remove them based on the `x` requirement again.

## The Magic of the Monotonic Stack



Now we have a growing active set of items, and we know for an absolute fact that every item in this active set currently satisfies the `nums1` threshold. The first condition is completely taken care of!

Our only remaining job for the current query is to look inside this active set, find all items that also satisfy the second condition (where `nums2` is greater than or equal to `y`), and pick the one with the highest total sum.

But as our active set grows to contain tens of thousands of items, searching through it could still be too slow. We need a way to filter out the "useless" items from our active set. 

Here is where we introduce the most beautiful mathematical deduction of this entire solution.

Suppose we want to add a newly valid item into our active set. Let us call this new item `Item B`. Let us say there is an older item already inside our active set, which we will call `Item A`. 

Because we process items in descending order of `nums1`, we know with absolute certainty that `Item A` has a `nums1` value that is greater than or equal to `Item B`.

Now, imagine that `Item B` has a total sum that is greater than or equal to the total sum of `Item A`. 

Let us write this out logically:
Sum of B is greater than or equal to Sum of A.
Since Sum is exactly `nums1` plus `nums2`, this means:
(nums1 of B) plus (nums2 of B) is greater than or equal to (nums1 of A) plus (nums2 of A).

We already established that (nums1 of A) is greater than or equal to (nums1 of B). 
For the sum of B to catch up and actually be larger than the sum of A, despite starting with a smaller `nums1` value, (nums2 of B) must be strictly greater than (nums2 of A)!

Think about the profound implication of this. If `Item B` has a larger sum, it is mathematically guaranteed to also have a larger `nums2` value than `Item A`. 

So, what does `Item A` offer us? 
Does it have a better sum? No, `Item B` has a better sum.
Does it help us satisfy strict `y` queries better? No, `Item B` has a larger `nums2`, so `Item B` can satisfy even stricter `y` queries than `Item A`.

`Item A` is completely and totally dominated by `Item B`. `Item A` will never be the optimal answer for any current or future query. We can permanently delete `Item A` from our active set!

By continuously removing dominated items, we naturally form a Monotonic Stack. If we push items into a stack, and pop the top of the stack whenever the incoming item has a greater or equal sum, our stack will maintain a strict ordering. From the bottom of the stack to the top, the `nums2` values will be strictly increasing, and the sum values will be strictly decreasing.

## Walking Through a Detailed Trace

To make this completely transparent, let us trace through our earlier example with our new strategy.

Arrays: `nums1` is [4, 3, 1, 2], `nums2` is [2, 4, 9, 5].
Items (nums1, nums2, sum): (4, 2, 6), (3, 4, 7), (1, 9, 10), (2, 5, 7).

Step 1: Sort items descending by `nums1`.
Item 1: (4, 2, sum is 6)
Item 2: (3, 4, sum is 7)
Item 3: (2, 5, sum is 7)
Item 4: (1, 9, sum is 10)

Queries (x, y): [4, 1], [1, 3], [2, 5].
Step 2: Sort queries descending by `x`.
Query 1: x is 4, y is 1 (original index 0)
Query 2: x is 2, y is 5 (original index 2)
Query 3: x is 1, y is 3 (original index 1)

Let us process Query 1 (x is 4, y is 1):
We look for items with `nums1` greater than or equal to 4. 
We find Item 1 (4, 2, 6). 
Our stack is empty, so we push Item 1. Stack holds the pair (nums2 is 2, sum is 6).
Now we need to answer the query. We search our stack for a `nums2` greater than or equal to 1. The pair (2, 6) satisfies this. The max sum is 6. Answer for original index 0 is 6.

Let us process Query 2 (x is 2, y is 5):
We look for more items with `nums1` greater than or equal to 2.
We check Item 2 (3, 4, 7). Its `nums1` is 3, which is valid.
We compare Item 2 to the top of our stack (2, 6). 
Item 2 has a sum of 7. The stack top has a sum of 6. Since 7 is greater than 6, Item 2 dominates the stack top! We pop (2, 6) out entirely.
We push Item 2. Stack now holds (4, 7).

We check Item 3 (2, 5, 7). Its `nums1` is 2, which is also valid.
We compare Item 3 to the top of our stack (4, 7).
Item 3 has a sum of 7. The stack top has a sum of 7. Since they are equal, the new item dominates! We pop (4, 7) out entirely.
We push Item 3. Stack now holds (5, 7).

Now we answer the query. We search our stack for a `nums2` greater than or equal to 5. The pair (5, 7) satisfies this perfectly. The max sum is 7. Answer for original index 2 is 7.

Let us process Query 3 (x is 1, y is 3):
We look for more items with `nums1` greater than or equal to 1.
We check Item 4 (1, 9, 10). Its `nums1` is 1, which is valid.
We compare Item 4 to the top of our stack (5, 7).
Item 4 has a sum of 10. The stack top has a sum of 7. Since 10 is greater than 7, Item 4 dominates! We pop (5, 7) out.
We push Item 4. Stack now holds (9, 10).

Now we answer the query. We search our stack for a `nums2` greater than or equal to 3. The pair (9, 10) satisfies this. The max sum is 10. Answer for original index 1 is 10.

Our final answer array is [6, 10, 7]. This matches the required output perfectly!

## Searching the Stack Efficiently

Because our stack guarantees that the `nums2` values are strictly increasing from bottom to top, we do not need to check every element in the stack to find the one we need. 

We can use the classic binary search algorithm to instantly pinpoint the exact location we want. In C++, the `lower_bound` function performs this binary search for us. We just ask it to find the first element in our stack where the `nums2` value is greater than or equal to our query's `y` value. 

Since the sum values are strictly decreasing from bottom to top, the very first valid element we find using binary search is absolutely guaranteed to have the largest possible sum out of all the remaining valid elements above it! We just grab its sum, store it in our answers array, and move forward.

## The C++ Implementation

Here is the complete, beautifully structured C++ code that brings all of these concepts together into a clean, functioning system.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int n1;
    int n2;
    int sum;
    
    bool operator<(const Item& other) const {
        return n1 > other.n1; 
    }
};

struct Query {
    int x;
    int y;
    int id;
    
    bool operator<(const Query& other) const {
        return x > other.x; 
    }
};

class Solution {
public:
    vector<int> maximumSumQueries(vector<int>& nums1, vector<int>& nums2, vector<vector<int>>& queries) {
        int n = nums1.size();
        vector<Item> items(n);
        
        for (int i = 0; i < n; ++i) {
            items[i] = {nums1[i], nums2[i], nums1[i] + nums2[i]};
        }
        
        sort(items.begin(), items.end());

        int q = queries.size();
        vector<Query> Q(q);
        
        for (int i = 0; i < q; ++i) {
            Q[i] = {queries[i][0], queries[i][1], i};
        }
        
        sort(Q.begin(), Q.end());

        vector<int> ans(q, -1);
        vector<pair<int, int>> st; 

        int j = 0; 

        for (int i = 0; i < q; ++i) {
            int current_x = Q[i].x;
            int current_y = Q[i].y;
            int original_id = Q[i].id;

            while (j < n && items[j].n1 >= current_x) {
                int current_n2 = items[j].n2;
                int current_sum = items[j].sum;

                while (!st.empty() && st.back().second <= current_sum) {
                    st.pop_back();
                }

                if (st.empty() || st.back().first < current_n2) {
                    st.push_back({current_n2, current_sum});
                }
                j++;
            }

            auto it = lower_bound(st.begin(), st.end(), make_pair(current_y, 0),
                [](const pair<int, int>& a, const pair<int, int>& b) {
                    return a.first < b.first;
                });

            if (it != st.end()) {
                ans[original_id] = (*it).second;
            }
        }

        return ans;
    }
};

```

## Wrapping Up Complexity Analysis

It is always important to evaluate the performance of our logic to understand exactly why it easily passes the strict judging limits.

For the Time Complexity:
We start by sorting the `items` array. Sorting an array of length N takes O(N log N) time.
Then we sort the `queries` array. Sorting an array of length Q takes O(Q log Q) time.
During our main processing loop, every single item from our arrays is pushed into the monotonic stack a maximum of one single time. It is also popped from the stack a maximum of one single time. This entire stack maintenance process takes a completely flat O(N) time overall.
Finally, for each of the Q queries, we perform a binary search over the stack. Since the stack size cannot exceed N, each binary search takes O(log N) time, resulting in O(Q log N) for all queries combined.
Adding all these pieces together, our final time complexity is strictly bounded by O((N + Q) log(N + Q)). This is incredibly fast and will process the absolute maximum constraints in a fraction of a second.

For the Space Complexity:
We create an `items` array that stores N elements.
We create a `Q` array that stores Q elements.
Our monotonic stack stores at most N elements.
Our answers array stores Q elements.
Therefore, our space complexity scales linearly with the input sizes, resulting in O(N + Q) auxiliary space. This is highly efficient and perfectly acceptable for memory limits.

I really hope this detailed walkthrough shed some light on the incredible power of offline query processing combined with monotonic stacks. It is such a rewarding feeling when you manage to strip away all the useless data and find exactly what you are looking for. Happy coding everyone!
