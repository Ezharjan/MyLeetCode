class Solution {
public:
    long long countSubstrings(string s) {
        // Precompute state transitions to avoid repeatedly doing math and mod operations
        int trans[10][10][10];
        for (int d = 1; d <= 9; ++d) {
            for (int r = 0; r < d; ++r) {
                for (int c = 0; c <= 9; ++c) {
                    trans[d][r][c] = (r * 10 + c) % d;
                }
            }
        }
        
        long long ans = 0;
        // dp[parity][divisor][remainder]
        long long dp[2][10][10] = {0};
        int cur = 0;
        
        for (char ch : s) {
            int c = ch - '0';
            int nxt = cur ^ 1;
            
            // Clear the tracking array for the next character's computations
            for (int d = 1; d <= 9; ++d) {
                for (int r = 0; r < d; ++r) {
                    dp[nxt][d][r] = 0;
                }
            }
            
            // Update the remainder states for all non-zero divisors
            for (int d = 1; d <= 9; ++d) {
                for (int r = 0; r < d; ++r) {
                    long long cnt = dp[cur][d][r];
                    if (cnt > 0) {
                        dp[nxt][d][trans[d][r][c]] += cnt;
                    }
                }
                // Account for the standalone single-digit substring `c`
                dp[nxt][d][trans[d][0][c]]++;
            }
            
            // If the current ending digit is not 0, add all substrings mapped to a 0 remainder
            if (c != 0) {
                ans += dp[nxt][c][0];
            }
            
            // Ping-pong the DP arrays
            cur = nxt;
        }
        
        return ans;
    }
};