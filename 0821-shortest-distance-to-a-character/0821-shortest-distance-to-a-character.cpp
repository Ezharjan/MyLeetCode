#include <vector>
#include <string>
#include <algorithm> // for std::min
#include <cmath>     // for std::abs

using namespace std;

class Solution {
public:
    vector<int> shortestToChar(string s, char c) {
        int n = s.length();
        vector<int> answer(n);
        
        // Pass 1: Find distance to the closest 'c' on the LEFT
        // Initialize prev to a value far to the left (effectively -infinity)
        // -n is sufficient because the max distance cannot exceed n
        int prev = -n; 
        for (int i = 0; i < n; i++) {
            if (s[i] == c) {
                prev = i;
            }
            // Distance is current index minus the index of the last seen 'c'
            answer[i] = i - prev;
        }

        // Pass 2: Find distance to the closest 'c' on the RIGHT and update
        // Initialize prev to a value far to the right (effectively +infinity)
        prev = 2 * n;
        for (int i = n - 1; i >= 0; i--) {
            if (s[i] == c) {
                prev = i;
            }
            // We take the minimum of the existing distance (from left) 
            // and the new distance (from right)
            answer[i] = min(answer[i], prev - i);
        }

        return answer;
    }
};