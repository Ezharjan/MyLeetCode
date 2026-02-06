# Cracking the Code of Circular Movement and Array Transformation [#3379]

[Blog Post Link](https://leetcode.com/discuss/post/7555084/cracking-the-code-of-circular-movement-a-dgjt/)


Hello, fellow coders and problem solvers! Welcome to another deep dive into the fascinating world of algorithmic challenges. Today we are tackling a problem that seems simple on the surface but contains a beautiful lesson on modular arithmetic and array manipulation. If you have ever struggled with "wrapping around" an array or confused yourself with negative indices, this guide is written specifically for you. We are going to explore the mechanics of the "Transformed Array" problem, break down the mathematics of circular indexing, and build a highly efficient C++ solution together.

### The Challenge at a Glance

Let us first understand what we are trying to solve. Imagine you are given a list of integers. This is not just a standard straight line of numbers. You must visualize this list as a circle. The last number connects right back to the first number.

Your goal is to build a new list of the exact same size. For every single position in your original list, you need to perform a specific movement to find a new value.

Here are the rules of movement for every index `i` in the list:

1. **If the number is positive**: You move that many steps to the **right**.
2. **If the number is negative**: You move that absolute number of steps to the **left**.
3. **If the number is zero**: You stay exactly where you are.

Once you finish moving, you look at the number written at your destination index. That number becomes the new value for your result at index `i`.

The "circular" part is the key.

* Moving past the last element? You appear at the beginning.
* Moving before the first element? You wrap around to the end.

### Visualizing the Problem

Let us try to visualize this with a concrete example to ensure we are all on the same page.

Imagine we have an array: `[3, -2, 1, 1]`

**Step 1**
Look at index 0. The value is **3**.
Because it is positive, we move 3 steps to the right.

* Start at index 0.
* Step 1 lands on index 1.
* Step 2 lands on index 2.
* Step 3 lands on index 3.
The value at index 3 is **1**. So, the first element of our new array is **1**.

**Step 2**
Look at index 1. The value is **-2**.
Because it is negative, we move 2 steps to the left.

* Start at index 1.
* Step 1 to the left lands on index 0.
* Step 2 to the left wraps around the circle and lands on index 3.
The value at index 3 is **1**. So, the second element of our new array is **1**.

**Step 3**
Look at index 2. The value is **1**.
Move 1 step right.

* Start at index 2.
* Step 1 lands on index 3.
The value at index 3 is **1**. So, the third element of our new array is **1**.

**Step 4**
Look at index 3. The value is **1**.
Move 1 step right.

* Start at index 3.
* Step 1 wraps around and lands on index 0.
The value at index 0 is **3**. So, the fourth element of our new array is **3**.

**Final Result**: `[1, 1, 1, 3]`

### The Mathematical Hurdle

The logic sounds straightforward until we start coding it. The primary obstacle in this problem is calculating the correct index when we wrap around, especially when moving backwards (left).

In a standard zero indexed world of size `N`, moving `k` steps to the right from index `i` is easy. We use the modulo operator:
`target = (i + k) % N`

If `N` is 4, `i` is 3, and we move 1 step (`k`=1), then `(3 + 1) % 4` equals `0`. Perfect.

However, problems arise when `k` is negative. Let us say we are at index `1` and we need to move `2` steps left (`k` = -2).
Mathematically, `1 + (-2)` is `-1`.
In C++, computing `-1 % 4` yields `-1`.
But `-1` is not a valid array index! We want it to wrap around to index `3`.

Many beginners try to solve this with `if` statements or loops.

* "If the index is less than 0, add N."
* "If the index is still less than 0, add N again."

This approach works but is messy and slow if the negative number is very large. We need a cleaner, mathematical solution that works for any number, no matter how large or how negative.

### The Golden Formula for Circular Arrays

To handle wrapping correctly in both directions without using loops or conditional checks, we can use a specific formula.

Let `i` be the current index.
Let `x` be the movement value (positive or negative).
Let `n` be the size of the array.

The formula is:
**`target = ((i + x) % n + n) % n`**

Let us break down why this works beautifully.

1. **`(i + x)`**: This gives us the raw destination. It might be greater than `n` or it might be negative.
2. **`(i + x) % n`**: This reduces the number to a range close to 0. In C++, if the input is negative, this result remains negative. For example, `-5 % 4` becomes `-1`.
3. **`... + n`**: We add `n` to the result. This effectively "lifts" the negative value into the positive range. If we had `-1`, adding `4` makes it `3`.
4. **`... % n`**: We apply modulo one last time. Why? Because if the original number was already positive, adding `n` might make it too big again (indexes must be less than `n`). This final modulo ensures the result stays strictly within the range `0` to `n-1`.

This formula is robust. It handles:

* Standard right shifts.
* Left shifts (negative numbers).
* Large numbers that wrap around the array multiple times.

### Implementing the Solution

Now that we have our mathematical key, the coding becomes elegant and simple. We do not need complex data structures. A simple iteration through the array is sufficient.

Here is the strategy:

1. Read the size of the input array.
2. Create a `result` array of the same size.
3. Loop through every index `i` from `0` to `n-1`.
4. Inside the loop, apply our Golden Formula to find where we land.
5. Read the value at that landing spot.
6. Save that value into our `result` array at index `i`.
7. Return the `result` array.

Let us look at the C++ implementation.

```cpp
class Solution {
public:
    vector<int> constructTransformedArray(vector<int>& nums) {
        // First we determine the size of the array
        int n = nums.size();
        
        // We prepare a new vector to hold our answers
        // It is important to initialize it with size n
        vector<int> result(n);
        
        // Now we iterate through every element of the original array
        for (int i = 0; i < n; ++i) {
            
            // Here is the application of our Golden Formula.
            // nums[i] represents the steps we need to take.
            // i is our starting position.
            // We compute the new position handling both negative 
            // and positive wraps efficiently.
            int steps = nums[i];
            int targetIndex = ((i + steps) % n + n) % n;
            
            // We assign the value found at the target index 
            // to our current position in the result array
            result[i] = nums[targetIndex];
        }
        
        // Finally we return the constructed array
        return result;
    }
};

```

### Why This Code is Acceptable

When we write software, we care about two main resources: **Time** and **Space**.

**Time Complexity: O(N)**
We touch every element in the array exactly one time. Inside our loop, we only perform basic arithmetic operations (addition, modulo). These operations take constant time. Therefore, if our array has 100 elements, we do roughly 100 operations. If it has 1,000,000 elements, we do 1,000,000 operations. The time taken grows linearly with the input size. This is the best possible time complexity because we must visit every index to fill the result.

**Space Complexity: O(N)**
We are creating a new array `result` to store the output. This takes up space proportional to the number of elements in the input. The problem statement implies we need to return a new array, so this space usage is necessary and unavoidable. We use no other significant memory.

### Edge Cases to Consider

A good programmer always thinks about what could go wrong. Let us review some edge cases.

**1. The Element is Zero**
If `nums[i]` is `0`, our formula becomes `((i + 0) % n + n) % n`.
This simplifies to `(i % n + n) % n`.
Since `i` is always valid (between `0` and `n-1`), this just simplifies back to `i`.
So if the value is `0`, we define `result[i]` as `nums[i]`. This matches the problem requirement exactly.

**2. The Steps are Larger than the Array Size**
Imagine the array size is 4, but `nums[i]` is 10.
Moving 10 steps in a circle of 4 is the same as moving 2 steps (because `10 % 4 = 2`).
Our formula handles this naturally via the modulo operator. `(i + 10) % 4` will correctly wrap around the circle multiple times and land on the correct spot.

**3. Large Negative Numbers**
Imagine the array size is 4, but `nums[i]` is -10.
Math time: `-10 % 4` is `-2` in C++.
Formula: `(-2 + 4) % 4` becomes `2`.
Moving 10 steps left is indeed the same as landing 2 steps to the right. The math holds up perfectly.

### Final Thoughts

This problem is a classic example of how understanding the underlying mathematics can simplify code. Instead of writing long `if-else` chains to handle moving left versus moving right, or using `while` loops to correct negative indices, we used the properties of modular arithmetic to create a universal one line solution.

This pattern of `((a % n) + n) % n` is a powerful tool in your coding toolkit. You will see it again in problems involving cyphers, rotating images, or scheduling cyclic events.

This solution is not the best one but it is acceptable.

I hope this guide helps you understand not just the solution to this specific problem, but the beauty of circular array logic. 
Happy leet-coding everyone, and see you in the next challenge!

Feel free to share your thoughts and insightful comments below for better solutions~
