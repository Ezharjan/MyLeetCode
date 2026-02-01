#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
    int n;
    // Segment Tree arrays
    vector<int> tree_min, tree_max, lazy;
    
    // Constants to represent inactive nodes. 
    // INF ensures min > 0, NEG_INF ensures max < 0, so they are ignored by queryFirstZero.
    const int INF = 1e9;
    const int NEG_INF = -1e9;

    // Propagate lazy values down to children
    void push(int node) {
        if (lazy[node] != 0) {
            tree_min[2 * node] += lazy[node];
            tree_max[2 * node] += lazy[node];
            lazy[2 * node] += lazy[node];
            
            tree_min[2 * node + 1] += lazy[node];
            tree_max[2 * node + 1] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
            
            lazy[node] = 0;
        }
    }

    // Range update: add val to [l, r]
    void update(int node, int start, int end, int l, int r, int val) {
        if (l > end || r < start) return;
        if (l <= start && end <= r) {
            tree_min[node] += val;
            tree_max[node] += val;
            lazy[node] += val;
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        tree_min[node] = min(tree_min[2 * node], tree_min[2 * node + 1]);
        tree_max[node] = max(tree_max[2 * node], tree_max[2 * node + 1]);
    }

    // Point update: Activate index idx by setting it to 0
    void set_zero(int node, int start, int end, int idx) {
        if (start == end) {
            tree_min[node] = 0;
            tree_max[node] = 0;
            lazy[node] = 0;
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        if (idx <= mid) set_zero(2 * node, start, mid, idx);
        else set_zero(2 * node + 1, mid + 1, end, idx);
        tree_min[node] = min(tree_min[2 * node], tree_min[2 * node + 1]);
        tree_max[node] = max(tree_max[2 * node], tree_max[2 * node + 1]);
    }

    // Query: Find the smallest index with value 0
    int queryFirstZero(int node, int start, int end) {
        // Optimization: If 0 is not in the range [min, max], prune this branch
        if (tree_min[node] > 0 || tree_max[node] < 0) return -1;
        
        if (start == end) return start;
        
        push(node);
        int mid = (start + end) / 2;
        
        // Try left child first to find the smallest index
        int res = queryFirstZero(2 * node, start, mid);
        if (res != -1) return res;
        
        // Otherwise try right child
        return queryFirstZero(2 * node + 1, mid + 1, end);
    }
    
    // Initialize tree with inactive values
    void build(int node, int start, int end) {
        if (start == end) {
            tree_min[node] = INF;
            tree_max[node] = NEG_INF;
            lazy[node] = 0;
            return;
        }
        int mid = (start + end) / 2;
        build(2 * node, start, mid);
        build(2 * node + 1, mid + 1, end);
        tree_min[node] = min(tree_min[2 * node], tree_min[2 * node + 1]);
        tree_max[node] = max(tree_max[2 * node], tree_max[2 * node + 1]);
    }

public:
    int longestBalanced(vector<int>& nums) {
        n = nums.size();
        // 4*N size for segment tree
        tree_min.assign(4 * n, 0);
        tree_max.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
        
        build(1, 0, n - 1);
        
        // Stores the last seen position of each number. 
        // Max value is 10^5, so size 100001 is sufficient.
        vector<int> last_pos(100001, -1);
        int max_len = 0;
        
        for (int i = 0; i < n; ++i) {
            // 1. A new potential start index 'i' becomes valid (initially empty subarray, count 0)
            set_zero(1, 0, n - 1, i);
            
            // 2. Determine contribution of nums[i]
            int val = nums[i];
            int prev = last_pos[val];
            // If even +1, if odd -1
            int diff = (val % 2 == 0) ? 1 : -1; 
            
            // 3. Update the balance for all start indices L where nums[i] is a new distinct element.
            // These are indices L such that prev < L <= i.
            update(1, 0, n - 1, prev + 1, i, diff);
            
            last_pos[val] = i;
            
            // 4. Find the leftmost start index L where the balance is 0
            int l = queryFirstZero(1, 0, n - 1);
            if (l != -1) {
                max_len = max(max_len, i - l + 1);
            }
        }
        
        return max_len;
    }
};