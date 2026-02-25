class Solution {
public:
    string subStrHash(string s, int power, int modulo, int k, int hashValue) {
        long long p = power;
        long long m = modulo;
        long long current_hash = 0;
        long long pk = 1;
        int n = s.length();
        int best_idx = -1;

        // Helper lambda to get the value of a character
        auto val = [](char c) -> long long {
            return c - 'a' + 1;
        };

        // 1. Compute pk = p^k % m for removing the trailing character
        for (int i = 0; i < k; ++i) {
            pk = (pk * p) % m;
        }

        // 2. Calculate the initial hash for the last window [n-k, n-1]
        long long p_pow = 1;
        for (int i = n - k; i < n; ++i) {
            current_hash = (current_hash + val(s[i]) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }

        if (current_hash == hashValue) {
            best_idx = n - k;
        }

        // 3. Slide the window backwards to find the earliest occurrence
        for (int i = n - k - 1; i >= 0; --i) {
            // Multiply current hash by power
            current_hash = (current_hash * p) % m;
            
            // Add the new character entering the window on the left
            current_hash = (current_hash + val(s[i])) % m;
            
            // Subtract the character that just left the window on the right
            long long subtract = (val(s[i+k]) * pk) % m;
            
            // Add 'm' before modulo to handle potential negative numbers in C++
            current_hash = (current_hash - subtract + m) % m;

            // Update best_idx if it matches (iterating backwards guarantees the first occurrence)
            if (current_hash == hashValue) {
                best_idx = i;
            }
        }

        return s.substr(best_idx, k);
    }
};