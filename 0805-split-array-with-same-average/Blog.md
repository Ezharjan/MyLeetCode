# Splitting An Array Into Two Parts With Identical Averages A Friendly Walkthrough [#805]

Hello wonderful coding community! Today I would absolutely love to share my thoughts and a complete walkthrough on a very fascinating array puzzle. This puzzle asks us to divide a group of numbers into two separate nonempty groups where both groups end up sharing the exact same mathematical average. 

Sometimes, solving these puzzles can feel a bit overwhelming, but if we break everything down logically and take it step by step, we can arrive at a very beautiful and reliable solution together. I am writing this to be as detailed and comprehensive as possible so that absolutely anyone can follow along, regardless of their background. Grab a comfortable beverage, and let us dive into the logic!

## Understanding The Core Puzzle

Imagine you are given a list of whole numbers. Your goal is to separate these numbers into exactly two distinct buckets. Let us call them Bucket A and Bucket B. 

There are a few simple rules we must follow to succeed at this puzzle
* Every single number from the original list must be placed into either Bucket A or Bucket B.
* We cannot leave any numbers behind.
* Neither Bucket A nor Bucket B can be completely empty. Both must contain at least one number.
* Finally, and most importantly, when you calculate the mathematical average of the numbers inside Bucket A, it must perfectly equal the mathematical average of the numbers inside Bucket B.

To recall how an average works, you add up all the numbers in a group and then divide that sum by the total count of numbers in that group.

Let us look at a simple example to visualize this concept. Suppose our original list of numbers is [1, 2, 3, 4, 5, 6, 7, 8]. We want to know if it is possible to split this list. If we try placing 1, 4, 5, and 8 into Bucket A, their total sum becomes 18. Since there are 4 numbers in Bucket A, their average is 18 divided by 4, which equals 4.5. 
The remaining numbers for Bucket B would be 2, 3, 6, and 7. The total sum of Bucket B is also 18. Since Bucket B also contains 4 numbers, its average is 18 divided by 4, which is exactly 4.5. 
Because both averages match perfectly, we would return a true result for this list!

If we are given a list like [3, 1], we can only split it by putting the 3 in one bucket and the 1 in the other bucket. The average of the first bucket is 3, and the average of the second bucket is 1. These do not match, so we would return a false result.

## The Mathematical Foundation

Now, let us think about the math behind this puzzle using plain text and simple concepts. We do not need complex formulas to understand the beauty of this relationship.

Suppose our entire starting list has a total sum which we will call TotalSum, and it has a total count of numbers which we will call N. 

If Bucket A and Bucket B have the exact same average, then an interesting mathematical rule applies. Their shared average must also be perfectly equal to the average of the entire original list combined together. 

Therefore, the average of Bucket A is simply TotalSum divided by N.

To find the actual sum of the numbers we need to gather for Bucket A, we can multiply the number of items we intend to put in Bucket A (let us call this target count K) by that overall average. 

This gives us a very simple plain text formula for the sum we are looking for
Target Sum For Bucket A = K multiplied by TotalSum, all divided by N.

Because all the numbers we are dealing with are whole integers, the combined sum for Bucket A must naturally be a whole integer as well. It cannot be a fraction or a decimal. This gives us our very first wonderful shortcut to filter out impossible situations! We only need to spend our time searching for combinations of length K if K multiplied by TotalSum is perfectly divisible by N. If there is a remainder when we divide, we know right away that building a whole integer sum is mathematically impossible, so we can skip checking that specific length entirely.

## Limiting Our Search Space

We can apply another very helpful logical constraint to save computing power. We only need to test sizes for Bucket A starting from exactly 1 up to N divided by 2. 

You might wonder why we stop exactly at the halfway mark. Think about it this way. If there exists a valid Bucket A that successfully solves the puzzle, and its size happens to be larger than half the original list, then the remaining numbers left over in Bucket B will naturally form a valid group that is smaller than or equal to half the list. 

Since the rules of the puzzle treat Bucket A and Bucket B completely equally, we could simply swap their names! We can just pretend the smaller bucket is our Bucket A. Therefore, by only searching for bucket sizes up to the halfway point, we guarantee that we will find a solution if one exists, without doing double the work.

## Building A State Machine With Dynamic Programming

Now we have our target sizes and our target sums. The puzzle has officially transformed. We no longer care about averages directly. We just need to answer one specific question. Can we pick exactly K numbers from our list so that they add up exactly to our Target Sum?

We can solve this using an approach called Dynamic Programming. Dynamic Programming is a method where we break a large problem down into smaller steps and remember the results of those smaller steps so we never have to calculate them twice.

Imagine keeping a written log for every possible length K. The log for length K holds every single unique sum you can possibly make using exactly K numbers from the list.

When we look at a new number from our original list, we can update our logs. We take every single sum we built previously using K minus one numbers, add our new number to all of them, and write these new totals down in the log for length K.

For instance, if we know we can make a sum of 10 using 2 numbers, and our new number is 5, we now know we can make a sum of 15 using 3 numbers. We repeat this addition process for every number in our list, carefully updating our logs from the largest bucket size down to the smallest. We update backwards so that we do not accidentally use the exact same new number more than once in the same passing turn.

