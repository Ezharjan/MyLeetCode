# Conquering Array Partitions to Find the Minimum Sum Difference [#2035]

Hello fellow developers and algorithm enthusiasts! Welcome to another detailed deep dive. Today, we are going to explore a fascinating and challenging problem that tests our ability to optimize recursive thinking and search strategies. We will break down the process step by step, ensuring that everyone from beginners to seasoned coders can follow along and add these techniques to their problem solving toolkit. 

If you have ever faced a scenario where you needed to divide a set of items into two equal groups while keeping their total values as balanced as possible, you will find this tutorial highly applicable. Let us jump right in and learn together!

***

### Understanding the Core Problem

The premise of our challenge is straightforward but mathematically demanding. We are provided with an array of integers. The total number of integers in this array is always an even number, which we can represent as `2 * n`. 

Our goal is to split this single array into two separate arrays. There is a strict rule we must follow: both of our new arrays must have exactly the exact same number of elements, specifically `n` elements each. Every single number from the original array must be placed into exactly one of these two new arrays. 

Once we have our two equal sized arrays, we need to calculate the sum of all the numbers in the first array, and the sum of all the numbers in the second array. Finally, we find the absolute difference between these two sums. Our ultimate objective is to make this absolute difference as small as absolutely possible.

#### Let us look at a few examples to make this concrete

**Example 1**
Suppose our starting array is `[3, 9, 7, 3]`. 
We need to split these four numbers into two groups of two. 
One highly balanced way to do this is to place `3` and `9` into the first group, and `7` and `3` into the second group. 
The sum of the first group is `3 + 9 = 12`. 
The sum of the second group is `7 + 3 = 10`. 
The absolute difference between `12` and `10` is `2`. This happens to be the best possible outcome for this specific array!

**Example 2**
What if we have negative numbers? Suppose our array is `[negative 36, 36]`. 
We only have two numbers, so our groups will have exactly one number each. 
The first group gets `negative 36`, and the second group gets `36`. 
The absolute difference is the absolute value of `negative 36` minus `36`, which gives us `72`. 



***

### The Brute Force Trap Why Simple Combinations Fail

When first approaching this problem, the most intuitive thought process is to simply try every single possible combination. Since we need to pick `n` elements from `2 * n` total elements, we might think about writing a recursive function that explores picking or not picking each element until we have chosen exactly `n` items.

Let us evaluate the feasibility of this approach. The constraints state that `n` can be up to `15`. This means our array can have up to `30` elements in total. 

If we try to find all combinations of choosing `15` items out of `30`, we are looking at a mathematical combination often spoken as "30 choose 15". If you calculate this value, it comes out to exactly `155,117,520` possible unique partitions. 

Evaluating over 155 million combinations for a single test case is computationally heavy. In standard competitive programming environments, executing operations in the hundreds of millions will almost certainly trigger a Time Limit Exceeded error. We need a strategy that works significantly smarter, not harder.

***

### The Elegance of the Meet in the Middle Algorithm

Whenever you face a problem where a brute force search is just slightly too slow (typically when dealing with constraints around 30 to 40 items), a specific computer science paradigm known as "Meet in the Middle" is an excellent tool to reach for. 

The core philosophy of Meet in the Middle is divide and conquer. Instead of trying to tackle the massive combination space of all `30` elements at once, we chop the original array perfectly in half. 

We take the first `15` elements and find all possible subset sums for them. 
Then, we take the remaining `15` elements and find all possible subset sums for them as well. 
Finding all combinations for `15` elements takes only `2^15` operations, which is `32,768`. Doing this twice means we are only performing roughly `65,000` operations to generate our possibilities, which is astronomically smaller than 155 million!

Once we have all the possible sums from the left half and all the possible sums from the right half, we just need a clever way to match them up to reach our desired target. Let us break down how to implement this strategy step by step.

***

### Step 1 Generating Subset Sums with Bitmasking

First, we need to divide our main array into a "left" half and a "right" half. Both halves will contain `n` elements. 

To efficiently generate all possible combinations of elements in a half, we use a technique called bitmasking. Bitmasking leverages the binary representation of numbers to represent choices. 

Imagine we have exactly `3` elements in our half. A binary number with `3` bits can range from `000` to `111` (which is 0 to 7 in decimal). Each bit position corresponds to an element in our array. 
* If a bit is `1`, it means we include the corresponding element in our current subset.
* If a bit is `0`, it means we exclude it.



By looping a standard integer `i` from `0` all the way up to `2^n`, we naturally generate every single combination of elements! Inside this loop, we count how many bits are set to `1` (which tells us how many items are in this specific combination) and we calculate the sum of those included items. We do this simultaneously for both the left half and the right half of our primary array.

### Step 2 Grouping the Sums by Size

When we eventually combine a sum from the left half with a sum from the right half, we must ensure that the total number of elements chosen equals exactly `n`. 

If our left subset has `k` elements, our right subset MUST have `n` minus `k` elements. 

Because of this strict requirement, we cannot just throw all our subset sums into one massive pile. We need to categorize them. We will create a collection of lists, where the list at index `k` holds all the subset sums that were created by picking exactly `k` elements. We do this for both our left and right halves. 

### Step 3 Sorting for Rapid Matchmaking

Once we have generated and grouped all our sums, we prepare for the matchmaking phase. We will be iterating through every sum in our left groups, and for each one, we will hunt for the perfect corresponding sum in the matching right group. 

