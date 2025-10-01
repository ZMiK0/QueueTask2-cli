typedef struct node {
    char* data;
    struct node* next;
} node;

typedef struct LinkedList {
    struct node* head;
} LinkedList;

node* create(char* data, node* next);
void prepend(LinkedList* list, char* data);
int count(node *head);
node *push(node* head, char* data);
node *pushByIndex(node* head, char* data, int n);
char* get(node* head, int n);
int removeByIndex(node* head, int n);
node* removeHead(node* head);
void traverseList(node* head);
node* purge(node* head);