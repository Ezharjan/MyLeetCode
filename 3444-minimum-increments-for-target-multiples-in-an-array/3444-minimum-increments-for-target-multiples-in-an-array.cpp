#include <vector>
#include <numeric>
#include <algorithm>

class Solution {
public:
    int minimumIncrements(std::vector<int>& nums, std::vector<int>& target) {
        int m = target.size();
        
        // Step 1: Precompute the LCM for every subset mask of the target array
        std::vector<long long> L(1 << m, 1);
        for (int mask = 1; mask < (1 << m); ++mask) {
            long long lcm_val = 1;
            for (int i = 0; i < m; ++i) {
                if ((mask >> i) & 1) {
                    lcm_val = std::lcm(lcm_val, (long long)target[i]);
                }
            }
            L[mask] = lcm_val;
        }
        
        // Use a safe maximum value for infinity to prevent overflow issues
        const long long INF = 1000000000000000000LL;
        std::vector<long long> dp(1 << m, INF);
        dp[0] = 0;
        
        long long cost[16];
        
        // Step 2: Iterate over every number in nums to calculate DP states
        for (int x : nums) {
            std::vector<long long> next_dp = dp;
            
            // Calculate the cost to satisfy each subset with the current 'x'
            for (int mask = 1; mask < (1 << m); ++mask) {
                long long rem = x % L[mask];
                cost[mask] = (rem == 0) ? 0 : L[mask] - rem;
            }
            
            // Step 3: Iterate through existing DP states to map out transitions
            for (int pmask = 0; pmask < (1 << m); ++pmask) {
                if (dp[pmask] == INF) continue;
                
                // Get the bitmask of targets we still need to satisfy
                int rem_mask = ((1 << m) - 1) ^ pmask;
                
                // Iterate exclusively over all disjoint subsets of 'rem_mask'
                for (int sub = rem_mask; sub > 0; sub = (sub - 1) & rem_mask) {
                    if (dp[pmask] + cost[sub] < next_dp[pmask | sub]) {
                        next_dp[pmask | sub] = dp[pmask] + cost[sub];
                    }
                }
            }
            dp = std::move(next_dp);
        }
        
        // Step 4: Minimum increments needed for all elements in the target array
        return static_cast<int>(dp[(1 << m) - 1]);
    }
};