class Solution {
public:
    vector<int> findPermutation(vector<int>& nums) {
        int n = nums.size();
        
        // dp[mask][last] stores the minimum cost to complete the permutation.
        vector<vector<int>> dp(1 << n, vector<int>(n, -1));
        
        // nxt_choice[mask][last] stores the optimal next element to pick.
        vector<vector<int>> nxt_choice(1 << n, vector<int>(n, -1));
        
        // Recursive lambda for DP
        auto solve = [&](auto& self, int mask, int last) -> int {
            // Base case: all elements have been picked, return the wrap-around cost
            if (mask == (1 << n) - 1) {
                return abs(last - nums[0]);
            }
            if (dp[mask][last] != -1) {
                return dp[mask][last];
            }
            
            int min_cost = 1e9;
            int best_nxt = -1;
            
            // Try picking every available next number
            for (int i = 0; i < n; ++i) {
                if (!(mask & (1 << i))) {
                    int cost = abs(last - nums[i]) + self(self, mask | (1 << i), i);
                    
                    // Strictly less (<) ensures we pick the lexicographically 
                    // smallest element in case of a tie, as we iterate i from 0 to n-1
                    if (cost < min_cost) {
                        min_cost = cost;
                        best_nxt = i;
                    }
                }
            }
            nxt_choice[mask][last] = best_nxt;
            return dp[mask][last] = min_cost;
        };
        
        // Start from perm[0] = 0. Mask is 1 (0th bit set), last element is 0.
        solve(solve, 1, 0);
        
        // Reconstruct the best permutation path
        vector<int> res;
        int mask = 1;
        int last = 0;
        res.push_back(0);
        
        for (int i = 1; i < n; ++i) {
            int nxt = nxt_choice[mask][last];
            res.push_back(nxt);
            mask |= (1 << nxt);
            last = nxt;
        }
        
        return res;
    }
};