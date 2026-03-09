# Reconstructing a Two Dimensional Grid from a Graph [#3311]

Hello fellow coding enthusiasts and developers! Welcome to another deep dive into the fascinating world of algorithms and data structures. Today, I want to share my thoughts and a complete walkthrough for a very interesting graph problem. We will explore how to take a flattened, unstructured list of connections and magically rebuild it into a perfectly aligned two dimensional grid. 

Whether you are preparing for technical interviews, studying computer science, or just love solving puzzles, I hope you find this breakdown helpful and easy to follow. We will go through the problem step by step, carefully uncovering the geometric properties of grids. I will share a solid, logical approach that we can implement together in C++.

## Understanding the Core Puzzle

Imagine you have a piece of graph paper where every intersection is a node, and the lines connecting them are edges. Now, imagine someone scrambles all the nodes, erasing their coordinates, and only hands you a list telling you which node is connected to which. Your goal is to figure out the original layout of the grid.

You are given an integer representing the total number of nodes, and a list of edges where each edge connects two unique nodes. The problem guarantees that these connections can flawlessly form a two dimensional grid. Every node from zero up to the total number of nodes minus one must appear exactly once in your final grid layout.

This problem asks us to reverse engineer the shape. The difficulty lies in the fact that we do not know the dimensions of the grid. It could be a long single row, a perfectly square board, or a wide rectangle. We have to deduce the structure purely from the connections.



## The Secret Lies in the Connections

When faced with a graph problem, the first thing to look at is the degree of each node. The degree is simply the number of neighbors a node has. Because our graph is guaranteed to be a grid, the degrees of the nodes follow very strict rules based on their geometric position.

Let us think about the shape of a standard two dimensional grid. 
* A node completely inside the grid, surrounded by other nodes, will always have exactly four neighbors (top, bottom, left, and right).
* A node on the outer boundary, but not on a corner, will have exactly three neighbors.
* A node sitting exactly at the corner of the grid will only have two neighbors.

There is also a special edge case we must consider. What if the grid is just a single straight line of nodes? In a one dimensional line, the nodes at the very ends will have exactly one neighbor, while all the nodes in the middle will have exactly two neighbors.

By counting the neighbors for every single node, we can easily identify the corners. Finding a corner is our crucial first step because a corner gives us a solid starting point to anchor our entire grid.



## Step One Identifying the Anchors

Our journey begins by translating the list of edges into an adjacency list. An adjacency list is a way to store graphs where every node has a list of its direct neighbors. This makes it incredibly easy to look up connections.

Once we build our adjacency list, we will iterate through every node and check how many neighbors it has. We want to find the absolute minimum number of connections any node possesses. As we discussed, this minimum number will be either one (for a single line) or two (for a standard grid).

After finding this minimum degree, we loop through our nodes one more time and collect all the nodes that have this exact minimum degree. These nodes are our corners! We will store them safely in a list because we need to pick one to start our reconstruction.

## Step Two Walking the Border

Now that we have our corners, we can pick any single corner to act as the top left cell of our new grid. However, just knowing the top left corner is not enough. We need to figure out the entire top row.

How do we find the rest of the top row? We know that the top row stretches straight from our starting corner to another corner. In a grid graph, the shortest path between two adjacent corners always perfectly traces the boundary row. 

To find this path, we will use a classic algorithm called Breadth First Search. Breadth First Search explores a graph evenly in all directions. It is the perfect tool for finding the shortest path between two points.

Here is how we will use it
1.  We initialize a queue and push our starting corner into it.
2.  We keep track of the distance from our starting corner to every other node. We will initialize these distances with a very large dummy value like 999999 to represent unvisited nodes.
3.  We also keep a parent tracker. When we move from node A to node B, we record that B came from A. This is crucial for tracing our steps backwards later.
4.  We process nodes level by level until we have explored the entire graph.



Once the Breadth First Search finishes, we look at the distances to all the other corners. We want to find the corner that is closest to our starting corner. This closest corner will be the top right corner of our grid. The shortest path connecting the top left to the top right forms exactly our first row!

To reconstruct this row, we start at the top right corner and use our parent tracker to walk backwards step by step until we reach the starting corner. We collect these nodes, reverse the order, and boom! We have perfectly successfully reconstructed the first row of our grid.

## Step Three Cascading Downward

With the entire first row secured, the hardest part is over. Now, we can systematically build the rest of the grid row by row, moving downwards.

Think about a node in our completed first row. It has a maximum of four neighbors. We already know some of them. It might be connected to the node to its left and the node to its right. Since we are at the top, it has no neighbor above it. This means that among all its neighbors, exactly one neighbor has not been placed in the grid yet. That single unvisited neighbor must be the node sitting directly below it in the second row!

