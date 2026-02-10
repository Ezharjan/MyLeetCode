#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
    struct Node {
        int l, r;       // Left and right children indices
        int count;      // Count of elements in this range
        long long sum;  // Sum of values in this range
    };
    
    vector<Node> tree;
    int node_cnt;

    // Build an empty segment tree
    void build(int &u, int l, int r) {
        u = ++node_cnt;
        // Ensure tree has enough space; usually resized once at start
        tree[u].count = 0;
        tree[u].sum = 0;
        if (l == r) return;
        int mid = l + (r - l) / 2;
        build(tree[u].l, l, mid);
        build(tree[u].r, mid + 1, r);
    }
    
    // Create a new version of the tree with an updated value
    void update(int prev, int &u, int l, int r, int val_idx, long long real_val) {
        u = ++node_cnt;
        tree[u] = tree[prev]; // Copy previous node
        tree[u].count++;
        tree[u].sum += real_val;
        if (l == r) return;
        int mid = l + (r - l) / 2;
        if (val_idx <= mid) {
            update(tree[prev].l, tree[u].l, l, mid, val_idx, real_val);
        } else {
            update(tree[prev].r, tree[u].r, mid + 1, r, val_idx, real_val);
        }
    }
    
    // Find the index in sorted_vals of the k-th smallest element in the range query
    int query_kth(int u_l, int u_r, int l, int r, int k) {
        if (l == r) return l;
        int mid = l + (r - l) / 2;
        int left_count = tree[tree[u_r].l].count - tree[tree[u_l].l].count;
        if (k <= left_count) {
            return query_kth(tree[u_l].l, tree[u_r].l, l, mid, k);
        } else {
            return query_kth(tree[u_l].r, tree[u_r].r, mid + 1, r, k - left_count);
        }
    }
    
    // Get the count and sum of elements whose rank is <= target_idx
    pair<int, long long> query_prefix(int u_l, int u_r, int l, int r, int target_idx) {
        if (target_idx < l) return {0, 0LL};
        if (r <= target_idx) {
            return {tree[u_r].count - tree[u_l].count, tree[u_r].sum - tree[u_l].sum};
        }
        int mid = l + (r - l) / 2;
        pair<int, long long> left_res = query_prefix(tree[u_l].l, tree[u_r].l, l, mid, target_idx);
        pair<int, long long> right_res = query_prefix(tree[u_l].r, tree[u_r].r, mid + 1, r, target_idx);
        return {left_res.first + right_res.first, left_res.second + right_res.second};
    }

public:
    vector<long long> minOperations(vector<int>& nums, int k, vector<vector<int>>& queries) {
        int n = nums.size();
        
        // 1. Precompute Possibility Checks
        // mod_diff_pref[i] stores the count of adjacent mismatches in (nums[j] % k) up to i
        vector<int> mod_diff_pref(n, 0);
        for (int i = 1; i < n; ++i) {
            bool diff = false;
            if (k == 0) {
                if (nums[i] != nums[i-1]) diff = true;
            } else {
                if ((nums[i] % k) != (nums[i-1] % k)) diff = true;
            }
            mod_diff_pref[i] = mod_diff_pref[i-1] + (diff ? 1 : 0);
        }
        
        // 2. Prepare Values for Operations Calculation
        // We operate on the quotients: val = nums[i] / k
        vector<long long> vals;
        vals.reserve(n);
        if (k == 0) {
            vals.assign(n, 0); // Placeholder, unused if k=0
        } else {
            for (int x : nums) vals.push_back((long long)x / k);
        }
        
        // Coordinate Compression
        vector<long long> sorted_vals = vals;
        sort(sorted_vals.begin(), sorted_vals.end());
        sorted_vals.erase(unique(sorted_vals.begin(), sorted_vals.end()), sorted_vals.end());
        int m = sorted_vals.size();
        
        auto get_rank = [&](long long val) {
            return lower_bound(sorted_vals.begin(), sorted_vals.end(), val) - sorted_vals.begin();
        };
        
        // 3. Build Persistent Segment Tree
        // Max nodes estimate: N * log(M) ~ N * 25. 
        int max_nodes = n * 25 + 50000;
        tree.resize(max_nodes);
        node_cnt = 0;
        
        vector<int> roots(n + 1);
        build(roots[0], 0, m - 1);
        
        for (int i = 0; i < n; ++i) {
            update(roots[i], roots[i+1], 0, m - 1, get_rank(vals[i]), vals[i]);
        }
        
        // 4. Answer Queries
        vector<long long> ans;
        ans.reserve(queries.size());
        
        for (const auto& q : queries) {
            int l = q[0];
            int r = q[1];
            
            // Check Possibility: Must have no modulo differences in range [l+1, r]
            if (l < r) {
                if (mod_diff_pref[r] - mod_diff_pref[l] > 0) {
                    ans.push_back(-1);
                    continue;
                }
            }
            
            if (k == 0) {
                // If possible and k=0, array is already equal. Cost 0.
                ans.push_back(0);
                continue;
            }
            
            // Find Median
            int cnt = r - l + 1;
            int rank_k = cnt / 2 + 1; // 1-based rank of the median
            
            int median_idx = query_kth(roots[l], roots[r+1], 0, m - 1, rank_k);
            long long median_val = sorted_vals[median_idx];
            
            // Calculate Cost: sum(|x - median|)
            // Split into sum(median - x) for x <= median AND sum(x - median) for x > median
            pair<int, long long> prefix = query_prefix(roots[l], roots[r+1], 0, m - 1, median_idx);
            long long cnt_le = prefix.first;    // Count of nums <= median
            long long sum_le = prefix.second;   // Sum of nums <= median
            
            long long total_sum = tree[roots[r+1]].sum - tree[roots[l]].sum;
            
            long long cost = (cnt_le * median_val - sum_le) + (total_sum - sum_le - (cnt - cnt_le) * median_val);
            ans.push_back(cost);
        }
        
        return ans;
    }
};