# Conquering the Grid Coloring Challenge A Friendly Guide to Maximizing Scores [#3225]

Hello everyone! Welcome to another community sharing post where we explore an interesting puzzle together. Today we are looking at a fascinating grid based puzzle that involves coloring cells and accumulating scores based on specific adjacency rules. I want to walk you through a detailed and friendly exploration of how we can approach this challenge using Dynamic Programming. 

This guide is designed to be as comprehensive as possible so that whether you are a beginner looking to understand advanced state tracking or an experienced coder looking for a fresh perspective, you will find value here. Grab a warm beverage and let us dive into the logic together!

### Understanding the Rules of the Game

Imagine you are given a square grid of numbers. Let us say the grid has a size of N by N. When we start, every single cell in this grid is colored white. 

You are allowed to perform a specific coloring operation. For any column in the grid, you can choose a certain row index. Once you pick that row index, you paint all the cells in that column from the very top row down to your chosen row. These painted cells become black. You can choose different stopping rows for different columns or even leave some columns completely white by choosing a height of zero.



Now, how do we score points? We only get points from the white cells, but not just any white cells. A white cell will only add its number to our total score if it is sitting horizontally right next to a black cell. This means it must have a black cell directly to its left or directly to its right. If a white cell is surrounded only by other white cells on its left and right, it gives us zero points. 

Our ultimate goal is to figure out the best sequence of coloring operations to make our final score as large as possible. 

### Breaking Down the Scoring Mechanism

To build a solid logic, we need to focus on a single column and understand exactly when its white cells contribute to the score. Let us look at a specific column, let us call it column C. 

Suppose we decide to paint column C black from the top down to row J. This means cells from row 0 to row J minus 1 are black, and cells from row J downwards are white. 
Now, let us look at the column to its left, column C minus 1, which is painted black down to row I. 
We also look at the column to its right, column C plus 1, which is painted black down to row K.

For any cell in our current column C at a specific row R, it will add its value to our score if two conditions are met
* First, the cell itself must be white. This means its row index R must be greater than or equal to J.
* Second, it must touch a black cell on the left or the right. This means the row index R must be strictly less than I (touching the left black cells) OR strictly less than K (touching the right black cells).

Combining these two thoughts, a cell in column C at row R contributes to our total score if R is greater than or equal to J AND R is strictly less than the maximum of I and K. 

If we precalculate the prefix sums of each column, we can find the sum of any continuous block of cells in constant time. If we know the prefix sums, the score we gain from column C is simply the prefix sum up to the maximum of I and K, minus the prefix sum up to J. If J is larger than the maximum of I and K, we simply get zero from this column.

### Designing the State for Dynamic Programming

Since the score from column C depends entirely on the black heights of the column to its left and the column to its right, we can build a Dynamic Programming solution. Dynamic Programming is a method where we solve smaller pieces of the puzzle and store their answers to build up to the final solution.

Let us define our state. As we move from the leftmost column to the rightmost column, we need to keep track of the heights. When we are at column C, we need to know the chosen height for column C itself, and we also need to know the chosen height for the column coming right after it, column C plus 1. 

Wait, you might ask, what about the column before it? The beauty of moving column by column is that we can test all possible heights for the previous column, calculate the score we would get, and just keep the highest possible result. 

Let our memory table store the maximum score we can achieve ending at column C, given that the current column C has a black height of J, and the next column C plus 1 has a black height of K. To figure this out, we need to look back at all possible heights I for the previous column C minus 1.

### Managing the Time Limits with Prefix and Suffix Arrays

If we try to loop through every possible column C, every possible previous height I, every possible current height J, and every possible next height K, we will have four nested loops. Since N can be up to 100, N to the power of 4 operations might run a bit too slowly and hit the time limit. We want to be careful and considerate of the system resources.

We can optimize this by splitting the problem into two distinct cases when we are searching for the best previous height I
* Case One occurs when the previous height I is less than or equal to the next height K. In this situation, the maximum bounding height for our white cells is K. The score we add is based purely on J and K. Since the added score does not depend on I, we can just look for the highest base score from our previous state among all I that are less than or equal to K.
* Case Two occurs when the previous height I is strictly greater than the next height K. Here, the bounding height is I. The score we add depends on I and J. 

To handle these cases quickly, before we loop through all possible next heights K, we can precalculate two helpful lists for our current height J
* A prefix maximum list that keeps a running maximum of the previous states as I increases. This instantly answers Case One.
* A suffix maximum list that keeps a running maximum of the previous states plus the score dependent on I, as I decreases from N down to 0. This instantly answers Case Two.

By preparing these two lists, we remove the need to loop through I for every single K. We simply look up the answers in our lists! This reduces our nested loops to just three, making our algorithm run in roughly N to the power of 3 operations, which is very comfortable for a grid size of 100.

### Step by Step Code Implementation

Let us walk through the process of writing the code. 

First, we need to build our prefix sum table. We create a two dimensional array where the element at column C and row R stores the sum of all grid numbers in that column from the very top down to row R minus 1. This allows us to quickly calculate the sum of values between any two rows.

