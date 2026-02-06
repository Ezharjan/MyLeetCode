#include <stack>
#include <string>

using namespace std;

class Solution {
public:
    bool isValid(string s) {
        // Optimization: An odd-length string can never be valid
        if (s.length() % 2 != 0) return false;

        stack<char> st;

        for (char c : s) {
            // If it's an opening bracket, push to stack
            if (c == '(' || c == '{' || c == '[') {
                st.push(c);
            } 
            // If it's a closing bracket
            else {
                // If stack is empty, there is no opening bracket for this closing one
                if (st.empty()) return false;

                char top = st.top();

                // Check for mismatch
                if ((c == ')' && top != '(') ||
                    (c == '}' && top != '{') ||
                    (c == ']' && top != '[')) {
                    return false;
                }

                // If matched, pop the opening bracket
                st.pop();
            }
        }

        // If stack is empty, all brackets were matched correctly
        return st.empty();
    }
};