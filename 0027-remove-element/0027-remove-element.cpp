class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int k = 0; // This acts as our "write" index

        // 'i' acts as our "read" index
        for (int i = 0; i < nums.size(); ++i) {
            // If the current element is NOT the value we want to remove...
            if (nums[i] != val) {
                // ...we place it at the 'k' position.
                nums[k] = nums[i];
                k++;
            }
            // If it IS the value, we just ignore it and continue the loop.
        }

        // k now represents the new length of the valid array
        return k;
    }
};