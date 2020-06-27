class Solution {
public:
    TreeList GetNext(TreeList pNode) {
        if (pNode == nullptr) return nullptr;
        if (pNode->right != nullptr) {
            pNode = pNode->right;
            while (pNode->left != nullptr)
                pNode = pNode->left;
            return pNode;
        }
        while (pNode->next != nullptr) {
            if (pNode->next->left == pNode)
                return pNode->next;
            pNode = pNode->next;
        }
        return nullptr;
    }
};
