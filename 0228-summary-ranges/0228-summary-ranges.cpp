#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> result;
        int n = nums.size();
        
        if (n == 0) return result; // Handle empty array case

        for (int i = 0; i < n; ++i) {
            int start = nums[i];
            
            // Look ahead: Keep advancing 'i' while the next element is consecutive
            // Condition: i < n - 1 (next element exists) AND nums[i+1] == nums[i] + 1
            while (i < n - 1 && nums[i + 1] == nums[i] + 1) {
                i++;
            }
            
            // Now 'i' points to the last element of the current range
            if (start != nums[i]) {
                // It's a range (e.g., "0->2")
                result.push_back(to_string(start) + "->" + to_string(nums[i]));
            } else {
                // It's a single number (e.g., "7")
                result.push_back(to_string(start));
            }
        }
        
        return result;
    }
};