# Unveiling the Magic of Sliding Windows to Count Subarrays with Exact Distinct Elements and Frequency Constraints [#3859]

Hello everyone! 

Welcome back to another deep dive into the wonderful world of algorithms and data structures. Today, I am super excited to share a comprehensive guide on solving a truly fascinating array challenge. Whether you are preparing for technical interviews or simply looking to sharpen your problem solving skills, this concept is incredibly rewarding to master. 

Array problems often ask us to find segments that match specific rules. Sometimes, the rules are simple. Other times, like in our topic today, the rules are layered and require a clever combination of techniques. We are going to break down this complex problem into small, digestible pieces so that everyone, regardless of their current skill level, can fully understand the logic and apply it to similar future challenges. 

Please grab your favorite beverage, get comfortable, and let us explore this amazing sliding window approach together!

***

### Understanding the Core Problem

Imagine you are given a long list of numbers. Your mission is to look at contiguous segments within this list. A contiguous segment, often called a subarray, is simply a slice of the array where all the elements are sitting right next to each other. You cannot skip elements or pick and choose from different parts of the list; the slice must be unbroken.



For this specific challenge, we need to find all the valid unbroken slices that satisfy two very strict conditions simultaneously. 

First, the slice must contain exactly a specific number of distinct integers. Let us refer to this target number of distinct integers as 'k'. If 'k' is two, then our slice must have exactly two different numbers in it. If it has only one unique number, it is invalid. If it has three unique numbers, it is also invalid.

Second, within that exact same slice, every single one of those distinct integers must appear at least a certain number of times. Let us call this minimum required frequency 'm'. If 'm' is two, then every unique number we found for our first condition must show up two or more times inside the slice. 

To visualize this, let us say our target unique count 'k' is two, and our minimum frequency 'm' is two. If we look at a slice like [1, 2, 1, 2], we can see it contains exactly two distinct numbers, which are 1 and 2. Now we check the frequencies. The number 1 appears twice. The number 2 appears twice. Since both unique numbers appear at least twice, this slice is a perfect match!

However, if we look at a slice like [1, 2, 1, 3], we have three distinct numbers. This immediately violates our first rule. What if the slice is [1, 2, 1]? Here we have two distinct numbers, which passes the first rule. But the number 2 only appears once. Since our target frequency 'm' is two, this slice fails the second rule.

Our ultimate goal is to write a program that can quickly and accurately count every single valid slice in the entire list.

***

### Why the Obvious Approach is Not Enough

When faced with array problems, our first instinct is often to try all possible combinations. We could use a starting point and an ending point, loop through every single possible slice, count the distinct numbers, and check their frequencies. 

While this thought process is completely logical and a great starting point, it runs into severe performance issues when the list of numbers gets very large. If the list has one hundred thousand elements, calculating every possible slice involves billions of operations. Checking the frequencies inside each of those slices adds even more work. Most coding platforms will stop your program and give you a Time Limit Exceeded error because it takes far too long.

We need a strategy that allows us to find the answer by traveling through the list ideally just once, or at most a few times. We need a linear time solution.

***

### Enter the Sliding Window Strategy

This is where the sliding window technique shines. Instead of starting from scratch for every single slice, a sliding window allows us to maintain a dynamic subset of the array. We use two markers, a left marker and a right marker. The elements between these two markers form our window.



We gradually expand the window by moving the right marker forward, adding new elements one by one. As we add elements, we update our count of distinct numbers and their frequencies. If our window suddenly violates a rule, we shrink it by moving the left marker forward, removing elements until the window becomes valid again. 

This expanding and shrinking motion resembles a caterpillar crawling along the array. Because both the left and right markers only move forward, we process each element a very limited number of times, making the algorithm incredibly fast.

***

### The Brilliant Two Window Concept

Our specific problem is quite tricky because we have an exact equality condition mixed with a minimum frequency condition. We need exactly 'k' distinct elements. Exact equality conditions are notoriously difficult for a standard single sliding window because the window does not know if it should shrink or stay when it hits the exact target. 

To solve this elegantly, we will maintain two separate sliding windows at the same time! Both windows will share the exact same right marker, but they will have their own independent left markers.

Let us define the roles of these two windows.

