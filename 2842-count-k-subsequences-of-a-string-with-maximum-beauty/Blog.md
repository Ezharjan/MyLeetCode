# Maximizing String Beauty by Counting Unique Character Subsequences [#2842]

Hello everyone in the coding community! Today I am very excited to share a walkthrough for a very interesting string manipulation and combinatorial problem. I recently spent some time working through this and thought it would be wonderful to share my thought process with all of you. My hope is that by breaking everything down step by step, we can all learn and understand the underlying concepts together. This is a very solid approach that gets the job done accurately. 

Let us dive deep into understanding how to find and count specific subsequences within a string while maximizing a property called beauty. I will explain every detail so that even beginners can follow along comfortably.

### Understanding the Core Problem

Imagine you are given a long string of lowercase English letters and a specific integer k. Your goal is to form a new string by picking exactly k characters from the original string. This new string is a subsequence, meaning you pick characters without changing their original relative order.

However, there is a catch! The characters you pick for this subsequence must all be completely unique. You cannot have two identical characters in your chosen subsequence. 

Now, we introduce the concept of beauty. The beauty of your chosen subsequence is calculated by looking at the original string. For every character you picked, you count how many times that character appeared in the original string. You then add all those counts together. That sum is the beauty of your subsequence.

Our ultimate objective is twofold. First, we need to figure out what the maximum possible beauty could be. Second, and more importantly, we need to count exactly how many distinct subsequences of length k we can form that achieve this exact maximum beauty. Since this count can become incredibly huge, we are required to return the final answer modulo 1000000007.

Two subsequences are considered different if they are formed by picking characters from different indices in the original string, even if the resulting formed strings look identical.

### Breaking Down the Strategy

To tackle this challenge, we need to combine a few different concepts. We will use a greedy approach to find the maximum beauty, basic combinatorics to count the ways to resolve ties, and modular arithmetic to keep our numbers within standard integer limits.

Let us explore the process in a very detailed manner.

#### Phase 1 Gathering Frequencies

Since the beauty of a subsequence depends entirely on how many times its characters appear in the original string, our very first logical step is to count the occurrences of every single character.

We can create a frequency map or an array of size 26 to tally up the counts of 'a' through 'z'. Once we have tallied everything, we can extract all the non zero frequencies into a separate list.

If our list of unique frequencies has fewer elements than k, it is physically impossible to form a subsequence of length k with all unique characters. In such a scenario, we can safely and immediately return 0.

#### Phase 2 Making Greedy Choices

To maximize the beauty, we should logically pick the characters that appear most frequently in the original string. This is the greedy part of our strategy. 

We sort our list of frequencies in descending order. The maximum beauty will always be achieved by selecting the characters corresponding to the top k frequencies in this sorted list.

For example, if we need to pick 3 characters and our sorted frequencies are 8, 6, 5, 5, 5, 2, we definitely want the character that appears 8 times and the character that appears 6 times. 

#### Phase 3 Managing Ties and Combinations

Things get incredibly interesting when we look at the boundary of our k selections. Continuing the previous example, we needed 3 characters and our frequencies were 8, 6, 5, 5, 5, 2. We firmly chose the top two 8 and 6. Now we need one more character to reach our quota of 3. 

However, the next highest frequency is 5, and there are three different characters that have a frequency of exactly 5! 

Which one do we pick? It actually does not matter for the beauty score, because picking any of them will add 5 to our total beauty. But it matters immensely for our total count of combinations. 

We must calculate how many ways we can choose the remaining required characters from the pool of characters that share this boundary frequency. This is a classic combinations problem often referred to as n choose r. In our example, we have a pool of 3 characters sharing the frequency of 5, and we need exactly 1 of them. We use the combination formula to calculate the number of ways to make this selection.

#### Phase 4 Factoring in Index Selections

Once we decide which unique characters will form our subsequence, we have another layer of choices. 

Suppose we decided to include the character 'a' in our subsequence, and 'a' appears 4 times in the original string. Because the problem states that subsequences are different if they use different original indices, we have 4 distinct choices for which specific 'a' to pick!

Therefore, for every character we include in our subsequence, we must multiply our total number of ways by the frequency of that character. If we pick a character that appears 8 times, we multiply our ways by 8. If we pick a character that appears 6 times, we multiply our ways by 6.

For the tied characters we selected in Phase 3, if we chose two characters that each have a frequency of 5, we multiply our ways by 5 to the power of 2.

#### Phase 5 Keeping Numbers Manageable

Because we are doing a lot of multiplying, our total count will grow astronomically fast. To prevent integer overflow and to satisfy the problem requirements, we must apply the modulo 1000000007 operation at every single step where we multiply or add to our total ways. 

We will need a custom exponentiation function to handle powers securely with the modulo, and our combinations function must also carefully apply the modulo.

### Comprehensive Code Walkthrough

Now let us translate this entire thought process into a clean, functioning C++ solution. I will avoid overly complex syntax to ensure the logic remains the star of the show.

