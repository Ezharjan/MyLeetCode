# Unlocking the Smallest Palindromic Permutation Greater Than A Target String [#3734]

Hello everyone! Today I want to share a thorough walkthrough for a fascinating string manipulation puzzle. Sometimes we encounter problems that seem heavily reliant on permutations or complex generation algorithms, but upon closer inspection, they reveal a beautiful underlying greedy logic. This post is dedicated to exploring one such puzzle in a friendly, detailed, and accessible way. I hope this explanation helps anyone who might be stuck or simply wants to understand the mechanics behind palindromes and lexicographical sorting.

### Understanding the Goal

Imagine you are given two strings of the exact same length. The first string is your source material, and the second string is your hurdle. Your objective is to rearrange the characters of your source material to create a new string that satisfies three specific conditions perfectly.

First, the new string must be a valid permutation of your source material. This means you must use every single character exactly as many times as it appears in the original string. You cannot add new letters, and you cannot leave any behind.

Second, the new string must be a perfect palindrome. A palindrome reads exactly the same forwards and backwards. 

Third, your newly formed palindrome must be strictly greater than the hurdle string when we compare them in alphabetical order (which we call lexicographical order). 

Finally, out of all the possible strings that meet these three conditions, you must return the absolute smallest one in alphabetical order. If it is impossible to meet all these conditions, we just return an empty string.



### Breaking Down the Mechanics of a Palindrome

To solve this efficiently, we first need to understand how palindromes are constructed. We do not need to build the entire string at once. Because a palindrome is perfectly symmetrical, the entire structure is dictated completely by its first half. 

Let us count the frequency of each character in our source string. For a string to be rearranged into a palindrome, it must have a very specific set of character counts. Characters must appear in pairs so they can be placed symmetrically on the left and right sides. 

If the total length of the string is an even number, every single character must appear an even number of times. If the total length is an odd number, exactly one character is allowed to have an odd frequency, and that specific character will sit precisely in the exact center of our palindrome.

If we count the frequencies and find more than one character with an odd count, we can immediately stop. It is mathematically impossible to form a palindrome, so we can safely return an empty string.

### The Power of the Left Half

Once we confirm a palindrome is possible, we can gather all the characters that will make up the left side of our string. We simply take half of the available even counts for each character. If there is an odd character, we set aside one instance of it for the middle and take half of its remaining even count for the left side.

From here on out, we only need to think about the left side. Let us say the length of the string is N, and the length of the left half is M. The first M characters of the hurdle string form what we will call the target left prefix.

Because of how alphabetical comparison works, the left side carries the most weight. If we make the left half of our palindrome alphabetically larger than the target left prefix, the entire palindrome will be alphabetically larger than the entire hurdle string. 

### Step by Step Strategy

Here is a capable approach to finding our answer without generating millions of permutations. 

**Step 1. The Exact Prefix Match**
We first want to see if we can perfectly mimic the target left prefix using our available characters. We check our character pool to see if we have exactly what it takes to build the first M characters of the hurdle string. 

If we do have the right characters, we build this exact left half, append the middle character if the string length is odd, and then attach the reversed left half to the end. This gives us a complete palindrome. We then check if this full palindrome is strictly larger than the hurdle string. If it is, this is an excellent candidate! In fact, because we matched the prefix exactly, this will be the alphabetically smallest valid palindrome we can form. We can just return it.

**Step 2. Finding the Point of Divergence**
If matching the prefix exactly does not work out (either we lack the characters, or the resulting palindrome is not strictly larger than the hurdle string), we have to change our strategy. We need to diverge from the hurdle string. 

To keep our palindrome alphabetically as small as possible while still beating the hurdle, we want to match the hurdle string for as long as possible before breaking away. We calculate the longest possible matching prefix we can build with our character pool. Let us say we can match the first K characters perfectly.

**Step 3. Breaking Away**
We look at the furthest point we can reach. We will loop backwards from that point down to the very first character. At each position, we will look at our available character pool and ask a simple question. Do we have a character that is strictly larger than the character at this same position in the hurdle string?

If we have multiple larger characters, we want to pick the smallest one among them. This guarantees we barely step over the hurdle, keeping our string alphabetically small. 

Once we place this slightly larger character, the strict greater than condition is permanently satisfied! For all the remaining positions after this point in our left half, we want to make them as small as possible. We simply take all our leftover available characters and sort them in ascending alphabetical order. 

**Step 4. Constructing the Final String**
We now have our optimal left half. We take this left half, add our designated middle character if necessary, and then add the mirrored version of the left half. This creates our final answer. If we finish checking all positions and cannot find any way to diverge and grow larger, then no valid palindrome exists, and we return an empty string.

