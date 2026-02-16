#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

class Solution {
public:
    int maxTotalReward(vector<int>& rewardValues) {
        // Step 1: Sort and remove duplicates
        sort(rewardValues.begin(), rewardValues.end());
        rewardValues.erase(unique(rewardValues.begin(), rewardValues.end()), rewardValues.end());
        
        // Step 2: Initialize bitset. Max possible reward is 99999.
        bitset<100000> dp;
        dp[0] = 1; // A sum of 0 is always achievable initially
        
        // Step 3: Populate reachable sums
        for (int v : rewardValues) {
            // Filter states where x < v
            // Shift left to push bits >= v out of bounds, then shift right to return them to place
            bitset<100000> valid_x = (dp << (100000 - v)) >> (100000 - v);
            
            // Shift the valid states by v to represent adding v to those sums
            dp |= (valid_x << v);
        }
        
        // Step 4: Find the maximum achievable sum
        for (int i = 99999; i >= 0; --i) {
            if (dp[i]) {
                return i;
            }
        }
        
        return 0;
    }
};