#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    int countSubstrings(string s) {
        int count = 0;
        int n = s.length();

        for (int i = 0; i < n; i++) {
            // Case 1: Odd length palindromes (center is character i)
            count += countPalindromesAroundCenter(s, i, i);

            // Case 2: Even length palindromes (center is between i and i+1)
            count += countPalindromesAroundCenter(s, i, i + 1);
        }

        return count;
    }

private:
    int countPalindromesAroundCenter(const string& s, int left, int right) {
        int found = 0;
        // Expand outwards as long as characters match and we are within bounds
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            found++; // Found a valid palindrome
            left--;  // Move left pointer outward
            right++; // Move right pointer outward
        }
        return found;
    }
};