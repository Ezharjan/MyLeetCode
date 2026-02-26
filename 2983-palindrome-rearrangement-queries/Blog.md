# Demystifying the Palindrome Rearrangement Queries Puzzle [#2983]

Hello everyone! Welcome to my latest blog post. Today I would like to share my journey and thoughts on solving a very fascinating string manipulation puzzle. If you enjoy breaking down complex queries and working with prefix sums, this walkthrough is right up your alley. 

In this post, we will explore a structural approach to solving the Palindrome Rearrangement Queries problem. My goal is to be as detailed and friendly as possible so that everyone, from beginners to seasoned coders, can follow along comfortably. We will look at the underlying logic, visualize the overlapping intervals, and build a reliable solution in C++ together.



### Understanding the Problem Requirements

Let us first get comfortable with what the puzzle asks of us. We are given a zero indexed string of even length. Let us call its length N. We are also given a list of queries. Every query provides us with four indices, typically named a, b, c, and d. 

The first two indices, a and b, dictate a range in the first half of the string. The latter two indices, c and d, dictate a range in the second half of the string. For every single query, we are allowed to rearrange the characters within the range from a to b, and we are also allowed to rearrange the characters within the range from c to d. 

The ultimate question for each query is simple but profound. Is it possible to rearrange those specific substrings such that the entire string becomes a perfect palindrome? 

A palindrome reads the exact same forwards and backwards. Every query is completely independent, meaning the string resets to its original state after we answer a query. We need to return an array of boolean values representing the answers to these queries.

### The Core Strategy Folding the String

When checking for a palindrome, our natural instinct is to place two pointers at opposite ends of the string and move them toward the center. However, dealing with rearrangements on two different sides of a string simultaneously can get very confusing very quickly. 

To simplify our lives, we can cut the string exactly in half. Let us take the first half and call it the left string. Then, let us take the second half, reverse it entirely, and call it the right string. 

Why do we do this? Because a palindrome is simply a string where the first half is identical to the reversed second half! By reversing the second half, our goal shifts from a complex two pointer palindrome check to a straightforward string equality check. We just need to find out if we can make the left string exactly equal to the right string.

### Mapping the Indices

Since we reversed the second half of the string, the indices c and d no longer point to the same relative characters. We need to map them to our new reversed right string. 

If the total length of the original string is N, the second half starts at N divided by 2. When we reverse this half, the original index d becomes the new starting index, and the original index c becomes the new ending index. 

To find the exact mirrored index for any position in the second half, we subtract that position from N, and then subtract 1 again. 
Therefore, our new range in the right string will be from an index we can call a2 to an index we can call b2. 
The mapped index a2 is N minus 1 minus d. 
The mapped index b2 is N minus 1 minus c.

Now, every query gives us a target range in the left string from a to b, and a target range in the right string from a2 to b2. Our puzzle is now unified. Can we rearrange the left string between a and b, and the right string between a2 and b2, so that the two strings match perfectly?

### Identifying the Scenarios

With both ranges mapped to the same coordinate system, we can compare their positions. To make things orderly, we can always ensure that the first range starts before or at the exact same time as the second range. If it does not, we can simply swap our view of the left and right strings for that specific query.

Once ordered, we face two distinct scenarios.



* **Scenario One Disjoint Intervals** In this scenario, the first range ends before the second range begins. They do not overlap at all. Because they are completely separate, they do not influence each other. For the strings to match, several conditions must be met perfectly. 
    First, any characters outside of these two ranges must already be identical. We cannot change them, so if there is a mismatch there, the answer is immediately false.
    Second, within the first range, the left string characters are scrambled, but the right string characters are locked. Therefore, the pool of available characters in the left string must perfectly match the exact character frequencies of the fixed right string in that range.
    Third, within the second range, the right string is scrambled while the left string is locked. The pool of available characters in the right string must match the frequencies of the locked left string.

