class Solution {
public:
    double champagneTower(int poured, int query_row, int query_glass) {
        // dp array to hold the champagne flowing through the current row.
        // Size is 101 to safely accommodate up to the 100th row.
        vector<double> dp(101, 0.0);
        
        // Pour all champagne into the very first glass at the top
        dp[0] = poured;
        
        // Simulate row by row down to the query_row
        for (int i = 0; i < query_row; ++i) {
            // Traverse from right to left to update the array in-place
            for (int j = i; j >= 0; --j) {
                if (dp[j] > 1.0) {
                    double excess = dp[j] - 1.0;
                    // Distribute the excess equally to the two glasses below
                    dp[j + 1] += excess / 2.0; 
                    dp[j] = excess / 2.0;       
                } else {
                    // No excess champagne falls through
                    dp[j] = 0.0;
                }
            }
        }
        
        // A glass cannot be more than completely full (1.0)
        return min(1.0, dp[query_glass]);
    }
};