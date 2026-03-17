#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    long long MOD = 1e9 + 7;

    // Helper function to calculate (base^exp) % MOD
    long long power(long long base, long long exp) {
        long long res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }

    // Helper function to calculate nCr (combinations)
    long long nCr(int n, int r) {
        if (r > n) return 0;
        if (r == 0 || r == n) return 1;
        if (r > n / 2) r = n - r; // Optimize calculation 
        
        long long res = 1;
        for (int i = 1; i <= r; i++) {
            res = res * (n - i + 1);
            res = res / i;
        }
        return res % MOD;
    }

public:
    int countKSubsequencesWithMaxBeauty(string s, int k) {
        int counts[26] = {0};
        
        // 1. Calculate frequencies for each character
        for (char c : s) {
            counts[c - 'a']++;
        }
        
        vector<int> V;
        for (int i = 0; i < 26; i++) {
            if (counts[i] > 0) {
                V.push_back(counts[i]);
            }
        }
        
        // If there are fewer unique characters than k, it's impossible
        if (V.size() < k) return 0;
        
        // 2. Sort frequencies in descending order to pick the largest
        sort(V.rbegin(), V.rend());
        
        // F is the frequency of the k-th element (the threshold)
        int F = V[k - 1]; 
        int c_greater = 0;
        int c_equal = 0;
        
        long long ways = 1;
        
        // 3. Separate logic for elements strictly > F and elements == F
        for (int v : V) {
            if (v > F) {
                c_greater++;
                ways = (ways * v) % MOD; // Multiply choices for mandatory inclusions
            } else if (v == F) {
                c_equal++;
            }
        }
        
        // Number of elements we still need to pick from the group having frequency == F
        int c_needed = k - c_greater;
        
        // 4. Multiply combinations to choose the tied elements 
        long long combs = nCr(c_equal, c_needed);
        ways = (ways * combs) % MOD;
        
        // Multiply by the actual index selections available for those tied elements
        ways = (ways * power(F, c_needed)) % MOD;
        
        return ways;
    }
};