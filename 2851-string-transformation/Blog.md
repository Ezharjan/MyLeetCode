# Transforming Strings with Math and Pattern Matching A Friendly Guide [#2851]

Hello everyone! I hope you are having a wonderful day. Today, I want to share my thoughts and a detailed walkthrough for a fascinating string manipulation and combinatorics challenge. We are going to explore a beautiful intersection of pattern matching and mathematics. I am sharing this in hopes that it helps someone out there who might be stuck, and I would love to hear your thoughts and approaches too!

This guide is designed to be as friendly and detailed as possible. We will break down the logic step by step, ensuring every part makes sense. Grab a cup of coffee or tea, and let us dive right in!

## Understanding the Core Problem

Let us start by looking at what we are asked to do. We are given two strings, let us call them string s and string t. Both of these strings have the exact same length, which we will refer to as n. 

We are allowed to perform a specific operation on string s. In a single operation, we can take a suffix of string s (which is just a piece of the string taken from the very end) and move it to the very beginning of the string. The length of this suffix must be greater than zero and strictly less than the total length of the string n. 

Our goal is to find out exactly how many unique ways we can apply this operation exactly k times to transform string s into string t. Because the total number of ways can grow to be exceptionally large, we are asked to return the final answer modulo 1000000007.

To visualize this, imagine our string s is "abcd". 
If we pick a suffix of length two, which is "cd", and move it to the front, our new string becomes "cdab". 

This operation is essentially a cyclic rotation. Every time we perform an operation, we are picking one of the n minus 1 possible cyclic rotations of the current string. 

## Breaking the Challenge into Two Pieces

When faced with a complex puzzle, a great strategy is to break it down into smaller, more manageable pieces. For this specific challenge, we can separate our logic into two distinct phases.

The first phase is pure string matching. Before we even worry about the number of operations k, we need to know which cyclic rotations of string s actually perfectly match string t. 

The second phase involves counting the combinations. Once we know the valid target destinations, we need to calculate the number of valid paths to reach those destinations in exactly k steps. 

Let us explore both of these phases in great detail.

## Phase One Finding Valid Rotations

If we rotate string s repeatedly, it creates a cycle. Since string s and string t are the same length, any valid transformation of string s into string t means that string s must be a substring of a repeated string t. 

To easily find all the valid starting positions where string s matches a rotation of string t, we can create a brand new combined string. We take string s, and we attach string t to it twice. So our combined string looks like this string s followed by string t followed by string t.

By doing this, every possible cyclic rotation of string t is naturally embedded within the "string t followed by string t" portion. Now, our task is simply to find all occurrences of string s within this combined text. 

To do this efficiently, we can use a wonderful technique known as the Z Algorithm. 

### The Magic of the Z Algorithm

The Z Algorithm is a clever way to process a string and build an array of numbers, often called the Z array. For any given index i in our string, the value in the Z array at that index tells us the length of the longest substring starting at index i that perfectly matches the very beginning (the prefix) of the entire string.


Here is how we apply it to our combined string. 
First, we run the Z Algorithm on our combined string (string s plus string t plus string t). 
Then, we look at the results in the Z array, but we only care about the indices that fall within the "string t plus string t" section. 
If we find a value in the Z array that is greater than or equal to the length of string s, it means we have found a perfect match! We record this index because it represents a valid cyclic shift that turns our starting string into our target string.

## Phase Two The Mathematics of Shifting

Now that we have a list of valid target positions, we need to figure out how many ways we can reach them in exactly k operations. 

Let us define two simple concepts to keep track of our states. 
State Zero means our current string is exactly the same as our starting string s. 
State One means our current string is any specific cyclic rotation that is NOT equal to our starting string s. 

In a single operation, we can transition between these states. 
If we are currently at State Zero, any operation we make will force us to choose one of the n minus 1 other rotations. So, we will definitely land on State One.
If we are currently at State One, we have two choices. We could make an operation that takes us perfectly back to State Zero. There is exactly one way to do this. Alternatively, we could move to any of the other n minus 2 different rotations, keeping us in State One.

Let us represent the number of ways to be in State Zero after exactly k steps as dp[k][0]. 
Let us represent the number of ways to be in a specific State One after exactly k steps as dp[k][1].

By looking at the total number of possible paths, we know that at every single step, we have exactly n minus 1 choices. Therefore, the total number of ways to make k moves is simply n minus 1 raised to the power of k.

