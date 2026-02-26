#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    struct Cell {
        long long val;
        int r;
        int c;
    };

public:
    long long maximumValueSum(vector<vector<int>>& board) {
        int m = board.size();
        int n = board[0].size();
        
        vector<Cell> C;
        C.reserve(m * 3);
        
        // 1. Keep only the top 3 elements of each row
        for (int i = 0; i < m; ++i) {
            vector<Cell> row_cells(n);
            for (int j = 0; j < n; ++j) {
                row_cells[j] = {(long long)board[i][j], i, j};
            }
            
            // Partially sort to rapidly find the top 3 without a full sort overhead
            if (n > 3) {
                nth_element(row_cells.begin(), row_cells.begin() + 3, row_cells.end(), [](const Cell& a, const Cell& b) {
                    return a.val > b.val;
                });
            }
            
            for (int k = 0; k < 3; ++k) {
                C.push_back(row_cells[k]);
            }
        }
        
        // 2. Globally sort all viable candidates descendingly
        sort(C.begin(), C.end(), [](const Cell& a, const Cell& b) {
            return a.val > b.val;
        });
        
        // Initializing with an extremely safe floor 
        long long ans = -4000000000000000000LL; 
        int sz = C.size();
        
        // 3. Smart nested backtracking with boundary breaks
        for (int i = 0; i < sz - 2; ++i) {
            // Unsurpassable theoretical bound trigger
            if (C[i].val + C[i+1].val + C[i+2].val <= ans) {
                break;
            }
            for (int j = i + 1; j < sz - 1; ++j) {
                if (C[i].val + C[j].val + C[j+1].val <= ans) {
                    break;
                }
                
                // Attack conflict checking
                if (C[i].r == C[j].r || C[i].c == C[j].c) {
                    continue;
                }
                
                for (int k = j + 1; k < sz; ++k) {
                    if (C[i].val + C[j].val + C[k].val <= ans) {
                        break;
                    }
                    if (C[i].r == C[k].r || C[i].c == C[k].c) {
                        continue;
                    }
                    if (C[j].r == C[k].r || C[j].c == C[k].c) {
                        continue;
                    }
                    
                    // Capture valid placement maximum sum update
                    long long current_sum = C[i].val + C[j].val + C[k].val;
                    if (current_sum > ans) {
                        ans = current_sum;
                    }
                }
            }
        }
        
        return ans;
    }
};