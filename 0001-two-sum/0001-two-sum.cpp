#include <vector>
#include <unordered_map>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        // Create a hash map to store numbers and their indices.
        // Key = the number, Value = its index
        std::unordered_map<int, int> numMap;

        for (int i = 0; i < nums.size(); ++i) {
            int complement = target - nums[i];

            // Check if the complement exists in the map
            if (numMap.find(complement) != numMap.end()) {
                // If found, return the index of the complement and the current index
                return {numMap[complement], i};
            }

            // Otherwise, add the current number and its index to the map
            numMap[nums[i]] = i;
        }

        // Return empty vector if no solution found (though the problem guarantees one)
        return {};
    }
};