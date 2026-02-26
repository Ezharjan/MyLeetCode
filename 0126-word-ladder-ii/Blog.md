# Transforming Words One Letter at a Time and Finding All Shortest Paths [#126]

Hello everyone and welcome to my blog! Today I want to share my journey and thoughts on a fascinating word puzzle. If you love string manipulation and graph traversal, you are in for a treat. We are going to explore a problem where we change a starting word into an ending word by modifying exactly one letter at a time. The catch is that every intermediate word we create must exist inside a provided dictionary. Our ultimate goal is to find all the shortest possible sequences of these transformations. 

This puzzle is basically a classic game invented by Lewis Carroll called Word Golf, but brought into the programming world. We will break this down step by step so that everyone, from beginners to experienced coders, can understand the mechanics and write a working solution. Grab a cup of coffee and let us dive in!

### Understanding the Rules of the Game

Imagine you are given a starting word, let us call it the begin word, and a target word, known as the end word. You are also handed a list of valid words which serves as your dictionary. 

A valid transformation sequence is a chain of words where:
* Every adjacent pair of words differs by exactly one single character.
* Every intermediate word in your chain must be present in the given dictionary.
* The sequence finishes exactly on the target end word.

We need to return a list of all shortest transformation sequences. If it is impossible to reach the end word, we simply return an empty list.

Let us look at a practical example to make this crystal clear. Suppose our starting word is "hit" and our target word is "cog". The valid dictionary contains the words "hot", "dot", "dog", "lot", "log", and "cog".

There are two shortest paths to reach "cog" from "hit":
1.  "hit" becomes "hot" which becomes "dot" which becomes "dog" which becomes "cog".
2.  "hit" becomes "hot" which becomes "lot" which becomes "log" which becomes "cog".

Both of these paths take exactly five words from start to finish. Since they are the shortest routes, our solution needs to find and return both of them.

### Visualizing the Problem as a Graph



To solve this, it helps immensely to view the problem as a graph. Imagine every word is an island. You can build a bridge between two islands if their names differ by only one letter. The dictionary is our map of all available islands. Our task is to navigate from the starting island to the destination island using the fewest bridges possible. 

Because we want the absolute shortest path, the Breadth First Search algorithm naturally comes to mind. Breadth First Search explores the graph level by level, radiating outward from the start like ripples in a pond. This guarantees that the first time we discover our destination, we have found the shortest route.

### The Memory Trap and Our Humble Approach

A common first instinct is to use Breadth First Search and store the entire path history inside the queue. While this sounds perfectly logical, it quickly leads to a massive problem. As the graph branches out, storing thousands of individual paths consumes an enormous amount of memory. Many of us have tried this and hit a wall where the computer simply runs out of memory or takes too long. It is a fantastic learning experience!

Instead of keeping the entire history in our queue, we can divide our approach into a sensible two part method.

#### Part One: Discovering the Shortest Distances


We will use Breadth First Search solely to build a map of relationships. Think of this as creating a family tree where we record "who discovered whom". We will explore the words level by level. When we are at a specific word, we generate all possible one letter changes. If a new word is in our dictionary, we link it back to the original word in our relationship map.

To prevent our algorithm from running in endless circles, we must remove words from our dictionary once they are fully processed for a level. However, we cannot remove a word the instant we see it, because another word on the exact same level might also be able to reach it, and we need to capture all shortest paths! Therefore, we gather all the words discovered in the current level and remove them from the dictionary only when we move on to the next level.

#### Part Two: Reconstructing the Sequences


Once our Breadth First Search successfully discovers the target end word, we stop building levels. We now have a complete relationship map. We then employ a Depth First Search, which is a backtracking technique. We start at our target end word and trace backwards using our relationship map until we reach our starting word. Once we hit the start, we reverse the path we just built and add it to our final list of answers.

### Tracing the Logic Step by Step

Let us mentally trace our strategy using the earlier example. 
Start word: "hit"
Target: "cog"
Dictionary: "hot", "dot", "dog", "lot", "log", "cog"

**Level 1:**
We start at "hit".
We generate changes. We find "hot" in the dictionary.
We record in our map that "hot" was discovered by "hit".
We remove "hit" from the active pool.

**Level 2:**
We explore "hot".
We change letters and discover "dot" and "lot" in the dictionary.
We record that "dot" was discovered by "hot", and "lot" was discovered by "hot".
We remove "hot" from the dictionary.

**Level 3:**
We explore "dot" and "lot".
From "dot", we discover "dog". We record "dog" discovered by "dot".
From "lot", we discover "log". We record "log" discovered by "lot".
We remove "dot" and "lot" from the dictionary.

**Level 4:**
We explore "dog" and "log".
From "dog", we discover "cog". We record "cog" discovered by "dog".
From "log", we discover "cog". We record "cog" discovered by "log".
We have found our target word "cog"! We stop the search.

Now, we backtrack from "cog". We see "cog" comes from both "dog" and "log". We follow both trails backwards, reversing them at the end to get our final valid sequences.

