# Cracking the Code of the Largest Divisible Palindromes [#3260]

[Blog Post Link](https://leetcode.com/discuss/post/7561525/cracking-the-code-of-the-largest-divisib-g137/)

Hello, coding friends! Today I want to share a fascinating journey through a problem that combines string manipulation, number theory, and greedy logic. If you have ever stared at a problem involving massive numbers and wondered how to handle them without overflowing your integer types, this is the perfect challenge for you.

We are going to explore how to construct the largest possible palindrome with a specific number of digits that is also divisible by a single digit number. It sounds specific, but the techniques we use here are applicable to so many other algorithmic challenges. Let's dive in!

## Understanding the Challenge

The problem asks us to perform a very specific task. We are given two numbers, **n** and **k**.

* **n** represents the number of digits our target number must have.
* **k** is a divisor, ranging from 1 to 9.

Our goal is to find the **largest** integer that satisfies three conditions:

1. It has exactly **n** digits.
2. It is a **palindrome** (it reads the same forwards and backwards).
3. It is **divisible by k**.

### Why This Problem is Tricky

At first glance, you might be tempted to simply start at the largest n digit number (which is a string of n nines) and count down until you find a palindrome that is divisible by k.

For very small values of n, like 3 or 4, this works fine. But look at the constraints! **n** can be as large as **100,000**. A number with 100,000 digits is unimaginably huge. It is far larger than the number of atoms in the universe. We cannot store this in a standard integer variable (like `long long` in C++), and we certainly cannot iterate through numbers of this size.

This means we have to stop thinking about this as a *math* problem where we perform division, and start thinking about it as a *construction* problem where we build a string character by character.

## The General Strategy: Think Greedy

Since we want the **largest** possible palindrome, we should apply a "greedy" approach. This means we want the digits with the highest place value (the ones on the far left) to be as large as possible. Ideally, we want our number to look like `999...999`.

However, we have constraints. The number must be a palindrome, so whatever we do to the left side, we must mirror on the right side. And it must be divisible by **k**.

Our strategy will be:

1. Start with the ideal candidate: a string of all '9's.
2. Adjust the digits only when necessary to satisfy the divisibility rule for **k**.
3. When we must adjust digits, we prefer to adjust the digits with the **lowest** place value impact. For a palindrome, changing the outer digits hurts the "largeness" of the number the most. Changing the middle digits hurts it the least.

Let us break this down case by case for every value of **k** from 1 to 9.

---

## Case 1: The Easy Divisors (k = 1, 3, 9)

Let us start with the simplest cases.

**If k is 1:**
Any integer is divisible by 1. Therefore, the largest n digit palindrome is simply n nines.

* Example (n=5, k=1): `99999`

**If k is 3:**
A number is divisible by 3 if the sum of its digits is divisible by 3. If we have a string of all nines, the sum is `9 * n`. Since 9 is a multiple of 3, `9 * n` is always a multiple of 3. So, n nines works perfectly.

* Example (n=5, k=3): `99999` (Sum is 45, which is divisible by 3)

**If k is 9:**
Similar to 3, a number is divisible by 9 if the sum of its digits is divisible by 9. Again, `9 * n` is always divisible by 9.

* Example (n=5, k=9): `99999`

**Conclusion for 1, 3, 9:** Return a string of all '9's.

---

## Case 2: The Last Digit Rules (k = 2, 5)

Now we look at divisors that depend entirely on the last digit.

**If k is 2:**
A number is divisible by 2 if it is even. This means the last digit must be 0, 2, 4, 6, or 8. Since we want the largest palindrome, we want the starting digit to be as large as possible. Because it is a palindrome, the first digit must match the last digit.
To maximize the number, we pick the largest valid last digit, which is **8**.

* We set the first and last digits to 8.
* We leave all the inner digits as 9.
* Example (n=5, k=2): `89998`

**If k is 5:**
A number is divisible by 5 if it ends in 0 or 5.

* If we end in 0, the first digit must also be 0 (because it is a palindrome). But the problem says "no leading zeros".
* Therefore, the number must end in 5. Consequently, it must start with 5.
* We leave the inner digits as 9 to maximize value.
* Example (n=5, k=5): `59995`

---

## Case 3: The Last Block Rules (k = 4, 8)

For these numbers, we need to check more than just the last digit.

**If k is 4:**
A number is divisible by 4 if the number formed by its last **two** digits is divisible by 4.
We want the number to look like `99...99xx`.
Since it is a palindrome, it starts with the same two digits `xx...`. To maximize the number, we want `xx` to be large.
Let us look at the 90s: 99, 98, 97...
The largest number in the 80s or 90s that is divisible by 4 is 88. (96, 92 are div by 4, but if we end in 96, we must start with 69, which is much smaller than starting with 88).
So, we set the first two and last two digits to **8**.

* Example (n=5, k=4): `88988`
* Note: If n is small (like 1, 2, or 3), we just fill the available slots with 8s.

**If k is 8:**
A number is divisible by 8 if the number formed by its last **three** digits is divisible by 8.
We want the number to start with high digits, so we look for a sequence like `888`.
Is `888` divisible by 8? Yes.
So we set the first three and last three digits to **8**.

* Example (n=6, k=8): `888888`
* Example (n=5, k=8): `88988` (Wait, for n=5, the palindrome `88988` ends in `988`. `988 / 8` is not an integer. We actually need the end to be `888`. So for n=5, it is `88888`).
* Actually, the simplest logic for k=8 is ensuring the first 3 and last 3 digits are 8.
* Correct Logic: Set indices 0, 1, 2 and n minus 1, n minus 2, n minus 3 to '8'.

---

## Case 4: The Composite (k = 6)

The number 6 is `2 * 3`. This means our number must satisfy two rules:

1. **Rule of 2:** It must be even. To be the largest palindrome, it should start and end with **8** (just like the k=2 case).
2. **Rule of 3:** The sum of all digits must be divisible by 3.

So, our number looks like `899....998`.
Let us calculate the sum of digits for this template.
The two 8s contribute 16. The rest are 9s.
Total Sum = 16 plus (number of nines multiplied by 9).
Since any number of 9s is divisible by 3, the "remainder" of the sum comes entirely from the 16.
16 modulo 3 is **1**.
This means our current sum is "1 too high" (or "2 too low") to be divisible by 3.

We need to reduce the sum of the digits by 1 (or 4, or 7, etc.) so that the total becomes divisible by 3.
To keep the number as large as possible, we should modify the **middle** digit(s).

* **If n is odd:** There is one middle digit. It is currently a 9. We need to reduce the total sum by 1. We change the 9 to an **8**.
* Result: `8...8...8`


* **If n is even:** There are two middle digits. They are both 9s. We need to reduce the sum by 1. We cannot generate a sum reduction of 1 by changing two integers symmetrically (e.g. changing 9 to 8 twice reduces sum by 2). So we reduce the sum by **4**.
* Changing both middle 9s to 7s reduces sum by `(9 minus 7) * 2 = 4`.
* Since 4 modulo 3 is 1, this removes the extra "1" remainder we had!
* Result: `8...77...8`



---

## Case 5: The Challenge (k = 7)

This is the hardest part. The divisibility rule for 7 is not simple. We need to use modular arithmetic.

We start with our greedy guess: all '9's.
`999...999`
We check the remainder of this number when divided by 7.
If the remainder is 0, great! We are done.

If not, we need to change the middle digit (if n is odd) or the middle two digits (if n is even) to fix the remainder. Why the middle? Because changing the middle impacts the magnitude of the number the least.

**The Algorithm for 7:**

1. Construct a string of all 9s.
2. Calculate the `remainder` of this huge number modulo 7.
3. We want to find a digit `d` (to replace the middle 9) such that the new number becomes divisible by 7.

Let us say the position of the middle digit is `p` (counting from the right, 0 indexed). The value of that digit contributes `9 * 10^p` to the total value.
If we change that digit from 9 to `d`, we change the total value by `(d minus 9) * 10^p`.
We iterate `d` from 9 down to 0. We check if `(original_number_remainder + change) modulo 7` equals 0.
The first `d` that satisfies this is our answer.

**Note for Even n:**
We change both middle digits. If their positions are `p` and `p+1`, the change in value is `(d minus 9) * 10^p + (d minus 9) * 10^(p+1)`. We factor this out to `(d minus 9) * (10^p + 10^(p+1))`.

This allows us to solve the k=7 case efficiently without big integer libraries!

---

## The Code Solution

Here is the C++ code that implements this logic. I have kept it clean and standard so it is easy to read.

```cpp
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:
    string largestPalindrome(int n, int k) {
        // Start with the largest possible base: all 9s
        string res(n, '9');

        // Case 1, 3, 9: Already optimal with all 9s
        if (k == 1 || k == 3 || k == 9) {
            return res;
        }

        // Case 2: Must end in even. Max digit is 8.
        if (k == 2) {
            res[0] = '8';
            res[n - 1] = '8';
            return res;
        }

        // Case 5: Must end in 0 or 5. Max is 5.
        if (k == 5) {
            res[0] = '5';
            res[n - 1] = '5';
            return res;
        }

        // Case 4: Last 2 digits div by 4. Max starts/ends with 88.
        if (k == 4) {
            res[0] = '8';
            res[n - 1] = '8';
            if (n > 2) {
                res[1] = '8';
                res[n - 2] = '8';
            }
            return res;
        }

        // Case 8: Last 3 digits div by 8. Max starts/ends with 888.
        if (k == 8) {
            res[0] = '8';
            res[n - 1] = '8';
            if (n > 2) {
                res[1] = '8';
                res[n - 2] = '8';
            }
            if (n > 4) {
                res[2] = '8';
                res[n - 3] = '8';
            }
            return res;
        }

        // Case 6: Even (ends in 8) AND sum divisible by 3.
        if (k == 6) {
            if (n == 1) return "6";
            if (n == 2) return "66";
            
            // Set boundaries to 8
            res[0] = '8';
            res[n - 1] = '8';
            
            // Sum of 8...8 is 16 + 9*(n-2).
            // 16 % 3 = 1. We have an excess of 1.
            // We need to reduce the sum by 1 (mod 3).
            
            if (n % 2 == 1) {
                // Odd length: Change 1 middle digit.
                // Changing 9 to 8 reduces sum by 1.
                res[n / 2] = '8';
            } else {
                // Even length: Change 2 middle digits.
                // Changing 99 to 77 reduces sum by 4.
                // 4 % 3 is 1. This cancels the excess.
                res[n / 2 - 1] = '7';
                res[n / 2] = '7';
            }
            return res;
        }

        // Case 7: The Modulo Logic
        if (k == 7) {
            if (n == 1) return "7";
            if (n == 2) return "77";
            
            // 1. Calculate the remainder of the all-9 string
            int remainder = 0;
            // We can iterate the string to compute mod
            for (char c : res) {
                remainder = (remainder * 10 + (c - '0')) % 7;
            }
            
            if (remainder == 0) return res;
            
            // 2. Adjust the middle digit(s)
            // Precomputed powers of 10 mod 7: 1, 3, 2, 6, 4, 5...
            // We compute the specific power of 10 for the middle position
            int mid = n / 2;
            
            // Helper to get power of 10 mod 7 efficiently
            auto getMod = [&](int pos) {
                int mods[] = {1, 3, 2, 6, 4, 5};
                return mods[pos % 6];
            };

            if (n % 2 == 1) {
                // Odd length: one middle digit at index 'mid'
                // The place value is 10^(n - 1 - mid)
                int powerOf10 = getMod(n - 1 - mid);
                
                // Try changing the middle digit from 9 downwards
                for (int d = 9; d >= 0; --d) {
                    int currentDigitVal = 9; 
                    // Calculate how the remainder changes
                    // NewRem = (OldRem - (9 * power) + (d * power)) % 7
                    // Simplified: NewRem = (OldRem + (d - 9) * power) % 7
                    int diff = (d - 9); 
                    int change = (diff * powerOf10) % 7;
                    int newRem = (remainder + change) % 7;
                    if (newRem < 0) newRem += 7;
                    
                    if (newRem == 0) {
                        res[mid] = d + '0';
                        return res;
                    }
                }
            } else {
                // Even length: two middle digits at mid-1 and mid
                int power1 = getMod(n - 1 - (mid - 1));
                int power2 = getMod(n - 1 - mid);
                int totalPower = (power1 + power2) % 7;
                
                for (int d = 9; d >= 0; --d) {
                    int diff = (d - 9);
                    int change = (diff * totalPower) % 7;
                    int newRem = (remainder + change) % 7;
                    if (newRem < 0) newRem += 7;
                    
                    if (newRem == 0) {
                        res[mid - 1] = d + '0';
                        res[mid] = d + '0';
                        return res;
                    }
                }
            }
        }

        return res;
    }
};

```

## Wrapping Up

This problem is a fantastic exercise in breaking down constraints. Instead of trying to find a "one size fits all" mathematical formula, we looked at the specific properties of each divisor.

We learned that:

* Greedy construction works best for large numbers.
* We prioritize the "edges" for divisors like 2, 4, 5, 8.
* We prioritize the "center" for divisors like 3, 6, 7 to keep the value high.
* Modulo arithmetic is our best friend when dealing with strings as numbers.

I hope this walkthrough helps you feel more confident tackling these types of problems. Happy coding, and keep learning!
