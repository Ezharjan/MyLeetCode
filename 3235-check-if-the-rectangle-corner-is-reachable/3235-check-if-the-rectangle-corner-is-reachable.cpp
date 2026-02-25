class Solution {
    bool touch_L(long long x, long long y, long long r, long long X, long long Y) {
        return (x <= r) && (y <= Y || x * x + (y - Y) * (y - Y) <= r * r);
    }
    
    bool touch_B(long long x, long long y, long long r, long long X, long long Y) {
        return (y <= r) && (x <= X || (x - X) * (x - X) + y * y <= r * r);
    }
    
    bool touch_R(long long x, long long y, long long r, long long X, long long Y) {
        return (abs(x - X) <= r) && (y <= Y || (x - X) * (x - X) + (y - Y) * (y - Y) <= r * r);
    }
    
    bool touch_T(long long x, long long y, long long r, long long X, long long Y) {
        return (abs(y - Y) <= r) && (x <= X || (x - X) * (x - X) + (y - Y) * (y - Y) <= r * r);
    }

public:
    bool canReachCorner(int xCorner, int yCorner, vector<vector<int>>& circles) {
        int n = circles.size();
        long long X = xCorner;
        long long Y = yCorner;
        
        vector<bool> g1(n, false);
        vector<bool> g2(n, false);
        
        for (int i = 0; i < n; ++i) {
            long long x = circles[i][0];
            long long y = circles[i][1];
            long long r = circles[i][2];
            
            // Group 1: Touches Left or Top
            if (touch_L(x, y, r, X, Y) || touch_T(x, y, r, X, Y)) {
                g1[i] = true;
            }
            // Group 2: Touches Right or Bottom
            if (touch_R(x, y, r, X, Y) || touch_B(x, y, r, X, Y)) {
                g2[i] = true;
            }
        }
        
        vector<vector<int>> adj(n);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                long long x1 = circles[i][0], y1 = circles[i][1], r1 = circles[i][2];
                long long x2 = circles[j][0], y2 = circles[j][1], r2 = circles[j][2];
                
                long long dx = x1 - x2;
                long long dy = y1 - y2;
                long long r_sum = r1 + r2;
                
                // Do the circles intersect?
                if (dx * dx + dy * dy <= r_sum * r_sum) {
                    // Center of mass filter checks to rule out out-of-bounds intersection links
                    if (x1 * r2 + x2 * r1 <= X * r_sum &&
                        y1 * r2 + y2 * r1 <= Y * r_sum) {
                        adj[i].push_back(j);
                        adj[j].push_back(i);
                    }
                }
            }
        }
        
        vector<bool> vis(n, false);
        for (int i = 0; i < n; ++i) {
            // Initiate BFS from any unvisited node in Group 1
            if (g1[i] && !vis[i]) {
                vector<int> q;
                q.push_back(i);
                vis[i] = true;
                
                int head = 0;
                while (head < q.size()) {
                    int u = q[head++];
                    
                    // If a node in Group 1 is chained to a node in Group 2, the path is blocked
                    if (g2[u]) return false; 
                    
                    for (int v : adj[u]) {
                        if (!vis[v]) {
                            vis[v] = true;
                            q.push_back(v);
                        }
                    }
                }
            }
        }
        
        return true; 
    }
};