* **Scenario Two Overlapping Intervals** Here, the first range and the second range overlap, or one is completely swallowed by the other. This creates a large continuous zone where we have the power to rearrange characters in both strings. 
    Just like before, the sections completely outside this massive combined zone must already match perfectly. 
    Within the combined zone, the total frequency of every single letter in the left string must equal the total frequency of every single letter in the right string. 
    Furthermore, we have to look at the unchangeable edges. If the right string has a locked portion at the beginning of the combined zone, the left string must possess enough of the correct characters in its available pool to satisfy that locked portion. The same logic applies to the end of the combined zone.

### The Power of Prefix Sums

To answer potentially hundreds of thousands of queries efficiently, we absolutely cannot scan the strings or count characters one by one for every query. This would cause a Time Limit Exceeded error. We need a way to instantly know the character counts and mismatch counts for any given range.

This is where prefix sums come to the rescue! 



We will create arrays that keep a running total of information as we walk through the strings from the beginning to the middle. 
We need three main prefix sum structures.
First, a running total of how many mismatched characters exist between the left string and the right string up to a certain point.
Second, a running total of the frequencies of all twenty six lowercase English letters for the left string.
Third, a running total of the frequencies of all twenty six lowercase English letters for the right string.

By building these running totals once at the very beginning, we can find the exact data for any arbitrary range in constant O(1) time. We just take the total at the end of our desired range and subtract the total from just before the range starts. 

### Writing the Code Together

Let us translate these concepts into clean, understandable C++ code. We will build helper functions to make the logic readable.

```cpp
#include <vector>
#include <string>
#include <algorithm>
#include <array>

using namespace std;

class Solution {
public:
    vector<bool> canMakePalindromeQueries(string s, vector<vector<int>>& queries) {
        int n = s.length();
        int m = n / 2;
        
        string s1 = s.substr(0, m);
        string s2 = s.substr(m, m);
        reverse(s2.begin(), s2.end());
        
        vector<int> pref_diff(m + 1, 0);
        vector<array<int, 26>> pref1(m + 1);
        vector<array<int, 26>> pref2(m + 1);
        
        for (int j = 0; j < 26; ++j) {
            pref1[0][j] = 0;
            pref2[0][j] = 0;
        }
        
        for (int i = 0; i < m; ++i) {
            pref_diff[i + 1] = pref_diff[i] + (s1[i] != s2[i] ? 1 : 0);
            pref1[i + 1] = pref1[i];
            pref2[i + 1] = pref2[i];
            pref1[i + 1][s1[i] - 'a']++;
            pref2[i + 1][s2[i] - 'a']++;
        }
        
        auto mismatches = [&](int L, int R) {
            if (L > R) return 0;
            return pref_diff[R + 1] - pref_diff[L];
        };
        
        auto get_cnt = [](int L, int R, const vector<array<int, 26>>& pref) {
            array<int, 26> res{};
            if (L > R) return res;
            for (int i = 0; i < 26; ++i) {
                res[i] = pref[R + 1][i] - pref[L][i];
            }
            return res;
        };
        
        auto sub_cnt = [](const array<int, 26>& sub, const array<int, 26>& sup) {
            for (int i = 0; i < 26; ++i) {
                if (sub[i] > sup[i]) return false;
            }
            return true;
        };
        
        auto eq_cnt = [](const array<int, 26>& c1, const array<int, 26>& c2) {
            for (int i = 0; i < 26; ++i) {
                if (c1[i] != c2[i]) return false;
            }
            return true;
        };
        
        auto solve = [&](int a1, int b1, int a2, int b2, 
                         const vector<array<int, 26>>& p1, 
                         const vector<array<int, 26>>& p2) {
            
            if (b1 < a2) { 
                if (mismatches(0, a1 - 1) > 0) return false;
                if (mismatches(b1 + 1, a2 - 1) > 0) return false;
                if (mismatches(b2 + 1, m - 1) > 0) return false;
                
                array<int, 26> c1_1 = get_cnt(a1, b1, p1);
                array<int, 26> c2_1 = get_cnt(a1, b1, p2);
                if (!eq_cnt(c1_1, c2_1)) return false;
                
                array<int, 26> c1_2 = get_cnt(a2, b2, p1);
                array<int, 26> c2_2 = get_cnt(a2, b2, p2);
                if (!eq_cnt(c1_2, c2_2)) return false;
                
                return true;
            } else { 
                int R = max(b1, b2);
                
                if (mismatches(0, a1 - 1) > 0) return false;
                if (mismatches(R + 1, m - 1) > 0) return false;
                
                array<int, 26> c1_all = get_cnt(a1, R, p1);
                array<int, 26> c2_all = get_cnt(a1, R, p2);
                if (!eq_cnt(c1_all, c2_all)) return false;
                
                array<int, 26> req2_left = get_cnt(a1, a2 - 1, p2);
                array<int, 26> avail1 = get_cnt(a1, b1, p1);
                if (!sub_cnt(req2_left, avail1)) return false;
                
                if (b1 < b2) {
                    array<int, 26> req1_right = get_cnt(b1 + 1, b2, p1);
                    array<int, 26> avail2 = get_cnt(a2, b2, p2);
                    if (!sub_cnt(req1_right, avail2)) return false;
                }
                
                return true;
            }
        };
        
        vector<bool> ans;
        ans.reserve(queries.size());
        
        for (const auto& q : queries) {
            int a1 = q[0], b1 = q[1];
            int a2 = n - 1 - q[3];
            int b2 = n - 1 - q[2];
            
            if (a1 <= a2) {
                ans.push_back(solve(a1, b1, a2, b2, pref1, pref2));
            } else {
                ans.push_back(solve(a2, b2, a1, b1, pref2, pref1));
            }
        }
        
        return ans;
    }
};

```

