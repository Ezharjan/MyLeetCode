#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    // Helper to unwrap 2D square coordinates to a 1D circular perimeter coordinate
    long long mapPoint(int x, int y, long long side) {
        if (y == 0) return x;
        if (x == side) return side + y;
        if (y == side) return 3LL * side - x;
        if (x == 0) return 4LL * side - y;
        return 0;
    }

    // Greedily checks if picking k points with at least D distance is possible
    bool check(long long D, const vector<long long>& A, int N, int k, long long C) {
        vector<int> nxt(2 * N);
        int j = 0;
        
        // Two-pointers to precompute the earliest next point >= current point + D
        for (int i = 0; i < 2 * N; ++i) {
            while (j < 2 * N && A[j] - A[i] < D) {
                j++;
            }
            nxt[i] = j;
        }
        
        // Try starting the k-sequence at each of the N available points
        for (int i = 0; i < N; ++i) {
            int curr = i;
            int steps = 0;
            
            for (; steps < k - 1; ++steps) {
                curr = nxt[curr];
                if (curr >= 2 * N) break; // Not enough points ahead
            }
            
            // If we successfully made k-1 jumps and the circular wrap-around is valid
            if (steps == k - 1 && curr < 2 * N && A[curr] <= A[i] + C - D) {
                return true;
            }
        }
        return false;
    }

public:
    int maxDistance(int side, vector<vector<int>>& points, int k) {
        int N = points.size();
        vector<long long> A(N);
        
        for (int i = 0; i < N; ++i) {
            A[i] = mapPoint(points[i][0], points[i][1], side);
        }
        
        // Sort mapped points to represent sequential traversal along the boundary
        sort(A.begin(), A.end());
        
        vector<long long> extended_A(2 * N);
        long long C = 4LL * side;
        
        // Duplicate array for circular traversal resolution
        for (int i = 0; i < N; ++i) {
            extended_A[i] = A[i];
            extended_A[i + N] = A[i] + C;
        }
        
        long long low = 1, high = C / k;
        long long ans = 1;
        
        // Binary search the optimal minimum distance
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (check(mid, extended_A, N, k, C)) {
                ans = mid;        // `mid` is possible; log it and try for a larger gap
                low = mid + 1;
            } else {
                high = mid - 1;   // `mid` is too large; scale down
            }
        }
        
        return static_cast<int>(ans);
    }
};