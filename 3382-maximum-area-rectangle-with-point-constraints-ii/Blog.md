# Solving the Empty Rectangle Puzzle with Point Constraints A Friendly Guide [#3382]

Hello everyone! Today I would like to share a walkthrough of a very interesting computational geometry puzzle. If you enjoy visualizing points on a grid and combining different algorithmic concepts, this one is a real treat. We are going to break down the problem, explore the underlying concepts, and build a working C++ solution together. 

I want to keep things conversational and accessible, so whether you are a seasoned competitive programmer or just someone looking to learn a new technique, I hope you find this guide helpful!

***

## Understanding the Problem

Imagine you have a massive piece of graph paper extending infinitely in all directions. On this paper, someone has drawn several distinct dots. Your goal is to draw a rectangle using exactly four of these dots as its corners. 

However, there are three strict rules you must follow:

1. The edges of your rectangle must be perfectly parallel to the X and Y axes. 
2. The rectangle must be completely empty. This means absolutely no other dots can be inside the rectangle, and no other dots can sit on the border lines of the rectangle. 
3. You want to find the rectangle that has the largest possible area while still obeying the first two rules. If you cannot form a single valid rectangle, you should report negative one.

The challenge here comes from the scale. You could be given up to two hundred thousand points, and the coordinates can reach up to eighty million. Because of this massive scale, we cannot simply guess every single combination of four points. A brute force approach would take far too long. Instead, we need a smarter way to organize our data and scan through it.

***

## The Conceptual Strategy

To solve this efficiently, we can combine three powerful techniques. Let us explore each one conceptually before we write any code.

### Concept 1 The Sweep Line Algorithm

Imagine a perfectly vertical laser beam starting at the far left side of your graph paper. Slowly, this laser beam moves to the right, stopping only when it hits a vertical column that contains at least one dot. This is the essence of a Sweep Line algorithm.



Instead of looking at all points at once, we sort our points based on their X coordinates. This allows our virtual laser to process the points column by column from left to right. When our laser stops at a specific X coordinate, we look at all the dots situated exactly on that vertical line.

Because any valid rectangle must have edges parallel to the axes, the left boundary of our rectangle will be formed by two dots on one vertical line, and the right boundary will be formed by two dots on another vertical line further to the right. By moving our sweep line, we can pair up historical vertical segments with current vertical segments to form rectangles.

### Concept 2 Coordinate Compression

We have a slight issue. The Y coordinates of our dots can be as large as eighty million. Later on, we will need to use these Y coordinates as indices in an array to keep track of point frequencies. An array of size eighty million is too large and wastes a lot of memory, especially since we only have at most two hundred thousand dots.

We solve this using Coordinate Compression. We gather all the unique Y coordinates from our input, sort them from smallest to largest, and assign them new, dense integer values starting from one. 

For example, if our Y coordinates are 10, 5000, and 80000000, we simply map them to 1, 2, and 3. The relative vertical ordering remains exactly the same, which is all that matters for determining if dots are inside our rectangle. The actual physical distances are only needed when we calculate the final area, for which we can just refer back to the original Y values.

### Concept 3 The Fenwick Tree for Point Counting

To ensure our rectangle is completely empty, we need a fast way to count dots. Specifically, if we have a left vertical boundary and a right vertical boundary, we need to know if any dots exist strictly between them within that specific vertical range.



Enter the Fenwick Tree, also known as a Binary Indexed Tree. This is a brilliant data structure that allows us to do two things very quickly:
* Update a tally (like adding a new dot at a specific Y coordinate).
* Query the total sum of tallies within a range of Y coordinates.

As our vertical sweep line moves from left to right, we will drop every dot we encounter into our Fenwick Tree based on its compressed Y coordinate. 

Here is the magic trick for the emptiness check: 
When we form a left vertical boundary using two dots, we ask our Fenwick Tree: "How many dots have we seen so far in this entire vertical band?" We save this number. 
Later, when our sweep line finds a matching right vertical boundary using two dots at the exact same Y elevations, we ask the Fenwick Tree the same question: "How many dots are in this vertical band NOW?"

If the new count is exactly the same as the old count, it means absolutely zero dots were added to our tree while the sweep line traveled between the left boundary and the right boundary. The space is completely empty! 

