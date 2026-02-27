#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

class Solution {
public:
    long long findMaximumElegance(vector<vector<int>>& items, int k) {
        // Sort items in descending order of profit
        sort(items.begin(), items.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] > b[0];
        });
        
        long long current_profit = 0;
        long long max_elegance = 0;
        unordered_set<int> seen_categories;
        vector<int> duplicates; // Stack to track replaceable profits from duplicate categories
        
        // Step 1: Pick the top k items with the highest profit
        for (int i = 0; i < k; ++i) {
            int profit = items[i][0];
            int category = items[i][1];
            current_profit += profit;
            
            if (seen_categories.count(category)) {
                // Since items are sorted descending, elements pushed later have smaller profits
                duplicates.push_back(profit);
            } else {
                seen_categories.insert(category);
            }
        }
        
        // Initial elegance calculation
        long long distinct_count = seen_categories.size();
        max_elegance = current_profit + distinct_count * distinct_count;
        
        // Step 2: Swap duplicates with new categories from the remaining items
        for (int i = k; i < items.size(); ++i) {
            if (duplicates.empty()) {
                break; // We have no safe items left to replace
            }
            
            int profit = items[i][0];
            int category = items[i][1];
            
            // If it's a new category, perform the swap to increase distinct_categories
            if (!seen_categories.count(category)) {
                seen_categories.insert(category);
                
                // Remove the lowest profit duplicate, add the new profit
                current_profit = current_profit - duplicates.back() + profit;
                duplicates.pop_back(); 
                
                distinct_count = seen_categories.size();
                long long current_elegance = current_profit + distinct_count * distinct_count;
                
                // Keep the maximum elegance found
                max_elegance = max(max_elegance, current_elegance);
            }
        }
        
        return max_elegance;
    }
};