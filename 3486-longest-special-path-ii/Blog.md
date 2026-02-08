# Unlocking Tree Patterns Finding the Longest Almost Unique Path [#3486]

[Blog Post Link](https://leetcode.com/discuss/post/7564222/unlocking-tree-patterns-finding-the-long-5uv3/)

Hello fellow developers and algorithm enthusiasts!

Now I want to share a fascinating journey into solving a complex tree traversal problem that beautifully combines Depth First Search with the sliding window technique. If you have ever struggled with path problems on trees or wondered how to efficiently track constraints while traversing deep recursion stacks, this post is for you. We are going to tackle a hard problem involving "special paths" where node values must be mostly unique.

Let us dive right in and explore how to break this down into manageable pieces.

## The Problem at a Glance

Imagine you are given a tree structure. As many of you know, a tree is a connected graph with no cycles. In this specific scenario, the tree is rooted at node 0, and every node has a specific integer value associated with it. The edges connecting these nodes have weights, representing a length or distance.

Our goal is to find a "Special Path."

**What defines a Special Path?**

1. **Directionality** The path must be "downward." This means it starts at some ancestor node and moves strictly down towards a descendant node. It does not go up and down; it only goes away from the root.
2. **Uniqueness Constraint** The sequence of values along this path must be distinct, with one specific exception. You are allowed **at most one value** to appear twice. It cannot appear three times, and you cannot have two different values that both appear twice.

**What do we need to return?**

We need to return two numbers:

1. The **length** of the longest special path (the sum of edge weights).
2. The **minimum number of nodes** required to achieve that longest length.

## Understanding the Constraints through Examples

Let us visualize the rules to ensure we are on the same page.

Suppose we have a path of nodes with values: `[1, 2, 3, 4]`.

* Are all distinct? Yes.
* Is this a valid special path? Yes.

Suppose we have: `[1, 2, 1, 3]`.

* The value `1` appears twice. Everything else is distinct.
* Is this valid? Yes. This uses our "one allowance" for a repeating value.

Suppose we have: `[1, 2, 1, 3, 1]`.

* The value `1` appears three times.
* Is this valid? **No**. A value can appear at most twice.

Suppose we have: `[1, 2, 1, 3, 2]`.

* The value `1` appears twice.
* The value `2` appears twice.
* Is this valid? **No**. We are only allowed *one* value to repeat. Here we have two different repeating pairs.

Our challenge is to scan the entire tree and find the path that maximizes the total length while adhering to these strict rules.

## The Strategy Depth First Search with a Sliding Window

The brute force approach would be to look at every single node, treat it as a start point, and try to walk downwards to every possible descendant. However, with up to 50,000 nodes, that approach is far too slow (quadratic time complexity). We need something linear, something that visits each node just once or twice.

The most effective pattern here is to use **Depth First Search (DFS)** combined with a **Sliding Window**.

### Why DFS?

Since we are looking for "downward" paths, DFS is a natural fit. As we recurse from the root to a leaf, the recursion stack actually contains the exact path we are currently exploring. When we are at a node `u` at depth `D`, the recursion stack holds `root -> child -> ... -> u`.

### Why Sliding Window?

Usually, we see sliding windows on arrays (like "longest substring with no repeating characters"). Here, we treat the current path from the root to our current node as our "array."

We maintain a window defined by two pointers:

1. **Bottom** (The current node we are visiting).
2. **Top** (The ancestor node where our valid special path starts).

As we move down the tree (pushing nodes onto the stack), we expand the window at the bottom. If the new node introduces a violation (like a third duplicate or a second pair), we shrink the window from the top (move the ancestor pointer down) until the path becomes valid again.

## Designing the State

To make this work efficiently, we need to track a few pieces of information as we travel. We cannot afford to scan the whole path every time we step to a new node. We need instant lookups.

We will use the following data structures:

1. **`path_history[value]`**
Instead of just knowing *if* a value is in the path, we need to know *where* it occurred. Since a value (like 5) might appear at depth 2 and again at depth 10, we can use a list or a vector to store the depths of every occurrence of a value in the current path.
* *Note:* When we backtrack (return from a child to a parent), we must remove the current depth from this history to keep it accurate.


2. **`start_index`**
This integer represents the depth of the ancestor node where our current valid path begins. Initially, it is 0 (the root). As we encounter conflicts, this number will increase, effectively shortening the path from the top.
3. **`repeat_start_depth`**
This is a special variable to track our "one allowed pair." It stores the depth of the *first* instance of the currently repeating value.
* If no value is currently repeating, this might be -1.
* If the value `X` appears at depth 3 and depth 7, and this is our only pair, then `repeat_start_depth` would be 3.


4. **`dist_from_root`**
To quickly calculate path lengths, we can track the total distance from the root to the current node. The length of a path between `ancestor` and `current` is simply `dist_from_root[current] - dist_from_root[ancestor]`.

## The Algorithm in Detail

Let us walk through the logic step by step for a single node visit.

**Step 1 Update Path Info**
We arrive at a node `u` at depth `d`. We record its distance from the root. We also look at its value, let us call it `val`. We add `d` to `path_history[val]`.

**Step 2 Check for "Three of a Kind"**
We check how many times `val` now appears in our current path history.
If `val` appears 3 or more times, this is invalid. The valid path *must* start after the first occurrence of `val` to eliminate the triplet.

* We move `start_index` to be at least `(first occurrence of val) + 1`.
* *Crucial Check* If moving `start_index` passed our `repeat_start_depth`, it means we chopped off the old repeating pair. We must reset `repeat_start_depth` to -1 because that specific repetition is no longer inside our window.

**Step 3 Check for "Two Pairs"**
If `val` appears at least twice (meaning we just completed a pair), we need to see if there is a conflict.

* Let `p` be the depth of the previous occurrence of `val`.
* We check: Is `p` inside our current window (i.e., `p >= start_index`)?
* If yes, we have a pair of `val`.


* **The Conflict Logic** If we *already* have a different repeating pair active (indicated by `repeat_start_depth` being valid and not equal to `p`), we now have two pairs. This is forbidden.
* We must sacrifice one of them. To maximize length, we keep the one that ends later, which effectively means we must remove the one that starts earlier to make the window valid.
* If the old pair started at `repeat_start_depth` and the new pair starts at `p`:
* If `repeat_start_depth < p`, the old pair is "higher" up the tree. We move `start_index` to `repeat_start_depth + 1`. This eliminates the old pair. Our new repeating pair is `val` (starting at `p`).
* If `p < repeat_start_depth`, the new pair is nested inside or higher than the old one (this case is subtle but possible depending on how the window moved). We would eliminate the "earlier" start to resolve the conflict.





**Step 4 Update Global Maximums**
Now that `start_index` is adjusted to ensure validity, we calculate the current path stats:

* `Length = current_dist - dist_at[start_index]`
* `Nodes = depth - start_index + 1`
* Compare with the global maximum length. If it is longer, update it. If it is equal, take the smaller node count.

**Step 5 Backtrack**
After visiting all children, we pop the current depth `d` from `path_history[val]`. This is vital to ensure that when we go back up to the parent and down a different branch, the history is clean.

## The Solution Code

Here is the implementation in C++. I have avoided using standard library headers in the snippets for brevity, but you should include `vector`, `algorithm`, and `map` as needed.

```cpp
class Solution {
    // Structure to represent graph edges
    struct Edge {
        int to;
        int weight;
    };
    
    // Global adjacency list
    vector<vector<Edge>> adj;
    vector<int> node_vals;
    
    // To store the depths of each value in the current path
    // We use a vector of vectors, or a simplified array of vectors if values are small
    // Given constraints up to 50,000, a pre-sized array of vectors works well
    vector<int> val_depths[50005];
    
    // To store distance from root to a specific depth
    vector<int> depth_dist;
    
    int max_len = -1;
    int min_nodes = 0;

public:
    vector<int> longestSpecialPath(vector<vector<int>>& edges, vector<int>& nums) {
        int n = nums.size();
        node_vals = nums;
        
        // Clear and rebuild adjacency list
        adj.assign(n, {});
        for (auto& e : edges) {
            adj[e[0]].push_back({e[1], e[2]});
            adj[e[1]].push_back({e[0], e[2]});
        }
        
        // Reset our state containers
        for(int i=0; i<=50000; i++) {
            val_depths[i].clear();
        }
        depth_dist.assign(n + 1, 0);
        
        max_len = -1;
        min_nodes = 0;
        
        // Start DFS from root (node 0)
        // Parent is -1
        // Depth is 0
        // Current Distance is 0
        // Window Start (L) is 0
        // Repeat Start (RS) is -1 (no repeating pair yet)
        dfs(0, -1, 0, 0, 0, -1);
        
        return {max_len, min_nodes};
    }

private:
    void dfs(int u, int p, int depth, int curr_dist, int L, int RS) {
        int val = node_vals[u];
        
        // Record the distance to this depth
        depth_dist[depth] = curr_dist;
        
        // Add current node's depth to the history of this value
        val_depths[val].push_back(depth);
        int count = val_depths[val].size();
        
        // PROPOSED NEW STATE
        int new_L = L;
        int new_RS = RS;
        
        // --- Constraint 1 No Triplets ---
        if (count >= 3) {
            // If we have 3 of the same value, the window must shrink 
            // to exclude the FIRST one.
            int first_occ = val_depths[val][count - 3];
            new_L = max(new_L, first_occ + 1);
            
            // If we shrank past the old repeating pair, clear it
            if (new_RS < new_L) {
                new_RS = -1;
            }
        }
        
        // --- Constraint 2 At Most One Pair ---
        // The potential start of a pair for the current value 'val'
        // is its second-to-last occurrence.
        int potential_pair_start = (count >= 2) ? val_depths[val][count - 2] : -1;
        
        if (potential_pair_start >= new_L) {
            // 'val' is repeating in the current window [new_L, depth]
            
            if (new_RS != -1 && new_RS != potential_pair_start) {
                // We have a conflict Two different pairs exist.
                // Pair 1 starts at new_RS.
                // Pair 2 starts at potential_pair_start.
                
                // We must break the earlier pair.
                if (new_RS < potential_pair_start) {
                    new_L = max(new_L, new_RS + 1);
                    new_RS = potential_pair_start;
                } else {
                    new_L = max(new_L, potential_pair_start + 1);
                    // new_RS remains the same
                }
            } else {
                // No conflict, just update the current repeating pair
                new_RS = potential_pair_start;
            }
        }
        
        // Final sanity check If window moved past the repeating start, reset RS
        if (new_RS < new_L) new_RS = -1;
        
        // --- Calculate Result ---
        int path_len = curr_dist - depth_dist[new_L];
        int nodes_count = depth - new_L + 1;
        
        if (path_len > max_len) {
            max_len = path_len;
            min_nodes = nodes_count;
        } else if (path_len == max_len) {
            min_nodes = min(min_nodes, nodes_count);
        }
        
        // --- Recurse ---
        for (auto& edge : adj[u]) {
            if (edge.to != p) {
                dfs(edge.to, u, depth + 1, curr_dist + edge.weight, new_L, new_RS);
            }
        }
        
        // --- Backtrack ---
        // Remove current node from history before returning
        val_depths[val].pop_back();
    }
};

```

## Why This Works

The beauty of this solution lies in how it handles the "state" of the path.

1. **Immutability of Parameters** Notice that `L` (window start) and `RS` (repeat start) are passed by value. This is intentional. When we go deeper into the recursion, we pass the *updated* constraints. When we return (backtrack), the function scope for the parent node still holds the *original* `L` and `RS` valid for that level. We do not need to manually "undo" changes to `L` and `RS` because the recursion stack does it for us naturally.
2. **Backtracking the Map** We *do* need to manually undo changes to `val_depths` because that is a global structure (or a reference) shared across all recursive calls. That is why `pop_back()` is crucial at the end of the function.

## Complexity Analysis

* **Time Complexity** . We visit every node exactly once. Inside each visit, we perform constant-time operations (vector push/pop and simple comparisons). The adjacency list traversal also ensures each edge is visited twice (once from each side), summing to linear time relative to nodes and edges.
* **Space Complexity** . The recursion stack can go up to depth  (in a skewed tree). The `val_depths` structure stores at most  entries across all vectors combined.

## Final Thoughts

This problem is a masterclass in combining graph theory with array-based sliding window logic. The hardest part is correctly identifying which "pair" to break when a conflict arises. By consistently checking if the new window start `new_L` has surpassed the `repeat_start_depth`, we ensure the invariants of the problem are always respected.

I hope this breakdown helps you visualize the solution! Tree problems can be intimidating, but once you linearize the path in your mind (or on the stack), they often turn into familiar array problems.

Happy Coding!
