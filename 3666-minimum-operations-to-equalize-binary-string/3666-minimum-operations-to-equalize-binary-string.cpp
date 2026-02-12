#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    int minOperations(string s, int k) {
        long long n = s.length();
        long long zeros = 0;
        for (char c : s) {
            if (c == '0') zeros++;
        }
        
        if (zeros == 0) return 0;
        
        // Impossible Parity Case:
        // If k is even, each operation flips an even number of bits. 
        // The parity of zeros never changes. If we start with odd zeros, we can never reach 0.
        if (k % 2 == 0 && zeros % 2 != 0) {
            return -1;
        }
        
        // Edge Case: k == n
        // Every operation flips the entire string.
        // We can only succeed if the string is already all 1s (0 ops) or all 0s (1 op).
        if (k == n) {
            if (zeros == n) return 1;
            return -1;
        }
        
        // Search for the minimum operations
        // Lower bound: We need at least enough total flips to cover all zeros.
        long long start_ops = (zeros + k - 1) / k; 
        if (start_ops < 1) start_ops = 1;

        // Iterate ops. In practice, the answer is usually close to n/k or n/(n-k).
        // A loop limit of 2*n is more than sufficient for convergence.
        for (long long ops = start_ops; ops <= 2 * n + 5; ++ops) {
            // Condition 1: Total Sum sufficiency (covered by start_ops, but good to keep)
            if (ops * k < zeros) continue;
            
            // Condition 2: Parity Consistency
            // The total capacity (ops*k) and the requirement (zeros) must have the same parity.
            if ((ops * k - zeros) % 2 != 0) continue;
            
            // Condition 3: Distribution / Max Constraint
            // We must ensure that we don't violate the constraint that a single index
            // can be flipped at most 'ops' times.
            if (ops % 2 == 0) {
                // If ops is even:
                // Zeros need odd flips. Max possible flips for a zero index is (ops - 1).
                // Each zero implies a "waste" of at least 1 flip capacity relative to 'ops'.
                // Total waste (zeros) must be <= Total Slack (ops * n - ops * k).
                if (zeros <= ops * (n - k)) return (int)ops;
            } else {
                // If ops is odd:
                // Ones need even flips. Max possible flips for a one index is (ops - 1).
                // Each one implies a "waste" of at least 1 flip capacity relative to 'ops'.
                // Total waste (ones) must be <= Total Slack (ops * n - ops * k).
                if ((n - zeros) <= ops * (n - k)) return (int)ops;
            }
        }
        
        return -1;
    }
};