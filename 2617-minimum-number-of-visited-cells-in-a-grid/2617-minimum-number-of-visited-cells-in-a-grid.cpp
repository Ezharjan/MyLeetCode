#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    int minimumVisitedCells(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        // Min-heaps for each row and each column
        // Stores pair: <cells_visited, max_reachable_index>
        vector<priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>> pq_row(m);
        vector<priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>> pq_col(n);
        
        int ans = -1;

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                
                // 1. Clean up outdated reachability limits in the heaps
                while (!pq_row[i].empty() && pq_row[i].top().second < j) {
                    pq_row[i].pop();
                }
                while (!pq_col[j].empty() && pq_col[j].top().second < i) {
                    pq_col[j].pop();
                }
                
                // 2. Find minimum visited cells to reach current (i, j)
                int current_dp = -1;
                
                if (i == 0 && j == 0) {
                    current_dp = 1; // Starting cell counts as 1
                } else {
                    int min_val = 1e9;
                    if (!pq_row[i].empty()) {
                        min_val = min(min_val, pq_row[i].top().first + 1);
                    }
                    if (!pq_col[j].empty()) {
                        min_val = min(min_val, pq_col[j].top().first + 1);
                    }
                    if (min_val != 1e9) {
                        current_dp = min_val;
                    }
                }
                
                // 3. If we are at the target destination, capture the answer
                if (i == m - 1 && j == n - 1) {
                    ans = current_dp;
                }
                
                // 4. Push current cell's reachability to heaps for future cells
                if (current_dp != -1) {
                    if (grid[i][j] > 0) { // Only push if we can actually move from here
                        pq_row[i].push({current_dp, j + grid[i][j]});
                        pq_col[j].push({current_dp, i + grid[i][j]});
                    }
                }
            }
        }
        
        return ans;
    }
};