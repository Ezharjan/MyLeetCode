# Demystifying Tree Rerooting A Friendly Walkthrough for the Node Marking Problem [#3241]

Hello everyone in the coding community! Today I would like to share a detailed walkthrough of a fascinating graph problem. It is a wonderful exercise in algorithmic thinking and tree traversal. If you have ever felt stuck on problems that ask you to find distances or times from every single node in a tree, you are in the right place. We are going to take a very deep dive into a technique known as Tree DP or Rerooting. 

I want to keep things friendly, modest, and highly accessible. We will not be claiming this is the only way to solve the problem, but it is a widely accepted and beautiful approach that can serve as a great learning tool. Grab your favorite beverage, get comfortable, and let us explore this together step by step!

***

## Understanding the Problem

Imagine we have an undirected tree made up of `N` nodes, numbered from 0 up to `N` minus 1. You can think of this tree as a network of radio towers. We want to broadcast a signal starting from a single tower, and we want to know how long it will take for every single tower in the network to receive the signal.



However, there is a catch! The towers are built with two different types of receivers based on their identification number:
* **Odd numbered towers** are fast. If a neighboring tower has the signal, it takes exactly 1 unit of time for this odd tower to get marked as receiving the signal.
* **Even numbered towers** are slow. If a neighboring tower has the signal, it takes exactly 2 units of time for this even tower to get marked.

We are given the edges of this tree, and our goal is to return an array of times. For every node `i`, we need to determine the total time it takes for the entire network to be marked if we start the initial broadcast exactly at node `i` at time zero. 

Each starting node is an independent scenario. When you start at node `i`, all other nodes are completely unmarked.

### A Quick Example to Clarify

Let us say we have a tiny tree with edges connecting node 0 to node 1, and node 0 to node 2. 
* Node 0 is even.
* Node 1 is odd.
* Node 2 is even.

If we start our broadcast at node 0:
Node 1 is odd, so it takes 1 second to receive the signal from node 0.
Node 2 is even, so it takes 2 seconds to receive the signal from node 0.
The whole network is marked when the last node is marked. The maximum of these times is 2 seconds.

If we start our broadcast at node 1:
The signal must travel from node 1 to node 0. Node 0 is even, so it takes 2 seconds for node 0 to get the signal.
From node 0, the signal then travels to node 2. Node 2 is also even, taking another 2 seconds. The total time for node 2 to get the signal is 2 plus 2, which equals 4 seconds. 
The whole network is marked after 4 seconds.

***

## Why Not Just Search From Every Node?

When looking at this problem, the most intuitive thought process is often to simulate the exact scenario they describe. We could use a standard traversal method like Breadth First Search or Depth First Search starting from node 0, record the maximum time, and then repeat the entire process starting from node 1, then node 2, all the way to the last node.

Let us evaluate the cost of doing this. A single search through a tree with `N` nodes takes an amount of time proportional to `N`. If we run this search `N` separate times, the total time required grows proportionally to `N` multiplied by `N`. In algorithmic terms, this is a quadratic time complexity. 

Given the constraints of modern coding challenges where `N` can be as large as 100,000, a quadratic approach will result in billions of operations. The computer will simply run out of time and give us a Time Limit Exceeded error. We need a method that can find the answer for all nodes in a much smaller amount of time, ideally just looking at every node a few times.

***

## The Magic of Tree DP and Rerooting

To solve this smoothly, we can use a concept called Dynamic Programming on Trees, specifically a pattern known as Rerooting. 

Instead of treating every starting point as a completely brand new problem, we try to share information between the scenarios. Imagine you are holding the tree up in the air by grabbing node 0. Gravity pulls all the other nodes downward. This creates a standard parent and child structure.



For any given node in this suspended tree, paths going out of it can be strictly divided into two categories:
1.  **Paths going DOWN:** These are paths that travel into the node's children and further down into its subtrees.
2.  **Paths going UP:** This is the single path that travels up to the node's parent, and from the parent, it can branch out anywhere else in the entire tree (except back down into the starting node).

If we know the longest time it takes to travel DOWN from a node, and the longest time it takes to travel UP from a node, the final answer for that node is simply the maximum of those two values! 

