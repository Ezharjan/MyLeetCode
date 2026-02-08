#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    int longestPalindrome(string s, string t) {
        int n = s.length();
        int m = t.length();
        
        string tr = t;
        reverse(tr.begin(), tr.end());
        
        string sr = s;
        reverse(sr.begin(), sr.end());
        
        // max_match_s[i] stores the length of the longest suffix of s[0...i]
        // that is also a substring of t^R (meaning its reverse is a substring of t).
        vector<int> max_match_s(n, 0);
        
        // Scope to manage DP memory
        {
            // dp[i][j] = length of common suffix of s[0...i-1] and tr[0...j-1]
            vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
            for (int i = 1; i <= n; ++i) {
                for (int j = 1; j <= m; ++j) {
                    if (s[i-1] == tr[j-1]) {
                        dp[i][j] = dp[i-1][j-1] + 1;
                    } else {
                        dp[i][j] = 0;
                    }
                    // We want the longest suffix of s ending at i-1 found ANYWHERE in t^R
                    max_match_s[i-1] = max(max_match_s[i-1], dp[i][j]);
                }
            }
        }
        
        // max_match_t[i] stores the length of the longest prefix of t[i...m-1]
        // that is also a substring of s^R.
        vector<int> max_match_t(m, 0);
        {
            // dp[i][j] = length of common prefix of t[i...m-1] and sr[j...n-1]
            vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
            // Fill backwards
            for (int i = m - 1; i >= 0; --i) {
                for (int j = n - 1; j >= 0; --j) {
                    if (t[i] == sr[j]) {
                        dp[i][j] = dp[i+1][j+1] + 1;
                    } else {
                        dp[i][j] = 0;
                    }
                    max_match_t[i] = max(max_match_t[i], dp[i][j]);
                }
            }
        }
        
        int ans = 0;
        
        // Case 0: Pure matches (Split exactly at the boundary, empty core)
        // A match of length L implies a palindrome of length 2*L.
        for (int x : max_match_s) {
            ans = max(ans, 2 * x);
        }
        
        // Case 1: Center in s
        for (int i = 0; i < n; ++i) {
            // Odd length palindromes in s
            for (int l = i, r = i; l >= 0 && r < n && s[l] == s[r]; --l, ++r) {
                int pal_len = r - l + 1;
                int match_len = 0;
                if (l > 0) match_len = max_match_s[l-1];
                ans = max(ans, pal_len + 2 * match_len);
            }
            // Even length palindromes in s
            for (int l = i, r = i + 1; l >= 0 && r < n && s[l] == s[r]; --l, ++r) {
                int pal_len = r - l + 1;
                int match_len = 0;
                if (l > 0) match_len = max_match_s[l-1];
                ans = max(ans, pal_len + 2 * match_len);
            }
        }
        
        // Case 2: Center in t
        for (int i = 0; i < m; ++i) {
            // Odd length palindromes in t
            for (int l = i, r = i; l >= 0 && r < m && t[l] == t[r]; --l, ++r) {
                int pal_len = r - l + 1;
                int match_len = 0;
                if (r < m - 1) match_len = max_match_t[r+1];
                ans = max(ans, pal_len + 2 * match_len);
            }
            // Even length palindromes in t
            for (int l = i, r = i + 1; l >= 0 && r < m && t[l] == t[r]; --l, ++r) {
                int pal_len = r - l + 1;
                int match_len = 0;
                if (r < m - 1) match_len = max_match_t[r+1];
                ans = max(ans, pal_len + 2 * match_len);
            }
        }
        
        return ans;
    }
};