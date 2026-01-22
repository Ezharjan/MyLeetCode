#include <vector>
#include <algorithm> // for swap

using namespace std;

class Solution {
public:
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& image) {
        int n = image.size();
        
        for (auto& row : image) {
            int left = 0; 
            int right = n - 1;
            
            while (left <= right) {
                // Logic optimization:
                // If values are equal (e.g. 1...1 or 0...0), after swap & invert they become 0...0 or 1...1
                // effectively just inverting both.
                if (row[left] == row[right]) {
                    row[left] ^= 1;  // XOR with 1 flips the bit (0->1, 1->0)
                    row[right] = row[left]; // Start and End are same, so copy the flipped value
                }
                // If values are different (1...0 or 0...1), swap makes them (0...1 or 1...0)
                // then invert makes them (1...0 or 0...1).
                // They return to original state! So we do nothing.
                
                left++;
                right--;
            }
        }
        
        return image;
    }
};