# Solving the Hard Regex Problem without Recursion [#10]

[Blog Post Link](https://leetcode.com/discuss/post/7540547/solving-the-hard-regex-problem-without-r-hlp1/)


Alright everyone, let's talk about Regular Expression Matching.

It’s marked "Hard" for a good reason. Standard string comparison is straightforward, but throwing wildcards into the mix—especially the dreaded `*`—can quickly turn your logic into spaghetti code.

When I first looked at this, my immediate thought was recursion. "Does the first character match? Okay, recurse on the rest." But the recursion gets messy fast, complicated by overlapping subproblems and tricky base cases.

The cleanest, most interview-ready way to tackle this is **Dynamic Programming**. It allows us to build up the solution systematically, handling those tricky edge cases one step at a time.

## The Mental Model

We need to compare string `s` and pattern `p`. Let's imagine a grid where the rows represent prefixes of `s` and the columns represent prefixes of `p`.

We want to define a state `dp[i][j]` which answers the boolean question:

> Does the prefix of `s` of length `i` (i.e., `s[0...i-1]`) match the prefix of `p` of length `j` (i.e., `p[0...j-1]`)?

Our goal is to find the value of `dp[s.length()][p.length()]`.

## The Logic & Transitions

Let's iterate through the grid. Suppose we are currently trying to calculate `dp[i][j]`. We are looking at the character `s[i-1]` and pattern character `p[j-1]`.

Here is the breakdown of possibilities:

### 1. The Easy Case: Simple Match or '.'

If the current pattern character `p[j-1]` is a regular letter (like 'a', 'b') or the dot '.', life is good.

* If `p[j-1]` matches `s[i-1]`, OR if `p[j-1]` is '.', then the result depends entirely on whether the strings *before* these characters matched.
* **Transition:** `dp[i][j] = dp[i-1][j-1]`

### 2. The Hard Case: The '*' Wildcard

This is where things get interesting. Remember, `*` doesn't match a character itself; it modifies the character *before* it to match zero or more times.

When `p[j-1]` is `*`, we look at the pattern character preceding it, let's call it `prev_pattern_char` (which is at `p[j-2]`). We have two main paths to a match:

**Path A: The "Zero Occurrences" Option**
We can decide that the `prev_pattern_char` followed by `*` represents an empty string. Essentially, we ignore both the `*` and the character before it.

* **Logic:** Does `s[0...i-1]` match `p[0...j-3]`?
* **Transition:** We look two steps back in the pattern columns: `dp[i][j-2]`.

**Path B: The "One or More Occurrences" Option**
We can only take this path if the `prev_pattern_char` actually matches current string character `s[i-1]` (or if `prev_pattern_char` is a `.`).

* **Logic:** If they match, we "consume" the current character in `s`, but we *keep* the `*` pattern active because it might match again later.
* **Transition:** We look one step back in the string rows: `dp[i-1][j]`.

If `p[j-1] == '*'`, our result is `(Path A is true) OR (Path B is possible AND true)`.

## The Crucial Initialization (Base Cases)

Before filling the grid, we need to set the ground rules:

1. **Empty vs Empty:** An empty string matches an empty pattern.
`dp[0][0] = true;`
2. **Empty String vs Non-Empty Pattern:** Can an empty string `s` match a pattern like `"a*"` or `"c*a*b*"`? Yes! These patterns can resolve to empty strings via the "Zero Occurrences" rule.
We iterate through the first row (`dp[0][j]`) and handle patterns like `x*`, `x*y*`, etc.

## The Solution Code

Here is the implementation following that logic accurately.

```cpp
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

```

## Complexity

Given `M` is the length of `s` and `N` is the length of `p`:

* **Time Complexity: O(M * N)**. We fill a grid of size , and each cell takes constant time  to compute.
* **Space Complexity: O(M * N)**. We store the entire DP table.


 *Feel free to write your comments below for other better options or insights.*