### The Code Implementation

Below is a clear C++ implementation of this logic. I have added comments to help map the code directly to the strategy we just discussed.

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    string lexPalindromicPermutation(string s, string target) {
        int n = s.length();
        vector<int> freq(26, 0);
        
        // Count the frequency of every letter in our source
        for (char c : s) {
            freq[c - 'a']++;
        }

        int odd_count = 0;
        char mid = 0;
        vector<int> chars(26, 0);
        
        // Build our character pool for the left side
        for (int c = 0; c < 26; ++c) {
            if (freq[c] % 2 == 1) {
                odd_count++;
                mid = 'a' + c;
            }
            chars[c] = freq[c] / 2;
        }

        // Stop early if a palindrome is impossible
        if (odd_count > 1) return "";

        int m = n / 2;
        string target_L = target.substr(0, m);

        // Attempt to match the target prefix exactly
        bool can_exact = true;
        vector<int> temp_chars = chars;
        for (int i = 0; i < m; ++i) {
            int c = target_L[i] - 'a';
            if (temp_chars[c] > 0) {
                temp_chars[c]--;
            } else {
                can_exact = false;
                break;
            }
        }

        if (can_exact) {
            string P = target_L;
            if (n % 2 == 1) P += mid;
            string rev = target_L;
            reverse(rev.begin(), rev.end());
            P += rev;
            
            // If the exact match wins, we return it
            if (P > target) return P;
        }

        // Find how far we can match the target prefix
        int k = 0;
        temp_chars = chars;
        for (int i = 0; i < m; ++i) {
            int c = target_L[i] - 'a';
            if (temp_chars[c] > 0) {
                temp_chars[c]--;
                k++;
            } else {
                break;
            }
        }

        // Work backwards to find the best place to break away
        int start_i = min(k, m - 1);
        for (int i = start_i; i >= 0; --i) {
            vector<int> rem_chars = chars;
            
            // Rebuild the character pool up to this specific index
            for (int j = 0; j < i; ++j) {
                rem_chars[target_L[j] - 'a']--;
            }

            int best_c = -1;
            
            // Look for the smallest character that is larger than the hurdle
            for (int c = target_L[i] - 'a' + 1; c < 26; ++c) {
                if (rem_chars[c] > 0) {
                    best_c = c;
                    break;
                }
            }

            // If we found a valid character, construct the left half
            if (best_c != -1) {
                string L = target_L.substr(0, i);
                L += (char)('a' + best_c);
                rem_chars[best_c]--;
                
                // Fill the rest with smallest available characters
                for (int c = 0; c < 26; ++c) {
                    while (rem_chars[c] > 0) {
                        L += (char)('a' + c);
                        rem_chars[c]--;
                    }
                }

                // Build the final palindrome
                string P = L;
                if (n % 2 == 1) P += mid;
                string rev = L;
                reverse(rev.begin(), rev.end());
                P += rev;
                return P;
            }
        }

        // Return empty string if all attempts fail
        return "";
    }
};

```

### Analyzing the Resources Used

Understanding the performance of our logic is just as important as writing it. Let us look at how much time and memory this process consumes.

**Time Complexity**
The time required to run this code scales linearly with the length of the string. We denote this mathematically as Order of N.
First, counting the character frequencies requires us to look at every single letter exactly one time, which takes time proportional to the string length.
Later, we might loop backwards through the prefix. The prefix is half the length of the whole string. In the worst case scenario, for every step backward, we reconstruct the remaining available characters and generate a temporary string segment. Because the alphabet size is strictly fixed at twenty six letters, the inner loops do not grow as the string gets longer. The total operations remain proportional to the string length. This makes the approach extremely fast and well suited for the problem constraints.

**Space Complexity**
The memory used by this code is also very modest. We denote it as Order of N as well.
We use small arrays of size twenty six to keep track of character counts. These arrays take up a microscopic and constant amount of memory.
The main memory consumption comes from building the new string variables to hold our left side, the reversed right side, and the final answer. Since these strings are always exactly the same length as the original source string, the memory footprint scales directly and linearly with the input size.

### Final Thoughts

This problem is a brilliant example of how we can avoid doing unnecessary work. By understanding the rules of palindromes, we cut the problem completely in half. By understanding alphabetical ordering, we learned that we only need to look for the first opportunity to diverge upward from our hurdle.

I hope this detailed breakdown sheds some light on greedy string manipulation! Exploring these patterns step by step is a great way to build strong problem solving intuition. Happy coding everyone, and keep up the great work!
