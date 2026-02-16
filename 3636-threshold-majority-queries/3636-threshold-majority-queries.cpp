#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

const uint64_t OFFSET = 2000000000ULL;
const uint64_t ONE_F = 1ULL << 32;

struct Query {
    int l, r, threshold, id;
};

class Solution {
    uint64_t tree[32768];
    int base;

    inline void update(int idx, int delta) {
        int i = base + idx;
        if (delta == 1) {
            tree[i] += ONE_F;
        } else {
            tree[i] -= ONE_F;
        }
        // Bottom-up max update to tree root
        for (i >>= 1; i >= 1; i >>= 1) {
            tree[i] = max(tree[i << 1], tree[(i << 1) | 1]);
        }
    }

public:
    vector<int> subarrayMajority(vector<int>& nums, vector<vector<int>>& queries) {
        int n = nums.size();
        int q_len = queries.size();
        
        // Coordinate Compression to map large values within tree index constraints safely
        vector<int> sorted_nums = nums;
        sort(sorted_nums.begin(), sorted_nums.end());
        sorted_nums.erase(unique(sorted_nums.begin(), sorted_nums.end()), sorted_nums.end());
        int U = sorted_nums.size();

        base = 2;
        while (base < U) base *= 2;

        // Reset and initialize Segment Tree
        for (int i = 0; i < 2 * base; ++i) {
            tree[i] = 0;
        }
        for (int i = 0; i < U; ++i) {
            tree[base + i] = OFFSET - sorted_nums[i];
        }
        for (int i = base - 1; i >= 1; --i) {
            tree[i] = max(tree[i << 1], tree[(i << 1) | 1]);
        }

        vector<int> compressed_nums(n);
        for (int i = 0; i < n; ++i) {
            compressed_nums[i] = lower_bound(sorted_nums.begin(), sorted_nums.end(), nums[i]) - sorted_nums.begin();
        }

        // Mo's Algorithm Setup
        int B = max(1, (int)(n / sqrt(q_len)));

        vector<Query> Q(q_len);
        for (int i = 0; i < q_len; ++i) {
            Q[i] = {queries[i][0], queries[i][1], queries[i][2], i};
        }

        // Standard alternating Mo's Query sort for optimal localized movement
        sort(Q.begin(), Q.end(), [B](const Query& q1, const Query& q2) {
            int b1 = q1.l / B;
            int b2 = q2.l / B;
            if (b1 != b2) return b1 < b2;
            return (b1 & 1) ? q1.r < q2.r : q1.r > q2.r;
        });

        vector<int> ans(q_len);
        int L = 0, R = -1;

        // Process queries iteratively
        for (const auto& q : Q) {
            // Expand Window First
            while (L > q.l) update(compressed_nums[--L], 1);
            while (R < q.r) update(compressed_nums[++R], 1);
            // Contract Window
            while (L < q.l) update(compressed_nums[L++], -1);
            while (R > q.r) update(compressed_nums[R--], -1);

            uint64_t root = tree[1];
            int max_f = root >> 32;
            int min_v = OFFSET - (root & 0xFFFFFFFFULL);

            if (max_f >= q.threshold) {
                ans[q.id] = min_v;
            } else {
                ans[q.id] = -1;
            }
        }

        return ans;
    }
};