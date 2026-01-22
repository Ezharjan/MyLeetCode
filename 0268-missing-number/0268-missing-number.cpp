#include <vector>
#include <numeric> // For std::accumulate

using namespace std;

class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = nums.size();
        
        // 1. Calculate Expected Sum: Sum of 0..n
        // Formula: n * (n + 1) / 2
        int expectedSum = n * (n + 1) / 2;
        
        // 2. Calculate Actual Sum: Sum of elements in the vector
        int actualSum = 0;
        for (int num : nums) {
            actualSum += num;
        }
        
        // 3. The difference is the missing number
        return expectedSum - actualSum;
    }
};