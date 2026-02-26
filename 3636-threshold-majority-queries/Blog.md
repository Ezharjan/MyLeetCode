# A Deep Dive Into Threshold Majority Queries And Offline Processing [#3636]

Hello everyone! Today I want to share a comprehensive walkthrough of a fascinating array problem. I recently spent some time studying this challenge, and I found the combination of techniques required to solve it to be incredibly instructive. Rather than claiming this is the absolute best way, I simply want to offer a solid, reliable method that helped me understand the problem deeply. I hope this detailed guide helps you on your own learning journey!



### Understanding the Problem

Let us first break down what the problem is asking us to do. We are given an array of integers, and a list of queries. Every single query provides three pieces of information:
* A starting index for our subarray.
* An ending index for our subarray.
* A specific frequency threshold.

For each query, we need to look strictly within the boundaries of that subarray. Our goal is to find an element that appears at least as many times as the given threshold. 

But there is a catch! There might be multiple elements that meet this threshold requirement. The problem specifies two very important rules for breaking ties:
1. We must select the element that has the absolutely highest frequency within that subarray.
2. If there are two or more elements tied for that highest frequency, we must pick the element with the smallest actual numeric value.

If no element in the subarray meets the threshold, we must return negative 1 for that specific query.

To make this concrete, imagine an array like `[1, 1, 2, 2, 1, 1]`. 
If a query asks us to look at the entire array with a threshold of 4, we count the frequencies. The number 1 appears four times, and the number 2 appears two times. Since 1 meets the threshold of 4, the answer is 1.
If another query asks us to look at the subarray `[1, 1, 2, 2]` with a threshold of 3, we count again. The number 1 appears twice, and the number 2 appears twice. Neither reaches the threshold of 3, so the answer is negative 1.

### The Pitfalls of a Direct Approach

When we first look at this, the most intuitive approach is to just run a loop for every query. We could go to the start index, loop to the end index, count up all the numbers using a hash map, and then find the one that fits our rules. 

However, we have to look at the constraints. The array can have up to 10000 elements, and we can have up to 50000 queries. If every query asks us to look at the entire array, we would be doing 10000 operations 50000 times. That is 500,000,000 operations! Most programming environments will cut us off for taking too much time. We need a strategy that reuses information between queries.

### Enter the Mo Algorithm

Because the problem does not ask us to change or update any values in the original array, we can process the queries in any order we want. We just need to remember to put the answers back in the correct original order before returning them. This is called offline processing.



A fantastic technique for this exact scenario is the Mo Algorithm. It is a way to organize our queries to minimize the amount of work we do. Instead of starting from scratch for every query, we maintain a sliding window. We have a left pointer and a right pointer, and we slide them to match the boundaries of the next query, adding and removing elements one by one as the window moves.

To make this sliding process truly efficient, we sort the queries in a very clever way known as square root decomposition. 
* We divide the array into blocks of a specific size (usually the size of the array divided by the square root of the number of queries).
* We sort the queries primarily by the block in which their left boundary falls.
* For queries that share the same left block, we sort them by their right boundary. 

By sorting the queries this way, the left pointer only drifts slowly within its block, and the right pointer moves smoothly across the array. This drastically reduces the total distance our pointers have to travel, bringing our total pointer movements down to a very manageable number.

### The Need for Speed and Segment Trees

While the Mo Algorithm limits how much we move our window, we still face a major hurdle. Every time we move a pointer, we change the frequency of one element. But how do we quickly know which element has the maximum frequency overall, and how do we break ties? 

If we just use an array to store frequencies, we would have to scan the entire frequency array for every single query to find the maximum. That is still too slow. 



This is where a Segment Tree becomes incredibly useful. A Segment Tree allows us to update the frequency of a single element and then recalculate the maximum frequency for the entire collection in just a few logarithmic steps. 

### Shrinking the World with Coordinate Compression

There is a slight complication. The numbers in our array can be as large as one billion. A Segment Tree needs an underlying array to represent the leaves, and we absolutely cannot create an array with one billion empty slots. It would consume too much memory.

Thankfully, the array only contains up to 10000 elements. Even if every number is totally unique, there are at most 10000 distinct values. We can use Coordinate Compression! 

Coordinate compression is a way to map large, sparse numbers to small, dense numbers. 
1. We take a copy of our original array and sort it.
2. We remove all duplicate values.
3. For every number in our original array, we find its position in our sorted unique list. 

Now, instead of dealing with numbers up to one billion, we are dealing with ranks from 0 to 9999. This perfectly fits into a small Segment Tree! We just need to remember to map the rank back to the original large number when we record our final answer.



### The Magic of Bit Packing

We have one final challenge. The Segment Tree needs to keep track of the maximum frequency. But remember our tie breaking rule? If frequencies are equal, we need the smallest original value. Writing custom logic inside the Segment Tree to handle both variables simultaneously can be messy and error prone.

We can solve this brilliantly by combining the two pieces of information into a single large variable! We can use a 64 bit integer. 
* We take the frequency and shift it left by 32 bits, placing it safely in the upper half of the integer.
* In the lower 32 bits, we want to place our value. But we want smaller values to win ties. To achieve this, we define a large constant Offset (like two billion). We subtract our original array value from this Offset. 

By doing this, a smaller original value results in a larger lower half. 
When we update our Segment Tree, we simply use the standard maximum function. The computer will naturally compare the upper 32 bits first. If one frequency is higher, it wins. If the frequencies are exactly identical, it checks the lower 32 bits. Because of our Offset subtraction, the item with the smaller original value will appear larger to the computer and win the tie! 

This trick keeps our Segment Tree update loop incredibly simple and fast.

### Putting It All Together step by step

Let us review the entire architecture of our solution:

