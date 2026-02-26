# Mastering String Trades and Range Queries A Deep Dive into Maximizing Active Sections [#3501]

**By a fellow coder**

Hello everyone! Today I want to share a journey through a challenging problem that recently stumped me. It is a story of initial failure, overcomplicating things, and finally arriving at a clean and robust solution. If you have ever felt frustrated by hard string manipulation problems involving range queries, this post is for you. We are going to dissect the problem "Maximize Active Section with Trade II" and look at how to solve it effectively using C++.

### The Challenge Before Us

Let us start by understanding what we are up against. We are given a binary string consisting of zeros and ones. In this world, a **1** represents an active section and a **0** represents an inactive section. We are allowed to perform at most one **trade** operation to maximize the total number of active sections.

But what exactly is a trade? It is a two step process involving a swap of sorts.
First you remove a contiguous block of ones that is strictly surrounded by zeros. You turn these ones into zeros.
Second you take that "removed" potential and apply it elsewhere. Specifically you find a block of zeros that is strictly surrounded by ones and fill it with ones.

However there is a catch. The problem provides a trade definition that implies a conservation of "potential" but simplifies into a net gain formula. When you remove a block of ones surrounded by zeros, you essentially merge the two surrounding zero blocks into one massive zero block. This new merged block is now surrounded by the ones that used to surround the original two zero groups. Consequently you can fill this entire new gap with ones.

We are also given a list of queries. Each query specifies a substring range. We need to calculate the maximum possible active sections for the entire string if we were to perform the optimal trade *strictly inside* that substring.

### The Mathematics of the Trade

Let us simplify the trade logic without using complex equations.
Imagine the string looks like this `... 111 000 11 0000 111 ...`
Here we have a block of two ones `11` surrounded by `000` on the left and `0000` on the right.
If we choose to "remove" this block of `11`, it becomes `00`.
Now the sequence looks like `... 111 000 00 0000 111 ...` which is just `... 111 000000000 111 ...`
Suddenly we have a huge block of nine zeros surrounded by ones. The second part of the trade allows us to fill this gap.
So `000000000` becomes `111111111`.

**The Net Result**
We lost the original block of ones (length 2).
We gained a new block of ones equal to the sum of the left zero block (length 3), the middle ones (length 2), and the right zero block (length 4).
The total increase in active sections is simply the sum of the lengths of the two surrounding zero blocks.

**Gain = Length of Left Zero Block + Length of Right Zero Block**

Our goal for each query is to find a block of ones inside the range such that its immediate left and right zero neighbors have the largest combined length.

### Why I Failed Initially

I want to be transparent about my first few attempts because failure is the best teacher.
When I first saw "Range Queries" and "Substrings", my mind immediately jumped to a **Segment Tree**. I thought I could build a tree where each node maintained complex states about its prefix and suffix. I tried to track:

* Length of ones at the start.
* Length of zeros at the start.
* Length of ones at the end.
* Length of zeros at the end.
* Zero blocks immediately adjacent to the suffix ones.

I tried to merge these nodes by writing complex logic to detect if a "0-1-0" pattern was formed across the boundary of two nodes.
**This was a trap.**
The logic became a nightmare of if statements. Did the left child end in one? Did the right child start in one? Did the left child have zeros before those ones? What if the block of ones was entirely inside the right child but touched the boundary?
The edge cases were endless. I kept getting Wrong Answers on small tricky cases like `0100` or `110010`. The recursive merging logic was just too fragile to handle the strict "surrounded by" condition correctly, especially when the query range sliced through the middle of blocks.

### The Shift in Perspective

The solution became clear when I stopped looking at the problem as "merging substrings" and started looking at it as "selecting objects".
Instead of viewing the string as a sequence of characters, let us view it as a sequence of **Blocks**.
The string `1110011110001` is really just:

* Block A: 1s (len 3)
* Block B: 0s (len 2)
* Block C: 1s (len 4)
* Block D: 0s (len 3)
* Block E: 1s (len 1)

A trade can only happen involving Block C if Block B and Block D exist. The gain would be `Length(B) + Length(D)`.
This means we can precompute the value of every possible trade!
A trade is defined by the gap between two zero blocks. If we identify all the zero blocks in the string, any two adjacent zero blocks (separated by some ones) represent a potential trade.
The value of that trade is simply `Size(ZeroBlock i) + Size(ZeroBlock i+1)`.

