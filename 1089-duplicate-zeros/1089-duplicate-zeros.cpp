#include <vector>

using namespace std;

class Solution {
public:
    void duplicateZeros(vector<int>& arr) {
        int zeros = 0;
        // Pass 1: Count the zeros to determine the theoretical size of the expanded array
        for (int num : arr) {
            if (num == 0) {
                zeros++;
            }
        }

        int n = arr.size();
        // The write pointer starts at the end of the "theoretical" expanded array.
        // Example: If n=3 and zeros=1, theoretical length is 4. indices: 0,1,2,(3). 
        // We start writing conceptually at index 3.
        int write = n + zeros - 1;

        // Pass 2: Iterate backwards from the last element of the original array
        for (int i = n - 1; i >= 0; i--) {
            
            // Check if we need to duplicate this zero
            if (arr[i] == 0) {
                // Try to write the second zero (the duplicate)
                if (write < n) {
                    arr[write] = 0;
                }
                write--;
                
                // Try to write the first zero (the original)
                if (write < n) {
                    arr[write] = 0;
                }
                write--;
            } else {
                // Non-zero: just copy it to the new position
                if (write < n) {
                    arr[write] = arr[i];
                }
                write--;
            }
        }
    }
};