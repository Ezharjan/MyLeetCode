#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
public:
    int minimumTime(vector<int>& nums1, vector<int>& nums2, int x) {
        int n = nums1.size();
        int sum1 = 0;
        int sum2 = 0;
        
        // Pairs to keep track of {nums2[i], nums1[i]}
        vector<pair<int, int>> items(n);
        for (int i = 0; i < n; ++i) {
            items[i] = {nums2[i], nums1[i]};
            sum1 += nums1[i];
            sum2 += nums2[i];
        }
        
        // Sort in ascending order based on nums2 to maximize reduction multipliers
        sort(items.begin(), items.end());
        
        // dp[j] will store the maximum sum reduction achieved using exactly j operations
        vector<int> dp(n + 1, 0);
        
        for (int i = 0; i < n; ++i) {
            int n2 = items[i].first;
            int n1 = items[i].second;
            
            // Traverse backwards to simulate the 0/1 knapsack process
            for (int j = i + 1; j >= 1; --j) {
                dp[j] = max(dp[j], dp[j - 1] + n1 + j * n2);
            }
        }
        
        // Check for the minimum time t (from 0 to n)
        for (int t = 0; t <= n; ++t) {
            int current_unreduced_sum = sum1 + t * sum2;
            if (current_unreduced_sum - dp[t] <= x) {
                return t;
            }
        }
        
        // Impossible to make sum <= x
        return -1;
    }
};