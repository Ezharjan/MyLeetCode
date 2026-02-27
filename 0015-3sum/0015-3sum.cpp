class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int n = nums.size();
        
        // A triplet requires at least 3 numbers
        if (n < 3) return result;
        
        // Step 1: Sort the array
        sort(nums.begin(), nums.end());
        
        // Step 2: Iterate to find triplets
        for (int i = 0; i < n - 2; ++i) {
            // Optimization: If the smallest number is positive, we can't reach zero
            if (nums[i] > 0) break;
            
            // Skip duplicate values for the first element to avoid duplicate triplets
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            // Step 3: Two Pointers approach
            int left = i + 1;
            int right = n - 1;
            
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                
                if (sum == 0) {
                    // Triplet found
                    result.push_back({nums[i], nums[left], nums[right]});
                    
                    // Skip duplicates for the second element
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    
                    // Skip duplicates for the third element
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    
                    // Move both pointers inward to look for other potential combinations
                    left++;
                    right--;
                } 
                else if (sum < 0) {
                    // The sum is too small, we need a larger number, so move the left pointer right
                    left++;
                } 
                else {
                    // The sum is too large, we need a smaller number, so move the right pointer left
                    right--;
                }
            }
        }
        
        return result;
    }
};