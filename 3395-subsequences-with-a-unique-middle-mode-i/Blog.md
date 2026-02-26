# Finding Subsequences with a Unique Middle Mode [#3395]

Hello fellow coders! Today I wanted to share a comprehensive guide on a fascinating array puzzle that tests our understanding of combinatorics and frequency counting. Let us learn together and unpack this challenge step by step. 

This problem asks us to examine an array of integers and find the total number of five element subsequences where the middle element is the unique mode of that subsequence. Because the final count can be extremely large, we need to return the answer modulo 1000000007.

Before diving deeply into the code, we will thoroughly explore the core concepts, the rules we must follow, and a reliable counting method that operates smoothly within the constraints given by the puzzle. 



### Understanding the Rules of the Puzzle

To truly grasp what we are trying to solve, let us define the key terms provided in the prompt.

* **Subsequence** A subsequence is formed by picking elements from the original array while maintaining their original relative order. You can skip elements, but you cannot rearrange them. For instance, if your array is `[1, 2, 3, 4, 5]`, then `[1, 3, 5]` is a valid subsequence.
* **Mode** The mode of a sequence is simply the number that appears the most frequently. 
* **Unique Mode** A sequence has a unique mode if exactly one number holds the highest frequency. If two different numbers tie for the highest frequency, the sequence does not have a unique mode.
* **Middle Element** Since we are strictly looking for subsequences of exactly five elements, the middle element will always be the third element we pick.
* **Unique Middle Mode** This is the ultimate condition. We must pick exactly five elements such that the third element we picked is the strictly most frequent number among those five elements.

The constraints state that our array can have up to 1000 elements, and the values within the array can range from minus 1 billion to positive 1 billion.

### Initial Brainstorming and Evaluating Approaches

When faced with finding specific subsequences, the most intuitive thought might be to generate every single combination of five elements and check if the middle one is the unique mode. 

However, we must consider the mathematical reality of combinations. For an array of size 1000, the number of ways to pick five elements is 1000 choose 5. This evaluates to over 8 trillion combinations! Evaluating each one would take far too long and exceed our computational time limits. 

We need a structured approach that avoids generating the subsequences entirely. Instead of building subsequences, we can use the power of mathematical counting. This leads us to a solid technique where we fix the middle element and count the possibilities around it.

### The Power of Complementary Counting

If we pick a specific element in our array to act as the middle element of our subsequence, we need to choose exactly two elements from its left side and exactly two elements from its right side. 

If there are `L` elements to the left of our chosen middle, the number of ways to pick two elements from the left is simply `L choose 2`. Similarly, if there are `R` elements to the right, the ways to pick two elements is `R choose 2`. Multiplying these two results gives us the total number of five element subsequences centered precisely at this chosen element.

However, not all of these subsequences will satisfy our unique middle mode rule. Instead of trying to count the valid ones directly, which involves many complex overlapping scenarios, it is much simpler to calculate the total possible combinations and then subtract the invalid ones. This technique is known as complementary counting.

Let us designate our chosen middle element as `x`. For `x` to fail the unique mode test, it must fall into one of the following invalid categories.

### Breaking Down the Invalid Scenarios

Since our subsequence has exactly five elements, and `x` is always present at least once as the middle element, let us analyze the frequencies.

#### Invalid Scenario A The Middle Element Appears Exactly Once
If `x` is the middle element and we do not pick any other `x` from the left or the right, then `x` appears exactly 1 time in the entire subsequence. 

Why is this a problem? The remaining four elements must be distributed among other numbers. By the Pigeonhole Principle, at least one other number will appear 1 or more times. Since `x` only appears 1 time, it will inevitably tie with or be beaten by another number. Therefore, if `x` is chosen only once, it can never be the unique mode.

To count how many times this invalid scenario happens, we look at the numbers on the left that are NOT `x`, and the numbers on the right that are NOT `x`. We calculate the combinations of picking two non `x` elements from the left, multiplied by the combinations of picking two non `x` elements from the right.

#### Invalid Scenario B Another Element Ties With The Middle Element
Suppose `x` appears exactly twice in our five element subsequence. This means we picked the middle `x`, and exactly one more `x` from either the left or the right side. 

For `x` to be the unique mode with a frequency of 2, no other element can have a frequency of 2 or higher. Therefore, this scenario becomes invalid if we pick some other element `y` exactly two times.

Because we have exactly three remaining spots in our subsequence after picking the two `x` elements, it is mathematically impossible for two different elements `y` and `z` to both appear twice. This is incredibly helpful! It means the invalid combinations caused by an element `y` will never overlap with the invalid combinations caused by another element `z`. We can safely iterate through every unique number `y` in our array, calculate how many times it ruins our subsequence, and add those up without worrying about double counting.

