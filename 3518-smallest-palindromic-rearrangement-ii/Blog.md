# Cracking the Code of Palindromic Permutations and Combinatorics [#3518]


[Blog Post Link](https://leetcode.com/discuss/post/7555573/cracking-the-code-of-palindromic-permuta-my1c/)



Hello code enthusiasts and algorithm lovers! I'm thrilled to share a fascinating journey into the world of string manipulation and combinatorics. Recently I encountered a problem that beautifully merges the symmetry of palindromes with the logic of lexicographical ordering. It challenges us not just to find a solution, but to find the *exact* permutation hidden among millions of possibilities.

If you have ever wondered how to efficiently navigate through massive search spaces without generating every single option, this post is for you. We are going to build a solution that is both elegant and efficient, capable of handling large inputs with grace.

### Understanding the Challenge

Let us start by clearly defining what we are up against. The problem asks us to perform a very specific task involving a string `s` and an integer `k`.

We are given a string `s` that is guaranteed to be a palindrome. This means it reads the same forwards and backwards. We are also given a number `k`. Our goal is to find the **kth lexicographically smallest palindromic permutation** of `s`.

If the number `k` is larger than the total number of unique palindromic rearrangements possible, we should return an empty string.

**What does this mean effectively?**

Imagine you have the string "abba". The possible palindromic rearrangements are:

1. "abba" (The smallest alphabetically)
2. "baab" (The next one)

If `k` is 1, we return "abba". If `k` is 2, we return "baab". If `k` is 3, we return an empty string because there are only two valid palindromes.

The term "lexicographically smallest" simply means "alphabetical order." Just like words in a dictionary, "apple" comes before "banana". Here, we are looking for the kth "word" in the dictionary of all valid palindromes formed by the letters of `s`.

### The Key Insight The Power of Halves

The first major hurdle is understanding the structure of a palindrome. A palindrome is like a mirror image. The first character must match the last character. The second character must match the second to last character, and so on.

This implies a powerful constraint **the first half of the string dictates the entire string.**

If you determine the first half of a palindrome, the second half is automatically determined. It is just the reverse of the first half.

For example, consider the letters available from the string "racecar".
The counts are:

* a: 2
* c: 2
* e: 1
* r: 2

To build a palindrome, we must split the even counts in half. The single 'e' must go in the very center.
Our "building blocks" for the first half are:

* a: 1
* c: 1
* r: 1

If we arrange these three letters as "acr", the full palindrome becomes "acr" + "e" + "rca" = "acreca".
If we arrange them as "cra", the full palindrome becomes "cra" + "e" + "arc" = "craearc".

**The Breakthrough:**
The problem of finding the *kth lexicographically smallest palindrome* is actually equivalent to finding the *kth lexicographically smallest permutation* of the **first half characters**.

This simplifies our problem immensely. Instead of worrying about the full length of `s`, we only need to arrange half the characters.

### The Mathematical Foundation

Now that we have reduced the problem to "find the kth permutation of a set of characters," we need to talk about counting. To find the kth item without listing them all, we need to be able to count how many permutations exist for a given set of letters.

Suppose we have a collection of characters where some repeat. For example, we want to arrange the letters `a, a, b`.
The total number of letters `N` is 3.
The count of 'a' is 2.
The count of 'b' is 1.

The formula for the number of distinct permutations is the factorial of the total length divided by the product of the factorials of each character count.

`Total Permutations = N! / (countA! * countB! * ...)`

For `a, a, b`:
`3! / (2! * 1!) = 6 / 2 = 3`
The permutations are `aab`, `aba`, `baa`.

We will need a function in our code that can calculate this value. Let us call this function `countWays`. It will tell us, "If I have these remaining letters, how many unique strings can I build?"

### The Greedy Strategy

How do we find the exact kth string without generating all of them? We use a "greedy" approach, building the string character by character from left to right.

Imagine we are building the first half of the palindrome. We have slots to fill: `_ _ _ ...`
We want the resulting string to be alphabetically as small as possible to match the target index `k`.

**The Algorithm Logic:**

1. **Iterate through positions:** Start at the first empty slot of our half string.
2. **Try candidates:** Iterate through the alphabet from 'a' to 'z'. We check if we have any of that character available to use.
3. **The "What If" Scenario:** Suppose we try placing an 'a' in the current slot. We then ask: "If I lock 'a' in this position, how many permutations can I form with the *remaining* characters?" Let us call this number `W`.
4. **Make a Decision:**
* **Case 1:** `k` is less than or equal to `W`.
This means the target string is indeed among the strings that start with 'a'. We confirm 'a' for this position, decrement the count of 'a', and move to the next slot. We do *not* change `k`.
* **Case 2:** `k` is greater than `W`.
This means the target string is *not* one of the strings starting with 'a'. It must be further down the list. We skip 'a'. However, we must account for the fact that we are skipping `W` valid strings. So, we subtract `W` from `k` (`k = k - W`) and try the next available character (like 'b').



This logic allows us to "zoom in" on the correct string character by character.

### Handling Massive Numbers

There is a technical catch. The input string length can be up to 10,000. Calculating the factorial of 5000 is impossible using standard integer types. It would result in a number with thousands of digits.

However, notice the constraint on `k`. `k` is at most 1,000,000.

This is a huge advantage. We only care if the number of ways `W` is smaller or larger than `k`. If `W` is 1,000,001 or if it is 1,000,000,000,000, the outcome is the same for our algorithm: `k <= W`.

Therefore, we can use **Saturation Arithmetic**. Whenever we calculate our combinations or factorials, if the value exceeds `k`, we can just cap it at `k + 1` or a similar small limit. We do not need the exact huge number; we just need to know it is "big enough."

This prevents integer overflow and keeps our calculations fast.

### Step by Step Implementation Details

Let us construct the solution block by block.

**Step 1 Frequency Map**
First, traverse the string `s` and count the occurrence of every character. Store this in an array of size 26.

**Step 2 Isolate the Half**
Identify the middle character. Since `s` is a palindrome, at most one character can have an odd count. Save this character to append later.
Then, divide every character count by 2. This new set of counts represents the characters available for the first half of our palindrome.

**Step 3 The Main Loop**
We need to fill `Length(s) / 2` positions.
For each position `i` from 0 to the end of the half string:

* Loop through characters `c` from 'a' to 'z'.
* If we have `c` available (count > 0):
* Temporarily decrement the count of `c`.
* Calculate `ways`, the number of permutations of the *remaining* letters.
* Check `ways` against `k`.
* If `k <= ways`: We found our character! Append `c` to our result, break the inner loop, and move to the next position.
* If `k > ways`: This character is not the one. Subtract `ways` from `k`, restore the count of `c` (backtrack), and continue to the next character.



**Step 4 Final Assembly**
Once we have the `firstHalf`, we construct the result by combining:
`firstHalf` + `middleChar` + `reversed(firstHalf)`

**Step 5 Edge Case**
If we finish the outer loop and have not successfully built the string (which theoretically should be handled by returning early if `k` remains too large), or if initially `k` is larger than the total permutations, we return an empty string.

### The C++ Solution

Here is the complete C++ code implementing this logic. I have added comments to explain each part.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    string smallestPalindrome(string s, int k) {
        // Step 1 Count frequencies
        vector<int> freq(26, 0);
        for (char c : s) {
            freq[c - 'a']++;
        }
        
        // Step 2 Identify the middle character and prepare half counts
        string mid = "";
        for (int i = 0; i < 26; ++i) {
            if (freq[i] % 2 == 1) {
                mid += (char)('a' + i);
                freq[i]--; // Remove the middle one to make it even
                break;
            }
        }
        
        // Divide by 2 to get the pool for the first half
        for (int i = 0; i < 26; ++i) {
            freq[i] /= 2;
        }
        
        int halfLen = s.length() / 2;
        string currentHalf = "";
        // Use a larger type for k to allow safe arithmetic
        long long currentK = k; 
        
        // Step 3 Construct the first half
        for (int i = 0; i < halfLen; ++i) {
            bool charFound = false;
            
            // Try every letter from 'a' to 'z'
            for (int c = 0; c < 26; ++c) {
                if (freq[c] > 0) {
                    // Try using this character
                    freq[c]--;
                    
                    // Calculate how many completions exist if we pick 'c'
                    long long ways = countPermutations(freq, halfLen - 1 - i, currentK);
                    
                    if (currentK <= ways) {
                        // The target is inside this branch
                        currentHalf += (char)('a' + c);
                        charFound = true;
                        break; // Move to the next position in the string
                    } else {
                        // The target is not here, skip this branch
                        currentK -= ways;
                        freq[c]++; // Backtrack
                    }
                }
            }
            
            // If we tried all chars and found nothing, k is too big
            if (!charFound) return "";
        }
        
        // Step 4 Assemble the final palindrome
        string result = currentHalf + mid;
        reverse(currentHalf.begin(), currentHalf.end());
        result += currentHalf;
        
        return result;
    }

private:
    // Helper function to calculate nCr with a saturation limit
    long long combinations(int n, int r, long long limit) {
        if (r < 0 || r > n) return 0;
        if (r == 0 || r == n) return 1;
        if (r > n / 2) r = n - r; // Symmetry optimization
        
        long long res = 1;
        for (int i = 1; i <= r; ++i) {
            // Formula res = res * (n - i + 1) / i
            // We apply the limit check at each step
            res = res * (n - i + 1);
            res /= i;
            if (res > limit) return limit + 1;
        }
        return res;
    }

    // Helper to calculate multinomial coefficient
    // Returns (totalLen)! / (count1! * count2! * ...)
    long long countPermutations(const vector<int>& counts, int totalLen, long long limit) {
        long long res = 1;
        int currentLen = totalLen;
        
        for (int count : counts) {
            if (count > 0) {
                // We use the property that multinomial coeff is a product of binomial coeffs
                // ways *= Choose(remaining_len, current_char_count)
                long long nCrVal = combinations(currentLen, count, limit);
                
                // Safe multiplication with overflow check
                if (limit / nCrVal < res) {
                    res = limit + 1;
                } else {
                    res *= nCrVal;
                }
                
                if (res > limit) res = limit + 1;
                
                currentLen -= count;
            }
        }
        return res;
    }
};

