# A Friendly Guide to Splitting an Array to Ensure the Left and Right Parts Share No Prime Factors [#2584]

Hello everyone! Today I would like to share a walkthrough of an interesting array problem. It is a wonderful exercise in combining number theory with array traversal techniques. I hope this guide helps you understand the underlying concepts so you can apply them to similar challenges in the future.

### Understanding the Core Request

Imagine you are given a list of positive integers. Your goal is to find a specific spot to cut this list into two separate pieces. We will call the piece on the left the "prefix" and the piece on the right the "suffix". 

The condition for a successful cut is that the product of all the numbers in the prefix and the product of all the numbers in the suffix must be coprime. 

Two numbers are coprime if their greatest common divisor is exactly one. In simpler terms, they do not share any prime building blocks. For example, fourteen and fifteen are coprime. Fourteen is built from the prime factors two and seven. Fifteen is built from the prime factors three and five. Since their lists of prime building blocks have no overlap, they are coprime.



[Image of Prime Factorization Tree]


The problem asks us to find the earliest possible valid cut. If we have an array of length N, we can place our cut at any index from zero up to N minus two. The left part will contain everything up to and including our chosen index, and the right part will contain everything after it. If no such cut is possible, we should return a value of negative one.

### The Trap of Direct Multiplication

When we first read the problem, a natural instinct might be to literally multiply the numbers together. We could create a loop, multiply the left side, multiply the right side, and then calculate their greatest common divisor. 

However, numbers in our array can be as large as one million. If we multiply just a few of these numbers together, the result will quickly exceed the maximum capacity of standard integer variables in programming languages. Even if we use specialized large number libraries, multiplying thousands of large numbers and calculating their greatest common divisor would be incredibly slow. We need a more practical method.

### Shifting Our Perspective to Prime Factors

Instead of dealing with massive products, we can break the problem down to its fundamental components. The essence of the problem lies entirely in the prime factors of the numbers.

If a prime number exists in the left piece and that exact same prime number also exists in the right piece, then the product of the left piece and the product of the right piece will both be divisible by that prime number. Because they share a divisor greater than one, they cannot possibly be coprime.

This leads us to a beautiful realization. For any given prime number present in our array, all of its occurrences must belong entirely to the left piece OR entirely to the right piece. We cannot split the occurrences of a single prime number across the dividing line.

If we spot a prime number at a certain index, and we know this same prime number appears again further down the array at a later index, our cut cannot be placed between these two indices. The cut must be placed after the very last appearance of this prime number if we want the left side to safely contain all instances of it.

### Framing it as an Interval Problem

We can visualize this as a game of extending our reach. As we walk through the array from left to right, we examine the prime factors of the number at our current position. For every prime factor we see, we look up the furthest position in the entire array where this specific prime factor appears.



We maintain a variable that tracks the maximum boundary we are forced to reach. If we are currently at index two, and we see a prime factor that also appears at index seven, we are bound to include everything up to index seven in our left piece. We cannot make a cut before index seven. 

We keep marching forward. Along the way, we might encounter new prime factors that push our required boundary even further to the right. We update our maximum boundary whenever we find a prime factor that extends further out.

If we ever reach a point where our current index perfectly matches our maximum required boundary, we have found a clean break! It means every single prime factor we have encountered so far will never appear again in the rest of the array. This is our valid cut.

### The Tool We Need for Quick Factorization

To make this plan work, we need a fast way to find the prime factors of any number. A standard approach of dividing by all smaller numbers would be too slow when applied to every element in a large array.

This is where a brilliant historical algorithm comes to our rescue. We can use a modified version of the Sieve of Eratosthenes. 



Normally, the Sieve is used to find all prime numbers up to a certain limit. It works by creating a list of numbers and systematically crossing out the multiples of each prime it finds. 

We will alter this process slightly. Instead of just crossing numbers out, we will record the smallest prime factor for each number. We will create an array called the smallest prime factor array. Let us call it SPF for short. 

For example, the SPF of twenty eight would be two. We can then divide twenty eight by two to get fourteen. The SPF of fourteen is also two. We divide fourteen by two to get seven. The SPF of seven is seven. We divide seven by seven to get one, and we are done. We have efficiently extracted the prime factors two and seven.

By precomputing this SPF array once for all numbers up to one million, we can factorize any number in our array almost instantly.

### Breaking Down the Algorithm Step by Step

Let us put all the pieces together into a clear and actionable plan.

Step One involves preparing our Sieve. We create an array large enough to hold values up to one million. We initialize each index to hold its own value. Then, starting from two, if a number holds its own value, we recognize it as a prime. We iterate through all of its multiples and update their values in our array to be this prime number, provided they have not been updated by a smaller prime already. 

Step Two involves a preliminary scan of our input array. We need to know the final resting place of every prime factor. We create a dictionary or an array to store the last seen index for each prime. We loop through our input array from left to right. For each number, we use our Sieve to find all its prime factors. For each prime factor, we update its record in our tracker with the current index. By the end of this scan, we know exactly how far right every prime factor stretches.

Step Three is the final traversal to find the split. We initialize a variable to keep track of our furthest required reach. We loop through the array again, stopping exactly one element before the end because a cut must leave at least one element on the right. 

