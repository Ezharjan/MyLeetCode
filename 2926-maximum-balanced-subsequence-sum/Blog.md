# Unlocking the Maximum Balanced Subsequence Sum Puzzle [#2926]

Welcome fellow coding enthusiasts to another deep dive into the fascinating world of algorithms and data structures! Today we are looking at a brilliant array puzzle that tests our ability to combine mathematics, dynamic programming, and clever data management. Finding the maximum sum of a balanced subsequence is a wonderful challenge that teaches us how to look at a problem from a completely different angle. 

Whether you are just starting your algorithmic journey or you are a seasoned programmer looking to sharpen your skills, I am thrilled to share my thoughts and a very solid, modest approach to solving this puzzle. Grab your favorite beverage, get comfortable, and let us unravel this together step by step!



## Understanding the Rules of the Game

Before we even think about writing code, we need to build a crystal clear understanding of what the problem is asking us to do. We are provided with a zero indexed array of integers. Our overarching goal is to select a "subsequence" from this array. 

For those who might be new to the term, a subsequence is simply a new array that you form by picking elements from the original array while strictly maintaining their original left to right order. You are allowed to delete some elements, or you might choose not to delete any at all. However, you can never swap the positions of the elements. For instance, if your original array is `[5, 1, 3, 8]`, a valid subsequence could be `[5, 8]` or `[1, 3]`. An invalid subsequence would be `[8, 5]` because the order has been reversed.

Now, what exactly makes a subsequence "balanced"? The problem gives us a very specific mathematical condition. For every adjacent pair of elements in our chosen subsequence, let us denote the current element and the previous element. The rule dictates that the value of the current element minus the value of the previous element must be greater than or equal to the original index of the current element minus the original index of the previous element.

To express this rule purely in words:
The difference in their values must be greater than or equal to the difference in their original indices.

If this condition holds true for every single consecutive pair in our chosen subsequence, then congratulations, we have a balanced subsequence! Furthermore, the problem states that a subsequence consisting of just one single element is always considered balanced by default. Our final mission is to look at all possible balanced subsequences and return the maximum possible sum of their elements.

## The Magic of Algebraic Rearrangement

When you first read the balancing condition, it might feel a bit intimidating. We are comparing the difference of values against the difference of indices. Comparing relative differences across varying gaps can be very tricky to track using standard programming loops. 

Whenever you encounter a relationship that compares a difference of two states, a fantastic strategy is to group the terms belonging to the same state on the same side of the inequality. Let us play with the rule a bit. 

We start with:
`Value of current element minus Value of previous element >= Index of current element minus Index of previous element`

Now, let us move the terms around. We can add the `Value of previous element` to both sides, and we can subtract the `Index of current element` from both sides. This simple algebraic shift completely transforms the problem. 

The rule elegantly becomes:
`Value of current element minus Index of current element >= Value of previous element minus Index of previous element`

Do you see the magic here? The condition is now entirely localized! We no longer have to worry about the gap between the two elements. Instead, we can calculate a special "score" or "transformed value" for every single element in our original array. This score is simply the element's value minus its original index.

The problem has now evolved into something much more approachable. We simply need to find a subsequence where these transformed scores are non decreasing, and among all such valid subsequences, we want to find the one that yields the highest sum of their original values.

## Why a Basic Approach Runs Out of Time

A natural first instinct is to use a classic Dynamic Programming approach. We could create an array where we store the maximum sum ending at each specific index. As we iterate through our elements one by one, we could look back at every single previously processed element. If a previous element has a transformed score that is less than or equal to our current element's transformed score, we could consider extending that previous sequence with our current element. 

While this logic is fundamentally correct and works perfectly for small inputs, we have to consider the scale of the problem. If our array contains one hundred thousand elements, looking back at all previous elements for every new element means we would be performing roughly five billion comparisons! In the world of competitive programming and efficient software development, this quadratic time complexity will almost certainly result in a Time Limit Exceeded error. We need a way to find that "best previous sum" much faster.

