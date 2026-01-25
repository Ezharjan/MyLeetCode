#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    int minimumDifference(vector<int>& nums, int k) {
        // Edge case: If k is 1, the difference is always 0 (max and min are the same element).
        if (k == 1) {
            return 0;
        }

        // Step 1: Sort the vector to group closest numbers together
        sort(nums.begin(), nums.end());

        int minDiff = INT_MAX;

        // Step 2: Iterate using a sliding window of size k
        // The window ends at index 'i' and starts at 'i - k + 1'
        for (int i = k - 1; i < nums.size(); ++i) {
            int currentDiff = nums[i] - nums[i - k + 1];
            
            // Update the minimum difference found so far
            if (currentDiff < minDiff) {
                minDiff = currentDiff;
            }
        }

        return minDiff;
    }
};