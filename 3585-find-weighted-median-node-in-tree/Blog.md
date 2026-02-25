# Finding the Balance Point in Weighted Trees [#3585]

[Blog Post Link](https://leetcode.com/discuss/post/7608820/finding-the-balance-point-in-weighted-tr-t5hi/)

## Introduction to the Weighted Median Challenge

Hello fellow developers! Today I want to walk you through a fascinating problem that combines tree traversal with the concept of a "weighted center." If you have ever worked with geographical data or network routing, you know that the physical midpoint isn't always the most important one. Sometimes, the "weight" or "cost" of the path determines where the true balance lies.

In this scenario, we are given a tree with $n$ nodes and weighted edges. For any two nodes $u$ and $v$, there is exactly one path between them. Our goal is to find the "weighted median node." This is defined as the very first node $x$ on the path from $u$ to $v$ such that the sum of the edge weights from $u$ to $x$ is at least half of the total path weight.

## Understanding the Problem Mechanics

Before we dive into the code, let's break down what "half the total path weight" actually looks like. Imagine a path from Node 3 to Node 4 with several edges between them. If the total sum of all edge weights on this path is 11, then the halfway point is 5.5. We are looking for the first node we encounter while walking from 3 toward 4 that makes our accumulated travel distance 5.5 or greater.

This problem is tricky because the path between two nodes in a tree isn't always a straight line up toward the root. It usually goes up to a shared ancestor and then back down to the destination.

## The Strategy: Binary Lifting and LCA

To solve this efficiently for many queries, we need a way to jump through the tree quickly rather than stepping node by node. We use a technique called Binary Lifting.

### 1. Preprocessing the Tree

We start by picking a root (Node 0) and performing a Depth First Search (DFS). During this search, we record three vital pieces of information for every node:

* **Depth:** How many steps is this node from the root?
* **Distance:** What is the total weight from the root to this node?
* **Ancestors:** We create a table where we store the 1st ancestor (parent), the 2nd ancestor, the 4th, the 8th, and so on. This table allows us to "jump" up the tree in powers of two.

### 2. Finding the Lowest Common Ancestor (LCA)

For any two nodes $u$ and $v$, the LCA is the highest node that is an ancestor to both. Finding this is crucial because the path from $u$ to $v$ is simply:
$u \rightarrow LCA \rightarrow v$

### 3. Calculating Path Weight

With our precalculated distances from the root, the total weight of the path between $u$ and $v$ is:
$TotalWeight = (DistToRoot(u) + DistToRoot(v)) - (2 * DistToRoot(LCA))$

## Locating the Median Node

Once we have the total weight and the LCA, we determine if the median sits on the "climbing" part of the path (from $u$ up to $LCA$) or the "descending" part (from $LCA$ down to $v$).

### Case A: The Median is between $u$ and $LCA$

If the distance from $u$ to the $LCA$ is already greater than or equal to half the total weight, we know our node $x$ is somewhere on the way up. We use binary lifting to find the highest possible ancestor that hasn't reached the halfway mark yet, then move one step further up to find our answer.

### Case B: The Median is between $LCA$ and $v$

If we haven't hit the halfway mark by the time we reach the $LCA$, the median must be on the path coming down toward $v$. A clever way to find this is to search from $v$ upwards. If we want the first node from $u$'s perspective that hits the weight, we are looking for the node closest to the $LCA$ on $v$'s side that satisfies the remaining distance requirement.

## The Implementation in C++

Below is the complete logic. Note how we use `long long` for weights to prevent overflow, as edge weights can be quite large.

```cpp
class Solution {
public:
    // We use 18 because 2^17 is 131,072, which covers our 10^5 node limit
    int LOG = 18;
    vector<vector<int>> up;
    vector<int> depth;
    vector<long long> dist;
    vector<vector<pair<int, long long>>> adj;

    // DFS to prepare the jump table and distances
    void dfs(int node, int parent, int d, long long w) {
        up[node][0] = parent == -1 ? node : parent;
        depth[node] = d;
        dist[node] = w;
        
        for (int i = 1; i < LOG; i++) {
            up[node][i] = up[up[node][i - 1]][i - 1];
        }
        
        for (auto& edge : adj[node]) {
            int neighbor = edge.first;
            long long weight = edge.second;
            if (neighbor != parent) {
                dfs(neighbor, node, d + 1, w + weight);
            }
        }
    }

    // Standard LCA function using binary lifting
    int getLCA(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        
        int diff = depth[u] - depth[v];
        for (int i = 0; i < LOG; i++) {
            if ((diff >> i) & 1) u = up[u][i];
        }
        
        if (u == v) return u;
        
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    vector<int> findMedian(int n, vector<vector<int>>& edges, vector<vector<int>>& queries) {
        up.assign(n, vector<int>(LOG));
        depth.assign(n, 0);
        dist.assign(n, 0);
        adj.assign(n, vector<pair<int, long long>>());

        for (auto& edge : edges) {
            adj[edge[0]].push_back({edge[1], (long long)edge[2]});
            adj[edge[1]].push_back({edge[0], (long long)edge[2]});
        }

        dfs(0, -1, 0, 0);

        vector<int> ans;
        for (auto& q : queries) {
            int u = q[0], v = q[1];
            int L = getLCA(u, v);
            long long totalW = dist[u] + dist[v] - 2 * dist[L];

            // Check if the median is on u's side of the LCA
            if (2 * (dist[u] - dist[L]) >= totalW) {
                int curr = u;
                for (int i = LOG - 1; i >= 0; i--) {
                    if (depth[curr] - (1 << i) >= depth[L]) {
                        int jumpNode = up[curr][i];
                        // If jumping here still hasn't reached the halfway weight
                        if (2 * (dist[u] - dist[jumpNode]) < totalW) {
                            curr = jumpNode;
                        }
                    }
                }
                // The first node to hit or exceed the weight is the parent of curr
                // unless u itself was the answer.
                if (2 * (dist[u] - dist[curr]) >= totalW) ans.push_back(curr);
                else ans.push_back(up[curr][0]);
            } else {
                // The median is on v's side of the LCA
                int curr = v;
                for (int i = LOG - 1; i >= 0; i--) {
                    if (depth[curr] - (1 << i) >= depth[L]) {
                        int jumpNode = up[curr][i];
                        // From u's perspective, we want the first node that is >= half.
                        // From v's perspective, this is the highest node where 
                        // dist(v, node) <= totalW - (totalW / 2)
                        if (2 * (dist[v] - jumpNode[dist]) <= totalW) {
                            // wait, let's use the distance from u logic for clarity
                            long long distFromU = totalW - (dist[v] - dist[jumpNode]);
                            if (2 * distFromU >= totalW) {
                                curr = jumpNode;
                            }
                        }
                    }
                }
                // Specifically for the v-side, we seek the ancestor of v 
                // that is closest to the LCA while still having dist(u, x) >= half weight.
                int vSideNode = v;
                for (int i = LOG - 1; i >= 0; i--) {
                    if (depth[vSideNode] - (1 << i) >= depth[L]) {
                        int jumpNode = up[vSideNode][i];
                        long long dUtoJump = totalW - (dist[v] - dist[jumpNode]);
                        if (2 * dUtoJump >= totalW) {
                            vSideNode = jumpNode;
                        }
                    }
                }
                ans.push_back(vSideNode);
            }
        }
        return ans;
    }
};

```

## Complexity Analysis

Let's look at how this performs as the tree grows.

The Preprocessing phase involves a single DFS traversal. At each node, we fill 18 entries in our jump table. This means the time spent preparing is proportional to $N * \log(N)$.

The Query phase happens for each request. Finding the LCA takes $\log(N)$ time, and our binary search for the median node also takes $\log(N)$ time. If we have $Q$ queries, the total time for this phase is $Q * \log(N)$.

In terms of space, we store the jump table which takes $N * \log(N)$ memory. For a tree with 100,000 nodes, this is roughly 1.8 million integers, which fits comfortably within the memory limits of most modern environments.

## Final Thoughts

This problem is a great example of how basic tree algorithms like LCA can be extended to solve complex path requirements. By transforming the "weighted median" into a distance search and using binary lifting to navigate, we turn what could be a slow linear search into a very fast logarithmic one.

I hope this detailed breakdown helps you tackle similar tree problems in the future. 

Happy coding!
