#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    string longestPalindrome(string s) {
        if (s.empty()) return "";
        
        int start = 0;
        int maxLen = 0;
        
        for (int i = 0; i < s.length(); ++i) {
            // Case 1: Odd length palindrome (center is character i)
            int len1 = expandAroundCenter(s, i, i);
            
            // Case 2: Even length palindrome (center is gap between i and i+1)
            int len2 = expandAroundCenter(s, i, i + 1);
            
            int len = max(len1, len2);
            
            // If we found a longer palindrome, update the start index and max length
            if (len > maxLen) {
                maxLen = len;
                // Calculate start index based on center 'i' and length 'len'
                // This formula works for both odd and even lengths due to integer division
                start = i - (len - 1) / 2;
            }
        }
        
        return s.substr(start, maxLen);
    }

private:
    // Helper function to expand outward from the center
    int expandAroundCenter(const string& s, int left, int right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            left--;
            right++;
        }
        // Return the length of the palindrome
        // (right - 1) - (left + 1) + 1 simplifies to right - left - 1
        return right - left - 1;
    }
};