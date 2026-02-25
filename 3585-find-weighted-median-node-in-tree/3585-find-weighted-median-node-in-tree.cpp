class Solution {
    int LOG = 18;
    vector<vector<int>> up;
    vector<int> depth;
    vector<long long> dist;
    vector<vector<pair<int, long long>>> adj;

    void dfs(int node, int parent, int d, long long w) {
        // Handle boundary at the root node
        up[node][0] = parent == -1 ? node : parent;
        depth[node] = d;
        dist[node] = w;
        
        // Populate the binary lifting table
        for (int i = 1; i < LOG; i++) {
            up[node][i] = up[up[node][i - 1]][i - 1];
        }
        
        for (auto& edge : adj[node]) {
            int neighbor = edge.first;
            long long weight = edge.second;
            if (neighbor != parent) {
                dfs(neighbor, node, d + 1, w + weight);
            }
        }
    }

    int getLCA(int u, int v) {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }
        
        int diff = depth[u] - depth[v];
        // Balance out the depths
        for (int i = 0; i < LOG; i++) {
            if ((diff >> i) & 1) {
                u = up[u][i];
            }
        }
        
        if (u == v) return u;
        
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        
        return up[u][0];
    }

public:
    vector<int> findMedian(int n, vector<vector<int>>& edges, vector<vector<int>>& queries) {
        LOG = 18; // Since N <= 10^5, log2(10^5) is bounded within 17 so 18 is plenty
        up.assign(n, vector<int>(LOG));
        depth.assign(n, 0);
        dist.assign(n, 0);
        adj.assign(n, vector<pair<int, long long>>());

        // Build adjacency list
        for (auto& edge : edges) {
            adj[edge[0]].push_back({edge[1], edge[2]});
            adj[edge[1]].push_back({edge[0], edge[2]});
        }

        // Initialize dfs processing from root node 0
        dfs(0, -1, 0, 0);

        vector<int> ans;
        ans.reserve(queries.size());
        
        for (auto& q : queries) {
            int u = q[0], v = q[1];
            if (u == v) {
                ans.push_back(u);
                continue;
            }

            int L = getLCA(u, v);
            long long W = dist[u] + dist[v] - 2LL * dist[L];

            if (2LL * (dist[u] - dist[L]) >= W) {
                // The Median is on the path segment running from u to L
                int curr = u;
                for (int i = LOG - 1; i >= 0; i--) {
                    if (depth[curr] - (1 << i) >= depth[L]) {
                        int next_node = up[curr][i];
                        if (2LL * (dist[u] - dist[next_node]) < W) {
                            curr = next_node;
                        }
                    }
                }
                ans.push_back(up[curr][0]);
            } else {
                // The Median is on the path segment running from L to v
                int curr = v;
                for (int i = LOG - 1; i >= 0; i--) {
                    if (depth[curr] - (1 << i) >= depth[L]) {
                        int next_node = up[curr][i];
                        if (2LL * (dist[v] - dist[next_node]) <= W) {
                            curr = next_node;
                        }
                    }
                }
                ans.push_back(curr);
            }
        }
        return ans;
    }
};