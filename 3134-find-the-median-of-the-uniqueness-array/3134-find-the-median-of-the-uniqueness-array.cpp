#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    int medianOfUniquenessArray(vector<int>& nums) {
        int n = nums.size();
        
        // Total number of subarrays
        long long total_subarrays = (long long)n * (n + 1) / 2;
        
        // Target index for the median (1-based index)
        long long target = (total_subarrays + 1) / 2;
        
        // Find the maximum element to size our frequency array properly
        int max_val = *max_element(nums.begin(), nums.end());
        vector<int> freq(max_val + 1, 0);
        
        unordered_set<int> unique_elements(nums.begin(), nums.end());
        int left = 1;
        int right = unique_elements.size();
        int ans = right;
        
        // Helper lambda to count subarrays with at most `k` distinct elements
        auto countSubarraysWithAtMostKDistinct = [&](int k) {
            long long count = 0;
            int l = 0;
            int distinct = 0;
            
            // Reset the frequency array for this binary search iteration
            fill(freq.begin(), freq.end(), 0); 
            
            for (int r = 0; r < n; ++r) {
                if (freq[nums[r]]++ == 0) {
                    distinct++;
                }
                
                // If we exceed k distinct elements, shrink the window from the left
                while (distinct > k) {
                    if (--freq[nums[l]] == 0) {
                        distinct--;
                    }
                    l++;
                }
                
                // Add the number of valid subarrays ending at index r
                count += (r - l + 1);
            }
            return count;
        };
        
        // Binary search for the median
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (countSubarraysWithAtMostKDistinct(mid) >= target) {
                ans = mid;       // Found a candidate, try to find a smaller one
                right = mid - 1;
            } else {
                left = mid + 1;  // Not enough subarrays, we need a larger median
            }
        }
        
        return ans;
    }
};