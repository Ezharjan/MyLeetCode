#include <vector>

using namespace std;

class Solution {
public:
    // Helper function to perform DFS
    void dfs(vector<vector<int>>& image, int r, int c, int originalColor, int newColor) {
        // 1. Check Boundary Conditions (strictly inside the grid)
        if (r < 0 || r >= image.size() || c < 0 || c >= image[0].size()) {
            return;
        }
        
        // 2. Check Validity (must match the original color to be filled)
        // Also stops if we hit a pixel that is already changed or a different color boundary
        if (image[r][c] != originalColor) {
            return;
        }

        // 3. Process the Node (Change color)
        image[r][c] = newColor;

        // 4. Recurse to 4 directions (Up, Down, Left, Right)
        dfs(image, r + 1, c, originalColor, newColor);
        dfs(image, r - 1, c, originalColor, newColor);
        dfs(image, r, c + 1, originalColor, newColor);
        dfs(image, r, c - 1, originalColor, newColor);
    }

    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int originalColor = image[sr][sc];
        
        // Edge Case: If the start pixel already has the target color, 
        // we don't need to do anything (and must avoid infinite loop).
        if (originalColor != color) {
            dfs(image, sr, sc, originalColor, color);
        }
        
        return image;
    }
};