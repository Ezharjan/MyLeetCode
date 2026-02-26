# A Friendly Guide to Navigating City Streets and Shortcuts [#3017]

Hello everyone! Welcome back to another coding adventure. Today we are going to explore a very interesting puzzle about city planning, streets, and finding the distance between houses. I am super excited to share my thoughts on this problem with all of you. We will take our time, walk through the logic step by step, and build an understanding together. 

This is a wonderful exercise in logical thinking and array manipulation. We will not claim this is the absolute ultimate way to solve it, but it is certainly a very neat and effective method that has helped me, and I hope it helps you too!

### Understanding the City Layout

Let us first paint a picture of our problem. Imagine a beautiful, long street in a quiet neighborhood. On this street, there are several houses lined up in a row. 

We are given an integer n, which represents the total number of houses. These houses are numbered from 1 all the way up to n. Because they are on a single straight street, each house is connected to its immediate neighbor. House 1 is connected to House 2, House 2 is connected to House 3, and so on.

If this was the whole story, finding the distance between any two houses would be incredibly simple. You would just count the steps down the street! However, our city mayor decided to build a special shortcut. 

There is one extra road built directly between a specific house x and another specific house y. This new road acts like a magic bridge. Suddenly, houses that were very far apart might be quite close if you take the shortcut!



Our ultimate goal is to find out how many pairs of houses exist for every possible shortest distance. We need to create an array where the value at position k tells us exactly how many pairs of houses have a minimum travel distance of exactly k streets. 

Note that a pair of houses going from House A to House B is considered distinct from going from House B to House A, so we will need to account for both directions.

### The Initial Thought Process and Why We Need a Clever Trick

When you first look at this puzzle, your mind might immediately jump to a classic graph traversal algorithm like Breadth First Search. After all, Breadth First Search is fantastic for finding the shortest path in an unweighted graph!

You could imagine starting a Breadth First Search from House 1, finding the distance to all other houses, and recording them. Then you move to House 2, run another Breadth First Search, and record those distances. You would repeat this for every single house.

While this approach is logically sound and will give you the correct answer, we have to look at the constraints. The number of houses n can be up to one hundred thousand. If we run a Breadth First Search from every single house, the number of operations will multiply rapidly. For one hundred thousand houses, this could take billions of operations, which will unfortunately lead to a Time Limit Exceeded error on most coding platforms.

We need a different perspective. Instead of simulating the walk from every house, what if we use mathematics and patterns to calculate the distances directly?

### Breaking Down the Math

Let us fix our starting position at a specific house, which we will call house i. We want to find the distance to any destination house, which we will call house j. To keep things organized, let us assume we are only looking forward, meaning house j is always greater than house i. Once we find the forward distances, we can just multiply our final counts by two to account for the reverse trips.

For any destination house j, there are exactly two ways to get there from our starting house i:

1. The Direct Route: We just walk straight down the main street. The distance is simply j minus i.
2. The Shortcut Route: We walk from our start house i to the shortcut entrance at house x, take the shortcut to house y, and then walk to our destination house j.

To make things simpler, let us assume house x is always positioned before or at the same place as house y. If it is not, we can just swap their values at the very beginning of our program.

The distance to reach the end of the shortcut from our starting house i is the distance from i to x, plus one street for the shortcut itself. Let us call this base shortcut cost C. 
C equals the absolute difference between i and x, plus 1.

Now, the total distance using the shortcut to reach house j would be C plus the absolute difference between j and y.

Our task for each starting house i is to figure out which route is shorter for every possible destination house j, and then count how many houses fall into each distance category. 

### Introducing Difference Arrays

Before we dive into the specific regions of our street, we need a tool to efficiently record our findings. If we determine that a whole block of houses, say from distance 3 to distance 7, all can be reached via the direct route, we do not want to use a loop to update distances 3, 4, 5, 6, and 7 one by one. That would be too slow!



Instead, we use a concept known as a difference array. A difference array allows us to add a value to a whole range of indices in constant time. 

If we want to add 1 to all distances in the range from L to R, we simply do two things in our difference array:
* Add 1 at index L.
* Subtract 1 at index R plus 1.

Later, when we are completely finished evaluating all houses, we can just run a running total or prefix sum across the difference array. This running total will perfectly reconstruct the final frequencies of all distances! This is a beautiful trick that saves us from nested loops.

### Analyzing the Street Regions

For a fixed starting house i, the destination houses j can be divided into distinct regions based on where they sit relative to our shortcut exit y.

Region A represents destination houses that are located before or exactly at the shortcut exit y. In this region, taking the shortcut means you travel forward to y, and then you have to travel backward to reach j. 
The direct route distance is: j minus i
The shortcut route distance is: C plus y minus j

