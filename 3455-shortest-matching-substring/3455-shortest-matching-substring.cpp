#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
    // KMP algorithm to find all starting indices of pattern `p` in string `s`
    vector<int> get_matches(const string& s, const string& p) {
        // An empty pattern conceptually matches at every position including the end
        if (p.empty()) {
            vector<int> res(s.length() + 1);
            iota(res.begin(), res.end(), 0);
            return res;
        }
        
        vector<int> pi(p.length());
        for (int i = 1, j = 0; i < p.length(); i++) {
            while (j > 0 && p[i] != p[j]) j = pi[j - 1];
            if (p[i] == p[j]) j++;
            pi[i] = j;
        }
        
        vector<int> res;
        for (int i = 0, j = 0; i < s.length(); i++) {
            while (j > 0 && s[i] != p[j]) j = pi[j - 1];
            if (s[i] == p[j]) j++;
            if (j == p.length()) {
                res.push_back(i - j + 1);
                j = pi[j - 1];
            }
        }
        return res;
    }

public:
    int shortestMatchingSubstring(string s, string p) {
        // Step 1: Split pattern around the exactly two '*' characters
        int first_star = p.find('*');
        int second_star = p.find('*', first_star + 1);
        
        string P1 = p.substr(0, first_star);
        string P2 = p.substr(first_star + 1, second_star - first_star - 1);
        string P3 = p.substr(second_star + 1);
        
        // Step 2: Get all matches for each sub-pattern
        vector<int> A = get_matches(s, P1);
        vector<int> B = get_matches(s, P2);
        vector<int> C = get_matches(s, P3);
        
        int ans = 1e9;
        int a_ptr = 0;
        int max_a = -1;
        int c_ptr = 0;
        
        // Step 3: Fast Two-Pointer lookup to minimize the matched bounds
        for (int b : B) {
            // Find the maximum valid start index for P1 that ends before/at b
            while (a_ptr < A.size() && A[a_ptr] <= b - (int)P1.length()) {
                max_a = A[a_ptr];
                a_ptr++;
            }
            
            // Find the minimum valid start index for P3 that begins after/at the end of P2
            while (c_ptr < C.size() && C[c_ptr] < b + (int)P2.length()) {
                c_ptr++;
            }
            
            // If both P1 and P3 can be legally placed relative to P2, process sequence length
            if (max_a != -1 && c_ptr < C.size()) {
                ans = min(ans, C[c_ptr] + (int)P3.length() - max_a);
            }
        }
        
        return ans == 1e9 ? -1 : ans;
    }
};