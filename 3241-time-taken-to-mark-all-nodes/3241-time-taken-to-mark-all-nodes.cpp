class Solution {
public:
    vector<int> timeTaken(vector<vector<int>>& edges) {
        int n = edges.size() + 1;
        vector<vector<int>> adj(n);
        
        // Build the tree
        for (const auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        
        // DP Arrays
        vector<int> down1(n, 0);   // Max distance down into the subtree
        vector<int> down2(n, 0);   // Second max distance down into the subtree
        vector<int> child1(n, -1); // Child that gives the max distance
        vector<int> up(n, 0);      // Max distance up outside the subtree
        
        // First DFS: Compute down1, down2, and child1 for each node
        auto dfs1 = [&](auto& self, int u, int p) -> void {
            for (int v : adj[u]) {
                if (v == p) continue;
                self(self, v, u);
                
                // Weight is 1 if target node is odd, 2 if even
                int w = (v % 2 == 1) ? 1 : 2;
                int d = down1[v] + w;
                
                if (d > down1[u]) {
                    down2[u] = down1[u];
                    down1[u] = d;
                    child1[u] = v;
                } else if (d > down2[u]) {
                    down2[u] = d;
                }
            }
        };
        
        // Second DFS: Compute the up array
        auto dfs2 = [&](auto& self, int u, int p) -> void {
            for (int v : adj[u]) {
                if (v == p) continue;
                
                // Weight to travel up to the parent `u`
                int w = (u % 2 == 1) ? 1 : 2;
                
                // If the longest path from `u` went through `v`, use the second longest
                int val_u = max(up[u], (child1[u] == v) ? down2[u] : down1[u]);
                
                up[v] = val_u + w;
                self(self, v, u);
            }
        };
        
        dfs1(dfs1, 0, -1);
        dfs2(dfs2, 0, -1);
        
        vector<int> ans(n);
        for (int i = 0; i < n; i++) {
            ans[i] = max(down1[i], up[i]);
        }
        
        return ans;
    }
};