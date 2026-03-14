# Mastering the Art of Counting Substrings with Exact Character Frequencies and Bounded Adjacent Differences [#2953]

Hello everyone in the wonderful coding community! Today I want to share a deep dive into a fascinating string problem. 

This challenge asks us to find specific types of substrings hidden within a larger string. It is a fantastic exercise in string manipulation, algorithmic thinking, and understanding the sliding window technique. We are going to break this down step by step so that beginners and experienced programmers alike can clearly understand the logic and implement a solid solution.

### Grasping the Core of the Problem

Imagine you are given a string of lowercase English letters and a target number. We will call the string "word" and the target number "k". Our goal is to count how many special segments or "substrings" exist inside this word. 

For a substring to be considered special or "complete", it must strictly follow two unique rules.

First rule. Every single character that appears in the substring must appear exactly "k" times. Not more, not less. If our target number "k" is three, and the letter "a" is in our substring, there must be exactly three "a"s. If there is a "b", there must be exactly three "b"s. 

Second rule. The alphabetical distance between any two adjacent letters in the substring cannot be more than two. Let us think about the alphabet. The letter "a" and the letter "c" have a distance of two. The letter "a" and the letter "d" have a distance of three. If a substring contains "a" right next to "d", the entire substring is instantly disqualified. 

We need to return the total count of these complete substrings.

### Breaking the Problem into Manageable Pieces

When faced with two distinct constraints, a good approach is to handle them one at a time. Let us look at the second rule regarding adjacent characters. 

Because a complete substring can never contain two adjacent characters with an alphabetical distance greater than two, we can use this to divide our main string into completely independent blocks. If we find a spot in the string where the difference between the current letter and the next letter is three or more, we know for a fact that no valid substring can ever cross this boundary. 

Therefore, our very first step is to read through the main string and slice it into smaller valid segments. For every segment, the adjacent distance rule is already satisfied! We can then analyze each segment independently to solve the first rule. This drastically simplifies our thought process.

### The Magic of the English Alphabet

Now we are looking at a valid segment where all adjacent letters are close enough to each other. We need to find substrings within this segment where every letter appears exactly "k" times. 

Here is the brilliant part about lowercase English letters. There are only twenty six of them! This fixed small number is our greatest tool. 

If every unique character in a substring must appear exactly "k" times, then the total length of the substring is entirely predictable. If the substring has exactly one unique character, its length must be exactly one multiplied by "k". If the substring has two unique characters, its length must be exactly two multiplied by "k". 

Since there are only twenty six possible unique characters, there are only twenty six possible lengths for a valid substring!

Instead of checking every single possible starting point and ending point which would take too much time, we can simply test each of the twenty six possible unique character counts. 

### The Sliding Window Technique

For each possible number of unique characters from one to twenty six, we calculate the required window length. Let us call the number of unique characters "u". The required length is "u" multiplied by "k".

We can then create a "window" of this exact length and slide it across our valid segment from left to right. As the window moves, we keep track of how many times each letter appears inside it. 

To do this efficiently, we maintain a frequency array of size twenty six initialized to zero. When our window slides forward by one position, exactly one new letter enters the window on the right side, and exactly one old letter leaves the window on the left side. We simply add one to the frequency of the new letter and subtract one from the frequency of the old letter.

We also need to know if the current window perfectly satisfies our frequency rule. Instead of checking the entire frequency array every time the window moves, we can maintain a special counter variable. Let us call it "valid character count". 

When a letter enters the window, its frequency increases. If its frequency becomes exactly "k", we increase our "valid character count" by one. If it was already "k" and becomes "k" plus one, it is no longer valid, so we decrease our "valid character count" by one.

Similarly, when a letter leaves the window, its frequency decreases. If its frequency drops from "k" plus one down to "k", it becomes valid again, so we increase our "valid character count". If it drops from "k" to "k" minus one, it is no longer valid, so we decrease our counter.

At any point, if our "valid character count" is exactly equal to "u", it means we have exactly "u" characters appearing exactly "k" times. Because our window length is strictly "u" multiplied by "k", there is no room for any other extra characters. This guarantees the window is a complete substring! We increment our final answer.

### Tracing an Example Together

Let us walk through a short example to solidify our understanding. Imagine the string is "igigee" and our target number "k" is two.

First, we check the adjacent character distances.
The distance between 'i' and 'g' is two. Valid.
The distance between 'g' and 'i' is two. Valid.
The distance between 'i' and 'g' is two. Valid.
The distance between 'g' and 'e' is two. Valid.
The distance between 'e' and 'e' is zero. Valid.

The entire string "igigee" is a single valid segment! The length of this segment is six.

Now we iterate through the possible number of unique characters "u" from one to twenty six.

Test case where "u" is one. 
The required window length is one multiplied by two, which is two.
We slide a window of size two across "igigee".
Window one is "ig". Frequencies are 'i' is one, 'g' is one. Valid character count is zero. Not complete.
Window two is "gi". Not complete.
Window three is "ig". Not complete.
Window four is "ge". Not complete.
Window five is "ee". Frequencies are 'e' is two. Valid character count is one. Since "u" is one, this perfectly matches! We found one complete substring.

Test case where "u" is two.
The required window length is two multiplied by two, which is four.
We slide a window of size four.
Window one is "igig". Frequencies are 'i' is two, 'g' is two. Valid character count is two. This matches "u"! We found another complete substring.
Window two is "gige". Frequencies are 'g' is two, 'i' is one, 'e' is one. Valid character count is one. Not complete.
Window three is "igee". Frequencies are 'i' is one, 'g' is one, 'e' is two. Valid character count is one. Not complete.

