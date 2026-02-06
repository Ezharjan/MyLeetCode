#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int minRemoval(vector<int>& nums, int k) {
        // Step 1: Sort the array to easily identify min and max in a range.
        sort(nums.begin(), nums.end());
        
        int n = nums.size();
        int max_kept = 0;
        int i = 0;
        
        // Step 2: Sliding window approach.
        // 'i' points to the minimum element of the current window.
        // 'j' points to the maximum element of the current window.
        for (int j = 0; j < n; ++j) {
            // While the condition is violated (max > min * k), shrink the window from the left.
            // We use (long long) to prevent integer overflow because nums[i] * k can exceed 2^31.
            while ((long long)nums[j] > (long long)nums[i] * k) {
                i++;
            }
            
            // Update the maximum number of elements we can keep.
            max_kept = max(max_kept, j - i + 1);
        }
        
        // The minimum removals = Total elements - Maximum elements kept.
        return n - max_kept;
    }
};