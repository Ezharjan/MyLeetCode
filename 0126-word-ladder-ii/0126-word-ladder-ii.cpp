#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        vector<vector<string>> res;
        
        // Fast fail: if endWord isn't in the dictionary, no path is possible.
        if (dict.find(endWord) == dict.end()) {
            return res;
        }

        unordered_map<string, vector<string>> parents; // Maps a word to its predecessors
        unordered_set<string> current_level;
        current_level.insert(beginWord);
        
        bool found = false;

        // Step 1: BFS to build the shortest-path DAG
        while (!current_level.empty() && !found) {
            // Remove all words of the current level from the dictionary 
            // to prevent cycles and ensure shortest paths.
            for (const string& w : current_level) {
                dict.erase(w);
            }

            unordered_set<string> next_level;
            
            for (string w : current_level) {
                string original = w;
                
                // Try changing every character of the current word
                for (int i = 0; i < w.size(); ++i) {
                    char old_c = w[i];
                    for (char c = 'a'; c <= 'z'; ++c) {
                        if (c == old_c) continue;
                        w[i] = c;
                        
                        // If the transformed word is in the dictionary
                        if (dict.count(w)) {
                            next_level.insert(w);
                            parents[w].push_back(original);
                            if (w == endWord) {
                                found = true; // We reached the end, but finish the level
                            }
                        }
                    }
                    w[i] = old_c; // Backtrack the character change
                }
            }
            current_level = next_level;
        }

        // Step 2: DFS to reconstruct all paths from endWord to beginWord
        if (found) {
            vector<string> path = {endWord};
            dfs(endWord, beginWord, parents, path, res);
        }

        return res;
    }

private:
    void dfs(const string& current, const string& beginWord, 
             unordered_map<string, vector<string>>& parents, 
             vector<string>& path, vector<vector<string>>& res) {
        
        // Base Case: We've backtracked to the start
        if (current == beginWord) {
            vector<string> valid_path = path;
            reverse(valid_path.begin(), valid_path.end()); // Reverse to get begin -> end
            res.push_back(valid_path);
            return;
        }

        // Recursive Step: Explore all valid parents
        for (const string& p : parents[current]) {
            path.push_back(p);
            dfs(p, beginWord, parents, path, res);
            path.pop_back(); // Backtrack
        }
    }
};