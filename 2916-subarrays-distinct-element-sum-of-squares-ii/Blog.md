# Mastering the Sum of Squares of Distinct Subarray Elements A Thorough Guide [#2916]

Hello everyone! Welcome back to another algorithm exploration. I recently came across a fascinating array challenge that requires a clever mix of mathematical observation and advanced data structures. I want to share my findings and walk you through a viable method to solve it. My goal is to break down the complex parts into easily digestible pieces so that we can all learn and grow together. Let us dive right into the details!

## Understanding the Challenge 

At its core, we are given an array of integers. Our task is to examine every possible contiguous block of elements within this array. These contiguous blocks are called subarrays. For each subarray, we need to count how many distinct or unique numbers exist inside it. Once we have that count, we square it. Finally, we add up all these squared counts across every single valid subarray.

Because the total sum can grow incredibly large, we are asked to return the final answer modulo 1000000007.

To ensure we are all on the same page, let us clarify the terminology. A subarray is a nonempty contiguous sequence of elements within an array. The distinct count is simply the number of unique integer values present in that specific sequence.

### A Detailed Look at the First Example

Suppose our array is `[1, 2, 1]`. Let us list out all possible subarrays and find their distinct counts, along with the squares of those counts. To keep things clear, I will use one based indexing for our positions.

* Subarray starting at position 1 and ending at 1 is `[1]`. It contains just the number 1. The distinct count is 1. Squaring this gives us 1.
* Subarray starting at position 2 and ending at 2 is `[2]`. It contains just the number 2. The distinct count is 1. Squaring this gives us 1.
* Subarray starting at position 3 and ending at 3 is `[1]`. It contains just the number 1. The distinct count is 1. Squaring this gives us 1.
* Subarray starting at position 1 and ending at 2 is `[1, 2]`. It contains the numbers 1 and 2. The distinct count is 2. Squaring this gives us 4.
* Subarray starting at position 2 and ending at 3 is `[2, 1]`. It contains the numbers 2 and 1. The distinct count is 2. Squaring this gives us 4.
* Subarray starting at position 1 and ending at 3 is `[1, 2, 1]`. It contains the numbers 1, 2, and 1. The distinct numbers are 1 and 2. The distinct count is 2. Squaring this gives us 4.

If we add all these squared values together, we get 1 + 1 + 1 + 4 + 4 + 4, which equals 15. This matches the expected output perfectly.

### A Detailed Look at the Second Example

Let us examine another array `[2, 2]`.

* Subarray from position 1 to 1 is `[2]`. The distinct count is 1. The square is 1.
* Subarray from position 2 to 2 is `[2]`. The distinct count is 1. The square is 1.
* Subarray from position 1 to 2 is `[2, 2]`. The distinct count is 1 because the number 2 is the only unique value. The square is 1.

Summing these up gives us 1 + 1 + 1, which equals 3.

## The Limitations of the Brute Force Approach

When initially approaching this problem, the most intuitive method is to use nested loops. We could use an outer loop to pick the starting point of our subarray and an inner loop to pick the ending point. Inside, we could maintain a collection of seen elements to count the unique values, square that count, and add it to our running total.

While this logic is completely sound, we must look at the constraints. The array can have up to 100,000 elements. If we use a nested loop approach to visit all subarrays, we are looking at roughly 5 billion operations. In modern computing environments, we typically aim for an algorithm that completes well within a few tens of millions of operations per second. A 5 billion operation approach will undeniably result in a Time Limit Exceeded error. We need a more structured way to avoid recomputing information.

## Shifting Our Perspective The Sweep Line Concept

Instead of independently building every possible subarray from scratch, what if we process the array element by element from left to right? Imagine a boundary moving across the array. As this boundary includes a new element, we ask ourselves how this new element affects all subarrays that end at this new boundary.

Let us say our boundary just moved to include the element at position `j`. There are many subarrays ending exactly at `j`. Each one has a distinct starting position `i` ranging from 1 up to `j`. Let us define `C(i)` as the distinct count of the subarray starting at `i` and ending at the previous boundary. 

When the new element at position `j` is revealed, how does `C(i)` change? The distinct count will only increase if the new element has not appeared anywhere between the starting position `i` and our new position `j`.

This means we only need to keep track of the most recent position where we saw this specific number. Let us call that position `L`. For any starting index `i` that comes after `L`, the new element is genuinely new to that subarray. Therefore, for all subarrays starting in the range from `L + 1` up to `j`, their distinct count increases exactly by 1. For any starting index `i` that is less than or equal to `L`, the subarray already contains this number, so its distinct count remains entirely unchanged.