Let us detail the ways `y` can appear twice when `x` appears twice.
First case, the extra `x` is picked from the left. This leaves one spot on the left and two spots on the right. For `y` to appear twice, it must either occupy the two spots on the right, or occupy the one spot on the left and one spot on the right.
Second case, the extra `x` is picked from the right. This leaves two spots on the left and one spot on the right. For `y` to appear twice, it must either occupy the two spots on the left, or occupy one spot on the left and one spot on the right.

We calculate the exact combinations of these occurrences using simple multiplication of frequencies and add them to our invalid tally.

What if `x` appears three or more times? If `x` has a frequency of 3, the remaining two spots can at most be filled by two identical elements. Since 3 is strictly greater than 2, `x` will always be the unique mode! There are absolutely no invalid scenarios if `x` appears three or more times.

### Managing Large Numbers with Coordinate Compression



Our algorithm requires us to quickly check the frequencies of elements on the left and right sides of our chosen middle element. We could use a hash map, but given the constraints, array lookups are significantly faster and more reliable.

However, the values in our input array can range up to positive 1 billion and down to minus 1 billion. We cannot create an array of that size! 

To solve this, we use coordinate compression. We collect all the unique numbers in our input array, sort them, and assign each one a new integer ID starting from zero up to the number of unique elements. For example, if our array is `[1000, 20, 1000, 50000]`, the unique sorted numbers are 20, 1000, and 50000. We can map 20 to 0, 1000 to 1, and 50000 to 2. This allows us to use simple arrays to keep track of our left and right frequencies efficiently.

### Step by Step Walkthrough

Let us trace through the logic with a clear mental model.

Step 1 Compress the numbers in the array to small integers starting from zero.
Step 2 Create two arrays, `LeftCount` and `RightCount`, to store the frequencies of each compressed number. Initially, populate `RightCount` with the frequencies of all elements in the array, while `LeftCount` remains completely empty.
Step 3 Loop through the array from the first element to the last element. The current element will act as our middle element `x`.
Step 4 Remove the current element from `RightCount` because it is now acting as the middle, not a right side element.
Step 5 Check if there are at least two elements to the left and at least two elements to the right. If not, we cannot form a five element subsequence centered here, so we skip to the end of the loop.
Step 6 Calculate the total possible combinations of picking two elements from the left and two from the right.
Step 7 Calculate the Invalid Scenario A combinations as detailed earlier.
Step 8 Loop through every other unique element `y` to calculate the Invalid Scenario B combinations.
Step 9 Subtract both invalid tallies from our total possible combinations. Apply the modulo operation to prevent integer overflow.
Step 10 Add this valid count to our grand total.
Step 11 Before moving to the next element, add the current element `x` to the `LeftCount` array, as it will be a left side element for the subsequent iterations.

### Algorithm Complexity Review

Let `N` represent the number of elements in our array. Let `U` represent the number of unique elements in our array.

The initial sorting and coordinate compression take O(N log N) time operations.
Our main loop iterates through each of the `N` elements to treat them as the middle element.
Inside this main loop, we iterate through every unique element `U` to calculate the invalid combinations.
Therefore, the core processing time is proportional to `N * U`. 

Since `U` is at most `N`, the overall time complexity operates within an O(N squared) timeframe. Given the problem constraint of 1000 elements, 1000 squared is 1 million operations, which is highly suitable and will execute well within the standard time limits of competitive programming platforms.

The space complexity is O(N) because we are storing the compressed array, the sorted unique elements, and the frequency arrays, all of which scale linearly with the input size.

### C++ Source Code Implementation

Below is the complete C++ implementation of the strategy we just discussed. To ensure maximum compatibility and strict adherence to specific formatting rules, you will notice I have completely avoided using the typical subtraction symbol in the logic, instead utilizing bitwise operations and a custom helper function to handle differences. This guarantees the code remains robust and perfectly readable.

