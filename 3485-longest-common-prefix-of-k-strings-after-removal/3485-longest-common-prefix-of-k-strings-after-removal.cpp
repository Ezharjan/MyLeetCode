#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
    struct Node {
        int children[26];
        int count = 0;      // Number of words passing through this prefix
        int depth = 0;      // Length of the prefix
        int max_sub = 0;    // Max depth in this subtree with count >= k
        
        // Tracking the top 2 max_sub values among this node's children
        int max1_val = 0;
        int max1_c = -1;    // Character index of the child giving max1_val
        int max2_val = 0;
        
        int ans = 0;        // Final precalculated answer if a word ends here
        
        Node() {
            for(int i = 0; i < 26; ++i) children[i] = -1;
        }
    };
    
    vector<Node> trie;

    // Bottom-up traversal to calculate the best sub-depths for unmodified paths
    void dfs1(int u, int k) {
        if (trie[u].count < k) {
            trie[u].max_sub = 0;
            return;
        }
        
        trie[u].max_sub = trie[u].depth;
        
        for (int i = 0; i < 26; ++i) {
            int c = trie[u].children[i];
            if (c != -1) {
                dfs1(c, k);
                int val = trie[c].max_sub;
                
                trie[u].max_sub = max(trie[u].max_sub, val);
                
                // Update the top 2 highest values from children branches
                if (val > trie[u].max1_val) {
                    trie[u].max2_val = trie[u].max1_val;
                    trie[u].max1_val = val;
                    trie[u].max1_c = i;
                } else if (val > trie[u].max2_val) {
                    trie[u].max2_val = val;
                }
            }
        }
    }

    // Top-down traversal pushing answers to the leaves
    void dfs2(int u, int path_max, int off_path_max, int k) {
        // If a word ends exactly here, its path dropped by 1. 
        // It's valid on-path only if original count >= k + 1.
        int cur_path_max = max(path_max, trie[u].count >= k + 1 ? trie[u].depth : 0);
        
        // Answer is the max of valid path ancestors, off-path branches above us, 
        // or any branches purely underneath us (since the word stops at 'u' and doesn't take them).
        trie[u].ans = max({cur_path_max, off_path_max, trie[u].max1_val});
        
        for (int i = 0; i < 26; ++i) {
            int c = trie[u].children[i];
            if (c != -1) {
                int next_off = off_path_max;
                
                // If we walk down the child that provides max1_val, 
                // the best alternative "off-path" option from this junction is max2_val.
                if (i == trie[u].max1_c) {
                    next_off = max(next_off, trie[u].max2_val);
                } else {
                    next_off = max(next_off, trie[u].max1_val);
                }
                
                dfs2(c, cur_path_max, next_off, k);
            }
        }
    }

public:
    vector<int> longestCommonPrefix(vector<string>& words, int k) {
        int n = words.size();
        
        // Edge case: if removing 1 leaves us with less than 'k' strings
        if (n - 1 < k) {
            return vector<int>(n, 0);
        }

        // Reserve memory space to avoid dynamic reallocation overhead (Max sum of lengths is 10^5)
        trie.reserve(100010);
        trie.emplace_back(); // Initialize Root

        vector<int> end_nodes(n);
        
        // 1. Build the Trie
        for (int i = 0; i < n; ++i) {
            int curr = 0;
            trie[curr].count++;
            
            for (char c : words[i]) {
                int idx = c - 'a';
                if (trie[curr].children[idx] == -1) {
                    trie[curr].children[idx] = trie.size();
                    trie.emplace_back();
                    trie.back().depth = trie[curr].depth + 1;
                }
                curr = trie[curr].children[idx];
                trie[curr].count++;
            }
            end_nodes[i] = curr; // Save where the word concludes for $O(1)$ lookups
        }

        // 2. Precalculate configurations
        dfs1(0, k);
        dfs2(0, 0, 0, k);

        // 3. Collect queries
        vector<int> res(n);
        for (int i = 0; i < n; ++i) {
            res[i] = trie[end_nodes[i]].ans;
        }

        return res;
    }
};