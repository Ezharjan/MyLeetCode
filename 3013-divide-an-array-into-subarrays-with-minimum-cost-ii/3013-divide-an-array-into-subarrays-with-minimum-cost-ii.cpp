#include <vector>
#include <set>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    long long minimumCost(vector<int>& nums, int k, int dist) {
        // k subarrays total means we pick index 0 + (k-1) other indices.
        // We fix 'i' as the start of the 2nd subarray.
        // We need to pick (k-2) more indices from the range [i+1, i+dist].
        
        int n = nums.size();
        long long baseCost = nums[0];
        int needed = k - 2; 
        
        // If we don't need to pick any extra numbers (k=2), 
        // we just scan for the minimum second start within valid range.
        // However, constraints say k >= 3, so needed >= 1.
        
        // L stores the 'needed' smallest elements in the current window.
        // R stores the rest of the elements in the window.
        multiset<int> L, R;
        long long sumL = 0;
        
        // Helper function to balance L and R so that L.size() == needed
        auto balance = [&]() {
            // If L has too many, move largest to R
            while (L.size() > needed) {
                int val = *L.rbegin();
                L.erase(prev(L.end()));
                sumL -= val;
                R.insert(val);
            }
            // If L has too few, move smallest from R to L
            while (L.size() < needed && !R.empty()) {
                int val = *R.begin();
                R.erase(R.begin());
                L.insert(val);
                sumL += val;
            }
        };
        
        // Helper to add a number to the window
        auto add = [&](int val) {
            L.insert(val);
            sumL += val;
            balance();
        };
        
        // Helper to remove a number from the window
        auto remove = [&](int val) {
            auto it = R.find(val);
            if (it != R.end()) {
                R.erase(it);
            } else {
                it = L.find(val);
                if (it != L.end()) {
                    sumL -= val;
                    L.erase(it);
                    balance();
                }
            }
        };

        // Initialize window for the first pivot (i = 1).
        // The candidates for the remaining k-2 selections must be in [2, 1 + dist].
        for (int j = 2; j <= 1 + dist && j < n; ++j) {
            add(nums[j]);
        }
        
        long long minTotal = -1;
        
        // Loop through every possible start index 'i' for the second subarray
        for (int i = 1; i < n; ++i) {
            // If we have enough elements in the window to pick 'needed' items
            if (L.size() == needed) {
                long long currentCost = baseCost + nums[i] + sumL;
                if (minTotal == -1 || currentCost < minTotal) {
                    minTotal = currentCost;
                }
            }
            
            // Slide the window to prepare for the next 'i' (which is i+1)
            
            // 1. Remove nums[i+1] from consideration. 
            // Why? Because in the next iteration, nums[i+1] becomes the 'start' 
            // of the 2nd subarray, so it cannot be one of the 'k-2' subsequent elements.
            if (i + 1 < n) {
                remove(nums[i+1]);
            }
            
            // 2. Add nums[i + 1 + dist] to consideration.
            // This is the new element entering the 'dist' range relative to i+1.
            if (i + 1 + dist < n) {
                add(nums[i + 1 + dist]);
            }
        }
        
        return minTotal;
    }
};