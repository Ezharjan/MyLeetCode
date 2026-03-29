#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int sortableIntegers(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return 1;

        // 1. Precompute prefix maximums
        vector<int> pref_max(n);
        pref_max[0] = nums[0];
        for (int i = 1; i < n; ++i) {
            pref_max[i] = max(pref_max[i - 1], nums[i]);
        }

        // 2. Precompute suffix minimums
        vector<int> suff_min(n);
        suff_min[n - 1] = nums[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            suff_min[i] = min(suff_min[i + 1], nums[i]);
        }

        // 3. Find valid global split points
        vector<bool> is_split(n, false);
        for (int j = 1; j < n; ++j) {
            // A boundary is a split if all elements on the left <= all elements on the right
            if (pref_max[j - 1] <= suff_min[j]) {
                is_split[j] = true;
            }
        }

        // 4. Prefix sum array to count adjacent value "drops" in O(1) time
        vector<int> drop_pref(n + 1, 0);
        for (int i = 0; i < n - 1; ++i) {
            drop_pref[i + 1] = drop_pref[i] + (nums[i] > nums[i + 1] ? 1 : 0);
        }
        drop_pref[n] = drop_pref[n - 1];

        int ans = 0;
        
        // 5. Check all k capable of dividing n
        for (int k = 1; k <= n; ++k) {
            if (n % k != 0) continue;

            bool ok = true;
            
            // Condition A: Validate all block boundaries are valid split points
            for (int m = 1; m < n / k; ++m) {
                if (!is_split[m * k]) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            // Condition B: Validate every block can become sorted via cyclic shifts
            for (int m = 0; m < n / k; ++m) {
                int first = nums[m * k];
                int last = nums[(m + 1) * k - 1];
                
                // Fetch linear drops inside the current chunk boundaries
                int linear_drops = drop_pref[(m + 1) * k - 1] - drop_pref[m * k];
                
                // Add the cyclic drop (if the wrap around breaks order)
                if (linear_drops + (last > first ? 1 : 0) > 1) {
                    ok = false;
                    break;
                }
            }
            
            // Tally successful sortable integer k
            if (ok) {
                ans += k;
            }
        }

        return ans;
    }
};