#include <vector>
#include <string>
#include <algorithm> // For std::sort

class Solution {
public:
    std::string longestCommonPrefix(std::vector<std::string>& strs) {
        // Guard clause for empty input
        if (strs.empty()) return "";

        // Sort the vector of strings lexicographically (alphabetical order)
        // This groups similar prefixes together.
        std::sort(strs.begin(), strs.end());

        // After sorting, the "longest common prefix" must be a prefix of 
        // the FIRST string and the LAST string.
        // Any string in the middle will inherently share this prefix.
        std::string first = strs[0];
        std::string last = strs.back();
        int idx = 0;

        // Compare only the first and last strings
        while (idx < first.length() && idx < last.length()) {
            if (first[idx] == last[idx]) {
                idx++;
            } else {
                break;
            }
        }

        return first.substr(0, idx);
    }
};