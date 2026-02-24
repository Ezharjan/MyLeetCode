#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int countOfPairs(vector<int>& nums) {
        int n = nums.size();
        int max_val = 0;
        for (int x : nums) {
            max_val = max(max_val, x);
        }
        
        // Use single-dimensional vectors to minimize space complexity to O(max(nums))
        vector<long long> dp(max_val + 1, 0);
        vector<long long> next_dp(max_val + 1, 0);
        vector<long long> pref(max_val + 1, 0);
        
        // Base case initialization for index i = 0
        for (int j = 0; j <= nums[0]; ++j) {
            dp[j] = 1;
        }
        
        const int MOD = 1e9 + 7;
        
        // Bottom-up DP through the array
        for (int i = 1; i < n; ++i) {
            // Compute the prefix sum of the previous DP array
            pref[0] = dp[0];
            for (int j = 1; j <= max_val; ++j) {
                pref[j] = (pref[j - 1] + dp[j]);
                if (pref[j] >= MOD) pref[j] -= MOD;
            }
            
            int d = max(0, nums[i] - nums[i - 1]);
            
            // Clear the next_dp array for the current iteration
            fill(next_dp.begin(), next_dp.end(), 0);
            
            // Build the possibilities for index i
            for (int j = d; j <= nums[i]; ++j) {
                next_dp[j] = pref[j - d];
            }
            
            // Swap is fast O(1) pointer-swap, reusing previously allocated memory space
            dp.swap(next_dp);
        }
        
        // Tally up all valid arrays concluding at the end of `nums` 
        long long ans = 0;
        for (int j = 0; j <= nums[n - 1]; ++j) {
            ans = (ans + dp[j]);
            if (ans >= MOD) ans -= MOD;
        }
        
        return ans;
    }
};