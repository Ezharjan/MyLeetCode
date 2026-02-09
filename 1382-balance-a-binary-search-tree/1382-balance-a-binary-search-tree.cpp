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
private:
    std::vector<TreeNode*> sortedNodes;

    // Helper 1: Perform in-order traversal to collect nodes in sorted order
    void inorderTraversal(TreeNode* root) {
        if (root == nullptr) {
            return;
        }
        inorderTraversal(root->left);
        sortedNodes.push_back(root);
        inorderTraversal(root->right);
    }

    // Helper 2: Recursively build a balanced BST from the sorted array
    TreeNode* buildBalancedBST(int start, int end) {
        if (start > end) {
            return nullptr;
        }

        // Pick the middle element to be the root of this subtree
        int mid = start + (end - start) / 2;
        TreeNode* node = sortedNodes[mid];

        // Recursively build the left and right subtrees
        node->left = buildBalancedBST(start, mid - 1);
        node->right = buildBalancedBST(mid + 1, end);

        return node;
    }

public:
    TreeNode* balanceBST(TreeNode* root) {
        // Step 1: Flatten the tree into a sorted list of nodes
        sortedNodes.clear(); // Ensure vector is empty before starting
        inorderTraversal(root);

        // Step 2: Rebuild the tree from the sorted list
        return buildBalancedBST(0, sortedNodes.size() - 1);
    }
};