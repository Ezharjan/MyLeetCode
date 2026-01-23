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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        // Use a dummy head to simplify handling the first node
        ListNode* dummyHead = new ListNode(0);
        ListNode* curr = dummyHead;
        int carry = 0;

        // Loop continues if either list has digits OR there is a remaining carry
        while (l1 != nullptr || l2 != nullptr || carry != 0) {
            // Get values, using 0 if we've reached the end of a list
            int x = (l1 != nullptr) ? l1->val : 0;
            int y = (l2 != nullptr) ? l2->val : 0;
            
            int sum = carry + x + y;
            
            // Update carry for the next iteration
            carry = sum / 10;
            
            // Create a new node with the digit part of the sum
            curr->next = new ListNode(sum % 10);
            
            // Advance pointers
            curr = curr->next;
            if (l1 != nullptr) l1 = l1->next;
            if (l2 != nullptr) l2 = l2->next;
        }

        // Return the actual head (skipping the dummy)
        ListNode* result = dummyHead->next;
        delete dummyHead; // Clean up the dummy node to prevent memory leaks
        return result;
    }
};