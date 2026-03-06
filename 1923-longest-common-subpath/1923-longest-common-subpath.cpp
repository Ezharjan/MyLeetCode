class Solution {
public:
    int longestCommonSubpath(int n, vector<vector<int>>& paths) {
        int min_len = INT_MAX;
        for (const auto& path : paths) {
            min_len = min(min_len, (int)path.size());
        }

        int left = 1, right = min_len, ans = 0;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (check(mid, paths)) {
                ans = mid;
                left = mid + 1; // Try to find a longer common subpath
            } else {
                right = mid - 1; // Mid is too long, search in lower half
            }
        }
        
        return ans;
    }

private:
    bool check(int L, const vector<vector<int>>& paths) {
        // Double hashing parameters to prevent collisions
        long long M1 = 1e9 + 7;
        long long M2 = 1e9 + 9;
        long long B1 = 100003;
        long long B2 = 100019;

        // Precompute Base^L % Modulo for removing the leading element
        long long BL1 = 1, BL2 = 1;
        for (int i = 0; i < L; ++i) {
            BL1 = (BL1 * B1) % M1;
            BL2 = (BL2 * B2) % M2;
        }

        unordered_set<uint64_t> common_hashes;

        for (int p = 0; p < paths.size(); ++p) {
            const auto& path = paths[p];
            unordered_set<uint64_t> current_hashes;
            long long h1 = 0, h2 = 0;

            for (int i = 0; i < path.size(); ++i) {
                // Add the current city to the hash
                h1 = (h1 * B1 + path[i]) % M1;
                h2 = (h2 * B2 + path[i]) % M2;

                // Remove the city that fell out of the window of length L
                if (i >= L) {
                    long long rem1 = (path[i - L] * BL1) % M1;
                    h1 = (h1 - rem1 + M1) % M1;
                    
                    long long rem2 = (path[i - L] * BL2) % M2;
                    h2 = (h2 - rem2 + M2) % M2;
                }

                // If our window has reached length L, process the hash
                if (i >= L - 1) {
                    uint64_t combined = ((uint64_t)h1 << 32) | h2;
                    
                    if (p == 0) {
                        current_hashes.insert(combined);
                    } else {
                        if (common_hashes.count(combined)) {
                            current_hashes.insert(combined);
                        }
                    }
                }
            }

            // Intersect: Update common hashes to only include ones found in the current path
            common_hashes = move(current_hashes);
            
            // Early exit if no common subpaths are found so far
            if (common_hashes.empty()) {
                return false;
            }
        }
        
        return !common_hashes.empty();
    }
};