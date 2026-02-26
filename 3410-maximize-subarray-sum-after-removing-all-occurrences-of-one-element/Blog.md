# A Friendly Guide to Maximizing Subarray Sums by Removing Elements [#3410]

Hello everyone! Today I would like to share a detailed walkthrough of a very interesting array puzzle. This problem asks us to look at an array of integers and find the largest possible sum of a contiguous segment. The twist is that we are allowed to pick exactly one number and completely erase all of its appearances from our array before we calculate the sum. 

I know that array problems can sometimes feel overwhelming, especially when there are modifications involved. But please do not worry! We will take this step by step. By breaking the logic down into smaller and easily digestible pieces, we can build a really solid understanding of how to approach this. We will explore a workable method that relies on a few classic programming concepts.

This post is designed to be comprehensive. Whether you are seeing these concepts for the first time or you just want a refresher, I hope you find this guide helpful and encouraging!

***

### Understanding the Problem

Let us start by getting a clear picture of what we are trying to achieve. 

We are given an array of integers. Some numbers are positive, some are zero, and some are negative. 
Our goal is to find a "subarray," which is simply a contiguous slice of the original array, such that the sum of all the numbers in that slice is as large as possible.

However, we have a special power. Before we find this maximum contiguous sum, we are allowed to choose one specific integer value. Let us call this chosen value our "target." We can then remove every single instance of our target from the array. The remaining elements slide together to close the gaps, forming a new, slightly shorter array. We are allowed to use this power at most once, and the rules state that the resulting array must not be empty. 

Our mission is to figure out which target value, if any, we should remove to achieve the absolute highest contiguous sum possible. 

Let us look at a small example to make this concrete. Suppose our array is:
[negative 3, 2, negative 2, negative 1, 3, negative 2, 3]

If we choose to remove nothing, the best contiguous slice is [3, negative 2, 3], which adds up to 4. 
But what if we choose "negative 2" as our target to remove?
The array becomes: [negative 3, 2, negative 1, 3, 3].
Now, the best contiguous slice is [2, negative 1, 3, 3], which beautifully adds up to 7! 

This tells us that taking the time to remove the pesky "negative 2" instances gave us a much better outcome.

***

### Initial Thoughts and Identifying the Roadblocks

A very natural first thought is to just try everything. We could make a list of every unique number in the array. Then, for each unique number, we could create a brand new array with that number removed, and run a standard maximum subarray sum algorithm on it. 

While this is perfectly logical, we have to consider how much time this will take for very large arrays. If an array has a hundred thousand elements, and all of them are different negative numbers, we would be making a hundred thousand new arrays and scanning each one. The time complexity for this approach would be Big O of N squared. For large inputs, our computers would take far too long to finish the calculations. 

Therefore, we need a method that can evaluate the removal of these target numbers much faster. We need a way to figure out the new maximum sum without physically building a new array every single time. 



To do this, let us think about what happens when we remove all instances of a target number. The target number basically acts like a pair of scissors. Everywhere the target number exists, it cuts the original array into separate pieces or "blocks." 

If we can quickly understand the properties of these individual blocks, we can chain them back together in a smart way. 

***

### The Three Pillars of Our Approach

To make our block chaining idea work efficiently, we are going to lean on three fundamental concepts in computer science. I will explain each one in detail so we are all on the same page.

#### Pillar 1: Kadane's Algorithm Background
Before we even think about removing numbers, we need to know how to find the maximum subarray sum of a normal, untouched array. The classic way to do this is known as Kadane's Algorithm.

The idea is simple but brilliant. We walk through the array from left to right, keeping a running total of the numbers we see. If our running total ever drops below zero, it means the section we just looked at is essentially "dead weight." It would only drag down any future numbers we might add to it. So, when the running total becomes negative, we simply reset it to zero and start fresh from the next number. All the while, we keep track of the highest running total we have ever seen.

This algorithm is incredibly fast, taking only Big O of N time, which means we only need to look at each element once. We will use a modified version of this thinking when we chain our blocks together later.

#### Pillar 2: Prefix Sums
The second tool we need is the Prefix Sum array. A prefix sum array is like a ledger that keeps a running total of everything up to a certain point. 

Imagine our array is [5, 2, negative 4, 3].
Our prefix sum array starts with a 0.
Then we add 5, so we get 5.
Then we add 2, getting 7.
Then we add negative 4, getting 3.
Then we add 3, getting 6.
The prefix sum array becomes [0, 5, 7, 3, 6].

Why is this useful? Because it allows us to find the sum of ANY contiguous slice in instant time! If we want the sum of the slice from the second element to the fourth element, we simply take the prefix sum at the end of the slice and subtract the prefix sum at the beginning of the slice. This trick prevents us from having to manually add up the numbers inside a block every time we need its total.

#### Pillar 3: Sparse Tables


The final piece of the puzzle is a data structure called a Sparse Table. Do not let the name intimidate you; it is just a clever way to answer questions like "What is the largest number in this specific slice?" or "What is the smallest number in this specific slice?" instantly.

A Sparse Table works by precomputing the answers for slices that have lengths equal to powers of two. We find the answers for slices of length 1, then length 2, then length 4, then length 8, and so on. 

