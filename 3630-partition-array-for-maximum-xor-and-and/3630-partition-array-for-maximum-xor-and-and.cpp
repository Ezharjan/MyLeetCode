#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maximizeXorAndXor(vector<int>& nums) {
        int n = nums.size();
        long long total_xor = 0;
        for (int x : nums) total_xor ^= x;

        long long max_score = 0;
        int limit = 1 << n; // 2^N

        // Basis array to store linear basis masks (max 31 bits needed for 10^9)
        int basis[32]; 

        // Iterate over all possible subsets for B
        for (int mask = 0; mask < limit; ++mask) {
            long long current_and_b = -1; // -1 represents "empty" (all 1s in bits)
            long long current_xor_b = 0;
            
            // 1. Identify B and calculate its AND and XOR stats
            for (int i = 0; i < n; ++i) {
                if ((mask >> i) & 1) { // If bit i is set, nums[i] is in B
                    int val = nums[i];
                    if (current_and_b == -1) current_and_b = val;
                    else current_and_b &= val;
                    current_xor_b ^= val;
                }
            }
            if (current_and_b == -1) current_and_b = 0; // Empty B has AND value 0

            // M is the XOR sum of all elements remaining for A and C
            // XOR(A) ^ XOR(C) = XOR(All_Remaining) = Total_XOR ^ XOR(B)
            long long M = total_xor ^ current_xor_b;
            
            // We want to maximize: M + 2 * (XOR(A) & ~M)
            // This means we want to find a subset A such that XOR(A) has 1s 
            // where M has 0s. The bits where M has 1s don't contribute to the 2*AND term.
            long long target_mask = ~M;

            // 2. Build Linear Basis for elements in A/C, masked by ~M
            // Reset basis for this iteration
            for(int k = 0; k < 32; ++k) basis[k] = 0;

            for (int i = 0; i < n; ++i) {
                if (!((mask >> i) & 1)) { // If bit i is NOT set, nums[i] is in A or C
                    // We only care about bits where target_mask is 1
                    int val = nums[i] & target_mask;
                    
                    // Standard Linear Basis Insertion
                    for (int k = 30; k >= 0; --k) {
                        if ((val >> k) & 1) {
                            if (!basis[k]) {
                                basis[k] = val;
                                break;
                            }
                            val ^= basis[k];
                        }
                    }
                }
            }

            // 3. Find max subset XOR from the basis (Greedy approach)
            long long max_val = 0;
            for (int k = 30; k >= 0; --k) {
                if (!basis[k]) continue;
                // If XORing with basis[k] increases the value, do it
                if ((max_val ^ basis[k]) > max_val) {
                    max_val ^= basis[k];
                }
            }

            // 4. Calculate total score for this partition of B
            // Score = AND(B) + (XOR(A) + XOR(C))
            // Score = AND(B) + M + 2 * (XOR(A) & ~M)
            // max_val here represents the maximized (XOR(A) & ~M)
            long long current_score = current_and_b + M + (max_val * 2);
            
            if (current_score > max_score) {
                max_score = current_score;
            }
        }
        
        return max_score;
    }
};