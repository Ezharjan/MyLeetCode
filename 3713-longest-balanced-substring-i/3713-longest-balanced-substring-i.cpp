#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int longestBalanced(string s) {
        int n = s.length();
        int maxLen = 0;

        // Iterate through all possible starting points
        for (int i = 0; i < n; ++i) {
            vector<int> freq(26, 0);
            int maxFreq = 0;
            int distinctCount = 0;

            // Iterate through all possible ending points
            for (int j = i; j < n; ++j) {
                int charIndex = s[j] - 'a';

                // If this is a new character in this window, increment distinct count
                if (freq[charIndex] == 0) {
                    distinctCount++;
                }

                // Update frequency
                freq[charIndex]++;
                
                // Track the maximum frequency seen in the current window
                maxFreq = max(maxFreq, freq[charIndex]);

                // Current length of the substring
                int currentLen = j - i + 1;

                // Validation Check:
                // If every distinct character appears 'maxFreq' times, 
                // then the total length must be exactly (maxFreq * distinctCount).
                if (maxFreq * distinctCount == currentLen) {
                    maxLen = max(maxLen, currentLen);
                }
            }
        }

        return maxLen;
    }
};