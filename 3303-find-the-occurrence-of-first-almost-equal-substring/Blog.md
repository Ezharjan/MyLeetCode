# Unlocking the Mystery of Nearly Perfect String Matches with the Z Algorithm [#3303]

[Blog Post Link](https://leetcode.com/discuss/edit/8W8iOYwN/7556294/)


Hello to all my fellow coding enthusiasts! I hope you are having a wonderful day of problem solving.

Today I want to share my journey tackling a really interesting challenge that deals with string manipulation. It is a problem that tests our ability to handle large datasets efficiently while looking for patterns that are *almost* perfect, but not quite. The problem is technically known as finding the **Occurrence of First Almost Equal Substring**, and it offers a fantastic opportunity to learn about advanced string matching techniques.

I spent some time scratching my head over this one, and while the solution I am sharing below works well, I want to be upfront that there are many ways to solve this! I have seen brilliant engineers use Rolling Hashes or the KMP algorithm to achieve similar results. The beauty of computer science lies in this variety. What I am presenting here is a solution using the **Z Algorithm**, simply because I find it elegant and robust for pattern matching tasks.

Let us dive deep into the logic, the pitfalls, and the code!

### Understanding the Problem

Before we write a single line of code, we must understand exactly what is being asked.

We are given two strings. Let us call the main text **s** and the pattern we are looking for **pattern**.
Our goal is to find a substring inside **s** that is "almost equal" to **pattern**.

**What does "almost equal" mean?**
The problem defines two strings as almost equal if we can change **at most one** character in the first string to make it identical to the second string.

This implies two possibilities for a valid match:

1. The substring is already identical to the pattern (0 changes needed).
2. The substring differs from the pattern by exactly one character (1 change needed).

We need to return the **smallest starting index** of such a substring in **s**. If no such substring exists, we return negative one.

**Let us look at an example to clarify.**

Imagine **s** is `"ababbababa"` and **pattern** is `"bacaba"`.

We slide our pattern along the string **s** to see if it fits.

* If we look at the substring starting at index 0 (`"ababba"`), it has too many differences compared to `"bacaba"`.
* However, let us look at the substring starting at index 4 (`"bababa"`).
* Pattern: `bacaba`
* Substring: `bababa`
* Notice that index 2 is `'c'` in the pattern and `'b'` in the substring.
* Every other character matches perfectly.
* Since only **one** character is different, this is a valid "almost equal" match!
* The answer would be index 4.



### Why the Naive Approach Fails

When I first looked at this, my immediate instinct was to check every single position.

The logic looked like this:

1. Loop through every possible starting position in **s**.
2. For each position, compare the characters of the substring against the pattern one by one.
3. Count the mismatches.
4. If the mismatch count is 0 or 1, we found our answer!

This sounds correct, right? Logic wise, it is. But let us look at the **constraints**.
The length of **s** and **pattern** can be up to 100,000 (10^5).

If we use a nested loop (one loop for the starting position, one loop for comparing characters), the time complexity becomes **O(N * M)**.

* If N is 100,000 and M is 50,000, that is roughly 5,000,000,000 operations.
* Most coding platforms, including LeetCode, will time out if you exceed roughly 100 million operations.

So, the brute force approach will get a "Time Limit Exceeded" error. We need something faster. We need a way to compare strings in linear time, or **O(N + M)**.

### The Core Insight: Prefix and Suffix

To solve this efficiently, we need to reframe how we view the "at most one mismatch" condition.

If a substring matches the pattern with only one error, it means the match looks like this:

* A perfectly matching **prefix** (left side).
* One bad character (the mismatch).
* A perfectly matching **suffix** (right side).

Or, if there is no error (perfect match), the prefix covers the whole string and the suffix is empty (or vice versa).

So, for any alignment of the pattern against the text, we check:
**Does the length of the matching prefix plus the length of the matching suffix equal at least (Pattern Length minus 1)?**

If `Length(Prefix Match) + Length(Suffix Match) >= M - 1`, then the missing part is at most 1 character long. This is exactly what we need!

### The Tool: The Z Algorithm

To implement this insight, we need a fast way to calculate "Length of Matching Prefix" and "Length of Matching Suffix" for *every* position in the string.

This is where the **Z Algorithm** shines.

The Z Algorithm processes a string and produces an array (called the Z array) where the value at index `i` tells us the length of the longest substring starting at `i` that is also a prefix of the string.

**How we use it for Prefix Matching:**
If we create a new string `T = pattern + "#" + s` (where # is a special separator character), and run the Z Algorithm on `T`, the Z values corresponding to the part of `s` will tell us exactly how long the prefix match is at that position.

**How we use it for Suffix Matching:**
Suffixes are just prefixes if you read backwards!
If we **reverse** the pattern and **reverse** the string s, we can use the exact same Z Algorithm logic. We create `T_prime = reverse(pattern) + "#" + reverse(s)`. The Z values here will tell us how long the suffix match is (reading from right to left).

### The Step by Step Strategy

Here is the exact plan we will follow in our code.

**Step 1: Calculate Prefix Matches**
We construct a string `ForwardString = pattern + "$" + s`.
We compute the Z array for this string. Let us call the relevant part of this array `pre`.
`pre[i]` will store the length of the longest common prefix between `pattern` and the substring of `s` starting at index `i`.

**Step 2: Calculate Suffix Matches**
We reverse both `s` and `pattern`.
We construct a string `BackwardString = reversed_pattern + "$" + reversed_s`.
We compute the Z array for this string. Let us call the relevant part of this array `suf`.
`suf[i]` will store the length of the longest common suffix between `pattern` and the substring of `s` ending at index `i`.
*Note: We have to be very careful mapping the reversed indices back to the original indices!*

**Step 3: Iterate and Check**
We loop through every valid starting index `i` in string `s` (from 0 to `n - m`).
For each `i`, we identify the end of the substring `j = i + m - 1`.
We check the condition: `pre[i] + suf[j] >= m - 1`.
The first index `i` that satisfies this is our answer.

### The Code Implementation

Here is the C++ solution. I have written this to be as clear as possible. I am not using obscure shortcuts because readability is key!

```cpp
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    /**
     * Helper function to compute the Z array.
     * * The Z array z[i] stores the length of the longest common prefix 
     * between the string str and the suffix of str starting at i.
     * * Time Complexity: O(Length of str)
     */
    vector<int> computeZ(const string& str) {
        int n = str.length();
        vector<int> z(n, 0); // Initialize with 0
        
        // l and r maintain the interval [l, r] which is the prefix match found so far
        int l = 0, r = 0;
        
        for (int i = 1; i < n; ++i) {
            // If i is inside the current Z box
            if (i <= r) {
                z[i] = min(r - i + 1, z[i - l]);
            }
            
            // Try to extend the match as far as possible
            while (i + z[i] < n && str[z[i]] == str[i + z[i]]) {
                z[i]++;
            }
            
            // If we extended past r, update the Z box
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
        
        // Corner case check (though constraints say strings are non empty)
        if (m > n) return -1;
        
        // ---------------------------------------------------------
        // Step 1: Compute Prefix Matches (Forward direction)
        // ---------------------------------------------------------
        
        // Create the combined string for Z-algo: pattern + separator + text
        // We use a separator that does not appear in the input (like '#')
        // to stop matches from overflowing between pattern and text.
        string fwd_str = pattern + "#" + s;
        
        // Compute Z array
        vector<int> z_pre_full = computeZ(fwd_str);
        
        // We only care about the values corresponding to the string 's'.
        // These start after "pattern" + "#".
        // The pattern has length m, separator is length 1.
        // So s starts at index m + 1.
        // Let us create a clean reference or just access directly later.
        
        // ---------------------------------------------------------
        // Step 2: Compute Suffix Matches (Backward direction)
        // ---------------------------------------------------------
        
        // Reverse both strings to treat suffixes as prefixes
        string s_rev = s;
        reverse(s_rev.begin(), s_rev.end());
        
        string p_rev = pattern;
        reverse(p_rev.begin(), p_rev.end());
        
        // Create the combined string: reversed_pattern + separator + reversed_text
        string bwd_str = p_rev + "#" + s_rev;
        
        // Compute Z array for the reversed version
        vector<int> z_suf_full = computeZ(bwd_str);
        
        // ---------------------------------------------------------
        // Step 3: Iterate through all valid starting positions
        // ---------------------------------------------------------
        
        // We can start a substring at any index i from 0 up to n - m
        for (int i = 0; i <= n - m; ++i) {
            
            // 1. Get length of prefix match starting at s[i]
            // In the fwd_str, s[i] is located at index (m + 1 + i)
            int len_pre = z_pre_full[m + 1 + i];
            
            // Optimization: If prefix match covers the whole pattern, 
            // we have 0 mismatches, which is valid (at most 1).
            if (len_pre >= m) {
                return i;
            }
            
            // 2. Get length of suffix match ending at s[i + m - 1]
            // The substring ends at index: end_idx = i + m - 1
            // We need to map this index to the reversed string s_rev.
            // In a reversed string of length n, index k maps to (n - 1 - k).
            int end_idx = i + m - 1;
            int rev_idx = n - 1 - end_idx; 
            
            // In the bwd_str, s_rev[rev_idx] is located at index (m + 1 + rev_idx)
            int len_suf = z_suf_full[m + 1 + rev_idx];
            
            // 3. Check the "Almost Equal" Condition
            // We need the matching prefix + matching suffix to cover 
            // at least (m - 1) characters. This leaves room for exactly 
            // 0 or 1 mismatch gap.
            if (len_pre + len_suf >= m - 1) {
                return i; 
            }
        }
        
        // If we check all positions and find nothing
        return -1;
    }
};

```

### Detailed Breakdown of the Logic

I want to spend a moment clarifying the index math because that is where bugs usually hide.

**The Prefix Part**
We constructed `pattern + "#" + s`.
The string `s` starts at index `m + 1`.
So, if we want to know the prefix match length starting at `s[i]`, we look at `z_pre_full[m + 1 + i]`. This is straightforward.

**The Suffix Part (The Tricky Part)**
We reversed everything.
Original `s`: `0 1 2 3 4 ...`
Reversed `s`: `... 4 3 2 1 0` (conceptually)

If we are checking a substring in `s` that starts at `i` and has length `m`, it ends at `i + m - 1`.
We need to know how well the **end** of this substring matches the **end** of the pattern.
In the reversed world, the "end" of the substring becomes the "start" of the segment.
The index conversion formula for reversing a 0 indexed array of size N is:
`New Index = (N - 1) - Old Index`

So, the end index `i + m - 1` becomes `n - 1 - (i + m - 1)` in `s_rev`.
Then, we shift it by `m + 1` to account for the `pattern + "#"` header in our `bwd_str`.
This gives us the correct lookup index for the Z value.

**The Condition**
Why is the condition `len_pre + len_suf >= m - 1`?

Imagine `pattern` is `abcde` (length 5).
We are matching against `abxde`.

* Prefix match `ab` has length 2.
* Suffix match `de` has length 2.
* Total match = 4.
* Pattern Length - 1 = 4.
* 4 >= 4 is True.

This works because `ab` covers indices 0,1. The mismatch is at 2. `de` covers indices 3,4.
The gap is exactly 1 character (index 2).

What if we have `abcxe`?

* Prefix `abc` (len 3).
* Suffix `e` (len 1).
* Total 4. Valid.

What if we have `abxye` (2 errors)?

* Prefix `ab` (len 2).
* Suffix `e` (len 1).
* Total 3.
* 3 is not >= 4. Invalid.

This logic holds up perfectly!

### Complexity Analysis

Let us talk about efficiency. This solution is designed to scale.

**Time Complexity: O(N + M)**

* Calculating the Z array takes time proportional to the length of the string.
* We run the Z algorithm twice. Once on a string of size roughly `N+M`, and again on a string of size roughly `N+M`.
* The final loop iterates `N` times, doing constant work inside (just index math and addition).
* Therefore, the total time is linear. For `N=100,000`, this runs instantly.

**Space Complexity: O(N + M)**

* We create new strings that combine `pattern` and `s`.
* We create Z arrays of size `N + M`.
* This is well within the memory limits of modern coding environments.

### Final Thoughts

This problem is a classic example of how combining two simple views (looking forward and looking backward) can solve a complex matching problem.

The Z Algorithm is a tool worth keeping in your back pocket. It is often faster to write and easier to understand than KMP once you grasp the concept of the Z box.

However, do not feel discouraged if you solved this using a different method!

* **Hashers:** You can solve this by computing rolling hashes for prefixes and suffixes and checking equality in O(1). That is also O(N) overall and very clever.
* **KMP Users:** You can adapt the KMP failure function to jump across a mismatch.

Programming is about critical thinking and finding the tool that fits your style. I hope this explanation clarified the logic behind finding "almost equal" substrings.

Keep coding, keep learning, and enjoy the process of discovery! If you have any questions or found a cleaner way to handle the indices, I would love to hear about it.

Happy Coding!
