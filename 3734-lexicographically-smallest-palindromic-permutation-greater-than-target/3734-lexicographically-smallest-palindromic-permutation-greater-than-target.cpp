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
        for (char c : s) {
            freq[c - 'a']++;
        }

        int odd_count = 0;
        char mid = 0;
        vector<int> chars(26, 0);
        
        // Populate left-half available characters and detect the middle character
        for (int c = 0; c < 26; ++c) {
            if (freq[c] % 2 == 1) {
                odd_count++;
                mid = 'a' + c;
            }
            chars[c] = freq[c] / 2;
        }

        // A palindrome cannot be formed if more than 1 character has an odd frequency
        if (odd_count > 1) return "";

        int m = n / 2;
        string target_L = target.substr(0, m);

        // 1. Try to match the left half to target_L exactly
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
            
            // If the exact match yields a strictly greater permutation, we are done
            if (P > target) return P;
        }

        // 2. Find the length of the longest valid prefix we can match with target_L
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

        // 3. Find the first point to diverge with a strictly greater character
        int start_i = min(k, m - 1);
        for (int i = start_i; i >= 0; --i) {
            vector<int> rem_chars = chars;
            for (int j = 0; j < i; ++j) {
                rem_chars[target_L[j] - 'a']--;
            }

            int best_c = -1;
            // Look for the smallest available character strictly greater than target_L[i]
            for (int c = target_L[i] - 'a' + 1; c < 26; ++c) {
                if (rem_chars[c] > 0) {
                    best_c = c;
                    break;
                }
            }

            // If found, greedily form the rest of the left-half string
            if (best_c != -1) {
                string L = target_L.substr(0, i);
                L += (char)('a' + best_c);
                rem_chars[best_c]--;
                
                // Append remaining characters in ascending lexicographical order
                for (int c = 0; c < 26; ++c) {
                    while (rem_chars[c] > 0) {
                        L += (char)('a' + c);
                        rem_chars[c]--;
                    }
                }

                // Construct and return the full palindrome
                string P = L;
                if (n % 2 == 1) P += mid;
                string rev = L;
                reverse(rev.begin(), rev.end());
                P += rev;
                return P;
            }
        }

        // No valid permutation found
        return "";
    }
};