#include <vector>
#include <algorithm>

using namespace std;

// Structure to hold our item pairs and their pre-calculated sum
struct Item {
    int n1, n2, sum;
    bool operator<(const Item& other) const {
        return n1 > other.n1; // Sort in descending order of nums1
    }
};

// Structure to map queries back to their original indices after sorting
struct Query {
    int x, y, id;
    bool operator<(const Query& other) const {
        return x > other.x; // Sort in descending order of x
    }
};

class Solution {
public:
    vector<int> maximumSumQueries(vector<int>& nums1, vector<int>& nums2, vector<vector<int>>& queries) {
        int n = nums1.size();
        vector<Item> items(n);
        for (int i = 0; i < n; ++i) {
            items[i] = {nums1[i], nums2[i], nums1[i] + nums2[i]};
        }
        // 1. Sort items descending by nums1
        sort(items.begin(), items.end());

        int q = queries.size();
        vector<Query> Q(q);
        for (int i = 0; i < q; ++i) {
            Q[i] = {queries[i][0], queries[i][1], i};
        }
        // 2. Sort queries descending by x
        sort(Q.begin(), Q.end());

        vector<int> ans(q, -1);
        vector<pair<int, int>> st; // Monotonic stack: stores pair<n2, sum>

        int j = 0; // Pointer for the items array

        for (int i = 0; i < q; ++i) {
            int x = Q[i].x;
            int y = Q[i].y;
            int id = Q[i].id;

            // 3. Add all valid items for the current query into the stack
            while (j < n && items[j].n1 >= x) {
                int n2 = items[j].n2;
                int sum = items[j].sum;

                // Remove strictly dominated elements from the stack
                while (!st.empty() && st.back().second <= sum) {
                    st.pop_back();
                }

                // Add the new element if it provides value (has a larger n2)
                if (st.empty() || st.back().first < n2) {
                    st.push_back({n2, sum});
                }
                j++;
            }

            // 4. Binary search for the first element in the stack with n2 >= y
            auto it = lower_bound(st.begin(), st.end(), make_pair(y, 0),
                [](const pair<int, int>& a, const pair<int, int>& b) {
                    return a.first < b.first;
                });

            // If a valid element is found, record its sum
            if (it != st.end()) {
                ans[id] = it->second;
            }
        }

        return ans;
    }
};