#include <vector>
#include <string>
#include <unordered_map>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        // If there are 2 or fewer points, they all reside on the same line.
        if (n <= 2) return n;

        int max_points = 1;

        for (int i = 0; i < n; i++) {
            unordered_map<string, int> slope_counts;
            int local_max = 0;
            
            for (int j = i + 1; j < n; j++) {
                int dx = points[j][0] - points[i][0];
                int dy = points[j][1] - points[i][1];

                // Reduce the fraction by dividing by the Greatest Common Divisor
                int g = std::gcd(dx, dy);
                dx /= g;
                dy /= g;

                // Standardize the sign so that equivalent slopes hash to the same key.
                // We force dx to be positive. If dx is 0, we force dy to be positive.
                if (dx < 0 || (dx == 0 && dy < 0)) {
                    dx = -dx;
                    dy = -dy;
                }

                // Create a string key for our hash map
                string slope = to_string(dx) + "_" + to_string(dy);
                slope_counts[slope]++;
                
                // Keep track of the maximum points on a line going through point i
                local_max = max(local_max, slope_counts[slope]);
            }
            // Add 1 to include the current point 'i' itself
            max_points = max(max_points, local_max + 1);
        }

        return max_points;
    }
};