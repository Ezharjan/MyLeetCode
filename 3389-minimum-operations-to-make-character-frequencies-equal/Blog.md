# String Frequency Balancing Magic A Friendly Guide [#3389]

Hello everyone! Welcome to another coding exploration. Today I want to share a very interesting string manipulation puzzle that I recently encountered. I am very excited to walk through the thought process with you all. We will explore how to make all characters in a string appear the exact same number of times using a specific set of rules. I will try to be as detailed and clear as possible so that beginners and experienced programmers alike can enjoy the journey. Grab a cup of coffee or tea, and let us dive right into this wonderful puzzle!

### Understanding the Goal

The core task is to take a given string of lowercase English letters and transform it into what the problem calls a "good" string. A string is considered good if every character that appears in the string occurs the exact same number of times. 

For instance, if your string is "aabbcc", it is already good because 'a', 'b', and 'c' each appear exactly twice. If your string is "abac", it is not good because 'a' appears twice, while 'b' and 'c' only appear once.

To achieve this balance, we are allowed to perform three types of operations, and each operation costs exactly one unit of effort
* Delete any character from the string.
* Insert any lowercase English letter into the string.
* Change any character in the string to its immediate next letter in the alphabet. For example, you can change an 'a' into a 'b', or a 'y' into a 'z'. The only restriction here is that you cannot wrap around from 'z' back to 'a'.

Our objective is to find the absolute minimum number of total operations required to make the string good.



### Brainstorming and Initial Thoughts

When we first look at this, we might think about just counting the frequencies of all characters and trying to average them out. However, the third operation completely changes the landscape. Because we can change a letter to its next alphabetical neighbor, the characters are no longer independent! 

Let us analyze the cost of these operations in depth.
Suppose we have an extra 'a' that we do not need, and we are missing a 'b'. 
If we use insertions and deletions independently, we would delete the 'a' (costing one operation) and insert the 'b' (costing one operation) for a total cost of two operations.
But wait! If we just use the third operation to change our extra 'a' into a 'b', it only costs one operation! We just saved an entire operation by linking the surplus of 'a' to the deficit of 'b'.

Now, what about changing an 'a' into a 'c'?
To do this using the shift operation, we would change 'a' to 'b' (one operation) and then 'b' to 'c' (another operation). The total cost is two. 
If we instead deleted the 'a' and inserted the 'c', the cost would also be two! 
This is a massive realization. Shifting a character by more than one position in the alphabet never gives us a better price than simply deleting the old character and inserting the new one. Therefore, we only ever need to consider shifting a character exactly one step to the right. This brilliant limitation means we only need to look at adjacent characters in the alphabet when thinking about transferring surpluses to deficits.

### The Strategy Exploring All Possibilities

Since we want all present characters to have the same frequency, let us call this target frequency F. What could F possibly be?
The minimum possible value for F is 1. The maximum possible value for F could technically be the length of the string, but practically, it makes sense to only test F up to the maximum frequency of any character already existing in the string. If we try to build a frequency higher than what already exists, we are just doing unnecessary insertions.

So, our strategy is to wrap our entire logic in a loop. We will guess that the final uniform frequency is F, checking every possible F from 1 up to the maximum current frequency. For each guessed F, we will calculate the minimum operations needed. At the very end, our answer will be the smallest result found among all our guesses.

For a fixed target frequency F, every letter in the alphabet from 'a' to 'z' has only two choices for its final frequency
1.  It appears exactly zero times (meaning we remove it completely).
2.  It appears exactly F times.

### Introducing Dynamic Programming

Because we only need to worry about interactions between a character and its immediate predecessor in the alphabet, this problem is a textbook candidate for Dynamic Programming. Dynamic Programming is simply a method of solving a large problem by breaking it down into smaller, manageable subproblems and remembering their results.



Let us define a table to keep track of our minimum costs. We will call it `dp`.
We need to track two things for our state
* Which character in the alphabet we are currently processing. Let us use an index `i` from 0 to 25.
* Whether the target frequency for this current character is zero or F. Let us use a variable `t` which is 0 if the target is zero, and 1 if the target is F.

So, `dp[i][t]` will store the minimum operations needed to balance all characters from 'a' up to the character at index `i`, assuming the character at index `i` is aiming for the target condition `t`.

### Calculating the Costs and Transitions

For a given character `i` and a chosen target condition `t`, how do we calculate the cost?

First, we look at the raw cost if we acted completely independently. Let the current amount of character `i` in our string be `C`. Let the target amount be `T`. (Remember, `T` is either 0 or F). The independent cost to make `C` become `T` is simply the absolute difference between `C` and `T`.

Next, we look back at the previous character `i` minus 1. We want to see if we can save any operations by shifting excess characters from `i` minus 1 over to `i`.
To do this, we iterate through the two possible target conditions for `i` minus 1. Let us call the previous condition `prev_t`.
Based on `prev_t`, the previous character had a target `T_prev`.
If the previous character had more copies than its target `T_prev`, that extra amount is a surplus. 
If our current character `i` has fewer copies than its target `T`, that missing amount is a deficit.

The beautiful part is that we can shift characters to cover the deficit! The number of characters we can successfully shift is the minimum of the previous surplus and the current deficit. Because every shifted character saves us exactly one operation compared to the independent cost, we simply subtract this shift amount from our total running cost.

We calculate this for both possible values of `prev_t` and keep the smallest resulting cost to store in our `dp` table.

### Step by Step Walkthrough Example

