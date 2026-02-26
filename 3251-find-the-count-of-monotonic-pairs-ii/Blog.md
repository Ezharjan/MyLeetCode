# Learn How to Count Valid Monotonic Array Pairs from an Integer Sequence [#3251]

Hello fellow developers and problem solvers! Welcome back to another algorithm discussion. Today I want to share my thoughts and a detailed walkthrough for a very interesting array problem. I found this challenge to be a wonderful learning opportunity, and I am excited to share a friendly and accurate guide to help everyone understand the underlying logic. 

Whether you are a beginner looking to grasp dynamic programming or an experienced coder brushing up on optimization techniques, this guide is designed to be comprehensive and easy to follow.

### Understanding the Core Rules

The problem provides us with a single array of positive numbers. Let us call this the input array. Our goal is to figure out how many valid pairs of new arrays we can create based on a specific set of rules. 

We need to generate two new arrays, let us call them Array A and Array B. Both of these new arrays must have the exact same length as our input array. Furthermore, they must strictly contain positive integers or zeros.

Here are the critical conditions these two arrays must satisfy

1. Array A must be monotonically increasing or flat. This means as you read the array from left to right, the numbers can stay the same or go up, but they can never ever go down.

2. Array B must be monotonically decreasing or flat. This means as you read the array from left to right, the numbers can stay the same or go down, but they can never ever go up.

3. The most important rule links them all together. If you pick any index, the number in Array A at that index plus the number in Array B at that exact same index must exactly equal the number in our original input array at that index.

If all these conditions are met, we consider the pair of arrays to be a valid monotonic pair. Because the total number of valid pairs can be exceptionally large, the problem asks us to return the final count modulo one billion and seven.

### A Conceptual Example

Let us look at a simple input array containing the numbers 2, 3, and 2. 

We need to find ways to build Array A and Array B. 

Imagine Array A starts with 0. 
Since the first number in the input array is 2, Array B must start with 2. 

Moving to the next index, the input number is 3. Array A must be equal to or greater than 0. Let us say we pick 1 for Array A. 
This forces Array B to be 2, because 1 plus 2 equals 3. 
We must check if this is valid. Array A went from 0 to 1, which is allowed. Array B went from 2 to 2, which is also allowed since it did not go up.

Moving to the final index, the input number is 2. Array A must be equal to or greater than 1. Let us say we pick 2.
This forces Array B to be 0, because 2 plus 0 equals 2.
Array A went from 1 to 2, which is valid. Array B went from 2 to 0, which is valid.

So, the arrays [0, 1, 2] and [2, 2, 0] form one valid pair! We would need to count all such valid combinations.

### The Initial Thought Process

When first looking at this, one might think about trying every single combination of numbers. You could write a program that guesses every possible number for Array A, calculates Array B, and then checks if the rules are followed. 

While this thought process is a great starting point, it leads to performance issues. As the input array gets longer and the numbers get bigger, the amount of guessing grows exponentially. A guessing approach would take far too long to compute. We need a more structured way to count the possibilities without evaluating every single one individually.

### Leveling Up with Dynamic Programming

Think of dynamic programming as remembering the past so you do not have to repeat your calculations. We can process the input array one element at a time, keeping track of how many valid array combinations we have formed up to that point.

Let us define a grid to store our memory. We will call it our memory table. 
The rows of our memory table will represent the index in our input array. 
The columns of our memory table will represent the chosen value for Array A at that specific index.
The value stored inside the memory table will be the total number of valid pairs formed up to that point.

To figure out the number of valid pairs at the current index if we choose a specific value for Array A, we need to look back at the previous index. We must sum up all the valid pairs from the previous index where the previous value for Array A was smaller than or equal to our current choice.

But remember, we also have a constraint from Array B! Array B must never increase. 
The current value for Array B is simply the current input number subtracted by the current value for Array A.
The previous value for Array B is the previous input number subtracted by the previous value for Array A.

To ensure Array B never increases, the current value for Array B must be less than or equal to the previous value for Array B.
Through some basic algebra substitution, this creates a strict upper limit on what the previous value for Array A could have been.

Specifically, the previous value for Array A must be less than or equal to the current value for Array A subtracted by the difference between the current input number and the previous input number. 

If this difference is a negative value, we just treat the difference as zero. Let us call this boundary value the "difference gap".
So, to fill out our memory table for a current value, we sum up all the values in the previous step from zero up to the current value subtracted by the difference gap.

### The Magic of Running Totals

If we use a standard loop to sum up the valid combinations from the previous step, our algorithm will still be a bit too slow. We would be adding the same numbers over and over again for each column in our memory table.

This is where a running total comes to the rescue! 

