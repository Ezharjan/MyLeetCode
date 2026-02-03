#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

class Solution {
public:
    // Global string to store the digits of n
    string S;
    
    // Memoization table:
    // [index][carry][stopped_a][stopped_b]
    // index: up to ~18 (since n <= 10^15)
    // carry: 0 or 1
    // stopped_a/b: 0 (false) or 1 (true)
    long long memo[20][2][2][2];

    long long dp(int idx, int carry, bool sa, bool sb) {
        // Base Case: If we have processed all digits of n
        if (idx == S.size()) {
            // Valid pair found only if there is no remaining carry
            return carry == 0;
        }
        
        // Return cached result if available
        if (memo[idx][carry][sa][sb] != -1) {
            return memo[idx][carry][sa][sb];
        }

        long long count = 0;
        int current_digit = S[idx] - '0';

        // Iterate over possible next_carry (0 or 1)
        for (int next_carry = 0; next_carry <= 1; ++next_carry) {
            
            // The equation for the current column:
            // val_a + val_b + carry = current_digit + 10 * next_carry
            // Rearranging to find the sum required from val_a and val_b:
            int required_sum = current_digit + 10 * next_carry - carry;

            // Optimization: Two single digits (0-9) sum to at most 18. 
            // If required_sum is out of bounds [0, 18], this carry is impossible.
            if (required_sum < 0 || required_sum > 18) continue;

            // --- Determine Valid Range for val_a ---
            int min_a = 0, max_a = 9;
            if (sa) {
                // If 'a' has already stopped, it must remain 0 (leading zeros)
                min_a = 0; max_a = 0;
            } else if (idx == 0) {
                // At the LSB (ones place), 'a' cannot be 0 because a >= 1
                min_a = 1; max_a = 9;
            }
            // Otherwise, val_a can be 0 (transition to stopped) or 1-9 (continue)

            // --- Determine Valid Range for val_b ---
            int min_b = 0, max_b = 9;
            if (sb) {
                min_b = 0; max_b = 0;
            } else if (idx == 0) {
                min_b = 1; max_b = 9;
            }

            // Iterate through valid digits for 'a'
            for (int val_a = min_a; val_a <= max_a; ++val_a) {
                int val_b = required_sum - val_a;

                // Check if the calculated val_b is allowed
                if (val_b >= min_b && val_b <= max_b) {
                    
                    // Update 'stopped' states:
                    // If we were not stopped (!sa) and we picked 0, we must stop now (next_sa = true).
                    // If we were already stopped (sa), we remain stopped.
                    bool next_sa = sa || (val_a == 0);
                    bool next_sb = sb || (val_b == 0);

                    count += dp(idx + 1, next_carry, next_sa, next_sb);
                }
            }
        }

        return memo[idx][carry][sa][sb] = count;
    }

    long long countNoZeroPairs(long long n) {
        S = to_string(n);
        // Reverse S because we process from LSB (index 0) to MSB
        reverse(S.begin(), S.end());
        
        // Initialize memoization table with -1
        memset(memo, -1, sizeof(memo));
        
        return dp(0, 0, false, false);
    }
};