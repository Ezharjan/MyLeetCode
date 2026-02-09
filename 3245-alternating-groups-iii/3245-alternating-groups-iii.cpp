#include <vector>
#include <set>
#include <iostream>

using namespace std;

class Fenwick {
    int n;
    vector<int> tree;
public:
    Fenwick(int size) : n(size), tree(size + 1, 0) {}
    
    // Adds val to the element at idx
    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx) {
            tree[idx] += val;
        }
    }
    
    // Returns sum from 1 to idx
    int query(int idx) {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx) {
            sum += tree[idx];
        }
        return sum;
    }
    
    // Returns sum from k to n (suffix sum)
    int querySuffix(int k) {
        if (k > n) return 0;
        return query(n) - query(k - 1);
    }
};

class Solution {
public:
    vector<int> numberOfAlternatingGroups(vector<int>& colors, vector<vector<int>>& queries) {
        int n = colors.size();
        vector<int> results;
        
        // BITs to maintain counts and sums of segment lengths
        Fenwick countBIT(n);
        Fenwick sumBIT(n);
        
        // Set to store indices i where colors[i] == colors[i+1]
        set<int> bad_indices;
        
        auto is_bad = [&](int i) {
            return colors[i] == colors[(i + 1) % n];
        };
        
        // Initialize bad_indices
        for (int i = 0; i < n; ++i) {
            if (is_bad(i)) bad_indices.insert(i);
        }
        
        auto add_len = [&](int len) {
            if (len <= 0) return; 
            countBIT.add(len, 1);
            sumBIT.add(len, len);
        };
        
        auto remove_len = [&](int len) {
            if (len <= 0) return;
            countBIT.add(len, -1);
            sumBIT.add(len, -len);
        };
        
        // Initial population of BITs
        if (bad_indices.empty()) {
            add_len(n);
        } else {
            int first = *bad_indices.begin();
            int prev = first;
            for (auto it = next(bad_indices.begin()); it != bad_indices.end(); ++it) {
                int curr = *it;
                add_len(curr - prev);
                prev = curr;
            }
            add_len(n - prev + first);
        }
        
        for (const auto& q : queries) {
            if (q[0] == 1) {
                int k = q[1];
                if (bad_indices.empty()) {
                    // Entire circle is alternating
                    results.push_back(n);
                } else {
                    // Calculate sum of (len - k + 1) for all len >= k
                    int cnt = countBIT.querySuffix(k);
                    int sum = sumBIT.querySuffix(k);
                    results.push_back(sum - cnt * (k - 1));
                }
            } else {
                int idx = q[1];
                int col = q[2];
                if (colors[idx] == col) continue;
                
                int prev_idx = (idx - 1 + n) % n;
                
                // Determine if we are in a "simple" case (few bad edges) or "complex" case
                // "Empty External" means all bad edges are contained within the set {prev_idx, idx}
                bool empty_external = false;
                if (bad_indices.size() <= 2) {
                    empty_external = true;
                    for (int x : bad_indices) {
                        if (x != prev_idx && x != idx) {
                            empty_external = false;
                            break;
                        }
                    }
                }
                
                if (empty_external) {
                    // Remove all current segments
                    if (bad_indices.empty()) remove_len(n);
                    else {
                        int first = *bad_indices.begin();
                        int p = first;
                        for (auto it = next(bad_indices.begin()); it != bad_indices.end(); ++it) {
                            remove_len(*it - p);
                            p = *it;
                        }
                        remove_len(n - p + first);
                    }
                    
                    // Update state
                    colors[idx] = col;
                    if (is_bad(prev_idx)) bad_indices.insert(prev_idx);
                    else bad_indices.erase(prev_idx);
                    
                    if (is_bad(idx)) bad_indices.insert(idx);
                    else bad_indices.erase(idx);
                    
                    // Add new segments
                    if (bad_indices.empty()) add_len(n);
                    else {
                        int first = *bad_indices.begin();
                        int p = first;
                        for (auto it = next(bad_indices.begin()); it != bad_indices.end(); ++it) {
                            add_len(*it - p);
                            p = *it;
                        }
                        remove_len(0); // Safety, though add_len handles <=0
                        add_len(n - p + first);
                    }
                } else {
                    // General case: We update only the local neighborhood of segments
                    // Find the nearest bad indices strictly outside {prev_idx, idx}
                    
                    // Helper to find previous bad index (wrapping) not in {prev_idx, idx}
                    auto find_prev_external = [&](int start) {
                        int curr = start;
                        while (true) {
                            auto it = bad_indices.lower_bound(curr);
                            if (it == bad_indices.begin()) it = bad_indices.end();
                            --it;
                            if (*it != prev_idx && *it != idx) return *it;
                            curr = *it;
                        }
                    };
                    
                    // Helper to find next bad index (wrapping) not in {prev_idx, idx}
                    auto find_next_external = [&](int start) {
                        int curr = start;
                        while(true) {
                            auto it = bad_indices.upper_bound(curr);
                            if (it == bad_indices.end()) it = bad_indices.begin();
                            if (*it != prev_idx && *it != idx) return *it;
                            curr = *it;
                        }
                    };

                    int pre = find_prev_external(prev_idx);
                    int suc = find_next_external(idx);

                    // 1. Remove old segments between 'pre' and 'suc'
                    int curr = pre;
                    while (true) {
                        auto it = bad_indices.upper_bound(curr);
                        if (it == bad_indices.end()) it = bad_indices.begin();
                        int next_val = *it;
                        
                        int dist = (next_val - curr + n) % n;
                        if (dist == 0) dist = n;
                        remove_len(dist);
                        
                        if (next_val == suc) break;
                        curr = next_val;
                    }
                    
                    // 2. Update state
                    colors[idx] = col;
                    if (is_bad(prev_idx)) bad_indices.insert(prev_idx);
                    else bad_indices.erase(prev_idx);
                    
                    if (is_bad(idx)) bad_indices.insert(idx);
                    else bad_indices.erase(idx);
                    
                    // 3. Add new segments between 'pre' and 'suc'
                    curr = pre;
                    while (true) {
                        auto it = bad_indices.upper_bound(curr);
                        if (it == bad_indices.end()) it = bad_indices.begin();
                        int next_val = *it;
                        
                        int dist = (next_val - curr + n) % n;
                        if (dist == 0) dist = n;
                        add_len(dist);
                        
                        if (next_val == suc) break;
                        curr = next_val;
                    }
                }
            }
        }
        return results;
    }
};