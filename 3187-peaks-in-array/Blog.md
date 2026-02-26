# Unlocking the Secrets of Array Peaks and Fast Range Queries [#3187]

Hello friends and fellow coding enthusiasts! Today we are going straight into an incredibly fascinating computer science puzzle. We will be exploring a problem that asks us to identify peaks in an array and maintain our counts while the array is constantly changing. We will walk through the logic step by step so that everyone from beginners to experienced programmers can follow along and add a wonderful new technique to their toolkit. 

I am excited to share this knowledge with the community. We will explore the concepts together, build a solid mental model, and write some clean C++ code. Let us jump right in!

### Understanding the Challenge

Before we write any code, we need to clearly define what we are dealing with. Imagine you are walking along a mountain range. Some points are higher than the points immediately next to them. In the context of our array of integers, a peak is defined as an element that is strictly greater than both its left neighbor and its right neighbor. 



For example, if we have the sequence of numbers 1, 4, 2, the number 4 is a peak because it is greater than 1 and greater than 2. However, there is a very important boundary rule to remember. The very first element of the array and the very last element of the array can never be considered peaks. They do not have both a left and a right neighbor to compare against!

The problem presents us with two distinct types of operations we must perform:
* Type 1: We are given a left index and a right index, and we must count exactly how many peaks exist entirely within that specific subarray boundary.
* Type 2: We are given an index and a new value, and we must update our original array by replacing the number at that index with the new value.

Our goal is to process all these operations in the exact order they are given and return the answers for all the Type 1 operations.

### Why a Simple Loop Falls Short

When we first look at this puzzle, the most intuitive approach might be to just run a loop. Whenever we get a Type 1 operation, we could start at the left index, walk all the way to the right index, and check every single element to see if it is a peak. Whenever we get a Type 2 operation, we simply change the value at the given index.

This simple method is wonderfully easy to understand, but it has a hidden trap. Imagine our array is huge, perhaps containing a hundred thousand elements. Now imagine we are given a hundred thousand operations, and most of them ask us to count the peaks from the very beginning to the very end of the array. Our simple loop would have to check a hundred thousand elements, a hundred thousand times! This would require billions of operations, which will definitely result in a Time Limit Exceeded error. We need a clever way to store our peak counts so we can fetch them almost instantly.

### Enter the Binary Indexed Tree

To solve this efficiently, we will use a brilliant data structure called a Binary Indexed Tree, which is also affectionately known as a Fenwick Tree. 



A Binary Indexed Tree is a clever way to store an array of numbers such that you can calculate the sum of any prefix of the array extremely fast. It also allows you to update any single value in the array extremely fast. Both fetching the sum and updating a value take logarithmic time, which is vastly superior to the linear time of our naive loop.

Here is how we will use it:
1.  We will maintain a separate list of integers called our peak tracker. If the element at index i is a peak, the peak tracker at index i will hold the value 1. If it is not a peak, it will hold the value 0.
2.  We will feed this peak tracker into our Binary Indexed Tree.
3.  When we need to find the number of peaks between a left index and a right index, we simply ask our Binary Indexed Tree for the total sum of all the 1s in that range.

### The Magic of Point Updates

The most beautiful part of this logic revolves around the Type 2 operations, where we change a value in our array. 

When you change a single number in an array, how many elements could possibly change their peak status? The answer is exactly three!
* The element you just changed might become a new peak, or it might stop being a peak.
* The element immediately to its left might have its peak status changed because its right neighbor just changed.
* The element immediately to its right might have its peak status changed because its left neighbor just changed.

No other elements in the entire array care about this update. Their local neighborhoods remain completely undisturbed. Therefore, whenever we process a Type 2 operation, we only need to recalculate the peak status for the target index, the index to its left, and the index to its right. If any of their peak statuses change from 0 to 1, or from 1 to 0, we simply tell our Binary Indexed Tree to add or subtract 1 at that specific position.

### Navigating the Boundaries

We must be extremely careful with the rules regarding boundaries. As stated earlier, the first and last elements of the main array cannot be peaks. 

But there is an even trickier boundary rule! When we are asked to count the peaks within a subarray from a left index to a right index, the problem strictly implies that the peaks must exist entirely within that subarray. This means the element at the left index of the query cannot be counted as a peak for this specific query, even if it is a peak in the main overall array! The same logic applies to the right index of the query.

To handle this elegantly, when we are asked to check the range from left to right, we will actually ask our Binary Indexed Tree for the sum of peaks between the index strictly after the left boundary and the index strictly before the right boundary. If the given subarray is so small that it holds fewer than three elements, it is physically impossible for it to contain any internal peaks, so we can immediately report 0.

### Step By Step Dry Run

Let us trace through a small example to solidify our understanding. Suppose our array is 4, 1, 4, 2, 1, 5. 
* Index 0 holds 4. Not a peak.
* Index 1 holds 1. Not a peak.
* Index 2 holds 4. It is greater than 1 and greater than 2. This is a peak! Our tracker records a 1 at index 2.
* Index 3 holds 2. Not a peak.
* Index 4 holds 1. Not a peak.
* Index 5 holds 5. Not a peak.

