# Maximizing Profit by Finding the Best Topological Order in a Directed Graph [#3530]

Hello everyone! Welcome to another community problem solving session. 

Now I'm very happy to share some thoughts on an incredibly interesting graph problem. It tests our ability to combine graph theory with dynamic programming in a very clever way. If you have ever wondered how to find a specific sequence of tasks that gives you the highest possible reward while strictly following dependency rules, this post is for you. I want to walk through the concepts step by step so that everyone can understand the logic and apply it to similar challenges. Let us dive right in.

### Understanding the Problem Requirements

We are given a collection of nodes. You can think of these nodes as individual tasks or projects. These nodes form a Directed Acyclic Graph. This means there are one way arrows pointing from some nodes to other nodes, and it is impossible to start at one node, follow the arrows, and end up back where you started. These arrows represent strict prerequisites. If an arrow points from node A to node B, you absolutely must complete task A before you can even begin task B.

Every single node in this graph has an associated score. Our goal is to process every single node in a valid order. A valid order simply means we never violate the prerequisite rules. 

Here is the interesting twist. When we process a node, we do not just get its raw score. The points we receive are multiplied by the node position in our sequence. The very first node we process gets its score multiplied by one. The second node gets its score multiplied by two. The third gets multiplied by three, and so forth. We want to find the absolute best sequence that yields the highest total profit. 

### Why a Simple Approach is Not Enough

At first glance, you might think we could just look at the available nodes at any given moment and pick the one with the highest score. Or maybe we should save the highest score nodes for the very end so they get the largest position multiplier. 

Unfortunately, those simple greedy strategies will often lead us down the wrong path. We are heavily restricted by the arrows in our graph. Sometimes we might be forced to process a node with a very low score early on simply because it unlocks a pathway to several massively valuable nodes later. Because every decision we make early on shifts the position multipliers of every node that follows, we cannot rely on local, immediate choices. We need a way to look at the global picture.

### Enter Dynamic Programming and Bitmasks

When we look at the problem constraints, we notice a very important detail. The total number of nodes will never exceed 22. This relatively small number is a massive hint! It tells us we can represent the exact state of our progress using a single integer.

This technique is called bitmasking. A computer stores integers as a sequence of binary bits, which are just ones and zeros. We can use a 32 bit integer to represent our graph. Each bit position in the integer corresponds to a specific node. If the bit at position five is a one, it means we have successfully processed node five. If it is a zero, node five is still waiting to be processed.



By using this bitmask integer, we can define a dynamic programming state. We will maintain a memory table where each index is a specific bitmask, representing a unique combination of completed nodes. The value stored at that index will be the maximum possible profit we could have earned to reach that exact state.

### Breaking Down the Logic Step by Step

Let us explore exactly how we can build this solution together.

* **Step One Tracking Prerequisites** Before we calculate any profits, we need to understand the rules of our graph. For every single node, we will create its very own prerequisite bitmask. We will look at all the arrows pointing toward a specific node. If an arrow comes from node X, we turn on the bit at position X in our prerequisite mask. Later on, when we want to know if we are allowed to process a node, we just compare our current progress bitmask with the node prerequisite mask. If our current progress contains all the required bits, we are good to go!

* **Step Two Setting up the Memory Table**
    We will create a large array to hold our maximum profits. The size of this array needs to be large enough to hold every possible combination of 22 nodes. That is two raised to the power of 22. We will fill this entire array with a special marker value to indicate that a state has not been reached yet or is impossible to reach. Since we want to avoid using hyphens or dashes in our code and text, we will use a clever bitwise trick. The bitwise NOT of zero, written as `~0`, flips all the zero bits to ones. In standard signed integers, this represents the value negative one. We will use `~0` as our "unreachable" marker. The only state we know for sure at the very beginning is the state where zero nodes are processed. The profit for zero nodes processed is exactly zero.

