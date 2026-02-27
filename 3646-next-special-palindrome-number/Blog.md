# Uncovering the Magic of Special Palindromes and How to Build Them From Scratch [#3646]

Hello everyone! Welcome to my blog. Today, I want to share a fun and fascinating journey through a very interesting programming problem. Finding numbers that fit strict patterns is a classic challenge in computer science, and the problem we are exploring today is a wonderful exercise in logic, combinatorics, and string manipulation.

In this post, we will look at how to find the Next Special Palindrome Number. I will explain the problem in plain terms, break down the underlying patterns, and share a working approach in C++ that you can learn from and adapt. My goal is to be as detailed and helpful as possible so that anyone, regardless of their background, can follow along and grasp the concepts. Let us dive right in!

***

## Understanding the Problem Requirements

Before we write any code, we need to completely understand what makes a number a "Special Palindrome". The problem gives us a starting integer, let us call it `n`, and asks us to find the strictly next largest number that satisfies two very strict rules.

### Rule 1 Being a Palindrome
A palindrome is a sequence that reads identical backwards and forwards. In the context of numbers, this means the first digit is the same as the last digit, the second digit is the same as the second to last digit, and so forth. Some simple examples of palindromes are 22, 313, and 4554.

### Rule 2 The Special Frequency Condition
This is the rule that makes the problem so unique. Every single digit present in the number must appear exactly as many times as its numeric value. 
* If the number contains the digit 1, the digit 1 must appear exactly one time in the entire number.
* If the number contains the digit 2, the digit 2 must appear exactly two times.
* If the number contains the digit 5, the digit 5 must appear exactly five times.

Let us look at a few examples to solidify our understanding.

**Example A The Number 22**
Is it a palindrome? Yes, it reads the same forwards and backwards.
Does it meet the frequency condition? The only digit is 2, and we can clearly see it appears exactly two times. Therefore, 22 is a valid special palindrome.

**Example B The Number 33**
Is it a palindrome? Yes.
Does it meet the frequency condition? The digit is 3, but it only appears two times. Since 3 needs to appear exactly three times to be valid, 33 is NOT a special palindrome.

**Example C The Number 212**
Is it a palindrome? Yes.
Does it meet the frequency condition? The digit 2 appears two times, and the digit 1 appears one time. Both digits satisfy their frequency rule. Thus, 212 is a valid special palindrome.

Our goal is simple. Given any starting number `n`, we need to find the smallest special palindrome that is strictly greater than `n`. The constraints tell us that `n` can be up to 10^15, which is a massive number. Because of this large constraint, a simple exhaustive search checking every single number one by one will definitely exceed our time limits. We need a more thoughtful approach.

***

## The Crux of the Logic Digits and Parity

To solve this efficiently, we must stop thinking about searching for the answer, and instead think about generating all possible answers. Because the rules are so incredibly restrictive, there are actually very few valid special palindromes in existence!

Let us analyze the structure of a palindrome and how the frequency rule interacts with it. We need to look at the "parity" of the digit counts. Parity simply refers to whether a count is even or odd.

In any valid palindrome string, the characters form a perfectly symmetrical reflection. Because of this symmetry, almost every character must appear an even number of times. Half of them sit on the left side, and the other half sit mirrored on the right side. The only exception to this rule is the exact middle character in a palindrome of odd length. The middle character is the only character that is allowed to appear an odd number of times.

This gives us a massive clue about which digits we can use!

### The Pool of Even Digits
The digits 2, 4, 6, and 8 dictate that they must appear 2, 4, 6, and 8 times respectively. Since these counts are all even numbers, they fit perfectly into a palindrome! We can split them symmetrically. For instance, if we use the digit 4, we need four 4s. We can place two of them on the left side and two of them mirrored on the right side. 

Because all even digits have even counts, we can theoretically pick any subset of the even digits to include in our number. We might pick just 2. We might pick 4 and 6. We might pick all of them, or we might pick none of them at all.

### The Pool of Odd Digits
The digits 1, 3, 5, 7, and 9 dictate that they must appear an odd number of times. As we established earlier, a palindrome can accommodate a maximum of ONE character with an odd frequency, and that character must act as the exact central pivot of the string.

