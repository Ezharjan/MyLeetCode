#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size();
        int maxLen = 0;

        // Iterate over all possible starting points of the subarray
        for (int i = 0; i < n; ++i) {
            unordered_set<int> evens;
            unordered_set<int> odds;

            // Iterate over the ending points
            for (int j = i; j < n; ++j) {
                // Insert number into the corresponding set based on parity
                if (nums[j] % 2 == 0) {
                    evens.insert(nums[j]);
                } else {
                    odds.insert(nums[j]);
                }

                // Check if the subarray nums[i...j] is balanced
                if (evens.size() == odds.size()) {
                    maxLen = max(maxLen, j - i + 1);
                }
            }
        }

        return maxLen;
    }
};