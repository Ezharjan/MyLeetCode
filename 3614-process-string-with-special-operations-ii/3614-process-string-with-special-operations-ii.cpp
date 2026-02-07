#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    char processStr(string s, long long k) {
        // Use long long to handle sizes up to 10^15
        vector<long long> lens;
        long long current_len = 0;
        int n = s.length();

        // --- Forward Pass: Calculate length at each step ---
        for (char c : s) {
            if (isalpha(c)) {
                current_len++;
            } else if (c == '*') {
                // Remove last char if exists
                if (current_len > 0) current_len--;
            } else if (c == '#') {
                // Duplicate the string
                current_len *= 2;
            } else if (c == '%') {
                // Reverse (length doesn't change)
            }
            lens.push_back(current_len);
        }

        // Check if k is valid for the final string
        if (lens.empty() || lens.back() <= k) {
            return '.';
        }

        // --- Backward Pass: Trace k to the source character ---
        long long cur_k = k;

        for (int i = n - 1; i >= 0; --i) {
            char op = s[i];
            // length after current op is lens[i]
            // length before current op is lens[i-1] (or 0 if i=0)
            long long len_before = (i == 0) ? 0 : lens[i-1];

            if (isalpha(op)) {
                // Logic: String became prev + op. 
                // The new char is at index 'len_before'.
                if (cur_k == len_before) {
                    return op;
                }
                // If cur_k < len_before, it's in the previous string. 
                // We keep looking.
            } 
            else if (op == '*') {
                // Logic: A character was popped. 
                // The current string is a prefix of the previous one.
                // cur_k maps 1:1 to the previous state.
            } 
            else if (op == '#') {
                // Logic: String duplicated (len_before -> len_before * 2).
                // If cur_k is in the second half, map it to the first half.
                if (cur_k >= len_before) {
                    cur_k -= len_before;
                }
            } 
            else if (op == '%') {
                // Logic: String reversed.
                // Index maps to (Length - 1 - Index).
                // The length here is len_before (which is same as lens[i])
                if (len_before > 0) {
                    cur_k = len_before - 1 - cur_k;
                }
            }
        }

        return '.';
    }
};