# Unlocking the Mystery of Subarray Reversals and Graph Traversal [#2612]

[Blog Post Link](https://leetcode.com/discuss/post/7560969/unlocking-the-mystery-of-subarray-revers-e4bo/)

Hello friends and fellow coding enthusiasts!

Today I want to share a journey through an interesting algorithmic challenge that involves array manipulation, graph theory, and some clever mathematical observations. If you have ever stared at a problem involving reversing subarrays and wondered how to optimize the process, this deep dive is for you. We are going to explore a method to solve the "Minimum Reverse Operations" problem.

### Understanding the Challenge

Let us start by clearly defining what we are trying to solve. Imagine you have a long array of zeros, with a length we will call **n**. In this entire array, there is exactly one position containing the number **1**, while everything else is **0**. Let us say the **1** starts at a specific index called **p**.

We are allowed to perform a specific operation on this array. We can choose any contiguous subarray of a fixed size, let us call it **k**, and reverse it. However, there is a catch! There are certain "banned" positions in the array. We are never allowed to place our **1** into any of these banned indices. If a reversal would cause the **1** to land on a banned index, that move is invalid (or simply, we consider that the **1** can never rest there).

Our goal is to calculate the minimum number of operations required to move the **1** from its starting position **p** to every other possible position in the array. If a position cannot be reached, we should return **-1** for that index.

**Let us look at a quick example to visualize this.**

Suppose we have an array of length 4. The **1** starts at index 0. We have a banned list containing indices 1 and 2. The reversal length **k** is 4.

* Initially, **1** is at index 0. Distance is 0.
* We cannot move **1** to index 1 or 2 because they are banned.
* We can reverse the subarray of size 4 starting at index 0. This subarray covers indices 0, 1, 2, and 3.
* When we reverse indices `[0, 1, 2, 3]`, the element at index 0 moves to index 3.
* Since index 3 is not banned, this is a valid move. We reached index 3 in 1 operation.

The result would be `[0, -1, -1, 1]`.

### The Graph Theory Perspective

At first glance, this problem might feel like a dynamic programming challenge or perhaps a complex greedy problem. However, since we are looking for the *minimum* number of operations to reach various states, this screams **Breadth First Search**, also known as BFS.

We can view the array indices `0` to `n minus 1` as nodes in a graph. An edge exists from index `u` to index `v` if we can perform one valid reversal that moves the **1** from `u` to `v`.

If we run a standard BFS starting from `p`, we can find the shortest path to all other nodes. But there is a significant hurdle here. The number of indices **n** can be up to 100,000. If we try to iterate through every possible subarray reversal for every node, the number of edges could be enormous. A naive BFS would be too slow because checking every possible move from every position results in a quadratic time complexity. We need something faster.

### The Mathematical Pattern

To optimize this, we need to analyze exactly where the **1** lands after a reversal.

Let us say our **1** is currently at index **u**. We choose to reverse a subarray of size **k** that starts at index **L**. This subarray covers the range from **L** to **L + k - 1**.
For the **1** to be affected by this reversal, it must be inside this subarray. This means **L** must be less than or equal to **u**, and **L + k - 1** must be greater than or equal to **u**.

When we reverse a range `[L, R]`, an element at position **u** moves to a new position **v** calculated by the formula:
`v = L + R - u`

Substituting `R = L + k - 1`, we get:
`v = L + (L + k - 1) - u`
`v = 2 * L + k - 1 - u`

This formula reveals a beautiful property. Look at the term `2 * L`. This implies that as we slide our reversal window one step to the right (increasing **L** by 1), the target position **v** increases by exactly **2**.

This observation tells us two critical things:

1. **Contiguous Range:** The set of reachable positions from **u** forms a contiguous sequence of values, but with a gap of 2 between them. It is an arithmetic progression with a common difference of 2.
2. **Parity:** All reachable positions from a single move will have the same parity (all even or all odd) because the difference between any two adjacent reachable positions is 2.

### Determining the Range

For a current position **u**, we do not need to iterate through every possible **L**. We only need to find the minimum possible **L** and the maximum possible **L** to know the full range of reachable targets.

* **Left Bound:** The subarray cannot start before index 0. Also, the subarray must end at least at **u** to include it. This gives us a constraint for the minimum **L**.
* **Right Bound:** The subarray cannot extend past the end of the array. Also, the subarray must start at or before **u**. This gives us a constraint for the maximum **L**.

Once we calculate the minimum and maximum **L**, we plug them into our formula `v = 2 * L + k - 1 - u` to find the minimum reachable index (`min_v`) and the maximum reachable index (`max_v`).

So, from node **u**, we can jump to any index `v` such that:
`min_v <= v <= max_v`
AND
`v` has the same odd or even parity as `min_v`.

### optimizing the Search

We have established that from **u**, we can reach a range of indices stepping by 2. If we simply iterate this range in a BFS, we might still visit nodes multiple times or check edges unnecessarily.

To solve this efficiently, we can use a mechanism to store "unvisited" nodes. Specifically, we can maintain two sets of available indices:

1. **Even Set:** Contains all unvisited, non banned even indices.
2. **Odd Set:** Contains all unvisited, non banned odd indices.

When we process a node **u** in our BFS:

1. We calculate the range `[min_v, max_v]`.
2. We identify which set to look in based on the parity of `min_v`.
3. We use the set's `lower_bound` function to efficiently find the first available node in our range.
4. We iterate through the set from that point, adding found nodes to our BFS queue and removing them from the set immediately.
5. We stop iterating the set once we pass `max_v`.

By removing nodes from the set as soon as we visit them, we ensure that each index is processed exactly once. This transforms our complexity. Instead of checking every edge, we are essentially performing a series of efficient range queries and deletions.

### The Algorithm Steps Summary

Here is the logical flow of our solution:

1. **Initialization:** Create an array `dist` filled with `-1` to store distances. Set the distance of start node `p` to `0`.
2. **Banned Processing:** Mark banned indices so we do not include them in our available sets.
3. **Sets Setup:** Create two sets (or similar structures), one for even indices and one for odd indices. Fill them with all indices from `0` to `n minus 1` that are not banned and are not the start node `p`.
4. **BFS Queue:** Push `p` into a queue.
5. **Processing Loop:** While the queue is not empty:
* Pop the current node `u`.
* Calculate the sliding window bounds. The smallest starting index for a subarray covering `u` is `max(0, u - k + 1)`. The largest is `min(u, n - k)`.
* Convert these start indices into target indices using `v = 2 * L + k - 1 - u`. Let us call the resulting range `[start_v, end_v]`.
* Select the correct set (even or odd) matching `start_v`.
* Search the set for values between `start_v` and `end_v`.
* For every found value: update its distance, add it to the queue, and remove it from the set.


6. **Return:** Return the `dist` array.

### The C++ Implementation

Below is the code that implements this logic. It is clean, uses standard libraries, and handles the logic step by step.

```cpp
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> minReverseOperations(int n, int p, vector<int>& banned, int k) {
        // Prepare the answer vector, initialized to -1
        vector<int> dist(n, -1);
        
        // Use a boolean vector for fast lookup of banned positions
        vector<bool> is_banned(n, false);
        for (int x : banned) {
            is_banned[x] = true;
        }
        
        // We use two sets to store unvisited nodes:
        // unvisited[0] for even indices
        // unvisited[1] for odd indices
        // This helps us quickly find the next available node in a range with step 2
        set<int> unvisited[2];
        for (int i = 0; i < n; ++i) {
            // We only add valid, unvisited nodes (excluding start p)
            if (i != p && !is_banned[i]) {
                unvisited[i % 2].insert(i);
            }
        }
        
        // Standard BFS queue
        queue<int> q;
        q.push(p);
        dist[p] = 0; // Distance to source is 0
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            // Calculate the range of possible subarray start positions (L)
            // A subarray of length k starting at L covers [L, L + k - 1]
            // We need this interval to cover index u.
            
            // Constraint 1: Subarray must end at or after u
            // L + k - 1 >= u  -->  L >= u - k + 1
            int min_L = max(0, u - k + 1);
            
            // Constraint 2: Subarray must start at or before u
            // L <= u
            // Also L + k <= n (must fit in array)
            int max_L = min(u, n - k);
            
            if (min_L > max_L) continue;
            
            // Convert L bounds to target index v bounds
            // Formula: v = 2*L + k - 1 - u
            int start_v = 2 * min_L + k - 1 - u;
            int end_v = 2 * max_L + k - 1 - u;
            
            // Determine parity to choose the correct set
            int parity = start_v % 2;
            auto& s = unvisited[parity];
            
            // Find the first unvisited node >= start_v
            auto it = s.lower_bound(start_v);
            
            // Iterate through the set and process nodes within range
            while (it != s.end() && *it <= end_v) {
                int v = *it;
                
                // Update distance and push to queue
                dist[v] = dist[u] + 1;
                q.push(v);
                
                // Remove from set to prevent revisiting
                // s.erase returns the iterator to the next element
                it = s.erase(it);
            }
        }
        
        return dist;
    }
};

```

### Why This Approach Works Well

Let us discuss the efficiency here.

1. **Time Complexity:** Each node is added to the queue at most once. Inside the BFS loop, we perform set operations. The critical part is that elements are removed from the set upon visitation. This means `s.erase` is called at most **N** times in total across the entire execution. Finding the lower bound takes logarithmic time. Therefore, the overall time complexity is roughly **O(N log N)**. Given **N** is up to 100,000, this is well within acceptable limits.
2. **Space Complexity:** We store the `dist` array, the `is_banned` array, and the sets of unvisited nodes. This consumes linear space, **O(N)**.

### Final Thoughts

This problem is a wonderful example of how direct simulation is often too slow, but mathematical insight can reveal structure in the chaos. By noticing that the target indices follow an arithmetic progression, we transformed a potentially dense graph traversal into a streamlined range query process using sets.

I hope this explanation helps you understand the mechanics behind this "Hard" problem. It is satisfying to see the `queue` and `set` work in harmony to hop through the array efficiently.

Happy Coding!
