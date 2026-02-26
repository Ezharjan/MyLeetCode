# Demystifying Frequencies of Shortest Supersequences A Friendly Walkthrough [#3435]

Hello everyone! Welcome to another coding journey. Today I am thrilled to share my thoughts and a detailed guide on an incredibly fascinating string and graph theory problem. We are going to explore how to find the frequencies of characters in the shortest common supersequences. 

This problem might seem intimidating at first glance, but do not worry! We will break it down piece by piece together. By the end of this guide, you will have a solid grasp of the concepts, and you will see how beautifully strings, graphs, and bitwise operations can dance together. I am sharing this so that anyone, regardless of their current experience level, can understand the logic and add this tool to their problem solving arsenal.

Let us dive right in!

### Understanding the Challenge Before Us

Imagine you are given a list of words. Each word in this list is exactly two letters long. Your goal is to create a master string, known as a supersequence. This master string must contain every single two letter word from your list as a subsequence. 

A subsequence means that the letters of the word appear in the master string in the same relative order, but they do not necessarily have to be right next to each other. For example, if your word is "ab", the master string could be "ab", "acb", or "xayb". In all these cases, 'a' comes before 'b'.

The catch is that we want the shortest possible master string. And to make things even more interesting, we do not just want to build the string itself. The problem asks us to figure out the exact frequency of every lowercase English letter in that shortest master string. Since there could be multiple valid shortest strings that are not just simple rearrangements of each other, we need to return a list containing the frequency profiles of all these unique shortest master strings.

The problem guarantees that all strings in the input array will together use no more than 16 unique lowercase letters. This specific constraint is a massive hint about how we can approach the problem, which we will explore shortly.

### Visualizing Strings as a Network of Dependencies

Whenever you see a problem where certain things must happen before other things, your mind should immediately jump to networks or graphs. In our case, a two letter word like "ab" is basically a rule. It says that an 'a' must appear before a 'b' in our master string. 



We can visualize every unique letter as a dot on a whiteboard, which we call a node or a vertex. Every two letter word "uv" becomes an arrow pointing from node 'u' to node 'v'. This arrow is a directed edge representing a dependency.

Let us think about what happens when we try to build our master string.
* If a letter only appears once in our master string, its position is completely fixed relative to the letters that come before it and after it.
* If we have a dependency arrow pointing from 'a' to 'b', and another from 'b' to 'a', we have a cycle. We cannot possibly satisfy both rules if 'a' and 'b' only appear once! To fix a cycle, at least one of the letters must appear more than once. For example, if we have "ab" and "ba", we could build "aba". Here, 'a' appears twice and 'b' appears once.

This realization is the absolute core of our method. Any letter that appears exactly one time in our master string must belong to a set of letters that have no cyclic dependencies among themselves. 

### The Magic of Acyclic Subgraphs

In graph theory, a collection of nodes with directed arrows that never form a loop is called a Directed Acyclic Graph. 



Our goal is to find the shortest master string. The length of our master string depends directly on how many times each letter needs to appear. 
* Letters that form a Directed Acyclic Graph can appear exactly one time.
* Letters that are involved in loops, or letters that form a word with themselves like "aa", must appear exactly two times. It turns out that two times is always enough to satisfy any complex web of two letter dependencies.

So, to make our master string as short as possible, we need to maximize the number of letters that appear only once. This means we need to find the largest possible subset of letters that forms a Directed Acyclic Graph! 

If we have a universe of V unique characters present in our input, and we find a valid acyclic subset U, the characters in subset U will have a frequency of 1. The remaining characters, which are the ones in V but not in U, will have a frequency of 2. 

If there are multiple acyclic subsets that share the maximum possible size, each of those subsets will give us one valid frequency profile for our answer. 

### Mapping Letters Without Subtraction

Before we get into the heavy lifting, we need a way to map our letters to numbers. Usually, programmers subtract the character 'a' from a letter to get a number from 0 to 25. However, there is a very neat trick using the modulus operator that does exactly the same thing for lowercase letters!

The ASCII value of 'a' is 97. 
If we take 97 modulus 97, we get 0.
The ASCII value of 'b' is 98.
If we take 98 modulus 97, we get 1.
The ASCII value of 'z' is 122.
If we take 122 modulus 97, we get 25.

This means we can use `character % 97` to map any lowercase letter to an index between 0 and 25. We will use this trick in our code to find which characters are actually present in the given words. Since the problem states there are at most 16 unique characters, we will compress these present characters into a smaller index range from 0 to K, where K is the number of unique characters.

### Enter the World of Bitmasks

Because K is at most 16, we have a very small number of unique characters to deal with. Whenever you see a constraint around 16 or 20, bitmasking is usually a fantastic tool to use. 



A bitmask is just an integer where we treat the binary bits as true or false flags. If the bit at position 3 is a 1, it means the 3rd character is included in our current subset. If it is a 0, it is not included. 

Since we have K characters, we can represent every possible subset of characters using integers from 0 up to 2 raised to the power of K. For 16 characters, 2 raised to the power of 16 is 65536, which is a very small number for modern computers to process.

We will create an adjacency list to represent our graph. But instead of arrays of neighbors, we will use an integer mask for each node. The integer at `adj[v]` will have a bit set to 1 for every node `u` that points to `v`. This means `v` depends on `u`.

### Building the Solution Step by Step

Now we need a way to check which subsets of nodes form a valid Directed Acyclic Graph. We will use a technique that builds solutions for larger subsets based on the solutions of smaller subsets. 

We will maintain a list of boolean values called `is_DAG`. The value `is_DAG[mask]` will be true if the subset represented by `mask` is a valid acyclic graph, and false otherwise. The empty set, represented by mask 0, is trivially an acyclic graph.

