#include <vector>
#include <numeric>
#include <unordered_map>

class Solution {
public:
    long long countPairs(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> gcd_counts;
        long long total_pairs = 0;
        
        for (int num : nums) {
            long long curr_gcd = std::gcd(num, k);
            
            // Check the current GCD against all previously recorded GCDs
            for (auto& [prev_gcd, count] : gcd_counts) {
                // Use 1LL to cast to long long and prevent integer overflow during multiplication
                if ((1LL * prev_gcd * curr_gcd) % k == 0) {
                    total_pairs += count;
                }
            }
            
            // Add the current GCD to the frequency map
            gcd_counts[curr_gcd]++;
        }
        
        return total_pairs;
    }
};