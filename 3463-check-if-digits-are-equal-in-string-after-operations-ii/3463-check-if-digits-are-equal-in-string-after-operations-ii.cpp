#include <string>
#include <vector>

using namespace std;

class Solution {
private:
    // Precomputed Binomial Coefficients for Modulo 5 (Pascal's triangle mod 5)
    // C[n][k] = nCk % 5
    const int C[5][5] = {
        {1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 2, 1, 0, 0},
        {1, 3, 3, 1, 0},
        {1, 4, 1, 4, 1}
    };

    // Lucas Theorem to compute nCk % 5
    int lucasMod5(int n, int k) {
        if (k < 0 || k > n) return 0;
        int res = 1;
        while (n > 0 || k > 0) {
            int ni = n % 5;
            int ki = k % 5;
            if (ki > ni) return 0; // If a digit in k is larger than in n, result is 0
            res = (res * C[ni][ki]) % 5;
            n /= 5;
            k /= 5;
        }
        return res;
    }

public:
    bool hasSameDigits(string s) {
        int n = s.length();
        int N = n - 2; // The degree of the binomial expansion
        
        int diffSum = 0;

        for (int i = 0; i <= N; ++i) {
            // 1. Calculate nCk % 2 using Bitwise property
            // (N & i) == i implies that for every bit set in i, it is also set in N
            int mod2 = ((N & i) == i) ? 1 : 0;

            // 2. Calculate nCk % 5 using Lucas Theorem
            int mod5 = lucasMod5(N, i);

            // 3. Combine using Chinese Remainder Theorem for Modulo 10
            // We need x such that: x % 2 == mod2 AND x % 5 == mod5
            // The mapping (5 * mod2 + 6 * mod5) % 10 works because:
            // - If mod2=1, mod5=0: (5*1 + 0) = 5. 5%2=1, 5%5=0.
            // - If mod2=0, mod5=1: (0 + 6*1) = 6. 6%2=0, 6%5=1.
            int coeff = (5 * mod2 + 6 * mod5) % 10;

            // 4. Add weighted difference to sum
            // We compare Left Digit vs Right Digit by checking if (Left - Right) % 10 == 0
            // Left contributes s[i], Right contributes s[i+1]
            int diff = s[i] - s[i+1];
            
            // Perform arithmetic carefully to handle C++ negative modulo behavior if necessary
            // Here we just accumulate and check % 10 at the very end
            diffSum = (diffSum + diff * coeff) % 10;
        }

        return diffSum == 0;
    }
};