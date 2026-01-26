#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.length();
        // dp[i][j] will store the length of the longest palindromic subsequence
        // for the substring s starting at index i and ending at index j.
        vector<vector<int>> dp(n, vector<int>(n, 0));

        // Iterate backwards from the last character to the first.
        // We go backwards for 'i' because dp[i][j] depends on dp[i+1][...], 
        // which represents the row below it.
        for (int i = n - 1; i >= 0; i--) {
            // Base case: A single character is always a palindrome of length 1
            dp[i][i] = 1;

            // Iterate forwards for the end index 'j'
            for (int j = i + 1; j < n; j++) {
                if (s[i] == s[j]) {
                    // Ends match: 2 + result from the inner part
                    dp[i][j] = 2 + dp[i + 1][j - 1];
                } else {
                    // Ends don't match: take max of ignoring left or ignoring right
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
                }
            }
        }

        // The answer for the whole string is the range s[0...n-1]
        return dp[0][n - 1];
    }
};