#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    int minimumDifference(vector<int>& nums) {
        int n = nums.size() / 2;
        int total_sum = accumulate(nums.begin(), nums.end(), 0);
        
        // Arrays to store combinations. 
        // left_sums[k] will hold all possible sums of picking exactly k elements from the left half.
        vector<vector<int>> left_sums(n + 1);
        vector<vector<int>> right_sums(n + 1);
        
        // 1. Generate all possible sums for both left and right halves using bitmasking
        for (int i = 0; i < (1 << n); ++i) {
            int sz = 0;
            int sumL = 0;
            int sumR = 0;
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) {
                    sz++;
                    sumL += nums[j];         // First half of the array
                    sumR += nums[n + j];     // Second half of the array
                }
            }
            left_sums[sz].push_back(sumL);
            right_sums[sz].push_back(sumR);
        }
        
        // 2. Sort the right_sums to enable binary searching
        for (int i = 0; i <= n; ++i) {
            sort(right_sums[i].begin(), right_sums[i].end());
        }
        
        int min_diff = INT_MAX;
        int half_sum = total_sum / 2;
        
        // 3. Meet in the Middle
        for (int k = 0; k <= n; ++k) {
            // If we pick k elements from the left, we MUST pick (n - k) from the right
            int right_k = n - k;
            auto& r_sums = right_sums[right_k];
            
            for (int sumL : left_sums[k]) {
                // We want sumL + sumR to be as close to total_sum / 2 as possible
                int target = half_sum - sumL;
                
                // Binary search for the closest match in the right half
                auto it = lower_bound(r_sums.begin(), r_sums.end(), target);
                
                // Check the element at the lower bound
                if (it != r_sums.end()) {
                    int sumR = *it;
                    min_diff = min(min_diff, abs(total_sum - 2 * (sumL + sumR)));
                }
                // Check the element immediately proceeding it (it might be closer)
                if (it != r_sums.begin()) {
                    int sumR = *prev(it);
                    min_diff = min(min_diff, abs(total_sum - 2 * (sumL + sumR)));
                }
                
                // Optimization: Absolute difference can never be less than 0
                if (min_diff == 0) return 0; 
            }
        }
        
        return min_diff;
    }
};