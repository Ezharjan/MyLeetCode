#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    vector<string> readBinaryWatch(int turnedOn) {
        vector<string> result;
        
        // Iterate through all possible hours (0 to 11)
        for (int h = 0; h < 12; ++h) {
            // Iterate through all possible minutes (0 to 59)
            for (int m = 0; m < 60; ++m) {
                // Check if the total active LEDs match the target 'turnedOn'
                if (__builtin_popcount(h) + __builtin_popcount(m) == turnedOn) {
                    // Format the string and add to the result list
                    string time = to_string(h) + (m < 10 ? ":0" : ":") + to_string(m);
                    result.push_back(time);
                }
            }
        }
        
        return result;
    }
};