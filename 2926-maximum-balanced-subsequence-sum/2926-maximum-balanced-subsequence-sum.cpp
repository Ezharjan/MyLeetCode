#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maxBalancedSubsequenceSum(vector<int>& nums) {
        int n = nums.size();
        
        // Handle edge case where all numbers are non-positive
        long long max_val = -1e18;
        for (int i = 0; i < n; ++i) {
            max_val = max(max_val, (long long)nums[i]);
        }
        if (max_val <= 0) {
            return max_val;
        }

        // Coordinate compression for (nums[i] - i)
        vector<int> vals(n);
        for (int i = 0; i < n; ++i) {
            vals[i] = nums[i] - i;
        }
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        int m = vals.size();
        // Fenwick tree to track the maximum sum sequence
        vector<long long> tree(m + 1, 0);

        auto update = [&](int i, long long val) {
            for (; i <= m; i += i & -i) {
                tree[i] = max(tree[i], val);
            }
        };

        auto query = [&](int i) {
            long long res = 0;
            for (; i > 0; i -= i & -i) {
                res = max(res, tree[i]);
            }
            return res;
        };

        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            int a = nums[i] - i;
            // Get the 1-based compressed index
            int c = lower_bound(vals.begin(), vals.end(), a) - vals.begin() + 1;
            
            // Query the max sum possible ending in a previous valid state, add current
            long long cur = query(c) + nums[i];
            
            // If it's a positive contribution, insert it into the BIT
            if (cur > 0) {
                update(c, cur);
            }
            ans = max(ans, cur);
        }

        return ans;
    }
};