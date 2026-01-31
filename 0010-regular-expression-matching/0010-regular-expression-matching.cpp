class Solution {
public:
    bool isMatch(string s, string p) {
        int m = s.length();
        int n = p.length();
        
        // dp[i][j] is true if the first i chars of s match the first j chars of p
        // Using m+1 and n+1 to handle empty strings (length 0)
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        
        // Base case: Empty string matches empty pattern
        dp[0][0] = true;
        
        // Initialize first row: handling patterns like a*, a*b*, c*a*b* matching empty string
        // We start from j=2 because a '*' needs a preceding character.
        for (int j = 2; j <= n; j++) {
            if (p[j - 1] == '*') {
                // If current is '*', it can match zero of preceding element.
                // So we look 2 positions back.
                dp[0][j] = dp[0][j - 2];
            }
        }
        
        // Fill the DP table
        // i ranges from 1 to m (lengths of s)
        // j ranges from 1 to n (lengths of p)
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                // Current characters being compared are s[i-1] and p[j-1]
                
                if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                    // Case 1: Regular match or '.' match
                    // Result depends on the state before this character
                    dp[i][j] = dp[i - 1][j - 1];
                } else if (p[j - 1] == '*') {
                    // Case 2: The wildcard '*'
                    // The character controlling the star is at p[j-2]
                    char precedingPatternChar = p[j - 2];
                    
                    // Option A: Zero occurrences of preceding char.
                    // Ignore char and '*'. Look back 2 columns.
                    bool matchZero = dp[i][j - 2];
                    
                    // Option B: One or more occurrences.
                    // Only valid if preceding char matches current s char (or is '.')
                    bool matchOneOrMore = false;
                    if (precedingPatternChar == '.' || precedingPatternChar == s[i - 1]) {
                         // Consume one char from s, stay on same pattern state. Look up 1 row.
                        matchOneOrMore = dp[i - 1][j];
                    }
                    
                    dp[i][j] = matchZero || matchOneOrMore;
                }
                // If p[j-1] is a letter that doesn't match s[i-1], dp[i][j] remains false.
            }
        }
        
        // The answer is in the bottom-right corner
        return dp[m][n];
    }
};