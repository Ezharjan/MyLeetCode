#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maximumStrength(vector<int>& nums, int k) {
        int n = nums.size();
        const long long MIN_INF = -1000000000000000000LL; 
        
        // f[j] tracks max strength of j subarrays where the j-th subarray ends exactly at the current element
        vector<long long> f(k + 1, MIN_INF);
        
        // g[j] tracks max overall strength of j subarrays up to the current element
        vector<long long> g(k + 1, MIN_INF);
        
        // Base cases
        f[0] = 0;
        g[0] = 0;
        
        for (int i = 1; i <= n; ++i) {
            long long prev_g = g[0]; 
            
            // We only need to check up to min(i, k) subarrays for the first i elements
            for (int j = 1; j <= min(i, k); ++j) {
                // Calculate the coefficient for the j-th subarray
                long long w = (long long)(k - j + 1) * (j % 2 == 1 ? 1 : -1);
                
                // Save the current g[j] before we overwrite it; it becomes g[j-1] for the next iteration j+1
                long long temp_g = g[j];
                
                // Maximize whether we extend the current j-th subarray or start a brand new j-th subarray
                long long next_f = max(f[j], prev_g) + w * nums[i - 1];
                
                // Update states for the current element
                f[j] = next_f;
                g[j] = max(g[j], f[j]);
                
                // Carry over the old g[j] to act as g[j-1] in the next loop
                prev_g = temp_g;
            }
        }
        
        // g[k] will contain the optimal max strength combining exactly k disjoint subarrays
        return g[k];
    }
};