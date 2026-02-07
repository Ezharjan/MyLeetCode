#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    long long maximumValueSum(vector<vector<int>>& board) {
        int m = board.size();
        int n = board[0].size();
        
        // Step 1: Precompute the top 3 largest values and their column indices for each row.
        // Storing structure: top3[row_index] = vector of {value, col_index}
        vector<vector<pair<int, int>>> top3(m);
        
        for(int i = 0; i < m; ++i) {
            vector<pair<int, int>> row_values;
            for(int j = 0; j < n; ++j) {
                row_values.push_back({board[i][j], j});
            }
            
            // Sort to find the largest values in this row
            // O(N log N) per row, which is fast enough for N=100
            sort(row_values.rbegin(), row_values.rend());
            
            // We only keep the top 3 candidates. 
            // Why 3? Because we are placing 3 rooks total. In the worst case, 
            // the top 2 best columns for this row might be taken by the other 2 rooks,
            // but the 3rd best is guaranteed to be available relative to them.
            for(int k = 0; k < 3 && k < n; ++k) {
                top3[i].push_back(row_values[k]);
            }
        }
        
        long long max_sum = LLONG_MIN;
        
        // Step 2: Iterate through all unique triplets of rows (i < j < k)
        for(int i = 0; i < m; ++i) {
            for(int j = i + 1; j < m; ++j) {
                for(int k = j + 1; k < m; ++k) {
                    
                    // Step 3: Try all combinations of the top 3 columns for these specific rows
                    for(auto& rook1 : top3[i]) {
                        for(auto& rook2 : top3[j]) {
                            // Ensure rook 1 and rook 2 are not in the same column
                            if(rook1.second == rook2.second) continue; 
                            
                            for(auto& rook3 : top3[k]) {
                                // Ensure rook 3 is not in the same column as 1 or 2
                                if(rook3.second == rook1.second || rook3.second == rook2.second) continue;
                                
                                // Valid configuration found
                                long long current_sum = (long long)rook1.first + rook2.first + rook3.first;
                                max_sum = max(max_sum, current_sum);
                            }
                        }
                    }
                }
            }
        }
        
        return max_sum;
    }
};