## The Bitset Data Structure

To make our log updating process incredibly fast and friendly on computer memory, we can use a very clever data structure called a bitset. 

A bitset is essentially a massively long row of true and false flags. Each flag represents a specific sum. 
If the flag at position 50 is set to true, it means we have successfully discovered a way to build a sum of exactly 50. If the flag at position 50 is false, it means we have not found a way to build that sum yet.

When we want to add a new number (let us say, the number 5) to all our existing true sums, we do not need to check each flag one by one. Instead, we can apply an operation called a left shift. 

A left shift literally takes the entire row of true and false flags and slides every single one of them up by exactly 5 positions all at once. 

So, if the flag at position 10 was true, the sliding motion automatically moves that true state to position 15. The computer hardware performs this sliding motion on many flags simultaneously, which is what makes this method so incredibly capable. We then merge these newly slid flags with our current flags using an inclusive OR operation, which simply ensures we keep all our old valid sums while also recording the new ones.

## Step By Step Code Walkthrough

Let us look at how we can implement this exact logic using C++. I will explain what each part of the code is doing. Note that in C++, we use standard subtraction symbols for the logic to compile correctly.

```cpp
class Solution {
public:
    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size();
        
        // If there is only one number, we cannot possibly split it into two nonempty buckets.
        if (n <= 1) return false;
        
        // We calculate the total sum of all numbers in the entire list.
        int totalSum = 0;
        for (int x : nums) {
            totalSum += x;
        }
        
        // We only need to check bucket sizes up to exactly half the total list size.
        int m = n / 2;
        bool possible = false;
        
        // Quick filter check
        // Is there any valid bucket length 'k' where the required target sum would be a whole integer?
        for (int k = 1; k <= m; ++k) {
            if (totalSum * k % n == 0) {
                possible = true;
                break;
            }
        }
        
        // If no valid whole integer sum exists for any size, it is mathematically impossible to solve.
        if (!possible) return false;
        
        // Here we prepare our logs using bitsets.
        // The maximum possible sum according to the problem constraints is 30 numbers multiplied by 10000.
        // So we need a bitset that can hold flags up to position 300000. We add a little extra room for safety.
        // We use a vector to store a separate bitset for each possible bucket length.
        vector<bitset<300005>> dp(m + 1);
        
        // We can always make a sum of 0 using exactly 0 elements. We set that flag to true.
        dp[0].set(0); 
        
        // Now we process every single number from our list one by one.
        for (int x : nums) {
            // We iterate backwards from the largest possible bucket size down to 1.
            // This guarantees we only use the current number 'x' exactly one time.
            for (int k = m; k >= 1; --k) {
                // We take all sums we built with 'k - 1' elements, add 'x' to all of them 
                // using a bitwise left shift, and merge the results into the log for 'k' elements.
                dp[k] |= (dp[k - 1] << x);
            }
        }
        
        // Finally, we check our logs to see if we successfully built any of our mathematical target sums.
        for (int k = 1; k <= m; ++k) {
            // We verify the target sum is a whole integer, and then check if the corresponding flag is true.
            if (totalSum * k % n == 0 && dp[k].test(totalSum * k / n)) {
                return true;
            }
        }
        
        // If we reach this point, no valid split was found.
        return false;
    }
};

```

## Exploring The Complexity

Understanding the complexity of our method helps us appreciate why it runs successfully within the time limits. Let us discuss the time and space requirements using clear and friendly terms.

For the Time Complexity, our main work happens inside the nested loops where we process each number. We loop over all N numbers in our list. For each number, we loop downwards through the possible bucket sizes, which takes roughly N divided by 2 steps. Inside that inner loop, we perform the bitwise left shift operation. Because a bitset processes flags in large chunks at the hardware level, this sliding operation is significantly faster than checking numbers individually.
Overall, the time taken is proportional to the total count of numbers squared, multiplied by the maximum possible sum, and then divided by the size of the chunks the computer processes at once. This results in a very fast execution time that easily handles the upper limits of the puzzle.

For the Space Complexity, we must allocate memory for our bitsets. We create a list of bitsets where the list length is roughly half the total count of numbers. Each bitset needs enough flags to cover the maximum possible sum, which is about three hundred thousand flags. Since each flag takes up merely a tiny fraction of memory, holding all these bitsets requires an exceptionally small amount of total memory space. This completely respects the memory limits of the platform and keeps our program running smoothly.

## Final Thoughts

I hope this thorough breakdown brings a lot of clarity to how we can reduce a seemingly complex average matching problem into a much simpler subset sum puzzle. By combining straightforward algebraic logic with the wonderful sliding capabilities of bitsets, we arrive at an answer that is clean, precise, and highly reliable.

Thank you so much for reading through this walkthrough. Learning these concepts takes patience, and you are doing fantastic by exploring them. Please feel free to copy the code, experiment with it on your own, and share your own discoveries with the community! Happy coding everyone!
