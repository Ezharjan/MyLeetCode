#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        // Create a hash set to store elements we have seen
        unordered_set<int> seen;
        
        for (int num : nums) {
            // Check if the current number is already in the set
            if (seen.count(num)) {
                return true; // Duplicate found
            }
            // If not, add it to the set
            seen.insert(num);
        }
        
        // If we loop through everything and find no duplicates
        return false;
    }
};