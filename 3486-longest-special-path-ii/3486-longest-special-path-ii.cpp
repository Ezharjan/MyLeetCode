#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Solution {
    struct Edge {
        int to;
        int weight;
    };
    
    // Adjacency list for the tree
    vector<vector<Edge>> adj;
    // Reference to input nums
    vector<int> node_values;
    
    // locs[v] stores the depths where value 'v' appears in the current DFS path
    vector<int> locs[50005];
    
    // path_dists[d] stores the distance from root to the node at depth 'd'
    vector<int> path_dists;
    
    // Global results
    int max_len = -1;
    int min_nodes = 0;

public:
    vector<int> longestSpecialPath(vector<vector<int>>& edges, vector<int>& nums) {
        int n = nums.size();
        node_values = nums;
        
        // Build the graph
        adj.assign(n, {});
        for (const auto& e : edges) {
            adj[e[0]].push_back({e[1], e[2]});
            adj[e[1]].push_back({e[0], e[2]});
        }
        
        // Reset data structures
        for (int i = 0; i < 50005; ++i) {
            locs[i].clear();
        }
        path_dists.assign(n + 1, 0);
        max_len = -1;
        min_nodes = 0;
        
        // DFS state: 
        // u = current node, p = parent
        // depth = current depth (0-indexed)
        // current_dist = sum of weights from root
        // L = start depth of the valid special path window
        // rs = depth of the first element of the current repeating pair (-1 if none)
        dfs(0, -1, 0, 0, 0, -1);
        
        return {max_len, min_nodes};
    }

private:
    void dfs(int u, int p, int depth, int current_dist, int L, int rs) {
        int val = node_values[u];
        path_dists[depth] = current_dist;
        
        // Add current depth to the history of this value
        locs[val].push_back(depth);
        int sz = locs[val].size();
        
        // --- Constraint Logic ---
        // We calculate new valid L and rs for the path ending at 'u'
        
        int new_L = L;
        int new_rs = rs;
        
        // 1. Constraint: No value can appear 3 times.
        // If we have 3 occurrences, the valid window must start strictly after the 1st one.
        if (sz >= 3) {
            int second_last_occurrence = locs[val][sz - 3];
            new_L = max(new_L, second_last_occurrence + 1);
            
            // If we moved L past the start of the existing repeating pair, that pair is broken.
            if (new_rs < new_L) {
                new_rs = -1;
            }
        }
        
        // 2. Constraint: At most one value can appear twice.
        // Check if the current value 'val' forms a pair (it's the end of a pair).
        int p1 = (sz >= 2) ? locs[val][sz - 2] : -1;
        
        if (p1 >= new_L) {
            // 'val' appears at least twice within the window [new_L, depth].
            // We have a pair of 'val' starting at p1.
            
            if (new_rs != -1 && new_rs != p1) {
                // Conflict: We already have a *different* repeating pair starting at 'new_rs'.
                // Now we have a second pair starting at 'p1'.
                // We must break the pair that starts earlier to satisfy "at most one pair".
                
                if (new_rs < p1) {
                    // The old pair starts earlier. Shrink window to break it.
                    new_L = max(new_L, new_rs + 1);
                    new_rs = p1; // The new pair (val) becomes the active repeating pair.
                } else {
                    // The new pair starts earlier (nested inside the old one or similar).
                    // Shrink window to break the new pair.
                    new_L = max(new_L, p1 + 1);
                    // The old pair (new_rs) remains valid.
                }
            } else {
                // No conflict. Either no pair existed, or the existing pair was also 'val'.
                new_rs = p1;
            }
        }
        
        // Final consistency check: if L moved past rs, the repeating pair is gone.
        if (new_rs < new_L) {
            new_rs = -1;
        }
        
        // --- Update Results ---
        int path_len = current_dist - path_dists[new_L];
        int num_nodes = depth - new_L + 1;
        
        if (path_len > max_len) {
            max_len = path_len;
            min_nodes = num_nodes;
        } else if (path_len == max_len) {
            min_nodes = min(min_nodes, num_nodes);
        }
        
        // --- Recurse ---
        for (const auto& e : adj[u]) {
            if (e.to != p) {
                dfs(e.to, u, depth + 1, current_dist + e.weight, new_L, new_rs);
            }
        }
        
        // --- Backtrack ---
        locs[val].pop_back();
    }
};