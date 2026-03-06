#include <string>

class Solution {
public:
    bool checkOnesSegment(std::string s) {
        // If "01" exists in the string, it means a new segment of 1s 
        // has started after a 0. We want to ensure this doesn't happen.
        return s.find("01") == std::string::npos;
    }
};