#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
public:
    long long findKthSmallest(vector<int>& coins, int k) {
        int n = coins.size();
        
        // Vector to store {lcm_value, sign} for all subsets
        // sign is +1 for odd sized subsets, -1 for even sized subsets.
        vector<pair<long long, int>> subsets;
        
        // Iterate through all non-empty subsets using bitmask (1 to 2^n - 1)
        for (int i = 1; i < (1 << n); ++i) {
            long long current_lcm = 1;
            int set_bits = 0;
            
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) {
                    set_bits++;
                    // LCM(a, b) = (a * b) / GCD(a, b)
                    // We calculate it as (a / GCD(a, b)) * b to prevent early overflow
                    // With coins[i] <= 25, the max LCM fits within long long.
                    long long g = std::gcd(current_lcm, (long long)coins[j]);
                    current_lcm = (current_lcm / g) * coins[j];
                }
            }
            
            // Inclusion-Exclusion Logic:
            // Odd number of elements -> Add (+)
            // Even number of elements -> Subtract (-)
            int sign = (set_bits % 2 == 1) ? 1 : -1;
            subsets.push_back({current_lcm, sign});
        }
        
        // Binary Search for the k-th smallest amount
        // Lower bound: 1
        // Upper bound: The smallest coin * k (This is a safe upper limit)
        long long min_coin = *min_element(coins.begin(), coins.end());
        long long low = 1;
        long long high = min_coin * (long long)k;
        long long ans = high;
        
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            long long count = 0;
            
            // Calculate how many numbers <= mid are divisible by any coin
            for (const auto& p : subsets) {
                count += p.second * (mid / p.first);
            }
            
            if (count >= k) {
                ans = mid;
                high = mid - 1; // Try to find a smaller number
            } else {
                low = mid + 1;  // Need a larger number
            }
        }
        
        return ans;
    }
};