# Unlocking the Mystery of Grouping Strings by Adding Deleting and Replacing Characters [#2157]

Hello everyone in the programming community! Welcome to another deep dive into algorithm exploration. Today, I would love to share my thoughts and a detailed approach to an incredibly fascinating string manipulation and grouping challenge. If you enjoy puzzles that require a blend of data structures and clever state representations, you will find this problem thoroughly enjoyable.

We are going to walk through the logic step by step, ensuring that whether you are a beginner or a seasoned coder, the underlying concepts will be clear and approachable. My goal is to present a modest, friendly, and highly accurate guide that you can read through comfortably. Let us dive right in!

***

### Understanding the Core Problem

Imagine we are given a large list of words. Each word is made up exclusively of lowercase English letters, and there is a special rule regarding these words: no single letter ever repeats within the same word. For instance, you might see "abc" or "xyz", but you will never see "hello" because the letter 'l' appears twice.

Our task is to organize these words into separate groups. But how do we decide which words belong together? Two words are considered connected if you can transform one into the other using exactly one of the following three operations:

* **Adding exactly one letter:** For example, if you have the word "a", you can add the letter 'b' to get the set of letters found in "ab". Therefore, "a" and "ab" are connected.
* **Deleting exactly one letter:** This is simply the reverse of adding. If you have the word "abc" and you delete the letter 'c', you are left with the letters "ab". Thus, "abc" and "ab" are connected.
* **Replacing exactly one letter:** If you have the word "abc" and you replace 'c' with 'd', you obtain "abd". This means "abc" and "abd" are connected.

If word A connects to word B, and word B connects to word C, then all three words belong to the exact same group. We need to find out two specific numbers: the total number of distinct groups we can form, and the size of the largest single group.

***

### The Challenge of Scale

Before we jump into coding, we need to carefully consider the boundaries of our challenge. We could have up to twenty thousand words in our list, and each word can have up to twenty six characters. 

If we try to compare every single word against every other word to check if they connect, the number of comparisons would grow incredibly large. Comparing twenty thousand items against twenty thousand items requires hundreds of millions of operations. This naive approach would be far too slow. We need a method that avoids comparing every pair directly. We need a way to look at a word, immediately generate the possible connections, and swiftly check if those connections exist in our given list.

***

### Transforming Strings into Numbers with Bitmasks

The key to solving this problem gracefully lies in how we represent our words. Since every word consists of unique lowercase English letters, and there are exactly twenty six letters in the alphabet, we can represent each word as a binary number, commonly known as a bitmask.

Think of a bitmask as a row of twenty six light switches. Each switch corresponds to a letter of the alphabet. If a word contains the letter 'a', we turn on the first switch. If it contains 'b', we turn on the second switch, and so on.

Let us look at an example:
* The word "a" turns on only the switch for 'a'. Its binary representation is `...00001`. In decimal, this is simply the number 1.
* The word "b" turns on only the switch for 'b'. Its binary representation is `...00010`. In decimal, this is the number 2.
* The word "ab" turns on the switches for both 'a' and 'b'. Its binary representation is `...00011`. In decimal, this is the number 3.

Why is this transformation so powerful? Because mathematical operations on numbers are incredibly fast for computers. Instead of manipulating strings and characters, we can use fast bitwise operations. For example, to simulate deleting a character from a word, we just turn off the corresponding light switch in our number.

In our implementation, to convert a character to a specific bit position safely, we can take the character and perform a bitwise AND operation with the number 31. The lowercase letter 'a' has an integer value of 97. If you evaluate 97 AND 31, the result is 1. The letter 'z' has an integer value of 122. If you evaluate 122 AND 31, the result is 26. This neatly maps our alphabet to positions 1 through 26 without needing subtraction.

***

### Managing Groups: The Disjoint Set Union Data Structure

Now that we have a fast way to represent words, we need a way to group them. A highly suitable tool for this job is the Disjoint Set Union data structure, often called Union Find.

Imagine every word starts as its own independent club, with itself as the president. When we discover that two words are connected, we merge their clubs. We do this by having the president of one club acknowledge the president of the other club as the new overall leader. 

