# A Friendly Guide to Dividing Arrays and Minimizing Total Costs Together [#3500]

Hello everyone! Welcome to another algorithmic discussion. Today I want to share my thoughts and a helpful approach for a very interesting array division puzzle. Solving these kinds of problems can sometimes feel overwhelming, but I have found that breaking them down step by step makes the process much more enjoyable and manageable. I am excited to share a method that worked well for me, and I hope it helps you too!

### Understanding the Challenge

The core of our puzzle asks us to take two lists of numbers of the same length, which we will call `nums` and `cost`, along with a special multiplier number `k`. Our goal is to split these lists into continuous blocks or subarrays. 

For every block we create, let us say the block starts at index `l` and ends at index `r`, we have to calculate a specific score. The score for a block is calculated by adding up all the numbers in `nums` from `l` to `r`, plus `k` multiplied by the block's sequence number (one for the first block, two for the second block, and so forth). We then multiply this entire sum by the sum of all the numbers in the `cost` array from `l` to `r`. 

Finally, we want to find a way to divide the arrays such that the total sum of all these block scores is as small as possible. 

This might sound like a lot of moving parts! The dependency on the block's sequence number is particularly tricky because the score of a future block depends on how many blocks we have already created. Let us explore a way to untangle this.

### Step One Making Things Simpler with Prefix Sums

Whenever we need to repeatedly calculate the sum of numbers between two indices, prefix sums are our best friends. 



Let us define two new arrays:
* `P` will store the running totals of the `nums` array.
* `C` will store the running totals of the `cost` array.

If we want the sum of `nums` from index `l` to `r`, we can simply compute `P[r] minus P[l minus 1]`. 

Let us say we have divided our arrays into several blocks. Let the end index of the previous block be `j`. This means our current block starts at `j + 1` and ends at `r`. 
Using our prefix sums, the sum of `nums` for this block is `P[r] minus P[j]`. The sum of `cost` for this block is `C[r] minus C[j]`.

### Step Two Untangling the Sequence Number

The hardest part of this puzzle is the sequence number. If we have `m` total blocks, let us imagine adding up the scores of all `m` blocks. 

If we write out the total sum algebraically and expand all the multiplications, a fascinating pattern emerges. A lot of terms start to cancel out or group together nicely. This is often called a telescoping sum.

Instead of writing out complex formulas, let me explain the result conceptually. We can completely remove the need to know the block sequence number during our block building process! 

It turns out that the total score across all blocks can be rewritten as a sum of individual transition costs. If we place a cut to end a block at index `j` and place the next cut to end the next block at index `r`, the isolated cost contribution for making this specific jump is:
`P[r] * C[r] minus P[r] * C[j] + k * C[last index] minus k * C[j]`

Notice what happened here! The sequence number variable has completely vanished. Every time we decide to transition from a previous cut `j` to a new cut `r`, we just add this isolated cost. This completely decouples our choices and makes Dynamic Programming possible.

### Step Three Structuring the Dynamic Programming

Now we can define a strategy. Let `dp[r]` be the minimum cost to perfectly divide the arrays up to the index `r`. 

To find the best `dp[r]`, we need to look back at all possible previous cut locations `j` that came before `r`. 
For a specific previous cut `j`, the total cost to reach `r` would be the cost to reach `j` successfully, which is `dp[j]`, plus the transition cost we just discovered.

So, the value for a specific choice of `j` is:
`dp[j] + P[r] * C[r] minus P[r] * C[j] + k * C[last index] minus k * C[j]`

To find `dp[r]`, we want the minimum possible value among all valid choices of `j`. We can separate the terms that depend only on `r` from the terms that depend on `j`:

`dp[r] = P[r] * C[r] + k * C[last index] + minimum of (dp[j] minus C[j] * P[r] minus k * C[j])` over all valid `j`.

### Step Four Overcoming the Speed Barrier

If we use a simple loop to calculate `dp[r]` and an inner loop to check all possible values of `j`, our program will take too many steps. For an array of size N, this takes N squared steps. We need a faster way to find that minimum value.

Look closely at the expression we are trying to minimize:
`dp[j] minus C[j] * P[r] minus k * C[j]`

We can think of this as a straight line equation, similar to the classic `y = slope * x + intercept`.
* The slope of the line is `minus C[j]`.
* The variable `x` is `P[r]`.
* The intercept of the line is `dp[j] minus k * C[j]`.



For every index `j` we process, we are essentially creating a new straight line. When we are at index `r` and want to find the minimum value, we are asking: "Out of all the lines we have created so far, which line gives the lowest y value when we plug in x equals P[r]?"

There is a wonderful data structure designed exactly for this purpose called the Li Chao Segment Tree. It helps us keep track of multiple lines and quickly find the minimum or maximum value at any given point `x` in logarithmic time.

### Step Five Implementing the Li Chao Tree

