#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findValidSplit(vector<int>& nums) {
        // Statically allocate and compute the Smallest Prime Factor (SPF) sieve 
        // once to optimize across multiple test cases.
        static int spf[1000005];
        static bool inited = false;
        if (!inited) {
            for (int i = 2; i <= 1000000; i++) {
                spf[i] = i;
            }
            for (int i = 2; i * i <= 1000000; i++) {
                if (spf[i] == i) {
                    for (int j = i * i; j <= 1000000; j += i) {
                        if (spf[j] == j) spf[j] = i;
                    }
                }
            }
            inited = true;
        }

        int n = nums.size();
        
        // Array to store the last seen index of each prime factor
        vector<int> last(1000005, 0); 
        for (int i = 0; i < n; ++i) {
            int x = nums[i];
            while (x > 1) {
                int p = spf[x];
                last[p] = i;
                while (x % p == 0) {
                    x /= p;
                }
            }
        }

        int max_reach = 0;
        
        // Find the earliest valid split point
        for (int i = 0; i < n - 1; ++i) {
            int x = nums[i];
            while (x > 1) {
                int p = spf[x];
                max_reach = max(max_reach, last[p]);
                while (x % p == 0) {
                    x /= p;
                }
            }
            
            // If the furthest required index to enclose overlapping prime factors is exactly i,
            // we have cleanly partitioned the left and right halves without shared primes.
            if (max_reach == i) {
                return i;
            }
        }

        return -1;
    }
};