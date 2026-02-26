#include <string>
#include <vector>

using namespace std;

const int MOD = 1e9 + 7;
int C[805][805];
int ops[805];
bool initialized = false;

// Precompute combinations (Pascal's Triangle) and operations required for lengths up to 800
void init() {
    if (initialized) return;
    
    // Compute nCr % MOD
    for (int i = 0; i <= 800; ++i) {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j) {
            C[i][j] = (C[i-1][j-1] + C[i-1][j]) % MOD;
        }
    }
    
    // Compute operations to reduce i to 1
    ops[1] = 0;
    for (int i = 2; i <= 800; ++i) {
        int bits = 0;
        int temp = i;
        while (temp > 0) {
            bits += (temp & 1);
            temp >>= 1;
        }
        ops[i] = ops[bits] + 1;
    }
    initialized = true;
}

class Solution {
public:
    int countKReducibleNumbers(string s, int k) {
        init();
        long long total_count = 0;
        int L = s.length();
        
        // Check all possible set bit counts a number can have 
        // A number < n can have at most L set bits
        for (int c = 1; c <= L; ++c) {
            // Because our first step replaces x with its set bits (c),
            // c must take <= k - 1 operations to reach 1.
            if (ops[c] <= k - 1) {
                int set_bits = 0; // Tracks consecutive matched set bits with string 's'
                
                for (int i = 0; i < L; ++i) {
                    if (s[i] == '1') {
                        // If we place '0' here instead of '1', any remaining arrangement
                        // of bits will create a number strictly less than 'n'
                        int remaining_positions = L - 1 - i;
                        int needed_bits = c - set_bits;
                        
                        // Add valid combinations to the total modulo 10^9 + 7
                        if (needed_bits >= 0 && needed_bits <= remaining_positions) {
                            total_count = (total_count + C[remaining_positions][needed_bits]) % MOD;
                        }
                        
                        // Proceed matching the string's '1'
                        set_bits++;
                    }
                }
            }
        }
        
        return total_count;
    }
};