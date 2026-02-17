#include <vector>

using namespace std;

class Solution {
    struct Node {
        long long sum1;
        long long sum2;
        long long lazy;
    };
    
    vector<Node> tree;
    long long MOD = 1e9 + 7;

    // Helper to apply the lazy value to a specific node
    void apply(int node, int l, int r, long long v) {
        long long len = r - l + 1;
        long long v2 = (v * v) % MOD;
        long long v_sum1 = (v * tree[node].sum1) % MOD;
        
        // (c + v)^2 = c^2 + 2*c*v + v^2
        tree[node].sum2 = (tree[node].sum2 + 2 * v_sum1 + len * v2) % MOD;
        tree[node].sum1 = (tree[node].sum1 + len * v) % MOD;
        tree[node].lazy = (tree[node].lazy + v) % MOD;
    }

    // Push down the lazy values to children nodes
    void pushdown(int node, int l, int r) {
        if (tree[node].lazy != 0) {
            int mid = l + (r - l) / 2;
            apply(2 * node, l, mid, tree[node].lazy);
            apply(2 * node + 1, mid + 1, r, tree[node].lazy);
            tree[node].lazy = 0;
        }
    }

    // Update current node using its children
    void pushup(int node) {
        tree[node].sum1 = (tree[2 * node].sum1 + tree[2 * node + 1].sum1) % MOD;
        tree[node].sum2 = (tree[2 * node].sum2 + tree[2 * node + 1].sum2) % MOD;
    }

    // Range update function
    void update(int node, int l, int r, int ql, int qr, long long v) {
        if (ql <= l && r <= qr) {
            apply(node, l, r, v);
            return;
        }
        pushdown(node, l, r);
        int mid = l + (r - l) / 2;
        if (ql <= mid) update(2 * node, l, mid, ql, qr, v);
        if (qr > mid) update(2 * node + 1, mid + 1, r, ql, qr, v);
        pushup(node);
    }

public:
    int sumCounts(vector<int>& nums) {
        int n = nums.size();
        tree.assign(4 * n + 5, {0, 0, 0});
        
        // As per constraints, nums[i] <= 10^5, so an array is faster than a hash map
        vector<int> last(100005, -1);
        long long ans = 0;

        for (int j = 0; j < n; ++j) {
            // The range of starting indices i that get a new distinct element
            int L = last[nums[j]] + 1;
            int R = j;
            
            // Add 1 to distinct counts for subarrays starting in [L, R]
            update(1, 0, n - 1, L, R, 1);
            
            // The root's sum2 contains the total sum of squares for all valid subarrays ending at j
            ans = (ans + tree[1].sum2) % MOD;
            
            // Update the last seen index for the current number
            last[nums[j]] = j;
        }

        return ans;
    }
};