## Introducing the Power of Coordinate Compression

Because our transformed scores are calculated by subtracting the index from the value, these scores can span a massive range. They could be extremely large positive numbers or extremely large negative numbers. We cannot simply use these scores as direct indices in a lookup array because an array cannot have negative indices, and an array of size one billion would consume too much memory.

This is where a beautiful technique called Coordinate Compression comes to the rescue. 

Coordinate compression is a way to map a scattered set of large numbers into a compact, contiguous range of small integers, usually starting from one. We do this by gathering all the unique transformed scores we encounter in our array, sorting them in ascending order, and then assigning them a rank based on their sorted position.

Let us illustrate this with a small example. Suppose our transformed scores are 500, negative 20, and 9000. 
* The smallest score is negative 20, so we assign it rank 1.
* The next score is 500, so we assign it rank 2.
* The largest score is 9000, so we assign it rank 3.

By compressing the coordinates, we retain the most crucial piece of information which is the relative order of the scores. If score A is less than score B, then rank A will be less than rank B. Now, instead of dealing with wild, scattered numbers, we only deal with neat ranks ranging from one up to the total number of unique scores. This compact range is perfect for indexing into an advanced data structure!



## Harnessing the Binary Indexed Tree

Now that our scores are compressed into manageable ranks, we need a data structure that can efficiently answer a specific question. Given a current rank, what is the maximum subsequence sum we have discovered so far among all previously processed elements whose rank is less than or equal to our current rank?

A Segment Tree or a Binary Indexed Tree are both excellent candidates for this job. For this solution, we will utilize the Binary Indexed Tree, affectionately known in the community as the Fenwick Tree. The Fenwick Tree is famous for being incredibly concise to code while providing lightning fast updates and queries.



While Fenwick Trees are most commonly used to calculate prefix sums, they can easily be adapted to keep track of prefix maximums! Our Fenwick Tree will be an array sized slightly larger than our maximum rank.

As we process each element in our original array:
1. We calculate its transformed score.
2. We look up its compressed rank.
3. We query the Fenwick Tree to find the maximum sum accumulated by any valid previous sequence ending with a rank less than or equal to our current rank.
4. We evaluate the best sum for the current element. If the best previous sum is positive, we add our current element's original value to it. If the best previous sum is negative or zero, adding our current element to it would only drag our current element down. In that case, we are better off starting a brand new subsequence starting fresh with just our current element.
5. Finally, we update the Fenwick Tree at our current rank with this newly calculated maximum sum, so that future elements appearing later in the array can build upon our success.

## Walking Through an Example Step by Step

To solidify our understanding, let us meticulously trace through a concrete example. Imagine our given array is `[3, 3, 5, 6]`.

First, we calculate the transformed scores by subtracting the index from the value:
* For value 3 at index 0, the transformed score is 3 minus 0, which equals 3.
* For value 3 at index 1, the transformed score is 3 minus 1, which equals 2.
* For value 5 at index 2, the transformed score is 5 minus 2, which equals 3.
* For value 6 at index 3, the transformed score is 6 minus 3, which equals 3.

Our collection of transformed scores is `3, 2, 3, 3`.
Next, we perform coordinate compression. We find the unique scores, which are `2` and `3`. We sort them in ascending order.
* The score 2 receives rank 1.
* The score 3 receives rank 2.

Now we initialize our Fenwick Tree to track the maximum sums for these ranks. The tree starts entirely empty, effectively filled with zeros.

We begin processing the original array from left to right:

**Element at Index 0:**
The original value is 3. Its transformed score is 3. Based on our compression, its rank is 2. We ask our Fenwick Tree for the maximum sum up to rank 2. The tree is empty, so it returns 0. The best sum ending at this element is `0 + 3 = 3`. We update our Fenwick Tree at rank 2 with the value 3. Our overall maximum sum found so far is 3.

