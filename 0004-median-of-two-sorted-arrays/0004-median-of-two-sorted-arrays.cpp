#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // Step 1: Ensure we always do binary search on the smaller array
        // This guarantees O(log(min(m, n))) complexity
        if (nums1.size() > nums2.size()) {
            return findMedianSortedArrays(nums2, nums1);
        }

        int m = nums1.size();
        int n = nums2.size();
        int low = 0, high = m;

        // Step 2: Perform Binary Search
        while (low <= high) {
            // Partition index for nums1
            int partitionX = (low + high) / 2;
            
            // Partition index for nums2 (derived to ensure left half has correct size)
            // (m + n + 1) / 2 works for both odd and even total lengths
            int partitionY = (m + n + 1) / 2 - partitionX;

            // Step 3: Handle edge cases where partition is at the very start or end
            // Use INT_MIN for empty left parts and INT_MAX for empty right parts
            int maxLeftX = (partitionX == 0) ? INT_MIN : nums1[partitionX - 1];
            int minRightX = (partitionX == m) ? INT_MAX : nums1[partitionX];

            int maxLeftY = (partitionY == 0) ? INT_MIN : nums2[partitionY - 1];
            int minRightY = (partitionY == n) ? INT_MAX : nums2[partitionY];

            // Step 4: Check if we found the correct partition
            if (maxLeftX <= minRightY && maxLeftY <= minRightX) {
                // Correct partition found!
                
                // If total number of elements is even
                if ((m + n) % 2 == 0) {
                    return (max(maxLeftX, maxLeftY) + min(minRightX, minRightY)) / 2.0;
                } 
                // If total number of elements is odd
                else {
                    return max(maxLeftX, maxLeftY);
                }
            } 
            else if (maxLeftX > minRightY) {
                // We are too far right in nums1, move left
                high = partitionX - 1;
            } 
            else {
                // We are too far left in nums1, move right
                low = partitionX + 1;
            }
        }

        return 0.0; // Should not reach here if input is valid sorted arrays
    }
};