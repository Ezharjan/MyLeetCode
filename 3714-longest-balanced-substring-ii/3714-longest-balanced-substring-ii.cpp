#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    int longestBalanced(string s) {
        int ans = 0;
        int n = s.length();
        if (n == 0) return 0;
        
        // Case 1: Exactly 1 distinct character
        int current_len = 1;
        for (int i = 1; i < n; i++) {
            if (s[i] == s[i-1]) {
                current_len++;
            } else {
                ans = max(ans, current_len);
                current_len = 1;
            }
        }
        ans = max(ans, current_len);
        
        // Case 2: Exactly 2 distinct characters
        auto get_max_2_chars = [&](char c1, char c2) {
            // first_occ tracks the first time we see a particular prefix sum. 
            // Shifted by +n to handle negative sum values since they range between [-n, n].
            vector<int> first_occ(2 * n + 1, -2);
            vector<int> modified;
            
            int max_len = 0;
            int sum = 0;
            int start_idx = 0;
            
            first_occ[n] = start_idx - 1;
            modified.push_back(n);
            
            for (int i = 0; i < n; i++) {
                if (s[i] == c1) {
                    sum += 1;
                } else if (s[i] == c2) {
                    sum -= 1;
                } else {
                    // Encountered the 3rd character. Reset the segment tracking!
                    for (int val : modified) {
                        first_occ[val] = -2;
                    }
                    modified.clear();
                    sum = 0;
                    start_idx = i + 1;
                    first_occ[n] = start_idx - 1;
                    modified.push_back(n);
                    continue;
                }
                
                if (first_occ[sum + n] != -2) {
                    max_len = max(max_len, i - first_occ[sum + n]);
                } else {
                    first_occ[sum + n] = i;
                    modified.push_back(sum + n);
                }
            }
            return max_len;
        };
        
        ans = max(ans, get_max_2_chars('a', 'b'));
        ans = max(ans, get_max_2_chars('b', 'c'));
        ans = max(ans, get_max_2_chars('a', 'c'));
        
        // Case 3: Exactly 3 distinct characters
        vector<pair<long long, int>> states;
        states.reserve(n + 1);
        
        int count_a = 0, count_b = 0, count_c = 0;
        
        // Create an un-collidable unique mapping for differences, shifted by n
        long long initial_key = ((long long)n) * (2LL * n + 1) + n;
        states.push_back({initial_key, -1});
        
        for (int i = 0; i < n; i++) {
            if (s[i] == 'a') count_a++;
            else if (s[i] == 'b') count_b++;
            else if (s[i] == 'c') count_c++;
            
            int diff1 = count_a - count_b;
            int diff2 = count_b - count_c;
            
            long long key = ((long long)(diff1 + n)) * (2LL * n + 1) + (diff2 + n);
            states.push_back({key, i});
        }
        
        // Sorting automatically groups matching keys together and their trailing indexes into ascending order 
        sort(states.begin(), states.end());
        
        int n_states = states.size();
        for (int i = 0; i < n_states; ) {
            int j = i;
            while (j < n_states && states[j].first == states[i].first) {
                j++;
            }
            // Length maxes at index furthest right (j-1) - index furthest left (i) for the same key state
            ans = max(ans, states[j-1].second - states[i].second);
            i = j;
        }
        
        return ans;
    }
};