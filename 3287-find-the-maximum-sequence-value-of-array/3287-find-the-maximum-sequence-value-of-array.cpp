#include <vector>
#include <array>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxValue(vector<int>& nums, int k) {
        int n = nums.size();
        
        // L[i][v] = true if we can obtain OR sum `v` with `k` elements from nums[0..i]
        vector<array<bool, 128>> L(n);
        for (auto& arr : L) arr.fill(false);
        
        // R[i][v] = true if we can obtain OR sum `v` with `k` elements from nums[i..n-1]
        vector<array<bool, 128>> R(n);
        for (auto& arr : R) arr.fill(false);
        
        // dp[j][v] maintains whether we can form OR sum `v` using exactly `j` elements
        vector<array<bool, 128>> dp(k + 1);
        for (auto& arr : dp) arr.fill(false);
        dp[0][0] = true;
        
        // 1. Populate Prefix DP
        for (int i = 0; i < n; ++i) {
            int x = nums[i];
            // Iterate downwards to use elements exactly once (Knapsack behavior)
            for (int j = min(i + 1, k); j >= 1; --j) {
                for (int v = 0; v < 128; ++v) {
                    if (dp[j - 1][v]) {
                        dp[j][v | x] = true;
                    }
                }
            }
            // Record state for prefix ending at i
            for (int v = 0; v < 128; ++v) {
                L[i][v] = dp[k][v];
            }
        }
        
        // Reset dp array for Suffix processing
        for (auto& arr : dp) arr.fill(false);
        dp[0][0] = true;
        
        // 2. Populate Suffix DP
        for (int i = n - 1; i >= 0; --i) {
            int x = nums[i];
            for (int j = min(n - i, k); j >= 1; --j) {
                for (int v = 0; v < 128; ++v) {
                    if (dp[j - 1][v]) {
                        dp[j][v | x] = true;
                    }
                }
            }
            // Record state for suffix starting at i
            for (int v = 0; v < 128; ++v) {
                R[i][v] = dp[k][v];
            }
        }
        
        int max_val = 0;
        
        // 3. Find the maximum XOR Value across valid split points
        // Ensure there's enough room for 'k' elements on the Left and 'k' on the Right.
        for (int i = k - 1; i < n - k; ++i) {
            for (int vL = 0; vL < 128; ++vL) {
                if (L[i][vL]) {
                    for (int vR = 0; vR < 128; ++vR) {
                        if (R[i + 1][vR]) {
                            if ((vL ^ vR) > max_val) {
                                max_val = vL ^ vR;
                                // Early return if absolute max 7-bit possible XOR value is hit
                                if (max_val == 127) return 127; 
                            }
                        }
                    }
                }
            }
        }
        
        return max_val;
    }
};