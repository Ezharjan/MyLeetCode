# Riding the Waves of Digits A Friendly Guide to Solving Total Waviness of Numbers in Range II [#3753]

Hello fellow developers and programming enthusiasts! Today I want to share my thoughts and a working solution for a very fascinating digit manipulation problem. We will be exploring the concept of waviness within numbers and how we can efficiently count this property across a massive range of values.

I am writing this guide to share my learning process with the community. I hope you find it helpful, clear, and easy to understand regardless of your experience level. We will walk through the problem statement, the underlying logic, the state design, and finally the code implementation together.

### Understanding the Problem

The problem asks us to calculate the total waviness for all numbers within an inclusive range starting from a number `num1` and ending at a number `num2`. 

But what exactly is waviness? The waviness of a number is the total count of its peaks and valleys. Let us break down those terms

* A digit is considered a peak if it is strictly greater than both the digit immediately to its left and the digit immediately to its right.
* A digit is considered a valley if it is strictly less than both the digit immediately to its left and the digit immediately to its right.
* The very first digit and the very last digit of a number can never be a peak or a valley because they do not have two neighbors.
* Any number that has fewer than three digits will always have a waviness of zero.

Our goal is to sum up the waviness of every single number from `num1` up to `num2`. 

To make this concrete, let us look at the number 4848. The second digit is 8. Its neighbors are 4 and 4. Since 8 is strictly greater than both 4s, the digit 8 is a peak. The third digit is 4. Its neighbors are 8 and 8. Since 4 is strictly less than both 8s, the digit 4 is a valley. So the number 4848 has one peak and one valley, giving it a total waviness of 2.

### Why We Cannot Just Count One by One

The constraints of this problem tell us that the upper bound `num2` can be as large as 10 to the power of 15. That is a massive number! If we tried to write a simple loop that checks every single integer from `num1` to `num2`, our program would take an impossibly long time to finish. 

Whenever we see a problem asking us to count properties of numbers over a massive range, it is a strong signal that we should use a technique called Digit Dynamic Programming. This technique allows us to build numbers digit by digit from left to right, grouping similar prefixes together to avoid redundant calculations.



### The Core Idea of Digit Dynamic Programming

Instead of solving the problem for the specific range between `num1` and `num2` directly, we can simplify our lives by creating a helper function. Let us call this helper function `solve(X)`. This function will calculate the total waviness for all numbers from 1 up to `X`. 

Once we have this helper function, finding the answer for the range `num1` to `num2` becomes a simple subtraction. We calculate the total waviness from 1 to `num2` and then subtract the total waviness from 1 to `num1 minus 1`. This effectively leaves us with the total waviness exactly within our desired boundaries.

### Designing Our State

To build numbers digit by digit and keep track of our progress, we need to define a state. A state represents a specific scenario during our number building process. If we encounter the exact same scenario again, we can just reuse our previously calculated result instead of doing all the hard work twice.

Here are the variables we need to keep track of in our state

* **Current Index** We need to know which position in the number we are currently filling. We start at position 0 representing the leftmost digit.
* **Is Less Flag** We need a boolean value to tell us if the digits we have placed so far make our newly built number strictly smaller than the prefix of our maximum boundary number `X`. If this flag is true, we are free to place any digit from 0 to 9. If it is false, we are restricted and cannot place a digit larger than the corresponding digit in `X`.
* **Is Started Flag** This is crucial for handling leading zeros. A number like 0056 is just 56. The zeros at the beginning should not be counted as valleys or peaks. This flag tells us if we have placed our first nonzero digit yet.
* **Digit One Step Back** To check for peaks and valleys, we need to know the digit we placed immediately before the current one.
* **Digit Two Steps Back** We also need to know the digit placed two steps before the current one. With the current digit, the digit one step back, and the digit two steps back, we have a window of three digits, which is exactly what we need to verify a peak or a valley.

Because we need to handle cases where these previous digits do not exist yet, we can use the number 10 as a special placeholder value indicating an empty spot.

### The Double Return Value

This is where the problem gets wonderfully tricky. In standard counting problems, our recursive function usually returns just one thing, which is the total count of valid numbers. 

However, in this problem, simply returning the waviness is not enough. When we place a new digit and create a peak or a valley, that single peak or valley will be repeated for every single valid combination of digits we can place in the remaining empty spots to the right. 

Therefore, our recursive state needs to return two separate pieces of information
1. The total count of valid ways to finish building the number from the current state.
2. The total accumulated waviness of all those valid ways.

When we find a new peak or valley, we calculate the new waviness by taking the total waviness of the subproblems and adding the count of valid suffixes. This perfectly distributes our newly found peak or valley across all the numbers that will branch out from our current position.

### Step by Step Transition Logic

Let us imagine we are inside our recursive function. We are currently trying to place a digit at our current index. Here is the friendly breakdown of what we do

First, we determine our upper limit for the current digit. If our `Is Less Flag` is true, our limit is 9. Otherwise, our limit is the digit at the current index in our target string.

Next, we loop through all possible digits from 0 up to our limit. For each possible digit, we evaluate what our next state will look like. 

We check if the new digit makes our prefix strictly less than the boundary. We also check if placing this digit means our number has officially started. 

Then comes the fun part tracking the previous digits. If our number has not started yet, we do not record any previous digits unless the current digit is greater than zero. If the number has started, we slide our window. The digit that was one step back becomes the digit two steps back, and our newly placed digit becomes the new digit one step back.

