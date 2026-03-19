class Solution {
public:
    int minOrAfterOperations(vector<int>& nums, int k) {
        int n = nums.size();
        int mask = 0; // Tracks the bits we have successfully forced to 0
        int ans = 0;  // Tracks the final OR result (the bits we are forced to leave as 1)
        
        // The maximum value is 2^30 - 1, so we check from the 29th bit down to the 0th bit
        for (int b = 29; b >= 0; --b) {
            int test_mask = mask | (1 << b);
            int cnt = 0;
            int curr = -1; // -1 in two's complement acts as an all-1s bitmask
            
            // Greedily partition the array into valid blocks
            for (int x : nums) {
                curr &= x;
                // If all bits required to be 0 by 'test_mask' are successfully 0
                if ((curr & test_mask) == 0) {
                    cnt++;
                    curr = -1; // Reset for the next block
                }
            }
            
            // If the required operations (n - blocks formed) is within our budget
            if (n - cnt <= k) {
                mask = test_mask; // Success: we can keep this bit as 0
            } else {
                ans |= (1 << b);  // Failure: we must leave this bit as 1
            }
        }
        
        return ans;
    }
};