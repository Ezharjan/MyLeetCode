# Uncovering the Longest Shared Journey Among Friends [#1923]

Hello everyone! Welcome to this comprehensive guide. Today I would like to share my thoughts and a detailed walkthrough for an incredibly fascinating sequence matching problem. Whether you are a beginner or looking to refresh your algorithmic toolkit, we will take our time and explore every concept thoroughly. My goal is to break the logic down into very small steps so we can all learn and understand the underlying magic together.

Let us imagine a beautiful country with many interconnected cities. A group of friends is traveling, and each friend takes a specific route through various cities. Our task is to find the longest continuous sequence of cities that every single friend visited. If no such shared sequence exists, we simply return zero.

This problem asks us to find the longest common subpath among multiple arrays of integers. It is a wonderful exercise that combines two powerful concepts working in harmony. We will use a search technique to guess the length of the path, and a hashing technique to verify if our guess is correct.

Let us dive deeply into the mechanics!

### Understanding the Challenge

Before we write any code, we need to understand the rules of the journey. We are given an integer representing the total number of cities, and a list of paths. Each path is a list of integers where each integer represents a city. 

The subpath we are looking for must be contiguous. This means the cities must be visited in the exact same sequential order without any skips. Additionally, this exact same sequence must be present in the itinerary of every single friend.

A naive approach might try to generate all possible subpaths from the first friend's journey and then check if those subpaths exist in everyone else's journey. However, there are thousands of cities and thousands of paths. Checking every combination would simply take way too long. We need a strategy that scales gracefully.

### Strategy Part One Exploring with Binary Search

How do we find the maximum length efficiently? We can use a property called monotonicity. 

Think about it this way. If all friends share a common subpath of length five, it is an absolute certainty that they also share a common subpath of length four, length three, length two, and length one. Conversely, if they do not share any subpath of length six, they absolutely cannot share a subpath of length seven or eight.

Because of this true or false boundary, we do not need to check every single length one by one. We can use a binary search approach on the answer itself.



Here is how our search works:
* We know the minimum possible length is one.
* We know the maximum possible length cannot exceed the length of the shortest path among all friends. A shared path cannot be longer than the shortest trip taken!
* We pick a length exactly in the middle of our minimum and maximum boundaries.
* We test if a common subpath of this middle length exists across all friends.
* If it does exist, we record this length as our best answer so far, and we change our minimum boundary to look for something even longer.
* If it does not exist, we know this length and all greater lengths are impossible, so we change our maximum boundary to look for something shorter.

This search cuts our possibilities in half every single time, making it incredibly fast.

### Strategy Part Two Verifying with a Sliding Window

Now we have a new problem. When our search algorithm guesses a length, how do we efficiently check if a subpath of that exact length exists in all the lists? 

If our guessed length is L, we need to look at every window of length L in the first friend's path, and then see if any of those windows appear in the second friend's path, the third friend's path, and so on.

Comparing long sequences of numbers item by item is slow. To speed this up, we can transform each sequence of numbers into a single unique identification number. This is called hashing. 



We will use a technique inspired by the Rabin Karp algorithm. It allows us to calculate the hash of a sequence, and then as our window slides forward by one city, we can update the hash in constant time instead of recalculating the whole thing from scratch.

Imagine our sequence of cities as a large number written in a specific base. When the window slides forward, we simply remove the mathematical value of the city that dropped off the left side, multiply the remaining value by our base, and add the new city on the right side.

To prevent our numbers from becoming too large for the computer to handle, we use a modulo operation. This wraps the numbers around a large prime number, keeping them within a safe range.

### The Problem of Collisions and Double Hashing

There is a small catch when using a modulo operation. Because we are wrapping our numbers around a limit, two completely different sequences of cities might accidentally produce the exact same hash value. This is known as a collision.

In rigorous testing environments, relying on a single hash might lead to false positives, making our code think two paths match when they actually do not. To be highly accurate, we will use a technique called double hashing. 

Instead of generating one hash for a window, we generate two distinct hashes using two different prime bases and two different modulo limits. We then combine these two hashes into one massive 64 bit integer. The mathematical probability of two completely different sequences producing the exact same pair of hashes is astronomically low. This gives us immense confidence in our matches.

### Putting It All Together

