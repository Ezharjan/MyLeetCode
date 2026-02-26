# Maximizing Distance on a Square Boundary A Fun Geometry and Binary Search Walkthrough [#3464]

Hello everyone! Welcome to my programming blog. Today I am incredibly excited to share a very interesting algorithmic challenge with you all. I recently spent some time working on a fascinating geometry and array traversal problem, and I thought it would be wonderful to share my findings and the step by step thought process with the community. 

Sometimes, a problem looks incredibly intimidating because it mixes two dimensional geometry with finding minimums and maximums. However, once we look at it from a different perspective, all the pieces fall together perfectly. My goal today is to walk you through this exact process. I want to make sure everyone, regardless of their current experience level, can understand the concepts, visualize the geometry, and write a solid working solution. Let us dive right in!



### Understanding the Core Problem

Let us start by clearly defining what we are trying to achieve. 

Imagine you have a large square drawn on a piece of paper. The bottom left corner of this square is positioned exactly at the origin of a graph, which is the coordinate (0, 0). The square has a specific edge length, which we will call `side`. This means the four corners of your square are located at the coordinates (0, 0), (0, side), (side, side), and (side, 0).

You are given a list of points. A crucial guarantee in this problem is that every single point in this list lies exactly on the boundary of the square. They are not floating inside the square, and they are not floating outside. They are strictly on the perimeter line. 

You are also given an integer `k`. Your mission is to carefully pick exactly `k` points from your list. 

Now, how do we decide which points to pick? We want to spread these `k` points out as much as possible along the boundary. Specifically, we want to look at the Manhattan distance between any two of our chosen points. The Manhattan distance between two points is simply the absolute difference of their x coordinates plus the absolute difference of their y coordinates. Think of it like walking along city blocks; you can only go left, right, up, or down. 

If we pick `k` points, there will be a minimum Manhattan distance among all pairs of the points we selected. We want to make this minimum distance as large as possible. In other words, we want to maximize the minimum gap between our selected points.

### The First Big Idea Flattening the Geometry

Working with points on a two dimensional square can be quite messy. You have to constantly check which edge a point is on, handle corners, and calculate distances across the open space. 

But here is a beautiful realization the boundary of a square is just a continuous loop. What if we could cut this loop at the origin point (0, 0) and unroll it into a single straight line? 

By doing this, we transform our two dimensional geometry problem into a much simpler one dimensional array problem. The entire perimeter of the square has a total length of `4 * side`. We can map every single coordinate (x, y) on the square to a single number representing its distance from the origin if we walk counter clockwise along the edge.

Let us define how we map these points
* If a point is on the bottom edge, its y coordinate is 0. Its distance from the start is simply its x coordinate.
* If a point is on the right edge, its x coordinate is equal to `side`. To reach it, we first walk the entire bottom edge (length `side`), and then walk up by its y coordinate. So its 1D position is `side + y`.
* If a point is on the top edge, its y coordinate is equal to `side`. We have walked the bottom edge and the right edge (total distance `2 * side`). Now we are walking backward along the top edge, so the distance is `3 * side minus x`.
* If a point is on the left edge, its x coordinate is 0. We have walked three full edges (total distance `3 * side`). Now we are walking down the left edge back to the origin, so the position is `4 * side minus y`.

By applying this simple mapping logic to every point in our input list, we completely eliminate the x and y coordinates. We now just have a simple list of numbers representing positions on a very long line. We can then sort this list to put the points in their natural consecutive order.

### The Second Big Idea The Distance Miracle

You might be wondering a very important question. The problem explicitly asks us to use the Manhattan distance. But if we unroll the square into a straight line, we are calculating the distance along the perimeter. Are these two distances actually the same?

The short answer is no, not always. If you pick a point on the bottom edge and a point on the top edge, their Manhattan distance is just `side` (going straight up through the middle of the square), but their perimeter distance would be much longer (going all the way around the corners). 

However, we have a massive advantage hiding in the problem constraints. We are asked to pick `k` points, and the problem guarantees that `k` will always be greater than or equal to 4. 

Let us think about the implications of this. If we place 4 points on the perimeter of a square and we want to spread them out as equally as possible, the maximum distance we can put between them is exactly the length of one side. If we try to push any two points further apart than `side`, we will inevitably push other points closer together, making the minimum gap smaller. Because `k` is at least 4, the final answer the maximum possible minimum gap can never strictly exceed `side`.

Now comes the beautiful mathematical miracle. For any two points on the boundary of a square, if their distance strictly along the perimeter is less than or equal to `side`, then their perimeter distance perfectly equals their Manhattan distance! 

