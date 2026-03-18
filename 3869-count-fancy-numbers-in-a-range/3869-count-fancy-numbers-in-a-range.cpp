class Solution {
public:
    long long countFancy(long long l, long long r) {
        // Step 1: Precompute all "good" numbers.
        // There are at most 2^9 + 2^10 such numbers.
        vector<long long> good_nums;
        
        // Generate strictly increasing numbers
        for (int mask = 1; mask < (1 << 9); ++mask) {
            long long num = 0;
            for (int i = 1; i <= 9; ++i) {
                if ((mask >> (i - 1)) & 1) {
                    num = num * 10 + i;
                }
            }
            good_nums.push_back(num);
        }
        
        // Generate strictly decreasing numbers
        for (int mask = 1; mask < (1 << 10); ++mask) {
            long long num = 0;
            for (int i = 9; i >= 0; --i) {
                if ((mask >> i) & 1) {
                    num = num * 10 + i;
                }
            }
            if (num > 0) { // Ignore '0' as l >= 1
                good_nums.push_back(num);
            }
        }
        
        // Sort and remove duplicates (single digits are both inc/dec)
        sort(good_nums.begin(), good_nums.end());
        good_nums.erase(unique(good_nums.begin(), good_nums.end()), good_nums.end());

        // Step 2: Precompute all valid "good" sums (max possible sum <= 135)
        vector<bool> is_good_sum(150, false);
        for (int i = 0; i < 150; ++i) {
            if (i < 10) {
                is_good_sum[i] = true;
            } else {
                string s = to_string(i);
                bool inc = true, dec = true;
                for (size_t j = 1; j < s.length(); ++j) {
                    if (s[j] <= s[j - 1]) inc = false;
                    if (s[j] >= s[j - 1]) dec = false;
                }
                is_good_sum[i] = inc || dec;
            }
        }

        // Helper lambda to find total fancy numbers in [0, X]
        auto solve = [&](long long X) -> long long {
            if (X < 0) return 0;
            
            string S = to_string(X);
            int n = S.length();
            
            // DP Table: memo[idx][sum][is_less]
            long long memo[18][150][2];
            memset(memo, -1, sizeof(memo));

            // Digit DP definition
            auto dfs = [&](auto& self, int idx, int sum, int is_less) -> long long {
                if (idx == n) {
                    return is_good_sum[sum] ? 1 : 0;
                }
                if (memo[idx][sum][is_less] != -1) {
                    return memo[idx][sum][is_less];
                }
                
                int limit = is_less ? 9 : S[idx] - '0';
                long long ans = 0;
                
                for (int d = 0; d <= limit; ++d) {
                    ans += self(self, idx + 1, sum + d, is_less | (d < limit));
                }
                return memo[idx][sum][is_less] = ans;
            };

            // Count from DP: Numbers with a 'good' sum of digits
            long long count = dfs(dfs, 0, 0, 0);

            // Add numbers that are 'good' themselves but their digit sum is NOT good
            for (long long g : good_nums) {
                if (g <= X) {
                    int s = 0;
                    long long temp = g;
                    while (temp > 0) {
                        s += temp % 10;
                        temp /= 10;
                    }
                    if (!is_good_sum[s]) {
                        count++;
                    }
                } else {
                    break; // Elements are sorted, safely terminate early
                }
            }
            
            return count;
        };

        // Return bounded difference
        return solve(r) - solve(l - 1);
    }
};