#include <string>
#include <algorithm>

class Solution {
public:
    int countBinarySubstrings(std::string s) {
        int prev_len = 0;
        int curr_len = 1;
        int total_count = 0;

        for (int i = 1; i < s.length(); ++i) {
            // If the character matches the previous one, extend the current group
            if (s[i] == s[i - 1]) {
                curr_len++;
            } 
            // If it changes, calculate substrings from the last two groups
            else {
                total_count += std::min(prev_len, curr_len);
                prev_len = curr_len; // The current group becomes the previous group
                curr_len = 1;        // Reset current group length for the new character
            }
        }
        
        // Don't forget to add the substrings formed by the final pair of groups
        total_count += std::min(prev_len, curr_len);

        return total_count;
    }
};