We can calculate these values once. Then the problem reduces to:
**"Find the maximum value in this precomputed array within the valid range."**
This is a classic Range Maximum Query problem, which is much easier to solve than the dynamic merging mess I attempted earlier.

### The Detailed Solution Walkthrough

Let us break down the provided solution which uses this "Block View" approach efficiently.

#### Step 1 Preprocessing Block Boundaries

To navigate the string efficiently we need to know where every block of identical characters begins and ends. We use two auxiliary arrays `le` and `ri`.

* `le[i]` stores the index of the **leftmost** character of the continuous block containing `s[i]`.
* `ri[i]` stores the index of the **rightmost** character of the continuous block containing `s[i]`.

For example if `s = "001110"`:
At index 2 (the first '1'), `le[2]` would be 2 and `ri[2]` would be 4.
This allows us to jump across blocks in O(1) time.

```cpp
// Building the Left Extension array
vector<int> le(n, 0), ri(n, n - 1);
for(int i = 1; i < n; i++) {
    if(s[i] == s[i-1]) le[i] = le[i-1];
    else le[i] = i;
}
// Building the Right Extension array
for(int i = n - 2; i >= 0; i--) {
    if(s[i] == s[i+1]) ri[i] = ri[i+1];
    else ri[i] = i;
}

```

#### Step 2 Identifying Zero Blocks and Trade Values

Next we scan the string to find all the Zero Blocks.
We store their starting positions in a list called `pos` and their lengths in a list called `vals`.
Important optimization: We only care about the trade value. A trade effectively merges two adjacent zero blocks.
If we have Zero Block at index `i` with length `L1` and the next Zero Block at index `j` with length `L2`, the trade utilizing the ones between them yields a gain of `L1 + L2`.

The code cleverly does this:

1. Extracts all zero blocks into `vals`.
2. Iterates through `vals` and updates `vals[i]` to be `vals[i] + vals[i+1]`.
Now `vals[i]` holds the gain of the trade corresponding to the gap between the i-th and (i+1)-th zero block.

#### Step 3 The Segment Tree

Now that we have a static array `vals` representing trade gains, we build a Segment Tree over it.
This tree supports one operation `query(l, r)` which returns the maximum value in the range `[l, r]`.
Since the values are static we could even use a Sparse Table, but a Segment Tree is very intuitive here.
The node structure is simple:

* `mi` and `ma` store the range of indices this node covers (from the `pos` array).
* `best` stores the maximum trade value in this range.

#### Step 4 Answering Queries and Handling Edges

This is the most critical part where accuracy is key.
A query gives us a range `[L, R]`.
However we cannot just blindly query our Segment Tree.
Why? Because the query range might cut halfway through a zero block or a one block.
Also the problem states the trade must happen *inside* the substring.
This means we need to find the **first fully contained zero block** and the **last fully contained zero block** within `[L, R]`.

1. **Adjust Boundaries**: If `L` starts on a '1', we move `L` forward to the start of the next block. If `R` ends on a '1', we move `R` backward to the end of the previous block.
2. **Valid Trade Check**: We use a helper `canApply` to see if `L` and `R` are still valid indices and if they point to different blocks. If we compressed our range so much that `L > R`, no trade is possible.
3. **The "Middle" Query**: We use the Segment Tree to find the best trade between the first valid zero block and the last valid zero block.
4. **The "Corner" Cases**:
There is a subtle case the Segment Tree might miss or handle awkwardly due to indexing mapping.
The optimal trade might involve the very first zero block in the range (merging with the second) or the very last zero block (merging with the second to last).
The solution manually checks `queryLeftMost` and `queryRightMost`.
* `queryLeftMost` calculates the specific gain if we trade the block of ones immediately following the first zero block.
* `queryRightMost` calculates the specific gain if we trade the block of ones immediately preceding the last zero block.
This explicit check ensures we do not miss the boundary interactions that purely index-based tree queries might obscure.



### The Final Code Block

Here is the complete solution. I have kept the structure identical to the successful submission to ensure you can copy and study it directly.

