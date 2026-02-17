class Solution {
    long long C[65][65];
    int depth[65];

public:
    Solution() {
        // Precompute combinations (Pascal's Triangle)
        for (int i = 0; i <= 60; ++i) {
            C[i][0] = 1;
            for (int j = 1; j <= i; ++j) {
                C[i][j] = C[i-1][j-1] + C[i-1][j];
            }
            for (int j = i + 1; j <= 60; ++j) {
                C[i][j] = 0;
            }
        }
        
        // Precompute depths for all possible popcounts up to 60
        depth[1] = 0;
        for (int i = 2; i <= 60; ++i) {
            depth[i] = 1 + depth[__builtin_popcount(i)];
        }
    }

    // Function to calculate how many numbers in [1, n] have exactly 'c' set bits
    long long count_with_popcount(long long n, int c) {
        if (c == 0) return 0; // x=0 is implicitly out of range [1, n]
        
        long long ans = 0;
        int current_set_bits = 0;
        
        for (int i = 60; i >= 0; --i) {
            if ((n >> i) & 1) {
                int needed = c - current_set_bits;
                if (needed >= 0 && needed <= i) {
                    ans += C[i][needed];
                }
                current_set_bits++;
            }
        }
        // Account for 'n' itself if it matches perfectly
        if (current_set_bits == c) {
            ans++;
        }
        
        return ans;
    }

    long long popcountDepth(long long n, int k) {
        // Special case: Only the number 1 has a depth of 0.
        if (k == 0) return 1;
        
        long long total_valid_x = 0;
        
        for (int c = 1; c <= 60; ++c) {
            if (depth[c] == k - 1) {
                total_valid_x += count_with_popcount(n, c);
            }
        }
        
        // x=1 has a popcount of 1 but a depth of 0, whereas normal sequence assumes 1. 
        // Thus, x=1 illegally gets counted when resolving depth 1, so we filter it out.
        if (k == 1) {
            total_valid_x -= 1;
        }
        
        return total_valid_x;
    }
};