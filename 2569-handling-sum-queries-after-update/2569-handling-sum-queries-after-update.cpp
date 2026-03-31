#include <vector>
#include <numeric>

using namespace std;

class Solution {
    vector<int> tree;
    vector<bool> lazy;

    // Build the segment tree initially
    void build(int node, int L, int R, const vector<int>& nums1) {
        if (L == R) {
            tree[node] = nums1[L];
            return;
        }
        int mid = L + (R - L) / 2;
        build(2 * node, L, mid, nums1);
        build(2 * node + 1, mid + 1, R, nums1);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    // Push down the lazy flip states to children nodes
    void push(int node, int L, int R) {
        if (lazy[node]) {
            int mid = L + (R - L) / 2;
            
            // Left child
            tree[2 * node] = (mid - L + 1) - tree[2 * node];
            lazy[2 * node] = !lazy[2 * node];
            
            // Right child
            tree[2 * node + 1] = (R - mid) - tree[2 * node + 1];
            lazy[2 * node + 1] = !lazy[2 * node + 1];
            
            // Clear current node's lazy flag
            lazy[node] = false;
        }
    }

    // Range flip update
    void update(int node, int L, int R, int l, int r) {
        if (l > R || r < L) return; // Out of bounds
        
        // Fully covered
        if (l <= L && R <= r) {
            tree[node] = (R - L + 1) - tree[node];
            lazy[node] = !lazy[node];
            return;
        }
        
        // Partial overlap, push lazy states down first
        push(node, L, R);
        
        int mid = L + (R - L) / 2;
        update(2 * node, L, mid, l, r);
        update(2 * node + 1, mid + 1, R, l, r);
        
        // Update current node based on newly updated children
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

public:
    vector<long long> handleQuery(vector<int>& nums1, vector<int>& nums2, vector<vector<int>>& queries) {
        int n = nums1.size();
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, false);

        // 1. Build the segment tree for nums1
        build(1, 0, n - 1, nums1);

        // 2. Track the total sum of nums2
        long long total_sum_nums2 = 0;
        for (int x : nums2) {
            total_sum_nums2 += x;
        }

        vector<long long> result;

        // 3. Process Queries
        for (const auto& q : queries) {
            if (q[0] == 1) {
                // Flip bits in range [q[1], q[2]]
                update(1, 0, n - 1, q[1], q[2]);
            } 
            else if (q[0] == 2) {
                // Add p * (count of 1s in nums1) to the total sum
                long long p = q[1];
                long long count_ones = tree[1]; // Root holds total 1s
                total_sum_nums2 += count_ones * p;
            } 
            else if (q[0] == 3) {
                // Return total sum of nums2
                result.push_back(total_sum_nums2);
            }
        }

        return result;
    }
};