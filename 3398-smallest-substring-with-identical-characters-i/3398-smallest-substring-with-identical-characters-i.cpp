#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int minLength(string s, int numOps) {
        int n = s.length();
        int low = 1, high = n;
        int ans = n;

        // Lambda to check if a max substring length of 'limit' is achievable
        auto canAchieve = [&](int limit) {
            // Case 1: Target length is 1 (Strictly alternating string)
            if (limit == 1) {
                int costPattern1 = 0; // Target: "010101..."
                int costPattern2 = 0; // Target: "101010..."
                
                for (int i = 0; i < n; ++i) {
                    if (i % 2 == 0) {
                        // Even index
                        if (s[i] != '0') costPattern1++;
                        if (s[i] != '1') costPattern2++;
                    } else {
                        // Odd index
                        if (s[i] != '1') costPattern1++;
                        if (s[i] != '0') costPattern2++;
                    }
                }
                return min(costPattern1, costPattern2) <= numOps;
            }

            // Case 2: Target length >= 2
            // We can process each run of identical characters independently.
            // For a block of size k, we need k / (limit + 1) flips to break it.
            int ops = 0;
            int currentRun = 0;
            char prevChar = ' '; // Placeholder

            for (int i = 0; i < n; ++i) {
                if (i == 0 || s[i] == prevChar) {
                    currentRun++;
                } else {
                    // End of a previous run, calculate needed flips
                    ops += currentRun / (limit + 1);
                    currentRun = 1;
                }
                prevChar = s[i];
            }
            // Don't forget the last run
            ops += currentRun / (limit + 1);

            return ops <= numOps;
        };

        // Binary Search for the minimum possible length
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (canAchieve(mid)) {
                ans = mid;
                high = mid - 1; // Try to find a smaller valid length
            } else {
                low = mid + 1;  // Current length is too small, need larger
            }
        }

        return ans;
    }
};