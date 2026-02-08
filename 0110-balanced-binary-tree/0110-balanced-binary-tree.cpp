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
    // Helper function: Returns the height of the tree if balanced, or -1 if unbalanced.
    int dfsHeight(TreeNode* node) {
        // Base case: An empty tree has height 0 and is balanced.
        if (node == nullptr) {
            return 0;
        }

        // Check left subtree
        int leftHeight = dfsHeight(node->left);
        if (leftHeight == -1) return -1; // Propagate unbalance

        // Check right subtree
        int rightHeight = dfsHeight(node->right);
        if (rightHeight == -1) return -1; // Propagate unbalance

        // Check current node balance
        if (abs(leftHeight - rightHeight) > 1) {
            return -1; // Current node is unbalanced
        }

        // Return actual height
        return max(leftHeight, rightHeight) + 1;
    }

    bool isBalanced(TreeNode* root) {
        // If dfsHeight returns -1, it's not balanced. Otherwise, it is.
        return dfsHeight(root) != -1;
    }
};