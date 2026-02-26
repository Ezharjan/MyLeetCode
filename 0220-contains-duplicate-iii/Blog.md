# Mastering the Sliding Window and Absolute Difference Puzzle in C++ [#220]

Hello fellow developers and welcome to my programming blog! Today I am very excited to share my thoughts and a detailed walkthrough on a fascinating coding puzzle. We are going to explore how to find almost duplicate elements within a restricted window of indices. Learning together makes the coding community wonderful, and I hope this article provides you with deep insights into data structures and algorithms. Grab a cup of coffee, make yourself comfortable, and let us explore this together!

## Understanding the Core Problem

Let us start by clearly defining what we are trying to solve. We are given a list of numbers, specifically an array of integers. Along with this array, we are handed two very important rules governed by two integer limits. Our mission is to figure out if there are two distinct positions in this array that satisfy both rules at the exact same time.

Let us call the first position index I and the second position index J.

The first rule states that the distance between index I and index J cannot be greater than a provided limit known as the index difference. If the limit is three, the two numbers can be right next to each other, one space apart, two spaces apart, or exactly three spaces apart. They cannot be four or more spaces apart. 

The second rule concerns the actual numerical values sitting at those positions. The absolute difference between the value at index I and the value at index J cannot exceed another provided limit known as the value difference. Absolute difference simply means we measure the gap between the two numbers regardless of which one is larger. 

If we find any pair of indices that passes both of these checks, our program should return true. If we scan through the entire array and exhaust all possibilities without finding a valid pair, we must return false.

At a quick glance, this sounds quite manageable. However, the constraints tell us that the array can contain up to one hundred thousand elements, and the numbers themselves can be as large as one billion or as small as negative one billion. These massive numbers mean we have to be extremely careful about how we design our solution.

## The Brute Force Idea and Its Limitations

When faced with a problem like this, my immediate instinct is often to try the most direct approach. The most straightforward method is to use two nested loops to compare every possible pair of numbers in the array.

The outer loop would pick our starting number. The inner loop would start immediately after the outer loop and look at the numbers ahead, but it would stop once it reaches the index difference limit. Inside this inner loop, we would calculate the difference between the two numbers. If the difference is within the allowed value limit, we return true.

This logic is perfectly sound and very easy to write. Unfortunately, we run into a severe scaling issue. Imagine an array with one hundred thousand elements and an index difference limit that is just as large. For every single element, we might have to check thousands of other elements. This results in billions of operations. In modern computing, this will trigger a Time Limit Exceeded error. 

The time complexity for this basic approach is Big O of N times K, where N is the total number of elements and K is the allowed index difference. We definitely need a way to minimize the redundant checks and only perform the operations that matter.

## The Sliding Window Concept

To improve our strategy, we need to utilize a concept known as the sliding window. As we iterate through our list of numbers, we only truly care about the most recent numbers we have seen. Specifically, we only care about the elements that fall within our index difference limit. 

Imagine placing a physical rectangular frame over a list of numbers written on a piece of paper. The frame is exactly wide enough to show a certain amount of numbers. As you read the list from left to right, you slide the frame along. Numbers enter the frame from the right side, and older numbers drop out of view from the left side. 

By applying this concept to our code, we can maintain a dynamic collection of the recent numbers. When a new number arrives, we check if it forms a valid pair with any number currently inside our collection. After checking, we add the new number to the collection. If our collection grows larger than the allowed index difference, we simply discard the oldest number.

Now the big question becomes how we structure this collection. We need a way to quickly find out if there is a number inside the collection that is close enough in value to our newly arrived number.

## Method One Using a Binary Search Tree

If we keep the numbers in our sliding window sorted, searching through them becomes significantly faster. In C++, the standard set library is implemented internally as a self balancing binary search tree. This means it automatically keeps its elements ordered and allows us to insert new elements, delete old elements, and search for specific values very rapidly.

For every new number we encounter, we want to know if there is already a number in our standard set that is mathematically close to it. We need to find a number in the set that is greater than or equal to our current number minus the value limit.

C++ provides a wonderful function for standard sets called lower bound. This function quickly finds the smallest number in the set that is greater than or equal to a target value. 

Let us trace the logic step by step. We take our current number and subtract the value difference limit. We use this result as our target and ask the standard set to find the lower bound. If the set finds a number, we must perform one more verification. We check if this found number is also less than or equal to our current number plus the value difference limit. 

If this second check passes, it means the number from the set is trapped perfectly within the allowed range, and we have successfully found our pair!

Let us look at how this is written in C++ code. Please note the use of long long data types. Because the numbers can be incredibly large, adding or subtracting them might exceed the maximum limit of standard integers. Using long long prevents these overflow errors.

