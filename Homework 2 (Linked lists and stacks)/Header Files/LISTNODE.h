#ifndef LISTNODE_H_INCLUDED
#define LISTNODE_H_INCLUDED

#define DUMMY_TRAILER "~"  //All valid names will have an ASCII value under ~
#define MAXNAMESIZE 30


typedef struct listnode LISTNODE;
struct listnode
{
    char name[MAXNAMESIZE];
    STACKNODE* scores;
    LISTNODE* forw;
    LISTNODE* back;
};

LISTNODE* init_list(void);
bool insert(LISTNODE* list, char input_name[MAXNAMESIZE], int points);
void traverse_forw(LISTNODE* list);
void traverse_back(LISTNODE* list);

/*
Initialization of a circularly doubly-linked
list with one sentinel node
*/
LISTNODE* init_list(void)
{
    LISTNODE* list;

    // allocate the sentinel node
    list = (LISTNODE*) malloc(sizeof (LISTNODE));
    if (!list)
    {
        printf("Error in init_list!\n");
        exit(1);
    }
    strcpy (list->name, DUMMY_TRAILER); //list->name = DUMMY_TRAILER;
    list->scores = NULL;
    list->forw = list;
    list->back = list;
    return list;
}

/*
Insert a node in a sorted circularly
doubly-linked list with one sentinel node
    return 1 - if duplicate
    return 0 - otherwise
*/
bool insert (LISTNODE* list, char input_name[MAXNAMESIZE], int points)
{
    LISTNODE* curr = list->forw;
    LISTNODE* prev = list;
    LISTNODE* pnew;
    int   duplicate = true;

    // search
    while (strcmp(input_name,curr->name) > 0)
    {
        prev = curr;
        curr = curr->forw;
    }

    if (strcmp(input_name, curr->name) )
    {
        duplicate = false; // not a duplicate
        pnew = (LISTNODE *) malloc(sizeof (LISTNODE));
        pnew->scores = NULL;
        if (!pnew)
        {
            printf("Fatal memory allocation error in insert!\n");
            exit(3);
        }
        strcpy(pnew->name, input_name);
        pnew->scores = push(pnew->scores, points);  //Push in the new scores
        pnew->forw = curr;
        pnew->back = prev;
        prev->forw = pnew;
        curr->back = pnew;
    }
    return duplicate;
}

#endif // LISTNODE_H_INCLUDED