**Element at Index 1:**
The original value is 3. Its transformed score is 2. Its rank is 1. We ask our Fenwick Tree for the maximum sum up to rank 1. The tree has no positive values at rank 1 yet, so it returns 0. The best sum ending at this element is `0 + 3 = 3`. We update our Fenwick Tree at rank 1 with the value 3. Our overall maximum sum remains 3.

**Element at Index 2:**
The original value is 5. Its transformed score is 3. Its rank is 2. We ask our Fenwick Tree for the maximum sum up to rank 2. The tree looks at the values stored up to rank 2 and finds the maximum is 3. The best sum ending at this element is `3 + 5 = 8`. We update our Fenwick Tree at rank 2 to reflect this new higher value of 8. Our overall maximum sum increases to 8.

**Element at Index 3:**
The original value is 6. Its transformed score is 3. Its rank is 2. We ask our Fenwick Tree for the maximum sum up to rank 2. The tree finds our previously stored maximum of 8. The best sum ending at this element is `8 + 6 = 14`. We update our Fenwick Tree at rank 2 with the value 14. Our overall maximum sum increases to 14.

We have finished processing the array! The highest sum we recorded during this entire journey is 14. This is exactly the correct answer for this example.

## Handling the All Negative Edge Case

Before we look at the complete code, we must address a very important edge case. What happens if every single number in our input array is negative? 

If all numbers are strictly less than zero, any combination of two or more numbers will result in an even smaller, more negative sum. Because the problem states that a subsequence of length one is perfectly valid and balanced, our absolute best strategy in an all negative scenario is to simply pick the single maximum element in the entire array. 

To handle this gracefully, we can do a quick preliminary scan of the array. We find the maximum individual value. If this maximum value is less than or equal to zero, we can immediately return it and skip the rest of the complex algorithm entirely. This small check saves a lot of unnecessary computation!

## The Complete Implementation

Here is how we translate all of these intricate concepts into a clean, well structured C++ solution. I have carefully documented the code to mirror the logical steps we just discussed.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maxBalancedSubsequenceSum(vector<int>& nums) {
        int n = nums.size();
        
        // Phase 1: The Negative Edge Case
        // We scan the array to find the highest single value.
        // If the highest value is zero or negative, we cannot possibly build 
        // a larger sum by adding more negative numbers together. 
        // We simply return this single highest value.
        long long maximum_single_element = -1e18;
        for (int i = 0; i < n; ++i) {
            maximum_single_element = max(maximum_single_element, (long long)nums[i]);
        }
        if (maximum_single_element <= 0) {
            return maximum_single_element;
        }

        // Phase 2: Preparing for Coordinate Compression
        // We calculate the transformed score for each element by 
        // subtracting its index from its original value.
        vector<int> compressed_scores(n);
        for (int i = 0; i < n; ++i) {
            compressed_scores[i] = nums[i] - i;
        }
        
        // We sort the scores and remove any consecutive duplicates.
        // This gives us a strictly increasing list of unique scores.
        sort(compressed_scores.begin(), compressed_scores.end());
        compressed_scores.erase(unique(compressed_scores.begin(), compressed_scores.end()), compressed_scores.end());

        int total_unique_ranks = compressed_scores.size();
        
        // Phase 3: Setting up the Fenwick Tree
        // The tree uses one based indexing, so we size it appropriately.
        // It will store the maximum sum we have seen for each rank.
        vector<long long> fenwick_tree(total_unique_ranks + 1, 0);

        // This local helper function updates the Fenwick tree.
        // It propagates a new maximum sum upward through the tree hierarchy.
        auto update_maximum_in_tree = [&](int current_rank, long long new_sequence_sum) {
            for (; current_rank <= total_unique_ranks; current_rank += current_rank & -current_rank) {
                fenwick_tree[current_rank] = max(fenwick_tree[current_rank], new_sequence_sum);
            }
        };

        // This local helper function queries the Fenwick tree.
        // It retrieves the absolute maximum sum stored across all ranks 
        // less than or equal to the requested rank.
        auto query_maximum_from_tree = [&](int target_rank) {
            long long maximum_found = 0;
            for (; target_rank > 0; target_rank -= target_rank & -target_rank) {
                maximum_found = max(maximum_found, fenwick_tree[target_rank]);
            }
            return maximum_found;
        };

        long long absolute_maximum_sum = 0;
        
        // Phase 4: Constructing the Balanced Subsequences
        // We iterate through our original array one last time.
        for (int i = 0; i < n; ++i) {
            int current_score = nums[i] - i;
            
            // We use binary search (lower bound) to quickly find the 
            // one based rank of our current score.
            int assigned_rank = lower_bound(compressed_scores.begin(), compressed_scores.end(), current_score) - compressed_scores.begin() + 1;
            
            // We ask our tree for the best sum we can append ourselves to.
            long long best_prior_sum = query_maximum_from_tree(assigned_rank);
            
            // We calculate the sum if we extend that sequence.
            long long current_total_sum = best_prior_sum + nums[i];
            
            // If our calculated sum is strictly positive, it is a valuable 
            // stepping stone for future elements, so we store it in the tree.
            if (current_total_sum > 0) {
                update_maximum_in_tree(assigned_rank, current_total_sum);
            }
            
            // We continuously update our global tracker to ensure we do not lose 
            // the highest value ever computed during this loop.
            absolute_maximum_sum = max(absolute_maximum_sum, current_total_sum);
        }

        return absolute_maximum_sum;
    }
};

