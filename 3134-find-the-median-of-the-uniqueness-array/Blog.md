# Finding the Middle Ground in Subarray Distinct Elements [#3134]

Hello everyone! Today I wanted to share my thoughts and a functional approach to a fascinating array problem that recently caught my attention. Array problems are a staple in our daily programming practice, and this particular one requires us to find the median of a conceptual array built from counting unique elements. 

I will walk you through the problem details, explain the thought process from the ground up, and present a practical solution that combines two beloved algorithmic patterns. My hope is that by the end of this read, you will feel completely comfortable tackling similar logic in your own projects. Let us dive right in!

### Understanding the Problem

Imagine you are given a list of integers. Your task is to look at every single possible continuous segment of this list. These continuous segments are known as subarrays. For every single subarray, you need to count how many unique numbers exist inside it. 

If you take all of these unique counts and write them down in a new list, and then sort that new list in ascending order, you have created what the problem calls a uniqueness array. The ultimate goal is to find the median value of this sorted uniqueness array. 

The median is simply the middle value when a list is sorted. If the list has an even number of items and there are two middle values, we are instructed to pick the smaller of the two.

Let us look at a concrete example to make this crystal clear.

Suppose our input array is `[1, 2, 3]`.
The possible subarrays and their unique element counts are as follows:
* `[1]` has 1 unique element
* `[2]` has 1 unique element
* `[3]` has 1 unique element
* `[1, 2]` has 2 unique elements
* `[2, 3]` has 2 unique elements
* `[1, 2, 3]` has 3 unique elements

If we collect these counts, we get `[1, 1, 1, 2, 2, 3]`. The array is already sorted. Since there are six elements, the middle ones are the third and fourth items, which are `1` and `2`. We take the smaller one, so the median is `1`.

### The Initial Thought Process and Why the Simple Way Struggles

When we first encounter a problem like this, the most natural instinct is to do exactly what the problem description says. We could write a set of nested loops to generate every single subarray, use a hash set to count the unique elements in each one, store all those counts in a massive array, sort it, and return the middle element.

However, we need to consider the constraints. The length of our input array can be up to 100,000. 

Let us calculate how many subarrays exist in an array of size N. The formula is N * (N + 1) / 2. 
If N is 100,000, the number of subarrays is 100,000 * 100,001 / 2, which equals a staggering 5,000,050,000. 

Creating an array of five billion elements, storing them in memory, and then sorting them is simply not feasible. It will consume too much memory and take far too long to execute. We need a strategy that does not require us to actually build the uniqueness array.

### The Core Insight Building Blocks

Instead of building the list, we can use a guessing game. This guessing game is formally known as Binary Search on the Answer. 

Think about the possible answers. The minimum number of unique elements any subarray can have is 1. The maximum number of unique elements any subarray can have is the total number of unique elements in the entire input array. The true median must fall somewhere in this range.

Because the property we are checking is monotonic, we can guess a median value, let us call it `M`. If we can efficiently count how many subarrays have at most `M` distinct elements, we can determine if our guess is too high or too low.

To find the median, we need a specific number of subarrays to fall below or equal to our guess. For an array of N total subarrays, the median index we are targeting is (N + 1) / 2. 

If the number of subarrays with at most `M` distinct elements is greater than or equal to our target, it means our guess `M` is either the correct median or it is too large. We then lower our upper bound. If the count is less than our target, our guess `M` is too small, and we must raise our lower bound.

### Counting Efficiently with a Sliding Window

Now we have a new subproblem. Given a number `M`, how do we quickly count the number of subarrays that contain at most `M` distinct elements? 

We can use the Sliding Window technique. We maintain a window defined by a left pointer and a right pointer. As we expand our window by moving the right pointer, we keep track of the frequencies of the elements inside the window. 

If adding a new element causes the number of distinct elements to exceed `M`, we must shrink our window from the left. We move the left pointer forward, decreasing the frequencies of the elements we remove, until the number of distinct elements drops back down to `M` or below.

Once the window is valid, every subarray ending at the right pointer and starting anywhere from the left pointer up to the right pointer is also valid. The number of such valid subarrays is exactly `right pointer minus left pointer + 1`. We add this to a running total. By the time the right pointer reaches the end of the array, we will have the total count of valid subarrays.

### A Detailed Step by Step Example

Let us trace the logic using a slightly larger example: `[4, 3, 5, 4]`.

First, let us find our target median index. 
The length of the array is 4.
Total subarrays = 4 * 5 / 2 = 10.
Target index = (10 + 1) / 2 = 5. We need to find the smallest number of distinct elements such that at least 5 subarrays have that many or fewer distinct elements.

The unique elements in the entire array are 4, 3, and 5. There are 3 unique elements in total.
Our binary search range is from 1 to 3.

**First Guess:**
Left is 1, Right is 3.
Our middle guess `M` is (1 + 3) / 2 = 2.
We want to count subarrays with at most 2 distinct elements.

