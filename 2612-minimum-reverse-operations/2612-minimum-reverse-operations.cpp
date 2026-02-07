#include <vector>
#include <queue>
#include <set>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> minReverseOperations(int n, int p, vector<int>& banned, int k) {
        // Initialize the answer array with -1 (unreachable)
        vector<int> dist(n, -1);
        
        // Mark banned positions for quick lookup (though we actually just exclude them from sets)
        // We use boolean vector just to easily filter initially.
        vector<bool> is_banned(n, false);
        for (int x : banned) {
            is_banned[x] = true;
        }
        
        // Use sets to store unvisited indices to allow efficient range removal.
        // We separate them by parity because a move always changes index by a multiple of 2.
        // sets[0] stores even unvisited indices, sets[1] stores odd unvisited indices.
        set<int> unvisited[2];
        for (int i = 0; i < n; ++i) {
            if (i != p && !is_banned[i]) {
                unvisited[i % 2].insert(i);
            }
        }
        
        queue<int> q;
        q.push(p);
        dist[p] = 0;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            // Calculate the valid range of subarray starts (L) that include index u
            // Subarray range: [L, L + k - 1]
            // Constraints: 
            // 1. L >= 0
            // 2. L + k - 1 < n  => L <= n - k
            // 3. L <= u (subarray must start before or at u)
            // 4. L + k - 1 >= u => L >= u - k + 1
            
            int min_L = max(0, u - k + 1);
            int max_L = min(u, n - k);
            
            if (min_L > max_L) continue;
            
            // The formula for the new position v given start L is:
            // v = L + (L + k - 1) - u = 2*L + k - 1 - u
            // As L increments by 1, v increments by 2.
            
            int start_v = 2 * min_L + k - 1 - u;
            int end_v = 2 * max_L + k - 1 - u;
            
            // Determine which set (even or odd) the targets belong to.
            // start_v, start_v + 2, ... all have the same parity.
            int parity = start_v % 2;
            auto& s = unvisited[parity];
            
            // Find the first element in the set >= start_v
            auto it = s.lower_bound(start_v);
            
            // Iterate through valid unvisited neighbors in the range [start_v, end_v]
            while (it != s.end() && *it <= end_v) {
                int v = *it;
                
                // Process node
                dist[v] = dist[u] + 1;
                q.push(v);
                
                // Remove from set and move to next iterator efficiently
                it = s.erase(it);
            }
        }
        
        return dist;
    }
};