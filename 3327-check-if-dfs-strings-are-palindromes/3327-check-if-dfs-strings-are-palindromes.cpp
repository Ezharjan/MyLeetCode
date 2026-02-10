#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
    // Global variables to store graph and traversal info
    vector<vector<int>> adj;
    string dfsStr;
    vector<int> rangeL, rangeR; // Stores the start and end indices in dfsStr for each node's subtree

    // Depth First Search to build the post-order string and record ranges
    void dfs(int u, const string& s) {
        // The start index of the subtree's string in the global dfsStr
        // is the current length (before processing this node or its children)
        // Note: For post-order (children then root), the 'start' is effectively 
        // the beginning of the first child processed.
        int start = dfsStr.length();
        
        // Visit children in increasing order of their indices
        for (int v : adj[u]) {
            dfs(v, s);
        }
        
        // Append current node's character
        dfsStr += s[u];
        
        // Record the range for this node's subtree
        rangeL[u] = start;
        rangeR[u] = dfsStr.length() - 1;
    }

public:
    vector<bool> findAnswer(vector<int>& parent, string s) {
        int n = parent.size();
        adj.assign(n, vector<int>());
        
        // Build the adjacency list
        // Since we iterate i from 1 to n-1, children are added in increasing order.
        // This satisfies the requirement: "Iterate over each child y of x in increasing order".
        for (int i = 1; i < n; ++i) {
            adj[parent[i]].push_back(i);
        }
        
        // Initialize structures
        dfsStr.reserve(n);
        rangeL.resize(n);
        rangeR.resize(n);
        
        // Perform DFS to build the string and map ranges
        dfs(0, s);
        
        // --- Manacher's Algorithm ---
        
        // Transform dfsStr into t to handle even/odd length palindromes uniformly
        // e.g., "aba" -> "^#a#b#a#$"
        string t = "^#";
        for (char c : dfsStr) {
            t += c;
            t += '#';
        }
        t += "$";
        
        int m = t.length();
        vector<int> p(m, 0); // p[i] will store the radius of the palindrome at center i
        int l = 0, r = 0; // Current palindrome boundaries
        
        for (int i = 1; i < m - 1; ++i) {
            int i_mirror = l + (r - i);
            
            if (r > i) {
                p[i] = min(r - i, p[i_mirror]);
            }
            
            // Expand palindrome centered at i
            while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) {
                p[i]++;
            }
            
            // Update boundaries if palindrome expands beyond r
            if (i + p[i] > r) {
                l = i - p[i];
                r = i + p[i];
            }
        }
        
        // --- Answer Queries ---
        
        vector<bool> answer(n);
        for (int i = 0; i < n; ++i) {
            int L = rangeL[i];
            int R = rangeR[i];
            int len = R - L + 1;
            
            // Calculate the center index in the transformed string t
            // Mapping: index k in dfsStr corresponds to index 2*k + 2 in t
            // The center of substring dfsStr[L...R] corresponds to (indices_sum / 2)
            // Center in t = ( (2*L + 2) + (2*R + 2) ) / 2 = L + R + 2
            int center = L + R + 2;
            
            // In Manacher's, p[center] represents the length of the longest palindrome
            // in the original string centered at that position.
            // We check if the longest palindrome covers our required length.
            if (p[center] >= len) {
                answer[i] = true;
            } else {
                answer[i] = false;
            }
        }
        
        return answer;
    }
};