We will use this brilliant deduction to our advantage. We maintain a tracker to mark nodes as visited once they are placed into our grid. Then, we look at the row we just completed. For every node in that row, we check its neighbors in the adjacency list. We find the one neighbor that is not yet visited, and we place it exactly in the same column in a new row.

We repeat this process over and over. We take the new row, find the unvisited downward neighbors, and create the next row. We continue this downward cascade until we look at a row and find absolutely no unvisited neighbors. When that happens, it means we have reached the bottom of the grid, and our reconstruction is fully complete.

## Analyzing the Strategy

It is always good practice to understand the resource requirements of our logic. Let us talk about Time Complexity and Space Complexity in simple terms.

For the time requirement, building the adjacency list takes time proportional to the number of edges. Finding the minimum degree and collecting corners takes time proportional to the number of nodes. Running the Breadth First Search touches every node and every edge once, so it also scales with the size of the graph. Finally, building the rows downward requires looking at each node and its neighbors once. Altogether, the total time required grows strictly linearly with the number of nodes plus the number of edges. This is highly acceptable and runs comfortably within the required limits.

For the space requirement, we need memory to store the adjacency list. We also need memory for our Breadth First Search queue, the distance tracker, the parent tracker, and the visited status tracker. All of these structures take space proportional to the number of nodes or edges. Thus, the space requirement is also strictly linear, which is a very practical memory footprint.

## The C++ Implementation

Below is the C++ code that implements all the logic we just discussed. Notice how we use 999999 as a safe dummy value to represent an unvisited state, ensuring we avoid any unintended arithmetic issues. The code is structured to be readable and directly mirrors the step by step breakdown above.

```cpp
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<int>> constructGridLayout(int n, vector<vector<int>>& edges) {
        // Build the adjacency list to represent the graph
        vector<vector<int>> adj(n);
        for(auto& e : edges){
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        
        // Find the absolute minimum number of connections
        int minDeg = n;
        for(int i = 0; i < n; ++i){
            minDeg = min(minDeg, (int)adj[i].size());
        }
        
        // Collect all nodes that match this minimum connection count
        vector<int> corners;
        for(int i = 0; i < n; ++i){
            if(adj[i].size() == minDeg){
                corners.push_back(i);
            }
        }
        
        // Pick the first corner to start our search
        int c = corners[0];
        vector<int> dist(n, 999999);
        vector<int> parent(n, 999999);
        queue<int> q;
        
        q.push(c);
        dist[c] = 0;
        
        // Run Breadth First Search to map out the entire grid distances
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(int v : adj[u]){
                if(dist[v] == 999999){
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        // Find the target corner that completes the first boundary row
        int targetCorner = 999999;
        if(minDeg == 1){
            targetCorner = corners[1];
        } else {
            int minDistance = 999999;
            for(int x : corners){
                if(x != c && dist[x] < minDistance){
                    minDistance = dist[x];
                    targetCorner = x;
                }
            }
        }
        
        // Trace the path backwards using the parent tracker
        vector<int> firstRow;
        int curr = targetCorner;
        while(curr != 999999){
            firstRow.push_back(curr);
            curr = parent[curr];
        }
        
        // Reverse the gathered path to arrange it from left to right
        reverse(firstRow.begin(), firstRow.end());
        
        // Initialize our final grid structure
        vector<vector<int>> grid;
        grid.push_back(firstRow);
        
        // Keep track of which nodes have been successfully placed
        vector<bool> visited(n, false);
        for(int x : firstRow){
            visited[x] = true;
        }
        
        // Iteratively deduce the nodes for the rows below
        while(true){
            vector<int> lastRow = grid.back();
            vector<int> newRow;
            bool hasNext = false;
            
            for(int u : lastRow){
                int nextU = 999999;
                
                // Find the single unvisited neighbor pointing downward
                for(int v : adj[u]){
                    if(!visited[v]){
                        nextU = v;
                        break;
                    }
                }
                
                if(nextU != 999999){
                    hasNext = true;
                }
                newRow.push_back(nextU);
            }
            
            // If no downward neighbors exist, the grid is fully built
            if(!hasNext) break;
            
            // Mark the freshly placed nodes as visited
            for(int x : newRow){
                if(x != 999999) visited[x] = true;
            }
            
            grid.push_back(newRow);
        }
        
        return grid;
    }
};

```

## Closing Thoughts

I hope this thorough walkthrough makes the problem much more approachable! By breaking down complex structures into smaller, observable properties like node connections and shortest paths, we can demystify even the most challenging graph puzzles.

The beauty of this solution lies in how it perfectly marries basic graph traversal techniques with geometric intuition. I encourage you to try tracing the code by hand on a small piece of paper. Drawing out the queue states and the growing grid really solidifies the learning experience.

Thank you so much for reading along. Keep exploring, keep questioning, and happy coding!
