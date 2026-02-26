#include <vector>
#include <numeric>
#include <algorithm>

class Solution {
public:
    long long countOperationsToEmptyArray(std::vector<int>& nums) {
        int n = nums.size();
        
        // Store the original indices of the elements
        std::vector<int> indices(n);
        std::iota(indices.begin(), indices.end(), 0);
        
        // Sort the indices based on the actual values in nums
        std::sort(indices.begin(), indices.end(), [&](int a, int b) {
            return nums[a] < nums[b];
        });
        
        // Baseline: we have to perform at least 1 operation to remove each element
        long long operations = n;
        
        // Traverse the sorted indices
        for (int i = 1; i < n; ++i) {
            // If the current element appeared before the previous strictly smaller element
            // in the original array, it means we have to wrap around.
            if (indices[i] < indices[i - 1]) {
                // Add the number of elements that are currently remaining in the array
                operations += (n - i);
            }
        }
        
        return operations;
    }
};