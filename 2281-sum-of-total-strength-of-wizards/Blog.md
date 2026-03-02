# Demystifying Network Clusters and Subarray Metrics [#2281]

Hello fellow programming enthusiasts! Today I'm very excited to share a walkthrough of a fascinating algorithm problem that involves network engineers, rows of wireless routers, and evaluating their total network score.

If you have ever felt overwhelmed by problems asking for the sum of all contiguous subarrays, you are definitely not alone. I stumbled upon this challenge recently, and while it seemed incredibly daunting at first glance, breaking it down step by step revealed a beautiful combination of data structures and prefix array techniques. I want to share my understanding with you all in a friendly, modest, and detailed manner. Let us dive into the networking magic!

## Understanding the Problem

Imagine you are a network engineer, and you have a system of routers lined up in a row. Each router has a specific signal capacity, represented by a zero-indexed integer array we will call `strength`.

Your goal is to evaluate the network score of different clusters of routers. However, the clusters cannot be randomly selected. A valid cluster must be a contiguous sequence of routers placed next to each other. In programming terms, this is known as a subarray. A subarray must be continuous and not empty.

For any given contiguous cluster of routers, their combined network score is calculated in a very specific way. You need to find two values:

* **First:** Identify the signal strength of the absolute weakest router (the bottleneck) in that specific cluster.
* **Second:** Calculate the total sum of the individual strengths of all the routers in that cluster.

The total score of that single cluster is the product of those two values. You multiply the minimum strength by the sum of all strengths.

Our mission is to calculate this total network score for every single possible contiguous cluster of routers in the system, and then add all those scores together to return one massive final sum. Because this final number can grow to be astronomically large, we are required to return the answer modulo 1000000007 (which is 10 to the power of 9, plus 7).

### An Illustrative Example

To make sure we are on the same page, let us look at a small example. Suppose our router strengths are represented by the array `[5, 4, 6]`.

Here are all the possible contiguous clusters and their calculated scores:

* Cluster `[5]`: The weakest is 5. The sum is 5. Score is 5 times 5, which equals 25.
* Cluster `[4]`: The weakest is 4. The sum is 4. Score is 4 times 4, which equals 16.
* Cluster `[6]`: The weakest is 6. The sum is 6. Score is 6 times 6, which equals 36.
* Cluster `[5, 4]`: The weakest is 4. The sum is 9. Score is 4 times 9, which equals 36.
* Cluster `[4, 6]`: The weakest is 4. The sum is 10. Score is 4 times 10, which equals 40.
* Cluster `[5, 4, 6]`: The weakest is 4. The sum is 15. Score is 4 times 15, which equals 60.

Adding all these scores together: 25 plus 16 plus 36 plus 36 plus 40 plus 60 gives us a total combined network score of 213.

---

## The Challenge of Scaling

When first approaching this, the most intuitive thought is to use nested loops. We could pick a starting router, pick an ending router, find the weakest one in that range, calculate the sum, and add it to our grand total.

However, if our network has up to 100,000 routers, evaluating every single combination individually would take way too much time. The number of subarrays grows quadratically, and searching within them adds even more operations. Our program would take far too long to finish. We need a different perspective.

Instead of looking at the problem from the perspective of the subarrays, what if we looked at it from the perspective of each individual router?

---

## A Shift in Perspective

Every router in the array will be the weakest router for a certain number of subarrays. If we can figure out exactly which subarrays a particular router is the bottleneck in, we can calculate its contribution to the final answer all at once!

Let us take a router at a specific position, let us call this position `index`. We want this router to be the absolute weakest in our cluster. How far left can our cluster stretch before we encounter a router weaker than our current one? And how far right can we stretch?

We need to find two boundaries for our router at `index`:

* **A left boundary (`LeftBound`):** This is the farthest index to the left where all routers from `LeftBound` to `index` are stronger than or equal to our router.
* **A right boundary (`RightBound`):** This is the farthest index to the right where all routers from `index` to `RightBound` are stronger than or equal to our router.

Any subarray that starts anywhere between `LeftBound` and `index`, and ends anywhere between `index` and `RightBound`, will definitively have our router at `index` as the weakest link.

