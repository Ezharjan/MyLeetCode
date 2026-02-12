#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    string minCostGoodCaption(string caption) {
        int n = caption.length();
        if (n < 3) return "";

        // Precompute prefix costs: P[c][i] is cost to turn caption[0..i-1] into char c
        vector<vector<int>> P(26, vector<int>(n + 1, 0));
        for (int c = 0; c < 26; ++c) {
            char target = 'a' + c;
            for (int i = 0; i < n; ++i) {
                P[c][i + 1] = P[c][i] + abs(caption[i] - target);
            }
        }

        // dp[i] stores min cost for suffix starting at i
        vector<int> dp(n + 1, 2e9);
        dp[n] = 0;
        
        // Reconstruction helpers
        vector<int> op_len(n + 1, 0);
        vector<int> min_char(n + 1, 26); // Initialize with sentinel > 'z'
        vector<int> run_len(n + 1, 0);

        for (int i = n - 1; i >= 0; --i) {
            for (int c = 0; c < 26; ++c) {
                // Try valid block lengths
                for (int len = 3; len <= 5; ++len) {
                    if (i + len > n) break;
                    if (dp[i + len] == 2e9) continue;

                    int cost = (P[c][i + len] - P[c][i]) + dp[i + len];
                    
                    // Calculate potential run length of character 'c'
                    int next_idx = i + len;
                    int current_run = len;
                    if (next_idx < n && min_char[next_idx] == c) {
                        current_run += run_len[next_idx];
                    }

                    bool update = false;
                    
                    if (cost < dp[i]) {
                        update = true;
                    } else if (cost == dp[i]) {
                        // Tie-breaking for lexicographically smallest string
                        if (c < min_char[i]) {
                            update = true;
                        } else if (c == min_char[i]) {
                            // Same start char: compare based on when the run of 'c' ends
                            int best_run = run_len[i];
                            
                            if (current_run != best_run) {
                                // Find the character X that follows the shorter run
                                int shorter = min(current_run, best_run);
                                int break_point = i + shorter;
                                
                                // Identify the character that breaks the run (if not end of string)
                                int X = (break_point < n) ? min_char[break_point] : 26;
                                
                                if (current_run < best_run) {
                                    // Candidate is shorter. It switches to X. Best stays c.
                                    // Prefer candidate if X < c.
                                    if (X < c) update = true;
                                } else {
                                    // Candidate is longer. It stays c. Best switches to X.
                                    // Prefer candidate if c < X.
                                    if (c < X) update = true;
                                }
                            }
                        }
                    }

                    if (update) {
                        dp[i] = cost;
                        op_len[i] = len;
                        min_char[i] = c;
                        run_len[i] = current_run;
                    }
                }
            }
        }

        if (dp[0] == 2e9) return "";

        string result = "";
        int idx = 0;
        while (idx < n) {
            int len = op_len[idx];
            int c = min_char[idx];
            result += string(len, (char)('a' + c));
            idx += len;
        }

        return result;
    }
};