A running total, often called a prefix sum, is a separate array where each element stores the sum of all elements before it. By building a running total of the previous row in our memory table, we can find the sum of any range of previous combinations instantly. This brilliant trick completely eliminates the need for an inner loop, making our logic incredibly fast.

### Saving Memory Space

As we fill out our memory table row by row, you might notice something interesting. We only ever need the values from the immediately preceding row to calculate the current row. Once a row is calculated, the older rows are never looked at again.

Instead of keeping a massive grid in memory, we can just use two single arrays. One array holds the previous step, and the other array holds the step we are currently calculating. Once we finish the current step, we simply swap their roles. This simple adjustment saves a massive amount of memory and makes our solution very lightweight.

### Fun with Bitwise Subtraction

As a fun educational twist for the code implementation below, I have decided to write a custom subtraction function using bitwise logic. This completely avoids using standard minus signs in the code, which is an excellent way to deepen your understanding of how computers handle arithmetic behind the scenes. 

By taking the bitwise NOT of a number and adding one, we get its two's complement. Adding this to our first number effectively performs a subtraction. It is a neat little trick to know!

### The Code Implementation

Here is my C++ implementation utilizing the logic we just discussed. Notice how we use the running totals to speed up the counting process and alternating arrays to save memory space.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

struct Solution {
    int bitwiseSubtract(int a, int b) {
        return a + (~b) + 1;
    }

    int countOfPairs(vector<int>& nums) {
        int n = nums.size();
        int maxVal = 0;
        
        for (int i = 0; i < n; i++) {
            int x = nums[i];
            maxVal = max(maxVal, x);
        }
        
        vector<long long> previousDp(maxVal + 1, 0);
        vector<long long> currentDp(maxVal + 1, 0);
        vector<long long> runningTotal(maxVal + 1, 0);
        
        for (int j = 0; j <= nums[0]; j++) {
            previousDp[j] = 1;
        }
        
        const int moduloValue = 1000000007;
        
        for (int i = 1; i < n; i++) {
            runningTotal[0] = previousDp[0];
            
            for (int j = 1; j <= maxVal; j++) {
                int prevJ = bitwiseSubtract(j, 1);
                runningTotal[j] = (runningTotal[prevJ] + previousDp[j]);
                
                if (runningTotal[j] >= moduloValue) {
                    runningTotal[j] = bitwiseSubtract(runningTotal[j], moduloValue);
                }
            }
            
            int prevIndex = bitwiseSubtract(i, 1);
            int inputDifference = bitwiseSubtract(nums[i], nums[prevIndex]);
            int differenceGap = max(0, inputDifference);
            
            fill(currentDp.begin(), currentDp.end(), 0);
            
            for (int j = differenceGap; j <= nums[i]; j++) {
                int targetIndex = bitwiseSubtract(j, differenceGap);
                currentDp[j] = runningTotal[targetIndex];
            }
            
            previousDp.swap(currentDp);
        }
        
        long long totalValidPairs = 0;
        int finalIndex = bitwiseSubtract(n, 1);
        
        for (int j = 0; j <= nums[finalIndex]; j++) {
            totalValidPairs = (totalValidPairs + previousDp[j]);
            
            if (totalValidPairs >= moduloValue) {
                totalValidPairs = bitwiseSubtract(totalValidPairs, moduloValue);
            }
        }
        
        return totalValidPairs;
    }
};

```

### Complexity Breakdown

Understanding the performance of our logic is just as important as writing the code itself. Here is a clear breakdown of the resource requirements.

* Time Complexity
The time it takes our code to run is directly proportional to the length of the input array multiplied by the maximum value found inside that array. Because we used the running total technique, we avoided nested loops that would have drastically slowed things down. We only perform a handful of operations for each possible value at each step. This results in a Time Complexity of Big O of N multiplied by M, where N represents the total elements in the input and M represents the highest integer value present. This runs blisteringly fast and handles large constraints effortlessly.
* Space Complexity
By implementing the rolling array memory trick, we completely discarded the need for a huge two dimensional grid. We only allocate three separate lists that scale with the maximum input value. The length of the input array no longer dictates our memory usage. Therefore, our Space Complexity is simply Big O of M. This ensures our footprint remains extremely small.

### Final Words

I sincerely hope this detailed exploration clears up any confusion surrounding this wonderful challenge! Breaking down complex constraints into manageable bounds is an incredibly rewarding skill to cultivate.

By applying dynamic programming alongside mathematical boundaries and prefix sums, we transformed an overwhelmingly large counting problem into a swift and elegant procedure.

Thank you for reading along with my learning journey. Keep practicing, stay curious, and happy coding to everyone in the community!
