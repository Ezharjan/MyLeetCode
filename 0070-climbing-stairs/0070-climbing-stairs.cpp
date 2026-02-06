class Solution {
public:
    int climbStairs(int n) {
        // Base cases: if n is 1 or 2, the answer is just n.
        if (n <= 2) {
            return n;
        }

        // Initialize the first two steps
        // prev1 represents ways(i-2)
        // prev2 represents ways(i-1)
        int prev1 = 1; 
        int prev2 = 2; 
        int current = 0;

        // Start calculating from step 3 up to n
        for (int i = 3; i <= n; i++) {
            current = prev1 + prev2; // Sum of the previous two steps
            
            // Shift the values for the next iteration
            prev1 = prev2;
            prev2 = current;
        }

        return current;
    }
};