#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
} node;

node* create(int data, node* next)
{
    node* new_node = (node*)malloc(sizeof(node));

    if (new_node == NULL)
    {
        printf("Error creating node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;
    return (new_node);
}

node* prepend(node* head, int data)
{
    node* new_node = create(data,NULL);
    head = new_node;

    return (head);
}

int count(node *head)
{
    node *cursor = head;
    int i;

    i = 0;
    while(cursor != NULL)
    {
        i++;
        cursor = cursor->next;
    }
    return (i);
}

node *push(node* head, int data)
{
    node *cursor = head;
    while (cursor->next != NULL)
        cursor = cursor->next;
    node* new_node = create(data,NULL);
    cursor->next = new_node;
    return (head);
}

int get(node* head, int n)
{
    node *cursor = head;
    int i = 0;

    while ((cursor->next != NULL) && (i != n))
    {
        cursor = cursor->next;
        i++;
    }
    return (cursor->data);
}

int removeByIndex(node* head, int n)
{
    node *cursor = head;
    node *temp;
    int i = 0;

    if (head == NULL) return 0;
    if (n == 0) return 0;
    while ((cursor->next != NULL) && (i != n-1))
    {
        cursor = cursor->next;
        i++;
    }
    temp = cursor->next->next;
    free(cursor->next);
    cursor->next = temp;
    return (1);
}

node* removeHead(node* head)
{
    node *temp;

    temp = head;
    head = head->next;
    free(temp);
    return head;
}

/*
int main()
{
    node* head = prepend(head, 2);

    push(head, 5);
    push(head, 8);
    push(head, 6);
    push(head, 64);
    printf("%d\n", get(head, 0));
    printf("%d\n", removeByIndex(head, 0));
    head = removeHead(head);
    printf("%d\n", get(head, 0));
    printf("%d\n", get(head, 1));
    printf("%d\n", get(head, 2));
    return 0;
}
*/