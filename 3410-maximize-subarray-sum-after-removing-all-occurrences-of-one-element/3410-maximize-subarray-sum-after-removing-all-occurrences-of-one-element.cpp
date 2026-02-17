#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maxSubarraySum(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        // Step 1: Handle global maximum unmodified subarray and edge cases where all elements <= 0
        long long max_val = nums[0];
        long long current_max = nums[0];
        long long global_max = nums[0];
        for (int i = 1; i < n; ++i) {
            max_val = max(max_val, (long long)nums[i]);
            current_max = max((long long)nums[i], current_max + nums[i]);
            global_max = max(global_max, current_max);
        }
        
        // If the best we have is a negative number or zero, returning that single max element is perfectly optimal 
        // because removing strictly negative elements cannot flip the array to positive values.
        if (max_val <= 0) {
            return max_val;
        }
        
        long long ans = global_max;
        
        // Step 2: Extract mapping of strictly negative numbers to their sorted indexes
        vector<pair<int, int>> neg_pos;
        neg_pos.reserve(n);
        for (int i = 0; i < n; ++i) {
            if (nums[i] < 0) {
                neg_pos.push_back({nums[i], i});
            }
        }
        
        if (neg_pos.empty()) {
            return ans;
        }
        
        sort(neg_pos.begin(), neg_pos.end());
        
        // Step 3: Fast O(1) Prefix sums Subarray Analysis Initialization
        vector<long long> P(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            P[i + 1] = P[i] + nums[i];
        }
        
        int logN = 32 - __builtin_clz(n + 1);
        vector<vector<long long>> st_max(n + 1, vector<long long>(logN));
        vector<vector<long long>> st_min(n + 1, vector<long long>(logN));
        
        for (int i = 0; i <= n; ++i) {
            st_max[i][0] = P[i];
            st_min[i][0] = P[i];
        }
        
        // Build RMQ Sparse Tables 
        for (int j = 1; j < logN; ++j) {
            for (int i = 0; i + (1 << j) <= n + 1; ++i) {
                st_max[i][j] = max(st_max[i][j - 1], st_max[i + (1 << (j - 1))][j - 1]);
                st_min[i][j] = min(st_min[i][j - 1], st_min[i + (1 << (j - 1))][j - 1]);
            }
        }
        
        auto query_max = [&](int L, int R) { // inclusive bounding [L, R] over P
            int j = 31 - __builtin_clz(R - L + 1);
            return max(st_max[L][j], st_max[R - (1 << j) + 1][j]);
        };
        
        auto query_min = [&](int L, int R) { // inclusive bounding [L, R] over P
            int j = 31 - __builtin_clz(R - L + 1);
            return min(st_min[L][j], st_min[R - (1 << j) + 1][j]);
        };
        
        // Step 4: Validate Subarrays ignoring particular `x` elements
        int m_total = neg_pos.size();
        int i_ptr = 0;
        
        while (i_ptr < m_total) {
            int x = neg_pos[i_ptr].first;
            vector<int> indices;
            while (i_ptr < m_total && neg_pos[i_ptr].first == x) {
                indices.push_back(neg_pos[i_ptr].second);
                i_ptr++;
            }
            
            int m = indices.size();
            long long max_pref = 0; // Represents the best preceding suffix crossing exactly x boundaries
            
            for (int k = 0; k <= m; ++k) {
                int L_k = (k == 0) ? 0 : indices[k - 1] + 1;
                int R_k = (k == m) ? n - 1 : indices[k] - 1;
                
                long long T_k = 0, P_max = 0, S_max = 0;
                
                if (L_k <= R_k) { // Normal nonempty block chunk
                    T_k = P[R_k + 1] - P[L_k];
                    P_max = query_max(L_k, R_k + 1) - P[L_k];
                    S_max = P[R_k + 1] - query_min(L_k, R_k + 1);
                }
                
                if (k > 0) {
                    ans = max(ans, max_pref + P_max);
                    max_pref = max(max_pref + T_k, S_max);
                } else {
                    max_pref = S_max;
                }
            }
        }
        
        return ans;
    }
};