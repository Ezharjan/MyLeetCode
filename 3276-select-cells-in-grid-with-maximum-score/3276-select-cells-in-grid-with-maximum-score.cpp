#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

class Solution {
private:
    // List of row indices where a specific value (index 1-100) appears.
    vector<int> rowsByValue[101];
    // A list of all unique values present in the grid, sorted descending.
    vector<int> allValues;
    // Memoization table: [value_index][row_mask]
    int memo[101][1024]; 

    int solve(int valIdx, int mask) {
        // Base Case: If we have considered all unique values, the remaining sum is 0.
        if (valIdx == allValues.size()) {
            return 0;
        }

        // Check if this state is already computed
        if (memo[valIdx][mask] != -1) {
            return memo[valIdx][mask];
        }

        int currentVal = allValues[valIdx];

        // Option 1: Skip the current value entirely
        int maxScore = solve(valIdx + 1, mask);

        // Option 2: Try to pick this value from one of the rows it appears in
        for (int row : rowsByValue[currentVal]) {
            // Check if the row is currently free (bit is 0)
            if (!(mask & (1 << row))) {
                // Pick this value: add to score, mark row as used, move to next value
                maxScore = max(maxScore, currentVal + solve(valIdx + 1, mask | (1 << row)));
            }
        }

        return memo[valIdx][mask] = maxScore;
    }

public:
    int maxScore(vector<vector<int>>& grid) {
        // 1. Reset data structures for the new test case
        for (int i = 0; i < 101; ++i) {
            rowsByValue[i].clear();
        }
        allValues.clear();
        memset(memo, -1, sizeof(memo));

        int m = grid.size();
        int n = grid[0].size();

        // 2. Preprocess: Map values to the rows they appear in
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                rowsByValue[grid[r][c]].push_back(r);
            }
        }

        // 3. Clean up the mapping and build the unique values list
        for (int v = 100; v >= 1; --v) {
            if (!rowsByValue[v].empty()) {
                // Remove duplicate row indices for the same value
                sort(rowsByValue[v].begin(), rowsByValue[v].end());
                rowsByValue[v].erase(unique(rowsByValue[v].begin(), rowsByValue[v].end()), rowsByValue[v].end());
                
                allValues.push_back(v);
            }
        }

        // 4. Start the recursion
        return solve(0, 0);
    }
};