# Unlocking the Magic of Array Elegance Through Greedy Strategies and Category Swapping [#2813]

Hello everyone in the wonderful coding community! Today I am very excited to share my thoughts and a detailed breakdown of a fascinating algorithmic puzzle. I recently spent some time studying this problem and found the logic so elegant that I just had to write a comprehensive guide for us all. If you have been looking for a friendly and thorough explanation, you are in the right place! Let us explore this puzzle step by step together.

### Understanding the Core of the Problem

Imagine you are given a collection of items. Every single item has two distinct attributes attached to it. The first attribute is its profit, which tells us how valuable the item is. The second attribute is its category, which tells us what group the item belongs to. You are also given a specific number, let us call it K, which represents the exact number of items you are allowed to choose from the collection.

Our goal is to maximize a special score called elegance. The problem defines elegance using a very specific formula. The elegance of your chosen selection of K items is equal to the total sum of the profits of the items you selected, plus the square of the number of entirely distinct categories present in your selection. 

To put it in simple terms, elegance equals Total Profit plus (Distinct Categories multiplied by Distinct Categories). 

This puts us in a very interesting dilemma. Do we just blindly pick the items with the highest possible profits and ignore the categories? Or do we focus heavily on picking as many different categories as possible, even if the items themselves have very low profits? The secret lies in finding the perfect balance between these two competing interests. We want to secure a high base profit while also reaping the exponential rewards of having multiple distinct categories.

### The Thought Process and Strategy

When faced with a problem that asks us to maximize a value based on multiple factors, a greedy approach is often a great place to start thinking. 



Let us break our strategy down into two logical phases.

**Phase One Building a Foundation**
Since profit directly contributes to our score in a linear way, the most logical first step is to sort our entire collection of items in descending order based strictly on their profit. We want the most valuable items sitting right at the front of our list. 

Once they are sorted, we will greedily pick the top K items. By doing this, we absolutely guarantee that we have secured the maximum possible base profit for any selection of K items. We will calculate our current distinct categories and record this initial elegance score. This gives us our baseline.

**Phase Two The Art of Swapping**
Now we have our initial K items. But what if there are items left over in the collection that belong to categories we have not seen yet? Adding a brand new category could dramatically increase our elegance score because the category count is squared! 

However, since we are strictly limited to exactly K items, bringing a new item in means we must kick an existing item out. We cannot just kick out any item. If we remove an item that is the only representative of its category, our distinct category count will not increase (we lose one category and gain one, resulting in no net change), and we will just be losing profit because the remaining items have smaller profits. 



Therefore, we can only safely remove items that share a category with another item already in our selection. We will refer to these as duplicate category items. To minimize our profit loss, we should always kick out the duplicate category item that has the lowest possible profit. 

### Designing the Data Structures

To make this plan work smoothly in code, we need a few simple tools.

1. A way to sort the items. We will just use the standard sorting function available in our programming language.
2. A way to remember which categories we currently have. A hash set is perfect for this because checking if a category exists is incredibly fast.
3. A way to keep track of the profits of our duplicate category items so we know which ones we can safely discard. We can use a standard list or array acting as a stack for this. Because we process the items in descending order of profit, the duplicates we find later will naturally have smaller profits. If we append them to our list as we find them, the item at the very end of the list will conveniently be the one with the absolute smallest profit!

### A Detailed Walkthrough

Let us imagine an example where our items are `[[10, 1], [5, 1], [3, 2]]` and K is 2. The format is `[profit, category]`.

First, we sort them descending by profit. They are already sorted perfectly! 
Next, we start picking our top 2 items.
We look at the first item `[10, 1]`. The profit is 10, category is 1. We add category 1 to our seen set.
We look at the second item `[5, 1]`. The profit is 5, category is 1. We check our set and see category 1 is already there! This means this item is a duplicate category item. We store its profit 5 in our list of replaceable items.

Our base selection is complete. The total profit is 15. We have 1 distinct category. 
The elegance score right now is 15 plus (1 multiplied by 1), which equals 16. We save 16 as our highest seen score.

Now we move to phase two and look at the remaining items. 
We look at the third item `[3, 2]`. The profit is 3, category is 2. We check our set. Category 2 is brand new!
We check our list of replaceable items. We have an item with a profit of 5 that we can safely discard. 
We perform the swap. We subtract 5 from our total profit and add the new profit of 3. Our new total profit becomes 13.
We add category 2 to our seen set. We now have 2 distinct categories.
Let us calculate the new elegance. It is 13 plus (2 multiplied by 2). That is 13 plus 4, which equals 17. 
We compare 17 with our previous best score of 16. 17 is higher, so we update our maximum elegance score!

We have no more items to check. Our final answer is 17.

### The Code Implementation

Here is how we can translate this exact logic into clear and readable C++ code. I have added plenty of comments so you can follow along with every single action.

