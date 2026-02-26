# Crack the Double Digit Swap Challenge to Find Almost Equal Numbers [#3267]

Hello everyone! Welcome back to my corner of the internet where we explore interesting programming challenges together. Today I want to share a fascinating problem that I recently encountered. It involves arrays, digit manipulation, and counting pairs. I found the logic behind solving it really enjoyable, and I hope you will find this guide helpful and easy to follow. We are going to build a solution step by step so that by the end of this read, you will have a deep understanding of how everything works under the hood. 

This post is designed to be completely beginner friendly while providing enough depth for seasoned coders to appreciate the mechanics. Grab your favorite beverage, get comfortable, and let us dive into the logic together!

### Understanding the Challenge

Imagine you are given an array of positive integers. Your goal is to find pairs of these numbers that are "almost equal". But what exactly does "almost equal" mean in this context? 

Two numbers are considered almost equal if you can make them identical by swapping their digits. The catch is that you are allowed to perform this digit swapping operation at most twice on one of the numbers. Furthermore, the problem allows numbers to have leading zeros during these operations. If a number has fewer digits than another, we can conceptually pad it with leading zeros to make their lengths match. 

We need to return the total count of valid pairs where the first number appears before the second number in the array, and they meet the almost equal criteria. 



Let us look at an example to make things perfectly clear. Suppose we have the numbers 1023 and 213. At first glance, they look entirely different. However, if we pad 213 with a leading zero, it becomes 0213. 
Now, let us take 1023. 
First, we swap the digit 1 and the digit 0. The number becomes 0123. 
Next, we swap the digit 1 and the digit 2. The number becomes 0213. 
Through exactly two swaps, 1023 has transformed into 0213, which is simply 213. Therefore, 1023 and 213 are almost equal!

### Breaking Down the Rules

Before we write a single line of code, we need to establish the boundaries of our data. The problem constraints usually give us massive clues about how to shape our approach. 

The length of the array is up to 5000 elements. 
The maximum value of any number in the array is strictly less than 10000000 (which is 10 to the power of 7). 

This second constraint is our golden ticket. Because every number is less than 10 to the power of 7, we know for an absolute certainty that no number will ever exceed 7 digits. This means we can standardize all our numbers to a fixed length of 7 digits by imagining leading zeros for the shorter ones. 

For instance, the number 1 becomes 0000001. The number 250 becomes 0000250. This fixed length simplifies our logic immensely because we only ever have to consider 7 positions when swapping digits. 

### The Trap of the Obvious Approach

When faced with a problem involving pairs, our initial instinct is often to compare every single number with every other number. We call this a nested loop approach. For an array of 5000 elements, comparing every pair means roughly 12.5 million comparisons. 

While 12.5 million operations might sound small for modern computers, the cost of checking if two numbers are almost equal is not free. If we try to convert numbers to strings, perform up to two swaps in every possible combination, and then compare them, the operations multiply rapidly. Doing this for every pair can lead to a Time Limit Exceeded error on many competitive programming platforms. 

We need a strategy that processes each number individually, determines its potential almost equal matches, and counts them efficiently. 

### A Fresh Perspective with Maps

Instead of looking forward and comparing our current number to everything ahead of it, what if we look backward? 

We can maintain a frequency map (often called a hash map or dictionary) that keeps track of how many times we have seen each number as we iterate through the array. 

When we look at a new number in the array, we can generate all the unique numbers it could possibly transform into using zero, one, or two swaps. Once we have this list of possible transformations, we simply check our map. If any of those transformations exist in our map, it means we have previously encountered a number that can pair with our current one! We add the count of those previous occurrences to our total answer. 

After checking and counting, we simply add our current original number to the map so it can be a potential match for future numbers in the array. This single pass approach is beautiful, clean, and avoids comparing every pair directly.

### The Magic of Math over Strings

A very common way to swap digits is to convert the integer into a string, swap the characters, and convert it back to an integer. I encourage you to avoid this! String conversions are relatively slow. They require allocating memory and parsing characters. 

