class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int k = 0; // Pointer for the position of the next non-zero element

        for (int i = 0; i < nums.size(); ++i) {
            // If the current element is not zero...
            if (nums[i] != 0) {
                // ...swap it with the element at index 'k'
                // We add a check (i != k) to avoid unnecessary self-swaps
                // (e.g., if the array starts with [1, 2...], we don't need to swap 1 with itself)
                if (i != k) {
                    swap(nums[k], nums[i]);
                }
                k++;
            }
        }
    }
};