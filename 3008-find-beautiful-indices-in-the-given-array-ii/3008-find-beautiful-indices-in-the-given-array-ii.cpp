class Solution {
    // Helper function to find all occurrences of a pattern in a text using KMP
    vector<int> getMatches(const string& text, const string& pattern) {
        int n = text.length(), m = pattern.length();
        if (n < m) return {};
        
        // 1. Build the Longest Proper Prefix which is also Suffix (LPS) array
        vector<int> lps(m, 0);
        for (int i = 1, len = 0; i < m; ) {
            if (pattern[i] == pattern[len]) {
                lps[i++] = ++len;
            } else if (len) {
                len = lps[len - 1];
            } else {
                lps[i++] = 0;
            }
        }
        
        // 2. Perform the KMP search
        vector<int> matches;
        for (int i = 0, j = 0; i < n; ) {
            if (text[i] == pattern[j]) {
                i++; 
                j++;
                if (j == m) {
                    matches.push_back(i - j);
                    j = lps[j - 1]; // To allow for overlapping matches
                }
            } else if (j) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
        return matches;
    }

public:
    vector<int> beautifulIndices(string s, string a, string b, int k) {
        vector<int> indicesA = getMatches(s, a);
        vector<int> indicesB = getMatches(s, b);
        vector<int> result;
        
        // Two-pointer approach to find valid pairs
        int j = 0;
        for (int i : indicesA) {
            // Fast-forward pointer `j` in `indicesB` until it's within the lower bound of `k`
            while (j < indicesB.size() && indicesB[j] < i - k) {
                j++;
            }
            // Check if the current candidate in `indicesB` satisfies the upper bound of `k`
            if (j < indicesB.size() && indicesB[j] <= i + k) {
                result.push_back(i);
            }
        }
        
        return result;
    }
};