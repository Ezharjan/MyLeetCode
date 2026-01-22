class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int low = 0;
        int high = nums.size() - 1;

        while (low <= high) {
            // Calculate mid to avoid potential integer overflow with (low + high)
            int mid = low + (high - low) / 2;

            if (nums[mid] == target) {
                return mid; // Found the target exactly
            } else if (nums[mid] < target) {
                low = mid + 1; // Target is in the right half
            } else {
                high = mid - 1; // Target is in the left half
            }
        }

        // If not found, 'low' corresponds to the index where the target
        // would be if inserted in order.
        return low;
    }
};