```cpp
struct Seg {
    int mi, ma, best;
    Seg *left, *right;
    // Constructor builds the tree recursively
    Seg(vector<int>& A, int l, int r) : mi(A[l]), ma(A[r]), best(0), left(nullptr), right(nullptr) {
        if(l ^ r) { // If l != r
            int m = l + (r - l) / 2;
            left = new Seg(A, l, m);
            right = new Seg(A, m + 1, r);
        }
    }
    // Update populates the tree with the precomputed trade values
    void update(int n, int x) {
        if(mi <= n and n <= ma) {
            best = max(best, x);
            if(left) left->update(n, x);
            if(right) right->update(n, x);
        }
    }
    // Query returns the max value in range
    int query(int l, int r) {
        if(l <= mi and ma <= r) return best;
        if(l > ma or r < mi) return 0;
        return max(left->query(l, r), right->query(l, r));
    }
};

class Solution {
public:
    vector<int> maxActiveSectionsAfterTrade(string s, vector<vector<int>>& queries) {
        int n = s.length();
        
        // Step 1: Precompute Block Boundaries
        vector<int> le(n, 0), ri(n, n - 1);
        for(int i = 1; i < n; i++) {
            if(s[i] == s[i-1]) le[i] = le[i-1];
            else le[i] = i;
        }
        for(int i = n - 2; i >= 0; i--) {
            if(s[i] == s[i+1]) ri[i] = ri[i+1];
            else ri[i] = i;
        }

        // Helper to check if indices are valid and distinct blocks
        auto canApply = [&](int l, int r) {
            if(l >= n or r < 0 or l >= r) return false;
            return 0 <= l and l < n and 0 <= r and r < n and le[l] != le[r];
        };

        // Helper to calculate trade gain near the left boundary
        auto queryLeftMost = [&](int l, int r) {
            int res = ri[l] - l + 1; // Length of first 0-block
            l = ri[ri[l] + 1] + 1;   // Jump to next 0-block
            res += min(r, ri[l]) - l + 1; // Add length of next 0-block
            return res;
        };

        // Helper to calculate trade gain near the right boundary
        auto queryRightMost = [&](int l, int r) {
            int res = r - le[r] + 1; // Length of last 0-block
            r = le[le[r] - 1] - 1;   // Jump to prev 0-block
            res += r - max(l, le[r]) + 1; // Add length of prev 0-block
            return res;
        };

        // Step 2: Identify all Zero Blocks
        vector<int> pos, vals;
        for(int i = 0; i < n; i++) {
            if(i and s[i] == s[i-1]) continue; // Skip middle of blocks
            if(s[i] == '1') continue;          // Skip 1-blocks
            pos.push_back(i);                  // Store start index
            vals.push_back(ri[i] - le[i] + 1); // Store length
        }

        int cnt = count(begin(s), end(s), '1');
        
        // Edge case: Not enough zero blocks to trade
        if(pos.size() <= 1) return vector<int>(queries.size(), cnt);

        // Step 3: Combine adjacent zero blocks into trade values
        for(int i = 0; i < vals.size() - 1; i++) vals[i] += vals[i+1];
        
        // Build Segment Tree on the positions
        Seg* seg = new Seg(pos, 0, pos.size() - 1);
        for(int i = 0; i < vals.size(); i++) seg->update(pos[i], vals[i]);

        vector<int> res;
        for(auto& q : queries) {
            int l = q[0], r = q[1];
            
            // Adjust to inner 0-blocks
            if(s[l] == '1') l = ri[l] + 1;
            if(s[r] == '1') r = le[r] - 1;

            if(canApply(l, r)) {
                // Check immediate left/right boundary trades manually
                int now = max(queryLeftMost(l, r), queryRightMost(l, r));
                
                // Narrow scope to strictly inside
                l = ri[ri[l] + 1] + 1;
                r = le[le[r] - 1] - 1;
                
                // Query tree for the middle section
                if(l <= r and le[l] != le[r]) {
                    now = max(now, seg->query(l, le[r] - 1));
                }
                res.push_back(cnt + now);
            } else {
                res.push_back(cnt);
            }
        }
        return res;
    }
};

```

### Closing Thoughts

This problem is a beautiful example of how changing your perspective on data can simplify a solution. By moving from "manipulating strings" to "evaluating block potentials", we transformed a messy recursive logic problem into a clean precomputation and query problem.
Remember that for range query problems, handling the boundaries is often where the bugs hide. Explicitly checking the leftmost and rightmost possibilities (as done with `queryLeftMost` and `queryRightMost`) is often safer than trying to force a general tree query to handle partial edge overlaps.

I hope this breakdown helps you in your coding journey. Keep practicing and do not get discouraged by failures. They are just the stepping stones to the correct solution.

Happy Coding!
