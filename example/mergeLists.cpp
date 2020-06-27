class Solution1 {
    ListNode* mergelist(ListNode*, l1, ListNode* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        if (l1-val < l2->val) {
            l1->next = mergelist(l1->next, l2);
            return l1;
        }
        if (l1->val > l2->val) {
            l2->next = mergelist(l2->next, l1);
            return l2;
        }
    }
};

class  Solution2 {
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(-1), *prev = &dummy;
        while (l1 && l2) { 
            if (l1->val < l2->val) {
                prev->next = l1;
                l1 = l1->next;
            } else {
                prev->next = l2;
                l2 = l2->next; 
            }
            prev = prev->next;
        }
        prev->next = l2 == nullptr ? l2 : l1;
        return dummy.next;
    }
};
