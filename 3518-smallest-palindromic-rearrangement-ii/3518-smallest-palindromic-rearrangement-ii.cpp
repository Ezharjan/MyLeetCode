#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
private:
    // Helper to calculate nCr with saturation at 'limit'
    // Returns min(nCr, limit + 1)
    long long nCr(int n, int r, long long limit) {
        if (r < 0 || r > n) return 0;
        if (r == 0 || r == n) return 1;
        if (r > n / 2) r = n - r; // Symmetry property
        
        long long res = 1;
        for (int i = 1; i <= r; ++i) {
            // We want to calculate res = res * (n - i + 1) / i
            // Check for overflow or limit exceed before full calculation
            
            // Optimization: Since limit is small (10^6), and n <= 10000, 
            // res * n fits in long long. We just need to cap it.
            res = res * (n - i + 1);
            res /= i;
            
            if (res > limit) {
                return limit + 1;
            }
        }
        return res;
    }

    // Helper to calculate multinomial coefficient for remaining characters
    // Formula: (TotalLen)! / (count1! * count2! * ...)
    // Calculated iteratively as product of binomial coefficients
    long long countWays(const vector<int>& counts, int totalLen, long long limit) {
        long long res = 1;
        int currentLen = totalLen;
        
        for (int count : counts) {
            if (count > 0) {
                // Calculate combinations for this character's positions
                long long combinations = nCr(currentLen, count, limit);
                
                // Multiply into result with saturation check
                // Check if res * combinations > limit
                // Using division to prevent overflow: if limit / combinations < res
                if (combinations > 0 && limit / combinations < res) {
                    res = limit + 1;
                } else {
                    res = res * combinations;
                }
                
                if (res > limit) res = limit + 1;
                
                currentLen -= count;
            }
        }
        return res;
    }

public:
    string smallestPalindrome(string s, int k) {
        // 1. Count character frequencies
        vector<int> freq(26, 0);
        for (char c : s) {
            freq[c - 'a']++;
        }
        
        // 2. Identify middle character (if any)
        string mid = "";
        for (int i = 0; i < 26; ++i) {
            if (freq[i] % 2 == 1) {
                mid += (char)('a' + i);
                freq[i]--; // Remove middle char from counts for splitting
                break;
            }
        }
        
        // 3. Prepare counts for the first half
        for (int i = 0; i < 26; ++i) {
            freq[i] /= 2;
        }
        
        int halfLen = s.length() / 2;
        string half = "";
        long long k_ll = k; // Working with long long for safety
        
        // 4. Construct the first half lexicographically
        for (int i = 0; i < halfLen; ++i) {
            bool found = false;
            
            // Try smallest available character first
            for (int c = 0; c < 26; ++c) {
                if (freq[c] > 0) {
                    // Tentatively pick character 'c'
                    freq[c]--;
                    
                    // Count permutations of the remaining characters
                    long long ways = countWays(freq, halfLen - 1 - i, k_ll);
                    
                    if (k_ll <= ways) {
                        // The k-th permutation is within this branch
                        half += (char)('a' + c);
                        found = true;
                        break; // Move to next position
                    } else {
                        // The k-th permutation is NOT in this branch
                        k_ll -= ways; // Skip these permutations
                        freq[c]++;    // Backtrack: restore count
                    }
                }
            }
            
            // If we iterated 0-25 and didn't find a char, k was too large
            if (!found) return "";
        }
        
        // 5. Construct full palindrome
        string res = half + mid;
        reverse(half.begin(), half.end());
        res += half;
        
        return res;
    }
};