Since we are only dealing with numbers, we can use pure mathematics to swap digits. We can use an array of powers of 10 to access any digit. 
Our powers of 10 array will look like this: 1, 10, 100, 1000, 10000, 100000, 1000000.

Suppose we want to find the digit at position "i". We divide our number by the "i"th power of 10, and then take the remainder when divided by 10 (modulo 10). 

Once we have extracted the two digits we want to swap, how do we build the new number? 
We take our original number. 
We subtract the value of the first digit at its original position. 
We subtract the value of the second digit at its original position. 
Then, we add the first digit multiplied by the position of the second digit. 
Finally, we add the second digit multiplied by the position of the first digit. 

This purely mathematical swap is incredibly fast and completely eliminates the overhead of string manipulation. It is a fantastic trick to keep in your programming toolkit.

### Step by Step Algorithm Construction

Let us clearly map out the steps our algorithm will take. 

Step 1. Initialize a counter for our total valid pairs and an empty hash map to store the frequencies of the numbers we process. 
Step 2. Define an array containing the first 7 powers of 10. 
Step 3. Begin a loop that goes through each number in our given array one by one. 
Step 4. For the current number, create a list to store all possible mutations (the numbers it can become). Start by adding the original number to this list, as zero swaps is a valid operation. 
Step 5. Create a nested loop to pick two different positions out of our 7 possible digit positions. This will represent our first swap. 
Step 6. Extract the digits at these two positions. If the digits are identical, swapping them does not change the number, so we can skip this specific swap to save time. 
Step 7. Calculate the new number using our math trick and add it to our list of mutations. 
Step 8. Now we need to account for the second swap. Using the new number we just generated from the first swap, we create another nested loop to pick two positions again. 
Step 9. Extract the digits of this partially swapped number at the new positions. Again, if they are the same, skip. 
Step 10. Calculate the final number after this second swap and add it to our list. 
Step 11. Once all swaps are complete, our list might contain duplicates because different swap combinations can sometimes result in the same number. We must sort the list and remove all duplicate entries. 
Step 12. Iterate through our unique list of mutations. For each mutation, check if it exists in our frequency map. If it does, add its frequency to our total pair counter. 
Step 13. Finally, increment the count of our original current number in the frequency map. 
Step 14. Return the total pair counter.

### Tracing the Code Logic

It is highly beneficial to trace how the deduplication step works. Why do we get duplicates? 

Imagine the number 1122. 
If you swap the first 1 and the second 1, you get 1122. 
If you swap the first 2 and the second 2, you get 1122. 
If you swap the first 1 with the first 2, and then swap them back, you return to 1122. 
Because of these overlapping possibilities, our generation phase will produce many identical values. If we look up the map for each of these identical values separately, we will count the same pairs multiple times, severely inflating our final answer. 

By sorting the generated list and using a unique filter, we guarantee that each potential match is queried exactly once per array element. In C++, this is elegantly handled by combining the `std::sort` function with the `std::unique` function followed by an `erase` call to shrink the container. 

Another subtle but important detail is the capacity of our list. How many elements can our list possibly hold before we remove duplicates? 
For zero swaps, we have exactly 1 possibility. 
For one swap, we choose 2 positions from 7. In combinatorics, 7 choose 2 equals 21 possibilities. 
For two swaps, we perform another 21 possible swaps on each of the 21 first swap outcomes. This gives us 21 multiplied by 21, which equals 441 possibilities. 
Adding them all together, 1 plus 21 plus 441 equals 463 maximum elements. 
Knowing this maximum size allows us to preallocate memory for our list using the `reserve` function, which prevents the list from constantly resizing itself during the loops, giving us a nice performance boost.

### Time and Space Requirements

It is always good practice to evaluate the resource footprint of our solutions. 

For the Time Complexity, we iterate through each of the N elements in our array exactly once. Inside the loop, we perform a fixed number of operations. We do at most 463 mathematical calculations, a sort operation on 463 elements, and at most 463 map lookups. Since 463 is a constant number that never changes regardless of how large our input array gets, the work done per array element is bounded by a constant. Therefore, the overall time complexity scales linearly with the size of the input array. We denote this as Big O of N. 

