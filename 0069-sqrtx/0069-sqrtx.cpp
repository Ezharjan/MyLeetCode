class Solution {
public:
    int mySqrt(int x) {
        // Handle the edge case for 0 immediately
        if (x == 0) return 0;
        
        int left = 1;
        int right = x;
        int ans = 0; // Variable to store the potential floor value
        
        while (left <= right) {
            // Prevent overflow for large range logic
            int mid = left + (right - left) / 2;
            
            // Cast to long long to prevent overflow during multiplication
            if ((long long)mid * mid <= x) {
                ans = mid;      // mid is a valid candidate (<= x)
                left = mid + 1; // Try to find a larger value
            } else {
                right = mid - 1; // mid is too large
            }
        }
        
        return ans;
    }
};