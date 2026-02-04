#include <climits> // For INT_MAX and INT_MIN
#include <string>

using namespace std;

class Solution {
public:
    int myAtoi(string s) {
        int i = 0;
        int n = s.length();
        
        // 1. Skip leading whitespace
        while (i < n && s[i] == ' ') {
            i++;
        }
        
        // Return 0 if string is empty or contains only whitespace
        if (i == n) return 0;
        
        // 2. Check for sign
        int sign = 1;
        if (s[i] == '-') {
            sign = -1;
            i++;
        } else if (s[i] == '+') {
            i++;
        }
        
        // 3. Convert digits and check for overflow
        int result = 0;
        while (i < n && isdigit(s[i])) {
            int digit = s[i] - '0';
            
            // Overflow check:
            // INT_MAX is 2147483647.
            // If result > INT_MAX / 10, appending any digit causes overflow.
            // If result == INT_MAX / 10, appending any digit > 7 causes overflow.
            if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7)) {
                return (sign == 1) ? INT_MAX : INT_MIN;
            }
            
            result = result * 10 + digit;
            i++;
        }
        
        return result * sign;
    }
};