This completely breaks the problem down. We can find all the DOWN paths in one sweep, and all the UP paths in a second sweep. 

### Sweeping Bottom to Top (Finding Downward Paths)

Our first task is to calculate the longest path heading downwards for every single node. We can do this using a Depth First Search that processes children before their parents. 

When we stand at a node, we look at all its children. For each child, we figure out how long it takes to traverse its subtree. Then we add the cost to step from our current node to that child. Remember the rules:
* If the child is an odd number, stepping to it costs 1.
* If the child is an even number, stepping to it costs 2.

We need to keep track of the maximum downward time for each node. But wait, there is a crucial detail we must not forget! We also need to keep track of the **second maximum** downward time, as well as **which specific child** gave us the absolute maximum time.

Why do we need the second maximum? Imagine a parent node wants to tell its child about the paths available outside the child's subtree. If the parent's absolute longest downward path actually goes straight through that very same child, the child cannot use that path (because it would be traveling back onto itself). In this specific scenario, the parent must offer its second longest path to the child instead.

So, for our first sweep, every node will record:
* `down1`: The absolute longest time into the subtrees.
* `down2`: The second longest time into the subtrees via a different child.
* `child1`: The identifier of the child that provided the `down1` path.

### Sweeping Top to Bottom (Finding Upward Paths)

With all the downward information gathered, we perform a second Depth First Search. This time, we start from the top and pass information down to the children.

When a parent is about to visit a child, the parent calculates the `up` value for that child. The `up` value for the child represents the longest path out of the child that goes through the parent.

To calculate this, the child must first step to the parent. The cost of this step depends on the **parent's parity**:
* If the parent is an odd number, stepping to the parent costs 1.
* If the parent is an even number, stepping to the parent costs 2.

Once the signal reaches the parent, it can continue traveling. The parent offers the best path it has available that does not go back into the child. 
The parent has two main options to offer:
1.  The parent's own `up` value (representing paths going even further up the tree).
2.  The parent's `down1` value. However, if the parent's `child1` is exactly the child we are looking at, we must use the parent's `down2` value instead to avoid going backwards!

We take the maximum of these options and add the step cost. This becomes the `up` value for the child. We do this for all nodes, passing the values downwards.



***

## Putting It All Together

After these two sweeps, we have successfully collected everything we need. For any node `i` in the network, we simply look at its longest downward path (`down1`) and its longest upward path (`up`). The time it takes for the entire network to be marked starting from node `i` is the larger of these two values.

