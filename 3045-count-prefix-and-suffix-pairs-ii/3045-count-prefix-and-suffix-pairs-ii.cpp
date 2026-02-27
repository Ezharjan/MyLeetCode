#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    long long countPrefixSuffixPairs(vector<string>& words) {
        // Parallel arrays strictly managing Trie Nodes to keep memory tight and fast
        vector<int> count;
        vector<int> first_child;
        vector<int> next_sibling;
        vector<int> val;
        
        // Helper lambda to dynamically spawn a new node
        auto add_node = [&](int v) {
            count.push_back(0);
            first_child.push_back(-1);
            next_sibling.push_back(-1);
            val.push_back(v);
        };
        
        // Initialize the root of the Trie
        add_node(-1);
        
        long long ans = 0;
        
        for (const string& w : words) {
            int u = 0; // Start at the root for every word
            int n = w.length();
            
            for (int i = 0; i < n; ++i) {
                // Determine our pair of characters from the front and the back
                int c1 = w[i] - 'a';
                int c2 = w[n - 1 - i] - 'a';
                
                // Represent the pair of chars uniquely as a base-26 integer
                int v = c1 * 26 + c2;
                
                // Find if the transitioning child node already exists
                int child = -1;
                for (int edge = first_child[u]; edge != -1; edge = next_sibling[edge]) {
                    if (val[edge] == v) {
                        child = edge;
                        break;
                    }
                }
                
                // If the path doesn't exist, append a new node mapping
                if (child == -1) {
                    child = count.size();
                    add_node(v);
                    next_sibling[child] = first_child[u];
                    first_child[u] = child;
                }
                
                // Traverse down
                u = child;
                
                // If an earlier word ended at this particular pair-path, add it to our valid pair tally
                ans += count[u];
            }
            // Mark the word as fully formed and completed at the final node path
            count[u]++;
        }
        
        return ans;
    }
};