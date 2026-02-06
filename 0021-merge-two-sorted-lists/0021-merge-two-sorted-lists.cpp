/**
 * Definition for singly-linked list.
 * struct ListNode {
 * int val;
 * ListNode *next;
 * ListNode() : val(0), next(nullptr) {}
 * ListNode(int x) : val(x), next(nullptr) {}
 * ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        // Create a dummy node to act as the anchor for the result list.
        // Using a stack-allocated node avoids memory management issues.
        ListNode dummy(0);
        ListNode* tail = &dummy;

        // Iterate while both lists have nodes
        while (list1 != nullptr && list2 != nullptr) {
            if (list1->val <= list2->val) {
                tail->next = list1;       // Attach list1 node
                list1 = list1->next;      // Advance list1
            } else {
                tail->next = list2;       // Attach list2 node
                list2 = list2->next;      // Advance list2
            }
            tail = tail->next;            // Advance the tail of the merged list
        }

        // If one list is not empty, attach the rest of it to the tail.
        // We don't need a loop here because the rest of the list is already connected.
        if (list1 != nullptr) {
            tail->next = list1;
        } else if (list2 != nullptr) {
            tail->next = list2;
        }

        // Return the next node of dummy, which is the actual head of the merged list.
        return dummy.next;
    }
};