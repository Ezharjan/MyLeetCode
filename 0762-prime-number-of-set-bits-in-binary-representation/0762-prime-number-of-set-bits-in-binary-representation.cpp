class Solution {
public:
    int countPrimeSetBits(int left, int right) {
        int count = 0;
        
        // A bitmask where the bit is 1 if the index is a prime number (up to 19).
        // Primes up to 19: 2, 3, 5, 7, 11, 13, 17, 19
        int primeMask = (1 << 2) | (1 << 3) | (1 << 5) | (1 << 7) | 
                        (1 << 11) | (1 << 13) | (1 << 17) | (1 << 19);
        
        for (int i = left; i <= right; ++i) {
            // Count the number of 1s in the binary representation of i
            int setBits = __builtin_popcount(i);
            
            // Check if the setBits-th bit is a 1 in our primeMask
            if ((primeMask >> setBits) & 1) {
                count++;
            }
        }
        
        return count;
    }
};