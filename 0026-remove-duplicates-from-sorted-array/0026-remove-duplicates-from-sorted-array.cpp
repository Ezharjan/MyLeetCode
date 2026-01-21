#include <vector>

class Solution {
public:
    int removeDuplicates(std::vector<int>& nums) {
        // Edge case: If array is empty, return 0
        if (nums.empty()) return 0;

        // 'insertIndex' acts as the slow pointer. 
        // It tracks the position where the next unique element should be placed.
        // We start at 1 because the first element (index 0) is always unique.
        int insertIndex = 1;

        // 'i' acts as the fast pointer, scanning through the array.
        for (int i = 1; i < nums.size(); ++i) {
            // If the current number is different from the previous one, it's unique.
            if (nums[i] != nums[i - 1]) {
                // Move the unique number to the 'insertIndex' position
                nums[insertIndex] = nums[i];
                // Increment the insert position for the next unique number
                insertIndex++;
            }
        }

        // Return the number of unique elements
        return insertIndex;
    }
};