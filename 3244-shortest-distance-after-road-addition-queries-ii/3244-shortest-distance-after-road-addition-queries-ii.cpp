class Solution {
public:
    vector<int> shortestDistanceAfterQueries(int n, vector<vector<int>>& queries) {
        // nxt[i] stores the next active node in the shortest path
        vector<int> nxt(n);
        for (int i = 0; i < n; ++i) {
            nxt[i] = i + 1;
        }
        
        int dist = n - 1;
        vector<int> ans;
        ans.reserve(queries.size());
        
        for (const auto& q : queries) {
            int u = q[0];
            int v = q[1];
            
            // Check if this new road actually bypasses intermediate nodes
            if (nxt[u] < v) {
                int curr = nxt[u];
                
                // Traverse and remove all nodes strictly between u and v
                while (curr < v) {
                    int next_node = nxt[curr];
                    nxt[curr] = v; // Path compression: point bypassed nodes directly to v
                    curr = next_node;
                    dist--;        // The path length decreases for each bypassed node
                }
                
                nxt[u] = v;
            }
            
            ans.push_back(dist);
        }
        
        return ans;
    }
};