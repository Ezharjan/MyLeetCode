# Escaping the Circular Obstacles A Community Guide to Reaching the Rectangle Corner [#3235]

Hello fellow programming enthusiasts! I am so glad you stopped by today. I want to share a very interesting geometry and graph theory problem that I recently encountered. It is a fantastic exercise in changing how we perceive a problem. We will walk through the solution together step by step so that everyone, regardless of their current experience level, can understand the logic and implement it successfully. 

I am sharing this approach because it really helped me grasp the concepts better, and I hope it can be a helpful resource for you as well. This is just one way to solve the problem, and I am excited to share it with our wonderful community!

## Understanding the Challenge Before Us

Imagine you are standing in a large empty room. The room is perfectly rectangular. You are standing exactly at the bottom left corner, which we will call the origin, or the point zero zero. Your goal is to walk to the exact opposite side of the room, which is the top right corner. 



However, the room is not entirely empty. Scattered throughout this rectangular room are several circular obstacles. You are not allowed to step inside any of these circles, and you cannot even brush against their edges. Furthermore, you cannot leave the rectangular room; your entire journey must stay strictly inside the walls. 

The question asks us to write a program that will return true if a safe path exists from our starting point to our destination, and false if no such path can be found. 

At first glance, you might think about writing an algorithm to simulate walking through the room, searching for a clear path. You might consider algorithms like A Star or Dijkstra. But continuous space is very tricky to navigate algorithmically because there are infinite possible paths you could take.

This is where a beautiful shift in perspective comes into play.

## Changing Our Perspective From Finding Paths to Finding Walls

Instead of trying to find a path for ourselves, what if we tried to figure out if the circles form an impassable wall? 

Think about it this way. If a group of circles overlap one another and stretch all the way from the left wall of the room to the right wall, they form a complete barrier. You would never be able to cross from the bottom to the top. 



What other barriers could exist? 
* A chain of overlapping circles connecting the left wall to the bottom wall. This would trap us at the starting corner!
* A chain of circles connecting the top wall to the right wall. This would block off our destination corner!
* A chain of circles connecting the top wall to the bottom wall. This would split the room in half!

We can simplify all of these blocking scenarios into two distinct groups.

Let us define the boundaries of our rectangular room. The room has four edges. Let us group them based on where they are relative to our start and end points.

* **Group One:** The Left edge and the Top edge. 
* **Group Two:** The Bottom edge and the Right edge.

If a continuous chain of overlapping circles connects any edge from Group One to any edge from Group Two, it is mathematically impossible for us to travel from the bottom left corner to the top right corner. The path is completely severed. 

If no such connection exists between Group One and Group Two, then a path is guaranteed to exist! This turns our continuous geometry problem into a discrete graph theory problem. This is a much easier problem for our computers to solve.

## Building Our Graph Step by Step

To solve this using graph theory, we need to define our "nodes" and our "edges". 

Every single circle in our list will be considered a node in our graph. 

An edge will exist between two nodes if those two circles intersect or touch each other. When circles intersect, they merge to form a larger combined obstacle. 

We also need to know which circles touch the edges of our rectangular room. We will create two lists to keep track of this. One list will record if a circle touches Group One boundaries. The other list will record if a circle touches Group Two boundaries.

Let us explore how to determine if a circle touches a boundary.

### Checking Boundary Intersections

We need to check if a circle intersects with the line segments that make up the walls of our rectangle. Let us assume our circle has a center at x and y, with a radius r. Let the top right corner of our rectangle be at X and Y.

**The Left Wall Check**
The left wall is the vertical line where the x coordinate is zero, spanning from a y coordinate of zero to Y. 
A circle touches or crosses this line if its x coordinate is less than or equal to its radius. We also need to ensure the circle is not completely above or completely below the rectangle. We do this by checking if the y coordinate is within the vertical bounds, or if the distance from the circle center to the top left corner is within the radius.

**The Bottom Wall Check**
The bottom wall is the horizontal line where the y coordinate is zero. 
A circle touches this line if its y coordinate is less than or equal to its radius. Similar to the left wall, we verify the horizontal alignment by checking if the x coordinate is within the horizontal bounds of the room, or if the distance to the bottom right corner is within the radius.

**The Right Wall Check**
The right wall is the vertical line at coordinate X. 
A circle touches this line if the absolute difference between its x coordinate and X is less than or equal to its radius. We again verify the vertical alignment.

**The Top Wall Check**
The top wall is the horizontal line at coordinate Y. 
A circle touches this line if the absolute difference between its y coordinate and Y is less than or equal to its radius. We verify the horizontal alignment similarly.

If a circle satisfies the conditions for the Left or Top wall, we mark it as belonging to Group One. If it satisfies the conditions for the Right or Bottom wall, we mark it as belonging to Group Two. 

### Checking Circle to Circle Intersections

Next, we need to know if two circles touch each other to form a larger barrier. 

Suppose we have two circles. Circle A has a center at x1 and y1 with radius r1. Circle B has a center at x2 and y2 with radius r2.

The mathematical condition for two circles intersecting is quite beautiful. We calculate the distance between their centers. If this distance is less than or equal to the sum of their radii, then the circles must be touching or overlapping. 

To avoid calculating square roots, which can introduce precision errors and slow down our program, we compare the squared values. We calculate the square of the difference in their x coordinates, and add it to the square of the difference in their y coordinates. We then check if this sum is less than or equal to the square of the sum of their radii.

### The Center of Mass Filter

There is a small edge case we must handle gracefully. Sometimes, two circles might intersect far outside the boundaries of our rectangular room. If they intersect completely outside the room, their intersection does not block our path inside the room.

