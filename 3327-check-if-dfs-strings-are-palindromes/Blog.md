# Mastering Palindromes on Trees using DFS and Manacher [#3327]

Welcome fellow coding enthusiasts! Today we are going to explore a fascinating problem that beautifully combines graph theory with string processing. It is always exciting when we get to mix two different domains of computer science into a single efficient solution.

The problem we are looking at involves determining if specific traversal strings within a tree form palindromes. While it might sound complex at first glance, we can break it down into manageable pieces and solve it quite elegantly.

## Understanding the Problem

Let us start by clearly defining what we are trying to solve. We are given a tree rooted at node 0. Each node in this tree has a character assigned to it.

We need to perform a specific type of Depth First Search (DFS) traversal for every single node in the tree. For any given node `x`, the rules for generating its string are as follows:

1.  First, we recursively visit all children of `x`. An important detail is that we must visit the children in increasing order of their node numbers.
2.  After visiting all the children, we append the character assigned to node `x` itself to the end of the string.

This is essentially a **post-order traversal**.

Our goal is to return a boolean result for every node. For each node `i`, we generate its specific traversal string. If that string reads the same forwards and backwards (a palindrome), the answer for node `i` is true. Otherwise, it is false.

## The Challenge with the Naive Approach

If we simply follow the instructions literally, we might be tempted to write a recursive function that builds a new string for every single node call.

Let us think about the computational cost of that.

In the worst case scenario, our tree could look like a straight line (a linked list). Node 0 is the parent of node 1, node 1 is the parent of node 2, and so on.
* The string for the last node has length 1.
* The string for the second to last node has length 2.
* The string for the root node has length N.

If we construct these strings individually, the total work involves summing the lengths of all strings, which leads to a time complexity of O(N squared). With N being as large as 100,000, an N-squared solution requires roughly 10 billion operations. This will almost certainly result in a Time Limit Exceeded error.

We need a more efficient way to look at the data. We need a solution that operates closer to O(N), or linear time.

## The Key Insight Linearization

The crucial observation to make here is about the nature of the DFS traversal.

When we perform a DFS on the root (node 0), we are visiting every node in the tree. Because of the recursive nature of DFS, the traversal sequence for any specific subtree forms a **contiguous block** within the global traversal of the entire tree.

This means we do not need to generate N separate strings. We can generate just **one** massive string that represents the post-order traversal of the entire tree.

Once we have this global string, let us call it `dfsStr`, the string corresponding to any node `u` is simply a substring of `dfsStr`.

To make this work, we need to record two pieces of information for every node during our global DFS:
1.  **Start Index:** The index in `dfsStr` where the traversal of the subtree of `u` begins.
2.  **End Index:** The index in `dfsStr` where the traversal of the subtree of `u` ends.

With these indices, the problem transforms completely. We are no longer traversing trees. We are now asking: **Given a static string and a list of ranges [L, R], which of these substrings are palindromes?**

## Choosing the Right Tool Manacher Algorithm

Now that we have reduced the problem to checking if substrings are palindromes, how do we do this efficiently?

If we simply extract the substring and check it character by character, we are back to O(N squared) in the worst case. We need something faster.

We could use Hashing (like Rolling Hash) to check for palindromes in O(1) time after O(N) preprocessing, but hashing involves probabilities of collision.

A robust, deterministic, and beautiful approach is **Manacher's Algorithm**.

Manacher's Algorithm allows us to find the longest palindrome centered at *every* position in a string in linear O(N) time.

### How Manacher's Algorithm Helps Us

Manacher's algorithm produces an array, often called `P` or `radius`. For every center position in the string, `P[i]` tells us the radius of the longest palindrome centered there.

If we want to know if the substring from index `L` to index `R` is a palindrome, we just need to check two things:
1.  Find the center of the substring `[L, R]`.
2.  Check if the palindrome radius at that center is large enough to cover the entire length `R - L + 1`.

If the radius is equal to or greater than the length of the substring, then the entire substring is a palindrome.

## Step by Step Implementation Details

Let us walk through the implementation logic.

### 1. Building the Adjacency List
The input gives us a parent array. To perform DFS efficiently, we need to know the children of each node. We can build an adjacency list where `adj[u]` contains all the children of `u`.
* *Note:* The problem requires us to visit children in increasing order of their indices. When we iterate from 1 to N, we naturally encounter children in increasing order, so simply appending them to the parent's list works perfectly.

### 2. The DFS Traversal
We create a global string `dfsStr`. We also create arrays `rangeL` and `rangeR` to store the start and end positions.
Inside the recursive function for node `u`:
* Record `rangeL[u]` as the current length of `dfsStr`.
* Recursively call the function for all children of `u`.
* Append `s[u]` to `dfsStr`.
* Record `rangeR[u]` as the current length of `dfsStr` (which is the index of the character we just added).

### 3. Preprocessing for Manacher
Palindromes can be of odd length (like "aba") or even length (like "abba"). Manacher's algorithm handles this by transforming the string to ensure all palindromes are effectively odd length.
We insert a special character (like `#`) between every character of the original string, and special boundary characters at the ends.
* Example: `aba` becomes `^ # a # b # a # $`