Next, we set up our memory table for the very first column. For column 0, there is no column to its left. So the previous height is effectively zero. We calculate the score it would get based on its own height J and the next column height K, and store this in our memory table.

Then, we start a loop traversing from column 1 all the way to the second to last column. For each column, we create a brand new memory table to store the updated maximums. 
Inside this column loop, we iterate through every possible height J for the current column. 
For each J, we generate the prefix maximum list and the suffix maximum list that I mentioned earlier. 
After preparing those lists, we iterate through every possible height K for the next column. We calculate the best score by comparing the value from the prefix list and the value from the suffix list, and we store the larger one in our new memory table.

Finally, we update our main memory table with the new one and move to the next column. Once we have processed all columns, our answer is simply the maximum value found in the memory table for the final column, assuming a imaginary column after it has a height of zero.

### The Complete C++ Solution

Here is the code written out clearly. I have added comments throughout to help you match the logic we just discussed with the actual programming instructions. 

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maximumScore(vector<vector<int>>& grid) {
        int n = grid.size();
        
        // Prepare a table to store the sums of columns from top to bottom
        // pref[c][r] will hold the sum of grid[0...r-1][c]
        vector<vector<long long>> pref(n, vector<long long>(n + 1, 0));
        for (int c = 0; c < n; ++c) {
            for (int r = 0; r < n; ++r) {
                pref[c][r + 1] = pref[c][r] + grid[r][c];
            }
        }
        
        // memory_table[j][k] stores the highest score up to the current column
        // where the current column has black height j, and the next has black height k
        vector<vector<long long>> memory_table(n + 1, vector<long long>(n + 1, 0));
        
        // Initialize the base case for the very first column
        for (int j = 0; j <= n; ++j) {
            for (int k = 0; k <= n; ++k) {
                // Since there is no left column, left height is 0
                // Score depends on j and k
                memory_table[j][k] = max(0LL, pref[0][k] - pref[0][j]);
            }
        }
        
        // Traverse the grid column by column starting from the second one
        for (int c = 1; c < n; ++c) {
            vector<vector<long long>> next_memory(n + 1, vector<long long>(n + 1, 0));
            
            for (int j = 0; j <= n; ++j) {
                vector<long long> pref_max(n + 1, 0);
                vector<long long> suff_max(n + 2, -1e18);
                
                // Build the prefix maximum array to optimize our search
                long long current_pref_max = -1e18;
                for (int i = 0; i <= n; ++i) {
                    current_pref_max = max(current_pref_max, memory_table[i][j]);
                    pref_max[i] = current_pref_max;
                }
                
                // Build the suffix maximum array to optimize our search
                long long current_suff_max = -1e18;
                for (int i = n; i >= 0; --i) {
                    current_suff_max = max(current_suff_max, memory_table[i][j] + max(0LL, pref[c][i] - pref[c][j]));
                    suff_max[i] = current_suff_max;
                }
                
                // Determine the best previous states for every possible next height k
                for (int k = 0; k <= n; ++k) {
                    // If we are at the last column, the imaginary next column must have height 0
                    if (c == n - 1 && k > 0) continue; 
                    
                    // Case One where bounding height is dictated by k
                    long long option_one = pref_max[k] + max(0LL, pref[c][k] - pref[c][j]);
                    
                    // Case Two where bounding height is dictated by i
                    long long option_two = suff_max[k + 1];
                    
                    next_memory[j][k] = max(option_one, option_two);
                }
            }
            // Move our calculated states over for the next iteration
            memory_table = next_memory;
        }
        
        // Look through the final memory table to find the absolute highest score
        long long max_score_achieved = 0;
        for (int j = 0; j <= n; ++j) {
            max_score_achieved = max(max_score_achieved, memory_table[j][0]);
        }
        
        return max_score_achieved;
    }
};

```

### Complexity Analysis

It is always good practice to understand the resources our logic consumes. Let us break it down gently.

* Time Complexity is proportional to N to the power of 3. We have an outer loop running N times for each column. Inside, we have a loop running N times for the current height J. Within that, we run a few separate loops that each run N times to build our prefix lists and compute the transitions for K. Because these inner loops are consecutive and not nested within each other, the work done inside the J loop is proportional to N. Therefore, N times N times N gives us a cubic time complexity. For a grid up to 100 by 100, a million operations is very safe and processes almost instantly.
* Space Complexity is proportional to N squared. We store a prefix sum array that is N by N plus one. We also keep a memory table and a temporary next memory table, each of size N plus one by N plus one. All of these require memory proportional to the square of N, which is incredibly lightweight for modern systems.

### Conclusion

I hope this thorough walkthrough helps you visualize the flow of the grid and the thought process behind breaking down complex dependencies into manageable subproblems. Dynamic Programming often feels like magic, but once you define the state clearly and handle the overlapping segments carefully, the pieces fall together beautifully.

Programming is all about learning from one another, so please feel free to take this logic, experiment with it, and see if you can trace the memory table with a small example on paper. It is a fantastic way to solidify the concepts!

Thank you for reading along, and happy coding everyone!
