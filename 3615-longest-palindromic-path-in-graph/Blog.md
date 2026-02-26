# Unraveling the Longest Palindromic Path in a Graph A Step by Step Guide [#3615]

Hello everyone! Today I would love to share my thoughts on a fascinating graph traversal problem that involves finding a symmetrical sequence of characters. Problems that mix graphs and strings are always a fun challenge, and this one provides a great opportunity to explore state tracking and graph exploration techniques. 

I want to break this down in a highly detailed, friendly, and accessible way so that anyone can understand the underlying logic and apply it successfully. Whether you are a beginner looking to understand bit manipulation or an experienced programmer reviewing graph algorithms, I hope you find this guide helpful and clear!

## Understanding the Core Problem

Imagine you are given a collection of distinct locations, and some of these locations are connected by pathways. In computer science terminology, we call this an undirected graph. Each location or node has a specific lowercase English letter assigned to it. 

Your objective is to travel along the pathways from node to node, recording the letters you encounter to form a word. However, there are two significant rules you must follow. First, you cannot visit the exact same location more than once during your journey. This ensures your path is simple and unique. Second, the word you form must read exactly the same forwards and backwards. Such a word is known as a palindrome. Your ultimate goal is to find the longest possible palindrome you can construct under these conditions.



A critical piece of information provided in the constraints is that the total number of nodes in the graph is at most fourteen. Whenever you see such a small maximum limit, it is a strong signal that we can use an advanced technique to keep track of every single location we have visited without running out of memory or processing time.

## The Strategy Growing from the Inside Out

When trying to find palindromes, a common pitfall is trying to build the word from the first letter to the last. While that approach works for some problems, it becomes incredibly complicated here because the path must be symmetrical. 

A much more natural way to think about a palindrome is to build it from the center outwards. Think about the word "racecar". The center is the letter "e". If you expand outwards by one step, you get "cec". Expand again, you get "aceca", and finally "racecar". 

We can apply this exact inside out logic to our graph! Palindromes generally fall into two categories based on their length.

First, we have odd length palindromes. These always have a single distinct center. For example, in the string "aba", the central letter is "b". In our graph, any single unvisited node can serve as the starting center for an odd length palindrome. 

Second, we have even length palindromes. These do not have a single central character; instead, their center is effectively the empty space between two identical characters. For example, in the string "abba", the center is between the two "b"s. In our graph, any pathway connecting two distinct nodes that share the exact same character can serve as the starting center for an even length palindrome.



Our strategy will be to identify all possible starting centers, both single nodes and valid connected pairs, and then simultaneously expand them outwards step by step until we can no longer make a valid symmetrical addition.

## Tracking Visited Nodes with Bitmasking

Because we are strictly forbidden from visiting any node more than once, we need a reliable way to remember exactly which nodes are currently part of our growing path. Using a standard list or array to check history would be too slow. This is where bitmasking comes to the rescue!

Bitmasking uses the binary representation of a single integer to act as a row of light switches. Since we have at most fourteen nodes, we can use an integer where the first fourteen binary digits represent our nodes. 



If node zero is visited, the zeroth switch is flipped to one. If node five is visited, the fifth switch is flipped to one. 

When we want to check if a specific node has been visited, we simply look at its corresponding binary digit. If the digit is one, we have already been there, and we must skip it. If the digit is zero, it is safe to visit! Furthermore, when we want to add a new node to our path, we use a bitwise OR operation to flip that specific zero to a one, creating a brand new bitmask state.

The beauty of the bitmask is that the number of "on" switches exactly equals the current length of our palindromic string. If a bitmask has five bits set to one, it means our path contains exactly five distinct nodes.

## Exploring with Breadth First Search

To systematically expand our palindromes, we will utilize an algorithm called Breadth First Search. This algorithm is wonderful because it explores all immediate possibilities before moving on to deeper, more complex ones.

We will initialize a queue to hold our active states. A state needs to contain three pieces of information to completely describe our current situation.
* The bitmask recording all the nodes we have included so far.
* The node situated at the left end of our current path.
* The node situated at the right end of our current path.

At the very beginning, we will populate our queue with all the base cases we discussed earlier. We will add all individual nodes as starting states. Then, we will look at all the pathways in the graph. If a pathway connects two nodes with the same letter, we will add that pair as a starting state too.

Once our queue is populated, the real work begins. We will enter a loop that continues as long as the queue is not empty. During each cycle, we take the state at the front of the queue and attempt to expand it.

To expand, we must look at all the neighboring nodes of our left end, and all the neighboring nodes of our right end. We want to pick one neighbor from the left and one neighbor from the right to add to our path. 

For an expansion to be completely valid, it must pass three checks.
First, the new left neighbor must not be in our current bitmask. 
Second, the new right neighbor must not be in our current bitmask, and it must also be a completely different node from the new left neighbor.
Third, the character assigned to the new left neighbor must perfectly match the character assigned to the new right neighbor.

If all three conditions are met, we have successfully created a longer palindrome! We update our bitmask to include both new nodes, and we push this fresh, expanded state into our queue for future exploration. Every time we pop a state from the queue, we check the number of active bits in its mask to see if we have broken our previous record for the longest path.

## Avoiding Redundant Work with Memory Compression

One small detail to consider is that a path expanding symmetrically is identical regardless of which end you call the left and which you call the right. Expanding node A and node B is logically the exact same as expanding node B and node A.

