#include <vector>
#include <stack>

using namespace std;

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 * int val;
 * TreeNode *left;
 * TreeNode *right;
 * TreeNode() : val(0), left(nullptr), right(nullptr) {}
 * TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 * TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> s;
        TreeNode* current = root;

        // Continue if there are nodes to visit or nodes waiting in the stack
        while (current != nullptr || !s.empty()) {
            
            // Step 1: Reach the leftmost node of the current subtree
            while (current != nullptr) {
                s.push(current);
                current = current->left;
            }

            // Step 2: Current is nullptr, so we backtrack.
            // Pop the top node (which is the parent of the nullptr we just hit)
            current = s.top();
            s.pop();

            // Add the node's value to our result list
            result.push_back(current->val);

            // Step 3: Move to the right subtree to continue the traversal
            current = current->right;
        }

        return result;
    }
};