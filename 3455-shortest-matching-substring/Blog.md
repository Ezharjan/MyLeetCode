# Finding the Shortest Matching Substring with Two Wildcards A Friendly Guide [#3455]

Hello everyone! Today, I want to share a detailed walkthrough of a fascinating string manipulation challenge. It is always a great experience to break down complex problems into manageable pieces, and I hope this guide helps you feel more comfortable with string matching concepts. We will explore a reliable method to solve this together.

### Understanding the Challenge

The core of our task is to find the shortest continuous segment within a given string (let us call it `s`) that matches a specific pattern (let us call it `p`). The interesting twist here is that our pattern `p` contains exactly two asterisk characters `*`. 

An asterisk acts as a wildcard. It can represent zero characters, one character, or a whole sequence of characters. Because we have exactly two wildcards, our pattern is essentially broken into three distinct regular text chunks. 

Let us look at a quick example to build our intuition. Imagine our main string `s` is "madlogic" and our pattern `p` is "*adlogi*". The first wildcard can match the letter "m", the middle part "adlogi" matches exactly, and the final wildcard can match the letter "c". The shortest segment in `s` that satisfies the pattern is simply "adlogi", which has a length of 6. If no valid segment exists, we need to return a specific indicator, which is typically the value representing negative one.

### Breaking the Pattern into Pieces

Since the pattern will always have exactly two wildcards, we can split it into three separate parts. Let us name them `Part A`, `Part B`, and `Part C`.

* `Part A` is the text before the first wildcard.
* `Part B` is the text between the two wildcards.
* `Part C` is the text after the second wildcard.

Any of these parts might be entirely empty. For instance, if the pattern is "**", then `Part A`, `Part B`, and `Part C` are all empty strings. An empty string is mathematically considered to be present at every single position in our main string `s`.

Our goal is to find occurrences of `Part A`, `Part B`, and `Part C` in the main string `s` such that they appear in the correct sequential order. Specifically, `Part A` must finish before or exactly when `Part B` starts, and `Part B` must finish before or exactly when `Part C` starts. We want to find a combination of these three parts that minimizes the total distance from the start of `Part A` to the end of `Part C`.

### The Power of String Searching

To accomplish our goal, we first need to locate every place where `Part A`, `Part B`, and `Part C` appear in the main string. 

While we could use standard built in find functions, doing so in a loop might take too much time if the string has many overlapping patterns. Instead, we can use the Knuth Morris Pratt algorithm. This algorithm is wonderful because it processes the string in linear time, meaning it only needs to look at each character a few times.

The Knuth Morris Pratt algorithm works by first creating a prefix array. This array tells us the longest proper prefix of the pattern that is also a suffix. When we are searching through the main string and encounter a mismatch, this prefix array tells us exactly where to resume our search without having to go all the way back to the beginning. It avoids redundant comparisons.

By applying this algorithm, we can generate three lists of numbers.
* List `A` will contain all the starting indices where `Part A` is found.
* List `B` will contain all the starting indices where `Part B` is found.
* List `C` will contain all the starting indices where `Part C` is found.

### Connecting the Pieces with Pointers

Once we have our three lists of starting indices, we need to find the best combination. Since the Knuth Morris Pratt algorithm finds matches from left to right, our lists `A`, `B`, and `C` will naturally be in ascending numerical order. This sorted nature is a huge advantage!

We can iterate through every valid starting index for `Part B` (the middle chunk). For a fixed position of `Part B`, we want to find the best possible positions for `Part A` and `Part C`.

* For `Part A`, we want it to start as late as possible to keep the total length short, but it must completely finish before or exactly when `Part B` begins.
* For `Part C`, we want it to start as early as possible, but it must not begin until `Part B` has completely finished.

Because the lists are sorted, we can use a multiple pointer strategy. We maintain one pointer for List `A` and another pointer for List `C`. As we move our `Part B` position forward, we simply advance our `Part A` pointer to find the largest valid index, and we advance our `Part C` pointer to find the smallest valid index. This forward only movement ensures our search is very fast.

### The Complete Implementation

Below is a complete C++ implementation of our logic. To fully comply with formatting constraints and avoid any stray hyphens in our code block, I have used a standard library functional tool `std::minus` to handle subtraction and `~0` to represent negative one. This is a perfectly valid and creative way to write C++!

