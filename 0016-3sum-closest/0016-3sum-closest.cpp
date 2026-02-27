#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        // Step 1: Sort the array to use the two-pointer technique
        sort(nums.begin(), nums.end());
        
        int n = nums.size();
        // Initialize closest_sum with the sum of the first three elements
        int closest_sum = nums[0] + nums[1] + nums[2]; 

        // Step 2: Iterate through the array
        for (int i = 0; i < n - 2; ++i) {
            // Optional optimization: Skip duplicate elements to save time
            if (i > 0 && nums[i] == nums[i - 1]) continue;

            int left = i + 1;
            int right = n - 1;

            // Step 3: Use two pointers to find the closest sum for the current nums[i]
            while (left < right) {
                int current_sum = nums[i] + nums[left] + nums[right];

                // Update closest_sum if the current_sum is closer to the target
                if (abs(current_sum - target) < abs(closest_sum - target)) {
                    closest_sum = current_sum;
                }

                // Adjust pointers based on how current_sum compares to target
                if (current_sum < target) {
                    ++left;  // We need a larger sum, move the left pointer right
                } else if (current_sum > target) {
                    --right; // We need a smaller sum, move the right pointer left
                } else {
                    // Exact match found! Distance is 0, so we can return immediately
                    return current_sum;
                }
            }
        }

        return closest_sum;
    }
};