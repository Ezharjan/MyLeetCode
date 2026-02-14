#include <string>
#include <cstring>

using namespace std;

class Solution {
    // Memoization table: dp[remaining_length][remainder][diff_offset]
    int dp[12][25][25];
    int K;

    int solve(const string& s, int idx, int rem, int diff, bool is_bound, bool is_lead) {
        // Base case: Reached the end of the built string
        if (idx == s.length()) {
            // Valid if:
            // 1. It's not completely formed of leading zeroes (!is_lead)
            // 2. It's completely divisible by K (rem == 0)
            // 3. Count of Even digits == Count of Odd digits (diff == 10)
            return (!is_lead && rem == 0 && diff == 10) ? 1 : 0;
        }
        
        int rem_len = s.length() - idx;
        
        // Fetch memoized result if it's an unbound and strictly significant number suffix
        if (!is_bound && !is_lead && dp[rem_len][rem][diff] != -1) {
            return dp[rem_len][rem][diff];
        }

        // Determine the maximum digit we can place at this index
        int limit = is_bound ? (s[idx] - '0') : 9;
        int ans = 0;

        for (int d = 0; d <= limit; ++d) {
            bool next_bound = is_bound && (d == limit);
            bool next_lead = is_lead && (d == 0);
            
            int next_rem = (rem * 10 + d) % K;
            int next_diff = diff;
            
            // Only update digit count difference if we are placing a significant digit (not a leading zero)
            if (!next_lead) {
                if (d % 2 == 0) {
                    next_diff++; // Increment offset for even digits
                } else {
                    next_diff--; // Decrement offset for odd digits
                }
            }
            
            ans += solve(s, idx + 1, next_rem, next_diff, next_bound, next_lead);
        }

        // Memoize the independent generic suffix state
        if (!is_bound && !is_lead) {
            dp[rem_len][rem][diff] = ans;
        }

        return ans;
    }

    int countBeautiful(int n) {
        string s = to_string(n);
        // Start diff at 10 to balance range easily [-10, 10] -> [0, 20]
        return solve(s, 0, 0, 10, true, true); 
    }

public:
    int numberOfBeautifulIntegers(int low, int high, int k) {
        K = k;
        
        // We only need to wipe memory once since `dp` completely avoids memoizing 
        // bound prefixes and caches purely suffix combinations which are universally shared.
        memset(dp, -1, sizeof(dp));
        
        return countBeautiful(high) - countBeautiful(low - 1);
    }
};