## The Mathematical Insight

Now we know that adding a new element simply adds 1 to the distinct count of a contiguous range of starting positions. But the problem asks for the sum of the squares of these counts. We need to figure out how adding 1 to a count affects its square.

Let us represent the current distinct count as `c`. The current square is `c * c`.
When the count increases by 1, the new count is `c + 1`.
The new square becomes `(c + 1) * (c + 1)`.
If we expand this using basic algebra, we get `c * c + 2 * c + 1`.

This formula is the crucial breakthrough! It tells us exactly how much the square changes. The new square is the old square plus two times the old count plus 1. 

If we are updating a whole range of starting positions simultaneously, the total increase in our sum of squares over that range will be two times the sum of the old counts in that range, plus the length of the range itself (since we add 1 for every single position in the range).

To manage these operations efficiently, we need a data structure capable of tracking both the sum of counts and the sum of squared counts for various ranges, while also allowing us to update ranges quickly.

## Introduction to Segment Trees

This is where the Segment Tree shines. A Segment Tree is a binary tree designed to answer range queries and apply range updates very quickly. 



In our Segment Tree, every node will represent a specific segment or range of starting indices. For its assigned segment, each node will keep track of two important totals. First, it will store the sum of all distinct counts for subarrays starting within this segment. Second, it will store the sum of the squares of all distinct counts for subarrays starting within this segment. 

When we combine two child nodes to figure out the totals for their parent node, we simply add their respective totals together. The left child handles the first half of the segment, and the right child handles the second half. This hierarchical structure allows us to query and update large segments by interacting with just a few nodes near the top of the tree.

## The Magic of Lazy Propagation

Updating large segments element by element defeats the purpose of the tree. If we want to add 1 to a large range of indices, we cannot afford to travel all the way down to the bottom leaves of the tree for every single index. This would be far too slow.



To solve this, we introduce a concept called Lazy Propagation. We add a third variable to our node called `lazy`. When we need to add 1 to a large range perfectly covered by a node, we update the sums for that node immediately using our mathematical formula, but we do not go any further down. Instead, we record the pending addition in the `lazy` variable of that node. 

This lazy value acts as a sticky note reminding us that the children of this node also need this addition, but we will only pass the message down when we actually need to visit those children later. This deferred updating mechanism keeps our operations incredibly fast.

## The Length Precalculation Strategy

One common pain point when writing segment trees is constantly needing to calculate the length of the segment a node covers. Normally, one might calculate this by taking the right boundary, subtracting the left boundary, and adding one. However, subtraction can sometimes lead to logic bugs or off by one errors if not managed carefully.

To make our implementation exceptionally clean and entirely avoid subtracting variables, we can use a structural trick. We will add a fourth variable to our node called `len`. When we initially build the tree, we can determine the length recursively. If a node is a leaf representing a single index, its length is simply 1. For any parent node, its length is simply the length of its left child plus the length of its right child. 

By precalculating these lengths once during the setup phase, our update formulas later on will become beautifully simple. When we apply a lazy value `v` to a node, the new sum of squares becomes the old sum of squares plus two times `v` times the old sum of counts, plus the precalculated length multiplied by `v * v`. We have completely bypassed any need for dynamic range subtraction!

## Modulo Arithmetic Basics

Because the problem constraints mention the answer can be huge, we must apply a modulo operation after every addition and multiplication. We will use the constant 1000000007. 

When we add two large numbers, we add them together and then apply the modulo. When we multiply two large numbers, we must ensure we are using long integers to prevent overflow before applying the modulo. Distributing the modulo correctly at every step within our segment tree ensures our numbers always stay within safe limits while preserving mathematical accuracy.

## Putting It All Together The Algorithm Steps

Let us consolidate our entire plan into a clear sequence of steps.

* Step 1. We determine the size of our input array. Let us call this `n`.
* Step 2. We initialize our Segment Tree to handle `n` elements. We build the tree to precalculate the lengths for all nodes.
* Step 3. We create an array called `last` to keep track of the most recent position we saw each number. We will initialize this with zeros.
* Step 4. We create a one indexed copy of our input array to keep our index tracking straightforward and positive.
* Step 5. We initialize a variable to hold our final answer.
* Step 6. We begin a loop from position 1 up to `n`. 
* Step 7. For the current number, we look up its previous occurrence in our `last` array. We add 1 to this value to find the start of our update range.
* Step 8. We tell our Segment Tree to add 1 to the distinct counts for all indices starting from that calculated start point up to our current position.
* Step 9. After the tree is updated, the root node of our tree holds the sum of squares for all subarrays ending at our current position. We add this root value to our final answer variable.
* Step 10. We update the `last` array to record that we have just seen the current number at the current position.
* Step 11. Once the loop finishes, we return the final accumulated answer.

