# Navigating City Roads and Discovering Shorter Paths Together [#3244]

Hello everyone! Welcome back to my programming journal. Today I would like to share my thoughts on a very interesting pathfinding challenge. I recently spent some time working through this puzzle, and I found the underlying logic to be incredibly fascinating. I am excited to walk through the problem, explore a potential solution, and break down the concepts so we can all learn together. 

Grab a cup of coffee or tea, and let us dive right into the world of cities, roads, and graph traversals!

### Understanding the Problem Landscape

Imagine you are looking at a map of several cities. These cities are arranged in a perfectly straight line. Let us say there are n cities in total, and they are conveniently numbered from 0 all the way up to n minus 1. 



Initially, the local government has only built one way roads connecting each city directly to its immediate neighbor. This means there is a road from city 0 to city 1, another road from city 1 to city 2, and so on, ending with a road to city n minus 1. 

Because you can only travel forward to the very next city, the starting distance from the very first city to the very last city is exactly n minus 1 roads. 

Now, things get interesting. The government decides to build new one way expressways. You are given a series of queries, where each query represents the addition of a brand new expressway jumping directly from a starting city to an ending city further down the line. 

Your goal is to figure out the length of the shortest possible path from city 0 to city n minus 1 immediately after each new expressway is built. 

### The Secret Ingredient The Constraints

Whenever we look at a programming puzzle, the constraints often whisper the secret to the solution. There is one specific rule in this problem that changes everything. 

The rule states that no two queries will ever partially overlap. 

What does this mean in plain English? If you have one new road jumping from city 2 to city 5, you will never see another new road jumping from city 3 to city 6. The intervals of the expressways will either be completely separate from one another, or one will be entirely contained inside another. 



This single rule is incredibly helpful. It implies that when we add a new expressway, any cities that get bypassed by this new road become completely irrelevant for our overall shortest path from start to finish. We will never need to "step down" into those bypassed cities to catch a different overlapping expressway because the rules guarantee no such overlapping expressway exists!

### Brainstorming a Strategy

If we try to run a standard pathfinding algorithm like Breadth First Search after every single new road is added, our program might take too long. Since we could have up to a hundred thousand cities and a hundred thousand queries, traversing the map from scratch every time would require too much computing power.

Instead, we can use a more modest and gentle approach. We can think of the cities as a sequence of linked items. 

We can maintain a simple list, let us call it the "next city" list. For every city, this list will tell us the next active city on our shortest path. 
* At the very beginning, the next city for city 0 is 1. 
* The next city for city 1 is 2. 
* And so forth.

We also keep a running tally of our total distance, which starts at n minus 1.

When a new expressway is built from city A to city B, we can check if it actually helps us. If our "next city" list says that from city A, we are already jumping to a city at or beyond city B, then this new expressway is useless. We can just ignore it.

However, if the new road actually skips over several cities we previously had to visit, we can do a cleanup process! We can travel through those intermediate bypassed cities and officially "remove" them from our active path. For every city we successfully bypass and remove, we decrease our running total distance by exactly one. 

To ensure we do not accidentally process these removed cities again in the future, we can update their "next city" pointer to jump straight to city B. This concept is somewhat similar to path compression, keeping our future steps fast and tidy.

### Walking Through an Example Together

Let us trace a small example to make this crystal clear. Suppose we have 5 cities (n equals 5). 
Our initial distance is 4.
Our "next city" array looks like this: `[1, 2, 3, 4, 0]` (the last element does not matter much).

**Query 1: Add a road from 2 to 4**
We look at city 2. The next active city is currently 3. Since 3 is less than 4, this new road helps us!
We start cleaning up the bypassed cities. The only city strictly between 2 and 4 on our path is city 3.
We bypass city 3, decrease our total distance from 4 to 3.
We update the next active city from 2 to be 4.
Our distance is now 3.

**Query 2: Add a road from 0 to 2**
We look at city 0. Its next active city is 1. Since 1 is less than 2, this road helps us!
We bypass city 1. We decrease our distance from 3 to 2.
We update the next active city from 0 to be 2.
Our distance is now 2.

**Query 3: Add a road from 0 to 4**
We look at city 0. Its next active city is currently 2 (because of the previous step). Since 2 is less than 4, this road helps us immensely!
We bypass city 2. We decrease our distance from 2 to 1. 
We jump to the next active city from 2, which is 4 (we updated this in Query 1). We reached our destination.
We update the next active city from 0 to be 4.
Our distance is now exactly 1.

As you can see, by keeping track of the next active city, we gracefully skip over chunks of the path we have already compressed.

### Translating Thoughts into Code

Here is how we might write this out in C++. I have kept the variable names descriptive and added comments so you can follow the logic easily.

```cpp
class Solution {
public:
    vector<int> shortestDistanceAfterQueries(int n, vector<vector<int>>& queries) {
        // We create an array to keep track of the next valid city in our path.
        vector<int> nextCity(n);
        for (int i = 0; i < n; ++i) {
            nextCity[i] = i + 1;
        }
        
        // We start with the maximum possible shortest distance.
        int currentDistance = n - 1;
        
        // This will hold our answers after every single query.
        vector<int> results;
        results.reserve(queries.size());
        
        for (const auto& query : queries) {
            int startCity = query[0];
            int endCity = query[1];
            
            // We check if the new expressway provides a real shortcut.
            if (nextCity[startCity] < endCity) {
                int currentNode = nextCity[startCity];
                
                // We journey through the cities that are being bypassed.
                while (currentNode < endCity) {
                    int nodeToVisitNext = nextCity[currentNode];
                    
                    // We compress the path by pointing the bypassed city to the end.
                    nextCity[currentNode] = endCity; 
                    
                    // We move to the next city in the chain.
                    currentNode = nodeToVisitNext;
                    
                    // Every bypassed city means our total journey is one step shorter.
                    currentDistance--;        
                }
                
                // Finally, we officially link the start city to the end city.
                nextCity[startCity] = endCity;
            }
            
            // We record the distance for this round.
            results.push_back(currentDistance);
        }
        
        return results;
    }
};

```

### Analyzing the Complexity

When sharing solutions, it is always a good practice to talk about how well the code handles large inputs. Let us break down the time and space requirements in simple terms.

**Time Complexity:** O(N + Q)
Where N represents the number of cities and Q represents the number of queries.
You might see the nested while loop and worry that it might run too many times. However, notice what happens inside that loop. Every time the loop runs, we bypass a city and reduce our total distance. Since we only have N cities, and we can only bypass each city exactly one time throughout the entire program, that inner loop will run at most N times across all queries combined! Therefore, processing the queries takes steps proportional to Q plus N.

**Space Complexity:** O(N)
We created a single array called `nextCity` to keep track of our path. This array scales linearly with the number of cities. We also have our results array which scales with the number of queries, which is expected for returning the answer. Overall, the auxiliary space we use is very reasonable.

### Closing Thoughts

I really enjoyed this problem because it initially looks like a heavy graph traversal puzzle, but once you observe the specific rules provided by the problem description, it unfolds into a neat array manipulation task. It is a great reminder that sometimes reading the rules carefully can save us from writing overly complex code.

I hope you found this explanation helpful and accessible. Programming is a continuous journey of learning, and breaking down problems like this helps all of us grow.

Feel free to try this approach out yourself and let the community know how it works for you. Happy coding, everyone!
