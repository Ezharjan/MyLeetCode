#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int sumOfPowers(vector<int>& nums, int k) {
        int n = nums.size();
        int MOD = 1e9 + 7;
        
        // Sorting the array allows adjacent elements in chosen subsequence to produce the min differences
        sort(nums.begin(), nums.end());
        
        // Coordinate Compression on all possible absolute differences
        vector<int> vals;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                vals.push_back(nums[j] - nums[i]);
            }
        }
        vals.push_back(2e9); // Infinity placeholder for single-element subsequence boundaries
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        
        int num_vals = vals.size();
        
        // Precompute diff to mapped index lookups for faster inner DP loops
        vector<vector<int>> diff_idx(n, vector<int>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int p = 0; p < i; ++p) {
                diff_idx[i][p] = lower_bound(vals.begin(), vals.end(), nums[i] - nums[p]) - vals.begin();
            }
        }
        
        // dp[i][d_idx]
        vector<vector<int>> dp(n, vector<int>(num_vals, 0));
        
        // Base case: length 1
        for (int i = 0; i < n; ++i) {
            dp[i][num_vals - 1] = 1;
        }
        
        // DP Building process up to length k
        for (int len = 2; len <= k; ++len) {
            vector<vector<int>> next_dp(n, vector<int>(num_vals, 0));
            
            for (int i = 1; i < n; ++i) {
                vector<long long> temp_dp(num_vals, 0); // Accumulate without modulo per step
                for (int p = 0; p < i; ++p) {
                    int idx = diff_idx[i][p];
                    
                    for (int d_idx = 0; d_idx < idx; ++d_idx) {
                        temp_dp[d_idx] += dp[p][d_idx];
                    }
                    long long sum_rest = 0;
                    for (int d_idx = idx; d_idx < num_vals; ++d_idx) {
                        sum_rest += dp[p][d_idx];
                    }
                    temp_dp[idx] += sum_rest;
                }
                
                // Finalize modulo logic once per `i` iteration
                for (int d_idx = 0; d_idx < num_vals; ++d_idx) {
                    next_dp[i][d_idx] = temp_dp[d_idx] % MOD;
                }
            }
            dp = move(next_dp);
        }
        
        // Tally all powers out of complete subsequences 
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            for (int d_idx = 0; d_idx < num_vals - 1; ++d_idx) { // Skipping inf
                if (dp[i][d_idx] > 0) {
                    long long ways = dp[i][d_idx];
                    long long val = vals[d_idx];
                    ans = (ans + ways * (val % MOD)) % MOD;
                }
            }
        }
        
        return ans;
    }
};