class Solution {
public:
    bool isNumber(string s) {
        bool seenDigit = false;
        bool seenDot = false;
        bool seenExponent = false;

        for (int i = 0; i < s.length(); i++) {
            char c = s[i];
            
            if (c >= '0' && c <= '9') {
                seenDigit = true;
            } 
            else if (c == '+' || c == '-') {
                // Sign is only valid at the start or immediately after an exponent
                if (i > 0 && s[i - 1] != 'e' && s[i - 1] != 'E') {
                    return false;
                }
            } 
            else if (c == '.') {
                // Decimals are invalid if we've already seen a decimal or an exponent
                if (seenDot || seenExponent) {
                    return false;
                }
                seenDot = true;
            } 
            else if (c == 'e' || c == 'E') {
                // Exponents are invalid if we've already seen one, or if there are no preceding digits
                if (seenExponent || !seenDigit) {
                    return false;
                }
                seenExponent = true;
                // Reset seenDigit because an exponent MUST be followed by an integer
                seenDigit = false; 
            } 
            else {
                // Any other character (letters, spaces, special chars) makes it invalid
                return false;
            }
        }
        
        // If we end with a dangling 'e' or sign, seenDigit will be false
        return seenDigit;
    }
};