Let us map out the exact sequence of events for our code:
* First, we iterate through all the paths to find the length of the shortest journey. This sets the upper boundary for our search.
* We start our search loop. We calculate a middle length to test.
* For this specific length, we create our double hashing base multipliers. 
* We look at the first friend's path. We slide a window of the guessed length across their path, calculate the combined double hash for every step, and store these hashes in a unique collection called a set.
* We then move to the next friend. We slide a window across their path and collect all their hashes. 
* Here is the crucial part. We compare the new friend's hashes with our stored hashes. We only keep the hashes that exist in both! This represents the intersection of their paths.
* We repeat this intersection process for every friend. If our storage set becomes empty at any point, it means no sequence is shared by everyone, so we can stop early and report failure for this length.
* If we finish checking all friends and our set still contains at least one hash, it means a common subpath of that length definitely exists!

### The C++ Implementation

Below is the code written in C++. I have structured it to be readable and straightforward. Feel free to copy it and study the mechanics.

```cpp
class Solution {
public:
    int longestCommonSubpath(int n, vector<vector<int>>& paths) {
        int shortest_path_length = paths[0].size();
        for (int i = 1; i < paths.size(); ++i) {
            if (paths[i].size() < shortest_path_length) {
                shortest_path_length = paths[i].size();
            }
        }

        int left = 1;
        int right = shortest_path_length;
        int best_length = 0;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            
            if (isValidLength(mid, paths)) {
                best_length = mid;
                left = mid + 1; 
            } else {
                right = mid - 1; 
            }
        }
        
        return best_length;
    }

private:
    bool isValidLength(int target_length, const vector<vector<int>>& paths) {
        long long prime_modulus_one = 1000000007;
        long long prime_modulus_two = 1000000009;
        long long base_multiplier_one = 100003;
        long long base_multiplier_two = 100019;

        long long highest_power_one = 1;
        long long highest_power_two = 1;
        
        for (int i = 0; i < target_length; ++i) {
            highest_power_one = (highest_power_one * base_multiplier_one) % prime_modulus_one;
            highest_power_two = (highest_power_two * base_multiplier_two) % prime_modulus_two;
        }

        unordered_set<uint64_t> universally_shared_hashes;

        for (int person_index = 0; person_index < paths.size(); ++person_index) {
            const vector<int>& current_path = paths[person_index];
            unordered_set<uint64_t> current_person_hashes;
            
            long long hash_one = 0;
            long long hash_two = 0;

            for (int i = 0; i < current_path.size(); ++i) {
                hash_one = (hash_one * base_multiplier_one + current_path[i]) % prime_modulus_one;
                hash_two = (hash_two * base_multiplier_two + current_path[i]) % prime_modulus_two;

                if (i >= target_length) {
                    long long value_to_remove_one = (current_path[i - target_length] * highest_power_one) % prime_modulus_one;
                    hash_one = (hash_one + prime_modulus_one - value_to_remove_one) % prime_modulus_one;
                    
                    long long value_to_remove_two = (current_path[i - target_length] * highest_power_two) % prime_modulus_two;
                    hash_two = (hash_two + prime_modulus_two - value_to_remove_two) % prime_modulus_two;
                }

                if (i >= target_length - 1) {
                    uint64_t combined_hash = ((uint64_t)hash_one << 32) | hash_two;
                    
                    if (person_index == 0) {
                        current_person_hashes.insert(combined_hash);
                    } else {
                        if (universally_shared_hashes.count(combined_hash) > 0) {
                            current_person_hashes.insert(combined_hash);
                        }
                    }
                }
            }

            universally_shared_hashes = current_person_hashes;
            
            if (universally_shared_hashes.empty()) {
                return false;
            }
        }
        
        return true;
    }
};

```

### Analyzing the Performance

When studying an algorithm, it is always helpful to understand its resource usage. We evaluate this in two categories.

**Time Complexity**
The outer loop of our search space cuts the possible answers in half each time. If the shortest path has a length of M, this takes O(log M) steps. Inside this loop, we slide our window across every single city in every single path. If the total number of cities across all paths is N, processing the sliding window takes O(N) operations. Combining these, our overall time complexity is O(N log M). This is a very solid approach and handles large inputs without timing out.

**Space Complexity**
We use an auxiliary data structure to store the hash values for the current window length. In the worst case, the first friend has a path entirely composed of unique windows, meaning we store a number of hashes proportional to their path length. Therefore, the space complexity is proportional to the size of the paths, scaling up to O(N) space.

### Final Thoughts

This problem elegantly bridges the gap between searching for an answer and verifying it with clever mathematical hashing. Breaking it down into the search phase and the verification phase makes the logic much easier to digest. Double hashing might seem intimidating at first, but once you view it as simply keeping two separate scores to avoid a tie, it becomes a very friendly tool in your programming arsenal.

I hope this thorough walkthrough has been helpful to you! Take your time reading through the sliding window logic, perhaps try drawing it out on a piece of paper, and you will see how beautifully the numbers flow. Happy coding to all of you, and keep enjoying the journey of learning!
