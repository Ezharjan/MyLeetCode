#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    struct Node {
        int children[26];
        int fail;       // Failure link: longest proper suffix that is a prefix of some word
        int output;     // Output link: nearest ancestor in failure chain that is a word end
        int cost;       // Minimum cost of the word ending at this node
        int len;        // Length of the word ending at this node

        Node(int l = 0) {
            fill(begin(children), end(children), -1);
            fail = 0;
            output = -1;
            cost = INT_MAX;
            len = l;
        }
    };

    vector<Node> trie;

    // Insert word into Trie
    void insert(const string& s, int c) {
        int node = 0;
        for (char ch : s) {
            int idx = ch - 'a';
            if (trie[node].children[idx] == -1) {
                trie[node].children[idx] = trie.size();
                trie.emplace_back(trie[node].len + 1);
            }
            node = trie[node].children[idx];
        }
        // Update cost (keep minimum if duplicate words exist)
        trie[node].cost = min(trie[node].cost, c);
    }

    // Build Failure and Output Links (Aho-Corasick construction)
    void buildAC() {
        queue<int> q;
        // Initialize depth 1 nodes
        for (int i = 0; i < 26; ++i) {
            if (trie[0].children[i] != -1) {
                q.push(trie[0].children[i]);
            } else {
                // Redirect non-existent edges to root for easier state transitions
                trie[0].children[i] = 0;
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            // Calculate output link
            int f = trie[u].fail;
            // If the failure node is a word end, it is our output link.
            // Otherwise, inherit the failure node's output link.
            if (trie[f].cost != INT_MAX) {
                trie[u].output = f;
            } else {
                trie[u].output = trie[f].output;
            }

            for (int i = 0; i < 26; ++i) {
                if (trie[u].children[i] != -1) {
                    int v = trie[u].children[i];
                    // Failure link for v is transition from u's fail
                    trie[v].fail = trie[f].children[i]; 
                    q.push(v);
                } else {
                    // Automaton optimization: direct edge to failure state
                    trie[u].children[i] = trie[f].children[i];
                }
            }
        }
    }

    int minimumCost(string target, vector<string>& words, vector<int>& costs) {
        trie.clear();
        trie.reserve(50005);
        trie.emplace_back(0); // Root

        for (int i = 0; i < words.size(); ++i) {
            insert(words[i], costs[i]);
        }

        buildAC();

        int n = target.size();
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = 0;

        int node = 0;
        for (int i = 0; i < n; ++i) {
            int charIdx = target[i] - 'a';
            
            // Move in the AC automaton
            // Because of buildAC lines 73-77, children[charIdx] always points to a valid state
            node = trie[node].children[charIdx];

            // Traverse the "Output Link" chain to find all words ending at this position
            // We check the current node, then follow output links up
            int curr = node;
            
            while (curr != -1 && curr != 0) {
                if (trie[curr].cost != INT_MAX) {
                    int prevIdx = (i + 1) - trie[curr].len;
                    if (dp[prevIdx] != INT_MAX) {
                         // Check for overflow before adding
                        if (dp[prevIdx] + trie[curr].cost < dp[i + 1]) {
                             dp[i + 1] = dp[prevIdx] + trie[curr].cost;
                        }
                    }
                }
                // Move to next word-ending node in the failure chain
                // Note: If current node has no valid cost, we might still need to jump to 'output'
                // But our logic above checks trie[curr].cost. We need to make sure we jump correctly.
                // The 'output' link points to the *next* word, so we rely on that.
                // However, 'curr' itself might have been a non-word node reached by 'children'.
                // So the loop should strictly follow 'output' after checking 'curr' once?
                // Actually, standard optimization is: check if curr is word, then loop output.
                // My logic: 'output' points to strictly proper suffix word.
                // So we check 'curr', then loop 'output'.
                
                curr = trie[curr].output;
            }
        }

        return dp[n] == INT_MAX ? -1 : dp[n];
    }
};