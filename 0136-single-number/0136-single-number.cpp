class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int result = 0;
        
        // XOR all elements together
        for (int num : nums) {
            result ^= num;
        }
        
        return result;
    }
};