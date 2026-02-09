#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:
    string lexSmallestAfterDeletion(string s) {
        // Frequency map to count occurrences of each char in the remaining suffix of s
        vector<int> remaining(26, 0);
        for (char c : s) {
            remaining[c - 'a']++;
        }

        // Frequency map to count occurrences of each char currently in our stack/result
        vector<int> inStack(26, 0);
        string res;

        for (char c : s) {
            // We are processing 'c' now, so it is no longer in the "remaining" future
            remaining[c - 'a']--;

            // Monotonic Stack Logic:
            // While the last character in result is larger than current 'c',
            // we try to remove it to make the string lexicographically smaller.
            while (!res.empty() && res.back() > c) {
                char top = res.back();
                
                // We can delete 'top' ONLY if:
                // 1. There are more occurrences of 'top' later in the string (remaining > 0)
                // 2. OR we have redundant copies of 'top' already in the stack (inStack > 1)
                // If neither is true, deleting 'top' would violate the "at least one" rule.
                if (remaining[top - 'a'] > 0 || inStack[top - 'a'] > 1) {
                    res.pop_back();
                    inStack[top - 'a']--;
                } else {
                    // We must keep 'top' to satisfy the condition
                    break;
                }
            }

            // Add the current character to the result
            res.push_back(c);
            inStack[c - 'a']++;
        }

        // EDGE CASE FIX (Handle "aa" -> "a"):
        // If the resulting string ends with characters that have duplicates, 
        // removing them makes the string shorter and therefore lexicographically smaller.
        // Example: "aba" -> "ab" is better because "ab" < "aba".
        while (!res.empty() && inStack[res.back() - 'a'] > 1) {
            inStack[res.back() - 'a']--;
            res.pop_back();
        }

        return res;
    }
};