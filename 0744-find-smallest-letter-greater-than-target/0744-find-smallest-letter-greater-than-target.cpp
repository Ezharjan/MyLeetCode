class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        int n = letters.size();
        int left = 0;
        int right = n - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (letters[mid] <= target) {
                // If mid is smaller or equal, we need to look in the right half
                // to find a character strictly greater.
                left = mid + 1;
            } else {
                // If mid is greater, it's a candidate, but we look left
                // to see if there is a smaller valid character.
                right = mid - 1;
            }
        }

        // If 'left' goes out of bounds (== n), it means target is >= all letters.
        // In that case, left % n becomes 0, returning the first element (wrap around).
        return letters[left % n];
    }
};