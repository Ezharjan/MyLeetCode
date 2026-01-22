#include <vector>
#include <climits> // For LLONG_MIN

using namespace std;

class Solution {
public:
    int thirdMax(vector<int>& nums) {
        // Use long long to ensure our sentinel value is smaller than any int,
        // specifically to handle the edge case where the input contains INT_MIN.
        long long m1 = LLONG_MIN;
        long long m2 = LLONG_MIN;
        long long m3 = LLONG_MIN;

        for (int num : nums) {
            // Skip duplicates to ensure distinct maximums
            if (num == m1 || num == m2 || num == m3) {
                continue;
            }

            if (num > m1) {
                m3 = m2;
                m2 = m1;
                m1 = num;
            } else if (num > m2) {
                m3 = m2;
                m2 = num;
            } else if (num > m3) {
                m3 = num;
            }
        }

        // If m3 is still the sentinel value, it means we found fewer than 3 distinct numbers.
        // In that case, the problem requires returning the maximum (m1).
        if (m3 == LLONG_MIN) {
            return (int)m1;
        }
        
        return (int)m3;
    }
};