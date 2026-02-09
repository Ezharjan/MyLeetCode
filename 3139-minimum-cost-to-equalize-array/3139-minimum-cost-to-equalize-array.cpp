#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>

using namespace std;

class Solution {
public:
    int minCostToEqualizeArray(vector<int>& nums, int cost1, int cost2) {
        int n = nums.size();
        long long MOD = 1e9 + 7;
        
        long long min_val = nums[0];
        long long max_val = nums[0];
        long long sum = 0;
        
        // Calculate basic stats: min, max, sum
        for (int x : nums) {
            min_val = min(min_val, (long long)x);
            max_val = max(max_val, (long long)x);
            sum += x;
        }

        long long total_diff_base = max_val * n - sum;

        // Case 1: Simple increment is cheaper, or valid pairs can't be formed to close gaps (n < 3).
        // If n=2, pairing adds +1 to both elements, maintaining the difference between them.
        // To equalize, we MUST use single operations to close the gap.
        if (cost1 * 2 <= cost2 || n <= 2) {
            return (total_diff_base * cost1) % MOD;
        }

        // Case 2: Pair operations are beneficial.
        // We iterate possible target values starting from max_val.
        // The upper bound 2*max_val is derived from the bottleneck equation: 
        // crossover happens roughly at (sum - 2*min)/(n-2), which is bounded by ~2*max_val.
        long long ans = LLONG_MAX;
        
        for (long long target = max_val; target <= 2 * max_val; ++target) {
            long long current_total_diff = target * n - sum;
            long long max_diff = target - min_val;
            long long cost = 0;

            if (max_diff > current_total_diff / 2) {
                // Bottleneck Scenario:
                // One element (min_val) needs so many increments that we run out of 
                // other elements to pair it with.
                // We pair as much as we can, then use singles for the rest of the bottleneck.
                long long pairs = current_total_diff - max_diff;
                long long singles = max_diff - pairs;
                cost = pairs * cost2 + singles * cost1;
            } else {
                // Uniform Scenario:
                // We can distribute the increments as pairs effectively.
                long long pairs = current_total_diff / 2;
                if (current_total_diff % 2 == 0) {
                    cost = pairs * cost2;
                } else {
                    // If odd sum, we need exactly one single operation
                    cost = pairs * cost2 + cost1;
                }
            }
            
            ans = min(ans, cost);
        }

        return ans % MOD;
    }
};