1. **Pre computation:** We collect all the unique values from the input array and map them to small index values.
2. **Tree Setup:** We build a Segment Tree where the leaves represent these unique values. We initialize the leaves with their respective Offset values, keeping the frequencies at zero.
3. **Query Organization:** We store the original index of each query, and then sort them using the Mo Algorithm block sorting rules.
4. **Sliding the Window:** We iterate through our sorted queries. For each query, we expand or shrink our left and right pointers to match the query boundaries. 
5. **Updating Frequencies:** Whenever a pointer moves, we update the frequency of that element in our Segment Tree. The Segment Tree bubbles up the new combined maximum integer to the root.
6. **Extracting the Answer:** Once the window matches the query, we look at the root of the Segment Tree. We split the 64 bit integer back into its two parts. We check if the frequency part is greater than or equal to the query threshold. If it is, we reconstruct the original value from the lower half and save it. If not, we save negative 1.
7. **Final Output:** After processing all queries, we return our answers array.

### Complexity Breakdown

* **Time Complexity:** Sorting the queries takes `O(Q * log Q)`. Moving the pointers takes `O(N * sqrt(Q))` total steps. Each step updates the Segment Tree, which takes `O(log U)` where U is the number of unique elements. Therefore, the core time complexity is bounded by `O(N * sqrt(Q) * log U)`. This comfortably passes the execution time limits.
* **Space Complexity:** We need space for the Segment Tree `O(U)`, the sorted queries `O(Q)`, and the compressed arrays `O(N)`. This results in a space complexity of `O(N + Q)`, which is very memory efficient.

### The Code Implementation

Here is the C++ implementation combining all these concepts. I have added comments to help map the code back to the concepts we just discussed.

```cpp
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>

using namespace std;

// We use an offset larger than the maximum possible value (10^9)
const uint64_t OFFSET = 2000000000ULL;
// This represents a frequency of 1, shifted into the upper 32 bits
const uint64_t ONE_F = 1ULL << 32;

// Structure to hold our queries for offline processing
struct Query {
    int l, r, threshold, id;
};

class Solution {
    // Segment tree array. 32768 is large enough for 10000 unique elements.
    uint64_t tree[32768];
    int base;

    // Function to update the frequency of a specific element in the tree
    inline void update(int idx, int delta) {
        int i = base + idx;
        if (delta == 1) {
            tree[i] += ONE_F; // Increase frequency
        } else {
            tree[i] -= ONE_F; // Decrease frequency
        }
        
        // Recalculate the maximums up to the root
        for (i >>= 1; i >= 1; i >>= 1) {
            tree[i] = max(tree[i << 1], tree[(i << 1) | 1]);
        }
    }

public:
    vector<int> subarrayMajority(vector<int>& nums, vector<vector<int>>& queries) {
        int n = nums.size();
        int q_len = queries.size();
        
        // Step 1: Coordinate Compression
        vector<int> sorted_nums = nums;
        sort(sorted_nums.begin(), sorted_nums.end());
        sorted_nums.erase(unique(sorted_nums.begin(), sorted_nums.end()), sorted_nums.end());
        int U = sorted_nums.size();

        // Find the nearest power of two for the tree base
        base = 2;
        while (base < U) base *= 2;

        // Step 2: Initialize Segment Tree
        for (int i = 0; i < 2 * base; ++i) {
            tree[i] = 0;
        }
        // Place the offset values into the leaves
        for (int i = 0; i < U; ++i) {
            tree[base + i] = OFFSET - sorted_nums[i];
        }
        // Build the initial tree
        for (int i = base - 1; i >= 1; --i) {
            tree[i] = max(tree[i << 1], tree[(i << 1) | 1]);
        }

        // Map original array to compressed indices
        vector<int> compressed_nums(n);
        for (int i = 0; i < n; ++i) {
            compressed_nums[i] = lower_bound(sorted_nums.begin(), sorted_nums.end(), nums[i]) - sorted_nums.begin();
        }

        // Step 3: Query Organization (Mo Algorithm)
        int B = max(1, (int)(n / sqrt(q_len)));

        vector<Query> Q(q_len);
        for (int i = 0; i < q_len; ++i) {
            Q[i] = {queries[i][0], queries[i][1], queries[i][2], i};
        }

        // Sort queries by block, then by right endpoint
        sort(Q.begin(), Q.end(), [B](const Query& q1, const Query& q2) {
            int b1 = q1.l / B;
            int b2 = q2.l / B;
            if (b1 != b2) return b1 < b2;
            // Alternating direction for even better pointer optimization
            return (b1 & 1) ? q1.r < q2.r : q1.r > q2.r;
        });

        vector<int> ans(q_len);
        int L = 0, R = -1;

        // Step 4: Process Queries
        for (const auto& q : Q) {
            // Expand the window
            while (L > q.l) update(compressed_nums[--L], 1);
            while (R < q.r) update(compressed_nums[++R], 1);
            
            // Contract the window
            while (L < q.l) update(compressed_nums[L++], -1);
            while (R > q.r) update(compressed_nums[R--], -1);

            // Step 5: Extract the Answer
            uint64_t root = tree[1];
            int max_f = root >> 32;
            int min_v = OFFSET - (root & 0xFFFFFFFFULL);

            if (max_f >= q.threshold) {
                ans[q.id] = min_v;
            } else {
                ans[q.id] = -1;
            }
        }

        return ans;
    }
};

```

### Conclusion

I hope you found this detailed walkthrough helpful! Breaking down complex problems into modular techniques like compression, tree structures, and offline processing is a great way to build up your algorithmic toolbox. Feel free to try implementing this yourself and adjusting the bit manipulation logic to see how it affects the outcome. Happy coding everyone!
