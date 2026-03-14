class Solution {
public:
    int countCompleteSubstrings(string word, int k) {
        int n = word.length();
        int res = 0;
        int left = 0;
        
        // Process the string in segments where adjacent characters' differences are <= 2
        while (left < n) {
            int right = left + 1;
            while (right < n && abs(word[right] - word[right - 1]) <= 2) {
                right++;
            }
            
            // Current segment is from `left` to `right - 1`
            int len = right - left;
            
            // Iterate over all possible numbers of unique characters (from 1 to 26)
            for (int u = 1; u <= 26; ++u) {
                int L = u * k;
                if (L > len) break; // If window size exceeds segment length, we can stop evaluating further `u`s
                
                int freq[26] = {0}; // Track occurrences of each character in the current window
                int count_k = 0;    // Track how many characters have a frequency of exactly `k`
                
                // Slide the window of length L over the current segment
                for (int i = 0; i < len; ++i) {
                    // Add the new character to the right of the window
                    int char_idx = word[left + i] - 'a';
                    freq[char_idx]++;
                    if (freq[char_idx] == k) count_k++;
                    else if (freq[char_idx] == k + 1) count_k--;
                    
                    // Remove the character that fell out of the left of the window
                    if (i >= L) {
                        int char_idx_remove = word[left + i - L] - 'a';
                        freq[char_idx_remove]--;
                        if (freq[char_idx_remove] == k) count_k++;
                        else if (freq[char_idx_remove] == k - 1) count_k--;
                    }
                    
                    // If the window is fully populated, check if it forms a complete substring
                    if (i >= L - 1) {
                        // If exactly `u` distinct characters occur `k` times, it's complete.
                        // (Because u * k == window length L, no other characters can exist in the window)
                        if (count_k == u) {
                            res++;
                        }
                    }
                }
            }
            // Move on to the next segment
            left = right;
        }
        
        return res;
    }
};