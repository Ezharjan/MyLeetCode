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

        vector<long long> vals(n);
        for(int i = 0; i < n; ++i) vals[i] = nums[i];

        // Doubly Linked List Simulation
        vector<int> left(n), right(n);
        for(int i = 0; i < n; ++i) {
            left[i] = i - 1;
            right[i] = i + 1;
        }
        right[n - 1] = -1; 

        // Initial bad count
        int bad_count = 0;
        for(int i = 0; i < n - 1; ++i) {
            if (vals[i] > vals[i+1]) {
                bad_count++;
            }
        }

        if (bad_count == 0) return 0;

        // Min-Heap: {sum, u, v}
        using Element = tuple<long long, int, int>;
        priority_queue<Element, vector<Element>, greater<Element>> pq;

        for(int i = 0; i < n - 1; ++i) {
            pq.push({vals[i] + vals[i+1], i, i+1});
        }

        int ops = 0;

        while (bad_count > 0 && !pq.empty()) {
            auto [sum, u, v] = pq.top();
            pq.pop();

            // Validation: 
            // 1. Check if 'u' thinks 'v' is its right neighbor.
            //    If u was removed, right[u] would be -1, so this check works for u being dead too.
            // 2. Check if the sum is current.
            if (right[u] != v || vals[u] + vals[v] != sum) {
                continue;
            }

            ops++;
            
            int p = left[u];
            int nxt = right[v];

            // 1. Remove old bad counts
            if (p != -1 && vals[p] > vals[u]) bad_count--;
            if (vals[u] > vals[v]) bad_count--; 
            if (nxt != -1 && vals[v] > vals[nxt]) bad_count--;

            // 2. Merge u and v into u
            vals[u] += vals[v];
            
            // Fix pointers
            right[u] = nxt;
            if (nxt != -1) left[nxt] = u;
            
            // CRITICAL FIX: Mark v as dead so it cannot be used as a 'u' in future pops
            right[v] = -1; 
            
            // 3. Add new bad counts
            if (p != -1 && vals[p] > vals[u]) bad_count++;
            if (nxt != -1 && vals[u] > vals[nxt]) bad_count++;

            // 4. Push new candidates
            if (p != -1) pq.push({vals[p] + vals[u], p, u});
            if (nxt != -1) pq.push({vals[u] + vals[nxt], u, nxt});
        }

        return ops;
    }
};