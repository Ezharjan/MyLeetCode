# Unlocking the Secrets of Divisibility and Binary Search to Find the Kth Smallest Value [#3116]

Hello fellow coders and algorithm enthusiasts!

Today I want to share a fascinating journey through a problem that combines number theory with binary search. It is a wonderful challenge that tests our understanding of how numbers interact. Specifically we are looking at how to find the "kth" smallest amount we can create using a set of specific coin denominations when we are not allowed to mix different coins together.

This problem is a great example of how we can turn a search for a value into a counting problem. Let us dive in and explore the logic step by step!

## Understanding the Puzzle

Imagine you have a collection of coins with different values. For instance you might have a 3 unit coin and a 5 unit coin.

The rule is simple but strict. You can take as many coins as you want but they must all be of the same type. You cannot combine a 3 unit coin and a 5 unit coin to make 8.

Instead you can only make multiples of each coin value.
If you have a coin worth 3 you can make 3, 6, 9, 12, 15 and so on.
If you have a coin worth 5 you can make 5, 10, 15, 20, 25 and so on.

Our goal is to list all these possible amounts in increasing order and find the one that sits at the "kth" position.

**Example Scenario**

Let us say our coins are 3 and 5 and we want to find the 7th smallest amount.
* Multiples of 3 are 3, 6, 9, 12, 15, 18, 21...
* Multiples of 5 are 5, 10, 15, 20, 25...

If we combine these lists and sort them we get:
3, 5, 6, 9, 10, 12, 15...

Notice something interesting here. The number 15 appears in both lists but in our combined sequence of unique values it represents just one distinct amount.
Counting from the start:
1.  3
2.  5
3.  6
4.  9
5.  10
6.  12
7.  15

So the 7th smallest amount is 15.

The challenge becomes much harder when `k` is very large. If `k` is 2 billion we cannot simply list numbers one by one. We need a faster strategy.

## The Strategy: Guess and Check

Since we cannot iterate up to the answer we need to search for it. The sequence of valid amounts is strictly increasing. This property suggests we can use **Binary Search**.

Instead of asking "What is the kth number?" we ask a different question: "If I pick a number X how many valid amounts are less than or equal to X?"

Let us call this counting function `count(X)`.
* If `count(X)` is less than `k` then X is too small. We need a larger number.
* If `count(X)` is greater than or equal to `k` then X might be our answer or our answer might be smaller. We record X as a possible answer and try to look for a smaller number.

The range for our binary search will start at 1. The upper bound can be estimated as the smallest coin value multiplied by `k` because that is the worst case scenario where we only use the smallest coin.

## The Core Challenge: How to Count?

The heart of this solution is writing the `count(X)` function. We need to calculate how many numbers up to X are divisible by at least one of our coin denominations.

If we simply added up the multiples of each coin we would overcount.
Using our previous example with 3 and 5 and trying to count multiples up to 15:
* Multiples of 3 up to 15: 3, 6, 9, 12, 15 (5 numbers)
* Multiples of 5 up to 15: 5, 10, 15 (3 numbers)

If we just add 5 plus 3 we get 8. But the real answer is 7. We counted 15 twice because it is a multiple of both 3 and 5.

To fix this we use the **Inclusion Exclusion Principle**.

### The Inclusion Exclusion Principle

This principle helps us find the size of the union of several sets.
The formula logic is:
1.  **Add** the count of multiples for every single coin.
2.  **Subtract** the count of multiples for every pair of coins (because we counted them twice).
3.  **Add** the count of multiples for every triplet of coins (because we removed them too many times).
4.  **Subtract** the count of multiples for every group of four...

And so on.

**The Rule of Signs:**
* If we select an **odd** number of coins (1, 3, 5...) we **add** the count to our total.
* If we select an **even** number of coins (2, 4, 6...) we **subtract** the count from our total.

### Calculating Multiples with LCM

When we consider a group of coins, a number is a multiple of *all* of them if it is a multiple of their **Least Common Multiple (LCM)**.

For a chosen subset of coins with a calculated LCM value called `L`:
The number of multiples of `L` less than or equal to `X` is simply `X` divided by `L` (integer division).

So the algorithm for `count(X)` is:
1.  Iterate through every possible non empty subset of coins.
2.  Calculate the LCM of that subset.
3.  Calculate `X / LCM`.
4.  Add or subtract this value based on whether the subset size is odd or even.

## Implementation Details

Since the number of coin types is small (up to 15) we can iterate through all subsets using a bitmask. A bitmask is an integer where each bit represents a coin. If the jth bit is set to 1 it means we include the jth coin in our subset.