Let us look at the code implementation. The code uses C++ and is heavily commented to help you map the concepts we just discussed directly to the syntax.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> timeTaken(vector<vector<int>>& edges) {
        // The number of nodes is always the number of edges plus one
        int n = edges.size() + 1;
        
        // Create an adjacency list to represent our tree network
        vector<vector<int>> adj(n);
        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }
        
        // Arrays to store our dynamic programming states for each node
        // down1 stores the maximum time going down into the subtrees
        vector<int> down1(n, 0);   
        
        // down2 stores the second maximum time going down into the subtrees
        vector<int> down2(n, 0);   
        
        // child1 stores the specific child node that gave us the down1 value
        vector<int> child1(n, -1); 
        
        // up stores the maximum time going up through the parent
        vector<int> up(n, 0);      
        
        // First Sweep: Bottom to Top traversal
        // We use a lambda function for our recursive depth first search
        auto computeDown = [&](auto& self, int currentNode, int parentNode) -> void {
            for (int neighbor : adj[currentNode]) {
                // We do not want to go backwards up the tree during a downward sweep
                if (neighbor == parentNode) {
                    continue;
                }
                
                // Recursively call for the child first (bottom to top processing)
                self(self, neighbor, currentNode);
                
                // Calculate the weight to step down to this neighbor
                // If the neighbor is odd, weight is 1. If even, weight is 2.
                int stepWeight = (neighbor % 2 == 1) ? 1 : 2;
                
                // The total time via this neighbor is the neighbor's max down time plus the step
                int timeViaNeighbor = down1[neighbor] + stepWeight;
                
                // Update our maximums and keep track of the winning child
                if (timeViaNeighbor > down1[currentNode]) {
                    // The old max becomes the new second max
                    down2[currentNode] = down1[currentNode];
                    // The new max takes the top spot
                    down1[currentNode] = timeViaNeighbor;
                    // Record which child gave us this top spot
                    child1[currentNode] = neighbor;
                } else if (timeViaNeighbor > down2[currentNode]) {
                    // If it doesn't beat the top spot, maybe it beats the second spot
                    down2[currentNode] = timeViaNeighbor;
                }
            }
        };
        
        // Second Sweep: Top to Bottom traversal
        auto computeUp = [&](auto& self, int currentNode, int parentNode) -> void {
            for (int neighbor : adj[currentNode]) {
                // We only want to pass information down to children
                if (neighbor == parentNode) {
                    continue;
                }
                
                // Calculate the weight to step UP from the neighbor to the currentNode (the parent)
                // If the currentNode (parent) is odd, weight is 1. If even, weight is 2.
                int stepWeight = (currentNode % 2 == 1) ? 1 : 2;
                
                // The parent wants to give the best path it has.
                // It can choose its own 'up' path, or one of its 'down' paths.
                int bestParentPath = up[currentNode];
                
                // If the parent's absolute best downward path goes through THIS neighbor,
                // we are forced to use the parent's second best downward path to avoid a U turn.
                if (child1[currentNode] == neighbor) {
                    bestParentPath = max(bestParentPath, down2[currentNode]);
                } else {
                    // Otherwise, it is perfectly safe to use the parent's absolute best downward path
                    bestParentPath = max(bestParentPath, down1[currentNode]);
                }
                
                // The 'up' value for the neighbor is the step cost plus the best path the parent can offer
                up[neighbor] = bestParentPath + stepWeight;
                
                // Recursively call for the child to continue passing information downwards
                self(self, neighbor, currentNode);
            }
        };
        
        // Execute the first sweep starting from an arbitrary root (node 0)
        computeDown(computeDown, 0, -1);
        
        // Execute the second sweep starting from the same arbitrary root
        computeUp(computeUp, 0, -1);
        
        // Prepare the final result array
        vector<int> result(n);
        
        // Synthesize the final answers
        for (int i = 0; i < n; i++) {
            // The time taken starting at node i is the maximum of its best downward and best upward paths
            result[i] = max(down1[i], up[i]);
        }
        
        return result;
    }
};

```

---

## Complexity Analysis Plainly Stated

It is always good practice to understand the resource footprint of our code.

**Time Complexity:** We traverse the tree exactly twice. Each traversal visits every node and looks at every edge a constant number of times. Building the adjacency list also takes time proportional to the number of edges. Therefore, the time complexity scales directly with the number of nodes `N`. We can express this plainly as Big O of N. This is wonderfully efficient and will easily pass large hidden test cases without timing out.

**Space Complexity:**
We allocate a few arrays (`down1`, `down2`, `child1`, `up`, and `result`) each of size `N`. The adjacency list also stores `N` nodes and their connections. Furthermore, the recursion stack during our depth first searches will go as deep as the tree is tall, which is at most `N` in the worst case (a straight line tree). Consequently, the space required scales linearly with the number of nodes. This is expressed plainly as Big O of N. Memory usage is well within acceptable limits.

---

## Final Thoughts and Pitfalls to Avoid

Tree DP can feel incredibly abstract the first time you see it. If you found it confusing initially, please do not worry! It is a complex pattern that takes practice to recognize.

One of the most common pitfalls in this specific problem is mixing up the step weights. When calculating the downward paths, the weight depends on the child node you are stepping into. When calculating the upward paths, the weight depends on the parent node you are stepping into. Getting those parity checks inverted is a very easy mistake to make, so double checking that logic is always a good idea.

I hope this detailed explanation has shed some light on the magic of Tree Rerooting. By breaking down the paths into distinct directions and passing information intelligently, we transformed a heavily repetitive task into two elegant, flowing sweeps of the tree structure.

Thank you for reading, and I wish you all the best in your continuous coding journey. Keep practicing, keep sharing, and happy coding!
