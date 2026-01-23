#include <vector>
#include <algorithm>
#include <climits> // For INT_MAX

using namespace std;

class Solution {
public:
    vector<vector<int>> minimumAbsDifference(vector<int>& arr) {
        // Step 1: Sort the array to ensure closest elements are adjacent
        sort(arr.begin(), arr.end());
        
        vector<vector<int>> result;
        int minDiff = INT_MAX;
        
        // Step 2: Iterate through the array comparing adjacent pairs
        for (int i = 0; i < arr.size() - 1; i++) {
            int currentDiff = arr[i+1] - arr[i];
            
            // If we find a new smaller difference, discard previous results
            if (currentDiff < minDiff) {
                minDiff = currentDiff;
                result.clear(); // O(1) conceptually, clears vector size
                result.push_back({arr[i], arr[i+1]});
            } 
            // If we find a pair with the same minimum difference, add it
            else if (currentDiff == minDiff) {
                result.push_back({arr[i], arr[i+1]});
            }
        }
        
        return result;
    }
};