We need to find the exact point where the shortcut becomes strictly faster than the direct route. By setting those two distances equal to each other, we can find a separation point. The separation point is exactly halfway between the direct route and the shortcut route math.
Separation point equals (i plus C plus y) divided by 2.

Any house located before or at this separation point will be reached faster or equally fast by just walking straight down the main street. Any house located after this separation point will be reached faster by taking the magic bridge!

Therefore, we can split Region A into two distinct chunks:
* Chunk 1: Houses where the direct route wins. The distances will form a continuous sequence starting from 1 up to a certain maximum. We update our difference array for this continuous range.
* Chunk 2: Houses where the shortcut wins. The distance formula changes, but it also forms a continuous sequence of distances! We calculate the start and end of this distance range and update our difference array again.

Region B represents destination houses that are located after the shortcut exit y. 
In this region, the direct route distance is still j minus i.
The shortcut route distance is now: C plus j minus y.

Notice something fascinating here! Both formulas grow at the exact same rate as j increases. This means the difference between the two routes remains perfectly constant for the entirety of Region B. 

Because the difference is constant, one route will permanently be better than the other for all houses in Region B. We just need to check the math once:
Is i plus C greater than or equal to y?
If yes, the direct route is always faster for every single house in Region B.
If no, the shortcut route is strictly faster for every single house in Region B.

Just like before, the resulting distances for Region B will form a perfect continuous range. We find the minimum and maximum distances for this region and record the range in our difference array.

### Putting It All Together in Code

Now that we have heavily detailed the logic, let us look at how this translates into a C++ program. I have added comments to help guide you through the translation of our ideas into actual code.

```cpp
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<long long> countOfPairs(int n, int x, int y) {
        // First we make sure our shortcut starts at the smaller house number
        if (x > y) {
            swap(x, y);
        }
        
        // We create a difference array with some extra padding to prevent out of bounds errors
        vector<long long> diff(n + 2, 0);
        
        // We iterate through every possible starting house i
        for (int i = 1; i <= n; i++) {
            
            // C is the cost to reach the end of the shortcut from house i
            int c = abs(i - x) + 1;
            
            // Checking Region A (destination houses between i and y)
            if (i + 1 <= y) {
                // Find the tipping point where the shortcut becomes better
                int sep = (i + c + y) / 2;
                int end_A1 = min(y, sep);
                
                // Chunk A1: The direct path is faster or equal
                if (i + 1 <= end_A1) {
                    diff[1]++;
                    diff[end_A1 - i + 1]--;
                }
                
                // Chunk A2: The shortcut path is strictly faster
                if (end_A1 + 1 <= y) {
                    diff[c]++;
                    diff[c + y - end_A1]--;
                }
            }
            
            // Checking Region B (destination houses strictly after y)
            int start_B = max(i + 1, y + 1);
            if (start_B <= n) {
                if (i + c >= y) {
                    // Direct path is permanently faster in this region
                    diff[start_B - i]++;
                    diff[n - i + 1]--;
                } else {
                    // Shortcut path is permanently faster in this region
                    diff[c + start_B - y]++;
                    diff[c + n - y + 1]--;
                }
            }
        }
        
        // Finally, we process our difference array to get the real answers
        vector<long long> res(n);
        long long current = 0;
        
        for (int k = 1; k <= n; k++) {
            current += diff[k];
            // We multiply by 2 because we need to count both directions
            res[k - 1] = current * 2; 
        }
        
        return res;
    }
};

```

### Understanding the Efficiency

Let us talk about why this method is so smooth and handles the large constraints effortlessly.

Time Complexity:
We have a single loop that runs n times, representing each starting house. Inside this loop, we are only performing basic arithmetic comparisons and updating exactly four values in our difference array at most. These are all constant time operations.
After the main loop finishes, we run one more loop from 1 to n to calculate the running total of our difference array.
Because there are no nested loops depending on the size of the input, our overall time complexity is entirely linear, commonly written as O(n). This is extremely fast and will pass all test cases with flying colors!

Space Complexity:
We allocate a difference array of size n plus 2, and an output array of size n. Memory allocation scales linearly with the number of houses, so our space complexity is also O(n). This easily fits within standard memory limits.

### Final Thoughts

I really hope this detailed walkthrough brought some clarity to the problem! By stepping back from traditional graph searches and instead looking at the mathematical relationship between the distances, we transformed a potentially overwhelming problem into an elegant, linear sequence of operations.

The difference array is a truly wonderful tool to keep in your back pocket for whenever you need to apply uniform changes across a range of items.

Thank you so much for reading along. Keep practicing, keep sharing your knowledge with the community, and happy coding! Let me know in the comments if you have any questions or if you approached this from a different angle.
