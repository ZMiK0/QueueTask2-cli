typedef struct node {
    int data;
    struct node* next;
} node;

node* create(int data, node* next);
node* prepend(node* head, int data);
int count(node *head);
node *push(node* head, int data);
int get(node* head, int n);
int removeByIndex(node* head, int n);
node* removeHead(node* head);