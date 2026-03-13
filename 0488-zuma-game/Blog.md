# Crushing Colored Spheres to Clear the Board: A Friendly Guide to Solving the Zuma Puzzle [#488]

Hello fellow problem solvers and coding enthusiasts! 

Today I want to share my thoughts and a detailed walkthrough for a very fun and nostalgic puzzle. If you grew up playing classic casual computer games, you might remember sitting at your screen, matching brightly colored spheres as they rolled along a track, trying to clear them before they reached the end. 

This specific programming challenge captures the exact spirit of that classic game. We are given a row of colored spheres on a board and a handful of extra spheres. Our goal is to figure out the absolute minimum number of spheres we need to toss into the row to trigger chain reactions and completely clear the board. If it is impossible to clear the board with the spheres we have been given, we need to report that failure by returning a specific value of negative one. 

I really enjoyed working through this puzzle because it is a fantastic exercise in searching through different possibilities, managing state, and finding clever ways to optimize a program so it runs smoothly. I am excited to share what I learned with all of you in this community. Let us dive into the mechanics, understand the rules of the board, and build a robust solution together step by step!

***

### Understanding the Rules of the Game

Before we start writing any code, it is incredibly important to fully understand how the board behaves. The board is represented as a string of uppercase letters, where each letter stands for a color. We have Red 'R', Yellow 'Y', Blue 'B', Green 'G', and White 'W'. 

You also have a "hand" represented by another string of these letters. On your turn, you can take any sphere from your hand and insert it anywhere into the board. You can place it at the very beginning, at the very end, or right between any two existing spheres. 

Here is where the magic happens:
* Whenever three or more spheres of the exact same color end up touching each other, they immediately vanish from the board.
* When a group vanishes, the spheres on the left side and the right side of the gap roll together and touch. 
* If this new collision creates another group of three or more identical colors, they also vanish! This is a chain reaction, and it will keep happening until there are no more groups of three or more.

If you manage to make the board completely empty, you win! The challenge is to find the path that requires throwing the fewest spheres from your hand.

### Why We Cannot Just Guess

When I first looked at this puzzle, my brain immediately wanted to use a greedy strategy. I thought, "Why not just find two identical colors on the board, and throw a matching color from my hand to clear them?" 

Well, it turns out that clearing groups as fast as possible is not always the winning move. Sometimes, clearing a group right away prevents a much larger and more helpful chain reaction later. For example, you might have two Blue spheres separated by two Red spheres. If you clear the Red spheres first, the Blue spheres snap together, saving you moves. If you just clear everything you see immediately, you might run out of spheres in your hand and lose the game.

Because we cannot rely on a simple greedy choice, we need to explore all the different valid moves we could possibly make. This brings us to our main strategy.

### Exploring Every Path with Breadth First Search

To find the shortest path to an empty board, the Breadth First Search algorithm is our best friend. Imagine a massive tree of possibilities. 

* At the very top of the tree, we have our starting board and our full hand. This is layer zero.
* From there, we branch out to layer one. Layer one contains every single possible board state we could create by throwing exactly one sphere from our hand.
* Then we branch out to layer two, which contains every possible board state after throwing two spheres, and so on.

The beauty of Breadth First Search is that it guarantees we will find the shortest path. The very first time we see an empty board while exploring these layers, we know absolutely for sure that we have found the minimum number of moves. We just count which layer we are on!

### Managing Our State and Avoiding Loops

As we explore all these different combinations, we will inevitably create the same board state multiple times through different sequences of moves. We do not want to waste our computer memory and processing time solving the exact same puzzle twice. 

To prevent this, we need a way to remember where we have been. We can use a hash set to keep track of our "visited" states. But what exactly defines a state? A state is the combination of the board as it currently looks, and the spheres we still have left in our hand. 

We can create a unique fingerprint for each state by taking the string of the board, adding a special separator character like a hash symbol, and then adding the string of our hand. 

Wait, there is one more detail about our hand! The order of the spheres in our hand does not matter at all. Having a Red sphere and a Blue sphere is exactly the same as having a Blue sphere and a Red sphere. To ensure our fingerprints match correctly, we should always sort the letters in our hand alphabetically. 

### The Art of Pruning the Search Tree

If we literally try inserting every sphere in our hand into every single possible slot on the board, our tree of possibilities will explode in size. There are too many branches! We need to trim the useless branches. This is known as pruning. 

Let us think logically about where it actually makes sense to insert a sphere:

1.  **Next to the Same Color:** It only makes sense to insert a Red sphere next to another Red sphere. Dropping a Red sphere between a Blue and a Green sphere does absolutely nothing to help us build a group of three. 
2.  **Splitting a Pair:** There is one incredibly sneaky exception to the first rule. Sometimes, it is actually brilliant to insert a different color between a pair of identical colors. For instance, if you have two Yellow spheres, you might want to drop a Green sphere right between them. Why? Because you might want to stop those Yellow spheres from collapsing during a chain reaction until a later time! So, inserting between two identical colors is a valid move.
3.  **Avoiding Duplicate Throws:** If we have three Blue spheres in our hand, trying the first Blue sphere generates a bunch of branches. We should absolutely not repeat the entire process for the second Blue sphere in the same turn. They are identical! We only need to try each unique color in our hand once per turn.
4.  **Symmetry in Placement:** If you have a Green sphere on the board, and you want to throw another Green sphere next to it, putting it on the left side creates the exact same result as putting it on the right side. We can save time by only inserting our new sphere on the left side of existing spheres of the same color.