### Breaking Down the Helper Functions

In the code block above, I structured the solution using small, single purpose lambda functions. This keeps our main logic clean and prevents us from repeating ourselves. Let us walk through what each helper accomplishes.

* **mismatches** This function simply tells us if there are any existing mismatches between the two strings in a specific locked zone. It takes the start and end of the zone, looks at our running totals, and returns the exact number of differences. If this number is greater than zero, we know the strings cannot match.
* **get_cnt** This function acts as our character counter. We give it a range and specify which string we are looking at by passing in the correct prefix sum array. It returns a neat package containing the exact frequencies of all twenty six letters within that range.
* **eq_cnt** When we have two completely separated zones that we are rearranging, the available characters in one string must perfectly match the locked characters in the other string. This function compares two character frequency packages to ensure they are exactly identical.
* **sub_cnt** This is vital for our overlapping scenario. Sometimes, a locked portion of one string needs to be built from the available pool of the other string. This function checks if our available pool has at least the required amount of every single character needed.

### Complexity Analysis

As programmers, we always want to be mindful of how our solutions scale with larger inputs. Let us break down the time and space complexity in simple terms.

For Time Complexity, the initial preparation phase takes O(N) time. We iterate through half of the string exactly once to build our prefix sum arrays. During this iteration, we do a constant amount of work for the twenty six alphabet characters. After the setup is complete, we process the queries. Thanks to our prefix sums, evaluating each query takes a constant amount of time, giving us an O(1) operation per query. Therefore, for a total of Q queries, the overall time complexity gracefully scales to O(N + Q). This is highly performant and handles the upper constraints easily.

For Space Complexity, we allocate a few arrays to store our running totals. These arrays scale directly with the length of the string, meaning we use O(N) extra space. The constants are very manageable, making this a reliable approach for practical memory limits.

### Final Thoughts and Takeaways

Working on string matching and interval problems requires a lot of careful bookkeeping. By folding the string over on itself, we managed to simplify the mental model drastically. Converting the reversed indices into a single unified coordinate space allowed us to categorize our approach into two very manageable scenarios disjoint intervals and overlapping intervals.

I hope this breakdown was helpful and illuminated the path toward solving this problem. Writing out the logic and building the prefix tools really solidifies our understanding of these core computer science concepts. I strongly encourage you to copy the markdown block, run the code in your own IDE, and play around with the indices to see how the logic handles different edge cases.

Thank you so much for reading along with my self sharing journey today. Keep coding, stay curious, and see you in the next puzzle breakdown!
