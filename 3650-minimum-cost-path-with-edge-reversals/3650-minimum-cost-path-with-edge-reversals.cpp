#include <vector>
#include <queue>
#include <tuple>
#include <climits>

using namespace std;

class Solution {
public:
    int minCost(int n, vector<vector<int>>& edges) {
        // Adjacency list: node -> vector of {neighbor, weight}
        vector<vector<pair<int, int>>> adj(n);
        
        for (const auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int w = edge[2];
            
            // 1. Normal traversal: u -> v with cost w
            adj[u].push_back({v, w});
            
            // 2. Switch traversal: 
            // The edge u->v is incoming to v. 
            // If we are at v, we can reverse it to v->u with cost 2*w.
            adj[v].push_back({u, 2 * w});
        }
        
        // Dijkstra's Algorithm
        // Min-priority queue storing {current_cost, u}
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        // Distance array to track min cost to each node
        vector<int> dist(n, INT_MAX);
        
        // Start at node 0
        dist[0] = 0;
        pq.push({0, 0});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            
            // If we found a shorter path to u previously, skip
            if (d > dist[u]) continue;
            
            // Optimization: If we reached the target, return immediately
            if (u == n - 1) return d;
            
            for (const auto& [v, weight] : adj[u]) {
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        
        // If target is unreachable
        return -1;
    }
};