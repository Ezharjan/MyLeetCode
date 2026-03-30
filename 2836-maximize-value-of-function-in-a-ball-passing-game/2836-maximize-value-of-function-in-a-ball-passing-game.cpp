class Solution {
public:
    long long getMaxFunctionValue(vector<int>& receiver, long long k) {
        int n = receiver.size();
        
        // Calculate the maximum number of bits needed to represent k
        int LOG = 64 - __builtin_clzll(k); 
        
        // up[j][i] stores the receiver after 2^j steps starting from i
        vector<vector<int>> up(LOG, vector<int>(n));
        
        // sum[j][i] stores the sum of node indices visited in 2^j steps from i
        // (excluding the starting node i itself)
        vector<vector<long long>> sum(LOG, vector<long long>(n));
        
        // Base case: 2^0 = 1 step
        for (int i = 0; i < n; i++) {
            up[0][i] = receiver[i];
            sum[0][i] = receiver[i];
        }
        
        // Build the binary lifting tables
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) {
                int half_way_node = up[j - 1][i];
                up[j][i] = up[j - 1][half_way_node];
                sum[j][i] = sum[j - 1][i] + sum[j - 1][half_way_node];
            }
        }
        
        long long max_score = 0;
        
        // Evaluate the maximum score possible starting from each player i
        for (int i = 0; i < n; i++) {
            long long current_score = i; // Initial score includes the starting node
            int current_node = i;
            
            // Jump according to the set bits in k
            for (int j = 0; j < LOG; j++) {
                if ((k >> j) & 1) {
                    current_score += sum[j][current_node];
                    current_node = up[j][current_node];
                }
            }
            
            max_score = max(max_score, current_score);
        }
        
        return max_score;
    }
};