Why is this true? Because a perimeter distance less than or equal to `side` means the two points must either be on the same exact edge, or they must be on two edges that touch at a corner. 
* If they are on the same edge, walking between them is a straight line, which is identical to the Manhattan path.
* If they are on adjacent edges, the perimeter path goes to the corner and then to the other point. The Manhattan path takes the exact same horizontal and vertical steps, just perhaps in a slightly different order, resulting in the exact same total sum.

This means we can completely forget about the Manhattan distance! Because our target answer will always be less than or equal to `side`, we can purely use the 1D perimeter distance for all our calculations. This transforms a complex 2D problem into a straightforward 1D spacing problem.

### The Strategy Guessing the Answer with Binary Search



Now that we have a sorted list of 1D positions, how do we find the maximum possible minimum gap? 

Instead of trying to place the points mathematically to calculate the gap, we can work backward. We can guess a gap distance, and then check if it is possible to pick `k` points from our list such that every chosen point is at least that guess distance apart.

What are the bounds of our guesses?
* The smallest possible gap is 1.
* The largest possible gap, as we discussed earlier, is the total perimeter divided by `k`.

Because the possibility of placing `k` points is monotonic meaning if we can successfully place `k` points with a gap of 10, we can definitely place them with a gap of 9, 8, or 1; and if we cannot place them with a gap of 15, we definitely cannot place them with a gap of 16 we can use the binary search algorithm to quickly hone in on the exact maximum gap.

We will find the middle point between our lowest possible guess and our highest possible guess. We will check if this middle gap is valid. If it is valid, we save it as our best answer so far and try to guess a larger gap. If it is invalid, we know we guessed too high, so we adjust our search space to smaller gaps.

### The Strategy Validating our Guess



The most critical part of our binary search is the validation step. Given a guessed target gap `D`, how do we efficiently check if we can select `k` points that are all at least `D` distance apart?

We can use a greedy approach. If we decide to start picking points starting from a specific point in our array, we should always pick the very next available point that is at least `D` distance away. Skipping valid points would only cramp the remaining space, so picking the earliest valid point is always a solid strategy.

However, we have one final complication. Our points are not exactly on a straight line segment; they are on a circular loop. When we unrolled the square, we created a start and an end. But a sequence of `k` points could start near the end of the unrolled line and wrap completely around to the beginning. 

To beautifully handle this circular wrap around without complex modulo math, we can simply duplicate our 1D array. We take all our mapped points, add the total perimeter length to them, and append them to the end of our list. This simulates exactly one full wrap around. If our original list has `N` points, our new working list will have `2 * N` points.

Now, for any point `i` from 0 up to `N minus 1`, we can try using it as the first point of our `k` points. From this starting point, we greedily jump to the next point that is at least `D` away. We do this jump `k minus 1` times. 

To make these jumps extremely fast, we can use a two pointer technique beforehand. We can create an array called `next_valid_point`. For every point in our duplicated array, we use a second pointer to scan forward and record the index of the first point that satisfies the `D` distance requirement. Precalculating this means each of our jumps during the checking phase takes only a single step of time.

If we successfully make `k minus 1` jumps, we must do one final check. Because the boundary is a circle, the very last point we picked must also be at least `D` distance away from the very first point we picked. Since our first point was `Start`, and its equivalent wrapped around position is `Start + perimeter`, we just ensure our final point's position is less than or equal to `Start + perimeter minus D`. 

If we find even one starting point that allows us to successfully place all `k` points with the valid wrap around distance, then our guessed gap `D` is entirely achievable!

### Putting it all together The C++ Code