This means that out of the entire pool of odd digits, we can select at most ONE odd digit to be part of our number. We cannot pick both 1 and 3, because we would have two odd counts, which makes forming a palindrome physically impossible.

### Combining the Pools
We have now unlocked the secret to generating every single valid special palindrome!
A valid special palindrome can be formed by choosing
1. Any combination from the set {2, 4, 6, 8}.
2. Exactly zero or one digit from the set {1, 3, 5, 7, 9}.

By iterating through all possible valid choices, we can build every single special palindrome that exists. Once we generate them all, we can simply sort them and look up the answer for any given `n`.

***

## Designing the Algorithm Step by Step

Now that we understand the combinatorics behind the problem, let us lay out a clear, structured algorithm to generate our numbers.

### Step 1 Defining the Choices
First, we will define our choices. 
We have an array of even digits: 2, 4, 6, 8.
We have an array of odd digits: 1, 3, 5, 7, 9, plus a special placeholder to represent "picking no odd digit". To keep our code clean and free of negative flags, we will use the number 10 as our placeholder for "none".

### Step 2 Iterating Through Subsets
We need to explore every possible subset of our even digits. Since there are four even digits, there are exactly sixteen possible combinations. We can use a simple bitmask loop from 0 to 15 to represent these choices. 
* Bit 0 controls the inclusion of the digit 2.
* Bit 1 controls the inclusion of the digit 4.
* Bit 2 controls the inclusion of the digit 6.
* Bit 3 controls the inclusion of the digit 8.

For every single one of these sixteen subsets, we will then iterate through our six odd digit choices.

### Step 3 Constructing the Left Half
For any given combination of even digits and our single odd digit choice, we need to figure out what characters belong on the left side of the palindrome.

Let us say our bitmask selected the digits 2 and 4, and our odd digit choice is 3.
* The digit 2 requires a total count of two. That means one '2' goes to the left side.
* The digit 4 requires a total count of four. That means two '4's go to the left side.
* The digit 3 requires a total count of three. One '3' must be reserved for the exact middle. The remaining two '3's are split, so one '3' goes to the left side.

Our left side needs to contain the characters '2', '4', '4', and '3'.
We concatenate these together into a string. To ensure we generate permutations correctly later, we must sort this left side string in increasing order first. Sorting "2443" gives us "2344".

We must also keep track of the total length of the resulting palindrome. The maximum value for `n` is 10^15, which is a fifteen digit number. To be safe and capture the immediate next largest number, restricting our generated palindromes to a maximum length of 18 digits is perfectly sufficient. This also ensures our numbers will comfortably fit inside a standard 64 bit integer type.

### Step 4 Generating Permutations
The characters "2344" represent just one possible arrangement for the left half. To find all valid palindromes for this specific digit combination, we need to shuffle these characters into every unique ordering.

We can use standard library tools to systematically generate every single unique permutation of our left string. Since we sorted the string beforehand, generating permutations will naturally produce them in lexicographical order, ensuring we never miss a single arrangement.

### Step 5 Assembling the Full String
For every unique permutation of the left side, we build the full palindrome.
1. Start with the left side string.
2. If we selected an odd digit, append that single odd digit to the string. This acts as our center pivot.
3. Take a copy of the left side string, reverse it entirely, and append it to the end.

If our left permutation was "2344" and our odd digit was 3, we would assemble:
Left side: "2344"
Center pivot: "3"
Right side: "4432"
Full resulting string: "234434432"

Once assembled, we convert this valid string into a 64 bit integer and add it to a collection of valid special palindromes.

### Step 6 Sorting and Searching
After we have completed our exhaustive loops, our collection will contain every possible valid special palindrome within our allowed length limit.

To answer the actual problem query, we simply sort this entire collection in ascending order. Then, we perform a binary search to find the very first number in our collection that is strictly greater than the input `n`. 

***

## The Complete C++ Implementation

Here is a straightforward and friendly C++ implementation of the algorithm we just designed. I have kept the code modular and added clear variable names to make it readable. 

