class Solution {
public:
    long long countGoodSubarrays(vector<int>& nums) {
        int n = nums.size();
        vector<int> L(n), R(n);
        
        // Find R[i]: rightmost bound where nums[i] is still a superset
        vector<int> next_pos(30, n);
        for (int i = n - 1; i >= 0; --i) {
            int min_j = n;
            for (int b = 0; b < 30; ++b) {
                // If a bit is 0 in nums[i], we cannot include any number to the right that has this bit as 1.
                if ((nums[i] & (1 << b)) == 0) {
                    min_j = min(min_j, next_pos[b]);
                }
            }
            R[i] = min_j - 1;
            
            // Update the next seen positions of bits that are set in nums[i]
            for (int b = 0; b < 30; ++b) {
                if ((nums[i] & (1 << b)) != 0) {
                    next_pos[b] = i;
                }
            }
        }
        
        // Find L[i]: leftmost bound where nums[i] is a superset AND is the first occurrence
        vector<int> last_pos(30, -1);
        unordered_map<int, int> last_seen;
        last_seen.reserve(n);
        
        for (int i = 0; i < n; ++i) {
            int max_j = -1;
            for (int b = 0; b < 30; ++b) {
                // If a bit is 0 in nums[i], we cannot include any number to the left that has this bit as 1.
                if ((nums[i] & (1 << b)) == 0) {
                    max_j = max(max_j, last_pos[b]);
                }
            }
            // To prevent double counting subarrays, we stop at the closest previous identical value
            if (last_seen.count(nums[i])) {
                max_j = max(max_j, last_seen[nums[i]]);
            }
            
            L[i] = max_j + 1;
            
            // Update the last seen positions of bits that are set in nums[i]
            for (int b = 0; b < 30; ++b) {
                if ((nums[i] & (1 << b)) != 0) {
                    last_pos[b] = i;
                }
            }
            last_seen[nums[i]] = i;
        }
        
        // Accumulate combinations
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            ans += (long long)(i - L[i] + 1) * (R[i] - i + 1);
        }
        
        return ans;
    }
};