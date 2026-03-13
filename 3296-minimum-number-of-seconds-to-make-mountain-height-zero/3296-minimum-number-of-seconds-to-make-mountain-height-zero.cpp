#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long minNumberOfSeconds(int mountainHeight, vector<int>& workerTimes) {
        // The upper bound of time is if the fastest worker reduced the entire mountain themselves.
        long long min_w = *min_element(workerTimes.begin(), workerTimes.end());
        long long low = 1;
        long long high = min_w * mountainHeight * ((long long)mountainHeight + 1) / 2;
        long long ans = high;

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            
            long long total_reduced = 0;
            for (long long w : workerTimes) {
                // Calculate an initial guess for how much this worker can reduce the mountain
                long long x = (sqrt(1.0 + 8.0 * (double)mid / w) - 1.0) / 2.0;
                
                // Fine-tune our target in case of microscopic floating point imprecision
                while (x > 0 && w * x * (x + 1) / 2 > mid) {
                    x--;
                }
                while (w * (x + 1) * (x + 2) / 2 <= mid) {
                    x++;
                }
                
                total_reduced += x;
                
                // If the sum is enough to demolish the mountain, we can exit early.
                if (total_reduced >= mountainHeight) {
                    break;
                }
            }
            
            if (total_reduced >= mountainHeight) {
                ans = mid;
                high = mid - 1; // Try to find an even smaller time bound
            } else {
                low = mid + 1; // Not enough time, search higher limits
            }
        }
        
        return ans;
    }
};