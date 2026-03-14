#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    bool equalFrequency(string word) {
        // Step 1: Count the frequencies of all characters
        vector<int> freq(26, 0);
        for (char c : word) {
            freq[c - 'a']++;
        }
        
        // Step 2: Simulate removing one character at a time
        for (int i = 0; i < 26; i++) {
            if (freq[i] > 0) {
                freq[i]--; // Remove one instance of the character
                
                // Step 3: Check if all remaining non-zero frequencies are equal
                int commonFreq = -1;
                bool possible = true;
                
                for (int j = 0; j < 26; j++) {
                    if (freq[j] > 0) {
                        if (commonFreq == -1) {
                            commonFreq = freq[j]; // Set the target frequency to match
                        } else if (freq[j] != commonFreq) {
                            possible = false; // Mismatch found
                            break;
                        }
                    }
                }
                
                if (possible) return true; // Found a valid removal
                
                freq[i]++; // Backtrack: put the character back before the next loop iteration
            }
        }
        
        return false; // No valid single removal made all frequencies equal
    }
};