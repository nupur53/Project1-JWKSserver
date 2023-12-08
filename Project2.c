/*Author: Nupur Rishi (NupurRishi@my.unt.edu)
   EUID: nr0335
   Date: 10/31/2023
   Section: CSCE 3550.001
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A node in the doubly linked list
typedef struct Node {
    char data[50];
    struct Node* prev;
    struct Node* next;
} Node;

// Function to insert a node at the end of the Doubly Linked List
void insertNode(Node** head_ref, char new_data[]) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->data, new_data);
    new_node->prev = NULL;
    new_node->next = NULL;

    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    Node* last = *head_ref;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = new_node;
    new_node->prev = last;
}

// Function to print the Doubly Linked List
void printList(Node* node) {
    while (node != NULL) {
        printf(" %s ", node->data);
        node = node->next;
    }
}

// Function to free the memory occupied by the list
void freeList(Node** node) {
    Node* current = *node;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *node = NULL;
}

int main() {
    Node* head = NULL;

    insertNode(&head, "A");
    insertNode(&head, "B");
    insertNode(&head, "C");
    insertNode(&head, "D");
    insertNode(&head, "E");

    printf("Doubly Linked List:\n");
    printList(head);

    freeList(&head);

    return 0;
}