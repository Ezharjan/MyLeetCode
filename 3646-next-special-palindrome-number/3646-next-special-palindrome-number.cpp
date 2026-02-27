#include <vector>
#include <string>
#include <algorithm>

class Solution {
public:
    long long specialPalindrome(long long n) {
        std::vector<long long> P;
        int evens[] = {2, 4, 6, 8};
        int odds[] = {-1, 1, 3, 5, 7, 9}; // -1 represents picking no odd digit

        // Iterate through all subsets of even digits (2^4 = 16 masks)
        for (int mask = 0; mask < 16; ++mask) {
            // Iterate through picking one or zero odd digits
            for (int odd : odds) {
                int len = 0;
                std::string left_half = "";
                
                for (int i = 0; i < 4; ++i) {
                    if (mask & (1 << i)) {
                        int d = evens[i];
                        len += d;
                        left_half += std::string(d / 2, '0' + d);
                    }
                }
                
                if (odd != -1) {
                    len += odd;
                    // Integer division inherently drops the remainder: 1/2 = 0, 3/2 = 1, etc.
                    left_half += std::string(odd / 2, '0' + odd); 
                }
                
                // Keep palindromes that fit within a safe long long range
                if (len > 0 && len <= 18) {
                    std::sort(left_half.begin(), left_half.end());
                    
                    // Generate all unique permutations for the current left_half
                    do {
                        std::string full = left_half;
                        if (odd != -1) {
                            full += std::to_string(odd); // Insert the middle character
                        }
                        
                        std::string right_half = left_half;
                        std::reverse(right_half.begin(), right_half.end());
                        full += right_half;
                        
                        P.push_back(std::stoll(full));
                    } while (std::next_permutation(left_half.begin(), left_half.end()));
                }
            }
        }
        
        // Sort the generated special numbers and search for strictly greater
        std::sort(P.begin(), P.end());
        auto it = std::upper_bound(P.begin(), P.end(), n);
        
        return *it;
    }
};