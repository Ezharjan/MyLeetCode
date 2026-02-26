class Solution {
public:
    int numberOfStableArrays(int zero, int one, int limit) {
        int MOD = 1e9 + 7;
        
        // dp[i][j][0] -> arrays with i zeros, j ones, ending in 0
        // dp[i][j][1] -> arrays with i zeros, j ones, ending in 1
        vector<vector<vector<long long>>> dp(zero + 1, vector<vector<long long>>(one + 1, vector<long long>(2, 0)));

        // Base cases: purely 0s or purely 1s up to the limit
        for (int i = 1; i <= min(zero, limit); ++i) {
            dp[i][0][0] = 1;
        }
        for (int j = 1; j <= min(one, limit); ++j) {
            dp[0][j][1] = 1;
        }

        // Fill the DP table
        for (int i = 1; i <= zero; ++i) {
            for (int j = 1; j <= one; ++j) {
                // Calculate paths ending in 0
                long long val0 = dp[i - 1][j][0] + dp[i - 1][j][1];
                if (i > limit) {
                    val0 -= dp[i - limit - 1][j][1];
                }
                // Handle negative modulo arithmetic
                dp[i][j][0] = (val0 % MOD + MOD) % MOD;

                // Calculate paths ending in 1
                long long val1 = dp[i][j - 1][0] + dp[i][j - 1][1];
                if (j > limit) {
                    val1 -= dp[i][j - limit - 1][0];
                }
                // Handle negative modulo arithmetic
                dp[i][j][1] = (val1 % MOD + MOD) % MOD;
            }
        }

        // The answer is the sum of valid states ending in 0 and 1 using all available digits
        return (dp[zero][one][0] + dp[zero][one][1]) % MOD;
    }
};