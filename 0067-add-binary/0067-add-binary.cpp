#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    string addBinary(string a, string b) {
        string result = "";
        int i = a.length() - 1;
        int j = b.length() - 1;
        int carry = 0;

        // Loop until both strings are processed and no carry remains
        while (i >= 0 || j >= 0 || carry > 0) {
            int sum = carry;

            // Add digit from string a if available
            if (i >= 0) {
                sum += a[i] - '0'; // Convert char to int
                i--;
            }

            // Add digit from string b if available
            if (j >= 0) {
                sum += b[j] - '0'; // Convert char to int
                j--;
            }

            // The digit to add to result is sum % 2 (0 or 1)
            result += (sum % 2) + '0'; 

            // Calculate new carry (1 if sum is 2 or 3)
            carry = sum / 2;
        }

        // The result was built backwards, so reverse it
        reverse(result.begin(), result.end());
        
        return result;
    }
};