### 4. Running Manacher's Algorithm
We iterate through this transformed string to build the radius array `P`.
* We maintain a `center` and a `right` boundary of the rightmost palindrome found so far.
* For a new index `i`, we can initialize `P[i]` using the mirror property around the `center`.
* We then attempt to expand the palindrome at `i` as far as possible.
* If our palindrome at `i` extends beyond `right`, we update `center` and `right`.

### 5. Answering the Queries
Finally, we iterate through all nodes from 0 to N.
* Retrieve `L` and `R` from our `rangeL` and `rangeR` arrays.
* Calculate the length `len = R - L + 1`.
* Map the center of the substring `[L, R]` to its corresponding index in the transformed Manacher string. The formula for the center index in the transformed string is typically `L + R + 2`.
* Check if `P[center] >= len`. If it is, `answer[i]` is true.

## Complexity Analysis

Let us look at the efficiency of this approach.

**Time Complexity:**
1.  Building the adjacency list takes O(N).
2.  The initial DFS traversal visits every node once, taking O(N).
3.  Manacher's Algorithm processes the string in linear time, taking O(N).
4.  The final loop runs N times, doing constant O(1) math checks in each iteration.
* **Total Time:** O(N). This is highly efficient and optimal for the given constraints.

**Space Complexity:**
1.  The adjacency list takes O(N).
2.  The recursion stack for DFS can go up to O(N) in the worst case.
3.  The global strings and arrays take O(N).
* **Total Space:** O(N).

## The Solution Code

Here is the complete C++ solution implementing the logic described above.

```cpp
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
    // Global variables to store graph and traversal info
    vector<vector<int>> adj;
    string dfsStr;
    // Arrays to store the start and end indices in dfsStr for each node's subtree
    vector<int> rangeL, rangeR; 

    // Depth First Search to build the post-order string and record ranges
    void dfs(int u, const string& s) {
        // The start index of the subtree's string in the global dfsStr
        // is the current length of the string before processing this node's subtree.
        int start = dfsStr.length();
        
        // Visit children in increasing order of their numbers
        for (int v : adj[u]) {
            dfs(v, s);
        }
        
        // Append current node's character (Post-order)
        dfsStr += s[u];
        
        // Record the range for this node's subtree
        // The subtree string is dfsStr[rangeL[u] ... rangeR[u]]
        rangeL[u] = start;
        rangeR[u] = dfsStr.length() - 1;
    }

public:
    vector<bool> findAnswer(vector<int>& parent, string s) {
        int n = parent.size();
        adj.assign(n, vector<int>());
        
        // Build the adjacency list
        // Since we iterate i from 1 to n, children are added in increasing order.
        // This satisfies the requirement naturally.
        for (int i = 1; i < n; ++i) {
            adj[parent[i]].push_back(i);
        }
        
        // Initialize structures
        dfsStr.reserve(n);
        rangeL.resize(n);
        rangeR.resize(n);
        
        // Perform DFS to build the string and map ranges
        dfs(0, s);
        
        // --- Manacher's Algorithm Section ---
        
        // Transform dfsStr into t to handle even and odd length palindromes uniformly
        // Example: "aba" becomes "^#a#b#a#$"
        string t = "^#";
        for (char c : dfsStr) {
            t += c;
            t += '#';
        }
        t += "$";
        
        int m = t.length();
        // p[i] will store the radius of the palindrome at center i in transformed string
        vector<int> p(m, 0); 
        int l = 0, r = 0; // Current palindrome boundaries
        
        for (int i = 1; i < m - 1; ++i) {
            int i_mirror = l + (r - i);
            
            if (r > i) {
                p[i] = min(r - i, p[i_mirror]);
            }
            
            // Attempt to expand palindrome centered at i
            while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) {
                p[i]++;
            }
            
            // Update boundaries if palindrome expands beyond r
            if (i + p[i] > r) {
                l = i - p[i];
                r = i + p[i];
            }
        }
        
        // --- Answer Queries Section ---
        
        vector<bool> answer(n);
        for (int i = 0; i < n; ++i) {
            int L = rangeL[i];
            int R = rangeR[i];
            int len = R - L + 1;
            
            // Calculate the center index in the transformed string t
            // Mapping: index k in dfsStr corresponds to index 2*k + 2 in t
            // The center of substring dfsStr[L...R] is (L + R) / 2
            // In the transformed string, this maps to L + R + 2
            int center = L + R + 2;
            
            // In Manacher's logic, p[center] represents the length of the 
            // longest palindrome in the original string centered at that position.
            // We check if this longest palindrome covers our required length.
            if (p[center] >= len) {
                answer[i] = true;
            } else {
                answer[i] = false;
            }
        }
        
        return answer;
    }
};

```

## Closing Thoughts

This problem is a wonderful example of how changing your perspective on the data can simplify the solution. By realizing that the recursive tree traversal can be flattened into a single linear string, we unlocked the ability to use powerful string processing tools like Manacher's Algorithm.

Instead of struggling with a complex tree problem, we solved a classic palindrome problem. I hope this explanation helps you understand not just the solution, but the thought process behind optimizing it.

Happy coding and keep exploring!
