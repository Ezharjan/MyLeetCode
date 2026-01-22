#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_set<int> window;

        for (int i = 0; i < nums.size(); ++i) {
            // 1. Maintain the window size
            // If we have moved past k elements, remove the element that fell out of the window.
            if (i > k) {
                window.erase(nums[i - k - 1]);
            }

            // 2. Check for duplicate in the current window
            if (window.count(nums[i])) {
                return true; 
            }

            // 3. Add the current element to the window
            window.insert(nums[i]);
        }

        return false;
    }
};