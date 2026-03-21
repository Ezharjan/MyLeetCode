#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    vector<int> countVisitedNodes(vector<int>& edges) {
        int n = edges.size();
        vector<int> in_degree(n, 0);
        
        // Calculate the in-degree for each node
        for (int v : edges) {
            in_degree[v]++;
        }
        
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }
        
        // Topo_order stores the tree nodes leading to cycles 
        vector<int> topo_order;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo_order.push_back(u);
            int v = edges[u];
            if (--in_degree[v] == 0) {
                q.push(v);
            }
        }
        
        vector<int> ans(n, 0);
        
        // Find cycles and their lengths
        for (int i = 0; i < n; ++i) {
            // If in_degree[i] > 0, it is part of a cycle
            if (in_degree[i] > 0 && ans[i] == 0) {
                int curr = i;
                vector<int> cycle;
                
                // Traverse the cycle
                while (ans[curr] == 0) {
                    cycle.push_back(curr);
                    ans[curr] = -1; // Mark temporarily to avoid infinite loops
                    curr = edges[curr];
                }
                
                // Every node in the cycle will visit `len` nodes
                int len = cycle.size();
                for (int u : cycle) {
                    ans[u] = len;
                }
            }
        }
        
        // Process the non-cycle nodes backwards to dynamically compute distances
        for (int i = topo_order.size() - 1; i >= 0; --i) {
            int u = topo_order[i];
            ans[u] = ans[edges[u]] + 1;
        }
        
        return ans;
    }
};