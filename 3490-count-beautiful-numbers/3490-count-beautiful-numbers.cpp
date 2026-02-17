#include <string>
#include <vector>
#include <numeric>

using namespace std;

// Global DP Array
// Dimensions: dp[target_Sum_S][length][current_sum][product_modulo_S]
int dp[82][10][82][81];
bool initialized = false;

class Solution {
public:
    void init() {
        if (initialized) return;
        initialized = true;
        
        // Base case initialization
        for (int S = 1; S <= 81; ++S) {
            dp[S][0][0][1 % S] = 1; // An empty string has a sum of 0 and product of 1
            for (int len = 0; len < 9; ++len) {
                for (int sum = 0; sum <= S && sum <= len * 9; ++sum) {
                    for (int p = 0; p < S; ++p) {
                        int ways = dp[S][len][sum][p];
                        if (ways) {
                            for (int d = 1; d <= 9; ++d) {
                                if (sum + d <= S) {
                                    dp[S][len + 1][sum + d][(p * d) % S] += ways;
                                } else {
                                    break; // Optimization: early exit since digits incrementally increase
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Counts the numbers <= X that DO NOT contain the digit '0'
    long long count_no_zero(const string& X) {
        long long res = 0;
        int L = X.length();
        long long p9 = 1;
        
        // Match lengths strictly smaller than the target
        for (int i = 1; i < L; ++i) {
            p9 *= 9;
            res += p9;
        }
        
        // Exact length match (prefix validation)
        for (int i = 0; i < L; ++i) {
            int d = X[i] - '0';
            for (int j = 1; j < d; ++j) {
                long long ways = 1;
                for (int k = i + 1; k < L; ++k) ways *= 9;
                res += ways;
            }
            if (d == 0) break; // Cannot use 0, abort this prefix depth sequence
            if (i == L - 1) res++; // Match with X itself
        }
        return res;
    }

    // Counts the beautiful numbers <= X that DO NOT contain the digit '0'
    long long countBeautifulNoZero(const string& X) {
        long long res = 0;
        int L = X.length();
        
        for (int S = 1; S <= 81; ++S) {
            // Include numbers with fewer digits than X limits
            for (int len = 1; len < L; ++len) {
                if (S <= len * 9) {
                    res += dp[S][len][S][0];
                }
            }
            
            int current_sum = 0;
            int current_prod = 1 % S;
            bool valid = true;
            
            for (int i = 0; i < L; ++i) {
                int limit = X[i] - '0';
                for (int d = 1; d < limit; ++d) {
                    if (current_sum + d <= S) {
                        int rem_len = L - 1 - i;
                        int req_sum = S - current_sum - d;
                        int V = (current_prod * d) % S;
                        
                        // Optimized step to determine valid remainders (only checking modular multiples)
                        int g = std::gcd(V, S);
                        int step = S / g;
                        
                        for (int P = 0; P < S; P += step) {
                            res += dp[S][rem_len][req_sum][P];
                        }
                    }
                }
                if (limit == 0) { // Number falls off valid DP mapping if a '0' enforces
                    valid = false;
                    break;
                }
                current_sum += limit;
                current_prod = (current_prod * limit) % S;
            }
            if (valid && current_sum == S && current_prod == 0) {
                res++;
            }
        }
        return res;
    }

    // Unified wrapper to sum zero & nonzero variants up to limit X
    long long countAll(long long X) {
        if (X == 0) return 0;
        string sX = to_string(X);
        long long total_with_zero = X - count_no_zero(sX);
        long long beautiful_no_zero = countBeautifulNoZero(sX);
        
        return total_with_zero + beautiful_no_zero;
    }

    int beautifulNumbers(int l, int r) {
        init();
        return countAll(r) - countAll(l - 1);
    }
};