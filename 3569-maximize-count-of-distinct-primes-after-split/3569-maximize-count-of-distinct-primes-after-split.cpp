#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

const int MAX_VAL = 100005;
bool is_prime[MAX_VAL];
bool sieve_initialized = false;

void init_sieve() {
    if (sieve_initialized) return;
    fill(is_prime, is_prime + MAX_VAL, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p < MAX_VAL; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i < MAX_VAL; i += p)
                is_prime[i] = false;
        }
    }
    sieve_initialized = true;
}

class SegmentTree {
public:
    int n;
    vector<int> tree;
    vector<int> lazy;

    SegmentTree(int n) : n(n) {
        // Size 4*n to handle tree structure safely
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }

    void push(int node) {
        if (lazy[node] != 0) {
            tree[2 * node] += lazy[node];
            lazy[2 * node] += lazy[node];
            tree[2 * node + 1] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
            lazy[node] = 0;
        }
    }

    // Range update: add val to [l, r]
    // current node covers [start, end]
    void update(int node, int start, int end, int l, int r, int val) {
        if (l > end || r < start) return;
        if (l <= start && end <= r) {
            tree[node] += val;
            lazy[node] += val;
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    int query() {
        return tree[1];
    }
};

class Solution {
    vector<set<int>> pos;
public:
    vector<int> maximumCount(vector<int>& nums, vector<vector<int>>& queries) {
        init_sieve();
        int n = nums.size();
        
        // pos[p] stores the sorted indices of prime p
        pos.assign(MAX_VAL, set<int>());
        
        // Segment Tree covers the split indices [1, n-1]
        SegmentTree st(n); 

        // Helper to update prime contribution to the score
        // p: prime number
        // idx: index in nums being modified
        // adding: true if adding idx, false if removing idx
        auto update_contribution = [&](int p, int idx, bool adding) {
            // 1. Remove OLD contribution of p based on current state
            if (!pos[p].empty()) {
                // Base contribution: p exists in array
                st.update(1, 1, n-1, 1, n-1, -1);
                
                int first = *pos[p].begin();
                int last = *pos[p].rbegin();
                
                // Overlap contribution: p exists in both prefix and suffix
                // This occurs for split k where first < k <= last
                if (first < last) {
                    st.update(1, 1, n-1, first + 1, last, -1);
                }
            }
            
            // 2. Modify the set of indices
            if (adding) pos[p].insert(idx);
            else pos[p].erase(idx);
            
            // 3. Add NEW contribution of p based on new state
            if (!pos[p].empty()) {
                st.update(1, 1, n-1, 1, n-1, 1);
                
                int first = *pos[p].begin();
                int last = *pos[p].rbegin();
                
                if (first < last) {
                    st.update(1, 1, n-1, first + 1, last, 1);
                }
            }
        };

        // Initialize with initial nums
        for (int i = 0; i < n; ++i) {
            if (is_prime[nums[i]]) {
                update_contribution(nums[i], i, true);
            }
        }

        vector<int> res;
        res.reserve(queries.size());

        for (auto& q : queries) {
            int idx = q[0];
            int val = q[1];
            int old_val = nums[idx];
            
            if (old_val != val) {
                // Remove old value if it was prime
                if (is_prime[old_val]) {
                    update_contribution(old_val, idx, false);
                }
                // Add new value if it is prime
                if (is_prime[val]) {
                    update_contribution(val, idx, true);
                }
                nums[idx] = val;
            }
            res.push_back(st.query());
        }
        return res;
    }
};