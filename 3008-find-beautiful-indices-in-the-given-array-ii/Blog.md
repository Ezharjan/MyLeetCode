# Discovering Beautiful Indices in Strings A Friendly Guide to Pattern Matching and Pointers [#3008]

Hello everyone in the coding community! Today I want to share a detailed walkthrough for an interesting string manipulation challenge. Sometimes we encounter problems that look simple at first glance but require a clever combination of classical algorithms to run successfully within the time limits. I hope this guide helps you understand the underlying concepts clearly and provides a solid foundation for tackling similar string challenges in your own journey.



### Understanding the Problem Requirements

Imagine you are given a massive sequence of text characters. Let us call this our main string. Along with this main string, you are given two smaller target strings, named target A and target B. Finally, you receive a positive number representing a maximum allowed distance. 

Our goal is to find all the starting positions of target A within the main string. However, there is a catch! A starting position of target A is only considered beautiful if we can find at least one occurrence of target B nearby. Specifically, the absolute difference between the starting index of target A and the starting index of target B must be less than or equal to our allowed maximum distance. We then need to return all these beautiful starting indices of target A in increasing order.

Let us look at a concrete example to make things perfectly clear. Suppose our main text is the phrase "isawsquirrelnearmysquirrelhouseohmy". Our target A is the word "my" and our target B is the word "squirrel". Let the allowed maximum distance be 15.

If we scan the text, we find target A at index 16 and index 33. We also find target B at index 4 and index 18. 
Now we check the distance rule.
For the occurrence of "my" at index 16, we can compare it with the "squirrel" at index 4. The distance is 16 minus 4, which equals 12. Since 12 is less than or equal to 15, index 16 is a beautiful index!
For the occurrence of "my" at index 33, we can compare it with the "squirrel" at index 18. The distance is 33 minus 18, which equals 15. This is exactly equal to our allowed distance, making index 33 another beautiful index!

### The Initial Thought Process and Why Brute Force Struggles

When we first approach this problem, the most intuitive method might be to use built in string searching functions. We could write a loop to find every instance of target A. For every instance we find, we could then search the surrounding area of the main string to see if target B is hiding nearby. 

While this logic is completely correct, we must consider the constraints. The main string can be up to half a million characters long. Both target strings can also be quite massive. If we use a simple nested search, our program might end up reading the exact same characters thousands of times. This repetitive checking leads to an explosion in operations, causing the dreaded Time Limit Exceeded error. We need a strategy that reads the characters of the strings as few times as possible.

### A Better Approach Breaking the Problem into Two Pieces

A great strategy for complex problems is dividing them into independent smaller tasks. We can split our challenge into two distinct phases.

Phase 1 is purely about string matching. We need an efficient way to find every single starting index of target A in the main string, and every single starting index of target B in the main string. We want to collect these indices into two separate lists.

Phase 2 is about comparing the lists. Once we have a list of indices for target A and a list of indices for target B, we just need to find out which indices in the first list have a buddy in the second list that falls within the allowed distance.

Let us explore both phases in deep detail.

### Deep Dive into Phase 1 The Knuth Morris Pratt Algorithm

To solve the string matching phase efficiently, we can use a famous technique known as the Knuth Morris Pratt algorithm, often abbreviated as KMP. The magic of KMP is that it searches for a word inside a larger text without ever going backward in the main text. It processes the text sequentially, which saves an enormous amount of time.



How does KMP avoid going backward? It uses a preprocessing step on the target string to build an array called the Longest Proper Prefix which is also Suffix array. We usually just call this the LPS array.

The LPS array tells us how much of the target string we have already matched if we suddenly encounter a mismatch. Imagine you are looking for the pattern "ABABC" and you have successfully matched "ABAB" but the next character is wrong. Instead of starting your search all over again from the second character of the text, the LPS array tells you that the "AB" at the end of what you matched is identical to the "AB" at the beginning of your pattern. You can safely skip checking those two characters again and just resume checking from the third character of your pattern!

Here is how we build the LPS array step by step
1. Create an array of the same length as the pattern, filled with zeros.
2. Use two trackers. One tracker moves through the pattern starting from the second character. The other tracker keeps track of the length of the current longest matching prefix.
3. If the characters at both trackers match, we increase the length tracker and record that value in our array.
4. If they do not match, and our length tracker is greater than zero, we look back in our array to find the next longest possible match. This prevents us from starting completely from scratch.
5. If they do not match and the length tracker is zero, we just record a zero and move our main tracker forward.

Once the LPS array is ready, searching the main text follows a very similar logic. We slide our pattern along the text. When characters match, we advance both. When they mismatch, we use the LPS array to shift our pattern intelligently without resetting our position in the main text. Every time our pattern tracker reaches the end of the pattern, we know we found a complete match! We record the starting index and then use the LPS array one more time to continue searching for overlapping matches.

### Deep Dive into Phase 2 The Two Pointer Technique

After running our KMP algorithm twice, we now possess two lists of integers. List A contains all the starting positions of target A. List B contains all the starting positions of target B. Because of the way we read the main string from left to right, both of these lists are naturally sorted from smallest to largest.

This sorted property is a massive advantage. We do not need to compare every index in List A with every index in List B. Instead, we can use the two pointer method.



We will create a pointer to track our current position in List B. We will start this pointer at the very beginning of the list. Then, we will loop through each index in List A one by one.

