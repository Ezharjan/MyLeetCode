# Smashing Walls with Robots A Friendly Guide to Solving an Interesting Coding Challenge [#3661]

[Blog Post Link](https://leetcode.com/discuss/post/7609511/smashing-walls-with-robots-a-friendly-gu-fxd1/)

Hello everyone! 

Now I want to share my thoughts and a detailed walkthrough on a fascinating programming puzzle. If you enjoy problems involving coordinate geometry, strategy, and a bit of dynamic programming, you will likely find this one very enjoyable. I recently spent some time working through it, and I am excited to share a friendly, accessible breakdown of how we can approach it together. We will not claim this is the absolute ultimate way to solve it, but it is a solid, logical approach that works well and is easy to grasp once we break it down into smaller pieces.



### Understanding the Problem Description

Before we dive into the solution, let us make sure we completely understand the scenario. Imagine a straight, endless flat line. On this line, we have two types of objects:
* **Robots:** These are positioned at specific coordinates. Each robot has exactly one bullet. When fired, the bullet can travel up to a specific maximum distance. The catch is that a robot can only choose to fire its bullet in one direction, either strictly to the left or strictly to the right.
* **Walls:** These are also positioned at specific coordinates along the same line. 

The goal of our little simulation is to destroy as many unique walls as possible. However, there are some very important rules governing how the bullets interact with the world:

* **Wall Destruction:** A bullet will destroy every single wall it encounters in its path, provided the wall is within the bullet range.
* **Robots as Obstacles:** This is the most crucial rule! Bullets cannot pass through robots. If a robot fires a bullet and there is another robot in the path, the bullet stops exactly at that second robot. It will not travel any further, meaning any walls behind that second robot are completely safe from this specific bullet.
* **Overlapping Positions:** A wall and a robot can exist at the exact same coordinate. If this happens, and the robot at that coordinate fires its bullet (in either direction), the wall sharing its space is instantly destroyed.
* **Indestructible Robots:** Bullets never destroy robots. They only stop them.

We are given three arrays. The first array gives us the positions of the robots. The second array gives us the corresponding maximum firing distance for each robot. The third array gives us the positions of all the walls. Our mission is to figure out the maximum number of unique walls we can destroy if we choose the optimal firing direction (left or right) for every single robot.

### A Detailed Look at an Example

Let us ground our understanding by walking through a concrete example. Imagine we have the following setup:

* Robot positions: 10 and 2
* Robot distances: 5 and 1
* Wall positions: 5, 2, and 7

First, let us organize our information. It is always easier to visualize things from left to right. 
We have a robot at position 2 with a range of 1.
We have another robot at position 10 with a range of 5.
Our walls are located at positions 2, 5, and 7.

Let us evaluate the robot at position 2. It has a range of 1.
If it shoots left, its bullet travels from position 2 down to position 1.
If it shoots right, its bullet travels from position 2 up to position 3.
Notice that there is a wall exactly at position 2. No matter which way this robot shoots, because the bullet originates at position 2, that wall is destroyed. 

Now let us look at the robot at position 10. It has a range of 5.
If it shoots left, its bullet travels from position 10 down to position 5. The walls at position 7 and position 5 fall within this path.
If it shoots right, its bullet travels from position 10 up to position 15. There are no walls in this direction.

If the first robot shoots left, it destroys the wall at 2.
If the second robot shoots left, it destroys the walls at 5 and 7.
In total, we have destroyed all 3 walls. Therefore, the answer for this example is 3.

### Breaking the Problem Down into Logical Steps

When faced with a complex problem, it is highly beneficial to break it apart. Let us look at the key observations that will guide our strategy.

#### Observation 1 Walls Sharing a Position with a Robot
As we noticed in our example, if a wall sits exactly on top of a robot, its fate is sealed. The robot has to shoot its bullet, and the bullet will always cover the robot starting position. Therefore, we can safely count all these overlapping walls right at the very beginning, add them to a base score, and then completely remove them from our list of walls to process. This simplifies our main logic tremendously.

#### Observation 2 Robots Isolate the Map
The rule stating that bullets stop when they hit another robot is the secret key to unlocking this problem. Imagine our endless line again. The robots act like giant, impenetrable barriers. The space between the first robot and the second robot is completely isolated from the space between the second robot and the third robot. 

A bullet fired by a robot can only affect the interval strictly to its immediate left or strictly to its immediate right. It can never cross over another robot to affect a distant interval. This means we can evaluate the map interval by interval, looking at the space between adjacent robots.

#### Observation 3 The Dynamic Programming Approach
Because the map is broken into these independent intervals, we can use a step by step approach to calculate the maximum walls destroyed. As we move from left to right, processing one robot after another, the choice we make for the current robot only interacts with the choice we made for the previous robot.

Let us define two tracking variables to help us remember our best scores as we move along:
* `dp0` will store the maximum number of walls destroyed up to the current interval assuming the current robot decides to shoot to the left.
* `dp1` will store the maximum number of walls destroyed up to the current interval assuming the current robot decides to shoot to the right.

### The Interval Logic

Let us zoom in on a single interval between a left robot and a right robot. Let us call the left robot Robot A and the right robot Robot B. We want to count how many walls are destroyed in the space strictly between them.

There are four possible combinations of choices for Robot A and Robot B:
1.  **Robot A shoots Left, Robot B shoots Right:** In this scenario, neither robot is shooting into the interval between them. Zero walls are destroyed in this specific interval.
2.  **Robot A shoots Left, Robot B shoots Left:** Robot A shoots away from the interval. Robot B shoots into the interval. We need to count how many walls fall within Robot B firing range.
3.  **Robot A shoots Right, Robot B shoots Right:** Robot A shoots into the interval. Robot B shoots away from the interval. We need to count how many walls fall within Robot A firing range.
4.  **Robot A shoots Right, Robot B shoots Left:** Both robots shoot into the interval towards each other. This is the crossfire scenario. We need to count the walls that are destroyed by Robot A, plus the walls destroyed by Robot B. However, we must be very careful not to double count any walls that happen to fall into the overlapping ranges of both bullets!



To make these calculations lightning fast, we can use a binary search technique on our sorted list of walls. By using standard functions that find the boundaries of a range, we can instantly know how many walls are trapped in a specific blast zone without having to check each wall individually.

### Step by Step Algorithm Construction

Let us stitch all these ideas together into a clear, actionable plan.

**Step One Preparation and Sorting**
We need our data organized. We will pair each robot position with its respective distance. Then, we sort these pairs based on the robot positions from left to right. We also sort the array of wall positions from left to right.

**Step Two Filtering the Overlaps**
We iterate through our sorted walls and sorted robots. If we find a wall that shares the exact coordinate with a robot, we increment a special counter called `base_destroyed` and we do not include this wall in our new, filtered list of walls. We only keep walls that strictly live in the empty spaces between robots.

**Step Three The First Robot**
The first robot on the far left has an interval stretching out to negative infinity. If it shoots left, we count how many walls in that boundless left interval are hit. This becomes our initial `dp0`. If it shoots right, it does not affect the left interval, so our initial `dp1` is simply zero.

**Step Four Processing Intermediate Intervals**
We loop through the remaining robots one by one. For each adjacent pair of robots, we isolate the walls that sit strictly between them. 
We calculate the reach of the left robot shooting right.
We calculate the reach of the right robot shooting left.
Using binary search, we count the affected walls for the different firing combinations.
We then update our `dp0` and `dp1` values based on the maximum possible scores taking into account the transitions from the previous state.

**Step Five The Last Robot**
After we finish looping through all adjacent pairs, we have one final interval to consider. The space stretching from the very last robot out to positive infinity. We only need to check this space if the last robot decided to shoot right. We count any walls destroyed in this final stretch and add it to our `dp1` total.

Finally, we compare our final `dp0` and `dp1`, pick the larger number, and add our `base_destroyed` score to get the grand total.

### Exploring the C++ Solution

Below is the C++ implementation of the strategy we just discussed. I have included comments to help map the code back to our logical steps. While C++ syntax requires the use of the minus symbol for arithmetic operations and decrements, I have kept the logic as clean and readable as possible.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxWalls(vector<int>& robots, vector<int>& distance, vector<int>& walls) {
        int numRobots = robots.size();
        
        // Step One Preparation and Sorting
        // We pair robots with their distances so we can sort them together
        vector<pair<int, int>> robotData(numRobots);
        for (int i = 0; i < numRobots; ++i) {
            robotData[i] = {robots[i], distance[i]};
        }
        sort(robotData.begin(), robotData.end());
        sort(walls.begin(), walls.end());

        // Step Two Filtering the Overlaps
        // We will create a clean list of walls that do not touch any robots
        vector<int> cleanWalls;
        int baseDestroyedCount = 0;
        int wallIndex = 0;
        int robotIndex = 0;
        
        while (wallIndex < walls.size() && robotIndex < numRobots) {
            if (walls[wallIndex] < robotData[robotIndex].first) {
                cleanWalls.push_back(walls[wallIndex]);
                wallIndex++;
            } else if (walls[wallIndex] == robotData[robotIndex].first) {
                baseDestroyedCount++; 
                wallIndex++;
            } else {
                robotIndex++;
            }
        }
        
        // Add any remaining walls that are far to the right
        while (wallIndex < walls.size()) {
            cleanWalls.push_back(walls[wallIndex]);
            wallIndex++;
        }

        int currentWallIdx = 0;
        int intervalStart = 0;
        
        // Step Three The First Robot
        // Process the open space to the left of the very first robot
        while (currentWallIdx < cleanWalls.size() && cleanWalls[currentWallIdx] < robotData[0].first) {
            currentWallIdx++;
        }
        int intervalEnd = currentWallIdx;
        
        long long reachLeft = (long long)robotData[0].first - robotData[0].second;
        
        // Find how many walls are destroyed if the first robot shoots Left
        auto boundLeft = lower_bound(cleanWalls.begin() + intervalStart, cleanWalls.begin() + intervalEnd, reachLeft);
        int dp0 = (cleanWalls.begin() + intervalEnd) - boundLeft;
        int dp1 = 0;

        // Step Four Processing Intermediate Intervals
        for (int k = 1; k < numRobots; ++k) {
            int prev_k = k - 1;
            intervalStart = currentWallIdx;
            
            // Advance the wall index to find all walls strictly between the previous and current robot
            while (currentWallIdx < cleanWalls.size() && cleanWalls[currentWallIdx] < robotData[k].first) {
                currentWallIdx++;
            }
            intervalEnd = currentWallIdx;

            long long reachRightPrev = (long long)robotData[prev_k].first + robotData[prev_k].second;
            long long reachLeftCurr = (long long)robotData[k].first - robotData[k].second;

            // Using binary search to quickly count walls in range
            auto boundRight = upper_bound(cleanWalls.begin() + intervalStart, cleanWalls.begin() + intervalEnd, reachRightPrev);
            int countHitByPrevRight = boundRight - (cleanWalls.begin() + intervalStart);
            
            auto boundLeftCurr = lower_bound(cleanWalls.begin() + intervalStart, cleanWalls.begin() + intervalEnd, reachLeftCurr);
            int countHitByCurrLeft = (cleanWalls.begin() + intervalEnd) - boundLeftCurr;
            
            // Calculating the crossfire scenario without double counting
            int countUnion = 0;
            if (reachRightPrev >= reachLeftCurr) {
                // The bullets cross paths completely, all walls in this gap are destroyed
                countUnion = intervalEnd - intervalStart; 
            } else {
                // The bullets do not overlap, we just add the separate counts together
                countUnion = countHitByPrevRight + countHitByCurrLeft;
            }

            // Updating our state trackers
            int nextDp0 = max(dp0 + countHitByCurrLeft, dp1 + countUnion);
            int nextDp1 = max(dp0 + 0, dp1 + countHitByPrevRight);
            
            dp0 = nextDp0;
            dp1 = nextDp1;
        }

        // Step Five The Last Robot
        // Process the open space to the right of the very last robot
        intervalStart = currentWallIdx;
        intervalEnd = cleanWalls.size();
        
        int lastRobotIdx = numRobots - 1;
        long long reachRightLast = (long long)robotData[lastRobotIdx].first + robotData[lastRobotIdx].second;
        
        auto boundRightLast = upper_bound(cleanWalls.begin() + intervalStart, cleanWalls.begin() + intervalEnd, reachRightLast);
        int countLastInterval = boundRightLast - (cleanWalls.begin() + intervalStart);

        // We choose the best final outcome
        int bestIntermediateResult = max(dp0, dp1 + countLastInterval);

        // Return the sum of intermediate walls and the guaranteed base overlaps
        return bestIntermediateResult + baseDestroyedCount;
    }
};

