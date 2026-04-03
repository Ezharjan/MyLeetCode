#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maximizeSumOfWeights(vector<vector<int>>& edges, int k) {
        int n = edges.size() + 1;
        
        // Use adjacency list to store the tree (neighbor, weight)
        vector<vector<pair<int, long long>>> adj(n);
        for (const auto& e : edges) {
            adj[e[0]].push_back({e[1], e[2]});
            adj[e[1]].push_back({e[0], e[2]});
        }
        
        // Lambda for Depth-First Search
        // Returns a pair: {dp[u][0], dp[u][1]}
        auto dfs = [&](auto self, int u, int p) -> pair<long long, long long> {
            long long base_sum = 0;
            vector<long long> diffs;
            
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                long long w = edge.second;
                
                // Don't traverse back to the parent
                if (v == p) continue;
                
                // Recursively compute DP values for the child
                auto [dp_v0, dp_v1] = self(self, v, u);
                
                base_sum += dp_v0;
                
                // Calculate the marginal gain if we include the edge (u, v)
                long long diff = dp_v1 + w - dp_v0;
                
                // We only consider keeping the edge if it increases the total sum
                if (diff > 0) {
                    diffs.push_back(diff);
                }
            }
            
            // Sort positive diffs descendingly to greedily pick the best edges
            sort(diffs.rbegin(), diffs.rend());
            
            long long dp0 = base_sum;
            long long dp1 = base_sum;
            
            // Calculate dp[u][0] -> Allowed to pick up to 'k' children edges
            for (int i = 0; i < min((int)diffs.size(), k); ++i) {
                dp0 += diffs[i];
            }
            
            // Calculate dp[u][1] -> Allowed to pick up to 'k-1' children edges
            for (int i = 0; i < min((int)diffs.size(), k - 1); ++i) {
                dp1 += diffs[i];
            }
            
            return {dp0, dp1};
        };
        
        // Final answer is the optimal tree sum starting from root 0 with up to k branches.
        return dfs(dfs, 0, -1).first;
    }
};