#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

class Solution {
public:
    int numberOfRoutes(vector<string>& grid, int d) {
        int n = grid.size();
        int m = grid[0].size();
        long long MOD = 1e9 + 7;

        // v[j]: number of valid paths ending at the current row, column j, 
        // where the last move was effectively "vertical" (or it's a start point).
        // These paths ALLOW a subsequent horizontal move.
        vector<long long> v(m, 0);

        // h[j]: number of valid paths ending at the current row, column j,
        // where the last move was "horizontal".
        // These paths DO NOT ALLOW a subsequent horizontal move.
        vector<long long> h(m, 0);

        // --- Base Case: Bottom Row (n-1) ---
        for (int j = 0; j < m; ++j) {
            if (grid[n - 1][j] == '.') {
                v[j] = 1; // Starting points are treated as vertical arrivals
            }
        }

        // Helper to compute prefix sums for O(1) range queries
        auto compute_prefix = [&](const vector<long long>& arr) {
            vector<long long> P(m + 1, 0);
            for (int k = 0; k < m; ++k) {
                P[k + 1] = (P[k] + arr[k]) % MOD;
            }
            return P;
        };

        // Helper to get sum from Prefix array P in range [L, R]
        auto get_sum = [&](const vector<long long>& P, int L, int R) {
            if (L < 0) L = 0;
            if (R >= m) R = m - 1;
            if (L > R) return 0LL;
            return (P[R + 1] - P[L] + MOD) % MOD;
        };

        // Compute h for the starting row (we can move horizontally immediately after starting)
        vector<long long> P_v = compute_prefix(v);
        for (int j = 0; j < m; ++j) {
            if (grid[n - 1][j] == '#') continue;
            // Horizontal move distance <= d. We subtract self (v[j]) because we must move to a different cell.
            long long sum_val = get_sum(P_v, j - d, j + d);
            long long self = v[j];
            h[j] = (sum_val - self + MOD) % MOD;
        }

        // --- Iterate from second to last row up to top ---
        for (int i = n - 2; i >= 0; --i) {
            vector<long long> next_v(m, 0);
            vector<long long> next_h(m, 0);

            // 1. Calculate Vertical Arrivals (moving from row i+1 to i)
            // Source: v[i+1] + h[i+1] (Total paths at previous row)
            vector<long long> total_prev(m);
            for(int k = 0; k < m; ++k) total_prev[k] = (v[k] + h[k]) % MOD;
            
            vector<long long> P_total = compute_prefix(total_prev);

            // Calculate max horizontal deviation for a vertical move
            // dist = sqrt(1^2 + dx^2) <= d  =>  1 + dx^2 <= d^2  =>  dx <= sqrt(d^2 - 1)
            long long d2_minus_1 = (long long)d * d - 1;
            int v_range = 0;
            if (d2_minus_1 >= 0) {
                v_range = floor(sqrt(d2_minus_1));
            } else {
                v_range = -1; // Should not happen given d >= 1
            }

            if (v_range >= 0) {
                for (int j = 0; j < m; ++j) {
                    if (grid[i][j] == '#') continue;
                    next_v[j] = get_sum(P_total, j - v_range, j + v_range);
                }
            }

            // 2. Calculate Horizontal Arrivals (moving within row i)
            // Source: next_v (paths that just arrived vertically at row i)
            // We use next_v because we cannot chain horizontal moves (cannot come from h).
            vector<long long> P_next_v = compute_prefix(next_v);
            
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == '#') continue;
                // Range is simply d for horizontal moves
                long long sum_val = get_sum(P_next_v, j - d, j + d);
                long long self = next_v[j];
                next_h[j] = (sum_val - self + MOD) % MOD;
            }

            // Update DP arrays for the next iteration
            v = next_v;
            h = next_h;
        }

        // --- Final Answer ---
        // Sum of all valid paths ending at row 0 (both vertical and horizontal arrivals)
        long long ans = 0;
        for (int j = 0; j < m; ++j) {
            ans = (ans + v[j] + h[j]) % MOD;
        }

        return ans;
    }
};