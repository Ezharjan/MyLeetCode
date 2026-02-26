#include <string>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

class Solution {
    int MOD = 1e9 + 7;
    int memo[105][11][2][2];
    string s;

    // Digit DP function
    int dp(int idx, int prev_digit, bool is_tight, bool is_leading_zero) {
        // Base case: we successfully built a number
        if (idx == s.length()) {
            return is_leading_zero ? 0 : 1; 
        }

        // Return memoized result if available
        if (memo[idx][prev_digit + 1][is_tight][is_leading_zero] != -1) {
            return memo[idx][prev_digit + 1][is_tight][is_leading_zero];
        }

        int limit = is_tight ? s[idx] - '0' : 9;
        long long ans = 0;

        for (int d = 0; d <= limit; d++) {
            bool next_tight = is_tight && (d == limit);
            
            if (is_leading_zero) {
                if (d == 0) {
                    // Still a leading zero, we haven't placed our first digit yet
                    ans = (ans + dp(idx + 1, -1, next_tight, true)) % MOD;
                } else {
                    // Placed the first non-zero digit
                    ans = (ans + dp(idx + 1, d, next_tight, false)) % MOD;
                }
            } else {
                // Number has started, current digit must satisfy stepping condition
                if (abs(d - prev_digit) == 1) {
                    ans = (ans + dp(idx + 1, d, next_tight, false)) % MOD;
                }
            }
        }

        return memo[idx][prev_digit + 1][is_tight][is_leading_zero] = ans;
    }

    // Computes stepping numbers in range [1, num_str]
    int solve(string num_str) {
        s = num_str;
        memset(memo, -1, sizeof(memo));
        // Start DP from index 0, no previous digit (-1), tight bound is true, and it is a leading zero
        return dp(0, -1, true, true);
    }

    // Helper to check if a single string is a stepping number
    bool isStepping(string num) {
        for (int i = 1; i < num.length(); i++) {
            if (abs(num[i] - num[i - 1]) != 1) return false;
        }
        return true;
    }

public:
    int countSteppingNumbers(string low, string high) {
        int right = solve(high);
        int left = solve(low);
        
        // Compute (right - left) modulo 10^9 + 7 cleanly to avoid negative numbers
        int ans = (right - left + MOD) % MOD;
        
        // Include `low` if it's a stepping number itself
        if (isStepping(low)) {
            ans = (ans + 1) % MOD;
        }
        
        return ans;
    }
};