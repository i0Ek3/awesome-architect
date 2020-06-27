## LCA -> Lowest Common Anestor

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* LCA(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) {
        return root;
    }
    
    TreeNode* left = LCA(root->left, p, q);
    TreeNode* right = LCA(root->right, p, q);

    return !left ? right : !right ? left : root;
}





