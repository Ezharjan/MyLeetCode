class Solution {
public:
    int lengthOfLIS(vector<int>& nums, int k) {
        // Find the maximum value in the array to size our Segment Tree
        int max_val = 0;
        for (int x : nums) {
            max_val = max(max_val, x);
        }
        
        // n is max_val + 1 so we can 1-index the values directly up to max_val
        int n = max_val + 1;
        vector<int> tree(2 * n, 0);
        
        // Lambda function to update the segment tree with the max length
        auto update = [&](int pos, int value) {
            pos += n;
            tree[pos] = max(tree[pos], value);
            for (pos >>= 1; pos > 0; pos >>= 1) {
                tree[pos] = max(tree[pos << 1], tree[pos << 1 | 1]);
            }
        };
        
        // Lambda function to query the maximum value in range [l, r]
        auto query = [&](int l, int r) {
            int res = 0;
            r++; // Adjust to make the interval right-open [l, r+1)
            for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
                if (l & 1) res = max(res, tree[l++]);
                if (r & 1) res = max(res, tree[--r]);
            }
            return res;
        };
        
        int ans = 0;
        for (int v : nums) {
            // The valid range for the previous element in the subsequence
            int l = max(1, v - k);
            int r = v - 1;
            
            int max_len = 0;
            if (l <= r) {
                max_len = query(l, r);
            }
            
            // Update the segment tree for the current value `v`
            update(v, max_len + 1);
            ans = max(ans, max_len + 1);
        }
        
        return ans;
    }
};