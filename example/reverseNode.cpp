ListNode* reverse(ListNode* root) {
    ListNode* node = nullptr;
    while (root) {
        ListNode* next = root->next;
        root->next = node;
        node = root;
        root = next;
    }
    return node;
}