## The Complete C++ Implementation

Here is the code putting all our discussed strategies into practice. I have carefully structured it to be straightforward and readable.

```cpp
#include <vector>

using namespace std;

class Solution {
public:
    struct Node {
        long long sum1;
        long long sum2;
        long long lazy;
        int len;
    };

    vector<Node> tree;
    long long MOD = 1000000007;

    void build(int node, int l, int r) {
        if (l == r) {
            tree[node].len = 1;
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid);
        build(2 * node + 1, mid + 1, r);
        tree[node].len = tree[2 * node].len + tree[2 * node + 1].len;
    }

    void apply(int node, long long v) {
        long long v2 = (v * v) % MOD;
        long long v_sum1 = (v * tree[node].sum1) % MOD;
        
        long long added_squares = (tree[node].len * v2) % MOD;
        long long mixed_term = (2 * v_sum1) % MOD;
        
        tree[node].sum2 = (tree[node].sum2 + mixed_term + added_squares) % MOD;
        
        long long added_sums = (tree[node].len * v) % MOD;
        tree[node].sum1 = (tree[node].sum1 + added_sums) % MOD;
        
        tree[node].lazy = (tree[node].lazy + v) % MOD;
    }

    void pushdown(int node) {
        if (tree[node].lazy != 0) {
            apply(2 * node, tree[node].lazy);
            apply(2 * node + 1, tree[node].lazy);
            tree[node].lazy = 0;
        }
    }

    void pushup(int node) {
        tree[node].sum1 = (tree[2 * node].sum1 + tree[2 * node + 1].sum1) % MOD;
        tree[node].sum2 = (tree[2 * node].sum2 + tree[2 * node + 1].sum2) % MOD;
    }

    void update(int node, int l, int r, int ql, int qr, long long v) {
        if (ql <= l && r <= qr) {
            apply(node, v);
            return;
        }
        pushdown(node);
        int mid = (l + r) / 2;
        if (ql <= mid) {
            update(2 * node, l, mid, ql, qr, v);
        }
        if (qr > mid) {
            update(2 * node + 1, mid + 1, r, ql, qr, v);
        }
        pushup(node);
    }

    int sumCounts(vector<int>& nums) {
        int n = nums.size();
        tree.assign(4 * n + 5, {0, 0, 0, 0});
        build(1, 1, n);

        vector<int> last(100005, 0);
        vector<int> A(n + 1);
        for(int i = 0; i < n; ++i) {
            A[i + 1] = nums[i];
        }

        long long ans = 0;

        for (int j = 1; j <= n; ++j) {
            int L = last[A[j]] + 1;
            int R = j;
            
            update(1, 1, n, L, R, 1);
            
            ans = (ans + tree[1].sum2) % MOD;
            
            last[A[j]] = j;
        }

        return ans;
    }
};

```

## Complexity Breakdown

Understanding the resource utilization of our solution is always an important final step.

**Time Complexity:** Processing our array requires visiting each of the `N` elements exactly once. For every single element, we calculate the previously seen position and request an update from our segment tree. Traversing down to the correct nodes and returning back up the tree takes a logarithmic amount of time relative to the size of the array. Therefore, the total time operation scales proportionally to `Order of N log N`. This comfortably processes 100,000 items well within time limits, avoiding any performance issues.

**Space Complexity:** We utilize additional memory structurally. Our segment tree utilizes memory strictly proportional to four times the array size. We also keep a secondary tracking array for the previously seen element positions. Since the problem limits inform us that the integer values within the array will not exceed 100,000, our tracking array only needs a fixed size of 100,005 spaces. Altogether, the extra memory required grows in direct proportion to the size of the input, giving us a memory complexity of `Order of N`. This is a very modest memory footprint.

## Final Thoughts

I hope you find this breakdown helpful for your own learning journey! We transformed a seemingly unapproachable nested loop problem into an elegant sweep line concept, powered seamlessly by a Segment Tree. By taking the time to write out the mathematical expansion, we discovered a direct path to our solution. Avoiding negative calculations and precalculating lengths kept our logic exceptionally clean and robust.

Would you like me to explain any specific part of the segment tree node merging logic in more detail, or perhaps we could explore another array based counting challenge together next? Let me know!