For a specific index in List A, our pointer in List B might be pointing to a number that is way too small. Specifically, the number in List B might be smaller than the index in List A minus the allowed distance. If this happens, it means our pointer is lagging too far behind. Because List B is sorted, we know that all future numbers in List B will only get larger. Therefore, we can safely advance our pointer in List B until it catches up to the valid distance window.

Once we advance the pointer to a number that is not too small, we only need to perform one simple check. Is this number in List B small enough to fit within the upper boundary? We check if the number is less than or equal to the index in List A plus the allowed distance. If it is, we have successfully found a valid buddy! We can confidently declare the index from List A as beautiful and add it to our final result. 

We then move on to the next index in List A. The beauty of this approach is that our pointer in List B never resets to zero. It only moves forward. This means we only traverse List B a single time overall, making it incredibly efficient.

### Putting It All Together The C++ Code

Here is a C++ implementation incorporating the ideas we just discussed. I have included comments throughout the code to explain the logic and connect it back to our discussion.

```cpp
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
    // This helper function performs the KMP string matching
    // It takes the main text and the pattern we want to find
    // It returns a list of all starting indices where the pattern occurs
    vector<int> getMatches(const string& text, const string& pattern) {
        int textLength = text.length();
        int patternLength = pattern.length();
        
        // If the pattern is longer than the text, it cannot exist inside it
        if (textLength < patternLength) {
            return {};
        }
        
        // Step 1: Build the LPS array for the pattern
        vector<int> lps(patternLength, 0);
        int lengthTracker = 0;
        int patternIndex = 1;
        
        while (patternIndex < patternLength) {
            if (pattern[patternIndex] == pattern[lengthTracker]) {
                lengthTracker++;
                lps[patternIndex] = lengthTracker;
                patternIndex++;
            } else {
                if (lengthTracker != 0) {
                    lengthTracker = lps[lengthTracker - 1];
                } else {
                    lps[patternIndex] = 0;
                    patternIndex++;
                }
            }
        }
        
        // Step 2: Search the text using the LPS array
        vector<int> matchIndices;
        int textPointer = 0;
        int searchPointer = 0;
        
        while (textPointer < textLength) {
            if (text[textPointer] == pattern[searchPointer]) {
                textPointer++;
                searchPointer++;
                
                // If we have matched the entire pattern
                if (searchPointer == patternLength) {
                    // Calculate the starting index and save it
                    matchIndices.push_back(textPointer - searchPointer);
                    // Use LPS array to look for overlapping matches
                    searchPointer = lps[searchPointer - 1];
                }
            } else {
                // If there is a mismatch
                if (searchPointer != 0) {
                    // Shift the search pointer using the LPS array
                    searchPointer = lps[searchPointer - 1];
                } else {
                    // If we are at the very beginning of the pattern, just advance the text pointer
                    textPointer++;
                }
            }
        }
        
        return matchIndices;
    }

public:
    vector<int> beautifulIndices(string s, string a, string b, int k) {
        // Phase 1: Find all occurrences of string a and string b
        vector<int> indicesA = getMatches(s, a);
        vector<int> indicesB = getMatches(s, b);
        
        vector<int> beautifulList;
        
        // Phase 2: Use the two pointer technique to find valid pairs
        int pointerB = 0;
        int sizeB = indicesB.size();
        
        for (int indexA : indicesA) {
            // Move pointerB forward if the current value is completely out of range to the left
            while (pointerB < sizeB && indicesB[pointerB] < indexA - k) {
                pointerB++;
            }
            
            // Check if the current value at pointerB is within range to the right
            if (pointerB < sizeB && indicesB[pointerB] <= indexA + k) {
                beautifulList.push_back(indexA);
            }
        }
        
        return beautifulList;
    }
};

```

### Complexity Breakdown Time and Space

Understanding the efficiency of our code is very important for competitive programming and technical interviews. Let us analyze how our solution scales as the input sizes grow.

For Time Complexity, building the LPS array for a pattern takes time proportional to the length of that pattern. Searching the main string takes time proportional to the length of the main string. We do this entire process twice, once for target A and once for target B. Finally, the two pointer sweep looks at each element in the index lists at most once. Because the lists can never be larger than the main string itself, this final step also takes linear time.
Combining all these steps, the overall time complexity is linear, specifically O(Length of S + Length of A + Length of B). This is highly efficient and easily passes the strict time limits!

For Space Complexity, we need to allocate memory for the LPS arrays of both target A and target B. We also need memory to store our lists of matching indices. In the absolute worst case scenario, where the main string is just repeating copies of the target strings, our index lists could contain a number of items roughly equal to the length of the main string. Therefore, the extra memory we use scales linearly with the input sizes. The overall space complexity is O(Length of S + Length of A + Length of B).

### Final Thoughts

This problem is a fantastic exercise because it connects different areas of computer science. It takes a raw string manipulation task and turns it into a mathematical sequence comparison task.

I sincerely hope this breakdown provided a clear mental model of how to tackle the beautiful indices challenge. String matching algorithms like KMP can seem daunting when you first look at the code, but once you understand the concept of the longest prefix suffix, they become incredibly logical and rewarding tools to have in your toolbox. The two pointer approach is another brilliant technique that you will find yourself using again and again in all sorts of algorithm problems.

Thank you for reading this guide! Keep practicing, keep analyzing your code, and happy coding to everyone in the community!
