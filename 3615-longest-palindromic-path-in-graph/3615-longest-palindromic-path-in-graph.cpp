#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxLen(int n, vector<vector<int>>& edges, string label) {
        // Build adjacency list for the graph
        vector<vector<int>> adj(n);
        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        // Setup flattened 3D visited array (dynamically sized)
        // Maximum bitmask is (1 << n) - 1. So (1 << n) bounds the bitmask size.
        int num_states = (1 << n) * n * n;
        vector<bool> visited(num_states, false);
        
        // Helper lambda to get the 1D index representing a state
        auto get_idx = [&](int mask, int u, int v) {
            return (mask * n + u) * n + v;
        };

        struct State {
            int mask;
            int u;
            int v;
        };
        queue<State> q;

        int max_len = 1;

        // Base cases: Odd-length palindromes (Length 1, center is a single node)
        for (int i = 0; i < n; i++) {
            int mask = 1 << i;
            int idx = get_idx(mask, i, i);
            visited[idx] = true;
            q.push({mask, i, i});
        }

        // Base cases: Even-length palindromes (Length 2, center is an edge)
        for (const auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            if (label[u] == label[v]) {
                int mask = (1 << u) | (1 << v);
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

        // Breadth-First Search outwards expansion
        while (!q.empty()) {
            auto [mask, u, v] = q.front();
            q.pop();

            int len = __builtin_popcount(mask);
            if (len > max_len) {
                max_len = len;
            }

            for (int nu : adj[u]) {
                if ((mask >> nu) & 1) continue;         // `nu` is already visited
                
                int mask_nu = mask | (1 << nu);
                
                for (int nv : adj[v]) {
                    // Check evaluates true if `nv` is already in mask OR if `nv` is the same as `nu`
                    if ((mask_nu >> nv) & 1) continue;  
                    if (label[nu] != label[nv]) continue;

                    int nmask = mask_nu | (1 << nv);
                    
                    // Maintain ordering u <= v to halve symmetric states and avoid redundant processing
                    int next_u = min(nu, nv);
                    int next_v = max(nu, nv);
                    
                    int idx = get_idx(nmask, next_u, next_v);
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