To make this efficient, we implement two important techniques:
* **Path Compression:** When a club member wants to find out who the ultimate president is, they might have to ask their local leader, who asks a regional leader, who finally asks the president. With path compression, once the member learns who the ultimate president is, they update their records to point directly to that ultimate president. Next time, the lookup is instantaneous.
* **Union by Size:** When merging two clubs, we always attach the smaller club to the larger club. This prevents the chain of command from becoming too tall and inefficient.

We will use an array named `parent` to keep track of leaders, and an array named `sz` to keep track of the number of members in each club.

***

### The Core Logic of Finding Connections

With our bitmasks and our group management structure ready, we can establish the connections. Let us tackle the rules one by one.

**Handling Duplicates**
First, it is entirely possible that our input contains multiple words that evaluate to the exact same bitmask. For example, "ab" and "ba" will both become the same number. We should sort our masks, group identical numbers together, and count their frequencies. Each unique mask will represent a starting club, and its initial size will be its frequency.

**Adding and Deleting Characters**
As mentioned earlier, adding a character to word A to get word B is exactly the same relationship as deleting a character from word B to get word A. Therefore, we only need to simulate deletions to cover both adding and deleting rules!

For every unique mask, we look at its active light switches. For each active switch, we turn it off. This creates a new mask representing the word after one deletion. We then quickly search our list of unique masks to see if this deleted state actually exists as a word in our input. If it does exist, we immediately merge the club of the original mask with the club of the deleted mask.

**Replacing Characters**
The replacing rule is the most interesting puzzle piece. A replacement means we turn off one switch and turn on a different switch. Simulating all possible replacements for every word would be quite slow.

However, there is a brilliant alternative perspective. Suppose word A and word B differ by exactly one replacement. If we delete the differing character from word A, we get an intermediate state. If we delete the differing character from word B, we get the exact same intermediate state!

Therefore, whenever we generate a deletion state from any word, we should save that state along with the identity of the word that produced it. We store these in a big list of pairs. Once we have generated all possible deletions for all words, we sort this big list. 

By sorting the list, all identical intermediate states will end up right next to each other. We can simply iterate through the sorted list, and whenever we see adjacent entries that share the same intermediate state, we merge the clubs of the words that produced them. This completely solves the replacement connection rule in a very elegant way!

***

### Detailed Step by Step Walkthrough

Let us trace through a small conceptual example to cement these ideas. Suppose our unique masks correspond to the words "a", "b", "ab", and "cde".

* **Initialization:** We create clubs for "a", "b", "ab", and "cde". Each club starts with a size of 1.
* **Generating Deletions:**
    * We look at "a". We turn off its only switch. The intermediate state is empty. We save this intermediate state. We check if the empty string exists in our input. It does not.
    * We look at "b". We turn off its only switch. The intermediate state is empty. We save this intermediate state. We check if the empty string exists. It does not.
    * We look at "ab". We turn off the 'a' switch. The deleted state is "b". We check if "b" exists in our input. It does! We merge the club of "ab" with the club of "b".
    * We continue looking at "ab". We turn off the 'b' switch. The deleted state is "a". We check if "a" exists in our input. It does! We merge the club of "ab" with the club of "a".
    * We look at "cde". We generate deletions "cd", "ce", and "de". None of these exist in our input. We save them as intermediate states.
* **Processing Replacements:** We sort our saved intermediate states. The empty intermediate state was generated by both "a" and "b". Since these generated the exact same intermediate state, we merge the club of "a" and the club of "b". (In this specific example, they are already merged through "ab", but this mechanism catches replacements when no linking word is present).
* **Final Count:** We look at our clubs. "a", "b", and "ab" all share the same ultimate president. "cde" is its own president. We have a total of 2 groups. The largest group contains 3 words.

***

### Complexity Analysis

It is always important to evaluate the resources our approach consumes.

