#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
public:
    int numberOfWays(string s, string t, long long k) {
        const int n = s.length();
        const int negOnePowK = (k % 2 == 0 ? 1 : -1); 
        
        // Z-algorithm on s + t + t
        const vector<int> z = zFunction(s + t + t);
        const vector<int> indices = getIndices(z, n);
        
        vector<long long> dp(2); // dp[0] := ways to reach same state; dp[1] := ways to reach diff state
        
        // dp[k][1] = ((n - 1)^k - (-1)^k) / n
        dp[1] = (modPow(n - 1, k) - negOnePowK + kMod) % kMod;
        dp[1] = (dp[1] * modPow(n, kMod - 2)) % kMod;
        
        // dp[k][0] = dp[k][1] + (-1)^k
        dp[0] = (dp[1] + negOnePowK + kMod) % kMod;
        
        long long totalWays = 0;
        for (int index : indices) {
            // If index == 0, it means s == t initially, so we need dp[0] ways.
            // Otherwise, we need dp[1] ways to reach that specific valid rotation.
            totalWays = (totalWays + (index == 0 ? dp[0] : dp[1])) % kMod;
        }
        
        return totalWays;
    }

private:
    static constexpr int kMod = 1'000'000'007;

    // Fast modular exponentiation
    long long modPow(long long x, long long n) {
        long long result = 1;
        x %= kMod;
        while (n > 0) {
            if (n % 2 == 1) result = (result * x) % kMod;
            x = (x * x) % kMod;
            n /= 2;
        }
        return result;
    }

    // Z-Algorithm
    vector<int> zFunction(const string& str) {
        const int n = str.length();
        vector<int> z(n, 0);
        int l = 0, r = 0;
        for (int i = 1; i < n; ++i) {
            if (i < r) {
                z[i] = min(r - i, z[i - l]);
            }
            while (i + z[i] < n && str[z[i]] == str[i + z[i]]) {
                ++z[i];
            }
            if (i + z[i] > r) {
                l = i;
                r = i + z[i];
            }
        }
        return z;
    }

    // Extract valid starting indices for rotations
    vector<int> getIndices(const vector<int>& z, int n) {
        vector<int> indices;
        // Check within the t + t portion of s + t + t
        for (int i = n; i < 2 * n; ++i) {
            if (z[i] >= n) {
                indices.push_back(i - n);
            }
        }
        return indices;
    }
};