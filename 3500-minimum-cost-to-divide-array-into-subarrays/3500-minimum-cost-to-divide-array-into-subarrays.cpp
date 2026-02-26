class Solution {
    struct Line {
        long long m;
        __int128_t c;
        __int128_t get(long long x) const {
            return (__int128_t)m * x + c;
        }
    };

    struct Node {
        Line line;
        int left, right;
        Node() : line({0, ((__int128_t)1 << 110)}), left(-1), right(-1) {}
    };

    vector<Node> tree;

    __int128_t min128(__int128_t a, __int128_t b) {
        return a < b ? a : b;
    }

    void add_line(int node, int l, int r, Line new_line, const vector<long long>& xs) {
        if (l == r) {
            if (new_line.get(xs[l]) < tree[node].line.get(xs[l])) {
                tree[node].line = new_line;
            }
            return;
        }
        int mid = l + (r - l) / 2;
        bool better_left = new_line.get(xs[l]) < tree[node].line.get(xs[l]);
        bool better_mid = new_line.get(xs[mid]) < tree[node].line.get(xs[mid]);
        
        if (better_mid) {
            swap(tree[node].line, new_line);
        }
        
        if (better_left != better_mid) {
            if (tree[node].left == -1) {
                tree[node].left = tree.size();
                tree.emplace_back();
            }
            // Passing tree[node].left by value handles dynamic reallocation safely 
            add_line(tree[node].left, l, mid, new_line, xs);
        } else {
            if (tree[node].right == -1) {
                tree[node].right = tree.size();
                tree.emplace_back();
            }
            add_line(tree[node].right, mid + 1, r, new_line, xs);
        }
    }

    __int128_t query(int node, int l, int r, int x_idx, const vector<long long>& xs) {
        if (node == -1) return ((__int128_t)1 << 110);
        __int128_t res = tree[node].line.get(xs[x_idx]);
        if (l == r) return res;
        int mid = l + (r - l) / 2;
        if (x_idx <= mid) {
            res = min128(res, query(tree[node].left, l, mid, x_idx, xs));
        } else {
            res = min128(res, query(tree[node].right, mid + 1, r, x_idx, xs));
        }
        return res;
    }

public:
    long long minimumCost(vector<int>& nums, vector<int>& cost, int k) {
        int n = nums.size();
        vector<long long> P(n), C(n);
        P[0] = nums[0];
        C[0] = cost[0];
        for (int i = 1; i < n; ++i) {
            P[i] = P[i - 1] + nums[i];
            C[i] = C[i - 1] + cost[i];
        }

        // Coordinate compress evaluated P[r] (X-Coordinates)
        vector<long long> xs = P;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        int M = xs.size();

        tree.clear();
        tree.reserve(n * 20); // Pre-allocate to prevent reallocation invalidation
        tree.emplace_back();

        // Base case setup for the j = -1 logic
        add_line(0, 0, M - 1, {0, 0}, xs);

        __int128_t current_dp = 0;
        for (int i = 0; i < n; ++i) {
            int idx = lower_bound(xs.begin(), xs.end(), P[i]) - xs.begin();
            __int128_t val = query(0, 0, M - 1, idx, xs);
            
            // Calculate best cost mapping up to current `i` index partition  
            current_dp = (__int128_t)P[i] * C[i] + (__int128_t)k * C[n - 1] + val;
            
            if (i < n - 1) {
                Line new_line;
                new_line.m = -C[i];
                new_line.c = current_dp - (__int128_t)k * C[i];
                add_line(0, 0, M - 1, new_line, xs);
            }
        }

        return (long long)current_dp;
    }
};