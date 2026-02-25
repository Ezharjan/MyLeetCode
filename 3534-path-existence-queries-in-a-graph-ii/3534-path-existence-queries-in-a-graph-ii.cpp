#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> pathExistenceQueries(int n, vector<int>& nums, int maxDiff, vector<vector<int>>& queries) {
        // Step 1: Extract, sort and get unique values
        vector<int> V = nums;
        sort(V.begin(), V.end());
        V.erase(unique(V.begin(), V.end()), V.end());
        
        int m = V.size();
        int LOG = 18; // 2^17 = 131072, which is > 100000
        
        // jump[i][k] stores the furthest index in V reachable from V[i] in 2^k steps
        vector<vector<int>> jump(m, vector<int>(LOG));
        
        int r = 0;
        for (int i = 0; i < m; ++i) {
            // Greedily find the furthest index we can jump to in 1 step
            while (r + 1 < m && V[r + 1] <= V[i] + maxDiff) {
                r++;
            }
            jump[i][0] = r;
        }
        
        // Step 2: Build the binary lifting table
        for (int k = 1; k < LOG; ++k) {
            for (int i = 0; i < m; ++i) {
                jump[i][k] = jump[jump[i][k - 1]][k - 1];
            }
        }
        
        // Step 3: Process the queries
        vector<int> answer(queries.size());
        for (int i = 0; i < queries.size(); ++i) {
            int u = queries[i][0];
            int v = queries[i][1];
            
            if (u == v) {
                answer[i] = 0;
                continue;
            }
            
            int A = nums[u];
            int B = nums[v];
            
            if (A == B) {
                answer[i] = 1; // Different nodes, exact same value
                continue;
            }
            
            if (A > B) {
                swap(A, B);
            }
            
            // Map values to their indices in V
            int start = lower_bound(V.begin(), V.end(), A) - V.begin();
            int target = lower_bound(V.begin(), V.end(), B) - V.begin();
            
            // If the target index can't even be reached with the maximum possible steps
            if (jump[start][LOG - 1] < target) {
                answer[i] = -1;
                continue;
            }
            
            // Step 4: Calculate minimal steps using powers of 2 (Binary Lifting)
            int curr = start;
            int steps = 0;
            
            for (int k = LOG - 1; k >= 0; --k) {
                if (jump[curr][k] < target) {
                    curr = jump[curr][k];
                    steps += (1 << k);
                }
            }
            
            // After the loop, `curr` is at the largest reachable point strictly less than `target`.
            // One final jump will take us to or past the `target`.
            answer[i] = steps + 1;
        }
        
        return answer;
    }
};