Let us run the sliding window:
* Move right pointer to index 0 (value 4). Distinct count is 1. Valid subarrays ending here: `[4]`. Total count is 1.
* Move right pointer to index 1 (value 3). Distinct count is 2. Valid subarrays ending here: `[4, 3]`, `[3]`. Total count is 1 + 2 = 3.
* Move right pointer to index 2 (value 5). Distinct count is 3. This exceeds our limit `M` of 2. We must shrink from the left.
* Move left pointer to index 1. We remove the first 4. Distinct count becomes 2. Window is valid. Valid subarrays ending here: `[3, 5]`, `[5]`. Total count is 3 + 2 = 5.
* Move right pointer to index 3 (value 4). Distinct count becomes 3 again (elements are 3, 5, 4). This exceeds our limit. We shrink from the left.
* Move left pointer to index 2. We remove the 3. Distinct count becomes 2. Window is valid. Valid subarrays ending here: `[5, 4]`, `[4]`. Total count is 5 + 2 = 7.

Our total count of subarrays with at most 2 distinct elements is 7. 
Our target was 5. Since 7 is greater than or equal to 5, our guess of 2 is a valid candidate for the median. We save 2 as our current answer, and we adjust our binary search right boundary to `M` minus 1, which is 1.

**Second Guess:**
Left is 1, Right is 1.
Our middle guess `M` is (1 + 1) / 2 = 1.
We want to count subarrays with at most 1 distinct element.

Let us run the sliding window:
* Right at 0 (value 4). Distinct is 1. Count = 1.
* Right at 1 (value 3). Distinct is 2. Exceeds limit. Move left to 1. Distinct is 1. Count = 1 + 1 = 2.
* Right at 2 (value 5). Distinct is 2. Exceeds limit. Move left to 2. Distinct is 1. Count = 2 + 1 = 3.
* Right at 3 (value 4). Distinct is 2. Exceeds limit. Move left to 3. Distinct is 1. Count = 3 + 1 = 4.

Our total count of subarrays with at most 1 distinct element is 4.
Our target is 5. Since 4 is less than 5, our guess of 1 is too small. We adjust our binary search left boundary to `M` plus 1, which is 2.

Now Left is 2 and Right is 1. The binary search loop terminates. 
Our saved answer was 2. The true median is indeed 2!

### The C++ Code Implementation

Here is a clean and functional implementation of the concepts we just discussed. I chose to use a preallocated vector for the frequency map because array lookups are incredibly fast, which helps avoid any timing out issues on larger test cases.

```cpp
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    int medianOfUniquenessArray(vector<int>& nums) {
        long long n = nums.size();
        
        // Calculate the absolute total number of subarrays
        long long total_subarrays = n * (n + 1) / 2;
        
        // We are looking for the median, which sits at this target index
        long long target = (total_subarrays + 1) / 2;
        
        // Find the absolute maximum element to properly size our frequency vector
        int max_val = *max_element(nums.begin(), nums.end());
        vector<int> freq(max_val + 1, 0);
        
        unordered_set<int> unique_elements(nums.begin(), nums.end());
        int left = 1;
        int right = unique_elements.size();
        int answer = right;
        
        // A helper function to count subarrays with at most K distinct elements
        auto countSubarraysWithAtMostK = [&](int k) {
            long long count = 0;
            int window_start = 0;
            int distinct = 0;
            
            // Clear the frequency array for this fresh counting run
            fill(freq.begin(), freq.end(), 0); 
            
            for (int window_end = 0; window_end < n; ++window_end) {
                if (freq[nums[window_end]]++ == 0) {
                    distinct++;
                }
                
                // If we have too many distinct elements, shrink the window
                while (distinct > k) {
                    if (--freq[nums[window_start]] == 0) {
                        distinct--;
                    }
                    window_start++;
                }
                
                // Tally up the valid subarrays ending at the current right pointer
                count += (window_end - window_start + 1);
            }
            return count;
        };
        
        // Execute the binary search to narrow down the median
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (countSubarraysWithAtMostK(mid) >= target) {
                answer = mid;       // We found a valid boundary, look for smaller
                right = mid - 1;
            } else {
                left = mid + 1;     // Not enough subarrays, we must increase our median guess
            }
        }
        
        return answer;
    }
};

```

### Analyzing the Complexity

It is always important to understand the footprint of our code. Let us break down both time and space.

**Time Requirements:**
We perform a binary search where the range is bounded by K, which is the total number of unique elements in the input. This means the binary search loop runs `log K` times.
Inside each step of the binary search, we execute our sliding window function. The sliding window travels across the entire array of length N once. Both the left and right pointers only move forward, meaning the operations inside the sliding window take linear time, or O(N).
Therefore, the overall time complexity is O(N log K). This is wonderfully efficient and will easily pass the tightest constraints without breaking a sweat.

**Space Requirements:**
We created a frequency array that is sized based on the maximum integer value found in the input. Let us call this maximum value V. The size of our frequency array is exactly V plus 1. We also maintain a hash set initially just to figure out the upper bound for our binary search, which holds up to N elements.
So the overall space complexity is O(V + N). Given that the problem constraints state the maximum value inside the array will not exceed 100,000, creating an array of this size requires a very tiny amount of memory. Using this flat array instead of a hash map for frequency counting provides a massive speed boost due to continuous memory allocation and lack of hash collisions.

### Final Thoughts

This problem is a fantastic showcase of how seemingly impossible memory and time limits can be bypassed by combining foundational techniques. By not generating the subarrays explicitly and instead relying on mathematical properties and binary search, we transformed a brute force nightmare into an elegant and performant logic flow.

I hope this breakdown was helpful and sheds some light on how to approach these types of advanced sliding window problems. Feel free to use this logic as a template for other "Kth smallest" or "Median of" array problems you encounter in the wild.

Happy coding, and let me know if you discover any fun variations of this algorithm!
