#include <vector>

using namespace std;

class Fancy {
private:
    unsigned long long mul;
    unsigned long long add;
    vector<unsigned long long> sequence;
    const int MOD = 1e9 + 7;

    // Helper: (base^exp) % MOD
    unsigned long long power(unsigned long long base, unsigned long long exp) {
        unsigned long long res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }

    // Helper: Modular Inverse
    unsigned long long modInverse(unsigned long long n) {
        return power(n, MOD - 2);
    }

public:
    Fancy() {
        mul = 1;
        add = 0;
    }
    
    void append(int val) {
        // FIX: Cast 'add' to long long to ensure signed subtraction.
        // Otherwise, if val < add, (val - add) wraps to a huge unsigned number.
        long long diff = (static_cast<long long>(val) - static_cast<long long>(add)) % MOD;
        
        if (diff < 0) {
            diff += MOD;
        }
        
        // (val - add) * mul^(-1)
        unsigned long long normalized_val = (diff * modInverse(mul)) % MOD;
        sequence.push_back(normalized_val);
    }
    
    void addAll(int inc) {
        add = (add + inc) % MOD;
    }
    
    void multAll(int m) {
        mul = (mul * m) % MOD;
        add = (add * m) % MOD;
    }
    
    int getIndex(int idx) {
        if (idx >= sequence.size()) {
            return -1;
        }
        // (stored_val * mul + add) % MOD
        return (sequence[idx] * mul + add) % MOD;
    }
};