If we have all three digits available in our window, we perform a simple check. If the middle digit is greater than both outer digits, we found a peak. If the middle digit is less than both outer digits, we found a valley. If we found either, we note that this specific placement adds a waviness value of 1.

We recursively call our function to get the results for the next index. We add the returned count to our running total of counts. For the running total of waviness, we add the returned waviness plus the count multiplied by our noted waviness value.

After checking all valid digits, we save our total count and total waviness into a memoization table so we never have to compute this specific state combination again. Finally, we return these two values up the chain.

### The C++ Code Implementation

Here is a clean and objective implementation in C++ based on the logic we just discussed. I have kept it as straightforward as possible so you can trace the logic easily.

```cpp
#include <string>
#include <vector>
#include <cstring>

using namespace std;

class Solution {
    long long memoCount[20][2][2][11][11];
    long long memoWaviness[20][2][2][11][11];

    pair<long long, long long> buildNumber(int currentIndex, bool isLess, bool isStarted, int digitTwoBack, int digitOneBack, const string& targetLimit) {
        if (currentIndex == targetLimit.length()) {
            return {1, 0};
        }
        
        if (memoCount[currentIndex][isLess][isStarted][digitTwoBack][digitOneBack] != -1) {
            return {
                memoCount[currentIndex][isLess][isStarted][digitTwoBack][digitOneBack], 
                memoWaviness[currentIndex][isLess][isStarted][digitTwoBack][digitOneBack]
            };
        }
        
        int currentLimit = isLess ? 9 : (targetLimit[currentIndex] - '0');
        long long totalValidSuffixes = 0;
        long long totalAccumulatedWaviness = 0;
        
        for (int candidateDigit = 0; candidateDigit <= currentLimit; ++candidateDigit) {
            bool nextIsLess = isLess || (candidateDigit < currentLimit);
            bool nextIsStarted = isStarted || (candidateDigit > 0);
            
            int nextDigitTwoBack = 10;
            int nextDigitOneBack = 10;
            long long currentPlacementWaviness = 0;
            
            if (!isStarted) {
                if (candidateDigit > 0) {
                    nextDigitTwoBack = 10;
                    nextDigitOneBack = candidateDigit;
                } else {
                    nextDigitTwoBack = 10;
                    nextDigitOneBack = 10;
                }
            } else {
                nextDigitTwoBack = digitOneBack;
                nextDigitOneBack = candidateDigit;
                
                if (digitTwoBack != 10) {
                    bool isPeak = (digitTwoBack < digitOneBack) && (digitOneBack > candidateDigit);
                    bool isValley = (digitTwoBack > digitOneBack) && (digitOneBack < candidateDigit);
                    if (isPeak || isValley) {
                        currentPlacementWaviness = 1;
                    }
                }
            }
            
            pair<long long, long long> nextStateResult = buildNumber(currentIndex + 1, nextIsLess, nextIsStarted, nextDigitTwoBack, nextDigitOneBack, targetLimit);
            
            totalValidSuffixes += nextStateResult.first;
            totalAccumulatedWaviness += nextStateResult.second + (nextStateResult.first * currentPlacementWaviness);
        }
        
        memoCount[currentIndex][isLess][isStarted][digitTwoBack][digitOneBack] = totalValidSuffixes;
        memoWaviness[currentIndex][isLess][isStarted][digitTwoBack][digitOneBack] = totalAccumulatedWaviness;
        
        return {totalValidSuffixes, totalAccumulatedWaviness};
    }

    long long calculateTotalForLimit(long long limitValue) {
        if (limitValue <= 0) return 0;
        string limitString = to_string(limitValue);
        
        memset(memoCount, -1, sizeof(memoCount));
        memset(memoWaviness, -1, sizeof(memoWaviness));
        
        return buildNumber(0, false, false, 10, 10, limitString).second;
    }

public:
    long long totalWaviness(long long num1, long long num2) {
        return calculateTotalForLimit(num2) - calculateTotalForLimit(num1 - 1);
    }
};

```

### Analyzing the Complexity

When evaluating our code, it is helpful to look at the Time and Space Complexity. I am a big believer in writing out exactly why our solution is fast enough.

For Time Complexity, the speed of our solution is directly proportional to the total number of unique states we can visit, multiplied by the work done inside each state.
Let us calculate the maximum number of states. The index can go up to 18 because 10 to the power of 15 has 16 digits, and we allow a little padding. The boolean flag for being less has 2 possibilities. The boolean flag for starting has 2 possibilities. The previous two digits each have 11 possibilities ranging from 0 to 10.
Multiplying these together gives us 18 multiplied by 2 multiplied by 2 multiplied by 11 multiplied by 11. This results in just a few thousand unique states. Inside each state, we loop at most 10 times. This means our program performs a very small number of operations, making it incredibly fast and well within the time limits. We can express this as a constant Big O of 1 operation relative to the maximum digit length.

For Space Complexity, the memory we use is entirely determined by our memoization arrays. Since the arrays perfectly mirror the dimensions of our state space, they require a similarly small and fixed amount of memory. Therefore, the Space Complexity is proportional to the number of digits in the maximum constraint, which takes up barely any memory at all.

### Final Thoughts

Digit Dynamic Programming can seem intimidating at first because there are so many state variables to track. However, if you break it down logically like we did today, it becomes a very systematic and enjoyable process. By defining clear rules for what happens when a number starts, and maintaining a sliding window of the past two digits, we elegantly conquered the waviness problem.

I hope this detailed walkthrough brings you clarity and helps you in your coding journey. Happy learning, and keep enjoying the logical puzzles that programming provides!
