#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxPathLength(vector<vector<int>>& coordinates, int k) {
        int n = coordinates.size();
        int kx = coordinates[k][0];
        int ky = coordinates[k][1];
        
        // Split coordinates into two groups relative to coordinates[k]
        vector<vector<int>> left, right;
        
        for (const auto& p : coordinates) {
            int x = p[0];
            int y = p[1];
            
            // Strictly smaller than target for the path BEFORE k
            if (x < kx && y < ky) {
                left.push_back(p);
            }
            // Strictly larger than target for the path AFTER k
            else if (x > kx && y > ky) {
                right.push_back(p);
            }
        }
        
        // The result is the longest path ending at k + longest path starting at k + 1 (for k itself)
        return solveLIS(left) + 1 + solveLIS(right);
    }

private:
    int solveLIS(vector<vector<int>>& points) {
        if (points.empty()) return 0;
        
        // Sort based on x ascending.
        // If x is same, sort by y descending to avoid picking same x twice.
        sort(points.begin(), points.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] == b[0]) {
                return a[1] > b[1]; // y descending
            }
            return a[0] < b[0]; // x ascending
        });
        
        // Standard LIS on y-coordinates using a 'tails' array
        vector<int> tails;
        
        for (const auto& p : points) {
            int y = p[1];
            
            // Use binary search (lower_bound) to find the first element >= y
            auto it = lower_bound(tails.begin(), tails.end(), y);
            
            if (it == tails.end()) {
                // If no element >= y, we can extend the longest increasing subsequence
                tails.push_back(y);
            } else {
                // If found, replace it with y to maintain the smallest ending element potential
                *it = y;
            }
        }
        
        return tails.size();
    }
};