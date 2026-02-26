# Cracking the Beautiful Substrings Code Finding Equal Vowels and Consonants with Divisibility [#2949]

## Hello and Welcome Everyone

Greetings fellow developers and programming enthusiasts! Today I am very excited to share a comprehensive walkthrough for a fascinating string manipulation problem. Whether you are practicing for interviews or simply enjoy sharpening your problem solving skills, this challenge offers a fantastic opportunity to blend string processing with some elegant mathematical logic. 

In this blog post, we will dive deep into finding beautiful substrings within a given text. We will explore the problem requirements thoroughly, discuss why simple approaches might fall short, and then build a very solid and effective solution together. My goal is to make every step incredibly clear so that anyone reading can fully grasp the concepts and apply them to similar challenges. Let us begin our journey into the world of strings and divisibility!

## Unpacking the Problem Requirements

Before we can solve any problem, we must completely understand what is being asked. We are given a string composed exclusively of lowercase English letters, along with a positive whole number called k. Our mission is to count the total number of non empty contiguous sequences of characters (which we call substrings) that satisfy two specific rules. If a substring meets both rules, we consider it to be beautiful.

Here are the two rules a substring must follow to be beautiful:

1. The number of vowels must be exactly equal to the number of consonants. For reference, the vowels in the English alphabet are 'a', 'e', 'i', 'o', and 'u'. Every other letter is considered a consonant.
2. If you multiply the number of vowels by the number of consonants, the resulting product must be perfectly divisible by our given number k. In other words, the product modulo k must equal zero.

To make this concrete, imagine we have a substring with two vowels and two consonants. The first rule is satisfied because two equals two. The product of vowels and consonants is four. If our given number k is two, then four is perfectly divisible by two, meaning the second rule is also satisfied. Thus, that substring is beautiful.

## Why a Simple Loop is Not Enough

When first looking at this challenge, a very natural thought is to check every possible substring one by one. You could use an outer loop to set the starting position and an inner loop to expand the substring character by character. As you expand, you would count the vowels and consonants, and whenever both rules are met, you increase your beautiful substring tally.

While this logic is completely correct and works perfectly for small strings, we must look at the constraints. The length of our string can be up to fifty thousand characters. If we check every possible starting and ending point, the number of operations scales quadratically. This is often referred to as O(N squared) time complexity. 

For a string of fifty thousand characters, an O(N squared) approach would require roughly over a billion operations. Most modern coding platforms will stop your program and throw a Time Limit Exceeded error if you try to run that many operations. Therefore, we need to think creatively and find a way to count the beautiful substrings in a single pass through the string.

## The Mathematical Secret Let Us Simplify

To avoid checking every single substring, we need to uncover the hidden mathematical properties of the two rules. Let us break them down using plain logic.

Let us represent the total length of any valid beautiful substring with the letter L. 
Because of the first rule, the number of vowels and the number of consonants must be identical. This immediately tells us that the total length L must be an even number. Furthermore, the number of vowels will always be exactly half of the total length, which we can write as L divided by 2. The number of consonants is also L divided by 2.

Now let us look at the second rule. The product of vowels and consonants must be divisible by k. 
Since both vowels and consonants equal L divided by 2, their product is simply (L divided by 2) multiplied by (L divided by 2). 

Let us introduce a new variable called v to represent the count of vowels. We know that v equals L divided by 2. 
The second rule translates to asking: when is v multiplied by v perfectly divisible by k?

Since the value of k is relatively small (up to one thousand), we can easily find the smallest positive whole number that satisfies this condition. Let us call this smallest number x. We just need to find the smallest x where x multiplied by x is a multiple of k. 

Once we find this magic number x, any valid vowel count v must be a multiple of x. Since the total length L is twice the vowel count v, the total length L of any beautiful substring must always be a multiple of 2 multiplied by x. 

This is a massive breakthrough! It means we no longer need to check every substring length. We only care about substring lengths that are exact multiples of our calculated value (2 multiplied by x).

## The Magic of State Tracking

Now that we know the length constraint, we need a fast way to check the first rule: ensuring vowels and consonants are equal. 

We can process the string character by character from left to right. As we move along, we want to keep track of the balance between vowels and consonants. If we encounter a vowel, we tilt the balance in one direction. If we encounter a consonant, we tilt it in the opposite direction. 

If we ever find ourselves at a balance point that we have seen before in the past, it means the characters added between that past moment and our current moment contain exactly the same number of vowels and consonants!



To combine both rules into a single check, we need to track a combined state. This state consists of two pieces of information:
1. The current balance between vowels and consonants.
2. The current position's remainder when divided by our magic length requirement (2 multiplied by x).

If we store this combined state in a hash map as we traverse the string, we can simply look up how many times we have seen this exact same state before. Every time we see a matching past state, it guarantees that the substring ending at our current position and starting just after that past position is beautiful. We simply add the frequency of that past state to our total answer.

## Step by Step Code Implementation

To write this in C++ without any formatting issues, we need to be clever about how we represent our balance. Usually, developers subtract one for consonants, but we can completely avoid subtraction logic. 

