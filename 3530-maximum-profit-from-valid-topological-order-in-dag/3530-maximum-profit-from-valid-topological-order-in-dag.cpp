class Solution {
public:
    int maxProfit(int n, vector<vector<int>>& edges, vector<int>& score) {
        // Precalculate the prerequisite mask for each node
        vector<int> req(n, 0);
        for (const auto& edge : edges) {
            req[edge[1]] |= (1 << edge[0]);
        }
        
        // DP table initialized to -1 (indicating invalid/unreachable states)
        vector<int> dp(1 << n, -1);
        dp[0] = 0; // Base case: 0 profit for 0 nodes processed
        
        for (int mask = 1; mask < (1 << n); ++mask) {
            int pos = __builtin_popcount(mask); // 1-based processing order position
            int best = -1;
            
            int temp = mask;
            while (temp > 0) {
                // Extract the lowest set bit's index efficiently
                int i = __builtin_ctz(temp);
                temp &= temp - 1; // Clear the lowest set bit from temp
                
                // If all prerequisites of node i are satisfied in the current mask
                if ((req[i] & mask) == req[i]) {
                    int prev = mask ^ (1 << i);
                    
                    // If the state before adding node i was reachable
                    if (dp[prev] != -1) {
                        int val = dp[prev] + score[i] * pos;
                        if (val > best) {
                            best = val;
                        }
                    }
                }
            }
            dp[mask] = best;
        }
        
        // The answer is the state where all n nodes are processed
        return dp[(1 << n) - 1];
    }
};