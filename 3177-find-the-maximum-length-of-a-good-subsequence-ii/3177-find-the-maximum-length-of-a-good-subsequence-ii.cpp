class Solution {
public:
    int maximumLength(vector<int>& nums, int k) {
        // Coordinate compression to map large integers to consecutive IDs [0, unique_count)
        unordered_map<int, int> val_to_idx;
        int idx = 0;
        for (int v : nums) {
            if (val_to_idx.find(v) == val_to_idx.end()) {
                val_to_idx[v] = idx++;
            }
        }
        
        // dp[v_idx][j] : max length of a good subsequence ending with value ID v_idx 
        // using at most j transitions
        vector<vector<int>> dp(idx, vector<int>(k + 1, 0));
        
        // max_all[j] : max length of a good subsequence ending in ANY value 
        // using at most j transitions
        vector<int> max_all(k + 1, 0);
        
        for (int v : nums) {
            int v_idx = val_to_idx[v];
            
            // Loop backwards to use values from the previous state safely
            for (int j = k; j >= 0; --j) {
                // Option 1: Append to a subsequence ending with the same value (no new transition)
                int cand1 = dp[v_idx][j] + 1;
                
                // Option 2: Append to the best subsequence ending in any value (adds 1 transition)
                // If j == 0, we can start a fresh sequence of length 1
                int cand2 = (j > 0) ? (max_all[j - 1] + 1) : 1;
                
                // Take the best choice
                int new_val = max(cand1, cand2);
                
                // Update states
                dp[v_idx][j] = new_val;
                max_all[j] = max(max_all[j], new_val);
            }
        }
        
        // The answer will always accumulate into max_all[k] since taking fewer 
        // than k transitions is also absorbed into larger j counts over time.
        return max_all[k];
    }
};