Our Binary Indexed Tree now knows that there is exactly one peak, located at index 2. 

If we receive a query to count peaks between index 0 and index 4, we adjust our boundaries to look strictly inside, meaning we check from index 1 to index 3. Our tree quickly calculates the sum in this range, finds the single peak at index 2, and returns 1. 

If we receive an update to change the value at index 3 from 2 to 6, we update the array. The array is now 4, 1, 4, 6, 1, 5.
We then check the surrounding neighborhood of the update.
* Index 2 used to be a peak. But now its right neighbor is 6. Since 4 is not greater than 6, index 2 is no longer a peak. We tell the tree to remove it.
* Index 3 is the updated element. It holds 6. It is greater than 4 and greater than 1. It is a brand new peak! We tell the tree to add it.
* Index 4 holds 1. Still not a peak.

The logic is remarkably clean and perfectly maintains our counts!

### The C++ Solution

Below is the C++ code that implements all the logic we just discussed. I have written this with clarity in mind so that you can easily follow the structure.

```cpp
#include <vector>

using namespace std;

class Solution {
    struct BinaryIndexedTree {
        int size;
        vector<int> tree;
        
        BinaryIndexedTree(int n) {
            size = n;
            tree.assign(n + 1, 0);
        }
        
        void add(int index, int delta) {
            index = index + 1; 
            while (index <= size) {
                tree[index] = tree[index] + delta;
                index = index + (index & (~index + 1)); 
            }
        }
        
        int query(int index) {
            int sum = 0;
            index = index + 1;
            while (index > 0) {
                sum = sum + tree[index];
                index = index - (index & (~index + 1));
            }
            return sum;
        }
    };

public:
    vector<int> countOfPeaks(vector<int>& nums, vector<vector<int>>& queries) {
        int n = nums.size();
        BinaryIndexedTree bit(n);
        vector<int> peakTracker(n, 0);
        
        auto evaluatePeak = [&](int i) {
            if (i <= 0 || i >= n - 1) {
                return;
            }
            
            bool isPeakNow = (nums[i] > nums[i - 1] && nums[i] > nums[i + 1]);
            int currentPeakStatus = isPeakNow ? 1 : 0;
            
            if (currentPeakStatus != peakTracker[i]) {
                int difference = currentPeakStatus - peakTracker[i];
                bit.add(i, difference);
                peakTracker[i] = currentPeakStatus;
            }
        };

        for (int i = 1; i < n - 1; ++i) {
            evaluatePeak(i);
        }
        
        vector<int> results;
        
        for (const auto& q : queries) {
            if (q[0] == 1) {
                int leftBound = q[1];
                int rightBound = q[2];
                
                if (rightBound - leftBound < 2) {
                    results.push_back(0);
                } else {
                    int totalPeaksToRight = bit.query(rightBound - 1);
                    int totalPeaksToLeft = bit.query(leftBound);
                    results.push_back(totalPeaksToRight - totalPeaksToLeft);
                }
            } else {
                int targetIndex = q[1];
                int newValue = q[2];
                
                nums[targetIndex] = newValue;
                
                evaluatePeak(targetIndex - 1);
                evaluatePeak(targetIndex);
                evaluatePeak(targetIndex + 1);
            }
        }
        
        return results;
    }
};

```

### Understanding the Complexities

It is always important to evaluate the resources our code consumes.

For the space complexity, we are storing a few extra structures. We have our peakTracker array which holds N elements, where N is the length of our input numbers. We also have the internal array of the Binary Indexed Tree, which holds N plus 1 elements. Both of these scale linearly with the size of the input. Therefore, the overall space complexity is O(N). This is quite standard and comfortably fits within standard memory limits.

For the time complexity, we have two distinct phases.
First, we have the initial setup where we loop through the array once to evaluate the initial peaks. For each element, updating the Binary Indexed Tree takes logarithmic time. Building the initial tree thus takes O(N log N) time.
Next, we process the queries. Let us say there are Q queries.
If a query is a counting query, we perform two fetches from the Binary Indexed Tree, which takes O(log N) time.
If a query is an update query, we update the array and reevaluate three adjacent spots. Reevaluating and updating the tree for these three spots takes O(log N) time.
Thus, processing all queries takes O(Q log N) time.
Combining both phases, the total time complexity is O((N + Q) log N). This is incredibly fast and will easily process hundreds of thousands of operations well within the required time limits!

I hope this thorough walkthrough helps you understand not just how to pass the tests, but the deeper mechanics of why this logic works so beautifully. Binary Indexed Trees are a wonderful tool to have in your programming arsenal, and breaking down problems into local neighborhood updates is a strategy you will use time and time again.

Happy coding, and please feel free to ask if anything at all remains unclear!
