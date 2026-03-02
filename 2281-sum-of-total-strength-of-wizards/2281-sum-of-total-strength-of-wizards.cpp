#include <vector>

using namespace std;

class Solution {
public:
    int totalStrength(vector<int>& strength) {
        long long MOD = 1e9 + 7;
        int n = strength.size();
        
        // L[i] stores the left-most index where strength[i] is still the minimum.
        vector<int> L(n, 0);
        vector<int> st;
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && strength[st.back()] > strength[i]) {
                st.pop_back();
            }
            L[i] = st.empty() ? 0 : st.back() + 1;
            st.push_back(i);
        }
        
        // R[i] stores the right-most index where strength[i] is still the minimum.
        vector<int> R(n, n - 1);
        st.clear();
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && strength[st.back()] >= strength[i]) {
                st.pop_back();
            }
            R[i] = st.empty() ? n - 1 : st.back() - 1;
            st.push_back(i);
        }
        
        // P[i] is the prefix sum of strength
        vector<long long> P(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            P[i + 1] = (P[i] + strength[i]) % MOD;
        }
        
        // PP[i] is the prefix sum of the prefix sum array P
        vector<long long> PP(n + 2, 0);
        for (int i = 0; i <= n; ++i) {
            PP[i + 1] = (PP[i] + P[i]) % MOD;
        }
        
        long long ans = 0;
        
        for (int i = 0; i < n; ++i) {
            long long left_count = i - L[i] + 1;
            long long right_count = R[i] - i + 1;
            
            // Subarray sums involving right bounds
            long long pos_term = (PP[R[i] + 2] - PP[i + 1] + MOD) % MOD;
            pos_term = (pos_term * left_count) % MOD;
            
            // Subarray sums involving left bounds
            long long neg_term = (PP[i + 1] - PP[L[i]] + MOD) % MOD;
            neg_term = (neg_term * right_count) % MOD;
            
            // Total sum of all subarrays where strength[i] is the minimum
            long long total_sum = (pos_term - neg_term + MOD) % MOD;
            
            // Multiply by the minimum element strength[i]
            long long strength_contrib = (total_sum * strength[i]) % MOD;
            
            ans = (ans + strength_contrib) % MOD;
        }
        
        return ans;
    }
};