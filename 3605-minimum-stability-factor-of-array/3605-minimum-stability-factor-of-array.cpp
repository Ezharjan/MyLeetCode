#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    int minStable(vector<int>& nums, int maxC) {
        int n = nums.size();
        // Sparse Table for Range GCD queries
        // st[j][i] stores GCD of subarray starting at i with length 2^j
        int logN = 32 - __builtin_clz(n); // Calculate max log needed
        vector<vector<int>> st(logN, vector<int>(n));

        // Initialize Sparse Table (Level 0)
        for (int i = 0; i < n; ++i) {
            st[0][i] = nums[i];
        }

        // Build Sparse Table
        for (int j = 1; j < logN; ++j) {
            for (int i = 0; i + (1 << j) <= n; ++i) {
                st[j][i] = std::gcd(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            }
        }

        // Helper lambda to query GCD of nums[L...R]
        auto query = [&](int L, int R) {
            int len = R - L + 1;
            int k = 31 - __builtin_clz(len);
            return std::gcd(st[k][L], st[k][R - (1 << k) + 1]);
        };

        // Check if it's possible to have max stability factor <= k
        // This means NO subarray of length k + 1 can be stable.
        auto check = [&](int k) {
            int len = k + 1;
            if (len > n) return true; // Cannot have subarray larger than n

            int modifications = 0;
            int last_modified_idx = -1;

            // Iterate through all windows of size 'len'
            for (int i = 0; i <= n - len; ++i) {
                // If the current window overlaps with the last modification,
                // we assume we broke the stability logic efficiently (by picking a coprime number)
                // so this window is no longer stable.
                if (i <= last_modified_idx) continue;

                // Check GCD of the current window [i, i + len - 1]
                if (query(i, i + len - 1) >= 2) {
                    // Window is stable (bad). We must modify an element.
                    // Greedy: Modify the last element of the window to extend 
                    // the "break" effect as far right as possible.
                    modifications++;
                    last_modified_idx = i + len - 1;
                }
            }
            return modifications <= maxC;
        };

        // Binary Search for the minimum stability factor
        int low = 0, high = n;
        int ans = n;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (check(mid)) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        return ans;
    }
};