#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    // Helper function to compute the Z-array
    // z[i] is the length of the longest common prefix between the string and the suffix starting at i.
    vector<int> computeZ(const string& str) {
        int n = str.length();
        vector<int> z(n);
        int l = 0, r = 0;
        for (int i = 1; i < n; ++i) {
            if (i <= r) {
                z[i] = min(r - i + 1, z[i - l]);
            }
            while (i + z[i] < n && str[z[i]] == str[i + z[i]]) {
                z[i]++;
            }
            if (i + z[i] - 1 > r) {
                l = i;
                r = i + z[i] - 1;
            }
        }
        return z;
    }

    int minStartingIndex(string s, string pattern) {
        int n = s.length();
        int m = pattern.length();
        
        // 1. Calculate Prefix Matches using Z-Algorithm
        // Construct: pattern + "#" + s
        // z_pre[m + 1 + i] will tell us the Longest Common Prefix of pattern and s[i...]
        string fwd = pattern + "#" + s;
        vector<int> z_pre = computeZ(fwd);
        
        // 2. Calculate Suffix Matches using Z-Algorithm on reversed strings
        // Reverse s and pattern to find common suffixes via prefix matching
        string s_rev = s;
        reverse(s_rev.begin(), s_rev.end());
        string p_rev = pattern;
        reverse(p_rev.begin(), p_rev.end());
        
        // Construct: rev(pattern) + "#" + rev(s)
        string bwd = p_rev + "#" + s_rev;
        vector<int> z_suf = computeZ(bwd);
        
        // 3. Iterate through all possible substrings in s
        // Valid range for starting index i is [0, n - m]
        for (int i = 0; i <= n - m; ++i) {
            // Length of matching prefix at start of substring
            // Index in z_pre: (pattern length) + (separator) + i
            int len_pre = z_pre[m + 1 + i];
            
            // If prefix match is the full pattern length, we have 0 mismatches.
            if (len_pre >= m) return i;
            
            // Length of matching suffix at end of substring
            // The substring ends at index (i + m - 1).
            // In the reversed string s_rev, this corresponds to index: n - 1 - (i + m - 1) = n - m - i
            int rev_idx = n - m - i;
            int len_suf = z_suf[m + 1 + rev_idx];
            
            // Condition for "almost equal":
            // The matching parts from left and right must cover at least (m - 1) characters.
            // This allows exactly one character (the mismatch) or zero characters to be skipped.
            if (len_pre + len_suf >= m - 1) {
                return i;
            }
        }
        
        return -1;
    }
};