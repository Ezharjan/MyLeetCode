#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

class Solution {
public:
    int minDifference(vector<int>& nums) {
        int n = nums.size();
        int max_fixed_diff = 0;
        
        // Requirements for the two modes
        // split_reqs: Used for Mode 1 (connected x, y). treats u...v with multi -1s as separate constraints u and v
        // joined_reqs: Used for Mode 2 (independent x, y). treats u...v with multi -1s as a single constraint {u, v}
        vector<pair<int, int>> split_reqs;
        vector<pair<int, int>> joined_reqs;
        
        for (int i = 0; i < n; ++i) {
            if (nums[i] != -1) {
                if (i + 1 < n && nums[i+1] != -1) {
                    max_fixed_diff = max(max_fixed_diff, abs(nums[i] - nums[i+1]));
                }
                continue;
            }
            
            // Found a segment of -1s starting at i
            int j = i;
            while (j < n && nums[j] == -1) {
                j++;
            }
            // Segment is from i to j-1
            
            int l_val = (i > 0) ? nums[i-1] : -1;
            int r_val = (j < n) ? nums[j] : -1;
            
            if (l_val != -1 && r_val != -1) {
                // Segment bounded by two numbers
                if (j - i == 1) { // Single -1: u, -1, v
                    int u = l_val, v = r_val;
                    split_reqs.push_back({min(u, v), max(u, v)});
                    joined_reqs.push_back({min(u, v), max(u, v)});
                } else { // Multiple -1s: u, -1, -1..., v
                    split_reqs.push_back({l_val, l_val});
                    split_reqs.push_back({r_val, r_val});
                    joined_reqs.push_back({min(l_val, r_val), max(l_val, r_val)});
                }
            } else if (l_val != -1) { // Tail: u, -1...
                split_reqs.push_back({l_val, l_val});
                joined_reqs.push_back({l_val, l_val});
            } else if (r_val != -1) { // Head: -1..., v
                split_reqs.push_back({r_val, r_val});
                joined_reqs.push_back({r_val, r_val});
            }
            // Note: If array is all -1s, reqs will be empty, correct.
            
            i = j - 1; // Advance loop
        }

        // Helper to solve the "Cover intervals with 2 intervals of len 2k" problem
        // If check_dist is true, we enforce |center_x - center_y| <= k
        auto can_cover = [&](const vector<pair<int, int>>& reqs, int k, bool check_dist) -> bool {
            if (reqs.empty()) return true;
            
            // Find global minimum start point to anchor the greedy strategy
            int min_s = 2e9;
            for (const auto& p : reqs) min_s = min(min_s, p.first);
            
            // Greedy: Everything that CAN fit in an interval starting at min_s goes to A.
            // Why? This minimizes the "spread" needed for B.
            // Interval A range: [min_s, min_s + 2k]
            long long limit_a = (long long)min_s + 2LL * k;
            
            int min_a = 2e9, max_a = -2e9;
            int min_b = 2e9, max_b = -2e9;
            bool b_empty = true;
            
            for (const auto& p : reqs) {
                if (p.second <= limit_a) {
                    min_a = min(min_a, p.first);
                    max_a = max(max_a, p.second);
                } else {
                    min_b = min(min_b, p.first);
                    max_b = max(max_b, p.second);
                    b_empty = false;
                }
            }
            
            // Check if B is valid
            if (!b_empty && ((long long)max_b - min_b > 2LL * k)) return false;
            
            if (!check_dist) return true; // Mode 2 satisfied
            
            // Mode 1: Check distance between valid centers
            if (b_empty) return true; // Can place y = x
            
            // Valid centers for A are in [max_a - k, min_a + k]
            // Valid centers for B are in [max_b - k, min_b + k]
            // We need these two intervals to be within distance k of each other.
            
            long long ca_min = (long long)max_a - k;
            long long ca_max = (long long)min_a + k;
            long long cb_min = (long long)max_b - k;
            long long cb_max = (long long)min_b + k;
            
            // Calculate distance between intervals [ca_min, ca_max] and [cb_min, cb_max]
            long long dist = 0;
            if (ca_max < cb_min) dist = cb_min - ca_max;
            else if (cb_max < ca_min) dist = ca_min - cb_max;
            
            return dist <= k;
        };

        int low = max_fixed_diff, high = 1000000000; // 1e9
        int ans = high;
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            
            // Check if either mode works
            if (can_cover(joined_reqs, mid, false) || can_cover(split_reqs, mid, true)) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        return ans;
    }
};