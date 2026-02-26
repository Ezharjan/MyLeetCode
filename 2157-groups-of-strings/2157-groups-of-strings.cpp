#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> groupStrings(vector<string>& words) {
        int n = words.size();
        vector<int> masks;
        masks.reserve(n);
        
        // Convert each word to a bitmask representation
        for (const string& w : words) {
            int mask = 0;
            for (char c : w) {
                mask |= (1 << (c - 'a'));
            }
            masks.push_back(mask);
        }
        
        // Sort to easily identify and count duplicate masks
        sort(masks.begin(), masks.end());
        
        vector<int> unique_masks;
        vector<int> freq;
        for (int m : masks) {
            if (unique_masks.empty() || unique_masks.back() != m) {
                unique_masks.push_back(m);
                freq.push_back(1);
            } else {
                freq.back()++;
            }
        }
        
        int N = unique_masks.size();
        vector<int> parent(N);
        vector<int> sz(N);
        int max_size = 0;
        
        for (int i = 0; i < N; ++i) {
            parent[i] = i;
            sz[i] = freq[i];
            max_size = max(max_size, sz[i]);
        }
        
        int num_components = N;
        
        // Disjoint Set Union `find` logic with path compression
        auto find_set = [&](int i) {
            int root = i;
            while (root != parent[root]) {
                root = parent[root];
            }
            int curr = i;
            while (curr != root) {
                int nxt = parent[curr];
                parent[curr] = root;
                curr = nxt;
            }
            return root;
        };
        
        // Disjoint Set Union `union` logic by size
        auto union_set = [&](int u, int v) {
            int root_u = find_set(u);
            int root_v = find_set(v);
            if (root_u != root_v) {
                if (sz[root_u] < sz[root_v]) swap(root_u, root_v);
                parent[root_v] = root_u;
                sz[root_u] += sz[root_v];
                max_size = max(max_size, sz[root_u]);
                num_components--;
            }
        };
        
        vector<pair<int, int>> deletions;
        deletions.reserve(N * 26);
        
        for (int u = 0; u < N; ++u) {
            int x = unique_masks[u];
            for (int i = 0; i < 26; ++i) {
                if ((x >> i) & 1) { // Check if the i-th bit is set
                    int d = x ^ (1 << i); // Clear the i-th bit to represent a deletion
                    deletions.push_back({d, u});
                    
                    // Search if this deletion mask naturally exists in unique_masks 
                    auto it = lower_bound(unique_masks.begin(), unique_masks.end(), d);
                    if (it != unique_masks.end() && *it == d) {
                        int v = distance(unique_masks.begin(), it);
                        union_set(u, v);
                    }
                }
            }
        }
        
        // Connect strings that can form valid replacements of each other.
        // Elements producing the exact same submask `d` will fall adjacently.
        sort(deletions.begin(), deletions.end());
        
        for (size_t i = 1; i < deletions.size(); ++i) {
            if (deletions[i].first == deletions[i - 1].first) {
                union_set(deletions[i].second, deletions[i - 1].second);
            }
        }
        
        return {num_components, max_size};
    }
};