#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // 'dict' stores the last known index of a character. 
        // We use 256 to cover all standard and extended ASCII characters.
        // Initialize with -1 to indicate characters haven't been seen yet.
        vector<int> dict(256, -1);
        
        int maxLength = 0;
        int left = 0;
        
        for (int right = 0; right < s.length(); right++) {
            char currentChar = s[right];
            
            // If the character was seen before, and the previous occurrence 
            // is within or after our current window start (left), we must move left.
            if (dict[currentChar] != -1 && dict[currentChar] >= left) {
                left = dict[currentChar] + 1;
            }
            
            // Update the last seen index of the current character
            dict[currentChar] = right;
            
            // Update the maximum length found so far
            maxLength = max(maxLength, right - left + 1);
        }
        
        return maxLength;
    }
};