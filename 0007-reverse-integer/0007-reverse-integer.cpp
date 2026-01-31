#include <limits.h> // Required for INT_MAX and INT_MIN

class Solution {
public:
    int reverse(int x) {
        int rev = 0;
        
        while (x != 0) {
            // Extract the last digit
            int pop = x % 10;
            // Shorten x
            x /= 10;
            
            // Check for Positive Overflow
            // INT_MAX is 2147483647
            if (rev > INT_MAX / 10 || (rev == INT_MAX / 10 && pop > 7)) {
                return 0;
            }
            
            // Check for Negative Overflow
            // INT_MIN is -2147483648
            if (rev < INT_MIN / 10 || (rev == INT_MIN / 10 && pop < -8)) {
                return 0;
            }
            
            // Push the digit
            rev = rev * 10 + pop;
        }
        
        return rev;
    }
};