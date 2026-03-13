# Finding the Best Path: A Friendly Guide to Network Recovery Pathways [#3620]

Hello fellow developers and algorithm enthusiasts! 

Today I am very excited to share my thoughts and a detailed walkthrough for a fascinating graph problem. Whether you are practicing for interviews or just love solving intricate puzzles, this problem offers a beautiful combination of different algorithmic concepts. I want to keep things friendly and approachable, so grab your favorite beverage, get comfortable, and let us dive into the world of network routing!

### Understanding the Problem Landscape

Imagine you are a network engineer managing a vast infrastructure of computer servers. These servers are represented as nodes in a network, numbered from zero up to n minus 1. Data travels between these servers through one way connections, which we can think of as directed edges. 

Each connection has a specific recovery cost associated with it. However, in the real world, things do not always work perfectly. Some servers might be offline due to maintenance or power failures. You are given a boolean array that tells you exactly which servers are currently online and which are offline. The good news is that your starting point (node zero) and your destination (node n minus 1) are guaranteed to be online.

Your mission is to find a valid communication path from the starting server to the destination server. But there are rules to what makes a path valid

* Every single intermediate server on your chosen path must be online.
* The total recovery cost of all the connections you use on this path must not exceed a given budget, which we call k.

If you can find valid paths, you need to evaluate them based on a specific score. The score of a valid path is defined as the minimum edge cost along that entire path. Your ultimate goal is to return the maximum possible score among all the valid paths. If it is impossible to reach the destination within the budget or due to offline servers, you simply return negative one.

It is also crucial to note that the network is a Directed Acyclic Graph. This means all connections flow in one direction and there are absolutely no loops or cycles in the network.  This specific property is a massive advantage for us, as it simplifies how we traverse the network.

### Breaking Down the Strategy

When faced with a problem that asks us to maximize a minimum value or minimize a maximum value, a specific technique should immediately spring to mind. That technique is Binary Search. 

Why does Binary Search work here? Let us think about the scoring system. If we can successfully find a path where every edge has a cost of at least 50, then we can certainly find a path where every edge has a cost of at least 40, 30, or 20. On the flip side, if it is impossible to find a path where the minimum edge cost is 60, it will also be impossible for 70 or 80. This creates a clear boundary between possible and impossible scores. 

So, our overall plan is to guess a score using Binary Search, and then verify if that score is achievable.

To verify if a guessed score is achievable, we need to find the shortest path from the start to the destination using only edges that have a cost greater than or equal to our guessed score. Furthermore, we must strictly avoid any offline nodes. Since our network is a Directed Acyclic Graph, we do not need complex shortest path algorithms like Dijkstra. Instead, we can use a technique called Topological Sorting combined with Dynamic Programming.

### Step One Setting Up the Network

First things first, we need to organize our raw data into a usable format. We will create an adjacency list to represent the network. For each server, we will store a list of its outgoing connections along with their respective costs. 

While we are building this network representation, we will also do two important things
* We will keep track of the indegree for every server. The indegree is simply the count of how many connections are pointing directly to a server. We need this for our topological sort later.
* We will find the absolute maximum edge cost present in the entire network. This maximum value will serve as the upper ceiling for our Binary Search.

### Step Two Organizing with Topological Sort

Now we need to determine the correct order to process our servers. In a Directed Acyclic Graph, a topological sort gives us a linear ordering of the nodes such that for every directed edge from server A to server B, server A comes before server B in the ordering. 

We will use Kahn algorithm to achieve this. Here is the process

* We gather all servers that have an indegree of zero meaning no connections point to them. We place these in a queue.
* We take a server from the front of the queue and add it to our sorted list.
* We then look at all the servers that this current server points to, and we reduce their indegree count by one. This simulates removing the current server from the network.
* If any of those neighboring servers reach an indegree of zero, we add them to our queue.
* We repeat this until the queue is empty.

### Step Three The Binary Search Engine

With our network organized, we set up our search boundaries. Our lowest possible guess is zero, and our highest possible guess is the maximum edge cost we found in step one. 

We will calculate a midpoint between our low and high boundaries. This midpoint is our current candidate score. We then pass this candidate score to a validation function. 

* If the validation function says Yes it is possible to reach the destination with this score under budget, we record this score as our best answer so far. We then move our lower boundary up to search for an even better score.
* If the validation function says No it is impossible, we know we guessed too high. We move our upper boundary down.

### Step Four Validating the Candidate Score

This is where the magic happens. We need to check if a valid path exists where every edge cost is at least our candidate score, and the total cost is within our budget k.

We create an array to store the minimum distance from the start to every other server. We initialize all distances to a very large number representing infinity, except for the starting server which gets a distance of zero.

We then iterate through our servers exactly in the order determined by our topological sort. For the current server we are looking at