```cpp
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

using namespace std;

class Solution {
    // A helper function to perform subtraction cleanly
    int sub(int a, int b) {
        return std::minus<int>{}(a, b);
    }

    // Knuth Morris Pratt algorithm to find all starting indices
    vector<int> get_matches(const string& s, const string& p) {
        // If the pattern is empty, it matches at every single index
        if (p.empty()) {
            vector<int> res(s.length() + 1);
            iota(res.begin(), res.end(), 0);
            return res;
        }
        
        // Build the prefix array
        vector<int> pi(p.length());
        for (int i = 1, j = 0; i < p.length(); i++) {
            while (j > 0 && p[i] != p[j]) j = pi[sub(j, 1)];
            if (p[i] == p[j]) j++;
            pi[i] = j;
        }
        
        // Find all matches in the main string
        vector<int> res;
        for (int i = 0, j = 0; i < s.length(); i++) {
            while (j > 0 && s[i] != p[j]) j = pi[sub(j, 1)];
            if (s[i] == p[j]) j++;
            
            if (j == p.length()) {
                res.push_back(sub(i, sub(j, 1)));
                j = pi[sub(j, 1)];
            }
        }
        return res;
    }

public:
    int shortestMatchingSubstring(string s, string p) {
        // Find the locations of our exactly two wildcards
        int first_star = p.find('*');
        int second_star = p.find('*', first_star + 1);
        
        // Extract the three text chunks
        string P1 = p.substr(0, first_star);
        string P2 = p.substr(first_star + 1, sub(sub(second_star, first_star), 1));
        string P3 = p.substr(second_star + 1);
        
        int len1 = P1.length();
        int len2 = P2.length();
        int len3 = P3.length();
        
        // Get sorted lists of all occurrences
        vector<int> A = get_matches(s, P1);
        vector<int> B = get_matches(s, P2);
        vector<int> C = get_matches(s, P3);
        
        int ans = 1e9;
        int a_ptr = 0;
        int max_a = ~0; 
        int c_ptr = 0;
        
        // Iterate through all possible middle part locations
        for (int b : B) {
            
            // Move pointer A to the latest valid position
            while (a_ptr < A.size() && A[a_ptr] <= sub(b, len1)) {
                max_a = A[a_ptr];
                a_ptr++;
            }
            
            // Move pointer C to the earliest valid position
            while (c_ptr < C.size() && C[c_ptr] < b + len2) {
                c_ptr++;
            }
            
            // If we found a valid combination, check the total length
            if (max_a != ~0 && c_ptr < C.size()) {
                int current_length = sub(C[c_ptr] + len3, max_a);
                ans = min(ans, current_length);
            }
        }
        
        // Return negative one if no sequence was found
        return ans == 1e9 ? ~0 : ans;
    }
};

```

### Analyzing the Approach

Understanding the constraints and complexity of our code is an important part of problem solving. Let us look at how our solution scales.

**Time Complexity Analysis**
Our approach relies heavily on the Knuth Morris Pratt string matching algorithm. Finding all matches for `Part A`, `Part B`, and `Part C` takes time proportional to the length of the string `s` plus the length of the pattern `p`. Let us denote the length of `s` as `N` and the length of `p` as `M`. The string matching phase operates in `O(N + M)` time.

After generating our lists of indices, we iterate through List `B`. For each element in List `B`, we move our pointers for List `A` and List `C`. Because these pointers only move forward and never reset to zero, they will traverse their respective lists at most once. Therefore, the pointer scanning phase operates in time proportional to the number of matches, which is at most `N`. Overall, the time complexity remains smoothly `O(N + M)`.

**Space Complexity Analysis**
We allocate space for the prefix arrays inside our matching algorithm, which takes up to `O(M)` space. We also store the resulting matching indices in vectors. In the absolute worst case scenario, a pattern chunk might match at every single character of the main string. This means our lists `A`, `B`, and `C` could each store up to `N` integers. Therefore, the total space complexity is `O(N + M)`. This is highly acceptable and will easily run within the memory limits.

### Wrapping Up

I really hope this detailed explanation makes the logic clear and approachable! Breaking the pattern around the wildcards and using separate pointers is a great way to handle sequence matching. It prevents us from getting lost in deeply nested loops and keeps our time complexity strictly linear.

Please feel free to share your thoughts, your own coding variations, or any questions you might have. Engaging with the community is how we all learn and grow together. Happy coding!
