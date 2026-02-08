# Mastering String Algorithms by Finding the Longest Palindrome Across Two Strings [#3504]

[Blog Post Link](https://leetcode.com/discuss/post/7563303/mastering-string-algorithms-by-finding-t-fw7v/)


Welcome friends and fellow coding enthusiasts! Today we are going to embark on a deep dive into a fascinating string manipulation challenge. We often encounter problems asking us to find palindromes within a single string, but what happens when we introduce a second string into the mix? This adds a delightful layer of complexity that requires us to think about string structure, symmetry, and efficient matching all at once.

In this extensive guide, we will explore a method to solve the "Longest Palindrome After Substring Concatenation II" problem. My goal is to break this down into extremely digestible pieces so that by the end, you will feel completely comfortable tackling similar hard level string challenges. We will walk through the logic, the algorithm design, and the code implementation step by step.

### Understanding the Challenge

Let us first ground ourselves in the problem statement. Imagine you are holding two separate strings in your hands, let us call them String S and String T. Your task is to play the role of a craftsman. You are allowed to cut out one continuous piece (a substring) from String S and one continuous piece from String T.

Once you have these two pieces, you must glue them together in a specific order: the piece from S comes first, and the piece from T comes second.

The goal? You want the final combined string to be a palindrome. A palindrome, as you likely know, is a word or phrase that reads the same backward as forward, like "radar" or "noon". Furthermore, you want to make this resulting palindrome as long as possible.

Let us look at a few examples to visualize this.

**Example 1**

* String S is "abcde"
* String T is "ecdba"
* If we cut "abc" from S and "ba" from T, we combine them to form "abcba".
* "abcba" reads the same forwards and backwards. Its length is 5. This turns out to be the longest possible result here.

**Example 2**

* String S is "a"
* String T is "a"
* We can take "a" from S and "a" from T.
* Result is "aa". Length is 2.

**Example 3**

* String S is "b"
* String T is "aaaa"
* Here, we can take an empty string from S (selecting nothing) and "aaaa" from T.
* Result is "aaaa". Length is 4.

The core difficulty lies in the fact that there are thousands of possible substrings in S and thousands in T. Trying every single combination would be impossibly slow. We need a smarter way to see the structure of the solution.

### Visualizing the Palindrome Structure

To solve this efficiently, we must stop thinking about "picking substrings" and start thinking about "building a palindrome". Any palindrome formed by joining a part of S and a part of T will generally look like a symmetrical structure.

We can categorize the final palindrome into three logical components:

1. **The Left Wing:** This is the beginning part of the palindrome.
2. **The Core:** This is the middle part of the palindrome.
3. **The Right Wing:** This is the ending part of the palindrome.

Because we are concatenating S then T, the "seam" where the two strings join must be somewhere inside this palindrome. This leads us to two primary scenarios based on where the center of the palindrome lands.

#### Scenario A: The Center is inside the piece from String S

In this case, the palindrome looks like this:
`[Left Part of S] + [Palindrome Core in S] + [Right Part of S] + [Piece from T]`

For this to be a valid total palindrome, the `[Left Part of S]` must be a mirror image of the `[Piece from T]`. The `[Palindrome Core in S]` acts as the center, and the `[Right Part of S]` is just the right side of that core (which mirrors the left side of the core).

Actually, let us simplify. If the center is in S, the structure is roughly:
`[Matching Suffix in S] + [Palindrome Center in S] + [Matching Suffix in S (reversed)]`

Wait, that is not quite right because the right side comes from T. Let us rephrase.
The palindrome is composed of a **Central Palindrome** found entirely within S, surrounded by wings.

* The Left Wing is a substring in S immediately to the left of the center.
* The Right Wing is a substring in T.
* For the total string to be a palindrome, the **Right Wing (from T)** must be the exact reverse of the **Left Wing (from S)**.

Therefore, if we pick a palindrome centered somewhere in S, we want to extend it outwards as much as possible. Expanding inside S is easy (it is just a bigger palindrome in S). But eventually, we hit the right end of our chosen substring in S. At that point, the "rest" of the palindrome must come from T.
The requirement is: The part of S to the left of our core must exist as a substring in T (but reversed).

#### Scenario B: The Center is inside the piece from String T

This is the symmetric case. The palindrome center lies inside the substring we chose from T.

* The Right Wing is a substring in T immediately to the right of the center.
* The Left Wing is a substring in S.
* The **Left Wing (from S)** must be the exact reverse of the **Right Wing (from T)**.

#### Scenario C: The Center is exactly at the join

This effectively means the core is empty, or the core is just an even split. This is covered by the logic of matching wings perfectly. For example, if S ends in "abc" and T starts with "cba", we get "abccba". The center is right between c and c.

### The Algorithm Strategy

Now we can formulate a plan. We cannot check every substring, but we *can* check every possible palindrome center.

**Step 1: Precomputing Matches (The Wings)**
Before we look for palindromes, we need to know how well S and T can "mate" to form the wings.

* For every position `i` in S, we want to know: "What is the longest suffix of the string `S` ending at `i` that appears somewhere in `T` as a reversed substring?" Let us call this `max_match_s[i]`.
* Similarly, for every position `j` in T, we want to know: "What is the longest prefix of the string `T` starting at `j` that appears somewhere in `S` as a reversed substring?" Let us call this `max_match_t[j]`.

This sounds like the **Longest Common Substring** problem. If we reverse T (let us call it `Tr`), finding a substring in T reversed is the same as finding it in `Tr` normally.

**Step 2: Iterating Centers (The Core)**
We will iterate through every possible center in S. A center can be a single character (for odd length palindromes) or the gap between characters (for even length palindromes).

* For a palindrome in S spanning from index `L` to `R`, its length is `R minus L plus 1`.
* We can add "wings" to this palindrome. The Left Wing would be the substring in S ending at `L minus 1`.
* We check our precomputed `max_match_s[L minus 1]`. Let's say this value is `K`. This means we can find a matching wing in T of length `K`.
* Total length = (Length of core in S) + 2 * `K`.

We do the same for centers in T.

* For a palindrome in T spanning from index `L` to `R`.
* The Right Wing starts at `R plus 1`.
* We check `max_match_t[R plus 1]`.
* Total length = (Length of core in T) + 2 * `K`.

**Step 3: Track the Maximum**
We simply keep a running counter of the largest length we have constructed.

### Detailed Implementation Guide

Now, let us get into the specifics of how to code this without errors.

#### Phase 1: Dynamic Programming for Matches

We need to compare String S and the reverse of String T (let us call it `Tr`).
We want to fill an array `max_match_s` where `max_match_s[i]` is the length of the longest suffix of `S[0...i]` that is a substring of `Tr`.

To do this, we can use a 2D Dynamic Programming table, let's call it `dp`.
`dp[i][j]` will store the length of the common suffix between the prefix of S ending at `i` and the prefix of `Tr` ending at `j`.

* If `S[i]` equals `Tr[j]`, then `dp[i][j] = dp[i minus 1][j minus 1] + 1`.
* Otherwise, `dp[i][j] = 0`.

While filling this row for `S[i]`, we might find matches ending at various positions in `Tr`. We are interested in the *best* match found anywhere in `Tr`. So, `max_match_s[i]` will be the maximum value found in the entire row `i` of our DP table.

We repeat this process symmetrically for String T. We compare String T and the reverse of S (let's call it `Sr`) to build `max_match_t`.

#### Phase 2: Expanding Palindromes

We need a helper logic to expand palindromes. For a string, we pick a center and expand pointers `left` and `right` outwards as long as the characters match.

* **Odd length:** Start `left` and `right` at the same index `i`.
* **Even length:** Start `left` at `i` and `right` at `i plus 1`.

As we expand, we have a valid palindrome core. At each step of expansion, we calculate the total candidate length using our precomputed match arrays.

### Complexity Analysis

Let N be the length of S and M be the length of T.

**Time Complexity:**

1. **DP Calculation:** We have a nested loop comparing S and reversed T. This takes roughly `N multiplied by M` operations.
2. **Palindrome Expansion:** We expand around every center. In the worst case (like a string "aaaaa"), expanding takes `O(N squared)` time for S and `O(M squared)` for T.
Given that N and M are up to 1000, `N squared` is 1,000,000. `N multiplied by M` is also 1,000,000. These are very safe numbers for modern processors, which can handle roughly 100 million operations per second. The solution is efficient enough.

**Space Complexity:**
We use a 2D DP table of size `N by M` to compute the matches. For 1000x1000 integers, this consumes about 4 Megabytes of memory, which is well within standard limits (usually 256MB or 512MB). We also store the `max_match` arrays which are small `O(N)` and `O(M)`.

### The C++ Solution

Here is the complete, clean C++ code implementing the logic we discussed. I have added comments to guide you through each section.

```cpp
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
        
        // Prepare the reversed versions of the strings for matching
        string tr = t;
        reverse(tr.begin(), tr.end());
        
        string sr = s;
        reverse(sr.begin(), sr.end());
        
        // ==========================================
        // Step 1: Compute max_match_s
        // max_match_s[i] will store the length of the longest suffix 
        // of s[0...i] that appears anywhere in t reversed.
        // ==========================================
        vector<int> max_match_s(n, 0);
        
        // We use a local scope for the DP table to free memory early
        {
            // dp[i][j] stores the length of the common suffix ending 
            // at s[i-1] and tr[j-1].
            // Dimensions are (n+1) x (m+1) to handle 1-based indexing easily.
            vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
            
            for (int i = 1; i <= n; ++i) {
                for (int j = 1; j <= m; ++j) {
                    if (s[i-1] == tr[j-1]) {
                        dp[i][j] = dp[i-1][j-1] + 1;
                    } else {
                        dp[i][j] = 0;
                    }
                    // For the position (i-1) in s, we want the longest match 
                    // found against ANY position in tr.
                    max_match_s[i-1] = max(max_match_s[i-1], dp[i][j]);
                }
            }
        }
        
        // ==========================================
        // Step 2: Compute max_match_t
        // max_match_t[i] will store the length of the longest prefix 
        // of t[i...m-1] that appears anywhere in s reversed.
        // ==========================================
        vector<int> max_match_t(m, 0);
        {
            // dp[i][j] stores length of common prefix of t[i...] and sr[j...]
            // We fill this backwards to handle prefixes naturally.
            vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
            
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
        
        // ==========================================
        // Step 3: Iterate Palindromes in S (Center in S)
        // ==========================================
        for (int i = 0; i < n; ++i) {
            // Check Odd Length Palindromes centered at i
            for (int l = i, r = i; l >= 0 && r < n && s[l] == s[r]; --l, ++r) {
                int core_len = r - l + 1;
                int wing_len = 0;
                // If there is space to the left, check for a matching wing from t
                if (l > 0) wing_len = max_match_s[l-1];
                ans = max(ans, core_len + 2 * wing_len);
            }
            
            // Check Even Length Palindromes centered at i and i+1
            for (int l = i, r = i + 1; l >= 0 && r < n && s[l] == s[r]; --l, ++r) {
                int core_len = r - l + 1;
                int wing_len = 0;
                if (l > 0) wing_len = max_match_s[l-1];
                ans = max(ans, core_len + 2 * wing_len);
            }
        }
        
        // ==========================================
        // Step 4: Iterate Palindromes in T (Center in T)
        // ==========================================
        for (int i = 0; i < m; ++i) {
            // Odd Length Palindromes centered at i
            for (int l = i, r = i; l >= 0 && r < m && t[l] == t[r]; --l, ++r) {
                int core_len = r - l + 1;
                int wing_len = 0;
                // If there is space to the right, check for a matching wing from s
                if (r < m - 1) wing_len = max_match_t[r+1];
                ans = max(ans, core_len + 2 * wing_len);
            }
            
            // Even Length Palindromes centered at i and i+1
            for (int l = i, r = i + 1; l >= 0 && r < m && t[l] == t[r]; --l, ++r) {
                int core_len = r - l + 1;
                int wing_len = 0;
                if (r < m - 1) wing_len = max_match_t[r+1];
                ans = max(ans, core_len + 2 * wing_len);
            }
        }
        
        // Edge Case: The palindrome might be purely wings (empty core at the boundary)
        // This is implicitly covered, but let's ensure we consider just the matches
        for (int x : max_match_s) ans = max(ans, 2 * x);
        
        return ans;
    }
};

```

### Walkthrough of Important Edge Cases

It is always good practice to verify our logic against edge cases.

1. **Disjoint Strings:** If S is "abc" and T is "xyz", no characters match.
* The `max_match` arrays will be all zeros.
* We will find palindromes inside S alone (like "a", "b", "c") and T alone. The code handles this because `wing_len` will be 0.
* Correct output: 1.


2. **Full Overlap:** If S is "abc" and T is "cba".
* The match logic will see that "abc" in S matches "cba" (reversed) in T.
* The loop will eventually find the match length 3.
* Result: `3 multiplied by 2` = 6.
* Correct output: 6.


3. **Single Characters:** S="a", T="a".
* `max_match_s` will find a match of length 1.
* The loop checks palindrome "a" in S. Left wing is empty, match is 0. But wait, the pure match logic (empty core) sees the match of 1.
* Alternatively, consider the even palindrome logic or simply that a match of length 1 implies "aa" is possible.
* The line `ans = max(ans, 2 * x)` handles the case where we just glue a suffix of S and prefix of T directly without a central pivot character.



### Conclusion

And there you have it! We have successfully deconstructed a hard string problem by breaking it down into manageable components. By separating the concept of the "Palindrome Core" from the "Matching Wings," we transformed a complex concatenation problem into a combination of standard Palindrome Expansion and Longest Common Substring logic.

This approach is robust, easy to read, and efficient enough to pass all constraints. 
I hope this explanation has helped you gain a better intuition for string algorithms. Happy coding, and may your test cases always pass!
