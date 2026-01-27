#include <string>
#include <vector>
#include <algorithm> // for min

using namespace std;

class Solution {
public:
    string convert(string s, int numRows) {
        // Base case: If there is only one row, no zigzag can occur.
        if (numRows == 1) return s;

        // Create a vector of strings to store characters for each row.
        // We take the min because if s.length() < numRows, we only need s.length() rows.
        vector<string> rows(min(numRows, int(s.length())));
        
        int curRow = 0;
        bool goingDown = false;

        for (char c : s) {
            rows[curRow] += c;
            
            // If we are at the top or bottom row, reverse direction.
            if (curRow == 0 || curRow == numRows - 1) {
                goingDown = !goingDown;
            }
            
            // Move to the next row index based on current direction
            curRow += goingDown ? 1 : -1;
        }

        // Combine all rows into a single result string
        string result;
        // Pre-allocate memory to avoid multiple reallocations
        result.reserve(s.length()); 
        
        for (const string& row : rows) {
            result += row;
        }

        return result;
    }
};