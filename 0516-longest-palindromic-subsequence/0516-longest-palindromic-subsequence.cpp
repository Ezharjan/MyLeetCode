#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.length();
        // dp[j] will store the LPS length for the substring s[i...j]
        // Initially, we treat it as the values from the previous row (i+1)
        vector<int> dp(n, 0);

        // Iterate backwards just like the 2D approach
        for (int i = n - 1; i >= 0; i--) {
            dp[i] = 1; // Base case: single character s[i] is length 1
            
            int prevDiag = 0; // Stores dp[i+1][j-1] (the diagonal value)

            for (int j = i + 1; j < n; j++) {
                int temp = dp[j]; // Store dp[i+1][j] before we overwrite it
                
                if (s[i] == s[j]) {
                    // Match: 2 + value from inner range (s[i+1...j-1])
                    // prevDiag holds the value of dp[j-1] from the previous iteration of i
                    dp[j] = 2 + prevDiag;
                } else {
                    // No Match: max(current range without left, current range without right)
                    // dp[j] is currently s[i+1...j] (ignore left)
                    // dp[j-1] is currently s[i...j-1] (ignore right)
                    dp[j] = max(dp[j], dp[j-1]);
                }
                
                // Update prevDiag for the next column's use
                prevDiag = temp;
            }
        }

        return dp[n - 1];
    }
};