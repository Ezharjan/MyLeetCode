#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int subsequencesWithMiddleMode(vector<int>& nums) {
        int n = nums.size();
        if (n < 5) return 0;
        
        // Coordinate Compression to map elements to 0 ... U-1
        vector<int> sorted_nums = nums;
        sort(sorted_nums.begin(), sorted_nums.end());
        sorted_nums.erase(unique(sorted_nums.begin(), sorted_nums.end()), sorted_nums.end());
        int U = sorted_nums.size();
        
        vector<int> compressed_nums(n);
        for (int i = 0; i < n; ++i) {
            compressed_nums[i] = lower_bound(sorted_nums.begin(), sorted_nums.end(), nums[i]) - sorted_nums.begin();
        }
        
        vector<long long> L_count(U, 0);
        vector<long long> R_count(U, 0);
        
        // Initializing right counts with the whole array
        for (int i = 0; i < n; ++i) {
            R_count[compressed_nums[i]]++;
        }
        
        long long total_valid = 0;
        long long MOD = 1e9 + 7;
        
        // Lambda to compute 'n choose 2'
        auto C2 = [](long long k) {
            if (k < 2) return 0LL;
            return k * (k - 1) / 2;
        };
        
        // Loop over each possible middle element
        for (int i = 0; i < n; ++i) {
            int x = compressed_nums[i];
            R_count[x]--; // Remove current element from the right sequence pool
            
            // To be a valid middle element, there must be at least 2 elements on left and 2 on right
            if (i >= 2 && i <= n - 3) {
                long long Lx = L_count[x];
                long long Rx = R_count[x];
                long long L_nex = i - Lx;
                long long R_nex = (n - 1 - i) - Rx;
                
                // Total possible ways to form a 5-element sequence centered at i
                long long total_seq = (C2(i) % MOD) * (C2(n - 1 - i) % MOD) % MOD;
                
                // Invalid Type A: 'x' is chosen 0 times among the 4 boundary elements
                long long invalid_A = (C2(L_nex) % MOD) * (C2(R_nex) % MOD) % MOD;
                long long invalid_B = 0;
                
                // Invalid Type B: 'x' is chosen 1 time, and some 'y' is chosen >= 2 times
                for (int y = 0; y < U; ++y) {
                    if (y == x) continue;
                    long long Ly = L_count[y];
                    long long Ry = R_count[y];
                    
                    if (Ly == 0 && Ry == 0) continue;
                    
                    // W1: Choose 1 non-x from left, 2 non-x from right -> contains >= two y's
                    long long W1 = L_nex * C2(Ry) + Ly * Ry * (R_nex - Ry);
                    
                    // W2: Choose 2 non-x from left, 1 non-x from right -> contains >= two y's
                    long long W2 = R_nex * C2(Ly) + Ry * Ly * (L_nex - Ly);
                    
                    // Add variations (x on left and the 3 rest on boundaries OR x on right and rest on boundaries)
                    long long cur_inv = (Lx * (W1 % MOD) + Rx * (W2 % MOD)) % MOD;
                    invalid_B = (invalid_B + cur_inv) % MOD;
                }
                
                // Compute mutually exclusive valid sequences subtracting invalid constraints A & B
                long long valid = total_seq;
                valid = (valid - invalid_A + MOD) % MOD;
                valid = (valid - invalid_B + MOD) % MOD;
                
                total_valid = (total_valid + valid) % MOD;
            }
            
            L_count[x]++; // Add current element into the left sequence pool for the next iterations
        }
        
        return total_valid;
    }
};