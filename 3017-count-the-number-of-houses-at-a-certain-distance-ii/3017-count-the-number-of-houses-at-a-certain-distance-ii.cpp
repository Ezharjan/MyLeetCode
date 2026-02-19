class Solution {
public:
    vector<long long> countOfPairs(int n, int x, int y) {
        // Without loss of generality, let x be the smaller index
        if (x > y) swap(x, y);
        
        // Difference array to record frequencies of each distance
        vector<long long> diff(n + 2, 0);
        
        for (int i = 1; i <= n; i++) {
            int c = abs(i - x) + 1;
            
            // Region A: j is between i + 1 and y
            if (i + 1 <= y) {
                int sep = (i + c + y) / 2;
                int end_A1 = min(y, sep);
                
                // Sub-region A1: direct path is faster or equal
                if (i + 1 <= end_A1) {
                    diff[1]++;
                    diff[end_A1 - i + 1]--;
                }
                // Sub-region A2: shortcut is strictly faster
                if (end_A1 + 1 <= y) {
                    diff[c]++;
                    diff[c + y - end_A1]--;
                }
            }
            
            // Region B: j is strictly greater than y
            int start_B = max(i + 1, y + 1);
            if (start_B <= n) {
                if (i + c >= y) {
                    // Direct path is always faster
                    diff[start_B - i]++;
                    diff[n - i + 1]--;
                } else {
                    // Shortcut is strictly faster
                    diff[c + start_B - y]++;
                    diff[c + n - y + 1]--;
                }
            }
        }
        
        vector<long long> res(n);
        long long current = 0;
        
        // Compute actual frequencies using a prefix sum
        for (int k = 1; k <= n; k++) {
            current += diff[k];
            res[k - 1] = current * 2; // Multiply by 2 for bidirectional pairs (house1, house2) & (house2, house1)
        }
        
        return res;
    }
};