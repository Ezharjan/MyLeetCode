#include <vector>

class Solution {
public:
    int superEggDrop(int k, int n) {
        // dp[i] represents the maximum number of floors we can cover with the current 
        // number of moves and 'i' eggs.
        std::vector<int> dp(k + 1, 0);
        int moves = 0;
        
        // Keep incrementing moves until we can cover at least 'n' floors with 'k' eggs
        while (dp[k] < n) {
            moves++;
            // Traverse backward to update the 1D DP array in place
            for (int i = k; i > 0; --i) {
                dp[i] = dp[i - 1] + dp[i] + 1;
            }
        }
        
        return moves;
    }
};