***

## Step by Step Implementation Guide

Let us walk through the execution of our logic step by step. 

### Step 1 Preparation and Sorting

First, we handle the coordinate compression for the Y axis. We extract all Y values, sort them, and remove duplicates. We then create a custom Point structure to hold the original X coordinate, the original Y coordinate, and the newly compressed Y coordinate. 

We populate a list of these Point objects and sort them. The primary sorting condition is the X coordinate (left to right), and the secondary condition is the Y coordinate (bottom to top).

### Step 2 Tracking Adjacent Pairs

For any vertical line drawn at a specific X coordinate, a valid left or right edge of an empty rectangle can only be formed by two adjacent points on that line. If we picked two points that had another point vertically between them, that middle point would end up sitting on the border of our rectangle, violating the rules.

Therefore, at every unique X coordinate, we look at the dots present and pair up the adjacent ones. We assign a unique identification number to every distinct pair of compressed Y coordinates we find. We also maintain an array called "last seen" to remember the exact X coordinate and the Fenwick Tree dot count the last time we encountered each specific Y coordinate pair.

### Step 3 Sweeping the Plane

Now we begin our main loop, moving our sweep line column by column. At each unique X coordinate, we perform three distinct phases.

**Phase A Checking for Rectangles**
Before we add the current column's dots into our Fenwick Tree, we look at all adjacent vertical pairs in the current column. For each pair, we check our "last seen" array. Have we seen this exact vertical segment before? 

If yes, we retrieve the previous X coordinate and the previous dot count. We then query our Fenwick Tree for the current dot count in that horizontal band. If the current count matches the previous count, we have found a completely empty rectangle! We calculate its area using the physical X and Y distances and update our maximum area variable.

**Phase B Adding Points to the Tree**
Once we have checked all pairs in the current column, we register every single dot from this column into our Fenwick Tree. We increment the value at their compressed Y index by one.

**Phase C Updating the Record**
Finally, we update our "last seen" array. For every adjacent pair we processed in this column, we record the current X coordinate. We also query the Fenwick Tree again to record the brand new dot count for this vertical band (which now includes the dots we just added). This prepares us for future columns.

***

## Complexity Analysis

Let us discuss the resources our approach requires.

**Time Complexity** * Sorting the initial points and preparing the compressed coordinates takes O(N log N) time.
* The Sweep Line processes each point exactly once. Sorting the adjacent pairs takes O(N log N) time. 
* Querying and updating the Fenwick Tree takes logarithmic time relative to the number of unique Y coordinates. Thus, the sweep line phase runs in roughly O(N log U) time, where U is the number of unique Y values. 
* Overall, the time complexity is nicely bounded by O(N log N), which easily handles the maximum constraint of two hundred thousand points without timing out.

**Space Complexity**
* We store arrays for the points, compressed Y values, and unique pair identifiers.
* The Fenwick tree requires space proportional to the number of unique Y values.
* Overall, the memory footprint scales linearly with the input size, giving us a Space Complexity of O(N).

***

## The C++ Source Code

Here is the complete code implementing the logic we just discussed. I have added comments throughout to help you connect the code back to our conceptual steps.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    // Fenwick Tree (Binary Indexed Tree) for point counting
    struct Fenwick {
        int n;
        vector<int> tree;
        Fenwick(int n) : n(n), tree(n + 1, 0) {}
        
        // Adds a value to the specific index
        void add(int i, int delta) {
            for (; i <= n; i += i & -i) {
                tree[i] += delta;
            }
        }
        
        // Queries the prefix sum from 1 to i
        int query(int i) {
            int sum = 0;
            for (; i > 0; i -= i & -i) {
                sum += tree[i];
            }
            return sum;
        }
    };

    // Custom structure to hold point data
    struct Point {
        int x;
        int y;
        int cy; // Compressed Y coordinate
        
        // Sorting logic left to right, then bottom to top
        bool operator<(const Point& o) const {
            if (x != o.x) return x < o.x;
            return y < o.y;
        }
    };