Test case where "u" is three.
The required window length is three multiplied by two, which is six.
We slide a window of size six.
Window one is "igigee". Frequencies are 'i' is two, 'g' is two, 'e' is two. Valid character count is three. This matches "u"! We found another complete substring.

We stop testing larger values of "u" because a window length larger than six cannot fit inside our segment.

Our final answer is three complete substrings. This logical progression is extremely reliable and prevents us from getting lost in complicated edge cases.

### The C++ Implementation

Below is the C++ code that beautifully translates our thought process into instructions for the computer. I have added detailed comments to help explain each step of the process.

```cpp
#include <string>
#include <cmath>
#include <vector>

class Solution {
public:
    int countCompleteSubstrings(std::string word, int k) {
        int totalLength = word.length();
        int totalCompleteSubstrings = 0;
        int leftBoundary = 0;
        
        // We will process the entire string by finding valid segments
        while (leftBoundary < totalLength) {
            int rightBoundary = leftBoundary + 1;
            
            // Expand the segment as long as the adjacent distance is at most two
            while (rightBoundary < totalLength && std::abs(word[rightBoundary] - word[rightBoundary - 1]) <= 2) {
                rightBoundary++;
            }
            
            // We now have a valid segment from leftBoundary to rightBoundary minus one
            int currentSegmentLength = rightBoundary - leftBoundary;
            
            // Iterate over all possible numbers of unique characters
            for (int uniqueChars = 1; uniqueChars <= 26; ++uniqueChars) {
                int windowLength = uniqueChars * k;
                
                // If the required window is larger than our segment we can stop checking
                if (windowLength > currentSegmentLength) {
                    break;
                }
                
                int characterFrequencies[26] = {0}; 
                int charactersWithExactFrequency = 0;    
                
                // Slide the window across the current valid segment
                for (int i = 0; i < currentSegmentLength; ++i) {
                    
                    // Step A Add the new character entering the window on the right
                    int newlyAddedCharIndex = word[leftBoundary + i] - 'a';
                    characterFrequencies[newlyAddedCharIndex]++;
                    
                    // Update our tracker for characters meeting the exact frequency rule
                    if (characterFrequencies[newlyAddedCharIndex] == k) {
                        charactersWithExactFrequency++;
                    } else if (characterFrequencies[newlyAddedCharIndex] == k + 1) {
                        charactersWithExactFrequency--;
                    }
                    
                    // Step B Remove the old character falling out of the window on the left
                    if (i >= windowLength) {
                        int removedCharIndex = word[leftBoundary + i - windowLength] - 'a';
                        characterFrequencies[removedCharIndex]--;
                        
                        // Update our tracker for the removed character
                        if (characterFrequencies[removedCharIndex] == k) {
                            charactersWithExactFrequency++;
                        } else if (characterFrequencies[removedCharIndex] == k - 1) {
                            charactersWithExactFrequency--;
                        }
                    }
                    
                    // Step C Check if the current window state is perfectly complete
                    if (i >= windowLength - 1) {
                        // If the number of perfectly frequent characters matches our target unique count
                        // it means every character in the window appears exactly k times
                        if (charactersWithExactFrequency == uniqueChars) {
                            totalCompleteSubstrings++;
                        }
                    }
                }
            }
            
            // Move our main pointer to the start of the next potential segment
            leftBoundary = rightBoundary;
        }
        
        return totalCompleteSubstrings;
    }
};

```

### Analyzing the Complexity and Efficiency

It is always important to evaluate how our code performs in terms of speed and memory usage.

Let us discuss the Time Complexity first.
The outer loops and the segment finding logic ensure that we process each character of the string to group them into valid segments. This takes a linear amount of time.
For every valid segment, we run an inner loop exactly twenty six times. Inside this inner loop, we slide a window across the segment. The sliding window visits each character in the segment exactly once.
Because the maximum number of unique letters in the English alphabet is twenty six, the total amount of work is proportional to twenty six multiplied by the length of the string. In computer science terms, we express this as Big O of N multiplied by twenty six, which simplifies perfectly to a linear time complexity, or Big O of N, where N is the total length of the word string. This is a very comfortable and well performing time complexity that handles strings up to a hundred thousand characters long without breaking a sweat.

Now let us discuss the Space Complexity.
We always want to write programs that use the computer's memory responsibly. In our approach, we only create a few basic integer variables like pointers and length counters. The only collection of data we create is the integer array to store character frequencies. Since we only deal with lowercase English letters, this array always has a strictly fixed size of exactly twenty six elements.
This means our memory requirement does not grow larger even if the input string becomes massive. A string of length ten requires the same twenty six element array as a string of length one hundred thousand. Thus, our space complexity is perfectly constant, expressed as Big O of 1. This is the gold standard for memory management.

### Wrapping Up

I hope this thorough walkthrough has demystified this problem for you! By breaking the complicated rules into two separate phases and leveraging the fixed size of the alphabet, we turned a daunting challenge into an elegant sliding window solution.

Remember that in string problems, whenever you are dealing with character counts, always keep the fixed size of the alphabet in mind. It is a powerful limitation that often unlocks linear time solutions.

Keep practicing, keep exploring different logic puzzles, and remember that every problem is just an opportunity to learn a new technique. 

Happy coding everyone, and feel free to ask questions or share your own thoughts on this approach!
