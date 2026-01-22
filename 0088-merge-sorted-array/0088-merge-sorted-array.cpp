class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i = m - 1;      // Last valid element of nums1
        int j = n - 1;      // Last element of nums2
        int k = m + n - 1;  // Last position in nums1 (total size)

        // While there are still elements in nums2 to merge
        while (j >= 0) {
            // If nums1 has elements left AND nums1's element is larger
            if (i >= 0 && nums1[i] > nums2[j]) {
                nums1[k] = nums1[i];
                i--;
            } else {
                // Otherwise, take from nums2 (this handles when nums1 runs out)
                nums1[k] = nums2[j];
                j--;
            }
            k--; // Move the placement pointer backwards
        }
    }
};