```cpp
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

class Solution {
public:
    long long findMaximumElegance(vector<vector<int>>& items, int k) {
        
        // Step one is to sort the items based on their profit in descending order.
        // We use a custom comparison to check the first element of each pair.
        sort(items.begin(), items.end(), [](const vector<int>& firstItem, const vector<int>& secondItem) {
            return firstItem[0] > secondItem[0];
        });
        
        long long currentTotalProfit = 0;
        long long absoluteMaximumElegance = 0;
        
        unordered_set<int> uniquelySeenCategories;
        vector<int> safeToReplaceProfits; 
        
        // We begin our first phase by iterating through the top k items.
        for (int index = 0; index < k; ++index) {
            int currentProfit = items[index][0];
            int currentCategory = items[index][1];
            
            currentTotalProfit = currentTotalProfit + currentProfit;
            
            // We check if we have already encountered this category.
            if (uniquelySeenCategories.count(currentCategory)) {
                // Since we sorted descending, items added later have smaller profits.
                // We save this profit because it belongs to a duplicate category.
                safeToReplaceProfits.push_back(currentProfit);
            } else {
                // This is a brand new category, so we record it.
                uniquelySeenCategories.insert(currentCategory);
            }
        }
        
        // We calculate the elegance of our initial greedy selection.
        long long distinctCategoryCount = uniquelySeenCategories.size();
        absoluteMaximumElegance = currentTotalProfit + (distinctCategoryCount * distinctCategoryCount);
        
        // We begin our second phase to explore potential category swaps.
        for (int index = k; index < items.size(); ++index) {
            
            // If we have no duplicate category items left to safely remove, we must stop.
            if (safeToReplaceProfits.empty()) {
                break; 
            }
            
            int potentialNewProfit = items[index][0];
            int potentialNewCategory = items[index][1];
            
            // We only care about items that introduce a category we have never seen.
            if (!uniquelySeenCategories.count(potentialNewCategory)) {
                
                // We officially adopt this new category.
                uniquelySeenCategories.insert(potentialNewCategory);
                
                // We perform the swap mathematics. 
                // We remove the smallest duplicate profit and add the new item profit.
                int profitToDiscard = safeToReplaceProfits.back();
                safeToReplaceProfits.pop_back(); 
                
                currentTotalProfit = currentTotalProfit - profitToDiscard;
                currentTotalProfit = currentTotalProfit + potentialNewProfit;
                
                // We recalculate the distinct count and the new elegance score.
                distinctCategoryCount = uniquelySeenCategories.size();
                long long newlyCalculatedElegance = currentTotalProfit + (distinctCategoryCount * distinctCategoryCount);
                
                // We update our record if this new score is larger than our previous best.
                if (newlyCalculatedElegance > absoluteMaximumElegance) {
                    absoluteMaximumElegance = newlyCalculatedElegance;
                }
            }
        }
        
        return absoluteMaximumElegance;
    }
};

```

### Analyzing the Complexity

It is always good practice to understand the resource requirements of our solutions. Let us discuss the time and space complexity in simple terms.

**Time Complexity Analysis**
The most computationally heavy part of our approach is the very first action we take, which is sorting the collection of items. Sorting an array of N items takes O(N log N) time operations.
After the sorting is complete, we run a loop for the first K items. Then we run a second loop for the remaining items up to the end of the array. Together, these two loops touch every item exactly one time. Inside the loops, all the operations we perform are adding numbers, subtracting numbers, checking a hash set, and pushing or popping from the back of a list. All of these inner operations take a constant O(1) time.
Therefore, the loops combined take an additional O(N) time.
Because O(N log N) grows much faster than O(N) as the number of items increases, the sorting step dominates the overall time. Thus, our final time complexity is perfectly captured as O(N log N).

**Space Complexity Analysis**
We introduced two extra storage structures to help us solve the problem. We used a hash set to store the unique categories we encounter, and we used a list to store the profits of the duplicate items we are willing to replace.
In the absolute worst case scenario, we might end up storing every single category in the hash set, or we might end up storing almost all of the items in our replaceable list if they all share the exact same category. Both of these structures scale directly with the total number of items N.
Therefore, the maximum extra memory we require grows linearly with the input size, giving us a space complexity of O(N).

### Final Thoughts and Takeaways

This problem beautifully illustrates how a greedy strategy can be layered. We did not just make a single greedy choice and hope for the best. We made a solid initial greedy choice to secure profits, and then we made a series of calculated, greedy sacrifices to boost a secondary multiplicative factor. Using a list to easily access the least valuable duplicate item was the key to making this logic translate into fast code.

I hope this detailed explanation illuminates the underlying mechanics of the solution for you! Taking the time to visualize the swapping process is a wonderful way to improve your general algorithm design skills. Keep practicing, keep sharing your knowledge with others, and happy coding to you all!
