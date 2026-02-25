#include <string>
#include <vector>
#include <cstring>

using namespace std;

class Solution {
    // dp_cnt stores the count of valid suffixes
    long long dp_cnt[20][2][2][11][11];
    // dp_wav stores the accumulated waviness from the suffixes
    long long dp_wav[20][2][2][11][11];

    pair<long long, long long> dfs(int idx, bool is_less, bool is_started, int d1, int d2, const string& S) {
        // Base case: successfully formed a number
        if (idx == S.length()) {
            return {1, 0};
        }
        
        // Return memoized results if already computed
        if (dp_cnt[idx][is_less][is_started][d1][d2] != -1) {
            return {dp_cnt[idx][is_less][is_started][d1][d2], dp_wav[idx][is_less][is_started][d1][d2]};
        }
        
        int limit = is_less ? 9 : (S[idx] - '0');
        long long total_cnt = 0;
        long long total_wav = 0;
        
        for (int curr = 0; curr <= limit; ++curr) {
            bool nxt_less = is_less || (curr < limit);
            bool nxt_started = is_started || (curr > 0);
            int nxt_d1 = 10, nxt_d2 = 10;
            
            long long wav_add = 0;
            
            // Handle placement depending on if we are bypassing leading zeros
            if (!is_started) {
                if (curr > 0) {
                    nxt_d1 = 10;
                    nxt_d2 = curr;
                } else {
                    nxt_d1 = 10;
                    nxt_d2 = 10;
                }
            } else {
                nxt_d1 = d2;
                nxt_d2 = curr;
                
                // If we have at least 3 digits placed, check for peak or valley
                if (d1 != 10) {
                    if ((d1 < d2 && d2 > curr) || (d1 > d2 && d2 < curr)) {
                        wav_add = 1;
                    }
                }
            }
            
            auto res = dfs(idx + 1, nxt_less, nxt_started, nxt_d1, nxt_d2, S);
            total_cnt += res.first;
            // The total waviness is the sum of the suffix waviness plus 
            // the waviness contributed by the current peak/valley forming across all valid suffixes
            total_wav += res.second + res.first * wav_add;
        }
        
        dp_cnt[idx][is_less][is_started][d1][d2] = total_cnt;
        dp_wav[idx][is_less][is_started][d1][d2] = total_wav;
        
        return {total_cnt, total_wav};
    }

    long long solve(long long N) {
        if (N <= 0) return 0;
        string S = to_string(N);
        memset(dp_cnt, -1, sizeof(dp_cnt));
        memset(dp_wav, -1, sizeof(dp_wav));
        
        // Start state: index 0, not less, not started, empty past digits (10)
        return dfs(0, false, false, 10, 10, S).second;
    }

public:
    long long totalWaviness(long long num1, long long num2) {
        return solve(num2) - solve(num1 - 1);
    }
};