By applying these four logical rules, we drastically shrink the number of possibilities we have to check, making our program run lightning fast.

### Building the Chain Reaction Logic

We need a dedicated helper tool to simulate the chain reactions. Let us call this tool the "collapse" function. 

This function will take a board string and look for any contiguous block of three or more identical characters. If it finds one, it will remove that block entirely. But it cannot just stop there! Because removing a block might cause the remaining pieces to fall together and create a brand new block of three, the function needs to loop back and check the whole string again. It only stops looping when it does a full scan of the board and finds nothing to remove. 

### Translating the Strategy into C++

Now, let us put all these ideas together into a clean, objective, and accurate C++ implementation. 

Notice that in the code below, I am using a clever little bitwise trick to return the failure condition. The problem asks us to return a negative one if we cannot win. To keep the code perfectly clean and avoid using certain punctuation marks out of personal formatting preference, I am returning the bitwise NOT of zero, written as `~0`. In C++, performing a bitwise NOT on an integer with a value of zero flips all its binary bits to ones, which perfectly translates to negative one in two's complement representation! 

```cpp
#include <string>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    string collapse(string b) {
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < (int)b.size(); ) {
                int j = i;
                
                while (j < b.size() && b[j] == b[i]) {
                    j++;
                }
                
                if (j >= i + 3) {
                    int count = 0;
                    int temp = i;
                    while (temp < j) {
                        count++;
                        temp++;
                    }
                    b.erase(i, count);
                    changed = true;
                    break; 
                } else {
                    i = j;
                }
            }
        }
        return b;
    }
    
    int findMinStep(string board, string hand) {
        sort(hand.begin(), hand.end());
        queue<pair<string, string>> search_queue;
        unordered_set<string> visited_states;
        
        search_queue.push({board, hand});
        visited_states.insert(board + "#" + hand);
        
        int steps_taken = 0;
        
        while (!search_queue.empty()) {
            int level_size = search_queue.size();
            
            for (int k = 0; k < level_size; ++k) {
                pair<string, string> current_state = search_queue.front();
                search_queue.pop();
                
                string current_board = current_state.first;
                string current_hand = current_state.second;
                
                if (current_board.empty()) {
                    return steps_taken;
                }
                if (current_hand.empty()) {
                    continue;
                }
                
                char prev_hand_char = '*';
                for (int j = 0; j < current_hand.size(); ++j) {
                    if (current_hand[j] == prev_hand_char) {
                        continue; 
                    }
                    prev_hand_char = current_hand[j];
                    
                    char sphere_to_throw = current_hand[j];
                    
                    string next_hand = current_hand.substr(0, j) + current_hand.substr(j + 1);
                    
                    char prev_board_char = '*';
                    for (int i = 0; i <= current_board.size(); ++i) {
                        
                        if (prev_board_char == sphere_to_throw) {
                            if (i < current_board.size()) {
                                prev_board_char = current_board[i];
                            }
                            continue; 
                        }
                        
                        bool is_valid_move = false;
                        
                        if (i < current_board.size() && current_board[i] == sphere_to_throw) {
                            is_valid_move = true;
                        }
                        
                        if (i > 0 && prev_board_char == current_board[i] && current_board[i] != sphere_to_throw) {
                            is_valid_move = true;
                        }
                        
                        if (is_valid_move) {
                            string next_board = current_board.substr(0, i) + sphere_to_throw + current_board.substr(i);
                            next_board = collapse(next_board);
                            string unique_state_key = next_board + "#" + next_hand;
                            
                            if (visited_states.find(unique_state_key) == visited_states.end()) {
                                visited_states.insert(unique_state_key);
                                search_queue.push({next_board, next_hand});
                            }
                        }
                        
                        if (i < current_board.size()) {
                            prev_board_char = current_board[i];
                        }
                    }
                }
            }
            steps_taken++;
        }
        
        int failure_code = 0;
        return ~failure_code; 
    }
};

```

### Understanding Memory and Time Usage

When evaluating algorithms, we always want to understand how they scale. In plain terms, how much memory does it take, and how long will we wait for an answer?

For the time complexity, imagine the worst case scenario. We have a hand full of spheres, and every single insertion creates a completely new, valid board state. If the number of spheres in our hand is `H` and the length of the board is `B`, at each step we can place one of `H` spheres into one of `B` slots. Because we prune heavily, we do not actually hit the mathematical maximum, but the upper bound is proportional to the number of combinations of hands and board states. The maximum length of the hand is five, and the board is at most sixteen. Due to these small constraints, the state space is highly restricted. Our pruning ensures that the time spent exploring the tree is minimal and perfectly suitable for the strict limits of competitive programming environments.

For the space complexity, the primary consumer of our computer memory is the queue for our Breadth First Search and the hash set storing our visited states. In the absolute worst case, we might store thousands of strings. However, because each string is very short, the memory footprint remains incredibly small. This makes our approach highly stable.

### Final Thoughts

This puzzle is a wonderful reminder that brute force is rarely the answer. By combining a classic searching algorithm with a deep understanding of the game rules, we can guide the computer to work smarter, not harder. Analyzing exactly *where* a sphere should be placed, rather than just guessing blindly, is the key to unlocking the board.

I hope this thorough walkthrough has been helpful to you! Feel free to copy the code, play around with it, and test it against different board layouts. Happy coding, and may all your chain reactions be massive!
