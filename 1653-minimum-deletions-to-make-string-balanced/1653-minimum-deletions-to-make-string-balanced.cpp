#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    int minimumDeletions(string s) {
        int countB = 0;
        int deletions = 0;

        for (char c : s) {
            if (c == 'b') {
                // Count 'b's as they are potential causes for imbalance
                countB++;
            } else {
                // We found an 'a'.
                // If there are preceding 'b's, we have a "ba" collision.
                if (countB > 0) {
                    // We increment deletions (removing either this 'a' or the previous 'b')
                    deletions++;
                    // We decrease countB because one 'b' has effectively been 
                    // "cancelled out" or removed to satisfy this specific imbalance.
                    countB--;
                }
            }
        }

        return deletions;
    }
};