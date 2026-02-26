#include <vector>

using namespace std;

class Solution {
    // Binary Indexed Tree (Fenwick Tree) for efficient point updates and range sums.
    struct BIT {
        int n;
        vector<int> tree;
        BIT(int n) : n(n), tree(n + 1, 0) {}
        
        void add(int i, int delta) {
            for (++i; i <= n; i += i & -i) {
                tree[i] += delta;
            }
        }
        
        int query(int i) {
            int sum = 0;
            for (++i; i > 0; i -= i & -i) {
                sum += tree[i];
            }
            return sum;
        }
    };

public:
    vector<int> countOfPeaks(vector<int>& nums, vector<vector<int>>& queries) {
        int n = nums.size();
        BIT bit(n);
        vector<int> is_peak(n, 0);
        
        // Helper lambda to update the peak status of an element in the Fenwick Tree
        auto update_peak = [&](int i) {
            // First and last elements can never be peaks
            if (i <= 0 || i >= n - 1) return;
            
            bool is_p = (nums[i] > nums[i - 1] && nums[i] > nums[i + 1]);
            
            // Only update if the peak status has actually changed
            if (is_p != is_peak[i]) {
                bit.add(i, is_p ? 1 : -1);
                is_peak[i] = is_p ? 1 : 0;
            }
        };

        // 1. Initialize the initial peaks in the array and populate the Fenwick Tree
        for (int i = 1; i < n - 1; ++i) {
            if (nums[i] > nums[i - 1] && nums[i] > nums[i + 1]) {
                is_peak[i] = 1;
                bit.add(i, 1);
            }
        }
        
        vector<int> ans;
        
        // 2. Process all queries
        for (const auto& q : queries) {
            if (q[0] == 1) {
                int li = q[1], ri = q[2];
                // If the subarray is too small to have strictly internal peaks, return 0.
                if (ri - li < 2) {
                    ans.push_back(0);
                } else {
                    // Count peaks strictly inside the boundaries of the subarray
                    ans.push_back(bit.query(ri - 1) - bit.query(li));
                }
            } else {
                int idx = q[1], val = q[2];
                nums[idx] = val;
                
                // An update only influences the peak status of the target and its direct neighbors
                update_peak(idx - 1);
                update_peak(idx);
                update_peak(idx + 1);
            }
        }
        
        return ans;
    }
};