* If its current shortest distance is infinity, it means we cannot even reach this server yet, so we skip it.
* If this server is currently marked as offline, we cannot route traffic through it, so we skip it.
* If it is reachable and online, we look at all its outgoing connections.

For each outgoing connection to a neighboring server

* We first check if the neighboring server is online.
* We then check if the cost of this connection is greater than or equal to our candidate score.
* If both conditions are met, we see if taking this connection offers a cheaper total path to the neighbor than what we have recorded so far. If it does, we update the neighbor minimum distance.

After evaluating all servers in the topological order, we simply look at the final calculated distance to our destination server. If this distance is less than or equal to our budget k, then the candidate score is perfectly valid!

### The C++ Implementation

Here is the code written in C++ keeping things clear and straightforward.

```cpp
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    int findMaxPathScore(vector<vector<int>>& edges, vector<bool>& online, long long k) {
        int numberOfNodes = online.size();
        vector<vector<pair<int, int>>> adjacencyList(numberOfNodes);
        vector<int> incomingEdgeCount(numberOfNodes, 0);
        
        int highestEdgeCost = 0;
        
        for (auto& connection : edges) {
            int source = connection[0];
            int destination = connection[1];
            int cost = connection[2];
            
            adjacencyList[source].push_back({destination, cost});
            incomingEdgeCount[destination]++;
            
            if (cost > highestEdgeCost) {
                highestEdgeCost = cost;
            }
        }
        
        queue<int> processingQueue;
        for (int i = 0; i < numberOfNodes; ++i) {
            if (incomingEdgeCount[i] == 0) {
                processingQueue.push(i);
            }
        }
        
        vector<int> topologicalOrder;
        topologicalOrder.reserve(numberOfNodes);
        
        while (!processingQueue.empty()) {
            int currentNode = processingQueue.front();
            processingQueue.pop();
            topologicalOrder.push_back(currentNode);
            
            for (auto& neighborEdge : adjacencyList[currentNode]) {
                int neighborNode = neighborEdge.first;
                incomingEdgeCount[neighborNode]--;
                if (incomingEdgeCount[neighborNode] == 0) {
                    processingQueue.push(neighborNode);
                }
            }
        }
        
        int searchLow = 0;
        int searchHigh = highestEdgeCost;
        int bestScoreFound = -1;
        const long long maxInfinity = 1e18;
        
        while (searchLow <= searchHigh) {
            int candidateScore = searchLow + (searchHigh - searchLow) / 2;
            
            vector<long long> minimumDistance(numberOfNodes, maxInfinity);
            minimumDistance[0] = 0;
            
            for (int currentNode : topologicalOrder) {
                if (minimumDistance[currentNode] == maxInfinity || !online[currentNode]) {
                    continue;
                }
                
                for (auto& neighborEdge : adjacencyList[currentNode]) {
                    int neighborNode = neighborEdge.first;
                    long long edgeCost = neighborEdge.second;
                    
                    if (edgeCost >= candidateScore && online[neighborNode]) {
                        if (minimumDistance[currentNode] + edgeCost < minimumDistance[neighborNode]) {
                            minimumDistance[neighborNode] = minimumDistance[currentNode] + edgeCost;
                        }
                    }
                }
            }
            
            if (minimumDistance[numberOfNodes - 1] <= k) {
                bestScoreFound = candidateScore;
                searchLow = candidateScore + 1;
            } else {
                searchHigh = candidateScore - 1;
            }
        }
        
        return bestScoreFound;
    }
};

```

### Analyzing the Complexity

Let us evaluate the resources our code requires to run. We will use V to represent the number of vertices or servers and E to represent the number of edges or connections.

**Time Complexity**
Building the adjacency list and calculating the indegrees takes time proportional to the number of edges, which is O(E). Running the topological sort visits every vertex and every edge exactly once, adding O(V + E) to our time.
The Binary Search will execute a certain number of times based on the maximum edge cost. Let us call the maximum edge cost M. The number of search steps is proportional to the base two logarithm of M.
Inside the Binary Search loop, our validation function iterates through the topological order, evaluating edges. This validation process takes O(V + E) time.
Therefore, the total time complexity is bounded by O(V + E) multiplied by the logarithm of M. This is highly acceptable and will run very smoothly for the given problem constraints.

**Space Complexity**
We need memory to store the adjacency list which takes O(V + E) space. The arrays for indegrees, topological ordering, and the distance array during validation each require O(V) space. The queue used during the topological sort can hold at most V elements.
Adding this all together, our total space complexity scales linearly with the size of the graph, which is simply O(V + E).

### Final Thoughts

This problem is a fantastic exercise in breaking down a complex requirement into manageable known techniques. By recognizing the monotonic nature of the scoring system we successfully applied Binary Search. By leveraging the Directed Acyclic Graph properties we efficiently calculated shortest paths using a topological order.

I hope this thorough walkthrough helps clarify the inner workings of this algorithm. Keep practicing, keep analyzing, and enjoy the journey of learning!
