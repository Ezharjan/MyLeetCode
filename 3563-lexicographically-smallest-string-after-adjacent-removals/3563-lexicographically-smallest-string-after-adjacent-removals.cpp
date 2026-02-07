#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    string lexicographicallySmallestString(string s) {
        int n = s.length();
        
        // dp[i][j] stores the lexicographically smallest string from s[i...j]
        vector<vector<string>> dp(n, vector<string>(n));
        
        // can_empty[i][j] is true if s[i...j] can be reduced to an empty string
        // This is an optimization to avoid repeatedly checking dp[i][j] == ""
        vector<vector<bool>> can_empty(n, vector<bool>(n, false));

        // Lambda to check if two characters are alphabetically consecutive (circular)
        auto are_adjacent = [](char a, char b) {
            if (abs(a - b) == 1) return true;
            if (a == 'a' && b == 'z') return true;
            if (a == 'z' && b == 'a') return true;
            return false;
        };

        // Iterate by length of the substring from 1 to n
        for (int len = 1; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                
                // Option 1: Keep s[i]. 
                // Result is s[i] + best result of the rest.
                string best_s;
                if (i + 1 <= j) {
                    best_s = s[i] + dp[i+1][j];
                } else {
                    best_s = string(1, s[i]);
                }
                
                // Option 2: Try to remove s[i] by pairing it with a valid s[k]
                // Iterating k from i+1 to j
                for (int k = i + 1; k <= j; ++k) {
                    
                    // Check if s[i] and s[k] can be paired
                    if (are_adjacent(s[i], s[k])) {
                        
                        // Check if everything between i and k can be deleted
                        bool middle_empty = false;
                        if (i + 1 > k - 1) { 
                            // No characters between them (e.g., indices 0 and 1)
                            middle_empty = true;
                        } else {
                            middle_empty = can_empty[i+1][k-1];
                        }
                        
                        if (middle_empty) {
                            // s[i] and s[k] are removed. The 'middle' is gone.
                            // We are left with the best result of the suffix after k.
                            string candidate;
                            if (k + 1 <= j) {
                                candidate = dp[k+1][j];
                            } else {
                                candidate = "";
                            }
                            
                            // Compare and pick the smallest string
                            if (candidate < best_s) {
                                best_s = candidate;
                            }
                        }
                    }
                    
                    // Optimization: If we found a way to reduce to empty string,
                    // that is the absolute best possible outcome (lexicographically).
                    if (best_s.empty()) break; 
                }
                
                dp[i][j] = best_s;
                if (best_s.empty()) {
                    can_empty[i][j] = true;
                }
            }
        }
        
        return dp[0][n-1];
    }
};