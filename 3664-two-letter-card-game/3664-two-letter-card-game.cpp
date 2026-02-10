#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <set>

using namespace std;

class Solution {
public:
    // Helper to calculate max matching for a set of counts + k "xx" cards
    long long calculateMatches(long long sumBase, long long maxBase, long long k) {
        if (k < 0) return 0; // Should not happen with proper clamping
        long long currentSum = sumBase + k;
        // The 'xx' cards act as a distinct type that is compatible with all other types 
        // in this specific group. However, 'xx' cannot pair with 'xx'. 
        // We take the max of original max frequency and the count of 'xx' added.
        long long currentMax = std::max((long long)maxBase, (long long)k);
        
        // Standard matching formula: min(Total/2, Total - MaxFreq)
        return std::min(currentSum / 2, currentSum - currentMax);
    }

    int score(vector<string>& cards, char x) {
        long long cntZ = 0; // Count of "xx"
        
        // Frequencies for "x_" (P set) and "_x" (S set)
        vector<int> pCounts(26, 0); 
        vector<int> sCounts(26, 0); 
        
        for (const string& s : cards) {
            bool char0 = (s[0] == x);
            bool char1 = (s[1] == x);
            
            if (char0 && char1) {
                cntZ++;
            } else if (char0) {
                // Starts with x, e.g., "xa".
                pCounts[s[1] - 'a']++;
            } else if (char1) {
                // Ends with x, e.g., "ax".
                sCounts[s[0] - 'a']++;
            }
        }
        
        // Calculate Base Stats for P
        long long sumP = 0, maxP = 0;
        for (int c : pCounts) {
            sumP += c;
            maxP = max(maxP, (long long)c);
        }
        
        // Calculate Base Stats for S
        long long sumS = 0, maxS = 0;
        for (int c : sCounts) {
            sumS += c;
            maxS = max(maxS, (long long)c);
        }
        
        // Identify Candidate 'k' values (split points for cntZ)
        // We look for "breakpoints" in the matching function for both sides.
        set<long long> candidates;
        
        // Boundaries
        candidates.insert(0);
        candidates.insert(cntZ);
        
        // Critical points for P side
        // 1. Where 'rest' catches up to 'max': S + k - M = (S+k)/2 => k = 2M - S
        candidates.insert(2 * maxP - sumP);
        // 2. Where k becomes the new max: k = M
        candidates.insert(maxP);
        // 3. Saturation: k = S
        candidates.insert(sumP);
        
        // Critical points for S side (mapped to P's k via k = cntZ - k_s)
        // We calculate ideal k_s for S, then convert to k for P
        long long critS1 = 2 * maxS - sumS;
        long long critS2 = maxS;
        long long critS3 = sumS;
        
        candidates.insert(cntZ - critS1);
        candidates.insert(cntZ - critS2);
        candidates.insert(cntZ - critS3);
        
        long long maxScore = 0;
        
        // Evaluate all valid candidates
        for (long long k : candidates) {
            // Check k and small neighborhood to handle integer floor nuances
            for (long long offset = -1; offset <= 1; offset++) {
                long long currK = k + offset;
                if (currK >= 0 && currK <= cntZ) {
                    long long scoreP = calculateMatches(sumP, maxP, currK);
                    long long scoreS = calculateMatches(sumS, maxS, cntZ - currK);
                    maxScore = max(maxScore, scoreP + scoreS);
                }
            }
        }
        
        return (int)maxScore;
    }
};