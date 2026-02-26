# Maximizing Board Values by Placing Three Rooks A Friendly Guide [#3257]

Hello everyone! Welcome back to another coding discussion. Today I want to share my thoughts on a very interesting grid puzzle. We will be looking at how to place three chess rooks on a board to get the highest possible score without them attacking each other. 

I really enjoyed working through this logic, and I am excited to share a practical method that works well. We will walk through the problem details, the brainstorming phase, the core logical breakthrough, and finally a clean implementation in C++. Let us dive right in!

## Understanding the Problem

Imagine you are given a rectangular grid. This grid acts like a chessboard, and every single square on this board contains a specific point value. These values can be positive, zero, or even negative numbers. 

Your mission is to place exactly three rooks on this board. In the game of chess, a rook can move horizontally along its row or vertically along its column. Two rooks are considered to be attacking each other if they share the same row or the same column. Therefore, to satisfy the non attacking rule, all three rooks must be placed in entirely distinct rows and entirely distinct columns.



The goal of our puzzle is to choose three valid squares for our rooks so that the sum of the values on those three squares is as large as possible. 

If the board is small, you might think about checking every possible combination of three squares. However, the board can be quite large, with up to five hundred rows and five hundred columns. Searching every single combination of three squares on a massive board would take far too long for a computer to process in a reasonable time. We need to find a way to narrow down our choices.

## Brainstorming and Initial Thoughts

When I first looked at this, my immediate thought was to group all the squares, sort them from largest to smallest, and just pick the top three. But that idea fails quickly because the top three largest numbers on the board might all be in the very same row! If they share a row, placing rooks on them breaks the non attacking rule.

So, we have to respect the row and column boundaries. If we use a brute force method, we would pick a cell, then pick a second cell in a different row and column, and then pick a third cell in a different row and column from the first two. With hundreds of thousands of cells, the number of triplets is astronomical. We clearly need a strategic observation to shrink our list of candidate squares.

## The Crucial Observation The Magic of Three

This is where a beautiful piece of logic comes into play. We are placing exactly three rooks. Let us focus on just one single row for a moment. 

Suppose a row has hundreds of cells. Do we need to consider every single cell in this row as a potential spot for a rook? Let us imagine we decide to place our rook on the fourth highest value in this specific row. 

What happens to the other two rooks we still need to place on the board? Those remaining two rooks will occupy exactly two columns. 



Because the other two rooks only take up two columns, they can block a maximum of two cells in our current row. But wait! We have three cells in our row that have a higher value than the one we chose. Even if the other two rooks block two of our top three cells, there will always be at least one top three cell left completely unblocked! 

Instead of settling for the fourth highest value, we could simply slide our rook over to that unblocked cell which is among the top three. By doing so, we strictly improve our total sum while still following all the rules. 

This leads us to a massive breakthrough. For any row on the entire board, we only ever need to care about the three squares with the largest values. We can completely ignore the fourth highest, fifth highest, and all other smaller values in every single row. 

## Step by Step Algorithm Construction

By applying our discovery, we can build a much more efficient approach. Here are the steps we will follow

* **Step One Narrowing the Field** We will go through the board row by row. For each row, we will identify the three cells with the highest values. We will store these three cells and remember their values, their row indices, and their column indices.
* **Step Two Building the Candidate Pool** We will place all these selected cells from every row into one single list. If our board has M rows, our list will contain exactly M multiplied by three candidates.
* **Step Three Global Sorting** We will sort this entire list of candidates in descending order. The cells with the absolute highest point values will be at the very front of our list.
* **Step Four Smart Iteration** We will use three loops to pick three distinct cells from our sorted candidate list. We will check if they share any rows or columns. If they are completely completely isolated from each other, we calculate their sum.
* **Step Five The Power of Pruning** Because our list is sorted from largest to smallest, we know that as we move further down the list, the values only get smaller. If we add the current cell we are looking at to the next two largest available cells and that theoretical maximum sum is still smaller than the best sum we have already found, we can instantly stop looking! There is no mathematical way to find a better sum further down. This early stopping is what makes our method incredibly fast.

## The C++ Implementation

Here is the complete C++ code that brings our logic to life. I have made sure to keep it clean and readable. We use a custom structure to hold the cell data. To avoid any negative bound issues, we initialize our answer with the absolute minimum possible value a long long integer can hold.