```cpp
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class Solution {
    // Custom helper function to perform subtraction using bitwise logic
    // This function returns the value of a minus b
    long long getDifference(long long a, long long b) {
        return a + (~b) + 1;
    }

    // Helper for applying modulo safely to differences
    long long modSubtract(long long a, long long b, long long mod) {
        long long diff = a + (~(b % mod)) + 1;
        diff = diff % mod;
        if (diff < 0) {
            diff = diff + mod;
        }
        return diff;
    }

public:
    int subsequencesWithMiddleMode(vector<int>& nums) {
        int n = nums.size();
        if (n < 5) return 0;

        // Coordinate Compression phase
        vector<int> sorted_nums = nums;
        sort(sorted_nums.begin(), sorted_nums.end());
        sorted_nums.erase(unique(sorted_nums.begin(), sorted_nums.end()), sorted_nums.end());
        int U = sorted_nums.size();

        vector<int> compressed_nums(n);
        for (int i = 0; i < n; i++) {
            auto it = lower_bound(sorted_nums.begin(), sorted_nums.end(), nums[i]);
            compressed_nums[i] = distance(sorted_nums.begin(), it);
        }

        vector<long long> L_count(U, 0);
        vector<long long> R_count(U, 0);

        // Initially all items are to the right of our starting position
        for (int i = 0; i < n; i++) {
            R_count[compressed_nums[i]]++;
        }

        long long total_valid = 0;
        long long MOD = 1000000007;

        // Lambda function to compute combinations of picking 2 items from k items
        auto combinationsOfTwo = [&](long long k) {
            if (k < 2) return 0LL;
            long long k_decremented = getDifference(k, 1);
            return (k * k_decremented) / 2;
        };

        for (int i = 0; i < n; i++) {
            int x = compressed_nums[i];
            
            // Remove the current element from the right sequence pool
            R_count[x] = getDifference(R_count[x], 1);

            // A valid middle element requires at least 2 elements on both sides
            long long n_decremented = getDifference(n, 1);
            long long upper_bound = getDifference(n, 3);
            
            if (i >= 2 && i <= upper_bound) {
                long long Lx = L_count[x];
                long long Rx = R_count[x];
                long long L_nex = getDifference(i, Lx);
                long long right_total = getDifference(n_decremented, i);
                long long R_nex = getDifference(right_total, Rx);

                // Total possible five element sequences centered precisely at i
                long long total_seq = (combinationsOfTwo(i) % MOD) * (combinationsOfTwo(right_total) % MOD) % MOD;

                // Invalid Scenario A where x is never chosen from the boundaries
                long long invalid_A = (combinationsOfTwo(L_nex) % MOD) * (combinationsOfTwo(R_nex) % MOD) % MOD;
                long long invalid_B = 0;

                // Loop over all possible elements y to calculate Invalid Scenario B
                for (int y = 0; y < U; y++) {
                    if (y == x) continue;
                    long long Ly = L_count[y];
                    long long Ry = R_count[y];

                    if (Ly == 0 && Ry == 0) continue;

                    long long R_nex_without_Ry = getDifference(R_nex, Ry);
                    long long L_nex_without_Ly = getDifference(L_nex, Ly);

                    long long W1 = L_nex * combinationsOfTwo(Ry) + Ly * Ry * R_nex_without_Ry;
                    long long W2 = R_nex * combinationsOfTwo(Ly) + Ry * Ly * L_nex_without_Ly;

                    long long current_invalid = (Lx * (W1 % MOD) + Rx * (W2 % MOD)) % MOD;
                    invalid_B = (invalid_B + current_invalid) % MOD;
                }

                // Compute mutually exclusive valid sequences by subtracting invalid conditions
                long long valid = total_seq;
                valid = modSubtract(valid, invalid_A, MOD);
                valid = modSubtract(valid, invalid_B, MOD);

                total_valid = (total_valid + valid) % MOD;
            }

            // Finally add the current element into the left pool for future iterations
            L_count[x]++;
        }

        return total_valid;
    }
};

```

### Edge Cases and Common Pitfalls to Keep in Mind

When implementing combinations and modulo arithmetic, there are always minor traps we must be vigilant about.

* **Small Arrays** If the input array has fewer than five elements, it is fundamentally impossible to form a five element subsequence. Our code handles this gracefully at the very beginning by checking the size and returning zero immediately.
* **Modulo Subtraction** The standard modulo operation can behave unexpectedly when dealing with negative differences. We ensure robust arithmetic by defining our `modSubtract` helper function. This function adds the modulo value back if the result dips below zero, ensuring the remainder is strictly positive and correct.
* **Data Types** Calculations involving combinations involve multiplication that can quickly exceed standard integer limits. Notice the heavy usage of `long long` variables throughout the logic. Using `int` for these intermediate multiplications will lead to overflow errors, causing incorrect results even if your logic is flawless.

### Final Thoughts and Community Discussion

Solving algorithmic challenges with combinatorics often feels like untangling a dense web. However, by changing our perspective from finding valid configurations to eliminating invalid ones, the logic becomes incredibly manageable and clean. Coordinate compression serves as the perfect cherry on top to optimize our memory footprint and execution speed.

I hope this thorough walkthrough provides you with a robust understanding of the moving parts inside this puzzle. Take your time to trace through the logic with a pencil and paper, and feel free to reach out and share your own insights or alternative implementations. Happy coding!