Let us translate all these wonderful concepts into clean, readable C++ code. I have added detailed comments to guide you through every block of logic.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    // This helper function flattens the 2D perimeter into a 1D line.
    long long mapPointTo1D(int x, int y, long long side) {
        if (y == 0) {
            return (long long)x;
        }
        if (x == side) {
            return side + (long long)y;
        }
        if (y == side) {
            return 3LL * side - (long long)x;
        }
        if (x == 0) {
            return 4LL * side - (long long)y;
        }
        return 0;
    }

    // This function greedily checks if a target distance D is achievable.
    bool isDistancePossible(long long targetGap, const vector<long long>& positions, int N, int k, long long perimeter) {
        int totalPoints = 2 * N;
        vector<int> nextPoint(totalPoints);
        int rightPointer = 0;
        
        // Precalculate the next valid jump for every point using the two pointer technique.
        for (int leftPointer = 0; leftPointer < totalPoints; ++leftPointer) {
            while (rightPointer < totalPoints && positions[rightPointer] - positions[leftPointer] < targetGap) {
                rightPointer++;
            }
            nextPoint[leftPointer] = rightPointer;
        }
        
        // Try starting our k points sequence from each original point.
        for (int startIndex = 0; startIndex < N; ++startIndex) {
            int currentIndex = startIndex;
            int jumpsMade = 0;
            
            // Greedily jump to the next valid point k minus 1 times.
            for (; jumpsMade < k - 1; ++jumpsMade) {
                currentIndex = nextPoint[currentIndex];
                if (currentIndex >= totalPoints) {
                    break; // We ran out of points before finishing our jumps.
                }
            }
            
            // If we made all necessary jumps, verify the final circular wrap around gap.
            if (jumpsMade == k - 1 && currentIndex < totalPoints) {
                if (positions[currentIndex] <= positions[startIndex] + perimeter - targetGap) {
                    return true; // We found a valid placement for this gap!
                }
            }
        }
        return false; // No starting point could satisfy this gap.
    }

    int maxDistance(int side, vector<vector<int>>& points, int k) {
        int N = points.size();
        vector<long long> mappedPositions(N);
        
        // Step 1 Flatten all points to 1D coordinates.
        for (int i = 0; i < N; ++i) {
            mappedPositions[i] = mapPointTo1D(points[i][0], points[i][1], side);
        }
        
        // Step 2 Sort the points so they follow the perimeter sequentially.
        sort(mappedPositions.begin(), mappedPositions.end());
        
        // Step 3 Duplicate the array to handle circular wrap around logic seamlessly.
        vector<long long> circularPositions(2 * N);
        long long totalPerimeter = 4LL * side;
        
        for (int i = 0; i < N; ++i) {
            circularPositions[i] = mappedPositions[i];
            circularPositions[i + N] = mappedPositions[i] + totalPerimeter;
        }
        
        // Step 4 Binary search the answer.
        long long searchLow = 1;
        long long searchHigh = totalPerimeter / k;
        long long bestGapFound = 1;
        
        while (searchLow <= searchHigh) {
            long long midGap = searchLow + (searchHigh - searchLow) / 2;
            
            if (isDistancePossible(midGap, circularPositions, N, k, totalPerimeter)) {
                bestGapFound = midGap;       // This gap works, save it!
                searchLow = midGap + 1;      // Let us be ambitious and try a larger gap.
            } else {
                searchHigh = midGap - 1;     // This gap failed, we must try a smaller gap.
            }
        }
        
        return static_cast<int>(bestGapFound);
    }
};

```

### Complexity Analysis

It is always important to understand the performance of our logic. Let us break down the time and space complexity.

**Time Complexity**
First, mapping the points takes an amount of time proportional to the number of points, which is `O(N)`. Sorting the mapped array takes `O(N log N)` time.

Then comes the binary search. Our search range starts at 1 and goes up to `4 * side / k`. The number of times the binary search loop runs is proportional to the logarithm of that maximum range, which we can write as `O(log(side))`.

Inside each step of the binary search, we call our validation function. The validation function first runs a two pointer scan over the array of size `2 * N`. Because both pointers only move forward, this scan takes `O(N)` time. After the scan, we try `N` different starting positions, and for each starting position, we follow the precalculated jumps up to `k` times. This jump phase takes `O(N * k)` time. Therefore, the entire validation function takes `O(N * k)` time.

Multiplying the binary search steps by the work done inside them, the total time for the search phase is `O(N * k * log(side))`.

Adding the sorting phase, our final overall time complexity is `O(N log N + N * k * log(side))`. Given the constraints of the problem where `N` is up to 15000 and `k` is up to 25, this logic will run incredibly fast and comfortably within any time limits.

**Space Complexity**
We create a mapped array of size `N`, a duplicated circular array of size `2 * N`, and a jump array inside the validation function also of size `2 * N`. All of these memory allocations scale linearly with the input size. Therefore, the total space complexity is perfectly bounded at `O(N)`.

### Final Thoughts

This problem is a magnificent example of how mathematical properties can dramatically simplify an algorithmic challenge. By recognizing the equivalence of Manhattan distance and perimeter distance under our specific constraints, we avoided highly complex 2D geometric intersection calculations. We reduced a daunting square coordinate problem into a very friendly array search.

I sincerely hope this detailed breakdown helped illuminate the concepts for you! Learning to shift perspectives to transform a problem into a simpler domain is a incredibly valuable skill in software engineering and competitive programming. Keep practicing, keep exploring, and most importantly, keep enjoying the process of problem solving.

Thank you so much for reading, and happy coding to you all!