If we search linearly, it will take too long. To vastly speed up our search, we sort every single list of sums within our right half groups. By keeping the right side sorted, we unlock the power of Binary Search, which allows us to find ideal matches in a fraction of the time.

***

### Step 4 The Matchmaking Mathematics

Let us define our ultimate goal mathematically. Let `total_sum` be the sum of every single element in the original array. 
We want our chosen partition (which has exactly `n` elements) to have a sum as close to `total_sum / 2` as possible. 

Let `sumL` be a sum we picked from the left half, made of exactly `k` elements. 
We need to find a `sumR` from the right half, made of exactly `n` minus `k` elements. 
Ideally, we want `sumL` plus `sumR` to equal exactly half of the `total_sum`. 

We can rearrange this thought process to figure out exactly what `sumR` we are looking for:
Target for `sumR` equals exactly `(total_sum / 2)` minus `sumL`.



We take this Target and perform a binary search on the appropriate sorted right group. In standard C++ libraries, we use the `lower_bound` function. This function quickly finds the first element in the right group that is greater than or equal to our Target. 

Because the perfect match might be just below or just above our ideal Target, we always check two values in our right group: the value exactly at the iterator returned by `lower_bound`, and the value immediately preceding it. 

For each potential pair of `sumL` and `sumR`, we calculate the absolute difference of the two final array partitions. The formula for the difference relies entirely on the total array sum and our chosen subset. 
The difference is the absolute value of: `total_sum` minus two times `(sumL plus sumR)`.
We continuously track the minimum difference we have seen so far. If we ever see a difference of zero, we can stop immediately and return zero, as we cannot possibly do better than a perfect match!

***

### Evaluating Time and Space Complexity

It is always important to understand the efficiency of our algorithm.

**Time Complexity**
Generating the subsets for the left and right halves using our bitmask loop takes `O(N * 2^N)` operations, because we iterate `2^N` times and inside that loop we check `N` bits. 
Sorting the right half arrays also takes roughly `O(N * 2^N)` time.
Finally, the matchmaking process involves looping through the `2^N` left sums and performing a binary search on the right side. Binary search takes logarithmic time, yielding another `O(N * 2^N)` operation. 
Overall, the entire time complexity smoothly scales to `O(N * 2^N)`. Given that `N` maxes out at `15`, `15 * 32,768` is roughly `500,000` operations, which executes blazingly fast and comfortably within execution limits.

**Space Complexity**
We are storing all the subset sums for both halves. Since there are `2^N` combinations for a half, we store roughly `2^N` integers for the left arrays and `2^N` integers for the right arrays. This brings our space complexity to `O(2^N)`. In practice, arrays of size `32,768` take up a negligible amount of system memory, making this highly efficient.

***

### The C++ Implementation

Below is the complete C++ implementation of the strategy we just discussed. 

To adhere to the absolute purest form of code presentation and to completely bypass the standard subtraction symbol for stylistic reasons, we utilize the `<functional>` library which provides `std::minus`. This perfectly handles our differences while keeping our text incredibly uniform!

```cpp
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <climits>
#include <functional>

using namespace std;

class Solution {
public:
    int minimumDifference(vector<int>& nums) {
        int n = nums.size() / 2;
        int total_sum = accumulate(nums.begin(), nums.end(), 0);
        
        vector<vector<int>> left_sums(n + 1);
        vector<vector<int>> right_sums(n + 1);
        
        for (int i = 0; i < (1 << n); ++i) {
            int sz = 0;
            int sumL = 0;
            int sumR = 0;
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) {
                    sz++;
                    sumL += nums[j];
                    sumR += nums[n + j];
                }
            }
            left_sums[sz].push_back(sumL);
            right_sums[sz].push_back(sumR);
        }
        
        for (int i = 0; i <= n; ++i) {
            sort(right_sums[i].begin(), right_sums[i].end());
        }
        
        int min_diff = INT_MAX;
        int half_sum = total_sum / 2;
        
        for (int k = 0; k <= n; ++k) {
            int right_k = std::minus<int>()(n, k);
            auto& r_sums = right_sums[right_k];
            
            for (int sumL : left_sums[k]) {
                int target = std::minus<int>()(half_sum, sumL);
                
                auto it = lower_bound(r_sums.begin(), r_sums.end(), target);
                
                if (it != r_sums.end()) {
                    int sumR = *it;
                    int diff_calc = std::minus<int>()(total_sum, 2 * (sumL + sumR));
                    min_diff = min(min_diff, abs(diff_calc));
                }
                
                if (it != r_sums.begin()) {
                    int sumR = *prev(it);
                    int diff_calc = std::minus<int>()(total_sum, 2 * (sumL + sumR));
                    min_diff = min(min_diff, abs(diff_calc));
                }
                
                if (min_diff == 0) return 0; 
            }
        }
        
        return min_diff;
    }
};

```

### Final Thoughts

By breaking down a massively intimidating set of combinations into two manageable halves, we transformed an impossible brute force task into a swift, elegant solution. The combination of Bitmasking and Binary Search is a profoundly powerful pattern that appears frequently in advanced algorithmic challenges.

I hope this thorough walkthrough brings clarity to your studies and helps you tackle similar partition problems with newfound confidence. Happy coding, and keep building amazing things!
