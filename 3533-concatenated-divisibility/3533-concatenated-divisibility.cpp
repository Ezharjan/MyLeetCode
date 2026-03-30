#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> concatenatedDivisibility(vector<int>& nums, int k) {
        int n = nums.size();
        
        // Sorting initially guarantees the first valid path found 
        // will be the lexicographically smallest permutation.
        sort(nums.begin(), nums.end());
        
        // Precompute the multiplier (10^length % k) for each number
        vector<int> mult(n);
        for (int i = 0; i < n; ++i) {
            int m = 1;
            int temp = nums[i];
            while (temp > 0) {
                m = (m * 10) % k;
                temp /= 10;
            }
            mult[i] = m;
        }
        
        // Memoization table: memo[mask][rem]
        // 0 = unvisited, 1 = true (can reach 0), -1 = false (cannot reach 0)
        vector<vector<int>> memo(1 << n, vector<int>(k, 0));
        vector<int> ans;
        
        // Lambda for recursive Depth-First Search
        auto dfs = [&](auto& self, int mask, int rem) -> bool {
            // Base case: all elements are used
            if (mask == ((1 << n) - 1)) {
                return rem == 0;
            }
            
            // Return memoized result if already computed
            if (memo[mask][rem] != 0) {
                return memo[mask][rem] == 1;
            }
            
            // Try picking every available number
            for (int i = 0; i < n; ++i) {
                // If the i-th number hasn't been used yet
                if ((mask & (1 << i)) == 0) {
                    
                    // Pruning: Skip duplicate elements to avoid redundant permutation branches
                    // If nums[i] == nums[i-1], only use nums[i] if nums[i-1] has already been used
                    if (i > 0 && nums[i] == nums[i - 1] && ((mask & (1 << (i - 1))) == 0)) {
                        continue;
                    }
                    
                    int next_rem = (rem * mult[i] + nums[i]) % k;
                    ans.push_back(nums[i]);
                    
                    // Proceed to next state
                    if (self(self, mask | (1 << i), next_rem)) {
                        memo[mask][rem] = 1;
                        return true; 
                    }
                    
                    // Backtrack
                    ans.pop_back();
                }
            }
            
            // Mark state as a dead-end
            memo[mask][rem] = -1;
            return false;
        };
        
        // Start recursion with mask = 0, rem = 0
        if (dfs(dfs, 0, 0)) {
            return ans;
        }
        
        // Return empty array if no valid concatenation is found
        return {};
    }
};