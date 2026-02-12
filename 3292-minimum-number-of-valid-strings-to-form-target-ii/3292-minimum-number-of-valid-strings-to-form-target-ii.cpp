#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
    // Segment Tree to perform Range Update and Point Query for Max values
    struct SegmentTree {
        int n;
        vector<int> tree;
        vector<int> lazy;

        SegmentTree(int size) : n(size), tree(4 * size, -1), lazy(4 * size, -1) {}

        void push(int node) {
            if (lazy[node] != -1) {
                tree[2 * node] = max(tree[2 * node], lazy[node]);
                lazy[2 * node] = max(lazy[2 * node], lazy[node]);
                tree[2 * node + 1] = max(tree[2 * node + 1], lazy[node]);
                lazy[2 * node + 1] = max(lazy[2 * node + 1], lazy[node]);
                lazy[node] = -1;
            }
        }

        // Update range [l, r] with value val
        void update(int node, int start, int end, int l, int r, int val) {
            if (l > end || r < start) return;
            if (l <= start && end <= r) {
                tree[node] = max(tree[node], val);
                lazy[node] = max(lazy[node], val);
                return;
            }
            push(node);
            int mid = (start + end) / 2;
            update(2 * node, start, mid, l, r, val);
            update(2 * node + 1, mid + 1, end, l, r, val);
            tree[node] = max(tree[2 * node], tree[2 * node + 1]);
        }

        // Query max value at specific index idx
        int query(int node, int start, int end, int idx) {
            if (start == end) return tree[node];
            push(node);
            int mid = (start + end) / 2;
            if (idx <= mid) return query(2 * node, start, mid, idx);
            else return query(2 * node + 1, mid + 1, end, idx);
        }
    };

    // AC Automaton structures
    vector<vector<int>> tr;
    vector<int> fail;
    vector<int> depth; 
    int nodes_count;

public:
    int minValidStrings(vector<string>& words, string target) {
        // 1. Initialize AC Automaton memory
        int total_len = 0;
        for(const string& w : words) total_len += w.length();
        
        // Assign size based on total characters + buffer
        tr.assign(total_len + 5, vector<int>(26, 0));
        fail.assign(total_len + 5, 0);
        depth.assign(total_len + 5, 0);
        nodes_count = 1;

        // 2. Build Trie
        for (const string& w : words) {
            int u = 0;
            for (char c : w) {
                int idx = c - 'a';
                if (!tr[u][idx]) {
                    tr[u][idx] = nodes_count++;
                    depth[tr[u][idx]] = depth[u] + 1;
                }
                u = tr[u][idx];
            }
        }

        // 3. Build Fail Links (AC Automation Graph Construction)
        queue<int> q;
        for (int i = 0; i < 26; ++i) {
            if (tr[0][i]) q.push(tr[0][i]);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < 26; ++i) {
                if (tr[u][i]) {
                    fail[tr[u][i]] = tr[fail[u]][i];
                    q.push(tr[u][i]);
                } else {
                    // Optimization: Direct transition to fail node if child doesn't exist
                    tr[u][i] = tr[fail[u]][i];
                }
            }
        }

        int n = target.length();
        
        // 4. Process Target to find longest valid string ending at each position
        vector<int> L(n, 0);
        int u = 0;
        for (int i = 0; i < n; ++i) {
            u = tr[u][target[i] - 'a'];
            L[i] = depth[u]; 
        }

        // 5. Populate Segment Tree with intervals
        // If L[i] = len, it means we can start a valid string at any k in [i-len+1, i]
        // and reach 'i'.
        SegmentTree st(n);
        for (int i = 0; i < n; ++i) {
            if (L[i] > 0) {
                int start = i - L[i] + 1;
                st.update(1, 0, n - 1, start, i, i);
            }
        }

        // 6. Greedy Jump Game (Minimum number of intervals to cover [0, n-1])
        int jumps = 0;
        int cur_end = 0;
        int farthest = 0;

        for (int i = 0; i < n; ++i) {
            // Find the furthest index we can reach if we start a new string at i
            int max_end_from_i = st.query(1, 0, n - 1, i);
            
            // If we cannot start any valid string at i, it's impossible
            if (max_end_from_i < i) return -1;
            
            // From i, we can cover up to max_end_from_i.
            // The next string would start at max_end_from_i + 1.
            farthest = max(farthest, max_end_from_i + 1);
            
            // If we reached the end of the current jump range
            if (i == cur_end) {
                jumps++;
                cur_end = farthest;
                // If we have covered the entire target
                if (cur_end >= n) return jumps;
            }
        }
        
        return -1;
    }
};