```cpp
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
    struct Cell {
        long long val;
        int r;
        int c;
    };

public:
    long long maximumValueSum(vector<vector<int>>& board) {
        int m = board.size();
        int n = board[0].size();
        
        vector<Cell> candidateCells;
        candidateCells.reserve(m * 3);
        
        for (int i = 0; i < m; ++i) {
            vector<Cell> rowCells(n);
            for (int j = 0; j < n; ++j) {
                rowCells[j] = {(long long)board[i][j], i, j};
            }
            
            if (n > 3) {
                nth_element(rowCells.begin(), rowCells.begin() + 3, rowCells.end(), [](const Cell& a, const Cell& b) {
                    return a.val > b.val;
                });
            }
            
            for (int k = 0; k < 3; ++k) {
                candidateCells.push_back(rowCells[k]);
            }
        }
        
        sort(candidateCells.begin(), candidateCells.end(), [](const Cell& a, const Cell& b) {
            return a.val > b.val;
        });
        
        long long bestSumFound = LLONG_MIN; 
        int totalCandidates = candidateCells.size();
        
        for (int i = 0; i + 2 < totalCandidates; ++i) {
            if (candidateCells[i].val + candidateCells[i+1].val + candidateCells[i+2].val <= bestSumFound) {
                break;
            }
            
            for (int j = i + 1; j + 1 < totalCandidates; ++j) {
                if (candidateCells[i].val + candidateCells[j].val + candidateCells[j+1].val <= bestSumFound) {
                    break;
                }
                
                if (candidateCells[i].r == candidateCells[j].r || candidateCells[i].c == candidateCells[j].c) {
                    continue;
                }
                
                for (int k = j + 1; k < totalCandidates; ++k) {
                    if (candidateCells[i].val + candidateCells[j].val + candidateCells[k].val <= bestSumFound) {
                        break;
                    }
                    
                    if (candidateCells[i].r == candidateCells[k].r || candidateCells[i].c == candidateCells[k].c) {
                        continue;
                    }
                    if (candidateCells[j].r == candidateCells[k].r || candidateCells[j].c == candidateCells[k].c) {
                        continue;
                    }
                    
                    long long currentCombinationSum = candidateCells[i].val + candidateCells[j].val + candidateCells[k].val;
                    if (currentCombinationSum > bestSumFound) {
                        bestSumFound = currentCombinationSum;
                    }
                }
            }
        }
        
        return bestSumFound;
    }
};

```

## Breaking Down the Code Logic

Let us look closely at how the code operates to ensure everything is crystal clear.

First, we define a small container called `Cell`. This object holds the value of the square, its row index, and its column index. Keeping these three pieces of information tied together is very important because we need to verify the non attacking rules later.

Next, we loop through every row in the board. For each row, we copy all its cells into a temporary list called `rowCells`. We need to find the top three cells in this row. Instead of fully sorting the entire row, which does more work than necessary, we use a very handy tool provided by C++ called `nth_element`. This tool partially sorts the list just enough to guarantee that the three largest elements are placed at the very beginning of our `rowCells` list. This is a great way to save processing time.

Once we have isolated the top three cells for a row, we push them into our master list named `candidateCells`.

After visiting all rows, our `candidateCells` list is complete. We then perform a full sort on this master list. We want the absolute most valuable cells sitting at index zero, index one, and so forth.

Finally, we enter our three nested loops. The variable `i` represents our first rook, `j` represents our second rook, and `k` represents our third rook.

Notice the `break` conditions inside each loop. This is our pruning mechanism in action! For example, in the outermost loop, we check if the value of our first chosen rook plus the values of the next two highest available squares is less than or equal to our `bestSumFound`. If it is, there is absolutely no reason to continue running the loops. Even the best possible future combination will not be enough to beat our current record. We immediately break out, saving massive amounts of computational energy.

If the theoretical maximum is still higher than our best record, we proceed to check the coordinates. We use the `continue` keyword to skip any combination where the rooks share a row or a column. If a triplet passes all these coordinate checks, we add their values together and update our `bestSumFound` if this new sum is strictly larger.

## Analyzing the Complexity

It is always good practice to understand how much time and memory a method requires. Let us evaluate our approach.

### Time Complexity Evaluation

Let M represent the number of rows and N represent the number of columns on our board.

In the first phase, we process every single row. For each row, we use the partial sorting tool to find the top three elements. The time it takes to partially sort a row is proportional to the number of columns, which is N. Since we do this for all M rows, the total time for the first phase is proportional to M multiplied by N.

Next, we sort our master list of candidates. The master list contains exactly three times M elements. Sorting this list takes time proportional to M multiplied by the logarithm of M.

Finally, we run our nested loops. Because of our aggressive early breaking strategy, the loops only investigate a very small cluster of elements at the very top of our sorted list. In practice, the loops finish incredibly fast and do not significantly slow down the program.

Therefore, the overall time taken by the program is heavily dominated by the initial board scanning phase, giving us a final time complexity proportional to M multiplied by N. This is very practical and runs smoothly within standard time limits.

### Space Complexity Evaluation

For memory usage, we created a master list to hold our candidate cells. We stored exactly three cells for every row on the board. Therefore, the amount of memory we use scales directly with the number of rows. This gives us a space complexity proportional to M. It is a highly efficient use of memory, especially since we do not create any massive secondary grids.

## Closing Thoughts

I hope this thorough walkthrough helps you understand the inner workings of this puzzle! The leap from checking every possible square to only checking the top three squares in each row is a fantastic example of how finding hidden constraints can drastically simplify a problem.

By combining that logical leap with sorting and smart early stopping, we built a highly capable and friendly solution. Coding puzzles like this are a great way to practice structural thinking.

Thank you for reading along! If you have any thoughts or alternative approaches to share, I would love to hear about them in the community discussions. Keep practicing, and happy coding!


Would you like me to elaborate on any specific section of the blog post to add even more detail?