There is one small detail to watch out for. What if there are two routers with the exact same strength next to each other? If we are not careful, we might count the exact same subarray twice, once for each router. To prevent this duplicate counting, we can make the left side strict. We will say the left boundary stops when it sees a router less than OR equal to our router, while the right boundary only stops when it sees a router strictly less than our router. This ensures each subarray is uniquely assigned to the leftmost minimum router.

### Finding Boundaries with a Monotonic Stack

To find these boundaries efficiently for every router without scanning the whole network over and over, we can use a data structure known as a Monotonic Stack.

A Monotonic Stack is simply a stack that we keep organized. In our case, we want to keep the indices of the routers in the stack such that their strengths are strictly increasing.

To find the left boundaries, we iterate through our network from left to right. Before adding a new router to the stack, we look at the router on top of the stack. If the router on top is stronger than or equal to our current router, we remove it from the stack. We keep doing this until we find a weaker router or the stack is empty. The router we stopped at tells us our left boundary! Then, we push our current router onto the stack.

We do the exact same process for the right boundaries, but we iterate from right to left, and we use a strictly greater comparison to handle those duplicate strengths we talked about earlier.

This technique allows us to find all left and right boundaries for the entire network in a single pass, which is a massive time saver!

---

## The Mathematical Reduction

Now comes the really clever part. For a router at `index`, we know the starting point of valid subarrays can be anywhere from `LeftBound` to `index`. Let us represent a possible starting point with the variable `start`. The ending point can be anywhere from `index` to `RightBound`. Let us represent a possible ending point with the variable `end`.

The score contribution of this router is its strength multiplied by the sum of ALL subarrays bounded by `start` and `end`.

How do we calculate the sum of a subarray quickly? We use a Prefix Sum array! Let us create an array called `PrefixSum` where the value at position `K` is the total sum of all routers from the very beginning up to `K minus 1`.

With this array, the sum of any subarray from `start` to `end` is simply:
`PrefixSum[end plus 1] minus PrefixSum[start]`

We need to add this up for every possible combination of `start` and `end`.

Let us break down the addition.

* For a fixed `start`, we need to add `PrefixSum[end plus 1]` for all possible `end` values from `index` to `RightBound`.
* Then, we subtract `PrefixSum[start]` for all those same `end` values.

Since `PrefixSum[start]` does not change as `end` changes, we are just subtracting `PrefixSum[start]` multiple times. Specifically, we subtract it `RightBound minus index plus 1` times. Let us call this quantity `RightCount`.

Similarly, `PrefixSum[end plus 1]` does not change as `start` changes. We are adding it `index minus LeftBound plus 1` times. Let us call this quantity `LeftCount`.

If we group all the positive terms and negative terms together, the total sum of all valid subarrays for our router becomes:
`LeftCount` multiplied by (the sum of `PrefixSum[end plus 1]` for all `end` from `index` to `RightBound`)
MINUS
`RightCount` multiplied by (the sum of `PrefixSum[start]` for all `start` from `LeftBound` to `index`)

### The Prefix Sum of Prefix Sums

To calculate those sums of `PrefixSum` arrays instantly, we can apply the prefix sum technique a second time! We create a new array called `DoublePrefixSum`.

`DoublePrefixSum[K]` will store the sum of all elements in the `PrefixSum` array from the beginning up to `K minus 1`.

Now, the sum of `PrefixSum[end plus 1]` from `index` to `RightBound` is simply:
`DoublePrefixSum[RightBound plus 2] minus DoublePrefixSum[index plus 1]`

And the sum of `PrefixSum[start]` from `LeftBound` to `index` is simply:
`DoublePrefixSum[index plus 1] minus DoublePrefixSum[LeftBound]`

By using this double prefix sum array, we can find the total sum of thousands of subarrays in a few simple arithmetic operations. We just calculate this total subarray sum, multiply it by the router's strength, and add it to our grand total.

---

## Putting It Into Code

Let us translate these concepts into a clean, functioning C++ solution. I have updated the comments to explain what each section is doing based on our router network discussion above.

