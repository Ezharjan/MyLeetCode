#include <vector>

using namespace std;

class Solution {
public:
    long long countNonDecreasingSubarrays(vector<int>& nums, int k) {
        int n = nums.size();
        
        // Prefix sums of the original array to query range sums in O(1)
        vector<long long> pref_nums(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref_nums[i + 1] = pref_nums[i] + nums[i];
        }

        vector<int> st;               // Monotonic stack storing indices
        vector<long long> pref_M;     // Prefix sums of the blocks governed by the stack
        long long ans = 0;
        int r = n - 1;

        // Lambda to compute the cost of converting nums[l...r_val] in O(log N)
        auto get_C = [&](int l, int r_val) {
            int low = 0, high = (int)st.size() - 1;
            int idx = 0;
            
            // Binary search to find the block in the stack containing index r_val
            while (low <= high) {
                int mid = low + (high - low) / 2;
                if (st[mid] <= r_val) {
                    idx = mid;
                    high = mid - 1;
                } else {
                    low = mid + 1;
                }
            }
            
            long long total_M = pref_M.back();
            long long end_of_block = (idx == 0) ? n : st[idx - 1];
            
            // Deduct bounds to strictly match the subarray [l, r_val]
            long long remainder = (end_of_block - 1 - r_val) * 1LL * nums[st[idx]];
            long long right_blocks = (idx == 0) ? 0 : pref_M[idx - 1];
            
            long long M_l_r = total_M - remainder - right_blocks;
            long long sum_l_r = pref_nums[r_val + 1] - pref_nums[l];
            
            return M_l_r - sum_l_r;
        };

        // Two-pointer logic: Traverse `l` backwards
        for (int l = n - 1; l >= 0; --l) {
            // Maintain a strictly decreasing monotonic stack
            while (!st.empty() && nums[st.back()] <= nums[l]) {
                st.pop_back();
                pref_M.pop_back();
            }
            
            long long block_len = (st.empty() ? n : st.back()) - l;
            long long block_sum = block_len * 1LL * nums[l];
            long long prev_pref = pref_M.empty() ? 0 : pref_M.back();
            
            st.push_back(l);
            pref_M.push_back(prev_pref + block_sum);

            // Shave off the right bound `r` if the operations needed exceed `k`
            while (get_C(l, r) > k) {
                r--;
            }
            
            ans += (r - l + 1);
        }

        return ans;
    }
};