```

### Analyzing the Efficiency

It is always good practice to understand how much time and memory our solution requires to run. Let N be the total number of robots and W be the total number of walls.

**Time Complexity**
The very first thing we do is sort the array of robots and the array of walls. Sorting algorithms generally take O(N log N) time for the robots and O(W log W) time for the walls.
After sorting, we iterate through the robots exactly once. Inside this loop, we perform binary searches on the wall array. The binary search takes logarithmic time, specifically bounded by O(log W). Since we do this N times, the interval processing phase takes O(N log W) time.
Combining these parts, our overall time complexity is dominated by the sorting steps, resulting in an O(N log N + W log W) time complexity. This is highly efficient and easily handles the large constraints given in the problem description.

**Space Complexity**
We allocate some extra memory to store our paired robot data array and our new, filtered clean walls array. The robot array takes O(N) space and the clean walls array takes O(W) space in the worst case scenario. We also use a few individual variables for our dynamic programming state trackers, which only take a constant amount of space. Therefore, the overall space complexity is O(N + W). This keeps our memory footprint nice and linear.

### Final Thoughts

This problem is a beautiful example of how breaking down a large, intimidating space into smaller, independent chunks can make a complex puzzle solvable. By realizing that robots act as natural partitions, we transformed a chaotic crossfire scenario into a neatly organized sequence of independent intervals.

I hope you found this detailed walkthrough helpful and easy to follow. Coding is all about sharing knowledge and learning from each other, so please feel free to use this approach, tinker with the code, and see if you can find even more interesting ways to frame the logic. Happy coding!
