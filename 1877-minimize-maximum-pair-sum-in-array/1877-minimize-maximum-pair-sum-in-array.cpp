#include <vector>
#include <algorithm> // For sort and max
#include <iostream>

using namespace std;

class Solution {
public:
    int minPairSum(vector<int>& nums) {
        // Step 1: Sort the array in ascending order
        // Time Complexity: O(N log N)
        sort(nums.begin(), nums.end());
        
        int max_sum = 0;
        int left = 0;
        int right = nums.size() - 1;
        
        // Step 2: Pair the smallest with the largest and find the max sum
        // Time Complexity: O(N)
        while (left < right) {
            int current_pair_sum = nums[left] + nums[right];
            max_sum = max(max_sum, current_pair_sum);
            
            left++;
            right--;
        }
        
        return max_sum;
    }
};