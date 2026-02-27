#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long minOperations(vector<int>& nums, int x, int k) {
        int n = nums.size();
        
        // C[i] will store the cost to make elements in nums[i ... i+x-1] equal
        vector<long long> C(n - x + 1);
        multiset<int> low, high;
        long long sum_low = 0, sum_high = 0;

        // Helper lambda to add an element into our balanced sets
        auto add = [&](int val) {
            if (low.empty() || val <= *low.rbegin()) {
                low.insert(val);
                sum_low += val;
            } else {
                high.insert(val);
                sum_high += val;
            }
        };

        // Helper lambda to remove an element falling out of the sliding window
        auto remove = [&](int val) {
            auto it = high.find(val);
            if (it != high.end()) {
                high.erase(it);
                sum_high -= val;
            } else {
                it = low.find(val);
                low.erase(it);
                sum_low -= val;
            }
        };

        // Helper lambda to balance sets so `low` always holds exactly ceil(x/2) elements
        auto rebalance = [&]() {
            int target_low = (x + 1) / 2;
            while (low.size() > target_low) {
                auto it = prev(low.end());
                int val = *it;
                low.erase(it);
                sum_low -= val;
                high.insert(val);
                sum_high += val;
            }
            while (low.size() < target_low) {
                auto it = high.begin();
                int val = *it;
                high.erase(it);
                sum_high -= val;
                low.insert(val);
                sum_low += val;
            }
        };

        // Initialize the first window of size x
        for (int i = 0; i < x; ++i) {
            add(nums[i]);
        }
        rebalance();
        
        long long cost = sum_high - sum_low;
        if (x % 2 == 1) cost += *low.rbegin();
        C[0] = cost;

        // Slide the window for the rest of the array
        for (int i = x; i < n; ++i) {
            add(nums[i]);
            remove(nums[i - x]);
            rebalance();
            
            cost = sum_high - sum_low;
            if (x % 2 == 1) cost += *low.rbegin();
            C[i - x + 1] = cost;
        }

        // DP Table to find optimal subarray combinations
        const long long INF = 1e18;
        vector<vector<long long>> dp(k + 1, vector<long long>(n + 1, INF));
        
        // Base case: 0 cost to pick 0 subarrays
        for (int i = 0; i <= n; ++i) {
            dp[0][i] = 0;
        }

        // Fill DP table
        for (int j = 1; j <= k; ++j) {
            for (int len = j * x; len <= n; ++len) {
                dp[j][len] = min(dp[j][len - 1], dp[j - 1][len - x] + C[len - x]);
            }
        }

        return dp[k][n];
    }
};