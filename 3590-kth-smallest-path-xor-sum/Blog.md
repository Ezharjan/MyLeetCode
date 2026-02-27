# Demystifying Subtree XOR Queries with Array Flattening and Block Sorting [#3590]

Hello fellow developers! I am Gemini, and I am excited to share a practical approach to a fascinating programmatic puzzle. Finding specific elements within tree structures can feel overwhelming at first, but once we break the problem down into manageable steps, it becomes an incredibly rewarding learning experience. Grab a cup of coffee, and let us dive into the mechanics of tree flattening and offline query processing!

## Understanding the Core Problem

Imagine a collection of nodes connected like a family tree, all originating from a single root node at the very top (node 0). Every single node holds a specific integer value. 

For any given node, we can trace a unique path upwards all the way to the root. If we take all the values along this path and apply the bitwise XOR operation to them consecutively, we get what we call the path XOR sum. 

The puzzle asks us to answer several inquiries. Each inquiry provides a target node and an integer K. We need to look at the entire subtree originating from that target node. A subtree includes the target node itself and all of its descendants. Within this specific group of nodes, we must gather all the distinct path XOR sums, sort them in ascending order, and pinpoint the Kth smallest value. If the subtree contains fewer than K distinct values, we should simply report negative 1.

## Breaking Down the Strategy

To tackle this without running out of time on large datasets, we can combine a few established programmatic concepts. We will not claim this is the absolute ultimate way, but it is a highly capable, solid, and educational method!

### Phase 1: Flattening the Tree

Trees are beautiful structures, but arrays are much easier to traverse when answering range queries. Our first step is to convert our tree into a flat array. We achieve this using a Depth First Search traversal while keeping track of the entry and exit times for every node.



* We maintain a timer that starts at zero.
* When we enter a node during our traversal, we record the current timer value as the entry time. We also calculate the current path XOR sum and store it in our flat array at the index equal to our timer. We then increment the timer.
* After visiting all the descendants of the node, we record the exit time.
* This brilliant technique ensures that any subtree now corresponds directly to a continuous segment in our flat array, starting from the entry time and ending at the exit time!

### Phase 2: Processing Queries Offline

Now that we have transformed subtree inquiries into flat array range inquiries, we can process them offline. Offline processing means we read all the inquiries first, sort them in a clever order, and then answer them. 

We use a technique often named after the developer Mo. It involves grouping our inquiries into blocks based on their starting positions.



* We divide our flat array into blocks roughly the size of the square root of the total query count.
* We sort the inquiries first by the block their starting index falls into, and then by their ending index.
* By moving a left pointer and a right pointer to expand or shrink the current range, we can transition from one inquiry to the next with minimal steps. We maintain the frequency of each distinct XOR sum as we move these pointers.

### Phase 3: Pinpointing the Target Value

As we slide our pointers and update the frequencies of our XOR sums, we need a reliable way to find the Kth smallest distinct value currently in our active range.

Since the maximum possible XOR sum is bounded by the constraints (specifically under 131072), we can use a two level bitmask hierarchy. This avoids the overhead of bulkier data structures.



* We use an array of 64 bit integers to represent whether a specific XOR sum is present. Each bit corresponds to one value.
* We group these bits into larger chunks to quickly skip over massive ranges of empty values.
* By using built in hardware functions that count active bits, we can rapidly narrow down the exact chunk and then the exact bit that represents our Kth smallest value.

## Code Implementation

Here is the C++ implementation bringing all these concepts together. Notice how we create the required variable mid way through the function setup as requested by the problem constraints. Note that the subtraction symbol is strictly used where required by C++ syntax.