When someone asks us for the maximum value in a slice of, say, length 6, we can overlap two slices of length 4 to cover the entire length 6 completely. By comparing the answers from those two overlapping slices of length 4, we instantly know the maximum for the whole slice of length 6.

Building the Sparse Table takes Big O of N log N time, but once it is built, answering any slice query takes Big O of 1 time. This is incredibly fast and exactly what we need to evaluate our blocks rapidly.

***

### Putting It All Together: The Block Evaluation Strategy

Now that we have our tools, let us see how we actually solve the puzzle.

First, we do a quick scan to find the best possible sum without removing anything at all. This is our baseline score. We also check if the entire array consists of zero and negative numbers. If the largest number in the entire array is zero or negative, removing numbers will not magically create a positive sum. In that sad case, the best we can do is just pick the single largest negative number (or zero) and we are done!

Assuming we have positive numbers to work with, we focus only on removing negative numbers. Removing a positive number would only hurt our sum, so we ignore them. We gather all the unique negative numbers and note down every single position where they appear in the array.

Let us pick one unique negative number, our "target", and see how we process it. 

As mentioned earlier, the target acts like scissors, cutting the array into several blocks. For example, if the array is [Block A] target [Block B] target [Block C], we have three blocks. We want to know the best way to travel through these blocks to get the highest sum.

For any given block, we need to know three things instantly:
1.  **The Total Sum:** What is the sum of every single number inside this block? We can get this instantly using our Prefix Sum array.
2.  **The Maximum Prefix:** If we start at the very beginning of this block and walk forward, what is the highest sum we can reach before stopping somewhere inside the block?
3.  **The Maximum Suffix:** If we start somewhere inside the block and walk all the way to the very end of the block, what is the highest sum we can achieve?

How do we get the Maximum Prefix and Maximum Suffix instantly? This is where the Sparse Table shines! 
The sum of a slice from the start of the block to some middle point is calculated by taking the Prefix Sum at that middle point and subtracting the Prefix Sum at the start of the block. Since the Prefix Sum at the start of the block is a fixed value, finding the highest slice sum just means finding the highest Prefix Sum value anywhere inside the block! Our Sparse Table of maximums gives us this instantly.

Similarly, to find the best suffix, we want to subtract the smallest possible Prefix Sum value found before our suffix starts. Our Sparse Table of minimums gives us this instantly as well.

### The Chaining Logic

Now we walk through the blocks one by one, simulating Kadane's algorithm but at a block level.

We maintain a variable called `best preceding sum`. This represents the best contiguous sum that ends exactly at the boundary of the previous block, ready to connect to the current block.

When we are looking at a new block, we have two ways to update our overall global best answer:
Option one: We just take the Maximum Prefix of the current block and add it to our `best preceding sum`. This represents a contiguous slice that started somewhere in the past, jumped over the target numbers, and ended somewhere inside the current block.
Option two: We completely ignore the past and just see if the current block has a really good internal contiguous sum all by itself. (We actually get this handled by our baseline check and standard Kadane).

After checking our global answer, we need to update our `best preceding sum` for the NEXT block to use. 
The new `best preceding sum` could be the old `best preceding sum` plus the entire Total Sum of the current block. This means we walk entirely through the current block.
Or, the new `best preceding sum` could just be the Maximum Suffix of the current block. This means we throw away the past and start a brand new chain from inside the current block, walking to its end.

We repeat this fast block chaining for every single unique negative number. Because we use Prefix Sums and Sparse Tables, evaluating a block takes constant time. The total time we spend chaining blocks is strictly proportional to the number of times the target elements appear. Since we only look at each appearance a small number of times across the entire process, the chaining part is incredibly fast!

***

### The C++ Implementation