### C++ Code Walkthrough

Below is the C++ implementation of the strategy we just discussed. I have included lots of comments so you can follow along with the logic comfortably.

```cpp
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        
        unordered_set<string> dictionary(wordList.begin(), wordList.end());
        vector<vector<string>> final_results;
        
        if (dictionary.find(endWord) == dictionary.end()) {
            return final_results; 
        }

        unordered_map<string, vector<string>> parent_map; 
        unordered_set<string> current_level_words;
        
        current_level_words.insert(beginWord);
        bool target_found = false;

        while (!current_level_words.empty() && !target_found) {
            
            for (const string& word : current_level_words) {
                dictionary.erase(word);
            }

            unordered_set<string> next_level_words;
            
            for (string current_word : current_level_words) {
                string original_word = current_word;
                
                for (int i = 0; i < current_word.size(); ++i) {
                    char original_char = current_word[i];
                    
                    for (char alphabet = 'a'; alphabet <= 'z'; ++alphabet) {
                        if (alphabet == original_char) {
                            continue;
                        }
                        
                        current_word[i] = alphabet;
                        
                        if (dictionary.count(current_word)) {
                            next_level_words.insert(current_word);
                            parent_map[current_word].push_back(original_word);
                            
                            if (current_word == endWord) {
                                target_found = true; 
                            }
                        }
                    }
                    
                    current_word[i] = original_char; 
                }
            }
            
            current_level_words = next_level_words;
        }

        if (target_found) {
            vector<string> current_sequence;
            current_sequence.push_back(endWord);
            build_paths(endWord, beginWord, parent_map, current_sequence, final_results);
        }

        return final_results;
    }

private:
    void build_paths(const string& current_word, const string& start_word, 
                     unordered_map<string, vector<string>>& parent_map, 
                     vector<string>& current_sequence, vector<vector<string>>& final_results) {
        
        if (current_word == start_word) {
            vector<string> valid_sequence = current_sequence;
            reverse(valid_sequence.begin(), valid_sequence.end()); 
            final_results.push_back(valid_sequence);
            return;
        }

        for (const string& predecessor : parent_map[current_word]) {
            current_sequence.push_back(predecessor);
            build_paths(predecessor, start_word, parent_map, current_sequence, final_results);
            current_sequence.pop_back(); 
        }
    }
};

```

### Breaking Down the Code

Let us take a moment to look at the important pieces of the code above.

First, we convert our provided list of words into a hash set named `dictionary`. Hash sets are wonderful because they allow us to check if a word exists almost instantly.

We initialize a `parent_map` which is essentially a dictionary where the key is a word, and the value is a list of all words that lead to it.

The main loop runs as long as we have words to explore and we have not found the target word. At the beginning of each loop iteration, we wipe out the `current_level_words` from our dictionary. This is a crucial step! It stops us from revisiting words and ensures we only move forward, maintaining the shortest possible route.

Inside the loop, we iterate through every character of the current word. We swap each character with every letter of the alphabet from 'a' to 'z'. If the new modified word exists in our dictionary, we add it to our `next_level_words` set and update our `parent_map`. Notice how we restore the original character before moving to the next position. This allows us to test all one letter variations properly.

Finally, the `build_paths` function is our recursive helper. It starts at the destination and recursively looks up the predecessors in our map. When the start word is reached, it reverses the collected words to form a chronological path and saves it to our results list. The `pop_back` call is our backtracking mechanism, allowing the recursive function to explore alternative paths cleanly.

### Analyzing the Complexity

When discussing algorithms, it is helpful to understand their efficiency regarding time and memory.

**Time Complexity Considerations:**
Let us define **V** as the total number of words in our dictionary and **L** as the length of each word.
During our breadth search, we process up to **V** words. For each word, we iterate over its **L** characters, and for each character, we try 26 alphabet substitutions. Creating a new string takes roughly **L** operations. This makes the search phase proportional to **O(V * L * L)** roughly.
The backtracking phase depends heavily on how many valid paths exist. In the absolute worst case scenario, a highly interconnected graph could have many paths, making the time proportional to the number of paths multiplied by their length.

**Space Complexity Considerations:**
We use a hash set for the dictionary which stores **V** words of length **L**, requiring **O(V * L)** space. The parent map also stores at most **V** keys, with associated strings, keeping the space within **O(V * L)** limits. The recursive stack memory is bounded by the length of the shortest path, which cannot exceed **V**. Overall, the space usage is very reasonable and perfectly suited for the given constraints.

### Final Thoughts and Encouragement

Navigating graph algorithms can seem intimidating at first, but breaking them down into separate, manageable tasks makes a world of difference. By separating the discovery phase from the path construction phase, we completely avoided the memory overload trap.

I hope this thorough walkthrough brings clarity to the problem and helps you in your coding journey. Sharing these concepts with the community is a joy, and I encourage you to type out the code, run it through some tests, and experiment with print statements to see the parent map being built in real time.

Keep practicing, stay curious, and happy coding!
