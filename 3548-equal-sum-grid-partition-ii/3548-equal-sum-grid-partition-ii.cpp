#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    bool canPartitionGrid(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        
        int MAX_VAL = 100000;
        vector<int> min_row(MAX_VAL + 1, 1e9);
        vector<int> max_row(MAX_VAL + 1, -1);
        vector<int> min_col(MAX_VAL + 1, 1e9);
        vector<int> max_col(MAX_VAL + 1, -1);
        
        long long total_sum = 0;
        vector<long long> row_sum(m, 0);
        vector<long long> col_sum(n, 0);
        
        // Precompute sums and coordinate bounding boxes for all values
        for(int i = 0; i < m; ++i) {
            for(int j = 0; j < n; ++j) {
                int val = grid[i][j];
                total_sum += val;
                row_sum[i] += val;
                col_sum[j] += val;
                if(val <= MAX_VAL) {
                    if(i < min_row[val]) min_row[val] = i;
                    if(i > max_row[val]) max_row[val] = i;
                    if(j < min_col[val]) min_col[val] = j;
                    if(j > max_col[val]) max_col[val] = j;
                }
            }
        }
        
        // 1. Evaluate all Horizontal cuts
        long long current_top_sum = 0;
        for(int k = 0; k < m - 1; ++k) {
            current_top_sum += row_sum[k];
            long long S_top = current_top_sum;
            long long S_bot = total_sum - S_top;
            
            if(S_top == S_bot) return true; // Valid cut with no discounts
            
            if(S_top > S_bot) {
                long long diff = S_top - S_bot;
                if(diff <= MAX_VAL) {
                    if(k == 0) { // Top section is 1 x n
                        if(grid[0][0] == diff || grid[0][n-1] == diff) return true;
                    } else {     // Top section is >= 2 x n
                        if(n == 1) { // Specifically >= 2 x 1
                            if(grid[0][0] == diff || grid[k][0] == diff) return true;
                        } else {     // Specifically >= 2 x >= 2
                            if(min_row[diff] <= k) return true;
                        }
                    }
                }
            } else {
                long long diff = S_bot - S_top;
                if(diff <= MAX_VAL) {
                    if(k == m - 2) { // Bottom section is 1 x n
                        if(grid[m-1][0] == diff || grid[m-1][n-1] == diff) return true;
                    } else {         // Bottom section is >= 2 x n
                        if(n == 1) { // Specifically >= 2 x 1
                            if(grid[k+1][0] == diff || grid[m-1][0] == diff) return true;
                        } else {     // Specifically >= 2 x >= 2
                            if(max_row[diff] >= k + 1) return true;
                        }
                    }
                }
            }
        }
        
        // 2. Evaluate all Vertical cuts
        long long current_left_sum = 0;
        for(int k = 0; k < n - 1; ++k) {
            current_left_sum += col_sum[k];
            long long S_left = current_left_sum;
            long long S_right = total_sum - S_left;
            
            if(S_left == S_right) return true; // Valid cut with no discounts
            
            if(S_left > S_right) {
                long long diff = S_left - S_right;
                if(diff <= MAX_VAL) {
                    if(k == 0) { // Left section is m x 1
                        if(grid[0][0] == diff || grid[m-1][0] == diff) return true;
                    } else {     // Left section is m x >= 2
                        if(m == 1) { // Specifically 1 x >= 2
                            if(grid[0][0] == diff || grid[0][k] == diff) return true;
                        } else {     // Specifically >= 2 x >= 2
                            if(min_col[diff] <= k) return true;
                        }
                    }
                }
            } else {
                long long diff = S_right - S_left;
                if(diff <= MAX_VAL) {
                    if(k == n - 2) { // Right section is m x 1
                        if(grid[0][n-1] == diff || grid[m-1][n-1] == diff) return true;
                    } else {         // Right section is m x >= 2
                        if(m == 1) { // Specifically 1 x >= 2
                            if(grid[0][k+1] == diff || grid[0][n-1] == diff) return true;
                        } else {     // Specifically >= 2 x >= 2
                            if(max_col[diff] >= k + 1) return true;
                        }
                    }
                }
            }
        }
        
        return false;
    }
};