# Rolling Backwards to Match Exact Substring Hash Values in C Plus Plus [#2156]

Hello everyone! Welcome back to another coding discussion. Today, I want to share a walkthrough for an incredibly fascinating string puzzle. If you enjoy string manipulation and modular arithmetic, you are in for a treat. We are going to look into a problem where we need to find a specific substring based on its polynomial hash value. 

This problem is a fantastic learning opportunity because it teaches us how to look at sliding windows from a completely different perspective. We often learn to slide windows from left to right, but this specific challenge invites us to reverse our thinking entirely. I will break down the problem, the math behind it using plain plain text, and provide a clean implementation in C Plus Plus. Grab a cup of coffee, and let us dive in together!

***

## Understanding the Problem

Let us start by clearly defining what we are trying to achieve. You are given a large string of lowercase English letters, and you need to find a contiguous block of characters inside it, which we call a substring. This substring must have a specific length, denoted by the variable `k`. 

However, we are not looking for a specific word. Instead, we are looking for a substring that produces a very specific mathematical result when passed through a hashing function. 

You are given the following variables
* `s` The large string of lowercase English letters.
* `power` A chosen integer used as the base for our polynomial hash.
* `modulo` A chosen integer used to keep our hash values within a certain limit, preventing integer overflow.
* `k` The exact length of the substring we want to find.
* `hashValue` The target hash number we want our substring to match.

The problem guarantees that a valid substring always exists, and our goal is to return the **first** substring of length `k` from the left that matches the given `hashValue`.

### How the Hash is Calculated

The hash function described in the problem is a standard polynomial rolling hash. For a substring of length `k`, the hash is calculated by multiplying the value of each character by an increasing power of our `power` variable, adding them all up, and then taking the remainder when divided by the `modulo`.

The value of a character, denoted as `val(c)`, is its alphabetical position. The letter 'a' has a value of 1, 'b' has a value of 2, all the way up to 'z' which has a value of 26.

If we have a substring spanning from index `0` to index `k minus 1`, the hash formula looks like this

`Hash = (val(s[0]) * power^0 + val(s[1]) * power^1 + ... + val(s[k minus 1]) * power^(k minus 1)) % modulo`

Notice how the power starts at zero for the first character and increases by one for each subsequent character. This specific ordering is the crux of the entire problem.

***

## The Pitfall of the Traditional Left to Right Approach

When dealing with substrings of a fixed length, the most common technique is the sliding window approach. You calculate the hash for the first window of length `k`, and then as you move the window one character to the right, you update the hash in constant time instead of recalculating it from scratch.

Let us imagine sliding from left to right. When the window moves one step forward
1. You need to subtract the value of the character that just left the window on the left side.
2. You need to divide the entire remaining hash by `power` because all the remaining characters have shifted one position to the left, meaning their associated power decreases by one.
3. You add the new character entering the window on the right side, multiplied by `power^(k minus 1)`.

Step 2 is where the nightmare begins. In modular arithmetic, division is not as simple as regular division. To divide by `power` under a modulo, you must multiply by the modular multiplicative inverse of `power`. 

The fatal flaw here is that a modular inverse only exists if the `power` and the `modulo` are coprime, meaning they share no common divisors other than 1. The problem description gives no guarantees that `power` and `modulo` are coprime. Therefore, calculating the modular inverse might be mathematically impossible for certain test cases, rendering the left to right sliding window completely broken.

***

## The Elegant Shift in Perspective Moving Right to Left

If dividing by `power` is potentially impossible, what if we never have to divide at all? 

Think about the opposite operation. If moving left to right requires division, then moving right to left should require multiplication! Multiplication under a modulo is always safe and always defined, regardless of whether the numbers are coprime.

Let us visualize sliding the window backwards, starting from the very end of the string and moving towards the beginning.

When we shift our window one position to the left
1. The entire existing window shifts to the right relative to the start of the new window. This means every character currently in our hash needs its power increased by one. We achieve this simply by multiplying our current hash by `power`.
2. A new character enters the window on the left side. Since it is at the very beginning of the new window, its power is zero. `power^0` is just 1. So, we simply add the alphabetical value of this new character to our hash.
3. The character that was at the far right of the old window now falls out of the new window. Because we multiplied the entire hash by `power` in step 1, this outgoing character is now effectively multiplied by `power^k`. To remove it, we subtract its alphabetical value multiplied by `power^k` from our hash.

By traversing the string in reverse, we completely avoid division. We only use multiplication, addition, and subtraction, all of which are perfectly safe under modular arithmetic! 

Additionally, because the problem asks for the **first** occurrence of the matching substring, traversing from right to left is incredibly convenient. Every time we find a matching hash, we just update our record of the best starting index. By the time we reach the beginning of the string, the last match we found will naturally be the earliest one in the string.

***

## Step by Step Algorithm Breakdown

Let us put this beautiful logic into a concrete plan.

**Step 1 Precompute the Power Factor**
We will frequently need to calculate the value of `power^k % modulo` to subtract the characters falling out of the right side of our window. To save time, we will calculate this value once at the very beginning using a simple loop.

**Step 2 Calculate the Initial Hash for the Last Window**
We go to the very end of our string `s` and isolate the last `k` characters. We calculate the polynomial hash for this specific window the slow way, just by iterating through its characters and applying the formula. This gives us our starting point.

**Step 3 Evaluate the Initial Hash**
We check if this very last window matches our target `hashValue`. If it does, we record its starting index.

