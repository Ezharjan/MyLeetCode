# Mastering String Reductions and Avoiding Common Pitfalls [#3816]

Welcome friends! Today I want to invite you on a little journey I took recently while solving a fascinating string manipulation puzzle. If you enjoy organizing things or optimizing sequences, you are going to love this challenge. It involves a mix of greedy decision making and a classic data structure, but it also has a hidden trap that caught me off guard! I want to share my entire thought process, including the mistake I made, so you can learn from it and master this type of problem.

### The Challenge Explained

Let us start by looking at the problem. We are given a string **s** composed entirely of lowercase English letters. We are granted a specific power called "The Operation."

**The Operation**
You can look at the string and choose any letter that appears **at least twice**. You are then allowed to delete **one** occurrence of that letter. You can repeat this process as many times as you like, or not at all.

**The Goal**
Our mission is to use this operation to transform the original string into the **lexicographically smallest** string possible.

To understand this goal, we need to be very precise about what "lexicographically smallest" means. It is the standard dictionary order we are all used to.

1. **Character Value**
"apple" comes before "banana" because the first letter 'a' is smaller than 'b'. We always want smaller characters to appear as early in the string as possible.
2. **String Length**
This is the subtle part! If one string is a prefix of another, the shorter one is considered smaller. For example, "car" comes before "cart". This creates an interesting dynamic: sometimes simply removing a character makes the string "better" just by making it shorter.

### Building Intuition

When I first looked at this, my mind immediately jumped to a Greedy approach. Why? Because in lexicographical problems, the earliest characters matter the most. Changing the first letter from 'z' to 'a' is far more impactful than changing the last letter.

Imagine you are holding cards with letters on them. You want to lay them out in a row to form the best possible word. You pick up a 'b'. Then you pick up an 'a'. You immediately think, "I wish I had picked up that 'a' first!"

If you are allowed to discard the 'b' (because you have a spare 'b' somewhere else), you should absolutely do it. This would let the 'a' slide into the first position. This logic suggests we should use a **Monotonic Stack**.

### The Initial Strategy

The Monotonic Stack is a fancy name for a simple idea. We maintain a stack of characters that we want to keep. As we iterate through the input string, we compare the current character with the one at the top of our stack.

* **Scenario A**
The current character is larger than the stack top. For example, the stack has 'a' and we see 'b'. This is fine. 'a' followed by 'b' is a valid increasing sequence. We push 'b' onto the stack.
* **Scenario B**
The current character is smaller than the stack top. For example, the stack has 'c' and we see 'a'. This is our chance! If we can get rid of that 'c', the 'a' will take its place, and our string will start with 'a' instead of 'c'. That is a huge improvement.

**But wait! Can we always delete the 'c'?**
No. The problem states we can only delete a character if it appears **at least twice**. So, before we toss that 'c' out, we have to check a safety condition:

> Do we have another copy of 'c' available?

There are two places a spare 'c' could be:

1. **In the future:** We haven't processed the rest of the string yet. If there is another 'c' waiting for us later, we can safely delete this one.
2. **In the past (Stack):** Maybe we already have another 'c' sitting lower down in our stack.

If either of these is true, we can delete the current 'c' to make room for the smaller 'a'.

### The Honest "Post Mortem" of My Failure

I implemented the logic above. I counted the frequency of all characters to know what was coming in the future. I used a stack to build the result. I felt confident.

Then I ran the test case: **s = "aa"**

* **My Output:** "aa"
* **Expected Output:** "a"

I was confused. My logic was perfect for sorting values!

1. I saw the first 'a'. Stack was empty. I pushed 'a'.
2. I saw the second 'a'. Is it smaller than the stack top ('a')? No. So I didn't pop. I pushed the second 'a'.
3. Result: "aa".

**The Realization**
I had completely forgotten about the **Length Rule** of lexicographical order.
"aa" is indeed sorted. But "a" is strictly smaller than "aa" because it is a prefix.
In my eagerness to order the characters by value ('a' < 'b'), I forgot that unnecessary characters at the end of the string are just dead weight. If I have two 'a's, and they end up right next to each other at the end of the string, keeping both of them hurts me. Deleting one makes the string shorter and thus "smaller".