```cpp
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class Solution {
public:
    long long specialPalindrome(long long n) {
        std::vector<long long> validPalindromes;
        
        // Define our available building blocks
        int evenDigits[] = {2, 4, 6, 8};
        
        // We use 10 as a clean placeholder meaning "do not include an odd digit"
        int oddChoices[] = {10, 1, 3, 5, 7, 9}; 

        // There are exactly 16 subsets for 4 items, from binary 0000 to 1111
        for (int mask = 0; mask < 16; ++mask) {
            
            // Loop through all odd digit possibilities
            for (int odd : oddChoices) {
                int totalLength = 0;
                std::string leftHalf = "";
                
                // Process the even digits based on the current mask
                for (int i = 0; i < 4; ++i) {
                    if (mask & (1 << i)) {
                        int digit = evenDigits[i];
                        totalLength += digit;
                        
                        // Half of the required digits go to the left side
                        // We construct the character using '0' + digit
                        leftHalf += std::string(digit / 2, '0' + digit);
                    }
                }
                
                // Process the odd digit if we chose to include one
                if (odd != 10) {
                    totalLength += odd;
                    
                    // Integer division naturally drops the remainder
                    // This gives us exactly half of the remaining items for the left side
                    leftHalf += std::string(odd / 2, '0' + odd); 
                }
                
                // We restrict generation to numbers that comfortably fit in a 64 bit integer
                if (totalLength > 0 && totalLength <= 18) {
                    
                    // Sorting is required before we can safely generate all permutations
                    std::sort(leftHalf.begin(), leftHalf.end());
                    
                    // Generate every unique arrangement of the left side
                    do {
                        std::string fullPalindrome = leftHalf;
                        
                        // Insert the central pivot if an odd digit is active
                        if (odd != 10) {
                            fullPalindrome += std::to_string(odd);
                        }
                        
                        // Create the mirrored right side and append it
                        std::string rightHalf = leftHalf;
                        std::reverse(rightHalf.begin(), rightHalf.end());
                        fullPalindrome += rightHalf;
                        
                        // Convert the constructed string into a number and store it
                        validPalindromes.push_back(std::stoll(fullPalindrome));
                        
                    } while (std::next_permutation(leftHalf.begin(), leftHalf.end()));
                }
            }
        }
        
        // Organize all our discovered special numbers from smallest to largest
        std::sort(validPalindromes.begin(), validPalindromes.end());
        
        // Execute a binary search to find the smallest number strictly greater than n
        auto it = std::upper_bound(validPalindromes.begin(), validPalindromes.end(), n);
        
        return *it;
    }
};

```

---

## Breaking Down the Complexity

It is always important to understand the resources our code consumes. Fortunately, the severe constraints of the problem work highly in our favor here!

### Time Complexity Overview

While the loops might look intimidating at first glance, the overall amount of work being done is remarkably small.
We have a fixed number of outer loops. Specifically, 16 even digit subsets multiplied by 6 odd digit choices, giving us exactly 96 base combinations to process.
For each valid combination, we generate permutations of a string. The length of this left string is at most 9 characters. The number of unique permutations for a string of length 9 with many repeating characters is mathematically small. In total across all combinations, we generate a few thousand numbers.
Generating these permutations, building the strings, and converting them to integers takes a fraction of a millisecond. Sorting the final list of numbers and performing a binary search on it takes negligible time.
Therefore, this approach operates comfortably well within any competitive programming time limit. The complexity is bounded by a small constant factor, making it incredibly fast.

### Space Complexity Overview

The main memory requirement for this algorithm is the container we use to store all the valid special palindromes we discover. As we concluded above, there are only a few thousand of these numbers in total. Storing a few thousand 64 bit integers requires an imperceptibly small amount of memory. Consequently, the space complexity is also modest and completely safe.

---

## Final Thoughts and Key Takeaways

I find problems like this deeply rewarding because they remind us that exhaustive search does not always mean brute forcing the problem space indiscriminately. By taking a moment to analyze the mathematical constraints specifically the parity of digit frequencies we were able to shift our perspective from "checking every number" to "building only the correct numbers".

Understanding combinatorics and utilizing standard library features like string reversal, bit manipulation for subsets, and permutation generation are excellent skills to have in your problem solving toolkit.

I sincerely hope this deep dive was informative, approachable, and helpful to your learning journey. I encourage you to copy the code, run it, tweak it, and see how the permutations print out in your terminal. Learning is all about experimentation and sharing knowledge with the community!

Thank you so much for reading, and happy coding! Let me know if you would like to explore any other fascinating logical puzzles together.
