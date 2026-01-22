class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int n = digits.size();

        // Traverse the array from right to left
        for (int i = n - 1; i >= 0; --i) {
            // Case 1: No carry needed
            if (digits[i] < 9) {
                digits[i]++;
                return digits; // We are done
            }
            
            // Case 2: Carry needed (digit was 9)
            // Set current digit to 0 and continue to the next iteration
            digits[i] = 0;
        }

        // Case 3: Overflow (e.g., 999 -> 000)
        // If we're here, all digits were 9. We need a leading 1.
        digits.insert(digits.begin(), 1);
        
        return digits;
    }
};