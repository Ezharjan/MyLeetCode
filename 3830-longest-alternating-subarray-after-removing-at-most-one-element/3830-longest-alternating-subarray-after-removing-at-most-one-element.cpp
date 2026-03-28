#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int longestAlternating(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return n;
        
        // DP arrays stored in a ring buffer of size 3 for O(1) space.
        // All lengths are initialized to 1 (a single element is a valid subarray).
        vector<int> dp0_up(3, 1), dp0_down(3, 1), dp1_up(3, 1), dp1_down(3, 1);
        int max_len = 1;
        
        for (int i = 1; i < n; ++i) {
            int curr = i % 3;
            int prev1 = (i - 1) % 3;
            int prev2 = (i - 2 + 3) % 3;
            
            // Reset the current states for index 'i' to base case 1
            dp0_up[curr] = 1;
            dp0_down[curr] = 1;
            dp1_up[curr] = 1;
            dp1_down[curr] = 1;
            
            // 1. Calculate states for 0 removals
            if (nums[i] > nums[i-1]) {
                dp0_up[curr] = dp0_down[prev1] + 1;
            } else if (nums[i] < nums[i-1]) {
                dp0_down[curr] = dp0_up[prev1] + 1;
            }
            
            // 2. Calculate states for 1 removal (Case A: Removal was used earlier)
            if (nums[i] > nums[i-1]) {
                dp1_up[curr] = max(dp1_up[curr], dp1_down[prev1] + 1);
            } else if (nums[i] < nums[i-1]) {
                dp1_down[curr] = max(dp1_down[curr], dp1_up[prev1] + 1);
            }
            
            // 3. Calculate states for 1 removal (Case B: Removal is used right now at i-1)
            if (i >= 2) {
                if (nums[i] > nums[i-2]) {
                    dp1_up[curr] = max(dp1_up[curr], dp0_down[prev2] + 1);
                } else if (nums[i] < nums[i-2]) {
                    dp1_down[curr] = max(dp1_down[curr], dp0_up[prev2] + 1);
                }
            }
            
            // Update the global maximum length found so far
            max_len = max({max_len, dp0_up[curr], dp0_down[curr], dp1_up[curr], dp1_down[curr]});
        }
        
        return max_len;
    }
};