**Step 4 Slide the Window Backwards**
We create a loop that starts just before the last window and walks backward to the beginning of the string. Inside this loop, we do three things
* Multiply the current hash by `power` and apply the modulo.
* Add the new character entering on the left, apply the modulo.
* Subtract the character falling out on the right multiplied by our precomputed `power^k`, and apply the modulo. 

*A quick note on C Plus Plus modulo arithmetic* When you subtract numbers, the result can become negative. In C Plus Plus, using the modulo operator on a negative number returns a negative remainder, which messes up our hashing logic. To fix this, whenever we subtract, we add the `modulo` to the result before applying the modulo operator again. This guarantees a positive result.

**Step 5 Update and Return**
Whenever our running hash equals the target `hashValue`, we update our `best_index` variable. Once the loop finishes, we simply extract the substring of length `k` starting at `best_index` and return it.

***

## A Detailed Practice Run

To make this completely clear, let us trace through Example 1 from the problem.

String `s` equals "leetcode"
`power` equals 7
`modulo` equals 20
`k` equals 2
`hashValue` equals 0

The length of "leetcode" is 8.
First, we find our `power^k % modulo`.
`k` is 2, so we need `7^2 % 20`.
7 times 7 is 49.
49 modulo 20 is 9.
Our removal factor is 9.

Now, we calculate the hash for the last window of length 2, which is "de" at the end of "leetcode".
'd' is the 4th letter, so its value is 4.
'e' is the 5th letter, so its value is 5.
Hash of "de" equals `(4 * 7^0 + 5 * 7^1) % 20`.
This is `(4 * 1 + 5 * 7) % 20`.
This is `(4 + 35) % 20`.
39 modulo 20 equals 19.
Our current hash is 19. This does not equal our target of 0.

Now we slide backwards! The next character to the left is 'o' (value 15). The string window is now "od". The character falling out is 'e' (value 5).
1. Multiply current hash by power: 19 * 7 equals 133. 133 % 20 equals 13.
2. Add new character 'o': 13 + 15 equals 28. 28 % 20 equals 8.
3. Subtract outgoing character 'e' times removal factor: The factor is 9. 'e' is 5. 5 * 9 equals 45. We subtract 45 from 8, getting negative 37. To handle the negative, we add 20 multiples until positive, or calculate `(8 - 45 % 20 + 20) % 20`. 45 % 20 is 5. 8 minus 5 is 3.
The hash for "od" is 3.

We continue this process. Let us skip ahead to when the window reaches "ee" at the beginning of "leetcode".
We will find that the hash evaluates to exactly 0. We record this index. Since we keep moving left, and there are no other matches, the index for "ee" remains our best index. We return "ee".

***

## The C Plus Plus Implementation

Here is the code structure that brings all of this logic to life. I have added comments to help map the code back to the steps we discussed.

```cpp
class Solution {
public:
    string subStrHash(string s, int power, int modulo, int k, int hashValue) {
        long long p = power;
        long long m = modulo;
        long long current_hash = 0;
        long long pk = 1;
        int n = s.length();
        int best_idx = -1;

        // A tiny helper to fetch the alphabetical value of a character
        auto val = [](char c) -> long long {
            return c - 'a' + 1;
        };

        // Step 1: Precompute the power factor for the outgoing character
        for (int i = 0; i < k; ++i) {
            pk = (pk * p) % m;
        }

        // Step 2: Calculate the initial hash for the very last window
        long long p_pow = 1;
        for (int i = n - k; i < n; ++i) {
            current_hash = (current_hash + val(s[i]) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }

        // Step 3: Check if the last window is a match
        if (current_hash == hashValue) {
            best_idx = n - k;
        }

        // Step 4: Slide the window backwards through the string
        for (int i = n - k - 1; i >= 0; --i) {
            
            // Multiply the running hash by the power base
            current_hash = (current_hash * p) % m;
            
            // Add the new character on the left
            current_hash = (current_hash + val(s[i])) % m;
            
            // Calculate what needs to be subtracted
            long long subtract = (val(s[i + k]) * pk) % m;
            
            // Carefully subtract, adding 'm' to prevent negative modulo results
            current_hash = (current_hash - subtract + m) % m;

            // Step 5: Update the best index if we find a match
            if (current_hash == hashValue) {
                best_idx = i;
            }
        }

        // Return the extracted string using our best recorded index
        return s.substr(best_idx, k);
    }
};

```

---

## Complexity Analysis

Let us talk about how efficient this approach is. It is always important to evaluate our resource usage.

**Time Complexity** `O(N)`
Where `N` represents the total length of the string `s`.
First, we run a loop of length `k` to find our power factor. Then we run another loop of length `k` to establish the hash of the last window. Finally, we loop `N minus K` times to slide our window backwards to the start of the string. Inside the sliding loop, all mathematical operations take constant time `O(1)`. Since `K` is always less than or equal to `N`, the overall time taken scales directly with the size of the string, giving us a clean linear time complexity. This easily passes all test cases within the required limits.

**Space Complexity** `O(1)`
We are keeping track of just a handful of long integer variables (`current_hash`, `pk`, `best_idx`, etc.). We do not create any large auxiliary data structures like arrays or maps that grow with the size of the input string. The only extra space generated is for the final substring that we return at the very end, which is standard. Therefore, our space complexity is strictly constant.

---

## Final Thoughts

I genuinely hope this walkthrough helped clarify the mechanics behind polynomial rolling hashes and why changing the direction of our sliding window can completely bypass complicated math issues like modular inverses. Sometimes, when you hit a wall moving forwards, the best solution is simply to put things in reverse!

If you have any questions about the math, the code, or just want to discuss other string algorithms, please feel free to share your thoughts. Keep practicing, keep experimenting, and happy coding to you all!
