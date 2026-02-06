# Master the Art of Constructing the Smallest Zero Free Number with Divisible Digit Products [#3348]

[Blog Post Link](https://leetcode.com/discuss/post/7556009/master-the-art-of-constructing-the-small-glgm/)


Hello coding community! 
Welcome back to another deep dive into the world of algorithmic problem solving. Today I am incredibly excited to share a walkthrough for a fascinating problem that really tests our ability to handle large numbers and prime factorizations. It is a "Hard" difficulty challenge that might look intimidating at first glance, but once we break it down, it becomes a beautiful exercise in greedy logic and number theory.

If you have ever struggled with problems involving digit products or constructing lexicographically smallest strings, this post is exactly for you. We are going to solve this without complex mathematical formulas, relying instead on logical steps and clean code.

### Understanding the Challenge

Let us look at what we are being asked to do.

We are given two inputs. First is a string called `num` which represents a very large positive integer. The length of this string can be up to 200,000 digits! This immediately tells us we cannot convert this string into a standard integer type like `int` or `long long` in C++. We must treat it as a string or an array of digits.

The second input is a long integer `t`.

Our goal is to find the **smallest** number that meets three specific criteria:

1. The number must be greater than or equal to `num`.
2. The number must not contain the digit '0' (it must be "zero free").
3. The product of all its digits must be divisible by `t`.

If no such number exists, we return "-1".

**Let us look at a quick example.**
Imagine `num` is "1234" and `t` is 256.
We need a number slightly bigger than 1234 where the digits multiply to a multiple of 256.
The answer turns out to be "1488".
Why? The digits are 1, 4, 8, 8.
Product = 1 * 4 * 8 * 8 = 256.
256 is divisible by 256.
It is the smallest valid number greater than 1234.

### The Key Insight Prime Factors

The first and most critical observation involves the nature of digits. We are constructing a number using only the digits 1 through 9.

Let us think about the prime factorization of these digits:

* 1 has no prime factors.
* 2 is a prime (2).
* 3 is a prime (3).
* 4 is 2 * 2.
* 5 is a prime (5).
* 6 is 2 * 3.
* 7 is a prime (7).
* 8 is 2 * 2 * 2.
* 9 is 3 * 3.

Notice something? The only prime factors we can ever generate by multiplying digits 1 to 9 are **2, 3, 5, and 7**.

**Crucial Deduction:**
If the target value `t` contains any prime factor other than 2, 3, 5, or 7 (like 11, 13, 17, etc.), it is absolutely impossible to form a number whose digit product is divisible by `t`. For example, no matter how many digits you multiply, you will never reach a number divisible by 11 because no single digit from 1 to 9 has 11 as a factor.

So, step one of our solution is simple. We factorize `t`. If `t` has any prime factor beyond 7, we immediately return "-1".

### The Strategy Greedy Construction

Since we want the **smallest** number greater than or equal to `num`, we should try to keep the new number as close to `num` as possible. This suggests a greedy approach.

Ideally, we want the new number to have the same length as `num`. We also want it to share the longest possible prefix with `num`.

We can iterate through the string `num` from right to left. Let us call the current index `i`. At each index `i`, we try to see if we can modify the digit at this position to make a valid number. Specifically, we try to replace `num[i]` with a larger digit `d` (where `d` is between `num[i] + 1` and 9).

If we increase the digit at index `i`, then the number is already guaranteed to be greater than `num`. This gives us total freedom for all the digits that come after index `i` (from `i+1` to the end). To make the number as small as possible, we should make the remaining digits in the suffix sorted in non descending order, with as many '1's as possible at the start of the suffix.

**The Algorithm in Plain English:**

1. **Check the impossible**: Factorize `t` into counts of 2s, 3s, 5s, and 7s. If anything else remains, return "-1".
2. **Check `num` itself**: If `num` contains no zeros and its digit product is already divisible by `t`, then `num` is the answer.
3. **Find the Pivot**: We look for a position `i` (from the back of the string to the front) and a new digit `d` (greater than the current `num[i]`) such that if we place `d` at `i`, we can fill the remaining positions to satisfy the divisibility requirement.
* **Special Note on Zeros**: Since we cannot have zeros, if `num` contains a '0', we **must** change the digit at the first '0' or somewhere before it. We cannot keep a prefix that includes a zero.


4. **Construct the Suffix**: Once we find a valid position `i` and a digit `d`, we calculate what prime factors are still missing. We then fill the remaining length with digits that provide these factors. To keep the number small, we pack the factors into the digits at the very end of the string (like 9s and 8s) and fill the gap with 1s.
5. **Fallback**: If we cannot find any solution with the same length as `num`, we try a solution with length `length of num + 1`. This solution will just be a '1' followed by the smallest sequence of digits that satisfy `t`.

### Deep Dive The Suffix Construction

This is the trickiest part. Suppose we are at index `i`, we placed a digit `d`, and we have `k` spots left to fill at the end of the string. We also know we still need to produce some factors (e.g., three 2s and one 5).

How do we construct the lexicographically smallest suffix?

We want small digits at the left (mostly 1s) and dense digits at the right.
We should prioritize packing factors into the largest possible digits to minimize the number of non 1 digits.

* Use 9s to cover pairs of 3s.
* Use 8s to cover triplets of 2s.
* Use 7s for prime 7.
* Use 6s for a 2 and a 3.
* Use 5s for prime 5.
* Use 4s for pairs of 2s.
* Use 3s and 2s for remaining factors.

After we figure out the digits needed (like "8", "5", "9"), we sort them. For example, if we need an 8 and a 5, the sorted order is "58". Then we pad the front with enough "1"s to match the required length.

### The C++ Implementation

Here is the complete solution implementing this logic. I have added detailed comments to help you follow along.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
    // We map the primes 2, 3, 5, 7 to indices 0, 1, 2, 3
    // This makes array access cleaner.
    int primes[4] = {2, 3, 5, 7};
    
    // digit_factors[d][k] stores how many times the k-th prime 
    // appears in digit d.
    // Example: digit_factors[8][0] would be 3 (because 8 = 2^3)
    int digit_factors[10][4];

    // Helper function to precompute factor counts for digits 0 to 9
    void init_factors() {
        for (int d = 0; d <= 9; ++d) {
            for (int k = 0; k < 4; ++k) digit_factors[d][k] = 0;
            if (d <= 1) continue;
            int temp = d;
            for (int k = 0; k < 4; ++k) {
                while (temp % primes[k] == 0) {
                    digit_factors[d][k]++;
                    temp /= primes[k];
                }
            }
        }
    }

    // This function calculates the minimum number of digits required
    // to satisfy the counts of prime factors r2, r3, r5, r7.
    // It is essentially a greedy packing check.
    int get_min_digits(long long r2, long long r3, long long r5, long long r7) {
        int cnt = 0;
        // 5 and 7 are prime digits, they consume 1 spot each strictly.
        cnt += r5 + r7; 
        
        long long t2 = r2, t3 = r3;
        
        // Pack pairs of 3s into 9s (since 9 = 3^2)
        cnt += t3 / 2;
        t3 %= 2;
        
        // Pack triplets of 2s into 8s (since 8 = 2^3)
        cnt += t2 / 3;
        t2 %= 3;
        
        // If we have a 2 and a 3 left, combine them into a 6.
        if (t3 == 1 && t2 >= 1) {
            cnt++;
            t3--;
            t2--;
        }
        
        // If a 3 is still left, it takes a digit (3).
        cnt += t3;
        
        // Remaining 2s: 
        // If 1 left -> digit 2. If 2 left -> digit 4. 
        // Both take 1 spot.
        cnt += (t2 + 1) / 2; 
        
        return cnt;
    }

    // This constructs the actual suffix string.
    // We build it using the largest digits first (9s, 8s, etc.) 
    // then reverse or sort at the end to make it smallest (1s first).
    string construct_suffix(int len, long long r2, long long r3, long long r5, long long r7) {
        string res = "";
        
        // Greedy packing logic again, but this time appending characters
        while (r3 >= 2) { res += '9'; r3 -= 2; }
        while (r2 >= 3) { res += '8'; r2 -= 3; }
        while (r7 > 0)  { res += '7'; r7--; }
        while (r3 >= 1 && r2 >= 1) { res += '6'; r3--; r2--; }
        while (r5 > 0)  { res += '5'; r5--; }
        while (r2 >= 2) { res += '4'; r2 -= 2; }
        while (r3 > 0)  { res += '3'; r3--; }
        while (r2 > 0)  { res += '2'; r2--; }
        
        // We fill the remaining length with '1's.
        // Because we want the smallest number, the '1's should act as padding.
        while (res.length() < len) {
            res += '1';
        }
        
        // Sorting puts the smaller digits (like 1, 2, 3) at the front
        // and dense digits (8, 9) at the back.
        sort(res.begin(), res.end());
        return res;
    }

public:
    string smallestNumber(string num, long long t) {
        // Step 1: Analyze t for validity
        long long req[4] = {0}; // Requirements for 2, 3, 5, 7
        long long temp_t = t;
        for (int i = 0; i < 4; ++i) {
            while (temp_t % primes[i] == 0) {
                req[i]++;
                temp_t /= primes[i];
            }
        }
        
        // If t has factors other than 2, 3, 5, 7, it is impossible.
        if (temp_t > 1) return "-1";

        init_factors();
        int n = num.length();

        // Step 2: Build Prefix Sums for the input number
        // This lets us quickly know the factors in num[0...i]
        vector<vector<int>> pref(n + 1, vector<int>(4, 0));
        int first_zero = -1;
        
        for (int i = 0; i < n; ++i) {
            if (num[i] == '0' && first_zero == -1) first_zero = i;
            for (int k = 0; k < 4; ++k) pref[i+1][k] = pref[i][k];
            if (num[i] != '0') {
                int d = num[i] - '0';
                for (int k = 0; k < 4; ++k) pref[i+1][k] += digit_factors[d][k];
            }
        }
        
        // Step 3: Check if num itself is valid
        if (first_zero == -1) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                if (pref[n][k] < req[k]) { ok = false; break; }
            }
            if (ok) return num;
        }

        // Step 4: Iterate backwards to find the Pivot point
        // If we saw a zero at index z, we can only pivot at indices <= z.
        // Any index after the first zero is unreachable because we cannot keep the zero.
        int start_i = (first_zero != -1) ? first_zero : n - 1;

        for (int i = start_i; i >= 0; --i) {
            // Try changing digit at i to something larger
            int current_digit = num[i] - '0';
            
            for (int d = current_digit + 1; d <= 9; ++d) {
                // Calculate what factors we still need if we fix the prefix
                // up to index i with digit d.
                long long rem_req[4];
                for(int k=0; k<4; ++k) {
                    long long needed = req[k];
                    // Subtract factors from prefix 0 to i-1
                    needed -= pref[i][k];
                    // Subtract factors from the new digit d
                    needed -= digit_factors[d][k];
                    rem_req[k] = max(0LL, needed);
                }
                
                // How many slots do we have left?
                int rem_len = n - 1 - i;
                
                // Can we fit the remaining required factors into the remaining length?
                int min_k = get_min_digits(rem_req[0], rem_req[1], rem_req[2], rem_req[3]);
                
                if (min_k <= rem_len) {
                    // Success! We found the split point.
                    string suffix = construct_suffix(rem_len, rem_req[0], rem_req[1], rem_req[2], rem_req[3]);
                    return num.substr(0, i) + to_string(d) + suffix;
                }
            }
        }

        // Step 5: If no solution of length n is found, go to length n+1 or more.
        // We construct the smallest number from scratch.
        string ans = construct_suffix(0, req[0], req[1], req[2], req[3]);
        int needed_len = ans.length();
        int final_len = max(n + 1, needed_len);
        
        // If the answer naturally requires more digits than n, we just return that.
        // Otherwise, we pad with 1s to reach length n+1.
        while (ans.length() < final_len) {
            ans = "1" + ans;
        }
        // Since construct_suffix sorts internally, we might need to re-sort or handle the '1's carefully.
        // But actually, construct_suffix logic:
        // 1. Pack dense digits.
        // 2. Fill rest with 1s.
        // 3. Sort.
        // If we need length > n, we just call construct_suffix with that larger length.
        
        return construct_suffix(final_len, req[0], req[1], req[2], req[3]);
    }
};

```

### Why This Logic Works

The beauty of this solution lies in how it handles the constraints.

1. **Efficiency**: The variable `num` can be 200,000 digits long. An  solution would time out. However, our main loop runs backwards from  down to 0. Inside the loop, we do a constant amount of work (checking digits 0 to 9, doing basic math). Thus, the complexity is roughly linear, .
2. **Greedy Correctness**: By searching from right to left, we ensure we keep the longest possible prefix of the original number. This guarantees the result is the "closest" valid number greater than `num`.
3. **Space**: We only store the prefix sums and a few auxiliary arrays, keeping memory usage very low.

### Final Thoughts

Problems like this remind us that brute force is rarely the answer when constraints are huge. By focusing on the properties of numbers (primes) and the structure of the output (lexicographical order), we transformed an impossible search space into a manageable linear scan.

I hope this walkthrough clarifies the mechanics behind solving "Smallest Divisible Digit Product II". It is a fantastic problem to practice greedy strategies and constructive algorithms.

Last but not least, note that this is not an optimal solution, and I saw there are brilliant guys (I really admire them) making it far more efficient and better than my solution does, so feel free to discuss and share your insightful comments below~ 

Let's happy leet-coding together, and see you in the next challenge!
