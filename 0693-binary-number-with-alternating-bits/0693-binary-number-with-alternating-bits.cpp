class Solution {
public:
    bool hasAlternatingBits(int n) {
        // 1. XOR n with itself shifted right by 1.
        // If n is alternating (e.g., 101), temp will be all 1s (e.g., 111).
        // We use 'long' to prevent overflow when doing (temp + 1) later.
        long temp = n ^ (n >> 1);

        // 2. Check if temp consists of all 1s.
        // If temp is all 1s (e.g., 111), then temp + 1 is a power of 2 (e.g., 1000).
        // The bitwise AND of these two will be 0.
        return (temp & (temp + 1)) == 0;
    }
};