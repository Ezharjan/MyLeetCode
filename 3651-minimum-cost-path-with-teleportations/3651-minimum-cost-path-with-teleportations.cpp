#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    int minCost(vector<vector<int>>& grid, int k) {
        int m = grid.size();
        int n = grid[0].size();
        // Use a safe large number for infinity.
        // Maximum possible path sum is approx 160 * 10000 = 1.6e6, so 1e9 is safe.
        const int INF = 1e9;
        
        // dist[i][j] stores the min cost to reach (i, j)
        vector<vector<int>> dist(m, vector<int>(n, INF));
        
        // Base case: Start at (0, 0) with cost 0
        dist[0][0] = 0;
        
        // Helper lambda to propagate normal grid moves (Right and Down)
        // Since movement is strictly Right/Down, simple iteration works (no Dijkstra needed for this part).
        auto propagate_moves = [&](vector<vector<int>>& d) {
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    // Skip the start cell updates in the loop to preserve 0 (or teleported value)
                    if (d[i][j] == INF && i == 0 && j == 0) continue; 
                    
                    // Move from Top
                    if (i > 0 && d[i-1][j] != INF) {
                        d[i][j] = min(d[i][j], d[i-1][j] + grid[i][j]);
                    }
                    // Move from Left
                    if (j > 0 && d[i][j-1] != INF) {
                        d[i][j] = min(d[i][j], d[i][j-1] + grid[i][j]);
                    }
                }
            }
        };

        // 1. Initial Propagation (Layer 0: No teleports used)
        propagate_moves(dist);
        
        // 2. Iterate k times, each time allowing one additional teleportation
        for (int step = 0; step < k; ++step) {
            // --- Step A: Efficiently find best teleport sources ---
            
            // min_cost_source[v] will store the minimum cost to reach ANY cell 
            // that has a grid value >= v.
            // Max grid value is 10000. Size 10002 handles index 10001 for safety.
            vector<int> min_cost_source(10002, INF);
            
            // Collect costs grouped by grid value
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dist[i][j] != INF) {
                        int val = grid[i][j];
                        min_cost_source[val] = min(min_cost_source[val], dist[i][j]);
                    }
                }
            }
            
            // Compute Suffix Minimums
            // After this, min_cost_source[v] = min cost of any node with value >= v
            for (int v = 10000; v >= 0; --v) {
                min_cost_source[v] = min(min_cost_source[v], min_cost_source[v + 1]);
            }
            
            // --- Step B: Apply Teleports and Propagate ---
            
            // Initialize next_dist with current dist (representing "at most" logic / choosing not to teleport)
            vector<vector<int>> next_dist = dist;
            
            // Try teleporting TO every cell (i, j)
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    int val = grid[i][j];
                    // To teleport TO (i, j), we need a source with value >= grid[i][j].
                    // The cost of the teleport itself is 0, so cost becomes cost of source.
                    if (min_cost_source[val] != INF) {
                        next_dist[i][j] = min(next_dist[i][j], min_cost_source[val]);
                    }
                }
            }
            
            // After landing via teleport, we can walk normally again
            propagate_moves(next_dist);
            
            // Update state for next iteration
            dist = next_dist;
        }
        
        return dist[m-1][n-1];
    }
};