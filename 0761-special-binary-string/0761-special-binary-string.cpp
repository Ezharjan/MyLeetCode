#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    string makeLargestSpecial(string s) {
        int count = 0;
        int i = 0;
        vector<string> substrings;
        
        // Step 1: Split the string into primitive special substrings
        for (int j = 0; j < s.size(); ++j) {
            // Increment for '1', decrement for '0'
            if (s[j] == '1') {
                count++;
            } else {
                count--;
            }
            
            // When count hits 0, we've found a balanced primitive special substring
            if (count == 0) {
                // Step 2: Recursively process the inside of the primitive string
                // i + 1 skips the outer '1', j - i - 1 takes the inner content
                string inner = s.substr(i + 1, j - i - 1);
                
                // Process the inner part, then wrap it back with the outer '1' and '0'
                string processed = "1" + makeLargestSpecial(inner) + "0";
                substrings.push_back(processed);
                
                // Move the start pointer for the next primitive string
                i = j + 1;
            }
        }
        
        // Step 3: Sort the processed substrings in descending order
        sort(substrings.begin(), substrings.end(), greater<string>());
        
        // Step 4: Join them back together
        string result = "";
        for (const string& str : substrings) {
            result += str;
        }
        
        return result;
    }
};