To prevent our algorithm from falsely thinking an outside intersection blocks the path, we use a concept similar to a center of mass check. We evaluate the weighted average of their coordinates based on their radii. If this weighted center falls outside the upper bounds of our rectangle, we can safely ignore the connection between these two circles for the purpose of forming an internal wall. This simple mathematical check keeps our graph accurate.

## Traversing the Graph to Find Blockages

Now that we have built our graph, we need to explore it. We have nodes representing circles, edges representing intersections, and we know which nodes belong to Group One and Group Two.



Our goal is to see if there is any pathway through our graph that starts at a node in Group One and ends at a node in Group Two. 

We can use a widely known algorithm called Breadth First Search for this task. 

Here is how the Breadth First Search will unfold:
1.  We will create a list to keep track of which circles we have already visited so we do not get stuck in an endless loop.
2.  We will look at every single circle. If a circle belongs to Group One and has not been visited yet, we will use it as a starting point.
3.  We will place this starting circle into a queue.
4.  While our queue is not empty, we will take the next circle out of the queue.
5.  We will immediately check if this current circle also belongs to Group Two. If it does, we have found a complete bridge from Group One to Group Two! This means a wall exists, and we must return false because the path is blocked.
6.  If it does not belong to Group Two, we will look at all the other circles connected to it. For every connected circle that we have not visited yet, we will mark it as visited and add it to our queue to explore next.
7.  If we finish exploring all possible connections from all starting points in Group One and we never find a link to Group Two, then no blocking wall exists. We can confidently return true!

## Analyzing the Efficiency

It is always good practice to understand the resource requirements of our code. 

**Time Complexity**
Our algorithm spends most of its time building the graph. We have to compare every circle against every other circle to see if they intersect. If we have N circles, we perform roughly N times N comparisons. Therefore, the time complexity is proportional to N squared. Given the constraints of the problem, this is perfectly acceptable and runs smoothly. The Breadth First Search also takes time proportional to the number of nodes and edges, which fits comfortably within the N squared boundary.

**Space Complexity**
We need memory to store our graph. We use an adjacency list to keep track of the connections. In the worst case scenario, every circle might touch every other circle. We also need memory for our visited list and our queue. Thus, the space complexity is also bounded by N squared in the worst case, but typically much less depending on how spread out the circles are. 

## The C++ Implementation

Below is the complete C++ code that brings all of these ideas together. I have kept the structure clean so you can follow the logic we just discussed.

```cpp
class Solution {
    bool touch_L(long long x, long long y, long long r, long long X, long long Y) {
        return (x <= r) && (y <= Y || x * x + (y - Y) * (y - Y) <= r * r);
    }
    
    bool touch_B(long long x, long long y, long long r, long long X, long long Y) {
        return (y <= r) && (x <= X || (x - X) * (x - X) + y * y <= r * r);
    }
    
    bool touch_R(long long x, long long y, long long r, long long X, long long Y) {
        return (abs(x - X) <= r) && (y <= Y || (x - X) * (x - X) + (y - Y) * (y - Y) <= r * r);
    }
    
    bool touch_T(long long x, long long y, long long r, long long X, long long Y) {
        return (abs(y - Y) <= r) && (x <= X || (x - X) * (x - X) + (y - Y) * (y - Y) <= r * r);
    }

public:
    bool canReachCorner(int xCorner, int yCorner, vector<vector<int>>& circles) {
        int n = circles.size();
        long long X = xCorner;
        long long Y = yCorner;
        
        vector<bool> g1(n, false);
        vector<bool> g2(n, false);
        
        for (int i = 0; i < n; ++i) {
            long long x = circles[i][0];
            long long y = circles[i][1];
            long long r = circles[i][2];
            
            if (touch_L(x, y, r, X, Y) || touch_T(x, y, r, X, Y)) {
                g1[i] = true;
            }
            if (touch_R(x, y, r, X, Y) || touch_B(x, y, r, X, Y)) {
                g2[i] = true;
            }
        }
        
        vector<vector<int>> adj(n);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                long long x1 = circles[i][0], y1 = circles[i][1], r1 = circles[i][2];
                long long x2 = circles[j][0], y2 = circles[j][1], r2 = circles[j][2];
                
                long long dx = x1 - x2;
                long long dy = y1 - y2;
                long long r_sum = r1 + r2;
                
                if (dx * dx + dy * dy <= r_sum * r_sum) {
                    if (x1 * r2 + x2 * r1 <= X * r_sum &&
                        y1 * r2 + y2 * r1 <= Y * r_sum) {
                        adj[i].push_back(j);
                        adj[j].push_back(i);
                    }
                }
            }
        }
        
        vector<bool> vis(n, false);
        for (int i = 0; i < n; ++i) {
            if (g1[i] && !vis[i]) {
                vector<int> q;
                q.push_back(i);
                vis[i] = true;
                
                int head = 0;
                while (head < q.size()) {
                    int u = q[head++];
                    
                    if (g2[u]) return false; 
                    
                    for (int v : adj[u]) {
                        if (!vis[v]) {
                            vis[v] = true;
                            q.push_back(v);
                        }
                    }
                }
            }
        }
        
        return true; 
    }
};

```

## Concluding Thoughts

I hope this thorough walkthrough has been beneficial to you! Changing the problem from finding a clear path to searching for blocking walls is a powerful technique that appears frequently in competitive programming and computer science in general.

By taking the time to carefully define our boundary conditions and using a standard graph traversal algorithm, we transformed a daunting continuous geometry problem into a manageable logical sequence.

Thank you so much for reading along. I encourage you to try writing the code from scratch yourself, as the practice of typing it out often cements the logic in your mind. Keep learning, keep sharing with the community, and happy coding!

Would you like me to explain how we might adapt this logic if the room was a different shape, perhaps a triangle or a circle?
