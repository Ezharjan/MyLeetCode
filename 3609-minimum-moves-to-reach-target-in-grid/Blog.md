# Solving the Code of the Infinite Grid Reverse Logic [#3609]

[Blog Post Link](https://leetcode.com/discuss/post/7556315/solving-the-code-of-the-infinite-grid-re-pzoc/)


Hello to everyone in the coding community! 
I hope you are all having a wonderful day solving problems and learning new algorithms. Today I want to share a journey I went through with a particularly tricky problem involving an infinite grid and some very large coordinates.

I must be honest with you all. I did not get this right on my first attempt. Or my second. In fact, I stared at the "Wrong Answer" screen quite a few times before the logic finally clicked. I fell into a few logical traps that I want to walk you through so you can avoid them. It is a humbling experience to see a logic error in what feels like a perfect mathematical solution, but I know that is exactly where the learning happens.

Let us dive deep into this problem, break down the mechanics, look at where I went wrong, and finally construct a robust solution together.

### Understanding the Infinite Grid Problem

The problem asks us to navigate a two dimensional grid. We are given a starting point `(sx, sy)` and a target point `(tx, ty)`.

The rules for moving are quite unique. If we are currently at a point `(x, y)`, we calculate a value `m` which is the maximum of `x` and `y`. Then we have two choices for our next step. We can either add `m` to `x`, moving to `(x + m, y)`, or we can add `m` to `y`, moving to `(x, y + m)`.

Our goal is to find the minimum number of moves to get from the start to the target. If it is impossible, we should return negative one.



The constraints are what make this problem interesting. The coordinates can be as large as 10 to the power of 9. This immediately tells us that we cannot use standard grid traversal algorithms like Breadth First Search or Depth First Search. The grid is simply too massive. We need a mathematical approach or a way to skip many steps at once.

### My Initial Thought Process and Failures

When I first looked at this, I thought about working backwards. This is a common strategy for grid problems where the forward branching factor is high but the backward path is deterministic.

In the forward direction, you have two choices at every step. This creates a binary tree of possibilities that grows exponentially. However, if we look at the target `(tx, ty)` and try to figure out where we came from, the choice is often unique.

**The Logic of Reversing**

If we are at `(tx, ty)` and `tx` is greater than `ty`, the last move must have changed the X coordinate. Why? Because the move rule adds a positive value to one coordinate. Coordinate values only grow. So if `tx > ty`, the previous Y must have been equal to `ty`, and the previous X was smaller.

So I started writing a simulation that simply reverses the moves.

1. If `tx > ty`, undo the operation on `tx`.
2. If `ty > tx`, undo the operation on `ty`.

But here is where I hit my first wall. The operation is "add max(x, y)". This actually creates two different scenarios for reversing.

Consider the forward move.

* Scenario A: `x` was greater than or equal to `y`. We add `x` to `x`. The new coordinate becomes `2 * x`. This is a doubling operation.
* Scenario B: `x` was less than `y`. We add `y` to `x`. The new coordinate becomes `x + y`.

So when we are moving backwards from `(tx, ty)` where `tx > ty`, we have to decide if we arrived there via doubling or via addition.

**My Fatal Mistake with the Diagonal**

The most embarrassing failure I had involved the diagonal case where `tx` equals `ty`.

In my early code, I reasoned that if `tx == ty`, say `(4, 4)`, the only way to reach this state is from `(0, 4)` or `(4, 0)`. This is because `max(0, 4)` is 4. Adding that to 0 gives 4.

So I wrote a condition like this:
"If `tx == ty`, check if the start point `(sx, sy)` matches `(0, ty)` or `(tx, 0)`. If it matches, we are done! If not, return negative one because it is impossible."

This logic seemed sound but it was flawed. I failed to consider that reaching `(0, 4)` might not be the end of the journey. The starting point could be `(0, 1)`. From `(0, 1)`, we can go to `(0, 2)`, then `(0, 4)`, then `(4, 4)`.

My code saw `(4, 4)`, realized the previous step was `(0, 4)`, checked if `(0, 4)` was the start, saw that it was not (since start is `0, 1`), and immediately gave up, returning an error.

I essentially treated the diagonal as a finish line. In reality, the diagonal is just a checkpoint. We might need to "bounce" off the diagonal to a state with a zero, and then continue shrinking that non zero coordinate all the way down to the start.

### The Detailed Solution Strategy

Let us build the correct logic step by step, handling the large numbers and the tricky edge cases.

#### 1. The Strategy: Inverse Operations

We will start at `(tx, ty)` and try to reduce these coordinates until they match `(sx, sy)`.

At any step `(tx, ty)`, assuming `tx > ty`:
The previous move updated X. The value added was `max(prev_x, ty)`.
Since `tx` grew from `prev_x` and `ty` stayed the same, and `tx` is now greater than `ty`, we have to determine if `prev_x` was larger or smaller than `ty`.

* **Possibility 1: Doubling.** If `prev_x >= ty`, then `max(prev_x, ty)` was `prev_x`. The new `tx` became `prev_x + prev_x` or `2 * prev_x`.
* This implies `tx` must be even.
* This implies the current `tx` must be at least `2 * ty`.


* **Possibility 2: Addition.** If `prev_x < ty`, then `max(prev_x, ty)` was `ty`. The new `tx` became `prev_x + ty`.
* This implies we just subtract `ty`.
* This happens when `tx` is less than `2 * ty`.



**Optimization:** If we just subtract `ty` repeatedly, we might get a Time Limit Exceeded error if `tx` is 1 billion and `ty` is 1. We would be doing 1 billion subtractions.
However, notice the threshold `2 * ty`.
If `tx` is massive (much larger than `2 * ty`), we know the previous moves were all likely doublings or huge additions. Actually, the rule is simpler:
If `tx > 2 * ty`, the previous move *must* have been a doubling of X. Why? Because if the previous move was `prev_x + ty`, then `prev_x` would have been `tx minus ty`. If `tx` is huge, `prev_x` is still huge (larger than `ty`), which contradicts the condition for addition (where `prev_x` must be smaller than `ty`).
So, as long as `tx > 2 * ty`, we can assume the operation was doubling.

Wait, strictly speaking, the doubling logic is:
Forward: `x -> 2x` (if `x >= y`).
Reverse: `tx -> tx / 2` (requires `tx` even and `tx / 2 >= ty`).

If `tx` is huge, can we just use modulo?
Actually, the Euclidean algorithm style reduction is safer.
If `tx > ty`, we want to reduce `tx`.
If `tx > 2 * ty`, it *must* be a doubling case. We check if `tx` is even. If so, `tx = tx / 2`.
If `tx < 2 * ty` but `tx > ty`, it *must* be the addition case (`prev_x + ty`). So `tx = tx minus ty`.

#### 2. Handling the "Impossible" Cases

We return negative one (impossible) if:

1. We overshoot the start (i.e., `tx < sx` or `ty < sy`).
2. We need to reverse a doubling (e.g., `tx > 2 * ty`) but `tx` is odd.
3. We hit the diagonal `(z, z)` but cannot transition to a valid previous state that leads to our start.

#### 3. The Diagonal Case Corrected

If `tx == ty`, we are at a state like `(4, 4)`.
The only valid previous states are `(0, 4)` or `(4, 0)`.
This is a critical junction.
We check our target `sx` and `sy`.

* If `sx == 0`, we should transition to `(0, ty)`. This matches the required X coordinate.
* If `sy == 0`, we should transition to `(tx, 0)`.
* If neither `sx` nor `sy` is zero, then it is impossible to have reached `(z, z)`, because the path to `(z, z)` requires passing through a zero coordinate.

Once we transition to `(0, ty)`, we do **not** stop. We increment our move count and let the `while` loop continue. The loop will then reduce `(0, ty)` down to `(0, sy)` using the standard logic. This was the missing piece in my original attempts.

### The Algorithm Walkthrough

Here is the flow of the final robust algorithm:

1. Initialize `moves = 0`.
2. Enter a loop that runs while `tx >= sx` and `ty >= sy`.
3. Inside the loop, first check if `(tx, ty)` matches `(sx, sy)`. If yes, return `moves`.
4. Check the Diagonal Condition:
* If `tx == ty`:
* If `sx == 0`, set `tx = 0`, increment moves.
* Else if `sy == 0`, set `ty = 0`, increment moves.
* Else, return ` -1`.




5. Check the Greater Coordinate (assume `tx > ty`):
* If `tx > 2 * ty`: This suggests doubling.
* Check if `tx` is divisible by 2. If not, return ` -1`.
* Update `tx = tx / 2`. Increment moves.


* Else (`tx < 2 * ty`): This suggests addition.
* Update `tx = tx minus ty`. Increment moves.




6. Symmetric logic applies if `ty > tx`.
7. If the loop terminates (meaning we dropped below `sx` or `sy` without matching), return ` -1`.

### Complexity Analysis

You might wonder about the efficiency.
When we perform the operation `tx = tx / 2`, we are halving the coordinate. This is logarithmic reduction.
When we perform `tx = tx minus ty`, it seems linear. However, we only do this subtraction when `tx < 2 * ty`. After one subtraction, `tx` becomes less than `ty`, and in the next iteration, we will likely be reducing `ty`.
This mimics the Euclidean algorithm for Greatest Common Divisor. The number of steps is proportional to the logarithm of the coordinate values.
Given coordinates up to 1 billion, the loop will run approximately 30 to 60 times. This is incredibly fast and well within time limits.

### The C++ Solution

Here is the clean, corrected code implementation. Note how we handle the diagonal case by updating the state and allowing the loop to proceed, rather than returning immediately.

```cpp
class Solution {
public:
    int minMoves(int sx, int sy, int tx, int ty) {
        // We will store the count of moves here
        int moves = 0;
        
        // We work backwards from the target to the start.
        // The loop continues as long as our current coordinates 
        // are not smaller than the starting coordinates.
        while (tx >= sx && ty >= sy) {
            
            // Success check: If we matched the start, return the count.
            if (tx == sx && ty == sy) {
                return moves;
            }
            
            // Case 1: The Diagonal Trap
            // If tx equals ty, we must have come from a state with a zero.
            if (tx == ty) {
                if (sx == 0) {
                    // Previous state was (0, ty)
                    tx = 0;
                    moves++;
                } else if (sy == 0) {
                    // Previous state was (tx, 0)
                    ty = 0;
                    moves++;
                } else {
                    // Impossible to reach (z, z) if start has no zeros
                    return -1;
                }
            } 
            // Case 2: X is the larger coordinate
            else if (tx > ty) {
                // We need to decide if we reverse a Double or an Add.
                // If tx is more than double ty, it must be a Double operation.
                if (tx > 2 * ty) {
                    // If tx is odd, it cannot be a result of doubling.
                    if (tx % 2 != 0) return -1;
                    tx /= 2;
                    moves++;
                } else {
                    // If tx is less than 2*ty, it was an Addition (prev_x + ty).
                    tx -= ty;
                    moves++;
                }
            } 
            // Case 3: Y is the larger coordinate
            else { 
                // Symmetric logic for Y
                if (ty > 2 * tx) {
                    if (ty % 2 != 0) return -1;
                    ty /= 2;
                    moves++;
                } else {
                    ty -= tx;
                    moves++;
                }
            }
        }
        
        // If we exit the loop, it means one of our coordinates 
        // became smaller than the start coordinate without finding a match.
        return -1;
    }
};

```

### Final Thoughts

This problem is a beautiful example of why we need to verify our assumptions about "end states." My failure to realize that reaching the diagonal was just an intermediate step cost me several attempts.

When you are solving problems that require working backwards, always trace the path fully. Ask yourself: "When I perform this reverse operation, does it guarantee I am done, or does it just put me in a new state that needs further processing?"

I hope this detailed breakdown helps you understand the mechanics of the grid and avoids the pitfalls I fell into. 
Happy coding and good luck with your algorithm practice!
