#include <vector>
#include <queue>
#include <tuple>
#include <functional>

using namespace std;

class Solution {
public:
    int minimumPairRemoval(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return 0;

        // Use long long to prevent integer overflow when summing elements
        vector<long long> vals(n);
        for(int i = 0; i < n; ++i) vals[i] = nums[i];

        // Simulate Doubly Linked List using arrays
        // left[i] is the index of the left neighbor of i
        // right[i] is the index of the right neighbor of i
        vector<int> left(n), right(n);
        for(int i = 0; i < n; ++i) {
            left[i] = i - 1;
            right[i] = i + 1;
        }
        right[n - 1] = -1; // End of list marker

        // Count initial "bad" pairs (inversions where left > right)
        int bad_count = 0;
        for(int i = 0; i < n - 1; ++i) {
            if (vals[i] > vals[i+1]) {
                bad_count++;
            }
        }

        if (bad_count == 0) return 0;

        // Min-Heap to store pairs: {sum, index_u, index_v}
        // "greater" ensures we pop the smallest sum first.
        // Tie-breaker: std::tuple compares elements in order, so if sums are equal,
        // it compares index_u (leftmost index), satisfying the problem constraint.
        using Element = tuple<long long, int, int>;
        priority_queue<Element, vector<Element>, greater<Element>> pq;

        for(int i = 0; i < n - 1; ++i) {
            pq.push({vals[i] + vals[i+1], i, i+1});
        }

        int ops = 0;

        // Process until the array is sorted
        while (bad_count > 0 && !pq.empty()) {
            auto [sum, u, v] = pq.top();
            pq.pop();

            // Lazy Validation:
            // 1. Check if u and v are still neighbors (topology check)
            // 2. Check if the sum is current (value check)
            if (right[u] != v || vals[u] + vals[v] != sum) {
                continue;
            }

            // Perform the Merge Operation
            ops++;
            
            int p = left[u];   // Previous neighbor
            int nxt = right[v]; // Next neighbor

            // 1. Remove old "bad" contributions before changing values
            if (p != -1 && vals[p] > vals[u]) bad_count--; // Violation between prev and u
            if (vals[u] > vals[v]) bad_count--;            // Violation between u and v
            if (nxt != -1 && vals[v] > vals[nxt]) bad_count--; // Violation between v and next

            // 2. Update values and links
            // u absorbs v
            vals[u] += vals[v];
            
            // Link u to nxt (skipping v)
            right[u] = nxt;
            if (nxt != -1) {
                left[nxt] = u;
            }
            // v is now effectively removed

            // 3. Add new "bad" contributions with new values
            if (p != -1 && vals[p] > vals[u]) bad_count++;
            if (nxt != -1 && vals[u] > vals[nxt]) bad_count++;

            // 4. Push new potential merges to the heap
            if (p != -1) {
                pq.push({vals[p] + vals[u], p, u});
            }
            if (nxt != -1) {
                pq.push({vals[u] + vals[nxt], u, nxt});
            }
        }

        return ops;
    }
};