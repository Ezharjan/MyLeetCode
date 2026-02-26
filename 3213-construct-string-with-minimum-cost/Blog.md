# Mastering String Construction with Aho Corasick Algorithm [#3213]

Have you ever encountered a Dynamic Programming problem that seemed straightforward at first glance but then refused to pass the time limits no matter how much you optimized it? I recently faced this exact scenario with a hard problem involving string construction and costs. It was a journey from a basic solution to a specialized string algorithm that is incredibly powerful for these types of challenges.

In this post, I want to share my experience solving the "Construct String with Minimum Cost" problem. We will look at why the intuitive approach failed and how we can use the Aho Corasick algorithm to solve it efficiently.

## The Challenge

We are given a `target` string and a collection of `words`, where each word has an associated cost. Our goal is to construct the `target` string by concatenating strings from our `words` collection. We want to find the minimum total cost to achieve this. If it is impossible to construct the target, we should return -1.

Imagine you have a target text like `abcdef`.
You have words like `abc` (cost 1), `d` (cost 1), `def` (cost 10), and `ef` (cost 5).
You could build `abcdef` by joining `abc` + `d` + `ef` for a total cost of 7.
Alternatively, if you used `abc` + `def`, the cost would be 11.
We need an algorithm that finds the cheapest path automatically.

## Attempt 1 The Intuitive Dynamic Programming

My immediate thought was to use Dynamic Programming (DP). The logic is quite natural for this type of problem.

We can define an array `dp` where `dp[i]` represents the minimum cost to construct the prefix of the target string ending at index `i`.
1.  **Base Case** `dp[0] = 0` (It costs nothing to build an empty string).
2.  **Transition** For every position `i` in the target, we look at all possible `words` that match the substring starting at `i`. If a word matches, we update the DP state for the position after that word.

To speed this up, I used a **Trie** (prefix tree). I inserted all the available words into a Trie. Then, for every position `i` in the `target`, I traversed the Trie to find all words that started at `target[i]`.



### Why It Failed

I submitted this solution and it passed the initial tests. However, it eventually hit a **Time Limit Exceeded (TLE)** error.

The problem arose from a specific type of test case. Imagine the target is a long string of 50,000 'a' characters, and our dictionary contains "a", "aa", "aaa", "aaaa", and so on.

In my approach, for every position in the target, I was traversing down the Trie. Because there were so many overlapping words, the inner loop of my algorithm was running too many times. In the worst case, the complexity approached O(N squared), where N is the length of the target. With N equal to 50,000, N squared is 2.5 billion operations. That is simply too slow for standard coding platforms which usually handle around 100 million operations per second.

## The Solution Aho Corasick Automaton

To solve this, we need to optimize how we search for words. We are essentially searching for multiple patterns (the `words`) inside a text (the `target`) simultaneously. The best tool for this job is the **Aho Corasick** algorithm.

You can think of Aho Corasick as a Trie combined with the logic of the KMP algorithm. It allows us to process the target string in a single pass while keeping track of all potential word matches.



### Key Concepts

1.  **The Trie** This remains the same. We build a tree of all our words.
2.  **Failure Links** This is the magic part. In a normal Trie, if you are at a node representing "abc" and the next letter in the target is 'd', but your Trie does not have a 'd' child, you have to go back to the root and start over. A failure link tells you "If you fail here, go to the longest suffix that is also a valid prefix in the Trie." For example, if you fail at "she", maybe you can jump to "he" and continue from there without restarting.
3.  **Output Links** Sometimes, a node in the Trie represents a word, but it is also part of a longer word. For example, if we have words "he" and "she", the node for "she" contains "he" as a suffix. Output links let us jump instantly to other completed words that end at the current position.

### The Algorithm Flow

1.  **Build the Trie** Insert all `words` and their costs into a Trie. If a word appears multiple times, keep the minimum cost.
2.  **Build Links** Use a Breadth First Search (BFS) to traverse the Trie and establish failure and output links for every node.
3.  **Run DP** Iterate through the `target` string one character at a time.
    * Move through the automaton states using the current character.
    * From the current state, traverse up the **Output Links** to find all words that end at the current position.
    * Update the `dp` array using the cost found in the Trie node plus the `dp` value from where that word started.

### Complexity Analysis

Let N be the length of the `target` and S be the sum of the lengths of all `words`.
* **Time Complexity** The construction of the automaton takes O(S) time. Processing the target takes roughly O(N multiplied by sqrt(S)). The sqrt(S) factor comes from the maximum number of words that can end at a single position (output link traversal). Given the constraints (N and S are up to 50,000), this fits comfortably within the time limit.
* **Space Complexity** O(S multiplied by the alphabet size), which is needed to store the Trie nodes and links.

## The C++ Implementation

Here is the complete solution using the concepts we discussed.

```cpp
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    // Definition of a Trie Node for Aho Corasick
    struct Node {
        int children[26];
        int fail;       // Points to the longest proper suffix that is also a prefix
        int output;     // Points to the nearest word-ending node in the failure chain
        int cost;       // Min cost to build the word ending at this node
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

    // Step 1 Insert words into the Trie
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
        // Store the minimum cost if the word appears multiple times
        trie[node].cost = min(trie[node].cost, c);
    }

    // Step 2 Build Failure and Output Links
    void buildAC() {
        queue<int> q;
        // Initialize the first level (children of root)
        for (int i = 0; i < 26; ++i) {
            if (trie[0].children[i] != -1) {
                q.push(trie[0].children[i]);
            } else {
                trie[0].children[i] = 0; // Route non-existent root edges back to root
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            int f = trie[u].fail;
            
            // Set Output Link
            // If the failure node is a word end, it becomes our output link
            // Otherwise, we inherit the failure node's output link
            if (trie[f].cost != INT_MAX) {
                trie[u].output = f;
            } else {
                trie[u].output = trie[f].output;
            }

            for (int i = 0; i < 26; ++i) {
                if (trie[u].children[i] != -1) {
                    int v = trie[u].children[i];
                    trie[v].fail = trie[f].children[i]; 
                    q.push(v);
                } else {
                    // Optimization Flatten the structure
                    // If child doesn't exist, point directly to the failure child
                    trie[u].children[i] = trie[f].children[i];
                }
            }
        }
    }

    int minimumCost(string target, vector<string>& words, vector<int>& costs) {
        trie.clear();
        trie.reserve(50005); // Reserve memory to prevent reallocations
        trie.emplace_back(0); // Create Root

        // Build the structure
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
            
            // Move to next state in O(1)
            node = trie[node].children[charIdx];

            // Traverse Output Links to find all matching words ending here
            int curr = node;
            while (curr != -1 && curr != 0) {
                if (trie[curr].cost != INT_MAX) {
                    int prevIdx = (i + 1) - trie[curr].len;
                    if (dp[prevIdx] != INT_MAX) {
                        // Update DP with the best cost found
                        long long newCost = (long long)dp[prevIdx] + trie[curr].cost;
                        if (newCost < dp[i + 1]) {
                             dp[i + 1] = newCost;
                        }
                    }
                }
                curr = trie[curr].output;
            }
        }

        return dp[n] == INT_MAX ? -1 : dp[n];
    }
};

```

## Summary

This problem is a fantastic example of why knowing advanced data structures pays off. The standard Trie approach is intuitive but fails under stress. By upgrading to the Aho Corasick automaton, we turn a quadratic time problem into a linear time problem.

The key takeaway is recognizing when you are doing repeated work. If you find yourself rescanning the same text or searching for many patterns at once, Aho Corasick is likely the tool you need.

I hope this explanation helps you understand the mechanics behind this hard problem. Happy coding!
