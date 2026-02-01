#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    int minimumCost(vector<int>& nums) {
        // The first element is always part of the cost.
        int sum = nums[0];
        
        // We need to find the two smallest elements in the rest of the array (nums[1] to end).
        // Initialize min1 and min2 to the maximum possible integer value.
        int min1 = INT_MAX;
        int min2 = INT_MAX;
        
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] < min1) {
                // If we find a new smallest, the old smallest becomes the second smallest.
                min2 = min1;
                min1 = nums[i];
            } else if (nums[i] < min2) {
                // If we find a number smaller than min2 (but not min1), update min2.
                min2 = nums[i];
            }
        }
        
        return sum + min1 + min2;
    }
};