We will maintain a balance variable initialized to the length of the string to ensure it never drops below zero. When we see a vowel, we increase the balance by one. When we see a consonant, we want to decrease the balance by one. To decrease a value by one without using standard subtraction characters, we can add the bitwise NOT of zero to our balance. In computer science, adding the bitwise NOT of zero is mathematically equivalent to decreasing the value by one.

Next, we need to encode our two pieces of state information into a single integer key for our hash map. We can take our balance, shift it to the left by 12 bits, and then use the bitwise OR operator to combine it with the positional remainder. We shift by 12 bits because our remainder will never exceed a few thousand, which easily fits within 12 binary bits, ensuring the two pieces of information never overlap.

## The C++ Source Code

Here is the complete implementation of our thought process in C++. I have made sure the code is extremely clean and straightforward.

```cpp
#include <string>
#include <unordered_map>

using namespace std;

class Solution {
public:
    long long beautifulSubstrings(string s, int k) {
        int x = 1;
        while ((x * x) % k != 0) {
            x++;
        }
        
        int mod_val = 2 * x;
        int n = s.length();
        
        unordered_map<int, int> counts;
        counts.reserve(n + 1);
        
        int initial_diff = n;
        counts[(initial_diff << 12) | 0] = 1; 
        
        long long ans = 0;
        int diff = n; 
        
        for (int i = 0; i < n; i++) {
            char c = s[i];
            
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                diff++;
            } else {
                diff = diff + (~0); 
            }
            
            int mod = (i + 1) % mod_val;
            
            int key = (diff << 12) | mod;
            
            ans += counts[key];
            
            counts[key]++;
        }
        
        return ans;
    }
};

```

## Walkthrough of a Real Example

To ensure this crystal clear, let us manually trace the execution using a small example string "baeyh" with k equal to 2.

First, the program calculates the magic number x. It checks if 1 multiplied by 1 is divisible by 2. It is not. It increments x to 2. It checks if 2 multiplied by 2 is divisible by 2. Four is divisible by two, so x becomes 2. The required length multiple, which we called mod_val, is 2 multiplied by 2, yielding 4.

The string has a length of 5. We initialize our hash map and insert our starting state.
Our starting balance is 5. We shift 5 by 12 bits and combine it with a remainder of 0. We record that we have seen this state exactly 1 time.

Now we iterate through the characters:

Index 0 is the character 'b'. It is a consonant. We decrease the balance by one, making it 4. The position index plus one is 1. The remainder of 1 divided by 4 is 1. We create our key using balance 4 and remainder 1. This state is not in our hash map, so we add 0 to our answer. We then record this state in our hash map.

Index 1 is the character 'a'. It is a vowel. We increase the balance by one, returning it to 5. The position index plus one is 2. The remainder of 2 divided by 4 is 2. We create our key using balance 5 and remainder 2. This state is new. We add 0 to our answer and record the state.

Index 2 is the character 'e'. It is a vowel. We increase the balance to 6. The position is 3. The remainder of 3 divided by 4 is 3. We create our key. This state is new. We add 0 to our answer and record it.

Index 3 is the character 'y'. It is a consonant. We decrease the balance back to 5. The position is 4. The remainder of 4 divided by 4 is 0. We create our key using balance 5 and remainder 0. We look this key up in our hash map. We actually stored this exact key before we started the loop! This means we found a beautiful substring. We add 1 to our answer. We then update the hash map to say we have seen this state 2 times.

Index 4 is the character 'h'. It is a consonant. We decrease the balance to 4. The position is 5. The remainder of 5 divided by 4 is 1. We create our key using balance 4 and remainder 1. We look this key up. We stored this exact key back at index 0! This means we found another beautiful substring. We add 1 to our answer.

The loop finishes, and our final answer is 2. This perfectly matches the expected output for the string "baeyh".

## Evaluating Time and Space Complexity

It is always important to reflect on the efficiency of our code.

For the time complexity, we have two phases. First, we have a tiny loop to find our magic number x. Since k is at most 1000, this loop will run a maximum of 1000 times, which happens almost instantly. Next, we have a single pass through our given string. Inside this loop, we perform basic arithmetic, bitwise shifts, and a hash map lookup. All of these operations run in constant time. Therefore, the overall time complexity is directly proportional to the length of the string, making it an O(N) solution. This easily passes the constraints without timing out.

For the space complexity, we use an unordered hash map to store our states. In the absolute worst case scenario, every single position in the string might generate a completely unique state. Thus, the maximum number of entries in our hash map will be proportional to the length of the string plus one. This results in an O(N) space complexity, which is highly efficient and well within standard memory limits.

## Final Thoughts

I hope you found this detailed breakdown helpful and illuminating! By taking a moment to analyze the mathematical constraints of the divisibility rule, we transformed a potentially slow quadratic search into a lightning fast linear scan. String manipulation combined with state tracking via hash maps is an incredibly powerful technique to have in your coding toolkit.

Thank you so much for reading. If you enjoyed this explanation, please feel free to share it with your peers. Keep practicing, stay curious, and happy coding!
