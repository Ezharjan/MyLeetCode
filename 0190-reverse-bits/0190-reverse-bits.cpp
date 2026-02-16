#include <cstdint>

class Solution {
public:
    int reverseBits(int n) {
        uint32_t result = 0;
        uint32_t un = n; // Cast to unsigned to avoid Undefined Behavior (UB) on shifts
        
        for (int i = 0; i < 32; ++i) {
            // 1. Make room in the result for the new bit
            result <<= 1;
            
            // 2. Extract the rightmost bit from 'un' and add it to 'result'
            result |= (un & 1);
            
            // 3. Shift 'un' to the right to process the next bit in the next iteration
            un >>= 1;
        }
        
        return result; // Implicitly converts back to signed int
    }
};