For the number at our current index, we extract its prime factors. For each factor, we check its last seen index from our records. We update our furthest required reach to be the maximum of its current value and this newly found last seen index. 

After checking all factors for the current number, we ask a crucial question. Is our current index equal to our furthest required reach? If the answer is yes, we have successfully enclosed a complete set of prime factors. We immediately return this current index as our valid split point.

If we finish checking all possible cut points and never find a match, it means the entire array is tangled together by shared prime factors. In this scenario, we return a value of negative one to indicate failure.

### A Concrete Example Walkthrough

Let us trace through an array containing the numbers four, seven, eight, fifteen, three, and five.

First we find the last occurrence of each prime factor.
The number four has the prime factor two.
The number seven has the prime factor seven.
The number eight has the prime factor two.
The number fifteen has prime factors three and five.
The number three has the prime factor three.
The number five has the prime factor five.

Looking at the whole picture, the prime factor two last appears at index two inside the number eight.
The prime factor seven last appears at index one inside the number seven.
The prime factor three last appears at index four inside the number three.
The prime factor five last appears at index five inside the number five.

Now we start our journey to find the cut.
At index zero, we have the number four. Its prime factor is two. The last time we see two is at index two. Our maximum reach becomes two. Since our current index zero is not equal to our maximum reach two, we cannot cut here.

At index one, we have the number seven. Its prime factor is seven. The last time we see seven is at index one. Our maximum reach remains two because two is greater than one. Since our current index one is not equal to our maximum reach two, we cannot cut here.

At index two, we have the number eight. Its prime factor is two. The last time we see two is at index two. Our maximum reach remains two. Now, our current index is two, and our maximum reach is also two! 

This means all the prime factors we have collected so far, which are two and seven, will never appear again in the rest of the array. The prefix contains four, seven, and eight. The suffix contains fifteen, three, and five. The prefix prime factors are two and seven. The suffix prime factors are three and five. They are perfectly coprime. We return index two.

### The C++ Implementation

Here is a practical way to write this logic in C++. I have statically allocated the sieve array so that if the system tests multiple test cases sequentially, the sieve is only built once. This is a nice little adjustment to save processing time.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findValidSplit(vector<int>& nums) {
        
        static int smallestPrimeFactor[1000005];
        static bool isSieveBuilt = false;
        
        if (!isSieveBuilt) {
            for (int i = 2; i <= 1000000; i++) {
                smallestPrimeFactor[i] = i;
            }
            for (int i = 2; i * i <= 1000000; i++) {
                if (smallestPrimeFactor[i] == i) {
                    for (int j = i * i; j <= 1000000; j += i) {
                        if (smallestPrimeFactor[j] == j) {
                            smallestPrimeFactor[j] = i;
                        }
                    }
                }
            }
            isSieveBuilt = true;
        }

        int arrayLength = nums.size();
        
        vector<int> lastSeenIndex(1000005, 0); 
        
        for (int i = 0; i < arrayLength; ++i) {
            int currentNumber = nums[i];
            while (currentNumber > 1) {
                int prime = smallestPrimeFactor[currentNumber];
                lastSeenIndex[prime] = i;
                while (currentNumber % prime == 0) {
                    currentNumber /= prime;
                }
            }
        }

        int maximumRequiredReach = 0;
        
        for (int i = 0; i < arrayLength - 1; ++i) {
            int currentNumber = nums[i];
            while (currentNumber > 1) {
                int prime = smallestPrimeFactor[currentNumber];
                maximumRequiredReach = max(maximumRequiredReach, lastSeenIndex[prime]);
                while (currentNumber % prime == 0) {
                    currentNumber /= prime;
                }
            }
            
            if (maximumRequiredReach == i) {
                return i;
            }
        }

        return -1;
    }
};

```

### Analyzing the Resources Used

It is always good practice to understand the footprint of our code.

For the time complexity, building the Sieve of Eratosthenes takes time proportional to the maximum value multiplied by the logarithm of the logarithm of the maximum value. Since our maximum value is one million, this is very fast.

After the sieve is built, we loop through our input array twice. For each number, we divide it by its prime factors. The number of prime factors a number can have is quite small. Therefore, extracting the factors takes logarithmic time relative to the value of the number. The total time spent traversing the array is roughly proportional to the number of elements in the array multiplied by the logarithm of the maximum value. This provides a very comfortable execution speed.

For the space complexity, we are storing the Sieve array and the tracker array. Both of these are sized based on the maximum possible value in the array, which is one million. Storing a few arrays of size one million takes just a few megabytes of memory. This space requirement is constant regardless of how many numbers are actually in the input array. It is a reasonable trade to make for the massive increase in speed.

### Final Thoughts

This problem beautifully demonstrates how breaking things down to their mathematical fundamentals can turn a seemingly impossible calculation into a very manageable logic puzzle. By treating prime factors as overlapping regions that we must completely envelop, we arrive at a clean and intuitive solution.

I hope this breakdown was helpful and easy to follow. 

Happy coding, and keep exploring those fascinating patterns in your daily practice!
