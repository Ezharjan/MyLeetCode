#include <string>
#include <unordered_map>

using namespace std;

class Solution {
public:
    long long beautifulSubstrings(string s, int k) {
        // Find the smallest x such that (x * x) is divisible by k
        int x = 1;
        while ((x * x) % k != 0) {
            x++;
        }
        
        // The length of any valid substring must be a multiple of 2x
        int mod_val = 2 * x;
        int n = s.length();
        
        // To avoid overhead, we encode the prefix sum and mod state into a single integer key.
        // We shift the prefix sum by +n to avoid negative numbers.
        unordered_map<int, int> counts;
        counts.reserve(n + 1);
        
        // Base case: Prefix sum of 0 (shifted by n) at index 0 (mod 0)
        counts[(n << 12) | 0] = 1; 
        
        long long ans = 0;
        int P = 0; // Running prefix sum
        
        for (int i = 0; i < n; i++) {
            char c = s[i];
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                P++;
            } else {
                P--;
            }
            
            // Current modulo
            int mod = (i + 1) % mod_val;
            
            // Generate unique state key
            // (P + n) will at most be 2 * 50000 = 100000. Shifting by 12 bits easily accommodates the mod_val
            int key = ((P + n) << 12) | mod;
            
            // If we have seen this state before, all of them form beautiful substrings ending at the current character
            ans += counts[key];
            
            // Record the current state for future computations
            counts[key]++;
        }
        
        return ans;
    }
};