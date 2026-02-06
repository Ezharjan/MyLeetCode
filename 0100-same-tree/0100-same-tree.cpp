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
    bool isSameTree(TreeNode* p, TreeNode* q) {
        // Case 1: Both nodes are null -> Trees are identical at this leaf
        if (p == nullptr && q == nullptr) {
            return true;
        }

        // Case 2: One is null but the other is not -> Structural mismatch
        if (p == nullptr || q == nullptr) {
            return false;
        }

        // Case 3: Both exist but values differ -> Value mismatch
        if (p->val != q->val) {
            return false;
        }

        // Recursion: Both left subtrees AND both right subtrees must match
        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
};