Here is a workable approach written in C++. I have structured it to follow the exact logic we just discussed. The code builds the sparse tables and then evaluates the blocks. 

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long maxSubarraySum(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        long long max_val = nums[0];
        long long current_max = nums[0];
        long long global_max = nums[0];
        
        for (int i = 1; i < n; ++i) {
            max_val = max(max_val, (long long)nums[i]);
            current_max = max((long long)nums[i], current_max + nums[i]);
            global_max = max(global_max, current_max);
        }
        
        if (max_val <= 0) {
            return max_val;
        }
        
        long long ans = global_max;
        
        vector<pair<int, int>> negative_positions;
        negative_positions.reserve(n);
        for (int i = 0; i < n; ++i) {
            if (nums[i] < 0) {
                negative_positions.push_back({nums[i], i});
            }
        }
        
        if (negative_positions.empty()) {
            return ans;
        }
        
        sort(negative_positions.begin(), negative_positions.end());
        
        vector<long long> P(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            P[i + 1] = P[i] + nums[i];
        }
        
        int logN = 32 - __builtin_clz(n + 1);
        vector<vector<long long>> st_max(n + 1, vector<long long>(logN));
        vector<vector<long long>> st_min(n + 1, vector<long long>(logN));
        
        for (int i = 0; i <= n; ++i) {
            st_max[i][0] = P[i];
            st_min[i][0] = P[i];
        }
        
        for (int j = 1; j < logN; ++j) {
            for (int i = 0; i + (1 << j) <= n + 1; ++i) {
                st_max[i][j] = max(st_max[i][j - 1], st_max[i + (1 << (j - 1))][j - 1]);
                st_min[i][j] = min(st_min[i][j - 1], st_min[i + (1 << (j - 1))][j - 1]);
            }
        }
        
        auto query_max = [&](int L, int R) { 
            int j = 31 - __builtin_clz(R - L + 1);
            return max(st_max[L][j], st_max[R - (1 << j) + 1][j]);
        };
        
        auto query_min = [&](int L, int R) { 
            int j = 31 - __builtin_clz(R - L + 1);
            return min(st_min[L][j], st_min[R - (1 << j) + 1][j]);
        };
        
        int m_total = negative_positions.size();
        int i_ptr = 0;
        
        while (i_ptr < m_total) {
            int x = negative_positions[i_ptr].first;
            vector<int> indices;
            while (i_ptr < m_total && negative_positions[i_ptr].first == x) {
                indices.push_back(negative_positions[i_ptr].second);
                i_ptr++;
            }
            
            int m = indices.size();
            long long max_preceding_suffix = 0; 
            
            for (int k = 0; k <= m; ++k) {
                int L_k = (k == 0) ? 0 : indices[k - 1] + 1;
                int R_k = (k == m) ? n - 1 : indices[k] - 1;
                
                long long total_k = 0, prefix_max = 0, suffix_max = 0;
                
                if (L_k <= R_k) { 
                    total_k = P[R_k + 1] - P[L_k];
                    prefix_max = query_max(L_k, R_k + 1) - P[L_k];
                    suffix_max = P[R_k + 1] - query_min(L_k, R_k + 1);
                }
                
                if (k > 0) {
                    ans = max(ans, max_preceding_suffix + prefix_max);
                    max_preceding_suffix = max(max_preceding_suffix + total_k, suffix_max);
                } else {
                    max_preceding_suffix = suffix_max;
                }
            }
        }
        
        return ans;
    }
};

```

---

### Code Breakdown and Analysis

Let us gently walk through the code blocks to see how our ideas translated into C++.

* **The Warm Up Phase:** Right at the beginning, we run a standard Kadane loop. This gives us our baseline `global_max`. We also check if `max_val` is zero or less. If the array is entirely made of negative numbers or zeros, we simply return `max_val` immediately. This cleanly handles tricky edge cases where nothing positive exists.
* **Sorting the Targets:** We gather all the negative numbers and their original positions into a list called `negative_positions`. We then sort this list. Sorting groups identical negative numbers together. This allows us to process all the appearances of a single target number in one clean sweep.
* **Building the Foundation:** Next, we build our Prefix Sum array `P`. Notice that `P` is one element larger than our input array. This is a common practice that makes calculating sums from the very first index much easier without going out of bounds.
* **Constructing the Sparse Tables:** We declare two sparse tables, `st_max` and `st_min`. The variable `logN` calculates how many powers of two we need. The nested loops build the tables from the bottom up. Slices of length 1 are filled first, then length 2, then length 4, and so on. The lambda functions `query_max` and `query_min` do the overlapping trick I mentioned earlier, providing instant answers for any slice.
* **The Main Evaluation Loop:** We use a pointer `i_ptr` to iterate through our grouped target numbers. For a specific target value `x`, we collect all its indexes into a smaller list.
* **Block Processing:** The inner loop bounded by `k` iterates over the blocks. We carefully calculate the left boundary `L_k` and right boundary `R_k` of the current block. If the block is valid and not empty, we consult our Prefix Sums and Sparse Tables to get `total_k`, `prefix_max`, and `suffix_max`.
* **Updating the Chains:** If we are past the very first block, we see if gluing the previous blocks to the `prefix_max` of the current block gives us a new global high score. Then, we update `max_preceding_suffix` for the next block to use, deciding whether to extend the existing chain or start a new one from the current `suffix_max`.

### Performance Considerations

Because we avoided the trap of building new arrays from scratch, our solution is highly efficient. Finding the initial maximum takes linear time. Gathering and sorting the positions takes linearithmic time, which is Big O of N log N. Building the sparse tables also takes Big O of N log N time. Finally, the block evaluation step looks at each block boundary only a small number of times, making it extremely fast. Overall, our entire process safely completes within the linearithmic time complexity bounds, keeping our performance smooth and avoiding any timeout issues even for the largest constraints!

### Concluding Thoughts

I hope this thorough breakdown has demystified the process! We took a complex modification problem, realized we could not use brute force, and creatively combined Prefix Sums with Sparse Tables to evaluate shattered array blocks at lightning speed.

Programming is all about building up a toolbox of these clever concepts. Once you are comfortable with how Prefix Sums manage running totals and how Sparse Tables provide instant minimums and maximums, you start seeing opportunities to use them everywhere.

Thank you so much for reading through this guide. I really encourage you to type this out, play with the variables, and see it working first hand. Happy coding, and have a wonderful time tackling more array puzzles!
