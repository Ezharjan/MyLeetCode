#include <vector>
#include <queue>
#include <numeric>

using namespace std;

class Solution {
public:
    int earliestSecondToMarkIndices(vector<int>& nums, vector<int>& changeIndices) {
        int n = nums.size();
        int m = changeIndices.size();
        
        // Find the global first occurrence of each index.
        vector<int> first_occ(n + 1, -1);
        for (int i = 1; i <= m; ++i) {
            if (first_occ[changeIndices[i - 1]] == -1) {
                first_occ[changeIndices[i - 1]] = i;
            }
        }
        
        long long sum_all = 0;
        for (int x : nums) {
            sum_all += x;
        }
        
        // Helper to check if T is a sufficient bound
        auto isValid = [&](int T) {
            int marks = 0;
            priority_queue<int, vector<int>, greater<int>> pq; // Min-Heap
            
            // Greedily process from right to left
            for (int t = T; t >= 1; --t) {
                int idx = changeIndices[t - 1];
                
                if (first_occ[idx] == t && nums[idx - 1] > 0) {
                    int v = nums[idx - 1];
                    if (marks > 0) {
                        marks--;
                        pq.push(v);
                    } else {
                        // Recover a mark and enhance savings if advantageous 
                        if (!pq.empty() && v > pq.top()) {
                            pq.pop();
                            pq.push(v);
                        }
                        marks++; 
                    }
                } else {
                    marks++;
                }
            }
            
            long long saved = 0;
            while (!pq.empty()) {
                saved += pq.top() - 1;
                pq.pop();
            }
            
            // Validate: Base cost if we decremented normally - Operations eliminated bounds within T limit.
            return sum_all + n - saved <= T;
        };
        
        // Binary search the optimal earliest second
        int low = 1, high = m;
        int ans = -1;
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (isValid(mid)) {
                ans = mid;
                high = mid - 1; // Attempt to find an even earlier valid second
            } else {
                low = mid + 1;
            }
        }
        
        return ans;
    }
};