For every other mask from 1 up to the maximum possible mask, we want to determine its validity. How do we know if a set of nodes is acyclic? A directed graph is acyclic if and only if there is at least one node in the graph that has zero incoming arrows from other nodes in the same graph.

So, for a given mask, we look at every node `i` that is included in the mask. We check if node `i` has any incoming dependencies from other nodes that are also in the mask. We do this by taking the bitwise AND of `adj[i]` and `mask`. If the result is 0, it means node `i` has no incoming arrows from within the current subset!

If we find such a node `i`, we can conceptually remove it from our subset. We then look at the remaining subset. If the remaining subset was already proven to be an acyclic graph, then adding node `i` back also creates an acyclic graph. We can mark `is_DAG[mask]` as true.

As we evaluate all these subsets, we keep track of the population count, which is the number of bits set to 1 in the mask. We want to find the maximum population count among all valid acyclic subsets.

### Constructing the Final Frequencies

After we finish evaluating all subsets, we know the maximum size an acyclic subset can be. We simply loop through all our subsets one last time. 

Whenever we find a mask that is a valid acyclic graph AND its population count exactly matches our maximum size, we have found a recipe for a shortest master string!

We create a frequency array of size 26 initialized to zero. 
* We look at all the unique characters that were present in the original input. 
* If a character is included in this perfect mask, it means it belongs to the acyclic graph and only needs to appear 1 time. We set its frequency to 1.
* If a character was present in the input but is excluded from this mask, it means it must be part of a cycle or needs to be repeated. We set its frequency to 2.
* Characters that never appeared in the input words at all remain at frequency 0.

We add this frequency array to our result list. Once we have checked all masks, our result list contains all the unique frequency profiles for the shortest common supersequences. 

### The Code Implementation

Here is a C++ implementation of the concepts we just discussed. I have written this carefully to be readable and to cleanly express the logic without relying on unnecessary language quirks.

```cpp
class Solution {
public:
    vector<vector<int>> supersequences(vector<string>& words) {
        int present = 0;
        
        for (const string& w : words) {
            int first_char = w[0] % 97; 
            int second_char = w[1] % 97;
            present |= (1 << first_char);
            present |= (1 << second_char);
        }
        
        vector<int> char_map;
        int orig_char[26];
        
        for (int i = 0; i < 26; i = i + 1) {
            orig_char[i] = 999; 
        }
        
        for (int i = 0; i < 26; i = i + 1) {
            if (present & (1 << i)) {
                orig_char[i] = char_map.size();
                char_map.push_back(i);
            }
        }
        
        int K = char_map.size();
        vector<int> adj(K, 0);
        
        for (const string& w : words) {
            int u = orig_char[w[0] % 97];
            int v = orig_char[w[1] % 97];
            adj[v] |= (1 << u); 
        }
        
        vector<bool> is_DAG(1 << K, false);
        is_DAG[0] = true; 
        int max_size = 0;
        
        for (int mask = 1; mask < (1 << K); mask = mask + 1) {
            for (int i = 0; i < K; i = i + 1) {
                
                if ((mask & (1 << i)) != 0) {
                    
                    if ((adj[i] & mask) == 0) {
                        
                        if (is_DAG[mask ^ (1 << i)]) {
                            is_DAG[mask] = true;
                            break;
                        }
                    }
                }
            }
            
            if (is_DAG[mask]) {
                int sz = __builtin_popcount(mask);
                if (sz > max_size) {
                    max_size = sz;
                }
            }
        }
        
        vector<vector<int>> result;
        
        for (int mask = 0; mask < (1 << K); mask = mask + 1) {
            if (is_DAG[mask] && __builtin_popcount(mask) == max_size) {
                vector<int> freq(26, 0);
                for (int i = 0; i < K; i = i + 1) {
                    int c = char_map[i];
                    if ((mask & (1 << i)) != 0) {
                        freq[c] = 1;     
                    } else {
                        freq[c] = 2;     
                    }
                }
                result.push_back(freq);
            }
        }
        
        return result;
    }
};

```

### Analyzing Our Approach

It is always important to understand the resources our logic will consume. Let us talk about time and space complexity in clear terms.

**Time Complexity:**
The heavy lifting of our solution happens in the nested loops where we evaluate every possible subset. There are 2 raised to the power of K total subsets. For each subset, we might check up to K characters to see if they can be safely removed.
This gives us a number of operations proportional to K multiplied by 2 raised to the power of K. Since the problem guarantees that K is at most 16, 2 raised to the power of 16 is 65536. Multiplying this by 16 gives roughly 1 million operations. This is an incredibly tiny number for a computer, meaning this logic will execute lightning fast. The other steps, like iterating through the words, take time proportional to the number of words, which is very small. Overall, this is a solid approach that performs well within the limits.

**Space Complexity:**
We need to store the validity of every possible subset. We use a boolean list of size 2 raised to the power of K for this. For K equals 16, this list takes up 65536 boolean values, which is practically negligible memory. The adjacency list and frequency arrays take minimal space. The space required to hold the final results depends on how many maximum acyclic subsets exist, but it will easily fit in memory.

### Final Thoughts

We have successfully navigated through a complex problem by breaking it down into smaller, manageable ideas. We translated a string sequence requirement into a graph dependency network. We then realized that maximizing single occurrence characters is equivalent to finding the largest acyclic subgraphs. We used clever bitmasking to represent these graphs and built our solution logically.

I hope this detailed walkthrough has been illuminating and helpful for your own learning journey. Sharing knowledge like this is a wonderful way for us all to grow as a community. Keep practicing, keep analyzing, and most importantly, keep having fun solving these brilliant puzzles!
