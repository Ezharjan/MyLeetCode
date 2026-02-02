#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
    long long MOD = 1e9 + 7;

    // Helper function for Modular Exponentiation: (base^exp) % MOD
    long long fastPow(long long base, long long exp) {
        long long res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }

public:
    vector<int> getFinalState(vector<int>& nums, int k, int multiplier) {
        // Edge Case: If multiplier is 1, the values never change.
        if (multiplier == 1) return nums;

        int n = nums.size();
        
        // Min-Heap storing pairs of {value, original_index}.
        // The default pair comparison handles the tie-breaking logic automatically:
        // (val1 == val2) -> compares indices, ensuring the smaller index is picked first.
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        
        long long current_max = 0;
        for (int i = 0; i < n; ++i) {
            pq.push({(long long)nums[i], i});
            current_max = max(current_max, (long long)nums[i]);
        }

        // Phase 1: Simulation
        // Run simulation until the array stabilizes or k is exhausted.
        // Stability is reached when the smallest element * multiplier > current_max.
        // This implies the next operation would make the current min the new max, starting a strict cycle.
        while (k > 0) {
            pair<long long, int> top = pq.top();
            
            // Optimization Check:
            // Use division to prevent long long overflow: top.first * multiplier > current_max
            if (top.first > current_max / multiplier) { 
                break; 
            }
            
            pq.pop();
            
            // Perform the multiplication
            top.first *= multiplier;
            
            // Update heap and max tracker
            pq.push(top);
            current_max = max(current_max, top.first);
            k--;
        }

        // Phase 2: Bulk Mathematical Application
        // If k > 0, the remaining operations are distributed in a round-robin fashion.
        long long cycles = k / n;
        int remaining = k % n;
        
        // Calculate multipliers
        long long pow_cycles = fastPow(multiplier, cycles);
        long long pow_cycles_plus_1 = (pow_cycles * multiplier) % MOD;

        vector<int> result(n);
        
        // The heap currently holds elements in the exact order they would be picked next.
        // 1. The first 'remaining' elements get (cycles + 1) multiplications.
        // 2. The other elements get (cycles) multiplications.
        while (!pq.empty()) {
            pair<long long, int> top = pq.top();
            pq.pop();
            
            long long val = top.first % MOD; // Apply modulo to the base value
            int idx = top.second;
            
            if (remaining > 0) {
                val = (val * pow_cycles_plus_1) % MOD;
                remaining--;
            } else {
                val = (val * pow_cycles) % MOD;
            }
            
            result[idx] = (int)val;
        }
        
        return result;
    }
};