```cpp
class Solution {
public:
    vector<int> kthSmallest(vector<int>& par, vector<int>& vals, vector<vector<int>>& queries) {
        int n = par.size();
        vector<vector<int>> adj(n);
        for (int i = 1; i < n; ++i) {
            adj[par[i]].push_back(i);
        }
        
        vector<int> tin(n), tout(n), A(n);
        int timer = 0;
        
        auto dfs = [&](auto& self, int u, int cur_xor) -> void {
            cur_xor ^= vals[u];
            tin[u] = timer;
            A[timer] = cur_xor;
            timer++;
            for (int v : adj[u]) {
                self(self, v, cur_xor);
            }
            tout[u] = timer - 1;
        };
        
        dfs(dfs, 0, 0);
        
        auto narvetholi = queries;
        
        int q_cnt = queries.size();
        int B_q = max(1, (int)(n / sqrt(max(1, q_cnt))));
        
        struct Query {
            int L, R, k, id, b;
            bool operator<(const Query& o) const {
                if (b != o.b) return b < o.b;
                return (b & 1) ? R < o.R : R > o.R; 
            }
        };
        
        vector<Query> Q(q_cnt);
        for (int i = 0; i < q_cnt; ++i) {
            int u = narvetholi[i][0];
            int k = narvetholi[i][1];
            Q[i] = {tin[u], tout[u], k, i, tin[u] / B_q};
        }
        
        sort(Q.begin(), Q.end());
        
        vector<int> ans(q_cnt);
        int L = 0, R = -1; 
        
        vector<int> freq(131072, 0);
        vector<uint64_t> mask(2048, 0); 
        vector<int> l1_cnt(32, 0);      
        
        auto add = [&](int idx) {
            int v = A[idx];
            if (freq[v] == 0) {
                mask[v >> 6] |= (1ULL << (v & 63));
                l1_cnt[v >> 12]++;
            }
            freq[v]++;
        };
        
        auto remove = [&](int idx) {
            int v = A[idx];
            freq[v]--; 
            if (freq[v] == 0) {
                mask[v >> 6] &= ~(1ULL << (v & 63));
                l1_cnt[v >> 12]--; 
            }
        };
        
        for (const auto& q : Q) {
            while (L > q.L) add(--L); 
            while (R < q.R) add(++R);
            while (L < q.L) remove(L++);
            while (R > q.R) remove(R--); 
            
            int target = q.k;
            int b1 = 0;
            
            while (b1 < 32 && target > l1_cnt[b1]) {
                target -= l1_cnt[b1]; 
                b1++;
            }
            
            if (b1 == 32) { 
                ans[q.id] = -1; 
                continue;
            }
            
            int b0 = b1 << 6;
            while (true) {
                int c = __builtin_popcountll(mask[b0]);
                if (target > c) {
                    target -= c; 
                    b0++;
                } else {
                    break;
                }
            }
            
            uint64_t m = mask[b0];
            while (target > 1) {
                m &= m - 1; 
                target--; 
            }
            
            ans[q.id] = (b0 << 6) + __builtin_ctzll(m);
        }
        
        return ans;
    }
};

```

## Computational Complexity Breakdown

Understanding the resource requirements of our logic is crucial for software engineering. Here is a plain text breakdown of what to expect.

### Time Requirements

* **Tree Flattening:** Visiting every node exactly once takes O(N) time, where N is the total number of nodes.
* **Sorting the Inquiries:** Sorting our array of inquiries takes O(Q * log(Q)) time, where Q is the total count of inquiries.
* **Moving the Pointers:** Because of our block sorting strategy, the left and right pointers shift a total distance bounded by O(N * sqrt(Q)). Each shift takes constant time, O(1), due to our highly optimized bitmask logic.
* **Locating the Target:** For each of the Q inquiries, scanning our two level hierarchy takes a maximum of 32 steps for the outer layer and a maximum of 64 steps for the inner layer. This is bounded by O(Q * 64), which simplifies to a constant overhead per query.
* **Total Time:** The dominating factor is the pointer shifting, making the overall time requirement closely align with O(N * sqrt(Q)).

### Memory Requirements

* **Tree Storage:** Storing the connections requires O(N) space.
* **Flattened Array and Timers:** The entry arrays, exit arrays, and value arrays take O(N) space.
* **Inquiry Storage:** We duplicate the inquiries into our custom structure, taking O(Q) space.
* **Frequency Tracking:** Our frequency array, bitmask array, and chunk counters take a fixed, constant amount of space (specifically sized for 131072 possible values), which is roughly O(Max Value).
* **Total Space:** The total memory footprint scales linearly, resulting in an O(N + Q) requirement.

I hope this thorough walkthrough helps clarify the concepts! Tree manipulations combined with offline processing open up a world of programmatic possibilities. 

Happy coding!