**Time Complexity:** Transforming the words into bitmasks requires us to look at every character of every word. Sorting the masks takes time proportional to the number of words multiplied by the logarithm of the number of words. 
Generating the deletions means for each unique word, we might create up to twenty six intermediate states. Sorting these intermediate states dominates the time taken. The sorting step takes time proportional to the number of states multiplied by the logarithm of the number of states. In the grand scheme, this is extremely efficient and comfortably runs within standard time limits. We can describe the time complexity broadly as Order of N log N, where N is the total number of intermediate states generated.

**Space Complexity:** We store the original bitmasks, the unique bitmasks, the club parent arrays, the club size arrays, and the large list of intermediate deletion states. The large list will have at most twenty six entries for each unique word. This memory requirement scales linearly with the input size. We can describe the space complexity as Order of N, meaning the memory grows steadily in direct proportion to the number of words.

***

### The C++ Implementation

Below is the C++ code encapsulating all the logic we have discussed. The code is carefully structured to be clean and readable. 

```cpp
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
        
        // Transform each string into a bitmask representing its characters.
        // We use bitwise AND with 31 to safely map 'a' to 1, 'b' to 2, and so on.
        for (const string& w : words) {
            int mask = 0;
            for (char c : w) {
                mask |= (1 << (c & 31)); 
            }
            masks.push_back(mask);
        }
        
        // Sort the masks to easily group duplicates together.
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
        
        // Initialize the disjoint set groups.
        for (int i = 0; i < N; ++i) {
            parent[i] = i;
            sz[i] = freq[i];
            max_size = max(max_size, sz[i]);
        }
        
        // Find function with path compression to keep the tree flat.
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
        
        // Union function attaching the smaller group to the larger group.
        auto union_set = [&](int u, int v) {
            int root_u = find_set(u);
            int root_v = find_set(v);
            if (root_u != root_v) {
                if (sz[root_u] < sz[root_v]) swap(root_u, root_v);
                parent[root_v] = root_u;
                sz[root_u] += sz[root_v];
                max_size = max(max_size, sz[root_u]);
            }
        };
        
        // We will store pairs of {intermediate state, original word index}.
        vector<pair<int, int>> deletions;
        deletions.reserve(N * 26);
        
        for (int u = 0; u < N; ++u) {
            int x = unique_masks[u];
            // Look through all twenty six possible letter switches.
            for (int i = 1; i <= 26; ++i) {
                if ((x >> i) & 1) { 
                    // Turn off the switch to simulate a deletion.
                    int d = x ^ (1 << i); 
                    deletions.push_back({d, u});
                    
                    // Check if this deleted state is a valid standalone word.
                    auto it = lower_bound(unique_masks.begin(), unique_masks.end(), d);
                    if (it != unique_masks.end() && *it == d) {
                        int v = distance(unique_masks.begin(), it);
                        union_set(u, v);
                    }
                }
            }
        }
        
        // Sort the intermediate states.
        // Identical intermediate states will end up adjacent to each other.
        sort(deletions.begin(), deletions.end());
        
        // Merge words that produce the exact same intermediate deletion state.
        for (size_t i = 0; i + 1 < deletions.size(); ++i) {
            if (deletions[i].first == deletions[i + 1].first) {
                union_set(deletions[i].second, deletions[i + 1].second);
            }
        }
        
        // Count the final number of unique groups.
        int num_components = 0;
        for (int i = 0; i < N; ++i) {
            if (parent[i] == i) {
                num_components++;
            }
        }
        
        return {num_components, max_size};
    }
};

```

---

### Concluding Thoughts

Thank you so much for reading through this detailed breakdown! Grouping problems often seem intimidating at first glance, especially when multiple connection rules apply. However, by breaking the rules down and realizing that a single intermediate deletion state can bridge both the adding and replacing logic, we turned a complex puzzle into a clean set of logical steps.

Converting strings into binary representation is a powerful technique to add to your toolkit. Paired with a robust group management structure like Disjoint Set Union, there are very few connectivity problems you cannot solve.

I hope this explanation brings clarity and that you feel more confident tackling similar string challenges in the future. Keep practicing, keep exploring, and happy coding! Let me know if you have any questions or alternate approaches you would like to share!
