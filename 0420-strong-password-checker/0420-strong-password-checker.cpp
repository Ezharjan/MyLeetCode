#include <string>
#include <algorithm>
#include <vector>
#include <cctype>

using namespace std;

class Solution {
public:
    int strongPasswordChecker(string password) {
        int n = password.length();
        bool hasLower = false, hasUpper = false, hasDigit = false;

        // 1. Analyze character types
        for (char c : password) {
            if (islower(c)) hasLower = true;
            else if (isupper(c)) hasUpper = true;
            else if (isdigit(c)) hasDigit = true;
        }

        int missing_types = (!hasLower) + (!hasUpper) + (!hasDigit);

        // Case 1: Too short (Length < 6)
        if (n < 6) {
            // Insertions can fix length and missing types simultaneously.
            // Even if we have "aaaaa", inserting 'B' gives "aaBaaa" (still weak), 
            // but actually, max(6-n, missing) covers repetition logic for n<6 
            // because max repetition is 5, and inserting to reach 6 breaks it enough.
            return max(6 - n, missing_types);
        }

        // 2. Count repeating sequences and replacements needed
        int replace_count = 0;
        int one_seqs = 0; // Sequences where len % 3 == 0 (e.g., 3, 6 chars)
        int two_seqs = 0; // Sequences where len % 3 == 1 (e.g., 4, 7 chars)

        for (int i = 0; i < n; ) {
            int len = 1;
            while (i + len < n && password[i + len] == password[i]) {
                len++;
            }
            
            if (len >= 3) {
                replace_count += len / 3;
                if (len % 3 == 0) one_seqs++;
                else if (len % 3 == 1) two_seqs++;
            }
            i += len;
        }

        // Case 2: Length fits (6 <= n <= 20)
        if (n <= 20) {
            return max(replace_count, missing_types);
        }

        // Case 3: Too long (Length > 20)
        int delete_count = n - 20;
        
        // We must delete characters. We use deletions to reduce 'replace_count' optimally.
        
        // Priority 1: Reduce sequences with len % 3 == 0. 
        // 1 deletion saves 1 replacement.
        replace_count -= min(delete_count, one_seqs);
        
        // Priority 2: Reduce sequences with len % 3 == 1. 
        // 2 deletions save 1 replacement.
        // We calculate remaining deletes after Priority 1 usage.
        replace_count -= min(max(delete_count - one_seqs, 0) / 2, two_seqs);
        
        // Priority 3: General reduction. 
        // 3 deletions save 1 replacement.
        replace_count -= max(delete_count - one_seqs - 2 * two_seqs, 0) / 3;
        
        return delete_count + max(replace_count, missing_types);
    }
};