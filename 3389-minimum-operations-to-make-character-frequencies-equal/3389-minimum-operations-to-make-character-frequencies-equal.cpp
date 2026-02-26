#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    int makeStringGood(string s) {
        vector<int> C(26, 0);
        for (char c : s) {
            C[c - 'a']++;
        }
        
        int max_freq = 0;
        for (int count : C) {
            max_freq = max(max_freq, count);
        }
        
        // If string is empty or no characters, cost is 0
        if (max_freq == 0) return 0;
        
        int min_ops = s.length(); // Upper bound: deleting all characters
        
        // Test all possible uniform frequencies
        for (int F = 1; F <= max_freq; ++F) {
            // dp[i][0] -> cost if target for char i is 0
            // dp[i][1] -> cost if target for char i is F
            vector<vector<int>> dp(26, vector<int>(2, 0));
            
            // Base case for the first character ('a')
            dp[0][0] = C[0];
            dp[0][1] = abs(C[0] - F);
            
            for (int i = 1; i < 26; ++i) {
                for (int t = 0; t <= 1; ++t) {
                    int T_curr = (t == 0) ? 0 : F;
                    int cost = abs(C[i] - T_curr);
                    int def_curr = (t == 0) ? 0 : max(0, F - C[i]);
                    
                    int min_val = 1e9;
                    for (int prev_t = 0; prev_t <= 1; ++prev_t) {
                        int T_prev = (prev_t == 0) ? 0 : F;
                        int exc_prev = (prev_t == 0) ? C[i-1] : max(0, C[i-1] - F);
                        
                        // Shifting saves 1 operation per character compared to discrete del/ins.
                        int savings = min(exc_prev, def_curr);
                        int val = dp[i-1][prev_t] + cost - savings;
                        
                        min_val = min(min_val, val);
                    }
                    dp[i][t] = min_val;
                }
            }
            
            // For the current target frequency F, record the minimum operations needed globally
            min_ops = min(min_ops, min(dp[25][0], dp[25][1]));
        }
        
        return min_ops;
    }
};