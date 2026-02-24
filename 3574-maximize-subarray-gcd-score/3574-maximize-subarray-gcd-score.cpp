#include <vector>
#include <numeric>
#include <algorithm>

class Solution {
public:
    long long maxGCDScore(std::vector<int>& nums, int k) {
        int n = nums.size();
        
        // Precalculate the power of 2 factor (number of trailing zeros) for each number
        std::vector<int> v(n);
        for (int i = 0; i < n; ++i) {
            v[i] = __builtin_ctz(nums[i]);
        }
        
        long long max_score = 0;
        
        // Iterate over all possible starting points of the subarray
        for (int i = 0; i < n; ++i) {
            int g = nums[i];
            int min_v = v[i];
            int count_min_v = 0;
            
            // Expand the subarray to the right
            for (int j = i; j < n; ++j) {
                // Update GCD strictly when needed to maintain an O(1) amortized inner loop cost
                if (nums[j] % g != 0) {
                    g = std::gcd(g, nums[j]);
                }
                
                // Track the minimum power of 2 and the frequency of elements having it
                if (v[j] < min_v) {
                    min_v = v[j];
                    count_min_v = 1;
                } else if (v[j] == min_v) {
                    count_min_v++;
                }
                
                // Base score without modified GCD
                long long current_score = (long long)(j - i + 1) * g;
                
                // If we have enough operations to double all limiting elements, the GCD doubles
                if (count_min_v <= k) {
                    current_score = std::max(current_score, (long long)(j - i + 1) * g * 2);
                }
                
                if (current_score > max_score) {
                    max_score = current_score;
                }
            }
        }
        
        return max_score;
    }
};