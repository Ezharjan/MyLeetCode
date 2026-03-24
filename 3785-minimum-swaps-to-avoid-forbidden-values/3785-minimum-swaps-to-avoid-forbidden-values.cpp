#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    int minSwaps(vector<int>& nums, vector<int>& forbidden) {
        int n = nums.size();
        unordered_map<int, int> freq;
        
        // Step 1: Check if a valid arrangement is mathematically possible.
        for (int i = 0; i < n; i++) {
            freq[nums[i]]++;
            freq[forbidden[i]]++;
        }
        
        // Pigeonhole Principle: If a number appears more than 'n' times in total, 
        // there is no way to place it without a collision.
        for (const auto& [val, count] : freq) {
            if (count > n) {
                return -1;
            }
        }
        
        // Step 2: Count the "conflicts" (where nums[i] == forbidden[i])
        int total_conflicts = 0;
        int max_conflict_freq = 0;
        unordered_map<int, int> conflict_freq;
        
        for (int i = 0; i < n; i++) {
            if (nums[i] == forbidden[i]) {
                total_conflicts++;
                conflict_freq[nums[i]]++;
                if (conflict_freq[nums[i]] > max_conflict_freq) {
                    max_conflict_freq = conflict_freq[nums[i]];
                }
            }
        }
        
        // Step 3: Calculate the minimum swaps using our greedy bounds.
        return max(max_conflict_freq, (total_conflicts + 1) / 2);
    }
};