* **Step Three Transitioning Between States**
    Now comes the beautiful part. We will count from the integer one all the way up to our maximum possible state. Each integer represents a combination of processed nodes. For every combination, we want to figure out the best way we could have arrived there.

    To do this, we look at the bits that are turned on in our current combination. Each "on" bit represents a node that is part of this group. We will take turns pretending that each of these nodes was the very last one added to the group. 

    If a node was the last one added, we first check if that was even a legal move. We look at the prerequisite mask for that node. Are all of its prerequisites present in the group? If they are, we calculate what the group looked like right before we added this node. We do this by turning off the bit for our chosen node. 

    We then look up the maximum profit for that previous state in our memory table. If that previous state is reachable, we calculate our new profit. We add the maximum profit of the previous state to the score of our newly added node multiplied by its sequence position. We compare this new profit against the best profit we have found so far for our current group. If it is better, we update our memory table.

### Optimizing with Built In Functions

To make our code run smoothly, we can use some extremely fast compiler functions to manipulate our bits. 

* We need to know the sequence position of the node we are adding. The position is exactly equal to the total number of nodes processed so far. We can find this instantly by counting the number of ones in our current bitmask using a function called `__builtin_popcount`.
* When we iterate through the nodes in our combination, we need to isolate the ones. We can find the position of the lowest set bit using `__builtin_ctz`. 
* After we check a bit, we need to turn it off so we can find the next one. A common trick is to subtract one from the value and perform a bitwise AND. To keep our code completely free of dashes, we will achieve the exact same mathematical subtraction by adding our `~0` marker! Adding negative one is the same as subtracting one.

### The C++ Implementation

Here is how we can put all of these concepts together into a clean C++ solution. I have carefully constructed this code to be robust and strictly follow our formatting preferences.

```cpp
class Solution {
public:
    int maxProfit(int n, vector<vector<int>>& edges, vector<int>& score) {
        
        vector<int> req(n, 0);
        for (const auto& edge : edges) {
            req[edge[1]] |= (1 << edge[0]);
        }
        
        vector<int> dp(1 << n, ~0);
        
        dp[0] = 0;
        
        for (int mask = 1; mask < (1 << n); ++mask) {
            
            int pos = __builtin_popcount(mask);
            
            int best = ~0;
            
            int temp = mask;
            while (temp > 0) {
                
                int i = __builtin_ctz(temp);
                
                temp &= (temp + (~0)); 
                
                if ((req[i] & mask) == req[i]) {
                    
                    int prev = mask ^ (1 << i);
                    
                    if (dp[prev] != ~0) {
                        
                        int val = dp[prev] + score[i] * pos;
                        
                        if (val > best) {
                            best = val;
                        }
                    }
                }
            }
            
            dp[mask] = best;
        }
        
        return dp[(1 << n) + (~0)];
    }
};

```

### Analyzing the Complexity

It is always a good habit to understand the resource requirements of our code. Let us break down the time and space complexity.

**Time Complexity Breakdown**
Our main loop iterates over every possible combination of nodes. Since each node can be either included or excluded, there are exactly two raised to the power of N combinations, where N is the total number of nodes. For each of these combinations, we look at the nodes that are included. In the worst case scenario, we might look at N nodes. Therefore, the total number of operations scales proportionally to N multiplied by two raised to the power of N. We write this as O(N * 2^N). Given that the maximum number of nodes is 22, two raised to the 22nd power is roughly four million. Multiplying that by 22 gives us around 90 million operations, which modern computers and online judges can handle very comfortably within standard time limits.

**Space Complexity Breakdown**
Our memory requirements are directly tied to the size of our dynamic programming table. We need one integer for every single possible bitmask state. As we determined earlier, there are two raised to the power of N possible states. Therefore, our space complexity is strictly O(2^N). An array of four million integers consumes roughly 16 megabytes of memory, which is highly efficient and perfectly acceptable for this kind of algorithmic challenge.

### Final Thoughts

I really hope this detailed walkthrough helps demystify dynamic programming on graphs using bitmasks! By treating combinations of nodes as binary numbers, we transform a very complex ordering puzzle into a systematic sequence of simple state transitions. It takes a little bit of practice to get comfortable reading and writing bitwise operations, but once the concept clicks, it becomes an incredibly powerful tool in your problem solving toolkit.

Thank you so much for reading. 

Keep practicing, keep sharing your knowledge with others, and happy coding!
