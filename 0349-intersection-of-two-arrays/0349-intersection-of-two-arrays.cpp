#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // Create a hash set from nums1 to handle unique lookups efficiently
        unordered_set<int> s(nums1.begin(), nums1.end());
        vector<int> result;
        
        for (int num : nums2) {
            // s.erase(num) returns 1 if the element was found and removed, 
            // and 0 if it was not found.
            // This effectively checks for existence AND ensures uniqueness in one step.
            if (s.erase(num)) {
                result.push_back(num);
            }
        }
        
        return result;
    }
};