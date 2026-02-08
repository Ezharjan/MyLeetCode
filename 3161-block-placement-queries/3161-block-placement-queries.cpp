#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

// Maximum coordinate value based on constraints (5 * 10^4)
const int MAX_VAL = 50005;

class SegmentTree {
    vector<int> tree;
    int size;

public:
    SegmentTree(int n) {
        size = n;
        tree.assign(4 * n, 0);
    }

    // Update the value at a specific index (coordinate)
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        // Store the maximum gap in the current range
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    // Query the maximum value in the range [l, r]
    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            return 0;
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        int mid = (start + end) / 2;
        int p1 = query(2 * node, start, mid, l, r);
        int p2 = query(2 * node + 1, mid + 1, end, l, r);
        return max(p1, p2);
    }
};

class Solution {
public:
    vector<bool> getResults(vector<vector<int>>& queries) {
        // Set stores obstacle positions. Always initialized with 0.
        set<int> obstacles;
        obstacles.insert(0);
        
        // Segment tree stores the size of the gap ENDING at index i.
        SegmentTree st(MAX_VAL);
        
        vector<bool> results;

        for (const auto& q : queries) {
            int type = q[0];
            int x = q[1];

            if (type == 1) {
                // Find the existing interval [prev_obs, next_obs] that contains x
                auto it = obstacles.upper_bound(x); // iterator to next_obs
                int prev_obs = *prev(it); // value of prev_obs
                
                // If there is an obstacle to the right, update its gap size
                if (it != obstacles.end()) {
                    int next_obs = *it;
                    // The gap ending at next_obs shrinks from (next - prev) to (next - x)
                    st.update(1, 0, MAX_VAL, next_obs, next_obs - x);
                }

                // Create a new gap ending at x with size (x - prev_obs)
                st.update(1, 0, MAX_VAL, x, x - prev_obs);
                
                // Add x to the set of obstacles
                obstacles.insert(x);

            } else { // type == 2
                int sz = q[2];
                
                // Find the largest obstacle <= x
                auto it = obstacles.upper_bound(x);
                int max_obs = *prev(it);
                
                // 1. Max gap strictly between existing obstacles in [0, max_obs]
                int max_existing_gap = st.query(1, 0, MAX_VAL, 0, max_obs);
                
                // 2. The "trailing" gap between the last obstacle and the query limit x
                int trailing_gap = x - max_obs;
                
                // Check if either is large enough
                if (max(max_existing_gap, trailing_gap) >= sz) {
                    results.push_back(true);
                } else {
                    results.push_back(false);
                }
            }
        }
        
        return results;
    }
};