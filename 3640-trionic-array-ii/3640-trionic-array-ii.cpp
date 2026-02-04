#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    long long maxSumTrionic(vector<int>& nums) {
        int n = nums.size();
        // Use a large negative number for invalid states. 
        // We avoid LLONG_MIN to prevent underflow when adding positive numbers.
        const long long INF = -1e18; 
        
        long long up1 = INF;
        long long down = INF;
        long long up2 = INF;
        
        long long ans = INF;
        
        for (int i = 1; i < n; ++i) {
            long long cur = nums[i];
            long long prev = nums[i-1];
            
            // Temporary variables for the next state values
            long long next_up1 = INF;
            long long next_down = INF;
            long long next_up2 = INF;
            
            if (cur > prev) {
                // We are in an increasing trend
                
                // 1. Update up1 (First Increasing Leg)
                // Logic: Either extend the previous up1 sequence OR start a new one [prev, cur].
                // This is essentially Kadane's logic applied to the increasing leg.
                next_up1 = max(up1, prev) + cur;
                
                // 2. Update up2 (Second Increasing Leg)
                // Logic: Either extend the previous up2 sequence OR transition from a 'down' sequence.
                // We only transition if the previous state was valid ( > INF).
                long long best_prev_for_up2 = max(up2, down);
                if (best_prev_for_up2 > INF) {
                    next_up2 = best_prev_for_up2 + cur;
                }
                
            } else if (cur < prev) {
                // We are in a decreasing trend
                
                // Update down (Decreasing Leg)
                // Logic: Either extend the previous down sequence OR transition from an 'up1' sequence.
                long long best_prev_for_down = max(down, up1);
                if (best_prev_for_down > INF) {
                    next_down = best_prev_for_down + cur;
                }
            }
            // If cur == prev, strict monotonicity breaks, and next states remain INF.
            
            // Move to next state
            up1 = next_up1;
            down = next_down;
            up2 = next_up2;
            
            // Update global answer if we have a valid complete trionic subarray
            if (up2 > INF) {
                ans = max(ans, up2);
            }
        }
        
        return ans;
    }
};