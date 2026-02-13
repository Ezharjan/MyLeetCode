#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long minimumMoves(vector<int>& nums, int k, int maxChanges) {
        // P will store the indices where nums[i] == 1
        vector<int> P;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] == 1) {
                P.push_back(i);
            }
        }
        
        // Prefix sum array of P to quickly query the sum of elements in any window
        vector<long long> pref(P.size() + 1, 0);
        for (size_t i = 0; i < P.size(); ++i) {
            pref[i + 1] = pref[i] + P[i];
        }
        
        // M is the absolute minimum amount of existing 1s we must take from nums
        int M = max(0, k - maxChanges);
        
        // We only ever need to replace up to 3 created ones by checking up to M + 3
        int max_L = min({k, (int)P.size(), M + 3});
        
        long long ans = -1;
        
        // Test gathering exactly `L` existing 1s and creating `k - L` ones
        for (int L = M; L <= max_L; ++L) {
            long long current_cost = 0;
            if (L > 0) {
                long long min_cost_L = -1;
                // Sliding window of size L over the positions array P
                for (int i = 0; i <= (int)P.size() - L; ++i) {
                    // The spatial median index
                    int m = i + L / 2;
                    long long left_count = m - i;
                    long long right_count = (i + L - 1) - m;
                    
                    long long left_sum = pref[m] - pref[i];
                    long long right_sum = pref[i + L] - pref[m + 1];
                    
                    // Total distance of this window's 1s collapsing down to the median P[m]
                    long long cost = left_count * P[m] - left_sum + right_sum - right_count * P[m];
                    
                    if (min_cost_L == -1 || cost < min_cost_L) {
                        min_cost_L = cost;
                    }
                }
                // Append the cost of the additionally created 1s 
                current_cost = min_cost_L + 2LL * (k - L);
            } else {
                // If L is 0, we're strictly creating all required 1s
                current_cost = 2LL * k;
            }
            
            // Log the overall minimum 
            if (ans == -1 || current_cost < ans) {
                ans = current_cost;
            }
        }
        
        return ans;
    }
};