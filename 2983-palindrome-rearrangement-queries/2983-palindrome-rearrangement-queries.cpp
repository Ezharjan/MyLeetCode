#include <vector>
#include <string>
#include <algorithm>
#include <array>

using namespace std;

class Solution {
public:
    vector<bool> canMakePalindromeQueries(string s, vector<vector<int>>& queries) {
        int n = s.length();
        int m = n / 2;
        
        string s1 = s.substr(0, m);
        string s2 = s.substr(m, m);
        reverse(s2.begin(), s2.end());
        
        // Prefix sum of mismatches
        vector<int> pref_diff(m + 1, 0);
        // Prefix sums of character counts
        vector<array<int, 26>> pref1(m + 1);
        vector<array<int, 26>> pref2(m + 1);
        
        for (int j = 0; j < 26; ++j) {
            pref1[0][j] = 0;
            pref2[0][j] = 0;
        }
        
        for (int i = 0; i < m; ++i) {
            pref_diff[i + 1] = pref_diff[i] + (s1[i] != s2[i] ? 1 : 0);
            pref1[i + 1] = pref1[i];
            pref2[i + 1] = pref2[i];
            pref1[i + 1][s1[i] - 'a']++;
            pref2[i + 1][s2[i] - 'a']++;
        }
        
        // Helper to get number of mismatches in range [L, R]
        auto mismatches = [&](int L, int R) {
            if (L > R) return 0;
            return pref_diff[R + 1] - pref_diff[L];
        };
        
        // Helper to get character counts in range [L, R]
        auto get_cnt = [](int L, int R, const vector<array<int, 26>>& pref) {
            array<int, 26> res{};
            if (L > R) return res;
            for (int i = 0; i < 26; ++i) {
                res[i] = pref[R + 1][i] - pref[L][i];
            }
            return res;
        };
        
        // Checks if all character counts in 'sub' are <= 'sup'
        auto sub_cnt = [](const array<int, 26>& sub, const array<int, 26>& sup) {
            for (int i = 0; i < 26; ++i) {
                if (sub[i] > sup[i]) return false;
            }
            return true;
        };
        
        // Checks if character counts are strictly equal
        auto eq_cnt = [](const array<int, 26>& c1, const array<int, 26>& c2) {
            for (int i = 0; i < 26; ++i) {
                if (c1[i] != c2[i]) return false;
            }
            return true;
        };
        
        // Main solver for an adjusted mapped query
        auto solve = [&](int a1, int b1, int a2, int b2, 
                         const vector<array<int, 26>>& p1, 
                         const vector<array<int, 26>>& p2) {
            
            if (b1 < a2) { 
                // Case 1: Intervals are entirely disjoint
                if (mismatches(0, a1 - 1) > 0) return false;
                if (mismatches(b1 + 1, a2 - 1) > 0) return false;
                if (mismatches(b2 + 1, m - 1) > 0) return false;
                
                array<int, 26> c1_1 = get_cnt(a1, b1, p1);
                array<int, 26> c2_1 = get_cnt(a1, b1, p2);
                if (!eq_cnt(c1_1, c2_1)) return false;
                
                array<int, 26> c1_2 = get_cnt(a2, b2, p1);
                array<int, 26> c2_2 = get_cnt(a2, b2, p2);
                if (!eq_cnt(c1_2, c2_2)) return false;
                
                return true;
            } else { 
                // Case 2: Intervals overlap or are nestled within each other
                int R = max(b1, b2);
                
                if (mismatches(0, a1 - 1) > 0) return false;
                if (mismatches(R + 1, m - 1) > 0) return false;
                
                array<int, 26> c1_all = get_cnt(a1, R, p1);
                array<int, 26> c2_all = get_cnt(a1, R, p2);
                if (!eq_cnt(c1_all, c2_all)) return false;
                
                array<int, 26> req2_left = get_cnt(a1, a2 - 1, p2);
                array<int, 26> avail1 = get_cnt(a1, b1, p1);
                if (!sub_cnt(req2_left, avail1)) return false;
                
                if (b1 < b2) {
                    array<int, 26> req1_right = get_cnt(b1 + 1, b2, p1);
                    array<int, 26> avail2 = get_cnt(a2, b2, p2);
                    if (!sub_cnt(req1_right, avail2)) return false;
                }
                
                return true;
            }
        };
        
        vector<bool> ans;
        ans.reserve(queries.size());
        
        for (const auto& q : queries) {
            int a1 = q[0], b1 = q[1];
            // Mirrored mapping for the right half intervals onto s2
            int a2 = n - 1 - q[3];
            int b2 = n - 1 - q[2];
            
            // We consistently ensure interval 1 comes before interval 2 for easier tracking
            if (a1 <= a2) {
                ans.push_back(solve(a1, b1, a2, b2, pref1, pref2));
            } else {
                ans.push_back(solve(a2, b2, a1, b1, pref2, pref1));
            }
        }
        
        return ans;
    }
};