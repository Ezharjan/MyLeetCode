class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.length();
        int m = needle.length();

        // Optimization: If needle is longer than haystack, it cannot exist
        if (m > n) return -1;

        // Iterate through haystack up to the point where needle can still fit
        for (int i = 0; i <= n - m; i++) {
            // Check substring match
            int j = 0;
            while (j < m && haystack[i + j] == needle[j]) {
                j++;
            }
            
            // If we successfully matched the entire needle
            if (j == m) {
                return i;
            }
        }

        return -1;
    }
};