Let us imagine a small example to solidify this concept. Suppose we have guessed our target frequency F is 2.
We are looking at character 'b' (index 1). Suppose our original string had four 'a's and zero 'b's.
If 'a' targets F (which is 2), it has a surplus of 2.
Now we want 'b' to also target F (which is 2). The independent cost to get 'b' from zero to 2 is two insertions.
But wait! 'a' has a surplus of 2, and 'b' has a deficit of 2. We can shift two 'a's into 'b's. This saves us 2 operations. So instead of doing two deletions for 'a' and two insertions for 'b', we just do two changes. 

### The C++ Code Implementation

Here is a very clean and heavily commented C++ solution based on the logic we just constructed. I have written this carefully to ensure it is readable and easy to follow.

```cpp
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Solution {
public:
    int makeStringGood(string s) {
        // Step 1 Count the occurrences of each character in the string
        vector<int> charCounts(26, 0);
        for (char c : s) {
            charCounts[c - 'a']++;
        }
        
        // Step 2 Find the highest frequency among all characters
        int maxFrequency = 0;
        for (int count : charCounts) {
            maxFrequency = max(maxFrequency, count);
        }
        
        // If the string is totally empty or missing letters, it costs zero
        if (maxFrequency == 0) return 0;
        
        // We start our minimum operations counter with the worst case scenario
        // The worst case is deleting every single character in the string
        int overallMinOperations = s.length(); 
        
        // Step 3 Try every possible target frequency F
        for (int F = 1; F <= maxFrequency; ++F) {
            
            // Create our dynamic programming table for this specific F
            // The table has 26 rows for letters, and 2 columns for target states (0 or F)
            vector<vector<int>> dp(26, vector<int>(2, 0));
            
            // Set up the base cases for the very first letter 'a'
            // If 'a' targets 0, we must delete all 'a's
            dp[0][0] = charCounts[0];
            // If 'a' targets F, the cost is the absolute difference
            dp[0][1] = abs(charCounts[0] - F);
            
            // Step 4 Fill the table for all remaining letters from 'b' to 'z'
            for (int i = 1; i < 26; ++i) {
                
                // Try both target states for the current letter (0 or F)
                for (int t = 0; t <= 1; ++t) {
                    
                    int currentTarget = (t == 0) ? 0 : F;
                    int independentCost = abs(charCounts[i] - currentTarget);
                    
                    // Calculate how many characters we are missing
                    int currentDeficit = 0;
                    if (t == 1 && charCounts[i] < F) {
                        currentDeficit = F - charCounts[i];
                    }
                    
                    int minCostForThisState = 1e9; // Start with a very large dummy number
                    
                    // Look back at the two possible states of the previous letter
                    for (int prev_t = 0; prev_t <= 1; ++prev_t) {
                        
                        int previousSurplus = 0;
                        if (prev_t == 0) {
                            // If previous target was 0, every occurrence is a surplus
                            previousSurplus = charCounts[i - 1];
                        } else {
                            // If previous target was F, surplus is anything above F
                            if (charCounts[i - 1] > F) {
                                previousSurplus = charCounts[i - 1] - F;
                            }
                        }
                        
                        // We save one operation for every character we can shift
                        int operationsSaved = min(previousSurplus, currentDeficit);
                        
                        // Calculate the combined cost
                        int combinedCost = dp[i - 1][prev_t] + independentCost - operationsSaved;
                        
                        // Keep track of the best path
                        minCostForThisState = min(minCostForThisState, combinedCost);
                    }
                    
                    dp[i][t] = minCostForThisState;
                }
            }
            
            // Step 5 Update the global minimum with the best result for the letter 'z'
            int bestForCurrentF = min(dp[25][0], dp[25][1]);
            overallMinOperations = min(overallMinOperations, bestForCurrentF);
        }
        
        return overallMinOperations;
    }
};

```

### Explaining the Complexity Plain and Simple

It is always wonderful to understand how much time and memory our solution requires.

For the time complexity, let us count the loops. First, we loop through the string once to count the characters. This takes time directly proportional to the length of the string, which we can call N. Then, we have an outer loop that runs up to the maximum frequency. In the absolute worst case scenario, the maximum frequency could be N, but realistically it is much smaller. Inside this loop, we iterate through the 26 letters of the alphabet, and for each letter, we do a tiny constant amount of work checking previous states. Because the alphabet size is strictly fixed at 26, the inner loops take a very small constant amount of time. Overall, the time grows in proportion to the string length multiplied by the alphabet size. Since the alphabet size never changes, we generally consider this an extremely fast linear time complexity, denoted commonly as Big O of N.

For the space complexity, we use an array to store the character counts, which takes 26 slots. Then, inside our loop, we create a dynamic programming table that has 26 rows and 2 columns. No matter how incredibly long the input string becomes, the size of our tables never grows beyond dealing with the 26 letters of the English alphabet. Because the memory usage is strictly capped and independent of the input size, we celebrate this as constant space complexity, commonly written as Big O of 1.

### Final Thoughts and Takeaways

What a beautiful problem this is! It perfectly illustrates how recognizing a small constraint like the nature of the shift operation can unlock a gorgeous dynamic programming solution. By proving that we never need to shift a character more than one slot, we avoided what could have been a massively complicated web of interactions and distilled it down into a simple chain linking one letter to the next.

I highly encourage everyone to type out this code, add your own print statements, and watch how the dynamic programming table fills up. Seeing the algorithm calculate the savings in real time is a truly rewarding experience.

Thank you so much for reading along with me. I hope this friendly walkthrough brought clarity and joy to your coding practice. Keep exploring, keep learning, and as always, happy coding!
