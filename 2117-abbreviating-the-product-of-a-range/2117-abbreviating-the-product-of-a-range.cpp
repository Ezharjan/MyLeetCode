#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    // Legendre's formula helper to quickly compute the max power of a prime p dividing n!
    int count(int n, int p) {
        int res = 0;
        while (n > 0) {
            res += n / p;
            n /= p;
        }
        return res;
    }
    
public:
    string abbreviateProduct(int left, int right) {
        // Calculate trailing zeros based on minimum count of factors of 2 and 5 in the range
        int C2 = count(right, 2) - count(left - 1, 2);
        int C5 = count(right, 5) - count(left - 1, 5);
        int C = min(C2, C5);
        
        int rem2 = C, rem5 = C;
        vector<int> V;
        V.reserve(right - left + 1);
        
        // Strip out exactly C factors of 2 and C factors of 5
        for (int i = left; i <= right; ++i) {
            int v = i;
            while (v % 2 == 0 && rem2 > 0) {
                v /= 2;
                rem2--;
            }
            while (v % 5 == 0 && rem5 > 0) {
                v /= 5;
                rem5--;
            }
            // Discard 1s as they don't contribute anything during multiplication
            if (v > 1) {
                V.push_back(v);
            }
        }
        
        // Group factors up to 10^9 to drastically speed up Big Integer multiplication
        vector<uint64_t> mults;
        uint64_t cur_mult = 1;
        for (int v : V) {
            if (cur_mult * v > 1000000000ULL) {
                mults.push_back(cur_mult);
                cur_mult = v;
            } else {
                cur_mult *= v;
            }
        }
        if (cur_mult > 1) {
            mults.push_back(cur_mult);
        }
        
        // Custom BigInt calculation keeping track in base 10^9 chunks
        const uint32_t BASE = 1000000000;
        vector<uint32_t> big;
        big.push_back(1);
        
        for (uint64_t m : mults) {
            uint64_t carry = 0;
            for (uint32_t& x : big) {
                uint64_t cur = (uint64_t)x * m + carry;
                x = cur % BASE;
                carry = cur / BASE;
            }
            while (carry > 0) {
                big.push_back(carry % BASE);
                carry /= BASE;
            }
        }
        
        // Reconstruct the full string mathematically
        string s;
        s.reserve(big.size() * 9);
        s += to_string(big.back());
        
        for (int i = (int)big.size() - 2; i >= 0; --i) {
            string word = to_string(big[i]);
            // Format base components correctly by padding zeros
            s.append(9 - word.length(), '0');
            s.append(word);
        }
        
        // Check final size and prepare result
        if (s.length() > 10) {
            return s.substr(0, 5) + "..." + s.substr(s.length() - 5) + "e" + to_string(C);
        } else {
            return s + "e" + to_string(C);
        }
    }
};