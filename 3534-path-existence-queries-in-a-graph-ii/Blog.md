# Conquering Path Existence Queries in a Graph II A Friendly Walkthrough [#3534]

[Blog Post Link](https://leetcode.com/discuss/post/7608881/conquering-path-existence-queries-in-a-g-3oz2/)

Hello fellow developers and problem solvers! 

Now I would like to share my thoughts and a detailed breakdown of a very interesting graph problem we recently encountered. I hope this guide serves as a helpful resource for anyone trying to understand advanced graph traversals and query optimizations. We will take things slowly, explore the mechanics of the problem, and build a solution step by step.



## Understanding the Problem

Imagine you are given a collection of nodes. There are N nodes in total, and each node is labeled from 0 up to N minus 1. Along with these nodes, we are provided with an array of values called `nums`, where each node has a corresponding value. We also receive a specific integer called `maxDiff`.

Here is the rule for connecting these nodes: an undirected edge exists between any two nodes if the absolute difference between their values is less than or equal to `maxDiff`. 

We are then handed a list of queries. Each query consists of a pair of nodes. For every pair, our goal is to find the minimum distance (the shortest path in terms of the number of edges) between the two nodes. If it is impossible to reach one node from the other, we must return negative one for that specific query.

### Breaking Down the Mechanics

Let us visualize what the problem is asking. The edges are not explicitly given to us as a standard list of connections. Instead, they are defined by a mathematical condition. If we have a node with a value of 5 and `maxDiff` is 2, this node is directly connected to any other node whose value falls in the range of 3 to 7 inclusive.

Because every single node with a value of 5 will connect to the exact same set of nodes, the actual index of the node matters much less than its value. This is a crucial observation! All nodes sharing the same value behave identically in our graph.

## The Core Concept Moving from Indices to Values

Since the connections depend entirely on the values, we can simplify our world. Instead of thinking about traversing from node index A to node index B, we can think about traversing from the value of node A to the value of node B.

To do this neatly, we can extract all the unique values from our `nums` array and sort them in ascending order. 

Let us say our unique, sorted values are `1, 2, 3, 4, 8` and our `maxDiff` is `3`.
* From value `1`, we can jump to any value up to `1 + 3 = 4`. The available values are `2, 3`, and `4`.
* If we want to travel as far right as possible, we should jump directly to `4`.
* From value `4`, we can jump to any value up to `4 + 3 = 7`. Looking at our list, there are no values between `4` and `7`, so we cannot reach `8` from `4`.

This reveals a greedy strategy. If we want to travel from a smaller value to a larger value, our best move is always to jump to the largest possible value within our reach. Moving backwards to a smaller value will never help us reach further to the right.

## Why a Simple Simulation is Not Enough

You might be thinking: why not just run a standard Breadth First Search for each query? 

The issue is the scale of the problem. We could have up to 100,000 nodes and 100,000 queries. If we run a search that takes linear time for every single query, the total operations would reach roughly 10 billion. This would unfortunately exceed the time limits set by most online judges. 

We need a way to answer each query much faster, ideally in logarithmic time.

## Introducing Binary Lifting

To speed up our jumping process, we can use a fascinating technique often called Binary Lifting. The idea is to precompute not just where we can jump in 1 step, but where we can jump in 2 steps, 4 steps, 8 steps, and so on.



By computing jumps in powers of two, we can cover massive distances in very few operations. If we need to take 13 steps to reach our destination, we do not simulate 13 individual steps. Instead, we take an 8 step jump, then a 4 step jump, and finally a 1 step jump.

### Building the Jump Table

Let us structure this idea carefully. We will create a two dimensional table. Let us call it `jump`. 
The entry `jump[i][k]` will tell us the furthest index in our sorted unique values array that we can reach starting from index `i`, by taking exactly two to the power of `k` steps.

* **Base Case (k = 0):** Taking two to the power of 0 steps means taking exactly 1 step. For every unique value at index `i`, we look ahead in our sorted array to find the furthest index `j` where the value is less than or equal to `value[i] + maxDiff`. This can be done efficiently using a sliding window or binary search.
* **Building Up:** Once we know where we can land in 1 step, we can figure out 2 steps. The destination after 2 steps is simply the destination of taking 1 step from the place we landed after our first 1 step. 
In general terms: `jump[i][k] = jump[ jump[i][k minus 1] ][k minus 1]`.

By building this table up to about 18 or 19 powers of two (since two to the power of 18 is over 200,000, which covers our maximum possible array size), we prepare ourselves to answer any query instantly.

## Processing the Queries Step by Step

When a query asks us for the shortest path between node `u` and node `v`, we follow a systematic checklist.

1. **Check for Identity:** If `u` and `v` are the exact same node, the distance is naturally 0.
2. **Check for Same Value:** If `u` and `v` are different nodes but happen to have the exact same value in the `nums` array, the distance is 1. (Because the difference is 0, which is always less than or equal to any positive `maxDiff`).
3. **Sort the Values:** We find the values of node `u` and node `v`. Let us call them `valA` and `valB`. Since the graph is undirected, traveling from `valA` to `valB` is the same as traveling from `valB` to `valA`. To keep our logic consistent, if `valA` is greater than `valB`, we swap them so we are always traveling from left to right.
4. **Map to Indices:** We locate the positions of `valA` and `valB` in our array of unique sorted values. Let us call these starting and target indices.
5. **Check Reachability:** We look at our `jump` table. If the furthest we can possibly reach from our starting index using our maximum power of two is still less than the target index, it means the target is entirely unreachable. We return negative one.
6. **Calculate Steps:** If it is reachable, we iterate through our powers of two from largest to smallest. If taking a jump of size two to the power of `k` lands us strictly before our target index, we take that jump! We update our current position and add two to the power of `k` to our step count. 
7. **Final Step:** By the end of this descending loop, we will be standing at the furthest possible point that is strictly less than our target. This means just one more single step will push us exactly to or past our target. We add 1 to our step count and record the answer.

## The C++ Implementation

Here is a clean and straightforward way to write this logic in C++. I have included comments to help map the code back to the concepts we just discussed.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> pathExistenceQueries(int n, vector<int>& nums, int maxDiff, vector<vector<int>>& queries) {
        // Create a copy of the numbers to find unique values
        vector<int> uniqueVals = nums;
        sort(uniqueVals.begin(), uniqueVals.end());
        uniqueVals.erase(unique(uniqueVals.begin(), uniqueVals.end()), uniqueVals.end());
        
        int m = uniqueVals.size();
        int LOG = 18; 
        
        // This table stores the furthest index reachable
        vector<vector<int>> jump(m, vector<int>(LOG));
        
        // Calculate the base case the furthest we can reach in 1 step
        int right = 0;
        for (int i = 0; i < m; ++i) {
            while (right + 1 < m && uniqueVals[right + 1] <= uniqueVals[i] + maxDiff) {
                right++;
            }
            jump[i][0] = right;
        }
        
        // Populate the rest of the table using previous powers of two
        for (int k = 1; k < LOG; ++k) {
            for (int i = 0; i < m; ++i) {
                jump[i][k] = jump[jump[i][k - 1]][k - 1];
            }
        }
        
        // Prepare to process all queries
        vector<int> answer(queries.size());
        
        for (int i = 0; i < queries.size(); ++i) {
            int u = queries[i][0];
            int v = queries[i][1];
            
            // Same exact node
            if (u == v) {
                answer[i] = 0;
                continue;
            }
            
            int valA = nums[u];
            int valB = nums[v];
            
            // Different nodes but identical values
            if (valA == valB) {
                answer[i] = 1;
                continue;
            }
            
            // Ensure we always travel from a smaller value to a larger value
            if (valA > valB) {
                swap(valA, valB);
            }
            
            // Find the index of these values in our unique array
            int startIdx = lower_bound(uniqueVals.begin(), uniqueVals.end(), valA) - uniqueVals.begin();
            int targetIdx = lower_bound(uniqueVals.begin(), uniqueVals.end(), valB) - uniqueVals.begin();
            
            // Check if the target is completely unreachable
            if (jump[startIdx][LOG - 1] < targetIdx) {
                answer[i] = -1;
                continue;
            }
            
            int currentPos = startIdx;
            int stepsTaken = 0;
            
            // Greedily take the largest possible jumps that do not overshoot the target
            for (int k = LOG - 1; k >= 0; --k) {
                if (jump[currentPos][k] < targetIdx) {
                    currentPos = jump[currentPos][k];
                    stepsTaken += (1 << k);
                }
            }
            
            // We are now just one step away from reaching or crossing the target
            answer[i] = stepsTaken + 1;
        }
        
        return answer;
    }
};