If we explore both combinations, we will end up doing twice the amount of work for no reason. To prevent this, we can establish a simple rule. Whenever we create a new state, we will always ensure the smaller node identifier is assigned to the left position, and the larger node identifier is assigned to the right position.

Additionally, to prevent processing the exact same state multiple times through different roundabout routes, we need a memory tracker. We will create a flat boolean array to record every state we evaluate. 

Since our state consists of three integers (the mask, the left node, and the right node), we can map these three values into a single unique index for our flat array. The maximum possible value for the mask is related to two to the power of fourteen. The maximum value for the nodes is fourteen. By multiplying the mask by fourteen, adding the left node, multiplying all of that by fourteen again, and finally adding the right node, we generate a perfectly unique index for every possible situation. 

## The Implementation

Below is a C++ implementation of the concepts we just explored. I have added comments to help map the text explanations directly to the code logic. 

```cpp
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxLen(int n, vector<vector<int>>& edges, string label) {
        // First we set up an adjacency list to represent the graph pathways
        vector<vector<int>> adj(n);
        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        // We calculate the maximum possible number of unique states
        // Two to the power of n bounds the bitmask
        // n bounds both the left and right nodes
        int num_states = (1 << n) * n * n;
        
        // We create a flat array to remember visited states to save time
        vector<bool> visited(num_states, false);
        
        // A small helper function to convert our 3 values into a single flat index
        auto get_idx = [&](int mask, int u, int v) {
            return (mask * n + u) * n + v;
        };

        // We define a structure to hold our current path situation
        struct State {
            int mask;
            int u;
            int v;
        };
        
        queue<State> q;
        int max_len = 1;

        // Step One Add all odd length starting centers to the queue
        for (int i = 0; i < n; i++) {
            int mask = 1 << i;
            int idx = get_idx(mask, i, i);
            visited[idx] = true;
            q.push({mask, i, i});
        }

        // Step Two Add all even length starting centers to the queue
        for (const auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            
            // They can only be a center if their letters match
            if (label[u] == label[v]) {
                int mask = (1 << u) | (1 << v);
                
                // Keep the smaller node first to avoid mirror duplicates
                int min_uv = min(u, v);
                int max_uv = max(u, v);
                int idx = get_idx(mask, min_uv, max_uv);
                
                if (!visited[idx]) {
                    visited[idx] = true;
                    q.push({mask, min_uv, max_uv});
                    max_len = max(max_len, 2);
                }
            }
        }

        // Step Three Begin the breadth first exploration
        while (!q.empty()) {
            auto current_state = q.front();
            q.pop();

            int mask = current_state.mask;
            int u = current_state.u;
            int v = current_state.v;

            // The number of set bits tells us how many distinct nodes we have
            int len = __builtin_popcount(mask);
            if (len > max_len) {
                max_len = len;
            }

            // Look at all neighbors of the current left end
            for (int nu : adj[u]) {
                // If this neighbor is already in our path we must skip it
                if ((mask >> nu) & 1) {
                    continue;
                }
                
                // Create a temporary mask with the new left neighbor added
                int mask_nu = mask | (1 << nu);
                
                // Look at all neighbors of the current right end
                for (int nv : adj[v]) {
                    // We must skip if the new right neighbor is already visited
                    // This also neatly prevents the right and left from picking the exact same node
                    if ((mask_nu >> nv) & 1) {
                        continue;
                    }  
                    
                    // The letters must match perfectly to keep the palindrome symmetrical
                    if (label[nu] != label[nv]) {
                        continue;
                    }

                    // Create the final mask containing both new ends
                    int nmask = mask_nu | (1 << nv);
                    
                    // Enforce our sorting rule to halve the remaining workload
                    int next_u = min(nu, nv);
                    int next_v = max(nu, nv);
                    
                    int idx = get_idx(nmask, next_u, next_v);
                    
                    // If this exact scenario is completely new to us we add it to the queue
                    if (!visited[idx]) {
                        visited[idx] = true;
                        q.push({nmask, next_u, next_v});
                    }
                }
            }
        }

        return max_len;
    }
};

```

## A Brief Look at Complexity

When solving puzzles like this, it is always a good idea to consider the resources the computer will use. We can evaluate this by looking at time and memory constraints.

For the time complexity, we have to consider every unique situation we process. The total number of valid bitmasks is proportional to two raised to the power of the number of nodes. For each bitmask, there are nodes squared potential combinations for the left and right endpoints. Inside our loop, we check neighbors, which is bounded by the pathways connected to each node. By recording visited states, we ensure we only process each valid scenario exactly one time. Thus, the time taken is roughly bounded by two to the power of N, multiplied by N squared, multiplied by the pathways squared. Given that N is at most fourteen, this easily runs within the execution limits!

For the space complexity, the primary memory consumer is our visited array. As discussed earlier, the array needs to hold boolean markers for two to the power of N, multiplied by N squared states. Additionally, the exploration queue will hold active states at any given moment. This memory requirement scales identically to the time complexity boundaries, making it a very practical way to solve this challenge.

## Concluding Thoughts

I really enjoy how this logic forces us to think from the inside out rather than strictly left to right. By combining bitmasking to track unique visits and a layered search algorithm to build symmetry, we break down a potentially complex graph problem into very manageable layers of logic.

I hope this thorough walkthrough brings you clarity and gives you new ideas for your own coding journey. Please feel free to experiment with the code and trace the logic with different graph shapes on paper, as that is always an amazing way to solidify these concepts! Happy programming!