```

## Evaluating Time and Space Complexity

A thorough programmer always evaluates the resource requirements of their solution. Let us break down both time and space complexity in plain terms.

**Time Complexity Analysis**
Our algorithm operates in a few distinct phases. First, we iterate through the array to check for the all negative edge case, which takes linear time. Next, we build our list of transformed scores and sort them. Sorting the array of scores requires `O(N log N)` time, where `N` represents the total number of elements in our input array. Removing the duplicates takes linear time.

The main workload happens in our final loop. For each of the `N` elements, we perform a binary search using the `lower_bound` function to find its rank, which takes logarithmic time, `O(log N)`. Then, we perform a query and a potential update on our Fenwick Tree. The tree operations jump through bits, resolving in logarithmic time as well. Since we perform logarithmic operations exactly `N` times, the dominant loop also bounds to `O(N log N)`.

Taking everything into consideration, the overall time complexity of this solution stands firmly at `O(N log N)`. This is highly efficient and will easily pass the time limit constraints even for very large arrays.

**Space Complexity Analysis**
We create a few auxiliary structures to support our logic. We have an array to store the unique transformed scores, which in the absolute worst case where every score is uniquely different, will grow to size `N`. Furthermore, our Fenwick Tree requires an array of size equal to the number of unique ranks plus one. Again, this scales directly with `N` in the worst case scenario.

Since all our extra data structures scale linearly with the size of the input, the total space complexity required by our approach is `O(N)`. This is a very reasonable memory footprint for the massive performance gains we achieve over the basic quadratic approach.

## A Friendly Wrap Up

I sincerely hope this extensive walkthrough has helped clarify this brilliant problem! By taking a moment to apply a simple algebraic trick, we managed to completely reshape the landscape of the challenge. Combining that mathematical insight with the space saving elegance of Coordinate Compression and the blazing speed of a Fenwick Tree transforms a daunting task into a smooth, logical progression.

Whenever you face a problem that compares differences between indices and values, always remember this algebraic rearrangement trick. And whenever you need to track prefix maximums or sums over an enormous range of scattered values, keep coordinate compression and binary indexed trees in your toolkit.

Thank you so much for reading and studying along with me. Keep practicing these intricate patterns, share your knowledge with the community, and I look forward to exploring the next coding puzzle with you all! Happy coding!