```cpp
#include <vector>

using namespace std;

class Solution {
public:
    int totalStrength(vector<int>& strength) {
        long long MOD = 1000000007;
        int n = strength.size();
        
        // Array to store the left boundary for each router
        vector<int> LeftBound(n, 0);
        vector<int> st;
        
        // Finding left boundaries using a monotonic stack
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && strength[st.back()] > strength[i]) {
                st.pop_back();
            }
            LeftBound[i] = st.empty() ? 0 : st.back() + 1;
            st.push_back(i);
        }
        
        // Array to store the right boundary for each router
        vector<int> RightBound(n, n - 1);
        st.clear();
        
        // Finding right boundaries using a monotonic stack
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && strength[st.back()] >= strength[i]) {
                st.pop_back();
            }
            RightBound[i] = st.empty() ? n - 1 : st.back() - 1;
            st.push_back(i);
        }
        
        // First prefix sum array to quickly get subarray sums
        vector<long long> PrefixSum(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            PrefixSum[i + 1] = (PrefixSum[i] + strength[i]) % MOD;
        }
        
        // Second prefix sum array to quickly get sums of PrefixSum elements
        vector<long long> DoublePrefixSum(n + 2, 0);
        for (int i = 0; i <= n; ++i) {
            DoublePrefixSum[i + 1] = (DoublePrefixSum[i] + PrefixSum[i]) % MOD;
        }
        
        long long finalAnswer = 0;
        
        // Iterate through each router to calculate its contribution
        for (int i = 0; i < n; ++i) {
            long long LeftCount = i - LeftBound[i] + 1;
            long long RightCount = RightBound[i] - i + 1;
            
            // Calculate the sum of PrefixSum[end + 1] elements
            long long positiveTermSum = (DoublePrefixSum[RightBound[i] + 2] - DoublePrefixSum[i + 1] + MOD) % MOD;
            positiveTermSum = (positiveTermSum * LeftCount) % MOD;
            
            // Calculate the sum of PrefixSum[start] elements
            long long negativeTermSum = (DoublePrefixSum[i + 1] - DoublePrefixSum[LeftBound[i]] + MOD) % MOD;
            negativeTermSum = (negativeTermSum * RightCount) % MOD;
            
            // The total sum of all valid subarrays where the router at index i is the bottleneck
            long long totalSubarraySum = (positiveTermSum - negativeTermSum + MOD) % MOD;
            
            // Multiply the sum by the router's actual signal strength
            long long routerContribution = (totalSubarraySum * strength[i]) % MOD;
            
            // Add to our grand total network score
            finalAnswer = (finalAnswer + routerContribution) % MOD;
        }
        
        return finalAnswer;
    }
};

```

---

## Complexity Analysis

It is always good practice to evaluate how much resources our solution will consume.

### Time Complexity

We iterate through our router network a few times: once to find the left boundaries, once to find the right boundaries, once to build the first prefix sum, once to build the second prefix sum, and finally one last time to calculate the answer.

During the boundary finding loops, every router index is pushed onto the stack exactly once and popped from the stack at most once. This means the stack operations take linear time overall.

Because we only do a constant number of linear passes, our overall time complexity is linearly proportional to the size of the input array. In computer science terms, we write this as Big O of N, where N is the number of routers. This is incredibly fast and easily passes the constraints.

### Space Complexity

We created several arrays to store intermediate information. We have the `LeftBound` array, the `RightBound` array, the stack `st`, the `PrefixSum` array, and the `DoublePrefixSum` array. All of these structures grow linearly with the number of routers.

Therefore, our space complexity is also linearly proportional to the size of the input, which is Big O of N. Trading a bit of memory for a massive speed increase is a very practical approach in this scenario.

---

## Wrapping Up

This problem is a wonderful demonstration of how shifting your perspective can lead to breakthroughs. By focusing on the minimum element rather than the subarrays themselves, we transformed a potentially incredibly slow loop into a swift, elegant sequence of linear operations. Combining monotonic stacks with multiple layers of prefix sums might seem intimidating, but when you break down the math logically, it clicks into place.

I hope this detailed explanation helps demystify the logic behind this problem. Feel free to run the code, experiment with different inputs, and print out the arrays to see how the numbers change. Happy coding, and may your algorithms always run efficiently!
