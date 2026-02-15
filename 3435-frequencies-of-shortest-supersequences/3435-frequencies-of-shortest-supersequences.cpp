class Solution {
public:
    vector<vector<int>> supersequences(vector<string>& words) {
        int present = 0;
        // Find all characters that exist in the given words
        for (const string& w : words) {
            present |= (1 << (w[0] - 'a'));
            present |= (1 << (w[1] - 'a'));
        }
        
        vector<int> char_map;
        int orig_char[26];
        for (int i = 0; i < 26; i++) {
            orig_char[i] = -1;
        }
        
        // Map available chars to a compressed 0 to K-1 index representation
        for (int i = 0; i < 26; i++) {
            if (present & (1 << i)) {
                orig_char[i] = char_map.size();
                char_map.push_back(i);
            }
        }
        
        int K = char_map.size();
        vector<int> adj(K, 0);
        
        // Build the graph using bitmask adjacency lists
        for (const string& w : words) {
            int u = orig_char[w[0] - 'a'];
            int v = orig_char[w[1] - 'a'];
            adj[v] |= (1 << u); // Represents an edge u -> v (v depends on u)
        }
        
        // is_DAG[mask] will be true if the subgraph induced by 'mask' is a DAG
        vector<bool> is_DAG(1 << K, false);
        is_DAG[0] = true; 
        int max_size = 0;
        
        for (int mask = 1; mask < (1 << K); mask++) {
            for (int i = 0; i < K; i++) {
                // If vertex 'i' is in the current 'mask'
                if ((mask & (1 << i)) != 0) {
                    // Check if 'i' has in-degree 0 within the induced subgraph
                    if ((adj[i] & mask) == 0) {
                        // If removing it yields a DAG, then the current mask is also a DAG
                        if (is_DAG[mask ^ (1 << i)]) {
                            is_DAG[mask] = true;
                            break;
                        }
                    }
                }
            }
            // Keep track of the maximum induced DAG size 
            if (is_DAG[mask]) {
                int sz = __builtin_popcount(mask);
                if (sz > max_size) {
                    max_size = sz;
                }
            }
        }
        
        vector<vector<int>> result;
        // Generate the final frequency arrays based off Optimal DAG masks
        for (int mask = 0; mask < (1 << K); mask++) {
            if (is_DAG[mask] && __builtin_popcount(mask) == max_size) {
                vector<int> freq(26, 0);
                for (int i = 0; i < K; i++) {
                    int c = char_map[i];
                    if ((mask & (1 << i)) != 0) {
                        freq[c] = 1;     // In max DAG -> Appears exactly once
                    } else {
                        freq[c] = 2;     // Excluded -> Appears exactly twice
                    }
                }
                result.push_back(freq);
            }
        }
        
        return result;
    }
};