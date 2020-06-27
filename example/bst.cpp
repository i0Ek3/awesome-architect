#include <iostream>

// 在二叉搜索树中实现搜索操作

TreeNode* search(TreeNode* root, int val) {
    TreeNode* cur = &root;
    if (val == cur->val) return cur;
    else if (val < cur->val) {
        search(root->left, val);
    } else {
        search(root->right, val);
    }
    return cur;
}

// 在二叉搜索树中实现插入操作
TreeNode* insert(TreeNode* root, int val) {
    if (root == nullptr) return nullptr;
    if (val > root->val) {
        insert(root->right, val);
    } else if (val < root->val) {
        insert(root->left, val);
    } else {
        return root;
    }
    return root;
}

// 在二叉搜索树中实现删除操作
TreeNode* delete(TreeNode* root, int val) {
    if (cur->left == nullptr && cur->right == nullptr) delete cur;
    else if (cur->left ) {

    }else if (cur->right) {
        
    } else {
        cur = cur->next->next;
        cur->next->next = cur->next;
        delete cur;
    }
    while (cur->val == val) {
        if (cur->left == nullptr && cur->right == nullptr) delete cur;
        else if (cur->left || cur->right) {
            cur = cur->next;
            cur->next = cur;
            delete cur;
        } else {
            cur->next = cur; 
        }
    }
}


//实现一个二叉搜索树迭代器
class BSTIterator {
public:
    BSTIterator(TreeNode* root) {

    }

    int next() {

    }

    bool hasNext() {

    }
};


bool isBST(TreeNode* root) {
    if (root == nullptr) return nullptr;
    TreeNode *left = nullptr;
    TreeNode *right= nullptr;
    while (root && left && right) {
        if (root->left->val < root->val) {
            isBST(root->left);
        } else if (root->right->val > root->val) {
            isBST(root->right)
        }
    }
    return false;
}


int main()
{
    std::cout << "Hello world" << std::endl;
    return 0;
}

