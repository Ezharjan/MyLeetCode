class Solution {
private:
    // Helper function to handle bucket assignment, especially for negative numbers
    long long getBucketId(long long num, long long w) {
        if (num >= 0) {
            return num / w;
        }
        // For negative numbers, we need to shift them to ensure proper bucket boundaries
        // E.g., if w=4, -1 to -4 should go to bucket -1, not 0.
        return (num + 1) / w - 1;
    }

public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        // Map to store bucket ID -> element value
        unordered_map<long long, long long> buckets;
        
        // Bucket width is valueDiff + 1 to account for 0-inclusive differences
        long long w = (long long)valueDiff + 1; 
        
        for (int i = 0; i < nums.size(); ++i) {
            long long num = nums[i];
            long long id = getBucketId(num, w);
            
            // Condition 1: An element already exists in the exact same bucket
            if (buckets.count(id)) {
                return true;
            }
            
            // Condition 2: An element exists in an adjacent bucket within valueDiff
            if (buckets.count(id - 1) && abs(num - buckets[id - 1]) <= valueDiff) {
                return true;
            }
            if (buckets.count(id + 1) && abs(num - buckets[id + 1]) <= valueDiff) {
                return true;
            }
            
            // Put the current element into its bucket
            buckets[id] = num;
            
            // Maintain the sliding window size of `indexDiff`
            if (i >= indexDiff) {
                long long removeId = getBucketId(nums[i - indexDiff], w);
                buckets.erase(removeId);
            }
        }
        return false;
    }
};