class Solution {
public:
    vector<long long> countKConstraintSubstrings(string s, int k, vector<vector<int>>& queries) {
        int n = s.length();
        vector<int> left(n);
        vector<long long> prefix(n + 1, 0);

        // Step 1: Sliding window to find the earliest valid start (left[j]) for every end (j)
        int l = 0;
        int count0 = 0, count1 = 0;
        
        for (int r = 0; r < n; ++r) {
            if (s[r] == '0') count0++;
            else count1++;

            // Shrink window if k-constraint is violated
            while (count0 > k && count1 > k) {
                if (s[l] == '0') count0--;
                else count1--;
                l++;
            }
            left[r] = l;
        }

        // Step 2: Build a prefix sum array of valid substring counts bounded by left[i]
        for (int i = 0; i < n; ++i) {
            // (i - left[i] + 1) is the number of valid substrings ending at i
            prefix[i + 1] = prefix[i] + (i - left[i] + 1);
        }

        // Step 3: Answer each query in O(log N)
        vector<long long> ans;
        ans.reserve(queries.size());
        
        for (const auto& q : queries) {
            int L = q[0];
            int R = q[1];

            // Binary search to find the split point `p` where left[p] > L
            int p = std::upper_bound(left.begin() + L, left.begin() + R + 1, L) - left.begin();
            
            // Part 1: Elements where left[j] <= L 
            // The segment length is (p - L)
            long long len = p - L;
            long long contrib1 = len * (len + 1) / 2;
            
            // Part 2: Elements where left[j] > L
            // Calculated using the prebuilt prefix sum array
            long long contrib2 = prefix[R + 1] - prefix[p];
            
            ans.push_back(contrib1 + contrib2);
        }

        return ans;
    }
};