#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maximumScore(vector<vector<int>>& grid) {
        int n = grid.size();
        
        // pref[c][r] stores the sum of grid[0...r-1][c]
        vector<vector<long long>> pref(n, vector<long long>(n + 1, 0));
        for (int c = 0; c < n; ++c) {
            for (int r = 0; r < n; ++r) {
                pref[c][r + 1] = pref[c][r] + grid[r][c];
            }
        }
        
        // dp[j][k] = max score given h_c = j and h_{c+1} = k
        vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
        
        // Base case initialization for column 0
        for (int j = 0; j <= n; ++j) {
            for (int k = 0; k <= n; ++k) {
                // For column 0, h_{-1} is effectively 0
                dp[j][k] = max(0LL, pref[0][k] - pref[0][j]);
            }
        }
        
        // Traverse columns from 1 to n - 1
        for (int c = 1; c < n; ++c) {
            vector<vector<long long>> next_dp(n + 1, vector<long long>(n + 1, 0));
            
            for (int j = 0; j <= n; ++j) {
                vector<long long> pref_max(n + 1, 0);
                vector<long long> suff_max(n + 2, -1e18);
                
                // Track highest score configurations for previous constraints
                long long current_pref_max = -1e18;
                for (int i = 0; i <= n; ++i) {
                    current_pref_max = max(current_pref_max, dp[i][j]);
                    pref_max[i] = current_pref_max;
                }
                
                long long current_suff_max = -1e18;
                for (int i = n; i >= 0; --i) {
                    current_suff_max = max(current_suff_max, dp[i][j] + max(0LL, pref[c][i] - pref[c][j]));
                    suff_max[i] = current_suff_max;
                }
                
                for (int k = 0; k <= n; ++k) {
                    // For the last column, height n (represented by k here) is effectively 0 bounded
                    if (c == n - 1 && k > 0) continue; 
                    
                    // Branch 1: the neighbor height bounded by right-side height k
                    long long val1 = pref_max[k] + max(0LL, pref[c][k] - pref[c][j]);
                    // Branch 2: the neighbor height bounded by left-side height i
                    long long val2 = suff_max[k + 1];
                    
                    next_dp[j][k] = max(val1, val2);
                }
            }
            dp = next_dp;
        }
        
        // Calculate the maximum outcome mapping off the last column logic configurations
        long long ans = 0;
        for (int j = 0; j <= n; ++j) {
            ans = max(ans, dp[j][0]);
        }
        
        return ans;
    }
};