We loop from 1 to `2 to the power of N` minus 1. This covers every combination from a single coin to all coins.

### Handling Large Numbers

We must be careful with the LCM calculation. The LCM of several numbers can grow very large and exceed the limit of a 64 bit integer. However the problem constraints tell us that coin values are small (up to 25). Even so the LCM can get big.

Thankfully we only care about the LCM if it is less than or equal to our upper search bound. If the LCM exceeds the maximum possible answer we can ignore that subset because `X / LCM` will be zero.

Also note that `LCM(a, b) = (a * b) / GCD(a, b)`.
To avoid overflow during calculation we should perform the division before the multiplication:
`LCM(a, b) = (a / GCD(a, b)) * b`.

## The C++ Solution

Here is the complete C++ code that implements this logic. I have added comments to make every part clear.

```cpp
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
public:
    long long findKthSmallest(vector<int>& coins, int k) {
        int n = coins.size();
        
        // We will store pairs of (LCM value, Sign) for every subset.
        // Sign is +1 if the subset size is odd.
        // Sign is -1 if the subset size is even.
        vector<pair<long long, int>> subsets;
        
        // Iterate through all non empty subsets using a bitmask.
        // A mask ranges from 1 to (2^n - 1).
        for (int i = 1; i < (1 << n); ++i) {
            long long current_lcm = 1;
            int set_bits = 0;
            
            // Inspect each bit to see which coins are in this subset.
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) {
                    set_bits++;
                    
                    // Calculate LCM safely.
                    // standard library gcd function is available in <numeric>.
                    long long g = std::gcd(current_lcm, (long long)coins[j]);
                    current_lcm = (current_lcm / g) * coins[j];
                }
            }
            
            // Determine the sign based on the number of elements (PIE principle).
            // Odd count means Add (+1), Even count means Subtract (-1).
            int sign = (set_bits % 2 == 1) ? 1 : -1;
            subsets.push_back({current_lcm, sign});
        }
        
        // Binary Search Setup
        // Lower Bound: 1
        // Upper Bound: The smallest coin times k. 
        // We cast to long long to prevent overflow.
        long long min_coin = *min_element(coins.begin(), coins.end());
        long long low = 1;
        long long high = min_coin * (long long)k;
        long long ans = high;
        
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            long long count = 0;
            
            // Apply Inclusion Exclusion Principle
            // Iterate over all precomputed subset LCMs
            for (const auto& p : subsets) {
                count += p.second * (mid / p.first);
            }
            
            // If we found k or more numbers less than or equal to mid
            // then mid is a candidate answer. 
            // We try to find a smaller candidate.
            if (count >= k) {
                ans = mid;
                high = mid - 1;
            } else {
                // Not enough numbers, we need a larger value.
                low = mid + 1;
            }
        }
        
        return ans;
    }
};

```

## Complexity Analysis

Let us break down the efficiency of this approach to understand why it works well within the limits.

**Time Complexity**

1. **Precomputation:**
We iterate through every subset of coins. If there are `N` coins there are `2 to the power of N` subsets. For each subset we iterate through the coins to calculate the LCM.
Since `N` is at most 15 `2 to the power of 15` is 32768. This is very small for a computer.
The cost here is roughly `O(N * 2^N)`.
2. **Binary Search:**
The range of our search is from 1 to roughly `25 * 2 * 10^9` which is `5 * 10^10`.
Binary search splits the range in half at every step. The number of steps is the logarithm base 2 of the range size.
`log2(5 * 10^10)` is approximately 36 steps.
Inside each step of the binary search we loop through our precomputed subsets list.
So the cost of the search is `O(Steps * 2^N)`.
Combining these the total work is roughly `36 * 32768` operations which is around 1.2 million operations. This easily runs in under a second.

**Space Complexity**

We store the LCM and sign for every subset. This requires a list of size `2 to the power of N`.
For `N = 15` this uses minimal memory.
The space complexity is `O(2^N)`.

## Summary

This problem is a classic application of combinatorics applied to computer science. By combining **Binary Search** for the answer with the **Inclusion Exclusion Principle** for counting we can solve problems that initially look impossible to calculate directly.

Key Takeaways:

1. If you are asked to find the "kth" value and the answer space is monotonic (sorted) think of Binary Search on the answer.
2. When counting items that might overlap (like multiples of numbers) think of Inclusion Exclusion.
3. Precomputing values for subsets (like LCMs) can save massive amounts of time inside a loop.

I hope this explanation helps you understand the mechanics behind this solution. It is a very robust template that you can apply to many similar counting problems.

Happy Coding!
