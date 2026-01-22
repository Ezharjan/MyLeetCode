#include <vector>

using namespace std;

class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        int count = 0;
        int size = flowerbed.size();

        for (int i = 0; i < size; i++) {
            // Check if current plot is empty
            if (flowerbed[i] == 0) {
                // Check if left neighbor is empty (or if we are at start)
                bool emptyLeft = (i == 0) || (flowerbed[i - 1] == 0);
                
                // Check if right neighbor is empty (or if we are at end)
                bool emptyRight = (i == size - 1) || (flowerbed[i + 1] == 0);

                // If both sides are valid, plant the flower!
                if (emptyLeft && emptyRight) {
                    flowerbed[i] = 1; // Mark as planted so it blocks the next spot
                    count++;
                    
                    // Optimization: If we have already planted enough, return true early.
                    if (count >= n) {
                        return true;
                    }
                }
            }
        }

        return count >= n;
    }
};