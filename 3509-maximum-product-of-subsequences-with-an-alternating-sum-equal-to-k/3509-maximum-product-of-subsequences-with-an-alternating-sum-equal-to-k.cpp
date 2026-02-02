#include <vector>
#include <bitset>
#include <array>
#include <cmath>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxProduct(vector<int>& nums, int k, int limit) {
        // Offset to handle negative alternating sums.
        // Max theoretical sum range is roughly [-1800, 1800].
        const int OFFSET = 2000;
        const int MAX_SUM = 4001; 

        if (abs(k) > 1800) return -1;

        // dp[prod][parity] = Bitset of achievable sums for valid products <= limit
        // parity 0: Even length (next op +)
        // parity 1: Odd length (next op -)
        vector<array<bitset<MAX_SUM>, 2>> dp(limit + 1);

        // global_sums[parity] = Bitset of ALL achievable sums, ignoring product limit.
        // This is necessary to track subsequences that exceed the limit but might 
        // later be multiplied by 0 to become valid.
        array<bitset<MAX_SUM>, 2> global_sums;

        for (int x : nums) {
            auto next_dp = dp;
            auto next_global_sums = global_sums;

            // --- 1. Update DP Table (Valid Products <= limit) ---
            if (x == 0) {
                // If x is 0, the product becomes 0.
                // We can append 0 to ANY existing subsequence (even those > limit).
                // Appending 0 to Even length (parity 0) -> New Odd length (parity 1), Sum + 0
                // Appending 0 to Odd length (parity 1) -> New Even length (parity 0), Sum - 0
                
                // Merge all globally reachable sums into dp[0]
                next_dp[0][1] |= global_sums[0];
                next_dp[0][0] |= global_sums[1];
                
                // Start new subsequence [0]
                next_dp[0][1].set(0 + OFFSET);
            } else {
                // If x > 0, extend existing valid products
                for (int p = 0; p <= limit; ++p) {
                    long long new_prod = (long long)p * x;
                    if (new_prod > limit) break; // Since p is increasing, we can stop early
                    
                    if (dp[p][0].none() && dp[p][1].none()) continue;

                    // Extend Odd Parity (currently length 1, 3...): Next op is Subtract x
                    if (dp[p][1].any()) {
                        next_dp[new_prod][0] |= (dp[p][1] >> x);
                    }

                    // Extend Even Parity (currently length 2, 4...): Next op is Add x
                    if (dp[p][0].any()) {
                        next_dp[new_prod][1] |= (dp[p][0] << x);
                    }
                }

                // Start new subsequence [x]
                if (x <= limit) {
                    next_dp[x][1].set(x + OFFSET);
                }
            }

            // --- 2. Update Global Sums (Ignoring Limit) ---
            // This tracks reachability for potential future multiplication by 0
            
            // Extend existing global sums
            // Odd -> Even (Subtract x)
            if (global_sums[1].any()) {
                next_global_sums[0] |= (global_sums[1] >> x);
            }
            // Even -> Odd (Add x)
            if (global_sums[0].any()) {
                next_global_sums[1] |= (global_sums[0] << x);
            }
            
            // Start new subsequence [x] in global tracker
            next_global_sums[1].set(x + OFFSET);

            // Commit updates
            dp = move(next_dp);
            global_sums = move(next_global_sums);
        }

        // --- 3. Find Result ---
        int target_idx = k + OFFSET;
        for (int p = limit; p >= 0; --p) {
            if (dp[p][0].test(target_idx) || dp[p][1].test(target_idx)) {
                return p;
            }
        }

        return -1;
    }
};