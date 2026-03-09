#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int numberOfPermutations(int n, vector<vector<int>>& requirements) {
        const int MOD = 1e9 + 7;
        const int MAX_INV = 400; // From constraints, max inversions is 400
        
        // Convert requirements to a fast lookup array. req[i] = required inversions at index i.
        // Initialize with -1 to represent "no requirement".
        vector<int> req(n, -1);
        for (const auto& r : requirements) {
            req[r[0]] = r[1];
        }
        
        // A prefix of length 1 (index 0) can only ever have 0 inversions.
        if (req[0] > 0) {
            return 0;
        }
        
        // dp[i][j] = ways to arrange first i+1 numbers with j inversions
        vector<vector<long long>> dp(n, vector<long long>(MAX_INV + 1, 0));
        dp[0][0] = 1;
        
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j <= MAX_INV; ++j) {
                // If there is a requirement for this prefix length and j doesn't match it,
                // there are 0 valid permutations ending in this state.
                if (req[i] != -1 && req[i] != j) {
                    dp[i][j] = 0;
                    continue;
                }
                
                long long sum = 0;
                // We can add the new largest element to create anywhere from 0 to i new inversions.
                for (int k = 0; k <= min(i, j); ++k) {
                    sum = (sum + dp[i-1][j-k]) % MOD;
                }
                dp[i][j] = sum;
            }
        }
        
        // The answer is the number of valid permutations for the full length n (index n-1).
        // The problem guarantees there is an endi == n - 1.
        return dp[n-1][req[n-1]];
    }
};