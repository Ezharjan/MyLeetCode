class Solution {
public:
    bool isPalindrome(int x) {
        // TRICK 1: Early Exit for Edge Cases
        // 1. Negative numbers (-121) are never palindromes.
        // 2. Numbers ending in 0 (10, 20) are never palindromes, unless the number is just 0.
        if (x < 0 || (x % 10 == 0 && x != 0)) {
            return false;
        }

        int reversedHalf = 0;

        // TRICK 2: Reverse Only Half the Number
        // We only process digits until the reversed part becomes larger than or equal to the remaining part.
        // This avoids integer overflow issues and cuts the loop execution time in half.
        while (x > reversedHalf) {
            reversedHalf = reversedHalf * 10 + x % 10;
            x /= 10;
        }

        // TRICK 3: Handling Even vs Odd Lengths
        // If length is even (e.g., 1221), x (12) should equal reversedHalf (12).
        // If length is odd (e.g., 121), x (1) should equal reversedHalf / 10 (12 / 10 = 1).
        return x == reversedHalf || x == reversedHalf / 10;
    }
};