#include <algorithm>

using namespace std;

class Solution {
public:
    int minMoves(int sx, int sy, int tx, int ty) {
        int moves = 0;
        
        // Work backwards from (tx, ty) to (sx, sy)
        while (tx >= sx && ty >= sy) {
            // Check if we reached the target
            if (tx == sx && ty == sy) {
                return moves;
            }
            
            if (tx == ty) {
                // (z, z) can only be reached from (0, z) or (z, 0)
                if (sx == 0) {
                    tx = 0;
                    moves++;
                } else if (sy == 0) {
                    ty = 0;
                    moves++;
                } else {
                    return -1; // Cannot reach (z, z) if start has no zeros
                }
            } else if (tx > ty) {
                // Last move changed x
                if (tx > 2 * ty) {
                    // Must have come from doubling
                    if (tx % 2 != 0) return -1;
                    tx /= 2;
                    moves++;
                } else {
                    // Must have come from addition (x_old + y)
                    tx -= ty;
                    moves++;
                }
            } else { // ty > tx
                // Last move changed y
                if (ty > 2 * tx) {
                    // Must have come from doubling
                    if (ty % 2 != 0) return -1;
                    ty /= 2;
                    moves++;
                } else {
                    // Must have come from addition (y_old + x)
                    ty -= tx;
                    moves++;
                }
            }
        }
        
        return -1;
    }
};