#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <climits>

using namespace std;

// Trie Node structure to store string IDs
struct TrieNode {
    TrieNode* children[26];
    int id;           // Maps to the graph node ID (0..V-1). -1 if not a terminal node.
    bool isOriginal;  // True if this node marks the end of a string in 'original' array

    TrieNode() {
        fill(begin(children), end(children), nullptr);
        id = -1;
        isOriginal = false;
    }
};

class Solution {
    TrieNode* root;

    // Insert a string into the Trie
    void insert(const string& s, int id, bool isOrig) {
        TrieNode* curr = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!curr->children[idx]) {
                curr->children[idx] = new TrieNode();
            }
            curr = curr->children[idx];
        }
        curr->id = id;
        // If this specific insertion is from the 'original' list, mark it.
        // Note: A string might be in both lists; we merge the properties.
        if (isOrig) curr->isOriginal = true;
    }

    // Helper to clean up memory (good practice)
    void deleteTrie(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; ++i) deleteTrie(node->children[i]);
        delete node;
    }

public:
    long long minimumCost(string source, string target, vector<string>& original, vector<string>& changed, vector<int>& cost) {
        // 1. Map all unique strings to integer IDs
        unordered_map<string, int> strToId;
        int nextId = 0;
        
        auto getId = [&](const string& s) {
            if (strToId.find(s) == strToId.end()) {
                strToId[s] = nextId++;
            }
            return strToId[s];
        };

        for (const string& s : original) getId(s);
        for (const string& s : changed) getId(s);

        int numNodes = nextId;
        const long long INF = 1e14; // Sufficiently large value > max possible cost

        // 2. Build Adjacency Matrix
        vector<vector<long long>> dist(numNodes, vector<long long>(numNodes, INF));
        for (int i = 0; i < numNodes; ++i) dist[i][i] = 0;

        for (size_t i = 0; i < cost.size(); ++i) {
            int u = strToId[original[i]];
            int v = strToId[changed[i]];
            dist[u][v] = min(dist[u][v], (long long)cost[i]);
        }

        // 3. Floyd-Warshall Algorithm for All-Pairs Shortest Path
        for (int k = 0; k < numNodes; ++k) {
            for (int i = 0; i < numNodes; ++i) {
                if (dist[i][k] == INF) continue;
                for (int j = 0; j < numNodes; ++j) {
                    if (dist[k][j] == INF) continue;
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }

        // 4. Populate Trie
        root = new TrieNode();
        // Insert all mapped strings to ensure IDs are present in the Trie
        for (auto const& [s, id] : strToId) {
            insert(s, id, false);
        }
        // Mark the 'original' strings specifically as valid starting points
        for (const string& s : original) {
            insert(s, strToId[s], true);
        }

        // 5. Dynamic Programming
        int n = source.length();
        vector<long long> dp(n + 1, INF);
        dp[n] = 0;

        for (int i = n - 1; i >= 0; --i) {
            // Option 1: Character match (skip cost)
            if (source[i] == target[i]) {
                dp[i] = min(dp[i], dp[i+1]);
            }

            // Option 2: Substring transformation
            // Traverse both source and target in the Trie simultaneously
            TrieNode* nodeS = root;
            TrieNode* nodeT = root;

            for (int j = i; j < n; ++j) {
                int charS = source[j] - 'a';
                int charT = target[j] - 'a';

                nodeS = nodeS->children[charS];
                nodeT = nodeT->children[charT];

                // If either path breaks, we can't extend the substring further
                if (!nodeS || !nodeT) break;

                // Check validity:
                // 1. nodeS must be a valid 'original' string (we can start an op here)
                // 2. nodeT must be a valid graph node (we can end an op chain here)
                if (nodeS->isOriginal && nodeT->id != -1) {
                    int u = nodeS->id;
                    int v = nodeT->id;
                    if (dist[u][v] < INF) {
                        if (dp[j + 1] < INF) {
                            dp[i] = min(dp[i], dist[u][v] + dp[j + 1]);
                        }
                    }
                }
            }
        }
        
        // deleteTrie(root); // Cleanup if necessary

        return (dp[0] >= INF) ? -1 : dp[0];
    }
};