#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    // Computes the number of pairs (u, v) such that 0 <= u < x, 0 <= v < y, and u + v <= k
    long long count_pairs(long long x, long long y, long long k) {
        if (k < 0) return 0;
        if (x > y) swap(x, y);
        
        // Case 1: The condition u + v <= k forms a clean triangle at the origin
        if (k < x) {
            return (k + 1) * (k + 2) / 2;
        }
        // Case 2: The triangle clips the x-bound but stays within the y-bound
        if (k < y) {
            return x * (k + 1) - x * (x - 1) / 2;
        }
        // Case 3: The valid pairs region clips both bounds; count inverse (violating pairs) and subtract
        if (k < x + y - 2) {
            long long k_prime = x + y - 3 - k;
            return x * y - (k_prime + 1) * (k_prime + 2) / 2;
        }
        // Case 4: k is large enough to encompass all possible pairs within x * y
        return x * y;
    }

public:
    long long minMaxSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        long long total_sum = 0;
        long long limit = k - 1;
        
        vector<int> L(n, -1), R(n, n);
        vector<int> st; // We can efficiently use a vector as our monotonic stack
        st.reserve(n);
        
        // --- Process Minimums ---
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && nums[st.back()] > nums[i]) {
                st.pop_back();
            }
            L[i] = st.empty() ? -1 : st.back();
            st.push_back(i);
        }
        
        st.clear();
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && nums[st.back()] >= nums[i]) {
                st.pop_back();
            }
            R[i] = st.empty() ? n : st.back();
            st.push_back(i);
        }
        
        for (int i = 0; i < n; ++i) {
            long long x = i - L[i];
            long long y = R[i] - i;
            long long ways = count_pairs(x, y, limit);
            total_sum += ways * nums[i];
        }
        
        // --- Process Maximums ---
        st.clear();
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && nums[st.back()] < nums[i]) {
                st.pop_back();
            }
            L[i] = st.empty() ? -1 : st.back();
            st.push_back(i);
        }
        
        st.clear();
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && nums[st.back()] <= nums[i]) {
                st.pop_back();
            }
            R[i] = st.empty() ? n : st.back();
            st.push_back(i);
        }
        
        for (int i = 0; i < n; ++i) {
            long long x = i - L[i];
            long long y = R[i] - i;
            long long ways = count_pairs(x, y, limit);
            total_sum += ways * nums[i];
        }
        
        return total_sum;
    }
};