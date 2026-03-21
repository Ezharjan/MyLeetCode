# Demystifying Directed Graphs and Cycle Lengths [#2876]

Hello fellow developers and welcome to my blog! Today I would like to share my thoughts and notes on an incredibly fascinating graph puzzle. Graph problems can sometimes feel a bit overwhelming, especially when we start talking about cycles and paths. However, if we take our time to break the logic down into smaller and more manageable pieces, the entire concept becomes much more approachable and actually quite fun to explore.

I want to walk you through a solid way to solve the problem of counting visited nodes in a directed graph. I will try to be as detailed as possible so that everyone, regardless of their current experience level with graph theory, can follow along and understand the underlying mechanics. My goal is to share this knowledge with the community in a clear and friendly manner. We will not use any complicated mathematical notation here, just plain logic and straightforward explanations!

## Understanding the Core Problem

Let us start by clearly defining what we are trying to achieve. Imagine a map of islands where each island has exactly one one way bridge pointing to another island. You are given a list of these connections. Your starting point can be any island. Your journey continues by crossing the bridges from island to island. Your journey officially stops the moment you arrive at an island you have already visited during this specific trip. 

The main objective is to figure out exactly how many unique islands you step on for every single possible starting island on the map. 

Because every single node in our graph has exactly one outgoing connection, the graph naturally forms a very specific and predictable structure. If you start walking from any node, you will eventually and inevitably get trapped in a loop. There are no dead ends because every node points to something. Therefore, the graph is composed of one or more loops. Some nodes are part of these loops directly. Other nodes form branches or paths that eventually lead right into these loops. 

If you start your journey on a node that is already part of a loop, you will simply walk around that entire loop until you hit your starting point again. The number of unique nodes you visit will be exactly equal to the number of nodes inside that loop. 

On the other hand, if you start your journey on a branch node, you will travel down the path, enter the loop at some intersection, and then walk around the loop. In this scenario, the total number of unique nodes you visit will be the distance you traveled down the branch plus the size of the loop you ended up in.

## Breaking Down the Strategy

To solve this effectively, we can divide our approach into three distinct phases. This makes the implementation clean and easy to follow.

### Phase One Identifying the Branches

First, we need a reliable way to separate the branch nodes from the loop nodes. We can achieve this by counting the number of incoming connections for every single node. We call this the indegree of a node. 

If a node has an indegree of zero, it means no other node points to it. This node must be the very beginning of a branch. It cannot possibly be part of a loop because loop nodes always have at least one incoming connection from the previous node in the loop.

We can gather all these starting nodes and place them into a waiting line or a queue. We then process them one by one. When we look at a node from our queue, we see where it points and imagine removing its outgoing bridge. By removing this bridge, the destination node now has one less incoming connection. We decrease the indegree of that destination node. If the destination node now has an indegree of zero, we add it to our queue because it has become the new tip of a branch.

This process is a common algorithm used to navigate dependencies, and it will effectively trim away all the branches from our graph. We will keep a record of the exact order in which we removed these branch nodes. This historical record will be extremely useful later.

### Phase Two Calculating the Loop Lengths

Once our queue is completely empty, we have successfully trimmed away all the branch paths. What remains? The loops! Any node that still has an indegree greater than zero is guaranteed to be part of a loop. 

Now, we can scan through our nodes. If we find a node with an indegree greater than zero that we have not processed yet, we know we have discovered an unexplored loop. We can simply start walking from this node, following the connections, and counting how many steps it takes to get back. We keep a list of all the nodes we visit on this circular walk. 

Once we complete the circle, we know the size of the loop. We can then assign this size as the final answer for every single node that belongs to this specific loop. We mark these nodes as completely processed so we do not measure the same loop twice.

### Phase Three Working Backwards Up the Branches

At this point, we know the exact number of visited nodes for every single loop node. The only pieces missing are the answers for the branch nodes we trimmed away in Phase One.

This is where our historical record comes into play. We have a list of all the branch nodes in the order we removed them. If we reverse this list, we will be looking at the branch nodes starting from those closest to the loops and working our way outwards to the very tips of the branches.

This reverse order is magical because when we look at any branch node, the node it points to has already been fully processed and has its final answer calculated. Therefore, the answer for our current branch node is simply one plus the answer of the node it points to. We iterate through our reversed list, performing this simple addition, and eventually, every single node in the graph will have its correct answer!

## Detailed Step by Step Walkthroughs

To ensure everyone fully grasps the concept, let us trace through some examples together.

### Example One Walkthrough

Imagine we have four nodes. The connections are as follows: node 0 points to 1, node 1 points to 2, node 2 points to 0, and node 3 points to 0.

Let us count the indegrees.
Node 0 receives incoming connections from node 2 and node 3. Its indegree is 2.
Node 1 receives a connection from node 0. Its indegree is 1.
Node 2 receives a connection from node 1. Its indegree is 1.
Node 3 receives zero incoming connections. Its indegree is 0.