Because the values in our arrays can grow quite large, multiplying them together might exceed standard integer limits. I recommend using larger data types internally, such as `__int128_t` in C++, to prevent any unexpected overflows during the calculations. 

Additionally, the `x` values we query are the prefix sums `P[r]`. Since we know all these `x` values right from the start, we can collect them, sort them, and remove duplicates. This process, known as coordinate compression, allows us to build a Li Chao Tree over a small, manageable range of indices rather than massive numerical values.

Here is a C++ implementation incorporating all these ideas. I have tried to keep the code neat and modest.

```cpp
class Solution {
    // A simple structure to represent our line equations
    struct Line {
        long long m;
        __int128_t c;
        
        // Calculate the y value for a given x
        __int128_t get(long long x) const {
            return (__int128_t)m * x + c;
        }
    };

    // Nodes for our Segment Tree
    struct Node {
        Line line;
        int left, right;
        // Initialize with a line that has a very high intercept
        Node() : line({0, ((__int128_t)1 << 110)}), left(-1), right(-1) {}
    };

    vector<Node> tree;

    // Helper function to find the minimum of two large integers
    __int128_t min128(__int128_t a, __int128_t b) {
        return a < b ? a : b;
    }

    // Function to add a new line to our tree
    void add_line(int node, int l, int r, Line new_line, const vector<long long>& xs) {
        if (l == r) {
            if (new_line.get(xs[l]) < tree[node].line.get(xs[l])) {
                tree[node].line = new_line;
            }
            return;
        }
        
        int mid = l + (r - l) / 2;
        bool better_left = new_line.get(xs[l]) < tree[node].line.get(xs[l]);
        bool better_mid = new_line.get(xs[mid]) < tree[node].line.get(xs[mid]);
        
        if (better_mid) {
            swap(tree[node].line, new_line);
        }
        
        if (better_left != better_mid) {
            if (tree[node].left == -1) {
                tree[node].left = tree.size();
                tree.emplace_back();
            }
            add_line(tree[node].left, l, mid, new_line, xs);
        } else {
            if (tree[node].right == -1) {
                tree[node].right = tree.size();
                tree.emplace_back();
            }
            add_line(tree[node].right, mid + 1, r, new_line, xs);
        }
    }

    // Function to find the minimum y value for a specific x coordinate
    __int128_t query(int node, int l, int r, int x_idx, const vector<long long>& xs) {
        if (node == -1) return ((__int128_t)1 << 110);
        
        __int128_t res = tree[node].line.get(xs[x_idx]);
        if (l == r) return res;
        
        int mid = l + (r - l) / 2;
        if (x_idx <= mid) {
            res = min128(res, query(tree[node].left, l, mid, x_idx, xs));
        } else {
            res = min128(res, query(tree[node].right, mid + 1, r, x_idx, xs));
        }
        return res;
    }

public:
    long long minimumCost(vector<int>& nums, vector<int>& cost, int k) {
        int n = nums.size();
        vector<long long> P(n), C(n);
        
        // Build the prefix sum arrays
        P[0] = nums[0];
        C[0] = cost[0];
        for (int i = 1; i < n; ++i) {
            P[i] = P[i - 1] + nums[i];
            C[i] = C[i - 1] + cost[i];
        }

        // Coordinate compression to make our tree smaller
        vector<long long> xs = P;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        int M = xs.size();

        tree.clear();
        // Reserve space to keep memory locations stable
        tree.reserve(n * 20); 
        tree.emplace_back();

        // Add our starting base case line
        add_line(0, 0, M - 1, {0, 0}, xs);

        __int128_t current_dp = 0;
        
        for (int i = 0; i < n; ++i) {
            // Find where our current P value sits in the compressed coordinates
            int idx = lower_bound(xs.begin(), xs.end(), P[i]) - xs.begin();
            
            // Ask the tree for the lowest possible previous combination cost
            __int128_t val = query(0, 0, M - 1, idx, xs);
            
            // Calculate the total cost up to the current index
            current_dp = (__int128_t)P[i] * C[i] + (__int128_t)k * C[n - 1] + val;
            
            // If we are not at the very end, we create a new line for future queries
            if (i < n - 1) {
                Line new_line;
                new_line.m = -C[i];
                new_line.c = current_dp - (__int128_t)k * C[i];
                add_line(0, 0, M - 1, new_line, xs);
            }
        }

        // Return the final result cast back to a standard long long
        return (long long)current_dp;
    }
};

```

### Wrapping Up

I hope this breakdown was clear and helpful! By stepping back and looking at the math, we transformed a confusing sequence dependent problem into a clean line evaluation task. The Li Chao Segment tree is a beautiful tool for these exact situations, keeping our time complexity manageable and our logic sound.

If you have any questions about coordinate compression or how the tree updates itself, please feel free to experiment with the code and see how the values change. Happy coding everyone, and good luck with your continued algorithmic journeys!
