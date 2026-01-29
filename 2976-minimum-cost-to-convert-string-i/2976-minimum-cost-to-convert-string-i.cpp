#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    long long minimumCost(string source, string target, vector<char>& original, vector<char>& changed, vector<int>& cost) {
        // 1. Initialize the distance matrix with Infinity
        // There are 26 lowercase English letters.
        // We use a large enough number for INF, but safe enough to prevent overflow when adding.
        constexpr long long INF = 1e15; 
        long long d[26][26];

        // Fill matrix: INF for distinct pairs, 0 for self-loops
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                d[i][j] = (i == j) ? 0 : INF;
            }
        }

        // 2. Build the initial graph from given conversion costs
        // If multiple conversions exist for the same pair, keep the minimum cost.
        int m = original.size();
        for (int i = 0; i < m; ++i) {
            int u = original[i] - 'a';
            int v = changed[i] - 'a';
            d[u][v] = min(d[u][v], (long long)cost[i]);
        }

        // 3. Apply Floyd-Warshall Algorithm to find all-pairs shortest paths
        // This updates d[i][j] to be the minimum cost using intermediate nodes 'k'
        for (int k = 0; k < 26; ++k) {
            for (int i = 0; i < 26; ++i) {
                if (d[i][k] == INF) continue; // Optimization: Skip if path i->k doesn't exist
                for (int j = 0; j < 26; ++j) {
                    if (d[k][j] == INF) continue; // Optimization: Skip if path k->j doesn't exist
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }

        // 4. Calculate total cost for converting source to target
        long long totalCost = 0;
        int n = source.length();

        for (int i = 0; i < n; ++i) {
            if (source[i] == target[i]) continue; // No conversion needed

            int u = source[i] - 'a';
            int v = target[i] - 'a';

            // If the cost is still INF, it means transformation is impossible
            if (d[u][v] == INF) return -1;

            totalCost += d[u][v];
        }

        return totalCost;
    }
};