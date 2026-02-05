class Solution {
public:
    vector<int> constructTransformedArray(vector<int>& nums) {
        int n = nums.size();
        vector<int> result(n);
        
        for (int i = 0; i < n; ++i) {
            // Calculate the target index.
            // We add 'nums[i]' to the current index 'i'.
            // The formula ((a % n) + n) % n ensures the result is always a 
            // positive index between 0 and n-1, handling both wrapping 
            // forward (positive steps) and backward (negative steps).
            int targetIndex = ((i + nums[i]) % n + n) % n;
            
            result[i] = nums[targetIndex];
        }
        
        return result;
    }
};