Our queue starts with Node 3.
We process Node 3. It points to Node 0. We decrease the indegree of Node 0 from 2 to 1. Since the indegree of Node 0 is not zero, we do not add it to the queue. 
We add Node 3 to our historical record of trimmed branches.
Our queue is now empty. Phase One is complete.

Now for Phase Two, identifying loops. We look at the remaining indegrees. Node 0, Node 1, and Node 2 all have indegrees greater than zero.
We start exploring from Node 0. Node 0 goes to 1, 1 goes to 2, 2 goes back to 0. We found a loop containing three nodes.
We assign the answer 3 to Node 0, Node 1, and Node 2.

Finally, Phase Three. We look at our trimmed branches. The only node in our historical record is Node 3. 
We know Node 3 points to Node 0. The answer for Node 0 is 3. Therefore, the answer for Node 3 is one plus 3, which equals 4.

Our final answers are exactly correct: 3 for node 0, 3 for node 1, 3 for node 2, and 4 for node 3.

### Example Two Walkthrough

Let us look at a graph consisting of five nodes forming one giant circle. Node 0 points to 1, 1 points to 2, 2 points to 3, 3 points to 4, and 4 points back to 0.

We count the indegrees. Every single node has exactly one incoming connection. 
Because no node has an indegree of zero, our queue starts completely empty. 
Phase One finishes immediately with an empty historical record. There are no branches to trim!

We move to Phase Two. All nodes have an indegree of 1.
We pick Node 0 and start walking. We visit 0, 1, 2, 3, 4, and return to 0. The loop contains exactly five nodes.
We assign the answer 5 to Node 0, Node 1, Node 2, Node 3, and Node 4.

In Phase Three, we have an empty historical record, so there is no work to do.
The final answers are 5 for every single node. This makes perfect sense because no matter where you start on a giant circle, you will visit every node exactly once before repeating.

## The C++ Implementation

Here is a clean and straightforward C++ implementation based on the logic we just discussed. I have utilized standard vectors and a standard queue. I used the standard reverse function to process the trimmed nodes backwards to keep the logic exceptionally clean. Note that while programming syntax requires minus signs for certain operations, I have kept the logic as readable as possible!

```cpp
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> countVisitedNodes(vector<int>& edges) {
        int n = edges.size();
        vector<int> indegree(n, 0);
        
        for (int destination : edges) {
            indegree[destination] += 1;
        }
        
        queue<int> leavesQueue;
        for (int i = 0; i < n; ++i) {
            if (indegree[i] == 0) {
                leavesQueue.push(i);
            }
        }
        
        vector<int> trimmedOrder;
        while (!leavesQueue.empty()) {
            int currentNode = leavesQueue.front();
            leavesQueue.pop();
            trimmedOrder.push_back(currentNode);
            
            int nextNode = edges[currentNode];
            indegree[nextNode] -= 1;
            if (indegree[nextNode] == 0) {
                leavesQueue.push(nextNode);
            }
        }
        
        vector<int> resultAnswers(n, 0);
        
        for (int i = 0; i < n; ++i) {
            if (indegree[i] > 0 && resultAnswers[i] == 0) {
                int walkNode = i;
                vector<int> currentCycle;
                
                while (resultAnswers[walkNode] == 0) {
                    currentCycle.push_back(walkNode);
                    resultAnswers[walkNode] = n + 1; 
                    walkNode = edges[walkNode];
                }
                
                int cycleLength = currentCycle.size();
                for (int cycleNode : currentCycle) {
                    resultAnswers[cycleNode] = cycleLength;
                }
            }
        }
        
        std::reverse(trimmedOrder.begin(), trimmedOrder.end());
        
        for (int branchTip : trimmedOrder) {
            int targetNode = edges[branchTip];
            resultAnswers[branchTip] = resultAnswers[targetNode] + 1;
        }
        
        return resultAnswers;
    }
};
```

## Understanding the Complexity

It is always good practice to understand the resources our solution requires. 

For the time complexity, we are looking at a performance that scales directly with the number of nodes. We call this Big O of N. We iterate through the edges to calculate incoming connections. We process branch nodes exactly once in the queue. We traverse the loop nodes exactly once to find cycle lengths. Finally, we iterate through the trimmed branches once more. Because we only visit each node a small constant number of times, the overall time grows linearly, which is a very solid and effective outcome.

For the space complexity, we also require Big O of N space. We maintain an array for the indegrees, a queue for trimming leaves, a list to keep track of the removal order, and an array to store our final answers. All of these data structures grow strictly in proportion to the number of nodes in our graph. This is well within expected memory limits and is completely standard for graph traversals.

I hope this thorough walkthrough helps you understand the fascinating dynamics of functional directed graphs! Breaking a problem down into branches and loops makes everything so much clearer. Thank you for reading, and I wish you all the best on your coding journey!