For the Space Complexity, we are storing elements in our frequency map. In the absolute worst case scenario, every single number in the given array is completely unique, meaning our map will store N distinct key value pairs. The temporary list we use for generating mutations only ever holds up to 463 elements, which is constant space. Therefore, the memory usage grows proportionally with the input size, resulting in a space complexity of Big O of N. 

This linear scaling is wonderful and easily handles the maximum constraint of 5000 elements well within standard time limits.

### The C++ Code

Here is a practical implementation in C++ incorporating all the concepts we just discussed. I have included descriptive variable names to help you map the code directly to the logic explained above. 

```cpp
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    int countPairs(vector<int>& nums) {
        int totalValidPairs = 0;
        unordered_map<int, int> numberFrequency;
        
        int powersOfTen[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};
        
        for (int currentNumber : nums) {
            vector<int> possibleMutations;
            possibleMutations.reserve(463); 
            
            possibleMutations.push_back(currentNumber);
            
            for (int firstPos = 0; firstPos < 7; ++firstPos) {
                for (int secondPos = firstPos + 1; secondPos < 7; ++secondPos) {
                    
                    int digitOne = (currentNumber / powersOfTen[firstPos]) % 10;
                    int digitTwo = (currentNumber / powersOfTen[secondPos]) % 10;
                    
                    if (digitOne == digitTwo) {
                        continue; 
                    }
                    
                    int numberAfterFirstSwap = currentNumber 
                                             - digitOne * powersOfTen[firstPos] 
                                             - digitTwo * powersOfTen[secondPos] 
                                             + digitOne * powersOfTen[secondPos] 
                                             + digitTwo * powersOfTen[firstPos];
                                             
                    possibleMutations.push_back(numberAfterFirstSwap);
                    
                    for (int thirdPos = 0; thirdPos < 7; ++thirdPos) {
                        for (int fourthPos = thirdPos + 1; fourthPos < 7; ++fourthPos) {
                            
                            int digitThree = (numberAfterFirstSwap / powersOfTen[thirdPos]) % 10;
                            int digitFour = (numberAfterFirstSwap / powersOfTen[fourthPos]) % 10;
                            
                            if (digitThree == digitFour) {
                                continue;
                            }
                            
                            int numberAfterSecondSwap = numberAfterFirstSwap 
                                                      - digitThree * powersOfTen[thirdPos] 
                                                      - digitFour * powersOfTen[fourthPos] 
                                                      + digitThree * powersOfTen[fourthPos] 
                                                      + digitFour * powersOfTen[thirdPos];
                                                      
                            possibleMutations.push_back(numberAfterSecondSwap);
                        }
                    }
                }
            }
            
            sort(possibleMutations.begin(), possibleMutations.end());
            possibleMutations.erase(unique(possibleMutations.begin(), possibleMutations.end()), possibleMutations.end());
            
            for (int validMutation : possibleMutations) {
                if (numberFrequency.find(validMutation) != numberFrequency.end()) {
                    totalValidPairs += numberFrequency[validMutation];
                }
            }
            
            numberFrequency[currentNumber]++;
        }
        
        return totalValidPairs;
    }
};
```

### Wrapping Up

I truly hope this detailed walkthrough has shed light on how to confidently tackle problems involving restricted operations and permutations. By looking at the constraints to determine boundaries, leaning on mathematics instead of strings, and creatively using maps to track our history, we turned what seemed like an overwhelming brute force task into a sleek and logically sound solution. 

Remember, solving coding challenges is rarely about finding the answer instantly. It is about the journey of breaking down the requirements, recognizing the potential pitfalls, and gradually layering logic until the picture becomes clear. Take your time, draw things out on paper, and experiment with small examples. 

Thank you so much for reading this guide. 


I would love to hear your thoughts on this approach. Would you like me to elaborate further on any specific part of the digit math or explore similar combinatorics problems next?