The First Window:
This window will focus on the maximum boundary. Its job is to ensure that the segment ending at the right marker contains at most 'k' distinct integers. We will call its starting boundary 'left1'. As the right marker moves forward, if the number of distinct integers inside the first window exceeds 'k', we will move 'left1' forward until the distinct count drops back down to 'k'. This 'left1' represents the furthest left we can possibly go without exceeding our distinct element limit.

The Second Window:
This window will focus on the minimum boundary regarding our frequency condition. Its job is to find the point where we stop having 'k' distinct integers that meet the minimum frequency 'm'. We will call its starting boundary 'left2'. As the right marker moves, we track how many distinct numbers appear at least 'm' times. If we have 'k' or more such numbers, we move 'left2' forward. We keep moving 'left2' until our count of numbers meeting the frequency requirement drops strictly below 'k'. 

The Intersection of Logic:
Now, pause and think about what 'left1' and 'left2' represent for a fixed right ending position. 
Any starting index from 'left1' up to 'left2' minus one will guarantee two things simultaneously. 
First, because the starting index is greater than or equal to 'left1', the slice has at most 'k' distinct elements. 
Second, because the starting index is strictly less than 'left2', the slice inherently contains 'k' distinct elements that meet the frequency requirement. 

If an array segment has at most 'k' distinct elements in total, and it also contains 'k' elements that appear at least 'm' times, the only mathematical possibility is that the total number of distinct elements is exactly 'k', and all of them meet the frequency rule! 

Therefore, for every step the right marker takes, the number of valid slices ending at that exact spot is simply the distance between 'left2' and 'left1'. We calculate this by subtracting 'left1' from 'left2'. If 'left2' is greater than 'left1', we add the difference to our total answer.

***

### Step by Step Walkthrough

Let us trace this logic with a concrete example to make it crystal clear. 

Suppose our array of numbers is [1, 2, 1, 2, 2]. 
Our target distinct count 'k' is 2. 
Our target minimum frequency 'm' is 2.

We set up our pointers. 
Right marker starts at 0. 
'left1' starts at 0. 
'left2' starts at 0. 
We need frequency trackers for both windows. 

Right moves to index 0, pointing to the number 1.
Window 1 sees the number 1. Distinct count is 1. This is less than or equal to 'k', so 'left1' stays at 0.
Window 2 sees the number 1. Its frequency is 1. We have zero numbers meeting the frequency 'm'. 'left2' stays at 0.
Valid slices ending here: 'left2' minus 'left1' is zero. Total answer is 0.

Right moves to index 1, pointing to the number 2.
Window 1 sees the number 2. Distinct count is 2. This is less than or equal to 'k', so 'left1' stays at 0.
Window 2 sees the number 2. Its frequency is 1. We still have zero numbers meeting the frequency 'm'. 'left2' stays at 0.
Valid slices ending here: 'left2' minus 'left1' is zero. Total answer is 0.

Right moves to index 2, pointing to another number 1.
Window 1 sees another 1. Distinct count remains 2. 'left1' stays at 0.
Window 2 sees another 1. The frequency of 1 becomes 2! We now have one number meeting the frequency 'm'. 'left2' stays at 0 because we need 'k' numbers (which is 2) meeting the frequency.
Valid slices ending here: 'left2' minus 'left1' is zero. Total answer is 0.

Right moves to index 3, pointing to another number 2.
Window 1 sees another 2. Distinct count remains 2. 'left1' stays at 0.
Window 2 sees another 2. The frequency of 2 becomes 2! We now have two distinct numbers meeting the frequency 'm'. This hits our target of 'k'.
Because our condition for moving 'left2' is when the valid frequency count is greater than or equal to 'k', we must shrink Window 2.
We remove the element at 'left2' (which is the first number 1 at index 0) from Window 2's tracker. The frequency of 1 drops to 1. 
Now, our count of valid frequent numbers drops to 1 (only the number 2 meets the rule). The condition fails, so we stop shrinking. 'left2' is now at index 1.
Valid slices ending here: 'left2' minus 'left1' is 1 minus 0, which is 1. We found our first valid slice! It starts at 0 and ends at 3. The slice is [1, 2, 1, 2]. Total answer is 1.

