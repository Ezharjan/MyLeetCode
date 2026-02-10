#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

typedef long long ll;

class BookMyShow {
    int n;
    int m;
    
    // Segment Tree arrays
    // min_tree: stores the minimum number of 'used' seats in a range of rows
    // sum_tree: stores the total sum of 'used' seats in a range of rows
    vector<int> min_tree;
    vector<ll> sum_tree;
    
    // Optimization: tracks the first row that is NOT full.
    // Rows < start_row are completely full (used == m).
    int start_row;

    // Standard Segment Tree Update: Point update
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            min_tree[node] = val;
            sum_tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        // Aggregate values
        min_tree[node] = min(min_tree[2 * node], min_tree[2 * node + 1]);
        sum_tree[node] = sum_tree[2 * node] + sum_tree[2 * node + 1];
    }

    // Query for gather: Find smallest row index in [0, maxRow] with used <= m - k
    int queryGather(int node, int start, int end, int k, int maxRow) {
        // Pruning: If the minimum used seats in this range is already too high, 
        // no row here can accommodate k seats.
        if (min_tree[node] > m - k) return -1;
        
        // If current range starts beyond maxRow, ignore
        if (start > maxRow) return -1;

        // Leaf node: we found a valid row
        if (start == end) return start;

        int mid = (start + end) / 2;
        
        // Prefer left child (smaller row index)
        int res = queryGather(2 * node, start, mid, k, maxRow);
        if (res != -1) return res;
        
        // If not found in left, try right child (only if it overlaps with valid range)
        if (mid < maxRow) {
            return queryGather(2 * node + 1, mid + 1, end, k, maxRow);
        }
        
        return -1;
    }

    // Query for scatter: Sum of used seats in range [0, maxRow]
    ll querySum(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return sum_tree[node];
        int mid = (start + end) / 2;
        return querySum(2 * node, start, mid, l, r) + querySum(2 * node + 1, mid + 1, end, l, r);
    }
    
    // Helper to store current usage of each row to avoid frequent leaf queries
    // or reconstructing state.
    vector<int> row_usage;

public:
    BookMyShow(int n, int m) : n(n), m(m), start_row(0) {
        // 4*n is standard safe size for segment tree
        min_tree.resize(4 * n, 0);
        sum_tree.resize(4 * n, 0);
        row_usage.resize(n, 0);
    }
    
    vector<int> gather(int k, int maxRow) {
        // Find the first row that can fit k contiguous seats
        int r = queryGather(1, 0, n - 1, k, maxRow);
        
        if (r == -1) return {}; // No such row found
        
        int seat = row_usage[r];
        
        // Update state
        row_usage[r] += k;
        update(1, 0, n - 1, r, row_usage[r]);
        
        return {r, seat};
    }
    
    bool scatter(int k, int maxRow) {
        // 1. Check Feasibility using Sum Tree
        // Total capacity in range [0, maxRow]
        ll capacity = (ll)(maxRow + 1) * m;
        // Total already used in range [0, maxRow]
        ll used = querySum(1, 0, n - 1, 0, maxRow);
        
        if (capacity - used < k) return false; // Not enough seats
        
        // 2. Perform Booking
        // Start from start_row (optimization to skip fully filled rows)
        for (int i = start_row; i <= maxRow && k > 0; ++i) {
            int current_used = row_usage[i];
            
            // If row is full, just update start_row pointer if applicable and continue
            if (current_used == m) {
                if (i == start_row) start_row++;
                continue;
            }
            
            int available = m - current_used;
            int take = min(k, available);
            
            row_usage[i] += take;
            k -= take;
            
            // Update Segment Tree
            update(1, 0, n - 1, i, row_usage[i]);
            
            // If we just filled this row completely and it was the start_row,
            // move the pointer forward for future efficiency.
            if (row_usage[i] == m && i == start_row) {
                start_row++;
            }
        }
        
        return true;
    }
};

/**
 * Your BookMyShow object will be instantiated and called as such:
 * BookMyShow* obj = new BookMyShow(n, m);
 * vector<int> param_1 = obj->gather(k,maxRow);
 * bool param_2 = obj->scatter(k,maxRow);
 */