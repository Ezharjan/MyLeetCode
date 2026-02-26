#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long numGoodSubarrays(vector<int>& nums, int k) {
        int n = nums.size();
        
        // Step 1: Compute Component 1 (Total Good Subarrays)
        vector<int> pref(n + 1, 0);
        long long current_sum = 0;
        for (int i = 0; i < n; ++i) {
            current_sum += nums[i];
            pref[i + 1] = current_sum % k;
        }
        
        // Sort prefix sums mod k to easily find frequencies of each remainder
        sort(pref.begin(), pref.end());
        
        long long comp1 = 0;
        long long count = 1;
        for (int i = 1; i <= n; ++i) {
            if (pref[i] == pref[i - 1]) {
                count++;
            } else {
                comp1 += count * (count - 1) / 2;
                count = 1;
            }
        }
        // Add permutations of the last remainder group
        comp1 += count * (count - 1) / 2;
        
        long long comp2 = 0;
        long long comp3 = 0;
        
        int i = 0;
        // Step 2 & 3: Compute Duplicate Deductions and Distinct Single-Value Sequences
        while (i < n) {
            int j = i;
            while (j < n && nums[j] == nums[i]) {
                j++;
            }
            
            long long v = nums[i];
            long long c = j - i;
            
            // Calculate sequence step intervals for satisfying mod k sum
            long long g = std::gcd(v, (long long)k);
            long long step = k / g;
            long long M = c / step;
            
            if (M > 0) {
                // Number of distinct good sequences within this repeating block
                comp3 += M;
                // Accumulate duplicate counts of the sub-sequences within this block 
                comp2 += M * (c + 1) - step * M * (M + 1) / 2;
            }
            
            i = j;
        }
        
        return comp1 - comp2 + comp3;
    }
};