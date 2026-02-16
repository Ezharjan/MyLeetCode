class Solution {
public:
    int numberOfCombinations(string num) {
        // A sequence cannot start with a leading zero
        if (num.empty() || num[0] == '0') return 0;
        
        int N = num.length();
        int MOD = 1000000007;
        
        // lcp[i][j] stores the length of the longest common prefix of num[i...] and num[j...]
        // We use `short` to conserve memory since N <= 3500
        vector<vector<short>> lcp(N + 1, vector<short>(N + 1, 0));
        for (int i = N - 1; i >= 0; --i) {
            for (int j = N - 1; j >= 0; --j) {
                if (num[i] == num[j]) {
                    lcp[i][j] = 1 + lcp[i + 1][j + 1];
                }
            }
        }
        
        // Helper lambda to compare num[i...i+l-1] and num[j...j+l-1] in O(1)
        auto isValid = [&](int i, int j, int l) {
            int common = lcp[i][j];
            if (common >= l) return true; // entirely equal prefix of length l
            return num[i + common] < num[j + common];
        };
        
        // ways[i][l]: valid sequences starting at i, where the first number has length l
        vector<vector<int>> ways(N + 1, vector<int>(N + 2, 0));
        // dp[i][l]: suffix sum of ways[i][k] for k >= l
        vector<vector<int>> dp(N + 1, vector<int>(N + 2, 0));
        
        // Process DP backwards
        for (int i = N - 1; i >= 0; --i) {
            // Numbers cannot have leading zeros
            if (num[i] == '0') continue;
            
            for (int l = N - i; l >= 1; --l) {
                if (i + l == N) {
                    // The number spans exactly to the end of the string
                    ways[i][l] = 1;
                } else {
                    int w = 0;
                    // If the next number also has length l, check if it's strictly >=
                    if (i + 2 * l <= N && isValid(i, i + l, l)) {
                        w = ways[i + l][l];
                    }
                    // Add all valid sequences where the next number is longer than l
                    w += dp[i + l][l + 1];
                    
                    // Manual modulo to avoid expensive '%' operations
                    if (w >= MOD) w -= MOD;
                    ways[i][l] = w;
                }
                
                // Update our suffix sum for length l
                int current_dp = dp[i][l + 1] + ways[i][l];
                if (current_dp >= MOD) current_dp -= MOD;
                dp[i][l] = current_dp;
            }
        }
        
        // The answer is the sum of all valid sequences starting at index 0 
        // with any length >= 1.
        return dp[0][1];
    }
};
