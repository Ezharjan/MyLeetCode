#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findMaximumLength(vector<int>& nums) {
        int n = nums.size();
        
        // Prefix sums array. pre[i] stores sum of nums[0...i-1].
        // Use long long to prevent overflow since sums can exceed 2^31 - 1.
        vector<long long> pre(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pre[i + 1] = pre[i] + nums[i];
        }

        // dp[i] = max length of non-decreasing array ending at index i (1-based)
        // last[i] = sum of the last element (subarray) in the sequence ending at i
        vector<int> dp(n + 1, 0);
        vector<long long> last(n + 1, 0);
        
        // q stores candidate indices 'j'.
        // We maintain the invariant that (pre[j] + last[j]) is strictly increasing in q.
        vector<int> q;
        q.push_back(0);

        for (int i = 1; i <= n; ++i) {
            // We need to find the largest index j < i such that:
            // last[j] <= sum(nums[j...i-1])  =>  last[j] <= pre[i] - pre[j]
            // Rewritten: (pre[j] + last[j]) <= pre[i]
            
            // Since q is sorted by (pre[j] + last[j]), we use upper_bound to find the first 
            // element strictly greater than pre[i]. The element just before it is our optimal j.
            auto it = upper_bound(q.begin(), q.end(), pre[i], [&](long long val, int index) {
                return val < (pre[index] + last[index]);
            });
            
            // 'it' points to the first invalid candidate, so prev(it) is the best valid candidate.
            int j = *prev(it);
            
            // Update DP states
            dp[i] = dp[j] + 1;
            last[i] = pre[i] - pre[j];
            
            // Prepare to add current 'i' to the queue.
            // Calculate the constraint value for index i.
            long long current_val = pre[i] + last[i];
            
            // Maintain monotonicity: Remove indices from the back if they have a 
            // larger or equal constraint value than the current i. Since i is a later index,
            // it is strictly better (or equal) if its constraint value is smaller.
            while (!q.empty() && (pre[q.back()] + last[q.back()]) >= current_val) {
                q.pop_back();
            }
            q.push_back(i);
        }
        
        return dp[n];
    }
};