```

### Deep Dive into the Helper Functions

You might notice the helper functions `combinations` and `countPermutations` are doing the heavy lifting.

**The Combination Logic**
We calculate `nCr` (n choose r) iteratively. A naive factorial approach would fail due to size. Instead, we multiply and divide sequentially: `val = val * (n-i+1) / i`. Since the result of `nCr` is always an integer, the division is always clean. Importantly, inside the loop, we check `if (res > limit)`. If our intermediate value is already larger than `k`, we stop and return `limit + 1`. This saves CPU cycles and prevents overflows.

**The Permutation Logic**
Standard permutation formulas use division: `N! / (A! * B! * C!)`.
Division is messy in integer arithmetic if you are not careful. A better way to think about this is using combinations.
To place `A` items, `B` items, and `C` items into `N` slots:

1. Choose positions for `A`: `Choose(N, A)`
2. Multiply by choosing positions for `B` in the remaining slots: `Choose(N - A, B)`
3. Multiply by choosing positions for `C` in the remaining slots: `Choose(N - A - B, C)`

This method avoids division entirely and keeps our saturation logic simple.

### Why This Works Efficiently

Let us discuss performance.

**Time Complexity:**
The length of the string is `L`. The alphabet size `A` is 26.
For each position in the half string (L/2 iterations), we loop through the alphabet (26 iterations). Inside that, we calculate permutations.
The permutation calculation iterates through the alphabet again.
The combination calculation takes `O(L)` time in the worst case, but since we saturate at `k` (which is small constant 10^6), it effectively becomes constant time `O(1)` or very fast for this specific problem context.
Roughly, the complexity is dominated by `O(L * A^2)`. Given `L=10000`, this is perfectly acceptable.

**Space Complexity:**
We store the frequency map (constant size) and the result string. So, the space complexity is `O(L)` to hold the output.

### Conclusion

Solving the "Smallest Palindromic Rearrangement" is a wonderful exercise in changing your perspective. By realizing that the palindrome is strictly defined by its first half, we simplified a complex string problem into a classic combinatorial ranking problem.

The greedy approach combined with saturation arithmetic is a pattern you will see often in "kth smallest" type problems. It allows us to navigate astronomically large search spaces with surgical precision.

I hope this breakdown helps you understand not just the *how*, but the *why* behind the solution. Combinatorics can be intimidating, but breaking it down into choices and slots makes it much more approachable.

Happy Coding everyone! 

If you found this explanation helpful, go forth and tackle those hard problems with confidence.
