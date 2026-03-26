#include <vector>
#include <algorithm>

using namespace std;

// Fenwick Tree (Binary Indexed Tree) for efficient prefix sums
class FenwickTree {
    vector<int> bit;
    int n;
public:
    FenwickTree(int n) : n(n) {
        bit.assign(n + 1, 0);
    }
    
    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx) {
            bit[idx] += val;
        }
    }
    
    int query(int idx) {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx) {
            sum += bit[idx];
        }
        return sum;
    }
};

class Solution {
public:
    vector<int> resultArray(vector<int>& nums) {
        int n = nums.size();
        if (n <= 2) return nums;

        // Step 1: Coordinate Compression
        vector<int> sorted_nums = nums;
        sort(sorted_nums.begin(), sorted_nums.end());
        sorted_nums.erase(unique(sorted_nums.begin(), sorted_nums.end()), sorted_nums.end());
        
        int max_rank = sorted_nums.size();
        
        // Helper lambda to get the compressed rank (1-indexed for Fenwick Tree)
        auto get_rank = [&](int val) {
            return lower_bound(sorted_nums.begin(), sorted_nums.end(), val) - sorted_nums.begin() + 1;
        };

        // Step 2: Initialize Fenwick Trees and Arrays
        FenwickTree ft1(max_rank);
        FenwickTree ft2(max_rank);
        
        vector<int> arr1;
        vector<int> arr2;
        
        // Process the first two elements
        arr1.push_back(nums[0]);
        ft1.add(get_rank(nums[0]), 1);
        
        arr2.push_back(nums[1]);
        ft2.add(get_rank(nums[1]), 1);

        // Step 3: Distribute the rest of the elements
        for (int i = 2; i < n; ++i) {
            int val = nums[i];
            int rank = get_rank(val);
            
            // strictly greater = total elements currently in array - elements less than or equal to current rank
            int greater1 = arr1.size() - ft1.query(rank);
            int greater2 = arr2.size() - ft2.query(rank);
            
            if (greater1 > greater2) {
                arr1.push_back(val);
                ft1.add(rank, 1);
            } else if (greater1 < greater2) {
                arr2.push_back(val);
                ft2.add(rank, 1);
            } else {
                // Tie-breaker: push to the array with fewer elements
                if (arr1.size() <= arr2.size()) {
                    arr1.push_back(val);
                    ft1.add(rank, 1);
                } else {
                    arr2.push_back(val);
                    ft2.add(rank, 1);
                }
            }
        }

        // Step 4: Concatenate arr1 and arr2
        vector<int> result;
        result.reserve(n);
        result.insert(result.end(), arr1.begin(), arr1.end());
        result.insert(result.end(), arr2.begin(), arr2.end());
        
        return result;
    }
};