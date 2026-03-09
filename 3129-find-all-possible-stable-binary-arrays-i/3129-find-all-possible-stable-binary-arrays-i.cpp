#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int numberOfStableArrays(int zero, int one, int limit) {
        // dp[i][j][k]
        // i: count of 0s
        // j: count of 1s
        // k: ends with (0 or 1)
        vector<vector<vector<long long>>> dp(zero + 1, vector<vector<long long>>(one + 1, vector<long long>(2, 0)));
        long long MOD = 1e9 + 7;
        
        // Base cases
        for (int i = 1; i <= min(zero, limit); ++i) {
            dp[i][0][0] = 1;
        }
        for (int j = 1; j <= min(one, limit); ++j) {
            dp[0][j][1] = 1;
        }
        
        // Fill DP table
        for (int i = 1; i <= zero; ++i) {
            for (int j = 1; j <= one; ++j) {
                // Calculate arrays ending in 0
                dp[i][j][0] = (dp[i-1][j][0] + dp[i-1][j][1]) % MOD;
                if (i > limit) {
                    // Subtract configurations where we have `limit + 1` consecutive 0s
                    dp[i][j][0] = (dp[i][j][0] - dp[i - 1 - limit][j][1] + MOD) % MOD;
                }
                
                // Calculate arrays ending in 1
                dp[i][j][1] = (dp[i][j-1][0] + dp[i][j-1][1]) % MOD;
                if (j > limit) {
                    // Subtract configurations where we have `limit + 1` consecutive 1s
                    dp[i][j][1] = (dp[i][j][1] - dp[i][j - 1 - limit][0] + MOD) % MOD;
                }
            }
        }
        
        // The answer is the total valid arrangements reaching `zero` 0s and `one` 1s ending in either 0 or 1.
        return (dp[zero][one][0] + dp[zero][one][1]) % MOD;
    }
};