### The Corrected Solution

We need a two phase approach to solve this perfectly.

#### Phase 1: The Greedy Scan

We proceed with the Monotonic Stack idea to handle the character values.
We iterate through the string `s` and maintain our stack `res`.
For every character `c`:

1. We check if `res.back()` (the top of the stack) is greater than `c`.
2. If it is, we ask: "Can we afford to lose `res.back()`?"
* We check our frequency map. Is `res.back()` going to appear again later?
* We check our stack count. Do we have another `res.back()` already in the stack?


3. If we can afford it, we pop `res.back()` to maximize our "sortedness".
4. Finally, we push `c` onto the stack.

#### Phase 2: The Tail Trimming

This is the fix for the "aa" bug. After we finish the loop, we look at the **end** of our result string.
If the last character has a duplicate (meaning its total count in our result is greater than 1), we can remove it!

* If we have "aa", the count of 'a' is 2. We remove the last 'a'. Now we have "a".
* If we have "aab", the last char is 'b'. Count of 'b' is 1. We stop.

We repeat this check until the last character is unique.

### The Code Implementation

Here is the C++ code. I have kept it clean and used vectors for frequency counting, which is very efficient.

```cpp
class Solution {
public:
    string lexSmallestAfterDeletion(string s) {
        // Frequency map to count occurrences of each char in the remaining suffix of s
        // We use a vector of size 26 for fast access
        vector<int> remaining(26, 0);
        for (char c : s) {
            remaining[c - 'a']++;
        }

        // Frequency map to count occurrences of each char currently in our stack result
        vector<int> inStack(26, 0);
        string res;

        for (char c : s) {
            // We are processing 'c' now, so it is no longer in the "remaining" future
            remaining[c - 'a']--;

            // Monotonic Stack Logic:
            // While the last character in result is larger than current 'c',
            // we try to remove it to make the string lexicographically smaller.
            while (!res.empty() && res.back() > c) {
                char top = res.back();
                
                // We can delete 'top' ONLY if:
                // 1. There are more occurrences of 'top' later in the string (remaining > 0)
                // 2. OR we have redundant copies of 'top' already in the stack (inStack > 1)
                // If neither is true, deleting 'top' would violate the "at least one" rule.
                if (remaining[top - 'a'] > 0 || inStack[top - 'a'] > 1) {
                    res.pop_back();
                    inStack[top - 'a']--;
                } else {
                    // We must keep 'top' to satisfy the condition
                    break;
                }
            }

            // Add the current character to the result
            res.push_back(c);
            inStack[c - 'a']++;
        }

        // EDGE CASE FIX (Handle "aa" -> "a"):
        // If the resulting string ends with characters that have duplicates, 
        // removing them makes the string shorter and therefore lexicographically smaller.
        // Example: "aba" -> "ab" is better because "ab" < "aba".
        while (!res.empty() && inStack[res.back() - 'a'] > 1) {
            inStack[res.back() - 'a']--;
            res.pop_back();
        }

        return res;
    }
};

```

### Complexity Analysis

Let us look at how efficient this is.

**Time Complexity**
We iterate through the input string exactly once to build the frequency map. Then we iterate through it one more time to build the result. Inside the loop, each character is added to the stack once and removed at most once. The final cleanup loop also runs quickly as it only trims the tail. This gives us a **Linear Time Complexity**, or O(N), where N is the length of the string. This is the best we can hope for!

**Space Complexity**
We use two vectors of size 26 to store counts. This is constant space, O(1). The result string `res` can grow up to size N. So, the auxiliary space is very low.

### Conclusion

This problem is a beautiful example of how details matter. The core algorithm (Monotonic Stack) gets us 90% of the way there, but understanding the precise definition of the goal (lexicographical order includes length!) is what gets us the final 10%.

I hope this explanation helps you understand not just the *how*, but the *why* behind the solution. Remember, every "Wrong Answer" is just a stepping stone to a deeper understanding. Happy coding!