```

## Understanding Time and Space Complexity

It is always good practice to evaluate how much time and memory our solution consumes. Let us break it down without getting bogged down by mathematical symbols.

### Time Complexity Evaluation

First, we copy and sort the original array. Sorting an array of N elements takes Big O of N times log N operations.

Next, we build our jump table. Finding the first step takes linear time because our two pointers only move forward. Populating the rest of the table requires iterating over M unique elements for LOG columns. This takes Big O of N times LOG operations.

Finally, for each query, we perform binary searches to find the start and target indices, which takes logarithmic time. Then, we loop LOG times to calculate the steps. For Q queries, this takes Big O of Q times LOG operations.

Combining all these parts, the overall time complexity is Big O of (N plus Q) times log N. This is highly efficient and will comfortably pass within the usual time limits.

### Space Complexity Evaluation

We create an array for the unique values, which takes Big O of N space in the worst case.
We also create our two dimensional jump table. It has N rows and LOG columns, resulting in a space complexity of Big O of N times LOG.
Therefore, the overall space complexity is Big O of N times log N. This easily fits into standard memory limits.

## Concluding Thoughts

This problem beautifully illustrates how redefining the state space from raw indices to conceptual values can simplify a seemingly complex graph. By utilizing binary lifting, we transformed an operation that could have taken linear time into a lightning fast logarithmic operation.

I hope this breakdown was helpful and sheds some light on how to approach these kinds of query problems. Feel free to use this code, play around with it, and try applying binary lifting to other problems you encounter!

Happy coding, and see you in the next challenge!
