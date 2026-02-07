#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    string largestPalindrome(int n, int k) {
        // Initialize result with all '9's as it's the largest starting point
        string res(n, '9');

        // Case k = 1, 3, 9:
        // Any string of '9's is a palindrome.
        // Sum of digits is 9*n, which is divisible by 3 and 9.
        if (k == 1 || k == 3 || k == 9) {
            return res;
        }

        // Case k = 2:
        // Must be even. Ends in 0,2,4,6,8. Largest is 8.
        if (k == 2) {
            res[0] = '8';
            res[n - 1] = '8';
            return res;
        }

        // Case k = 5:
        // Must end in 0 or 5. Leading zero not allowed. Max is 5.
        if (k == 5) {
            res[0] = '5';
            res[n - 1] = '5';
            return res;
        }

        // Case k = 4:
        // Divisible by 4 if last two digits divisible by 4.
        // Largest palindrome ends in ...88.
        if (k == 4) {
            res[0] = '8';
            res[n - 1] = '8';
            if (n > 2) {
                res[1] = '8';
                res[n - 2] = '8';
            }
            return res;
        }

        // Case k = 8:
        // Divisible by 8 if last three digits divisible by 8.
        // Largest palindrome ends in ...888.
        if (k == 8) {
            res[0] = '8';
            res[n - 1] = '8';
            if (n > 2) {
                res[1] = '8';
                res[n - 2] = '8';
            }
            if (n > 4) {
                res[2] = '8';
                res[n - 3] = '8';
            }
            return res;
        }

        // Case k = 6:
        // Divisible by 2 and 3.
        // Ends in 8 (max even). Sum of digits div by 3.
        if (k == 6) {
            if (n == 1) return "6";
            if (n == 2) return "66";
            
            res[0] = '8';
            res[n - 1] = '8';
            
            // Base sum analysis for "8...9...8":
            // Sum = 16 + (n-2)*9.
            // 16 % 3 = 1. (n-2)*9 is divisible by 3.
            // Current Sum % 3 = 1. We need to reduce sum by 1 mod 3.
            
            if (n % 2 == 1) {
                // Odd length: Change 1 middle digit.
                // Current is '9'. Changing to '8' reduces sum by 1.
                res[n / 2] = '8';
            } else {
                // Even length: Change 2 middle digits.
                // Current is '99'. Changing to '77' reduces sum by 4 (== 1 mod 3).
                res[n / 2 - 1] = '7';
                res[n / 2] = '7';
            }
            return res;
        }

        // Case k = 7:
        // Use Modulo Arithmetic to find the center digit(s).
        if (k == 7) {
            if (n == 1) return "7";
            if (n == 2) return "77";
            
            // Calculate remainder of the all-'9' string efficiently.
            int baseRem = 0;
            // Pattern of 10^i mod 7: 1, 3, 2, 6, 4, 5...
            vector<int> mods = {1, 3, 2, 6, 4, 5};
            
            for (char c : res) {
                baseRem = (baseRem * 10 + (c - '0')) % 7;
            }
            
            if (baseRem == 0) return res;
            
            // Modify middle digit(s) to fix the remainder
            if (n % 2 == 1) {
                int mid = n / 2;
                int p = (n - 1 - mid) % 6; // Power of 10 position
                int multiplier = mods[p];
                
                for (int d = 9; d >= 0; --d) {
                    // Check remainder change
                    int change = (d - 9); 
                    int diff = (change * multiplier) % 7;
                    int newRem = (baseRem + diff) % 7;
                    if (newRem < 0) newRem += 7;
                    
                    if (newRem == 0) {
                        res[mid] = d + '0';
                        return res;
                    }
                }
            } else {
                int m1 = n / 2 - 1;
                int m2 = n / 2;
                int p1 = (n - 1 - m1) % 6;
                int p2 = (n - 1 - m2) % 6;
                int multiplier = (mods[p1] + mods[p2]) % 7;
                
                for (int d = 9; d >= 0; --d) {
                    int change = (d - 9);
                    int diff = (change * multiplier) % 7;
                    int newRem = (baseRem + diff) % 7;
                    if (newRem < 0) newRem += 7;
                    
                    if (newRem == 0) {
                        res[m1] = d + '0';
                        res[m2] = d + '0';
                        return res;
                    }
                }
            }
        }

        return res;
    }
};