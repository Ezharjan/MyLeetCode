#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    int countPairs(vector<int>& nums) {
        int ans = 0;
        unordered_map<int, int> freq;
        
        // Powers of 10 to extract and manipulate digits up to 7 places
        int p10[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};
        
        for (int x : nums) {
            vector<int> st;
            // 1 (0-swap) + 21 (1-swap max) + 21*21 (2-swap max) = 463 max elements
            st.reserve(463); 
            st.push_back(x);
            
            for (int i = 0; i < 7; ++i) {
                for (int j = i + 1; j < 7; ++j) {
                    int d_i = (x / p10[i]) % 10;
                    int d_j = (x / p10[j]) % 10;
                    
                    if (d_i == d_j) continue; // Unnecessary redundant swap
                    
                    // Generate 1-swap
                    int nx = x - d_i * p10[i] - d_j * p10[j] + d_i * p10[j] + d_j * p10[i];
                    st.push_back(nx);
                    
                    // Using the 1-swap state, process the 2nd swap
                    for (int k = 0; k < 7; ++k) {
                        for (int l = k + 1; l < 7; ++l) {
                            int nd_k = (nx / p10[k]) % 10;
                            int nd_l = (nx / p10[l]) % 10;
                            
                            if (nd_k == nd_l) continue;
                            
                            int nnx = nx - nd_k * p10[k] - nd_l * p10[l] + nd_k * p10[l] + nd_l * p10[k];
                            st.push_back(nnx);
                        }
                    }
                }
            }
            
            // Removing duplicates to make sure previous similar numbers are only added once
            sort(st.begin(), st.end());
            st.erase(unique(st.begin(), st.end()), st.end());
            
            for (int m : st) {
                auto it = freq.find(m);
                if (it != freq.end()) {
                    ans += it->second;
                }
            }
            
            freq[x]++;
        }
        
        return ans;
    }
};