#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

class Solution {
    struct Block {
        int start_idx;
        int len;
        long long val;
        long long pref_sum;
    };

    // Helper binary search to fetch sum contributions out of block limits
    long long query_st(int count, const vector<Block>& st) {
        if (count <= 0) return 0;
        int low = 0, high = st.size() - 1;
        int idx = 0;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (st[mid].start_idx <= count - 1) {
                idx = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        long long prev_sum = (idx == 0) ? 0 : st[idx - 1].pref_sum;
        return prev_sum + (long long)(count - st[idx].start_idx) * st[idx].val;
    }

public:
    long long maxTotalValue(vector<int>& nums, int k) {
        int n = nums.size();
        if (n == 0 || k == 0) return 0;

        int max_val = nums[0], min_val = nums[0];
        for (int x : nums) {
            if (x > max_val) max_val = x;
            if (x < min_val) min_val = x;
        }

        // Vectors functioning dynamically as deques to ensure 0 overhead allocations
        vector<int> max_dq(n), min_dq(n);
        
        auto count_ge = [&](long long M) {
            long long cnt = 0;
            int L = 0;
            int max_head = 0, max_tail = 0;
            int min_head = 0, min_tail = 0;
            
            for (int r = 0; r < n; ++r) {
                while (max_tail > max_head && nums[max_dq[max_tail - 1]] <= nums[r]) max_tail--;
                max_dq[max_tail++] = r;
                
                while (min_tail > min_head && nums[min_dq[min_tail - 1]] >= nums[r]) min_tail--;
                min_dq[min_tail++] = r;
                
                while (L <= r && nums[max_dq[max_head]] - nums[min_dq[min_head]] >= M) {
                    if (max_dq[max_head] == L) max_head++;
                    if (min_dq[min_head] == L) min_head++;
                    L++;
                }
                cnt += L;
            }
            return cnt;
        };

        // 1. Binary search the exact k-th threshold M
        long long low = 0, high = max_val - min_val, M = 0;
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (count_ge(mid) >= k) {
                M = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        long long total_sum = 0;
        long long count_greater = 0;
        int L = 0;
        int max_head = 0, max_tail = 0;
        int min_head = 0, min_tail = 0;
        vector<Block> max_st; max_st.reserve(n);
        vector<Block> min_st; min_st.reserve(n);

        // 2. Tally Up exactly all Subarrays mapped out > M
        for (int r = 0; r < n; ++r) {
            // Standard maintainable sliding window ranges via pointer arrays
            while (max_tail > max_head && nums[max_dq[max_tail - 1]] <= nums[r]) max_tail--;
            max_dq[max_tail++] = r;
            
            while (min_tail > min_head && nums[min_dq[min_tail - 1]] >= nums[r]) min_tail--;
            min_dq[min_tail++] = r;
            
            // Maintain Maximum Monotonic Intervals Structure
            int start = r;
            while (!max_st.empty() && max_st.back().val <= nums[r]) {
                start = max_st.back().start_idx;
                max_st.pop_back();
            }
            long long prev_max_sum = max_st.empty() ? 0 : max_st.back().pref_sum;
            long long length_max = r - start + 1;
            max_st.push_back({start, (int)length_max, nums[r], prev_max_sum + length_max * nums[r]});
            
            // Maintain Minimum Monotonic Intervals Structure
            start = r;
            while (!min_st.empty() && min_st.back().val >= nums[r]) {
                start = min_st.back().start_idx;
                min_st.pop_back();
            }
            long long prev_min_sum = min_st.empty() ? 0 : min_st.back().pref_sum;
            long long length_min = r - start + 1;
            min_st.push_back({start, (int)length_min, nums[r], prev_min_sum + length_min * nums[r]});
            
            // Update left constraint
            while (L <= r && nums[max_dq[max_head]] - nums[min_dq[min_head]] >= M + 1) {
                if (max_dq[max_head] == L) max_head++;
                if (min_dq[min_head] == L) min_head++;
                L++;
            }
            
            count_greater += L;
            total_sum += query_st(L, max_st) - query_st(L, min_st);
        }

        // Output Result padded identically to the threshold limit targeting exactly 'k'
        return total_sum + (long long)(k - count_greater) * M;
    }
};