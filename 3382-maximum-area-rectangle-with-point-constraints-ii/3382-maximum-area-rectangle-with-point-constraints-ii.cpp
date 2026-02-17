#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    // Fenwick Tree (Binary Indexed Tree) for point counting
    struct Fenwick {
        int n;
        vector<int> tree;
        Fenwick(int n) : n(n), tree(n + 1, 0) {}
        void add(int i, int delta) {
            for (; i <= n; i += i & -i) tree[i] += delta;
        }
        int query(int i) {
            int sum = 0;
            for (; i > 0; i -= i & -i) sum += tree[i];
            return sum;
        }
    };

    struct Point {
        int x, y, cy;
        bool operator<(const Point& o) const {
            if (x != o.x) return x < o.x;
            return y < o.y;
        }
    };

public:
    long long maxRectangleArea(vector<int>& xCoord, vector<int>& yCoord) {
        int n = xCoord.size();
        if (n < 4) return -1;
        
        // 1. Coordinate compression on Y-coordinates
        vector<int> y_vals = yCoord;
        sort(y_vals.begin(), y_vals.end());
        y_vals.erase(unique(y_vals.begin(), y_vals.end()), y_vals.end());
        int U = y_vals.size();
        
        vector<Point> pts(n);
        for(int i = 0; i < n; ++i) {
            pts[i].x = xCoord[i];
            pts[i].y = yCoord[i];
            // Get 1-based compressed Y-index for the Fenwick tree
            pts[i].cy = distance(y_vals.begin(), lower_bound(y_vals.begin(), y_vals.end(), yCoord[i])) + 1;
        }
        // Sort points left-to-right, bottom-to-top
        sort(pts.begin(), pts.end());
        
        // 2. Extract and assign an ID to all uniquely occurring adjacent Y-coordinate pairs
        vector<pair<int, int>> all_pairs;
        all_pairs.reserve(n);
        for (int i = 0; i < n; ) {
            int j = i;
            while (j < n && pts[j].x == pts[i].x) j++;
            for (int p = i; p < j - 1; ++p) {
                all_pairs.push_back({pts[p].cy, pts[p+1].cy});
            }
            i = j;
        }
        sort(all_pairs.begin(), all_pairs.end());
        all_pairs.erase(unique(all_pairs.begin(), all_pairs.end()), all_pairs.end());
        
        int num_pairs = all_pairs.size();
        // Stores {previous_x, count_of_points_in_band}
        vector<pair<int, int>> last_seen(num_pairs, {-1, -1}); 
        
        Fenwick bit(U);
        long long max_area = -1;
        vector<int> current_ids;
        current_ids.reserve(n);
        
        // 3. Sweep Line phase
        for (int i = 0; i < n; ) {
            int j = i;
            while (j < n && pts[j].x == pts[i].x) {
                j++;
            }
            current_ids.clear();
            
            // Step A: Check segments formed at the current X to compute areas
            for (int p = i; p < j - 1; ++p) {
                int cy1 = pts[p].cy;
                int cy2 = pts[p+1].cy;
                auto it = lower_bound(all_pairs.begin(), all_pairs.end(), make_pair(cy1, cy2));
                int id = distance(all_pairs.begin(), it);
                current_ids.push_back(id);
                
                int current_count = bit.query(cy2) - bit.query(cy1 - 1);
                
                // If we've recorded this Y segment at an earlier X before
                if (last_seen[id].first != -1) {
                    auto [prev_x, prev_count] = last_seen[id];
                    // Verify if the region inside and along horizontal borders is devoid of other points
                    if (current_count == prev_count) {
                        int y1 = pts[p].y;
                        int y2 = pts[p+1].y;
                        long long area = 1LL * (pts[i].x - prev_x) * (y2 - y1);
                        if (area > max_area) {
                            max_area = area;
                        }
                    }
                }
            }
            
            // Step B: Mark the points existing at the current X on our Fenwick tree
            for (int p = i; p < j; ++p) {
                bit.add(pts[p].cy, 1);
            }
            
            // Step C: Update `last_seen` maps after inclusion
            for (int p = i; p < j - 1; ++p) {
                int cy1 = pts[p].cy;
                int cy2 = pts[p+1].cy;
                int id = current_ids[p - i];
                
                int count_after = bit.query(cy2) - bit.query(cy1 - 1);
                last_seen[id] = {pts[i].x, count_after};
            }
            i = j;
        }
        
        return max_area;
    }
};