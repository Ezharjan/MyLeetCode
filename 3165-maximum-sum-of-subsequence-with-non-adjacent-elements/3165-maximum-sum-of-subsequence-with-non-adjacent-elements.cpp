#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
private:
    // Segment Tree Node
    struct Node {
        // v_start_end: 0 for excluded, 1 for allowed
        long long v00, v01, v10, v11;
    };

    vector<Node> tree;
    int n;

    // Merge function to combine left child and right child results
    Node merge(const Node& l, const Node& r) {
        Node res;
        // The boundary is between l (ending at mid) and r (starting at mid+1).
        // Constraint: Cannot pick both 'mid' and 'mid+1'.

        // To calculate v00 (Excl L, Excl R):
        // 1. Exclude mid (l.v00) -> r can Allow mid+1, but must Exclude R (r.v10)
        // 2. Allow mid (l.v01) -> r must Exclude mid+1, and must Exclude R (r.v00)
        res.v00 = max(l.v00 + r.v10, l.v01 + r.v00);

        // To calculate v01 (Excl L, Allow R):
        // 1. Exclude mid (l.v00) -> r can Allow mid+1, Allow R (r.v11)
        // 2. Allow mid (l.v01) -> r must Exclude mid+1, Allow R (r.v01)
        res.v01 = max(l.v00 + r.v11, l.v01 + r.v01);

        // To calculate v10 (Allow L, Excl R):
        // 1. Exclude mid (l.v10) -> r can Allow mid+1, Exclude R (r.v10)
        // 2. Allow mid (l.v11) -> r must Exclude mid+1, Exclude R (r.v00)
        res.v10 = max(l.v10 + r.v10, l.v11 + r.v00);

        // To calculate v11 (Allow L, Allow R):
        // 1. Exclude mid (l.v10) -> r can Allow mid+1, Allow R (r.v11)
        // 2. Allow mid (l.v11) -> r must Exclude mid+1, Allow R (r.v01)
        res.v11 = max(l.v10 + r.v11, l.v11 + r.v01);

        return res;
    }

    void build(const vector<int>& nums, int node, int start, int end) {
        if (start == end) {
            long long val = max(0, nums[start]);
            // If we allow picking (1), sum is val. If we forbid (0), sum is 0.
            // Note: Since L==R, if we forbid L or R, we forbid the element.
            tree[node] = {0, 0, 0, val};
            return;
        }
        int mid = start + (end - start) / 2;
        build(nums, 2 * node, start, mid);
        build(nums, 2 * node + 1, mid + 1, end);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            long long v = max(0, val);
            tree[node] = {0, 0, 0, v};
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

public:
    int maximumSumSubsequence(vector<int>& nums, vector<vector<int>>& queries) {
        n = nums.size();
        tree.resize(4 * n);
        build(nums, 1, 0, n - 1);

        long long totalSum = 0;
        int MOD = 1e9 + 7;

        for (const auto& q : queries) {
            int pos = q[0];
            int x = q[1];
            update(1, 0, n - 1, pos, x);
            // The answer for the current state is tree[1].v11
            // (Max sum of 0...n-1 with both ends allowed to be picked)
            totalSum = (totalSum + tree[1].v11) % MOD;
        }

        return totalSum;
    }
};