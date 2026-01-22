#include <vector>

using namespace std;

class Solution {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& img) {
        int m = img.size();
        int n = img[0].size();
        // Create a result matrix of the same size initialized to 0
        vector<vector<int>> result(m, vector<int>(n));

        // Iterate over every pixel in the image
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                
                int sum = 0;
                int count = 0;

                // Iterate over the 3x3 window centered at (i, j)
                for (int r = i - 1; r <= i + 1; r++) {
                    for (int c = j - 1; c <= j + 1; c++) {
                        // Check if the neighbor (r, c) is within the grid boundaries
                        if (r >= 0 && r < m && c >= 0 && c < n) {
                            sum += img[r][c];
                            count++;
                        }
                    }
                }

                // Calculate average and store in the result matrix
                // Integer division automatically implements floor()
                result[i][j] = sum / count;
            }
        }

        return result;
    }
};