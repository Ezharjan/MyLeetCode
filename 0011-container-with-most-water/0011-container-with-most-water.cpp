#include <vector>
#include <algorithm> // for max, min

using namespace std;

class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = height.size() - 1;
        int max_water = 0;

        while (left < right) {
            // Calculate the height of the container (limited by the shorter side)
            int h = min(height[left], height[right]);
            // Calculate width
            int w = right - left;
            
            // Update the maximum area found so far
            max_water = max(max_water, h * w);

            // Move the pointer corresponding to the shorter line
            // Moving the taller line would limit the area further (less width, same max height)
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }

        return max_water;
    }
};