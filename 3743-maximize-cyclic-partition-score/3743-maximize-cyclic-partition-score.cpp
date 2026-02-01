#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    long long maximumScore(vector<int>& nums, int k) {
        int n = nums.size();
        long long ans = 0;
        
        // Use a static-sized array to avoid heap allocation overhead.
        // Size: 2*k + padding. Max k=1000 -> 2000 picks. 2005 is safe.
        // Dimensions: [picks][balance]
        long long dp[2005][3];
        
        // Use a safe large number that won't overflow when adding nums[i]
        // Max score is approx 1000 * 10^9 = 10^12. 1e16 is safe.
        const long long INF = 1e16; 

        // We iterate over possible starting balances to handle the cyclic nature.
        // Balance 0: Net -1 (Open Min)
        // Balance 1: Net  0 (Neutral)
        // Balance 2: Net +1 (Open Max)
        for (int start_bal = 0; start_bal <= 2; ++start_bal) {
            
            // Reset DP table for this start_bal
            for(int j = 0; j <= 2 * k; ++j) {
                dp[j][0] = -INF;
                dp[j][1] = -INF;
                dp[j][2] = -INF;
            }
            
            // Base case: 0 picks, balance must equal start_bal
            dp[0][start_bal] = 0;
            
            for (int i = 0; i < n; ++i) {
                long long x = nums[i];
                
                // Optimization: Iterate j backwards.
                // This allows us to use data from the previous iteration (i-1) stored at dp[j-1]
                // without needing a temporary 'next_dp' array.
                // Max picks cannot exceed i+1 (one per element) or 2*k.
                int limit = min(2 * k, i + 1);
                
                for (int j = limit; j >= 1; --j) {
                    // Cache values from the previous 'picks' level (j-1)
                    // These represent the state before processing the current element 'x'
                    long long prev_bal0 = dp[j-1][0];
                    long long prev_bal1 = dp[j-1][1];
                    long long prev_bal2 = dp[j-1][2];
                    
                    // Current values represent "Skip current x" option (inherited from i-1 at same j)
                    long long cur0 = dp[j][0];
                    long long cur1 = dp[j][1];
                    long long cur2 = dp[j][2];

                    // Transition 1: Update Balance 0 (Net -1)
                    // Can come from: 
                    // - Skip (already in cur0)
                    // - Pick Min (-x): Previous Balance 1 -> New Balance 0
                    if (prev_bal1 > -INF) cur0 = max(cur0, prev_bal1 - x);
                    dp[j][0] = cur0;

                    // Transition 2: Update Balance 1 (Net 0)
                    // Can come from:
                    // - Skip (already in cur1)
                    // - Pick Min (-x): Previous Balance 2 -> New Balance 1
                    // - Pick Max (+x): Previous Balance 0 -> New Balance 1
                    if (prev_bal2 > -INF) cur1 = max(cur1, prev_bal2 - x);
                    if (prev_bal0 > -INF) cur1 = max(cur1, prev_bal0 + x);
                    dp[j][1] = cur1;

                    // Transition 3: Update Balance 2 (Net +1)
                    // Can come from:
                    // - Skip (already in cur2)
                    // - Pick Max (+x): Previous Balance 1 -> New Balance 2
                    if (prev_bal1 > -INF) cur2 = max(cur2, prev_bal1 + x);
                    dp[j][2] = cur2;
                }
            }
            
            // After processing all numbers, we must:
            // 1. Have returned to the starting balance (completing the cycle).
            // 2. Have picked an even number of elements (pairs of Min/Max).
            for (int j = 2; j <= 2 * k; j += 2) {
                ans = max(ans, dp[j][start_bal]);
            }
        }
        
        return ans;
    }
};