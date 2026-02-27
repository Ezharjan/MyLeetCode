# Unraveling the Mystery of Finding Matching Front and Back Words in an Array [#3045]

Welcome friends! Today I want to share a journey I took while solving a very interesting string puzzle. It is a wonderful exercise that blends string manipulation with tree data structures. I hope you grab a cup of tea or coffee, and let us dive deep into the mechanics of this algorithm together. This guide is written to be as detailed and approachable as possible, ensuring that everyone from beginners to seasoned programmers can follow along. 

### The Core Puzzle

Imagine you are given a sequence of words. Your goal is to find pairs of words from this list. But we are not looking for just any pairs! We want to pick an earlier word in the list and a later word in the list such that the earlier word forms both the exact beginning and the exact ending of the later word. 

If we choose an earlier word and call it the "source" and choose a later word and call it the "target", we need the source to be a prefix of the target and also a suffix of the target. 

Let us look at a simple scenario. Suppose your earlier word is "aba" and your later word is "ababa". 
* Does the target word "ababa" start with "aba"? Yes, it absolutely does. This means it is a prefix.
* Does the target word "ababa" end with "aba"? Yes, it does. This means it is a suffix.

Because both conditions are met, this combination is considered a valid pair, and we would add one to our final answer count. If our earlier word was "abc" and the later word was "abcd", it starts with "abc" but it ends with "bcd", so it fails the suffix test and we would ignore it.

We need to figure out exactly how many valid pairs exist in the entire sequence of words provided to us. The words only contain lowercase English letters, but the catch is that there can be up to one hundred thousand words, and the total number of characters across all words can be up to half a million.

### Why a Simple Loop is Not Enough

When faced with a matching problem, our first instinct is often to compare everything with everything else. We could write a loop that picks the first word, and then a second loop that checks every single word that comes after it to see if it matches the prefix and suffix rules. 

While this thought process is completely natural and correct in its logic, we run into a physical limitation with the computer. If we have one hundred thousand words, comparing every word against every other word requires billions of checks. Our program would take far too long to finish, and we would be sitting at our screens waiting forever. We need a way to look backwards at all the words we have already processed without having to reread them one by one.

### The Magic of the Prefix Tree

To solve this smoothly, we can use a wonderful data structure known as a Trie. 



A Trie, often pronounced like the word "try", is a special kind of tree used to store strings. Instead of storing an entire word in a single box, a Trie breaks the word down character by character. The path from the top of the tree down to the bottom spells out the word. 

Whenever we want to know if we have seen a certain prefix before, we just walk down the tree from the top. If we can follow the path letter by letter, the prefix exists! If we hit a dead end, we know immediately that no such prefix was ever added. This saves us from having to search through a massive list of strings.

### A New Way to Look at Words

A standard Trie is brilliant for checking prefixes. However, our puzzle requires us to check prefixes AND suffixes simultaneously. This sounds complicated, but we can use a very creative perspective to make it easy.

Instead of looking at a word from left to right one letter at a time, what if we looked at it from the outside in? 

Let us take the word "ababa" and pick characters from the front and the back at the same time.
* Step one brings us the first character "a" and the last character "a". We pair them together as (a, a).
* Step two brings us the second character "b" and the second to last character "b". We pair them as (b, b).
* Step three brings us the middle character "a" and the middle character "a". We pair them as (a, a).
* Step four brings us the fourth character "b" and the second character "b". We pair them as (b, b).
* Step five brings us the last character "a" and the first character "a". We pair them as (a, a).

The sequence of pairs for "ababa" is (a, a), (b, b), (a, a), (b, b), (a, a).

Now let us do the same for the smaller word "aba".
* Step one brings us the first and last characters. They are both "a". The pair is (a, a).
* Step two brings us the middle character. It is "b" for both the front step and the back step. The pair is (b, b).
* Step three brings us the last character "a" and the first character "a". The pair is (a, a).

The sequence of pairs for "aba" is (a, a), (b, b), (a, a).

Do you notice something wonderful here? The sequence of pairs for "aba" perfectly matches the beginning of the sequence of pairs for "ababa"! 

By pairing the front and back characters together at each step, a word is a prefix and a suffix of another word if and only if its pair sequence is a prefix of the other word's pair sequence! We have turned a tricky two part rule into a simple one part rule.

### Building Our Custom Tree

Now that we know we just need to match sequences of pairs, we can build a Trie where each pathway represents a pair of characters rather than a single character.

