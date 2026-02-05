#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
    // Prime indices: 0->2, 1->3, 2->5, 3->7
    int primes[4] = {2, 3, 5, 7};
    int digit_factors[10][4];

    // Initialize factor counts for digits 0-9
    void init_factors() {
        for (int d = 0; d <= 9; ++d) {
            for (int k = 0; k < 4; ++k) digit_factors[d][k] = 0;
            if (d <= 1) continue;
            int temp = d;
            for (int k = 0; k < 4; ++k) {
                while (temp % primes[k] == 0) {
                    digit_factors[d][k]++;
                    temp /= primes[k];
                }
            }
        }
    }

    // Calculate minimum digits needed to satisfy required factors (r2, r3, r5, r7)
    // Uses greedy packing: 9s, then 8s, 7s, 6s...
    int get_min_digits(long long r2, long long r3, long long r5, long long r7) {
        int cnt = 0;
        cnt += r5 + r7; // 5 and 7 cannot be combined into denser digits
        
        long long t2 = r2, t3 = r3;
        
        // Pack 3s into 9s (3^2)
        cnt += t3 / 2;
        t3 %= 2;
        
        // Pack 2s into 8s (2^3)
        cnt += t2 / 3;
        t2 %= 3;
        
        // Try to combine remaining 2 and 3 into 6 (2*3)
        if (t3 == 1 && t2 >= 1) {
            cnt++;
            t3--;
            t2--;
        }
        
        // Remaining 3s (at most 1)
        cnt += t3;
        
        // Remaining 2s (0, 1, or 2). Use 4 (2^2) or 2.
        cnt += (t2 + 1) / 2; 
        
        return cnt;
    }

    // Construct the lexicographically smallest suffix of length 'len' 
    // satisfying the required factors.
    string construct_suffix(int len, long long r2, long long r3, long long r5, long long r7) {
        string res = "";
        // We pack factors into largest possible digits to minimize the count of non-1 digits.
        while (r3 >= 2) { res += '9'; r3 -= 2; }
        while (r2 >= 3) { res += '8'; r2 -= 3; }
        while (r7 > 0)  { res += '7'; r7--; }
        while (r3 >= 1 && r2 >= 1) { res += '6'; r3--; r2--; }
        while (r5 > 0)  { res += '5'; r5--; }
        while (r2 >= 2) { res += '4'; r2 -= 2; }
        while (r3 > 0)  { res += '3'; r3--; }
        while (r2 > 0)  { res += '2'; r2--; }
        
        // Fill remaining space with '1's (lexicographically smallest)
        while (res.length() < len) {
            res += '1';
        }
        
        // Sort digits to put smaller digits (like 1s) at the front
        sort(res.begin(), res.end());
        return res;
    }

public:
    string smallestNumber(string num, long long t) {
        // 1. Validate t: must only be composed of 2, 3, 5, 7
        long long req[4] = {0}; 
        long long temp_t = t;
        for (int i = 0; i < 4; ++i) {
            while (temp_t % primes[i] == 0) {
                req[i]++;
                temp_t /= primes[i];
            }
        }
        if (temp_t > 1) return "-1";

        init_factors();
        int n = num.length();

        // 2. Precompute prefix factor counts for num
        vector<vector<int>> pref(n + 1, vector<int>(4, 0));
        int first_zero = -1;
        
        for (int i = 0; i < n; ++i) {
            if (num[i] == '0' && first_zero == -1) first_zero = i;
            for (int k = 0; k < 4; ++k) pref[i+1][k] = pref[i][k];
            if (num[i] != '0') {
                int d = num[i] - '0';
                for (int k = 0; k < 4; ++k) pref[i+1][k] += digit_factors[d][k];
            }
        }
        
        // 3. Check if num itself is valid (only if no zeros)
        if (first_zero == -1) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                if (pref[n][k] < req[k]) { ok = false; break; }
            }
            if (ok) return num;
        }

        // 4. Iterate backwards to find the split point
        // If there's a zero, we can't keep digits past the first zero.
        int start_i = (first_zero != -1) ? first_zero : n - 1;

        for (int i = start_i; i >= 0; --i) {
            int d_start = (num[i] - '0') + 1;
            
            // Calculate factors needed by the prefix (excluding current digit i)
            long long cur_req[4];
            for(int k=0; k<4; ++k) cur_req[k] = max(0LL, req[k] - pref[i][k]);

            for (int d = d_start; d <= 9; ++d) {
                long long rem_req[4];
                for(int k=0; k<4; ++k) rem_req[k] = max(0LL, cur_req[k] - digit_factors[d][k]);
                
                int rem_len = n - 1 - i;
                int min_k = get_min_digits(rem_req[0], rem_req[1], rem_req[2], rem_req[3]);
                
                // If we can satisfy the remainder within the remaining length
                if (min_k <= rem_len) {
                    string suffix = construct_suffix(rem_len, rem_req[0], rem_req[1], rem_req[2], rem_req[3]);
                    return num.substr(0, i) + to_string(d) + suffix;
                }
            }
        }

        // 5. Fallback: Solution must be longer than n
        // Check if the natural digits required by t are longer than n.
        // If so, just return those digits (sorted). Otherwise, return "1" + padding + digits.
        int min_k = get_min_digits(req[0], req[1], req[2], req[3]);
        if (min_k > n) {
             return construct_suffix(0, req[0], req[1], req[2], req[3]);
        } else {
             return "1" + construct_suffix(n, req[0], req[1], req[2], req[3]);
        }
    }
};