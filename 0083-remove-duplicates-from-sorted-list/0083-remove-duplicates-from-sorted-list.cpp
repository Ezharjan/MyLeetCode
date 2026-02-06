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
    ListNode* deleteDuplicates(ListNode* head) {
        // Edge case: Empty list or list with only one node
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        ListNode* current = head;

        // Traverse until we reach the last node
        while (current->next != nullptr) {
            // Check if current value is same as next value
            if (current->val == current->next->val) {
                ListNode* duplicate = current->next;
                
                // Skip the duplicate node
                current->next = current->next->next;
                
                // Optional: Free memory of the removed node (good practice in C++)
                delete duplicate; 
                // Note: We do NOT move 'current' forward here because 
                // the new 'next' node might also be a duplicate.
            } else {
                // Values are different, move to the next node
                current = current->next;
            }
        }

        return head;
    }
};