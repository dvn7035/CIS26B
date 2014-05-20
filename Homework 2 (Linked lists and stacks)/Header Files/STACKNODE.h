#ifndef STACKNODE_H_INCLUDED
#define STACKNODE_H_INCLUDED

typedef struct stacknode STACKNODE;
struct stacknode
{
    int points;
    STACKNODE* next;
};

STACKNODE* push(STACKNODE* stack, int data);
STACKNODE* pop(STACKNODE** stack);


/***************************************************
    Stack Insert: insert in the beginning
*/
STACKNODE* push(STACKNODE* stack, int data)
{
    STACKNODE* pnew;

    pnew = (STACKNODE*) malloc(sizeof (STACKNODE));
    if (!pnew)
    {
        printf("... error in push!\n");
        exit(1);
    }
    pnew->points = data;
    pnew->next = stack;
    stack = pnew;

    return stack;
}

/***************************************************
    Stack Delete: delete the first node
*/
STACKNODE* pop(STACKNODE** stack)
{
    STACKNODE* first;

    if (*stack == NULL) return NULL;
    first = *stack;
    *stack = (*stack)->next;
    first->next = NULL;

    return first;
}
#endif // STACKNODE_H_INCLUDED
