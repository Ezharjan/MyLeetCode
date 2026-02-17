class Solution {
public:
    int maxIncreasingGroups(vector<int>& usageLimits) {
        // Sort the usage limits in ascending order to greedily use the most restricted items first.
        sort(usageLimits.begin(), usageLimits.end());
        
        int k = 0;              // Tracks the maximum number of groups we can currently form
        long long total = 0;    // Tracks the total sum of usage limits processed so far
        
        for (int limit : usageLimits) {
            total += limit;
            
            // Check if we have enough total elements to form `k + 1` groups.
            // The total elements needed for `k + 1` groups is the sum of the first `k + 1` integers.
            if (total >= 1LL * (k + 1) * (k + 2) / 2) {
                k++;
            }
        }
        
        return k;
    }
};