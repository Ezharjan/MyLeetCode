class Solution {
public:
    bool isTrionic(vector<int>& nums) {
        int n = nums.size();
        // A Trionic array must have at least 4 elements to satisfy 
        // the indices 0 < p < q < n-1 structure.
        if (n < 4) return false;

        int i = 0;

        // 1. First Segment: Strictly Increasing
        while (i < n - 1 && nums[i] < nums[i + 1]) {
            i++;
        }
        // Fail if we didn't move (i == 0) or if the whole array is increasing (i == n - 1)
        if (i == 0 || i == n - 1) return false;

        // 2. Second Segment: Strictly Decreasing
        int p = i; // This is our peak
        while (i < n - 1 && nums[i] > nums[i + 1]) {
            i++;
        }
        // Fail if we didn't move (i == p) or if we reached the end (i == n - 1)
        // (If we reach the end, there is no room for the third increasing segment)
        if (i == p || i == n - 1) return false;

        // 3. Third Segment: Strictly Increasing
        while (i < n - 1 && nums[i] < nums[i + 1]) {
            i++;
        }

        // If we successfully traversed strictly to the last element, it is Trionic.
        return i == n - 1;
    }
};