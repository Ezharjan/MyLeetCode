#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> gcdValues(vector<int>& nums, vector<long long>& queries) {
        // Find the maximum element in nums to define our frequency limits
        int M = 0;
        for (int x : nums) {
            if (x > M) M = x;
        }

        // Count the occurrences of each number in nums
        vector<int> freq(M + 1, 0);
        for (int x : nums) {
            freq[x]++;
        }

        // cnt[i] will eventually store the exact number of pairs with GCD == i
        vector<long long> cnt(M + 1, 0);
        
        // Step 1: Count pairs where the GCD is a MULTIPLE of i
        for (int i = 1; i <= M; ++i) {
            long long c = 0;
            // Sum the frequencies of all multiples of i
            for (int j = i; j <= M; j += i) {
                c += freq[j];
            }
            // Number of pairs formed by these multiples
            cnt[i] = c * (c - 1) / 2;
        }

        // Step 2: Compute exact GCD counts using the inclusion-exclusion principle
        for (int i = M; i >= 1; --i) {
            for (int j = 2 * i; j <= M; j += i) {
                cnt[i] -= cnt[j];
            }
        }

        // Step 3: Compute prefix sums of the GCD frequencies
        for (int i = 1; i <= M; ++i) {
            cnt[i] += cnt[i - 1];
        }

        // Step 4: Answer each query using binary search
        vector<int> ans(queries.size());
        for (size_t i = 0; i < queries.size(); ++i) {
            // Find the first GCD whose cumulative frequency is strictly greater than queries[i]
            auto it = upper_bound(cnt.begin() + 1, cnt.end(), queries[i]);
            ans[i] = distance(cnt.begin(), it);
        }

        return ans;
    }
};