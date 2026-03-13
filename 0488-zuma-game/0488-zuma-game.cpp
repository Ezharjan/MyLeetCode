#include <string>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    // Helper function to recursively collapse the board
    string collapse(string b) {
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < (int)b.size(); ) {
                int j = i;
                // Find contiguous identically colored balls
                while (j < b.size() && b[j] == b[i]) j++;
                if (j - i >= 3) {
                    b.erase(i, j - i);
                    changed = true;
                    break; // Restart from the beginning as chain reaction might have triggered
                } else {
                    i = j;
                }
            }
        }
        return b;
    }
    
    int findMinStep(string board, string hand) {
        sort(hand.begin(), hand.end());
        queue<pair<string, string>> q;
        unordered_set<string> visited;
        
        q.push({board, hand});
        visited.insert(board + "#" + hand);
        
        int steps = 0;
        
        while (!q.empty()) {
            int sz = q.size();
            for (int k = 0; k < sz; ++k) {
                auto [b, h] = q.front();
                q.pop();
                
                // If board is fully cleared, return the step count
                if (b.empty()) return steps;
                if (h.empty()) continue;
                
                // Try playing each ball in our hand
                for (int j = 0; j < h.size(); ++j) {
                    // Skip identical balls in hand to prevent redundant search paths
                    if (j > 0 && h[j] == h[j-1]) continue;
                    
                    char c = h[j];
                    string next_h = h.substr(0, j) + h.substr(j+1);
                    
                    // Try inserting the ball at every possible strategic position on the board
                    for (int i = 0; i <= b.size(); ++i) {
                        // Optimization: if inserting next to an identical ball, only insert it on one side
                        if (i > 0 && b[i-1] == c) continue;
                        
                        bool valid = false;
                        // Condition 1: Insert next to a ball of the same color
                        if (i < b.size() && b[i] == c) valid = true;
                        // Condition 2: Insert between two balls of the SAME color to split them
                        if (i > 0 && i < b.size() && b[i-1] == b[i] && b[i] != c) valid = true;
                        
                        if (valid) {
                            string next_b = b.substr(0, i) + c + b.substr(i);
                            next_b = collapse(next_b);
                            string state = next_b + "#" + next_h;
                            
                            // Only queue unexplored states
                            if (visited.find(state) == visited.end()) {
                                visited.insert(state);
                                q.push({next_b, next_h});
                            }
                        }
                    }
                }
            }
            steps++; // Increment depth/step
        }
        
        return -1; // If the queue runs empty, it's impossible to clear
    }
};