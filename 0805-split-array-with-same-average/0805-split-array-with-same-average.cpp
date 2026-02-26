class Solution {
public:
    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return false;
        
        int sum = 0;
        for (int x : nums) {
            sum += x;
        }
        
        int m = n / 2;
        bool possible = false;
        
        // Quick check: is there any valid subset length 'k' where the sum would be an integer?
        for (int k = 1; k <= m; ++k) {
            if (sum * k % n == 0) {
                possible = true;
                break;
            }
        }
        
        // If no valid length exists, it's mathematically impossible
        if (!possible) return false;
        
        // dp[k] represents a bitset of all possible sums using exactly k elements.
        // The maximum possible sum is 30 elements * 10,000 max value = 300,000.
        // Using a vector allocates this safely on the heap to avoid a stack overflow.
        vector<bitset<300005>> dp(m + 1);
        dp[0].set(0); // 0 elements perfectly sum to 0
        
        for (int x : nums) {
            // Iterate backwards to avoid reusing the same element in the current pass
            for (int k = m; k >= 1; --k) {
                dp[k] |= (dp[k - 1] << x);
            }
        }
        
        // Check if any valid target sum was successfully built
        for (int k = 1; k <= m; ++k) {
            if (sum * k % n == 0 && dp[k].test(sum * k / n)) {
                return true;
            }
        }
        
        return false;
    }
};