Since all characters are lowercase English letters, there are twenty six possible letters. We can easily convert a pair of letters into a single unique number to store in our tree. If we treat the first letter like the tens column and the second letter like the ones column in a base twenty six number system, we can calculate a unique identifier. 

We take the alphabetical position of the first letter, multiply it by twenty six, and then add the alphabetical position of the second letter. This guarantees every pair from (a, a) to (z, z) gets a unique number from zero to six hundred seventy five.

As we read through the list of words one by one, we will do the following for each word.
* We start at the top of our tree.
* We generate the front and back character pairs for the word.
* We walk down the tree following the path created by these pairs.
* If the path does not exist yet, we carefully build new branches.
* As we arrive at each node along the path, we look to see if any previous completely finished words ended exactly at this spot. If they did, it means those earlier words are perfect prefix and suffix matches for the word we are currently walking with. We add the count of those completed words to our running total of valid matches.
* Once we finish walking the entire word, we mark that final landing spot by increasing its completed word count by one. This leaves a footprint for future words to find.

### Keeping It Light and Fast

Trees are often built by creating lots of separate objects in memory. While this works fine, it can sometimes be a little heavy and slow because the computer has to constantly find new empty spaces in memory to put the objects.

A neat trick shared by many community members is to use parallel flat arrays to represent the tree. We can create simple integer lists that grow as we need them.
* One list called count will track how many finished words end at a given tree node.
* One list called first_child will point to the index of the first branch sprouting from a node.
* One list called next_sibling will help us chain together multiple branches sharing the same parent node.
* One list called val will store the unique number representing the pair of letters for that branch.

Whenever we need a new node, we just add a new slot to the end of all these lists. It keeps all our data tightly packed together, which computers absolutely love. It makes the execution highly responsive and keeps memory usage strictly controlled.

### The Code Implementation

Below is the C++ code that brings all these ideas to life. I have left comments throughout the code so you can see exactly where each part of our plan is happening. Even though the language uses hyphens for math and structure, remember that the logic flows exactly as we discussed!

```cpp
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    long long countPrefixSuffixPairs(vector<string>& words) {
        
        vector<int> count;
        vector<int> first_child;
        vector<int> next_sibling;
        vector<int> val;
        
        auto add_node = [&](int v) {
            count.push_back(0);
            first_child.push_back(-1);
            next_sibling.push_back(-1);
            val.push_back(v);
        };
        
        add_node(-1);
        
        long long ans = 0;
        
        for (const string& w : words) {
            int u = 0; 
            int n = w.length();
            
            for (int i = 0; i < n; ++i) {
                
                int c1 = w[i] - 'a';
                int c2 = w[n - 1 - i] - 'a';
                
                int v = c1 * 26 + c2;
                
                int child = -1;
                for (int edge = first_child[u]; edge != -1; edge = next_sibling[edge]) {
                    if (val[edge] == v) {
                        child = edge;
                        break;
                    }
                }
                
                if (child == -1) {
                    child = count.size();
                    add_node(v);
                    next_sibling[child] = first_child[u];
                    first_child[u] = child;
                }
                
                u = child;
                
                ans += count[u];
            }
            
            count[u]++;
        }
        
        return ans;
    }
};

```

### Analyzing the Approach

It is always good practice to understand the resources our logic will consume.

When considering the Time Complexity, we are processing each word exactly once. For every word, we loop through its characters. Inside that loop, we do a small amount of work to check our tree branches. Because the number of branches at any node is limited by the number of possible letter pairs, the branch checking is very quick. Therefore, the time it takes is directly proportional to the total number of characters across all words in the input array. In Big O notation, if L is the sum of the lengths of all words, the time complexity is Big O of L. This is incredibly fast and easily handles the large constraints of the puzzle.

When considering the Space Complexity, we are storing tree nodes in our flat arrays. In the absolute worst case where every single word has completely unique character pairs and shares no branches, we would create one new node for every single character of every single word. Thus, the memory required is also directly proportional to the total number of characters across all words. The space complexity is Big O of L. This guarantees we will not run out of memory.

### Final Thoughts

This puzzle is a wonderful demonstration of how a change in perspective can simplify a complex problem. By viewing prefixes and suffixes not as separate elements but as a unified sequence of pairs, we transformed a heavy counting task into an elegant pathfinding journey down a custom tree.

I really enjoyed putting this explanation together and I hope it sheds light on this clever technique. Coding is all about sharing knowledge and growing together. If you have any questions or just want to share your own experience with this puzzle, please feel free to do so. 

Happy coding everyone!
