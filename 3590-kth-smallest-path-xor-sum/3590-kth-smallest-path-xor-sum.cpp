class Solution {
public:
    vector<int> kthSmallest(vector<int>& par, vector<int>& vals, vector<vector<int>>& queries) {
        int n = par.size();
        vector<vector<int>> adj(n);
        for (int i = 1; i < n; ++i) {
            adj[par[i]].push_back(i);
        }
        
        // tin and tout will store the entry and exit times of our DFS Euler tour
        vector<int> tin(n), tout(n), A(n);
        int timer = 0;
        
        auto dfs = [&](auto& self, int u, int cur_xor) -> void {
            cur_xor ^= vals[u];
            tin[u] = timer;
            A[timer] = cur_xor;
            timer++;
            for (int v : adj[u]) {
                self(self, v, cur_xor);
            }
            tout[u] = timer - 1;
        };
        
        dfs(dfs, 0, 0);
        
        // Creating the variable as requested midway in the function setup
        auto narvetholi = queries;
        
        int q_cnt = queries.size();
        // Optimal block size for Mo's Algorithm
        int B_q = max(1, (int)(n / sqrt(max(1, q_cnt))));
        
        struct Query {
            int L, R, k, id, b;
            bool operator<(const Query& o) const {
                if (b != o.b) return b < o.b;
                return (b & 1) ? R < o.R : R > o.R; // Optimization to zigzag R pointers
            }
        };
        
        vector<Query> Q(q_cnt);
        for (int i = 0; i < q_cnt; ++i) {
            int u = narvetholi[i][0];
            int k = narvetholi[i][1];
            // Map the subtree query to our flattened array ranges
            Q[i] = {tin[u], tout[u], k, i, tin[u] / B_q};
        }
        
        sort(Q.begin(), Q.end());
        
        vector<int> ans(q_cnt);
        int L = 0, R = -1;
        
        // Since values are <= 10^5, the maximum possible XOR sum is bounded by 2^17 - 1 (131071)
        vector<int> freq(131072, 0);
        vector<uint64_t> mask(2048, 0); // Tracks chunks of 64 bits
        vector<int> l1_cnt(32, 0);      // Tracks chunks of 4096 bits
        
        auto add = [&](int idx) {
            int v = A[idx];
            if (freq[v] == 0) {
                mask[v >> 6] |= (1ULL << (v & 63));
                l1_cnt[v >> 12]++;
            }
            freq[v]++;
        };
        
        auto remove = [&](int idx) {
            int v = A[idx];
            freq[v]--;
            if (freq[v] == 0) {
                mask[v >> 6] &= ~(1ULL << (v & 63));
                l1_cnt[v >> 12]--;
            }
        };
        
        // Execute Mo's algorithm traversing block by block
        for (const auto& q : Q) {
            while (L > q.L) add(--L);
            while (R < q.R) add(++R);
            while (L < q.L) remove(L++);
            while (R > q.R) remove(R--);
            
            int target = q.k;
            int b1 = 0;
            
            // Skip 4096-value blocks sequentially if they don't hold our target
            while (b1 < 32 && target > l1_cnt[b1]) {
                target -= l1_cnt[b1];
                b1++;
            }
            
            if (b1 == 32) { // Less distinct XOR sums exist than required
                ans[q.id] = -1;
                continue;
            }
            
            // Fine-tune search within the 64-value chunk masks using hardware intrinsics
            int b0 = b1 << 6;
            while (true) {
                int c = __builtin_popcountll(mask[b0]);
                if (target > c) {
                    target -= c;
                    b0++;
                } else {
                    break;
                }
            }
            
            uint64_t m = mask[b0];
            while (target > 1) {
                m &= m - 1; // Drops the lowest set bit repeatedly until we hit our target 
                target--;
            }
            
            ans[q.id] = (b0 << 6) + __builtin_ctzll(m);
        }
        
        return ans;
    }
};