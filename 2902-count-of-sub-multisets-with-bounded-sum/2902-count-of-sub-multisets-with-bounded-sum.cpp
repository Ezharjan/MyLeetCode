#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int countSubMultisets(vector<int>& nums, int l, int r) {
        const int MOD = 1e9 + 7;
        vector<int> count(20005, 0);
        int total_sum = 0;
        
        // Count the frequency of each number and sum up the array
        for (int num : nums) {
            count[num]++;
            total_sum += num;
        }
        
        // We only care about sums that do not exceed the minimum of r or the total sum
        int S = min(r, total_sum);
        
        // If our lower bound strictly demands more than the total array provides, it's impossible
        if (l > total_sum) {
            return 0;
        }
        
        vector<int> dp(S + 1, 0);
        dp[0] = 1; // Base case: 1 way to make a sum of 0 (using an empty multiset)
        
        // Dynamic Programming: Bound Knapsack processed for each unique non-zero element
        for (int v = 1; v <= min(20000, S); ++v) {
            if (count[v] > 0) {
                int c = count[v];
                
                // 1st Pass: Unbounded knapsack (accumulation forward)
                for (int j = v; j <= S; ++j) {
                    dp[j] += dp[j - v];
                    if (dp[j] >= MOD) {
                        dp[j] -= MOD;
                    }
                }
                
                // 2nd Pass: Remove combinations that used the element more times than its count
                int limit = (c + 1) * v;
                for (int j = S; j >= limit; --j) {
                    dp[j] -= dp[j - limit];
                    if (dp[j] < 0) {
                        dp[j] += MOD;
                    }
                }
            }
        }
        
        long long ans = 0;
        // Collect all distinct ways to form sums standing within the inclusive range [l, S]
        for (int j = l; j <= S; ++j) {
            ans += dp[j];
        }
        
        ans %= MOD; // Bring answering pool within bounds
        
        // Multiply by combinations of 0s.
        // E.g., if there are 3 zeroes, they offer 4 possibilities: picking 0, 1, 2, or 3 zeroes.
        ans = (ans * (count[0] + 1)) % MOD;
        
        return ans;
    }
};