```cpp
#include <vector>
#include <set>
#include <cmath>

using namespace std;

class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        set<long long> slidingWindow;
        
        for (int i = 0; i < nums.size(); ++i) {
            long long currentNumber = nums[i];
            long long targetLowerLimit = currentNumber - valueDiff;
            
            auto iterator = slidingWindow.lower_bound(targetLowerLimit);
            
            if (iterator != slidingWindow.end() && *iterator <= currentNumber + valueDiff) {
                return true;
            }
            
            slidingWindow.insert(currentNumber);
            
            if (i >= indexDiff) {
                long long oldestNumber = nums[i - indexDiff];
                slidingWindow.erase(oldestNumber);
            }
        }
        
        return false;
    }
};

```

This method is highly reliable and very clean to read. Because standard set operations take logarithmic time, the overall time performance is Big O of N times the logarithm of K. The space requirement is Big O of K, as we store at most K elements at any given time.

## Method Two The Bucket Grouping Strategy

While the binary search tree approach is wonderful, I want to share another perspective that is even faster. We can solve this by treating it as a grouping problem, much like sorting objects into labeled buckets.

Imagine we set the width of our buckets to be exactly the value difference limit plus one. The reason we add one is to properly account for a difference of zero. For instance, if the allowed value difference is three, a bucket width of four ensures that numbers zero, one, two, and three will all fall into bucket zero. The maximum possible difference between any two numbers in bucket zero is precisely three, which satisfies our problem condition perfectly.

As we traverse the array, we calculate which bucket the current number belongs to. If we look inside that bucket and discover that it already contains a number, we immediately know we have found a valid pair! Two numbers in the same bucket are mathematically guaranteed to have a difference less than or equal to the allowed limit.

However, there is a small nuance. What if the valid pair of numbers happen to fall into two neighboring buckets? Imagine the allowed difference is three. The number three goes to bucket zero, and the number four goes to bucket one. They are in separate buckets, but their absolute difference is only one, which means they are a valid pair.

To handle this scenario, whenever we place a number in a bucket, we also inspect the bucket directly below it and the bucket directly above it. If we find a number in either of those adjacent buckets, we manually calculate the absolute difference between the two numbers to verify if they are within the acceptable range.

We still maintain a sliding window, but instead of a standard set, we use an unordered map to store our buckets. If our window size exceeds the index difference limit, we simply remove the bucket entry corresponding to the oldest number.

## Handling Negative Values in Buckets

A very important detail with the bucket strategy is dealing with negative numbers correctly. In C++, when you divide a negative integer by a positive integer, the result truncates towards zero. This means both positive one and negative one divided by a width of four would evaluate to bucket zero.

We absolutely do not want this behavior because it would merge negative numbers into the positive zero bucket, making the bucket far too wide. To resolve this, we need to mathematically shift negative numbers down before performing the division. This ensures they map to separate and correctly sized negative buckets.

Let us look at the C++ code for the bucket strategy.

```cpp
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

class Solution {
private:
    long long calculateBucketId(long long number, long long bucketWidth) {
        if (number >= 0) {
            return number / bucketWidth;
        }
        return (number + 1) / bucketWidth - 1;
    }

public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        unordered_map<long long, long long> buckets;
        long long bucketWidth = (long long)valueDiff + 1;
        
        for (int i = 0; i < nums.size(); ++i) {
            long long currentNumber = nums[i];
            long long bucketId = calculateBucketId(currentNumber, bucketWidth);
            
            if (buckets.count(bucketId)) {
                return true;
            }
            
            if (buckets.count(bucketId - 1) && abs(currentNumber - buckets[bucketId - 1]) <= valueDiff) {
                return true;
            }
            
            if (buckets.count(bucketId + 1) && abs(currentNumber - buckets[bucketId + 1]) <= valueDiff) {
                return true;
            }
            
            buckets[bucketId] = currentNumber;
            
            if (i >= indexDiff) {
                long long oldestNumber = nums[i - indexDiff];
                long long oldestBucketId = calculateBucketId(oldestNumber, bucketWidth);
                buckets.erase(oldestBucketId);
            }
        }
        
        return false;
    }
};

```

## Performance Comparison and Final Words

The bucket method is incredibly capable. By utilizing a hash map for our buckets, we can insert items, delete items, and check for the existence of buckets in roughly constant time. This reduces our overall time complexity down to a linear scale, strictly Big O of N. Similar to the first method, the space requirement is Big O of K because our hash map will never hold more elements than our allowed index difference limit.

Both strategies are practical and provide excellent learning opportunities. Understanding when to leverage a balanced binary search tree versus when to engineer a custom hashing mechanism is a highly valuable skill in software development.

I sincerely hope this deep exploration of the sliding window and absolute difference puzzle helps you understand the underlying concepts clearly. Exploring algorithms step by step is a rewarding experience, and I encourage you to type out these solutions and trace them with your own test cases.

Thank you so much for reading through this detailed guide! I love sharing knowledge with the community. Please feel free to test these ideas out in your own projects. Happy coding everyone!
