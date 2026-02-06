#include <string>
#include <unordered_map>

using namespace std;

class Solution {
public:
    int romanToInt(string s) {
        // Map to store the integer value of each Roman numeral
        unordered_map<char, int> roman = {
            {'I', 1}, {'V', 5}, {'X', 10},
            {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}
        };

        int total = 0;
        int n = s.length();

        for (int i = 0; i < n; i++) {
            // If the current value is less than the next value, subtract it.
            // (e.g., IV: I is 1, V is 5. 1 < 5, so we subtract 1)
            // We also check (i < n - 1) to ensure we don't go out of bounds.
            if (i < n - 1 && roman[s[i]] < roman[s[i + 1]]) {
                total -= roman[s[i]];
            } else {
                // Otherwise, just add the value.
                // (e.g., VI: V is 5, I is 1. 5 >= 1, so we add 5)
                total += roman[s[i]];
            }
        }

        return total;
    }
};