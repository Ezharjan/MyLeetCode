#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findMaxPathScore(vector<vector<int>>& edges, vector<bool>& online, long long k) {
        int n = online.size();
        vector<vector<pair<int, int>>> adj(n);
        vector<int> in_degree(n, 0);
        
        int max_w = 0;
        // Build adjacency list and record in-degrees
        for (auto& e : edges) {
            adj[e[0]].push_back({e[1], e[2]});
            in_degree[e[1]]++;
            if (e[2] > max_w) {
                max_w = e[2];
            }
        }
        
        // Kahn's algorithm for topological sorting
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }
        
        vector<int> topo;
        topo.reserve(n);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo.push_back(u);
            for (auto& edge : adj[u]) {
                if (--in_degree[edge.first] == 0) {
                    q.push(edge.first);
                }
            }
        }
        
        // Binary search for the maximum possible score
        int low = 0, high = max_w, ans = -1;
        const long long INF = 1e18; // Representation of Infinity
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            
            vector<long long> dist(n, INF);
            dist[0] = 0;
            
            // Calculate shortest path respecting the current weight threshold
            for (int u : topo) {
                // Skip if unreachable or current node is offline
                if (dist[u] == INF || !online[u]) continue;
                
                for (auto& edge : adj[u]) {
                    int v = edge.first;
                    long long w = edge.second;
                    
                    // Consider edge only if it's thick enough and enters an online node
                    if (w >= mid && online[v]) {
                        if (dist[u] + w < dist[v]) {
                            dist[v] = dist[u] + w;
                        }
                    }
                }
            }
            
            // If the total target distance is <= k, try for a higher score
            if (dist[n - 1] <= k) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        return ans;
    }
};