```cpp
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    long long MOD = 1000000007;

    long long power(long long base, long long exp) {
        long long res = 1;
        base = base % MOD;
        while (exp > 0) {
            if (exp % 2 == 1) {
                res = (res * base) % MOD;
            }
            base = (base * base) % MOD;
            exp = exp / 2;
        }
        return res;
    }

    long long nCr(int n, int r) {
        if (r > n) return 0;
        if (r == 0 || r == n) return 1;
        if (r > n / 2) r = n - r; 
        
        long long res = 1;
        for (int i = 1; i <= r; i++) {
            res = res * (n - i + 1);
            res = res / i;
        }
        return res % MOD;
    }

public:
    int countKSubsequencesWithMaxBeauty(string s, int k) {
        int counts[26] = {0};
        
        for (char c : s) {
            counts[c - 'a']++;
        }
        
        vector<int> V;
        for (int i = 0; i < 26; i++) {
            if (counts[i] > 0) {
                V.push_back(counts[i]);
            }
        }
        
        if (V.size() < k) return 0;
        
        sort(V.rbegin(), V.rend());
        
        int boundaryFreq = V[k - 1]; 
        int countGreater = 0;
        int countEqual = 0;
        
        long long totalWays = 1;
        
        for (int v : V) {
            if (v > boundaryFreq) {
                countGreater++;
                totalWays = (totalWays * v) % MOD; 
            } else if (v == boundaryFreq) {
                countEqual++;
            }
        }
        
        int charactersNeeded = k - countGreater;
        
        long long combinations = nCr(countEqual, charactersNeeded);
        totalWays = (totalWays * combinations) % MOD;
        
        long long indexChoices = power(boundaryFreq, charactersNeeded);
        totalWays = (totalWays * indexChoices) % MOD;
        
        return totalWays;
    }
};
```

### Explaining the Helper Functions in Detail

Let us spend some time understanding the two helper functions included in the code above, as they are crucial for the mathematical correctness of our solution.

#### The Power Function
The power function calculates the base raised to an exponent. A naive loop multiplying the base by itself exponent times would be incredibly slow. Instead, we use a technique called binary exponentiation. 

By checking if the exponent is odd or even, we can square the base and divide the exponent by two in each step. This drastically reduces the number of operations required. Notice how we apply the modulo operator after every single multiplication. This guarantees that our intermediate variables never exceed the storage limits of a standard integer data type.

#### The Combinations Function
The nCr function calculates the number of ways to choose r items from a pool of n items. We include a few early exit conditions to save processing time. For example, if r is exactly zero or exactly n, there is only 1 way to make the choice.

We also optimize by checking if r is greater than half of n. Choosing 8 items from 10 is mathematically identical to choosing 2 items from 10. Modifying r in this way reduces the number of iterations in our loop. Inside the loop, we multiply and divide sequentially to build up the factorial relationship without actually computing massive factorials upfront. Finally, we apply the modulo operator before returning the result.

### Examining the Main Logic Step by Step

Now let us walk through the main function. 

First, we initialize an array of integers with 26 slots, all starting at zero. We iterate through every character in our input string. By subtracting the character 'a' from our current character, we get a numeric index from 0 to 25. We increment the value at that index to build our frequency map.

Next, we transfer all the non zero frequencies into a dynamic list. We check our boundary condition. If the size of this list is smaller than our target length k, we immediately return zero.

We then sort our list of frequencies in descending order. The `rbegin` and `rend` iterators make this completely straightforward.

The element sitting at index k minus 1 is our crucial boundary frequency. We loop through our sorted list and count how many frequencies are strictly greater than our boundary, and how many are exactly equal to it. For every frequency strictly greater than the boundary, we are absolutely forced to include that character to maximize beauty. So, we multiply our running total by that frequency and apply the modulo.

After that loop, we determine how many more characters we still need to reach k. We call our combinations helper function to find out how many ways we can select these remaining needed characters from the pool of tied frequencies. We multiply our running total by this combination value and apply the modulo.

Finally, for those tied characters we just chose, we have to account for the index choices. We call our power helper function, raising the boundary frequency to the power of the number of characters we needed. We multiply our total by this result, apply a final modulo, and we have our answer!

### Complexity Analysis

It is always good practice to understand the resource footprint of our code.

For Time Complexity, iterating through the initial string to count the characters takes linear time based on the length of the string. Sorting the unique frequencies takes very little time because there are at most 26 unique characters in the English alphabet. Calculating the combinations and the power functions also takes negligible time due to the strict upper limit of 26. Therefore, the overall time complexity is firmly tied to the length of the input string, making it an O(N) solution where N is the length of the string.

For Space Complexity, we only allocate a static array of size 26 for counting, and a dynamic list that will never exceed 26 elements. Because these memory allocations do not scale with the size of the input string, our space complexity is strictly O(1) constant space.

### Final Thoughts

This problem beautifully highlights how string processing can smoothly transition into mathematical combinatorics. By carefully organizing our frequencies and understanding exactly when our choices diverge, we can compute massive permutations securely and efficiently.

I hope this detailed explanation illuminates the logic behind the code and helps you understand the concepts on a fundamental level. Thank you so much for reading, and I wish you all the best of luck on your continued coding journeys! Please feel free to share your thoughts or alternative approaches below.