Right moves to index 4, pointing to the last number 2.
Window 1 sees another 2. Distinct count remains 2. 'left1' stays at 0.
Window 2 sees another 2. The frequency of 2 becomes 3. We still have one number meeting the frequency 'm'. 'left2' stays at 1.
Valid slices ending here: 'left2' minus 'left1' is 1 minus 0, which is 1. We found another valid slice! It starts at 0 and ends at 4. The slice is [1, 2, 1, 2, 2]. Total answer becomes 2.

We have reached the end of the array. The final output is 2. This matches the correct expectation perfectly!

***

### The C++ Implementation

Now that we have a solid grasp of the mechanics, let us translate our dual window logic into clean and efficient C++ code. 

To make our program as fast as possible, instead of using hash maps which have some overhead, we can use simple arrays or vectors to track the frequencies. We find the maximum value in the input array first, and create frequency vectors of that size. This guarantees instant index lookups.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long countSubarrays(vector<int>& nums, int k, int m) {
        int n = nums.size();
        
        // Step 1: Find the maximum element to allocate perfectly sized frequency arrays
        int max_val = *max_element(nums.begin(), nums.end());
        vector<int> freq1(max_val + 1, 0);
        vector<int> freq2(max_val + 1, 0);
        
        long long total_valid_subarrays = 0;
        int left1 = 0;
        int left2 = 0;
        int distinct_count1 = 0;
        int frequent_count2 = 0;
        
        // Step 2: Iterate through the array with the right marker
        for (int R = 0; R < n; ++R) {
            
            // Step 3: Update the first window
            // This window tracks elements to ensure we have at most 'k' distinct integers
            freq1[nums[R]]++;
            if (freq1[nums[R]] == 1) {
                distinct_count1++;
            }
            
            // If we exceed 'k' distinct integers, shrink the first window from the left
            while (distinct_count1 > k) {
                freq1[nums[left1]]--;
                if (freq1[nums[left1]] == 0) {
                    distinct_count1--;
                }
                left1++;
            }
            
            // Step 4: Update the second window
            // This window tracks distinct integers that appear at least 'm' times
            freq2[nums[R]]++;
            if (freq2[nums[R]] == m) {
                frequent_count2++;
            }
            
            // We push the left2 boundary as far right as possible 
            // while we still have 'k' elements appearing at least 'm' times
            while (frequent_count2 >= k) {
                freq2[nums[left2]]--;
                if (freq2[nums[left2]] == m - 1) {
                    frequent_count2--;
                }
                left2++;
            }
            
            // Step 5: Calculate valid subarrays ending at current right marker
            // Any starting index from left1 up to left2 minus 1 is a valid subarray
            if (left2 > left1) {
                total_valid_subarrays += (left2 - left1);
            }
        }
        
        return total_valid_subarrays;
    }
};

```

---

### Analyzing Efficiency

It is always important to understand the performance profile of our solutions.

For the time complexity, we iterate through the given array with the right marker exactly once. The left markers, 'left1' and 'left2', also travel from the beginning of the array towards the end. They never move backwards. In the absolute worst case scenario, every element is visited by the right marker once, and by the left markers once. This means the total number of operations scales linearly with the size of the input array. We denote this linear relationship as Big O of N time complexity, where N is the number of elements in the array. This is incredibly efficient and easily passes all strict timing tests.

For the space complexity, we decided to use vectors to store the frequency of the numbers instead of hash maps. The size of these vectors is determined by the maximum integer value present in the input array. Therefore, the memory footprint scales with this maximum value. We denote this as Big O of Maximum Element space complexity. Given typical problem constraints where values might go up to one hundred thousand, allocating an array of that size takes a negligible amount of memory and provides the benefit of extremely fast lookups.

---

### Final Thoughts and Encouragement

I truly hope this detailed breakdown demystifies the dual sliding window technique for you. Problems involving exact constraints often seem daunting at first glance. However, by breaking the exact requirement into a combination of a maximum boundary and a minimum boundary, we unlock a powerful way to leverage the sliding window strategy.

Programming is a journey of continuous learning and sharing. Do not be discouraged if a concept takes a few reads to fully click. Try writing down the array on a piece of paper and physically moving your fingers as the pointers, just like we did in the walkthrough. That physical mapping often builds incredible intuition.

If you have any questions, alternative approaches, or simply want to share your success running the code, please feel free to contribute to the community discussion. 

Happy coding, and keep exploring!
