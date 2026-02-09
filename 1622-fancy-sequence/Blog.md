# Mastering Fancy Sequences via Linear Math [#1622]

[Blog Post Link](https://leetcode.com/discuss/post/7566169/mastering-fancy-sequences-via-linear-mat-xs3m/)

Hello fellow coding enthusiasts!

Today I want to share a personal journey through a problem that initially left me scratching my head. The problem is Fancy Sequence. It appears deceptively simple at first glance but hides a beautiful mathematical puzzle underneath.

I remember staring at this problem for quite some time. My first few attempts were absolute failures. I hit time limits, got wrong answers, and struggled with integer overflows. It was a humbling experience! However, the moment the final mathematical model clicked, it felt like magic. I want to walk you through my exact thought process, including the mistakes I made, so you can see how to arrive at the optimal solution yourself.

## Understanding the Challenge

Let us first look at what we are being asked to build. We need to implement a class called `Fancy` that manages a growing list of integers. This sequence starts empty, and we need to support four specific operations.

**1. The Append Operation**
We receive a request to `append(val)`. We simply add the integer `val` to the end of our current sequence.

**2. The Add All Operation**
We receive a request to `addAll(inc)`. We must increment **every single number** currently in the sequence by the value `inc`.

**3. The Multiply All Operation**
We receive a request to `multAll(m)`. We must multiply **every single number** currently in the sequence by the integer `m`.

**4. The Get Index Operation**
We receive a request to `getIndex(idx)`. We need to return the value at the specified index `idx` (which is zero indexed). Because the numbers can get incredibly large, we must return the result modulo `1000000007` (10^9 + 7).

**The Constraints**
We might handle up to `100000` total calls to these functions. This number is massive and was the primary reason my first few attempts failed.

## My First Failed Attempt The Naive Simulation

When I first opened this problem, I thought "This is easy! I will just use a dynamic array."

My logic was straightforward. I created a standard vector.
* When `append` came in, I pushed the value to the back.
* When `addAll` came in, I wrote a `for` loop to iterate through every element and add the value.
* When `multAll` came in, I wrote another `for` loop to multiply every element.

**Why I Failed**
I submitted the code and immediately got **Time Limit Exceeded**.
The reason became obvious after I looked at the constraints again. Imagine I have `100000` elements in my list. If the judge calls `addAll`, my computer has to perform `100000` operations. If the judge calls `multAll` ten times in a row, that is `1000000` operations. In the worst case, the total operations could reach ten billion (10^10). Most coding platforms can only handle about one hundred million (10^8) operations per second. My brute force approach was simply too slow.

## My Second Failed Attempt The Lazy Propagation

I tried to be smarter. I thought, "What if I use a Segment Tree or some form of lazy propagation?"

I tried to build a tree structure where I stored the updates at the nodes and only pushed them down when needed. This is a common technique for range update problems.

**Why I Failed**
While this approach is theoretically faster (logarithmic time), it was overkill for this specific problem. The memory usage was high, and the implementation was complex and prone to bugs. I found myself writing hundred lines of code for what should be a simpler task. I realized I was overengineering the solution. There had to be a cleaner mathematical way.

## How I Arrived at the Solution

I took a step back and grabbed a pen and paper. I decided to trace what happens to a single number as we apply operations.

Let us say we append a number `x`.
The sequence is `[x]`.

Now, imagine we call `addAll(3)`.
The value becomes `x + 3`.

Now, we call `multAll(2)`.
The value becomes `(x + 3) * 2`.
Distributing the 2, we get `x * 2 + 6`.

Now, we call `addAll(4)`.
The value becomes `(x * 2 + 6) + 4`.
Simplifying, we get `x * 2 + 10`.

**The Aha Moment**
I stared at that final expression: `x * 2 + 10`.
I realized that no matter how many additions or multiplications we perform, the value of any number in the sequence can always be expressed in the linear form:
`Final Value = (Initial Value * m) + c`

Here, `m` represents the cumulative multiplier, and `c` represents the cumulative increment.

This meant I did not need to update the array at all! I could just maintain two global variables, let us call them `GlobalMul` and `GlobalAdd`.
* `GlobalMul` starts at 1.
* `GlobalAdd` starts at 0.

When we call `addAll(inc)`, we just update `GlobalAdd`.
When we call `multAll(m)`, we update both `GlobalMul` and `GlobalAdd` because of the distributive property.

## The Mathematical Hurdle

This global strategy worked perfectly for numbers already in the list. But then I hit a wall with the `append` operation.

Suppose `GlobalMul` is `10` and `GlobalAdd` is `5`.
I now want to append the number `7`.
If I just store `7` in my list, later when I read it back, I will apply the global formula:
`Result = 7 * 10 + 5 = 75`.
But I want the result to be `7`!

This was the tricky part. I needed to store a "transformed" value. I needed to store a number `y` such that when the global formula is applied to it, it returns my original input `val`.

I wrote down the equation:
`(y * GlobalMul + GlobalAdd) = val`

Now I just needed to solve for `y`.
`y * GlobalMul = val - GlobalAdd`
`y = (val - GlobalAdd) / GlobalMul`

**The Division Problem**
In modular arithmetic (working with remainders), you cannot just divide. Division does not exist in the same way. Instead of dividing by `GlobalMul`, we must multiply by the **Modular Multiplicative Inverse** of `GlobalMul`.

So the formula for the value to store became:
`y = (val - GlobalAdd) * Inverse(GlobalMul)`

This was the final piece of the puzzle!

## Implementing the Solution

To implement this, I needed a way to calculate the Modular Inverse. Since the modulo `1000000007` is a prime number, I could use **Fermat's Little Theorem**. This theorem states that the inverse of `a` is `a` raised to the power of `MOD - 2`.

I wrote a helper function for modular exponentiation to calculate this power efficiently.

**The Bug That Almost Stopped Me**
I implemented the logic above, but I kept getting "Wrong Answer" on a specific test case. I was confused. The math seemed perfect.

After debugging for an hour, I found the culprit: **C++ Type Promotion**.

In my formula `(val - GlobalAdd)`, `val` was an integer and `GlobalAdd` was an unsigned long long.
When `GlobalAdd` was larger than `val`, the result should have been negative. But in C++, subtracting an unsigned number from an int results in an unsigned number! So instead of `-5`, I got a massive positive integer (like 18446744...). This massive number messed up my modulo calculation completely.

**The Fix**
I had to explicitly cast `GlobalAdd` to a signed `long long` before subtracting. This forced the operation to use signed arithmetic, preserving the negative value correctly.

## The Final Code

Here is the complete C++ solution, incorporating the global linear tracking and the modular inverse logic.

```cpp
#include <vector>

using namespace std;

class Fancy {
private:
    unsigned long long mul;
    unsigned long long add;
    vector<unsigned long long> sequence;
    const int MOD = 1e9 + 7;

    // Helper function to calculate (base raised to exp) % MOD
    // We use Binary Exponentiation for logarithmic efficiency
    unsigned long long power(unsigned long long base, unsigned long long exp) {
        unsigned long long res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }

    // Helper function to calculate Modular Inverse using Fermat's Little Theorem
    // Since MOD is prime, a^(MOD - 2) is the modular inverse of a
    unsigned long long modInverse(unsigned long long n) {
        return power(n, MOD - 2);
    }

public:
    Fancy() {
        // Initialize our global linear transformation variables
        // Initially, val = val * 1 + 0, so no change.
        mul = 1;
        add = 0;
    }
    
    void append(int val) {
        // We need to store 'y' such that:
        // (y * mul + add) % MOD = val
        // Therefore: y = (val - add) * inverse(mul)
        
        // Step 1: Calculate the difference (val - add)
        // CRITICAL: Cast to long long to perform signed subtraction!
        // If we do not cast, the result wraps to a huge positive number.
        long long diff = (static_cast<long long>(val) - static_cast<long long>(add)) % MOD;
        
        // Step 2: Handle negative results from modulo
        // In C++, -5 % 7 is -5, but we want 2.
        if (diff < 0) {
            diff += MOD;
        }
        
        // Step 3: Multiply by the inverse of the current multiplier
        unsigned long long normalized_val = (diff * modInverse(mul)) % MOD;
        
        // Step 4: Store this normalized value
        sequence.push_back(normalized_val);
    }
    
    void addAll(int inc) {
        // Update the global adder
        // The multiplier remains the same
        add = (add + inc) % MOD;
    }
    
    void multAll(int m) {
        // Update both global variables
        // New formula becomes: (old_val * m) + (old_add * m)
        mul = (mul * m) % MOD;
        add = (add * m) % MOD;
    }
    
    int getIndex(int idx) {
        // Check if index is valid
        if (idx >= sequence.size()) {
            return -1;
        }
        
        // Retrieve the stored normalized value
        // Apply the current global transformation to get the real value
        return (sequence[idx] * mul + add) % MOD;
    }
};

```

## Complexity Analysis

Let us look at why this solution works so well compared to my first attempt.

* **Time Complexity**
* `addAll` is now constant time `O(1)`. We just update one variable.
* `multAll` is now constant time `O(1)`. We just update two variables.
* `getIndex` is now constant time `O(1)`. We just do a quick math calculation.
* `append` takes logarithmic time `O(log MOD)`. This is because finding the modular inverse requires exponentiation. However, since the exponent is fixed (approx 10^9), this takes only about 30 tiny steps.


This is drastically faster than the naive approach. We went from billions of operations to just a handful!
* **Space Complexity**
* We use linear space `O(N)` to store the numbers in the vector. This is unavoidable as we must store the data somewhere.



## Closing Thoughts

This problem taught me a valuable lesson. When a simulation approach seems too slow, look for the math. By treating the sequence operations as a linear equation, we turned a complex data structure problem into a simple algebra problem.

The key takeaways I want you to remember are:

1. Identify if operations are linear (additions and multiplications).
2. Use Fermat's Little Theorem for division in modular arithmetic.
3. Always be careful with signed versus unsigned subtraction in C++.

I hope this walkthrough helps you crush this problem and others like it. 

Happy leet-coding everyone!