public:
    long long maxRectangleArea(vector<int>& xCoord, vector<int>& yCoord) {
        int n = xCoord.size();
        if (n < 4) return -1;
        
        // Step 1 Coordinate compression on Y coordinates
        vector<int> y_vals = yCoord;
        sort(y_vals.begin(), y_vals.end());
        y_vals.erase(unique(y_vals.begin(), y_vals.end()), y_vals.end());
        int U = y_vals.size();
        
        vector<Point> pts(n);
        for(int i = 0; i < n; ++i) {
            pts[i].x = xCoord[i];
            pts[i].y = yCoord[i];
            // Get one based compressed Y index for the Fenwick tree
            pts[i].cy = distance(y_vals.begin(), lower_bound(y_vals.begin(), y_vals.end(), yCoord[i])) + 1;
        }
        
        // Sort points left to right, bottom to top
        sort(pts.begin(), pts.end());
        
        // Step 2 Extract and assign an ID to all uniquely occurring adjacent Y pairs
        vector<pair<int, int>> all_pairs;
        all_pairs.reserve(n);
        for (int i = 0; i < n; ) {
            int j = i;
            while (j < n && pts[j].x == pts[i].x) {
                j++;
            }
            for (int p = i; p < j - 1; ++p) {
                all_pairs.push_back({pts[p].cy, pts[p+1].cy});
            }
            i = j;
        }
        
        sort(all_pairs.begin(), all_pairs.end());
        all_pairs.erase(unique(all_pairs.begin(), all_pairs.end()), all_pairs.end());
        
        int num_pairs = all_pairs.size();
        
        // Stores previous X coordinate and count of points in band
        // Initialize with negative one to indicate empty state
        vector<pair<int, int>> last_seen(num_pairs, {-1, -1}); 
        
        Fenwick bit(U);
        long long max_area = -1;
        vector<int> current_ids;
        current_ids.reserve(n);
        
        // Step 3 Sweep Line phase across X coordinates
        for (int i = 0; i < n; ) {
            int j = i;
            // Find all points in the current vertical column
            while (j < n && pts[j].x == pts[i].x) {
                j++;
            }
            current_ids.clear();
            
            // Phase A Check segments formed at the current X to compute areas
            for (int p = i; p < j - 1; ++p) {
                int cy1 = pts[p].cy;
                int cy2 = pts[p+1].cy;
                
                // Find the unique ID for this vertical segment
                auto it = lower_bound(all_pairs.begin(), all_pairs.end(), make_pair(cy1, cy2));
                int id = distance(all_pairs.begin(), it);
                current_ids.push_back(id);
                
                // Query current points strictly in this Y boundary
                int current_count = bit.query(cy2) - bit.query(cy1 - 1);
                
                // If we have recorded this Y segment at an earlier X before
                if (last_seen[id].first != -1) {
                    auto [prev_x, prev_count] = last_seen[id];
                    
                    // Verify if the region inside is devoid of other points
                    if (current_count == prev_count) {
                        int y1 = pts[p].y;
                        int y2 = pts[p+1].y;
                        long long area = 1LL * (pts[i].x - prev_x) * (y2 - y1);
                        if (area > max_area) {
                            max_area = area;
                        }
                    }
                }
            }
            
            // Phase B Mark the points existing at the current X on our Fenwick tree
            for (int p = i; p < j; ++p) {
                bit.add(pts[p].cy, 1);
            }
            
            // Phase C Update our last seen maps after inclusion
            for (int p = i; p < j - 1; ++p) {
                int cy1 = pts[p].cy;
                int cy2 = pts[p+1].cy;
                int id = current_ids[p - i];
                
                // Record the count after adding the points of the current column
                int count_after = bit.query(cy2) - bit.query(cy1 - 1);
                last_seen[id] = {pts[i].x, count_after};
            }
            
            // Move our sweep line to the next distinct X coordinate
            i = j;
        }
        
        return max_area;
    }
};

```

---

## Wrapping Up

By blending Coordinate Compression to manage large scales, a Sweep Line to organize our geometric search, and a Fenwick Tree to quickly verify empty space, we created a very neat and practical solution to a seemingly complex constraint problem.

Taking the time to break down geometry problems into horizontal and vertical components often reveals elegant logic underneath. I hope this explanation provided you with a clear mental model of how these data structures can collaborate to solve 2D spacial problems.

Happy coding, and feel free to share your thoughts or alternative approaches in the community!
