class Solution {
public:
    int minNumberOfSemesters(int n, vector<vector<int>>& relations, int k) {
        // prereq[i] will store the bitmask of prerequisites for course i
        vector<int> prereq(n, 0);
        for (const auto& r : relations) {
            // Convert 1-based indexing to 0-based indexing
            prereq[r[1] - 1] |= (1 << (r[0] - 1));
        }

        int num_states = 1 << n;
        // dp[mask] represents the min semesters to complete the subset of courses in mask
        vector<int> dp(num_states, n + 1);
        dp[0] = 0;

        for (int mask = 0; mask < num_states; ++mask) {
            // Skip unreachable states
            if (dp[mask] == n + 1) continue;

            int available = 0;
            // Find all courses that can be taken in the next semester
            for (int i = 0; i < n; ++i) {
                // If course i is not in mask AND all its prerequisites are in mask
                if (!(mask & (1 << i)) && (mask & prereq[i]) == prereq[i]) {
                    available |= (1 << i);
                }
            }

            // Count how many courses are available
            int c = __builtin_popcount(available);
            
            if (c <= k) {
                // If we can take all available courses, take them all
                dp[mask | available] = min(dp[mask | available], dp[mask] + 1);
            } else {
                // Otherwise, iterate through all submasks of the available courses
                // and consider the subsets where exactly k courses are chosen
                for (int sub = available; sub > 0; sub = (sub - 1) & available) {
                    if (__builtin_popcount(sub) == k) {
                        dp[mask | sub] = min(dp[mask | sub], dp[mask] + 1);
                    }
                }
            }
        }

        // The answer is the minimum semesters to reach the state where all n courses are taken
        return dp[num_states - 1];
    }
};