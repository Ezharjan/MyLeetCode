#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maximumBeauty(vector<int>& flowers, long long newFlowers, int target, int full, int partial) {
        int n = flowers.size();
        
        // Cap flowers at target since exceeding target yields no extra beauty
        for (int& f : flowers) {
            f = min(f, target);
        }
        
        // Sort to efficiently manage the smallest (incomplete) and largest (complete) gardens
        sort(flowers.begin(), flowers.end());

        // Edge case: All gardens are already complete
        if (flowers[0] == target) {
            return (long long)n * full;
        }

        // Prefix sum for O(1) cost calculations
        vector<long long> sum(n + 1, 0);
        for (int i = 0; i < n; i++) {
            sum[i + 1] = sum[i] + flowers[i];
        }

        // Find the index of the last incomplete garden
        int p = n - 1;
        while (p >= 0 && flowers[p] == target) {
            p--;
        }

        long long ans = 0;
        long long rem = newFlowers;

        // i represents the index of the last garden we leave incomplete
        for (int i = p; i >= -1; i--) {
            // If we've shrunk the incomplete window, we must spend flowers to complete the (i + 1)-th garden
            if (i < p) {
                rem -= (target - flowers[i + 1]);
            }
            
            // If we ran out of flowers, we can't complete this many gardens. Break early.
            if (rem < 0) {
                break;
            }

            // If we decided to make ALL gardens complete (i == -1)
            if (i == -1) {
                ans = max(ans, (long long)n * full);
                continue;
            }

            // Binary search to find the maximum index 'j' such that we can level 
            // gardens 0...j to the height of flowers[j]
            int left = 0, right = i;
            int best_j = 0;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                long long cost = (long long)flowers[mid] * (mid + 1) - sum[mid + 1];
                if (cost <= rem) {
                    best_j = mid;
                    left = mid + 1; // Try to include more gardens
                } else {
                    right = mid - 1;
                }
            }

            // Calculate the exact minimum height we can achieve for gardens 0...i
            long long cost = (long long)flowers[best_j] * (best_j + 1) - sum[best_j + 1];
            long long extra = rem - cost;
            long long min_height = flowers[best_j] + extra / (best_j + 1);
            
            // The height of an incomplete garden cannot exceed target - 1
            min_height = min(min_height, (long long)target - 1);

            // Calculate the total beauty for this configuration
            long long current_beauty = (long long)(n - 1 - i) * full + min_height * partial;
            ans = max(ans, current_beauty);
        }

        return ans;
    }
};