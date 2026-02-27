#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    int minimumValueSum(vector<int>& nums, vector<int>& andValues) {
        int n = nums.size();
        int m = andValues.size();
        
        // Flatten the 2D state [i][j] into a 1D vector of unordered_maps to store masks
        vector<unordered_map<int, int>> memo(n * m);
        const int INF = 1e9 + 7;
        
        auto dfs = [&](auto& self, int i, int j, int mask) -> int {
            // Pruning: if there are fewer elements left than needed subarrays
            if (n - i < m - j) return INF; 
            
            // Base cases
            if (i == n) return j == m ? 0 : INF;
            if (j == m) return INF;
            
            int memo_idx = i * m + j;
            if (memo[memo_idx].count(mask)) {
                return memo[memo_idx][mask];
            }
            
            // If mask is -1, it means we are starting a new subarray
            int next_mask = (mask == -1) ? nums[i] : (mask & nums[i]);
            int res = INF;
            
            // Pruning: We can only continue or finish if next_mask can eventually become andValues[j]
            if ((next_mask & andValues[j]) == andValues[j]) {
                
                // Option 1: The current subarray satisfies the condition, cut it here
                if (next_mask == andValues[j]) {
                    res = min(res, nums[i] + self(self, i + 1, j + 1, -1));
                }
                
                // Option 2: Keep extending the current subarray
                res = min(res, self(self, i + 1, j, next_mask));
            }
            
            return memo[memo_idx][mask] = res;
        };
        
        int ans = dfs(dfs, 0, 0, -1);
        
        // Return -1 if it's impossible to partition
        return ans >= INF ? -1 : ans;
    }
};