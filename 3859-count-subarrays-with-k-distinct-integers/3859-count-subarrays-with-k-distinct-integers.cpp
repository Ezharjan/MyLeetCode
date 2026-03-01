#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long countSubarrays(vector<int>& nums, int k, int m) {
        int n = nums.size();
        
        // Find the maximum element to allocate precisely sized frequency arrays
        int max_val = *max_element(nums.begin(), nums.end());
        vector<int> freq1(max_val + 1, 0);
        vector<int> freq2(max_val + 1, 0);
        
        long long ans = 0;
        int left1 = 0, left2 = 0;
        int count1 = 0, count2 = 0;
        
        for (int R = 0; R < n; ++R) {
            // Update Window 1: Maintains elements appearing at least 1 time
            freq1[nums[R]]++;
            if (freq1[nums[R]] == 1) {
                count1++;
            }
            
            // Shrink Window 1 until we have at most k distinct elements
            while (count1 > k) {
                freq1[nums[left1]]--;
                if (freq1[nums[left1]] == 0) {
                    count1--;
                }
                left1++;
            }
            
            // Update Window 2: Maintains elements appearing at least m times
            freq2[nums[R]]++;
            if (freq2[nums[R]] == m) {
                count2++;
            }
            
            // Shrink Window 2 to find the boundary where we no longer have k elements with frequency >= m
            while (count2 >= k) {
                freq2[nums[left2]]--;
                if (freq2[nums[left2]] == m - 1) {
                    count2--;
                }
                left2++;
            }
            
            // If left2 > left1, all starting indices between them are valid subarrays ending at R
            ans += max(0, left2 - left1);
        }
        
        return ans;
    }
};