Through some algebraic substitution, we can derive a direct formula for our states without needing to simulate every single step. 
The formula for reaching a specific different rotation (State One) after k steps is the total number of ways, minus negative one raised to the power of k, all divided by the total length n.

In pure text formatting, it looks like this 
dp[k][1] equals ( (n minus 1) to the power of k, minus (negative 1) to the power of k ) divided by n.

And the formula for returning to the exact starting string (State Zero) is simply 
dp[k][0] equals dp[k][1] plus (negative 1) to the power of k.

### Handling Large Numbers with Modular Arithmetic

Because k can be a massive number, we must perform all our math modulo 1000000007. 

Addition, subtraction, and multiplication are straightforward under a modulo. However, division is a little tricky. In our formula for State One, we need to divide by n. In the world of modular arithmetic, we cannot just divide. Instead, we must multiply by the modular multiplicative inverse of n.

Thanks to Fermat's Little Theorem, we know that to divide by n under a prime modulo, we can simply multiply by n raised to the power of the modulo minus 2. This is a brilliant little mathematical trick that keeps our numbers integers while preserving the correct modulo logic!

## Putting It All Together

We now have all the ingredients we need. 
First, we calculate our Z array and find all the valid indices where string s can become string t. 
Next, we calculate the number of ways to reach State Zero and State One using our formulas and fast modular exponentiation. 
Finally, we loop through all our valid indices. If the valid index represents zero shifts (meaning string s and string t are already identical), we add the ways for State Zero to our total. If the valid index requires a shift, we add the ways for State One to our total. 

Here is the C++ code that beautifully brings these concepts together. 

```cpp
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    int numberOfWays(string s, string t, long long k) {
        const int n = s.length();
        const int negOnePowK = (k % 2 == 0 ? 1 : -1); 
        
        string combinedString = s + t + t;
        const vector<int> z = zFunction(combinedString);
        const vector<int> indices = getIndices(z, n);
        
        vector<long long> dp(2); 
        
        long long waysToDiffState = (modPow(n - 1, k) - negOnePowK + kMod) % kMod;
        waysToDiffState = (waysToDiffState * modPow(n, kMod - 2)) % kMod;
        dp[1] = waysToDiffState;
        
        long long waysToSameState = (dp[1] + negOnePowK + kMod) % kMod;
        dp[0] = waysToSameState;
        
        long long totalWays = 0;
        for (int index : indices) {
            if (index == 0) {
                totalWays = (totalWays + dp[0]) % kMod;
            } else {
                totalWays = (totalWays + dp[1]) % kMod;
            }
        }
        
        return totalWays;
    }

private:
    static constexpr int kMod = 1000000007;

    long long modPow(long long base, long long exponent) {
        long long result = 1;
        base %= kMod;
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result = (result * base) % kMod;
            }
            base = (base * base) % kMod;
            exponent /= 2;
        }
        return result;
    }

    vector<int> zFunction(const string& str) {
        const int n = str.length();
        vector<int> z(n, 0);
        int left = 0, right = 0;
        for (int i = 1; i < n; ++i) {
            if (i < right) {
                z[i] = min(right - i, z[i - left]);
            }
            while (i + z[i] < n && str[z[i]] == str[i + z[i]]) {
                ++z[i];
            }
            if (i + z[i] > right) {
                left = i;
                right = i + z[i];
            }
        }
        return z;
    }

    vector<int> getIndices(const vector<int>& z, int n) {
        vector<int> indices;
        for (int i = n; i < 2 * n; ++i) {
            if (z[i] >= n) {
                indices.push_back(i - n);
            }
        }
        return indices;
    }
};

```

## Understanding the Efficiency

Whenever we write an algorithm, it is great practice to think about how much time and memory it consumes.

For the time taken, creating our combined string and running the Z Algorithm takes an amount of time strictly proportional to the length of our string. We denote this as Big O of N. Next, our mathematical calculations using fast modular exponentiation take logarithmic time relative to k, which we write as Big O of log k. Combined together, the time complexity is extremely fast and will easily pass the constraints of the problem!

For the memory used, we allocate space for our combined string, which is three times the length of our original string. We also create a Z array of the same size. Therefore, our space complexity scales directly with the length of the string, or Big O of N.

## Final Thoughts

I truly hope this detailed breakdown helps you understand both the string matching and the combinatorics aspects of this challenge. Breaking large problems into distinct, solvable parts is a skill that takes time to develop, but it is incredibly rewarding.

Thank you so much for reading along. Keep practicing, keep sharing your knowledge, and happy coding to you all!
