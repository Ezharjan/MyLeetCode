#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    int max_len;
    int min_nodes;
    vector<vector<pair<int, int>>> adj;
    vector<int> path_dists;
    vector<int> last_seen;
    
    void dfs(int u, int p, int depth, int dist, int start_depth, const vector<int>& nums) {
        int val = nums[u];
        int prev_d = last_seen[val];
        
        // The start of our continuous unique downward segment ending at `u` must be past the last duplication
        int new_start = max(start_depth, prev_d + 1);
        
        // Cache distance from root for the current depth
        path_dists[depth] = dist;
        
        int current_len = dist - path_dists[new_start];
        int current_nodes = depth - new_start + 1;
        
        // Try resolving new global constraints
        if (current_len > max_len) {
            max_len = current_len;
            min_nodes = current_nodes;
        } else if (current_len == max_len) {
            if (current_nodes < min_nodes) {
                min_nodes = current_nodes;
            }
        }
        
        // Update backtracking state
        int old_last_seen = last_seen[val];
        last_seen[val] = depth;
        
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (v != p) {
                dfs(v, u, depth + 1, dist + weight, new_start, nums);
            }
        }
        
        // Restore previous value upon finishing the current recursion branch (backtracking step)
        last_seen[val] = old_last_seen;
    }
    
public:
    vector<int> longestSpecialPath(vector<vector<int>>& edges, vector<int>& nums) {
        int n = nums.size();
        adj.assign(n, vector<pair<int, int>>());
        
        // Construct the undirected tree
        for (auto& edge : edges) {
            adj[edge[0]].push_back({edge[1], edge[2]});
            adj[edge[1]].push_back({edge[0], edge[2]});
        }
        
        path_dists.assign(n, 0);
        
        // Dynamically find max to accurately initialize lookup map
        int max_val = 0;
        for (int x : nums) {
            if (x > max_val) max_val = x;
        }
        last_seen.assign(max_val + 1, -1);
        
        max_len = -1;
        min_nodes = 1e9;
        
        // Process starting from root 0:
        // parameters are (current_node, parent_node, curr_depth, prefix_dist, max_start_depth, numbers_ref)
        dfs(0, -1, 0, 0, 0, nums);
        
        return {max_len, min_nodes};
    }
};