#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "Trie.h"
#include "Stack.h"

/* Initialising Trie
 * and creating its root
 * root's value is
 * initialised with '\0'
*/
void Trie(struct TrieNode **root){

    *root = (struct TrieNode*)malloc(sizeof(struct TrieNode));

    if(*root == NULL){
        printf("Malloc Error\n");
        return;
    }

    (*root)->val = '\0';
    (*root)->sibling = NULL;
    (*root)->child = NULL;
    (*root)->PList = NULL;
}

/*
 * Root has the value of '/0'
 * So insertion starts from root's child
 * After inserting all the letters of a word
 * We construct a '/0' node to symbolise the end of string
 * This node contains also the posting list
*/
void Insert(struct TrieNode** root, char* value, int line, char* word){

    if(Search(*root,value,line) == 1)
        return;

    if(*root == NULL){
        printf("NULL Root");
        return;
    }

    struct TrieNode *temp = (*root)->child;

    if(temp == NULL){               //Ιnserting first node
        temp = *root;
        while(*value){

            temp->child = (struct TrieNode*)malloc(sizeof(struct TrieNode));
            if(temp->child == NULL){
                printf("Malloc Error\n");
                return;
            }
            temp->child->val = *value;
            temp->child->sibling = NULL;
            temp->child->child = NULL;
            temp->child->PList = NULL;

            temp = temp->child;
            value++;
        }
        temp->child = (struct TrieNode*)malloc(sizeof(struct TrieNode));
        if(temp->child == NULL){
            printf("Malloc Error\n");
            return;
        }
        temp->child->val = '\0';
        temp->child->sibling = NULL;
        temp->child->child = NULL;
        temp->child->PList = (struct Posting_List*)malloc(sizeof(struct Posting_List));
        if(temp->child->PList == NULL){
            printf("Malloc Error\n");
            return;
        }
        temp->child->PList->frequency = 1;
        temp->child->PList->line = line;
        temp->child->PList->next = NULL;
    }
    else{

        while(*value == temp->val){   //Traversing trie until we find a different letter
            value++;
            temp = temp->child;
        }

        while(temp->sibling!=NULL){
            if(temp->sibling->val == *value){
                value++;
                Insert(&(temp->sibling),value,line,word);    //recalling this function using matching node as root
                return;
            }
            temp = temp->sibling;
        }

        temp->sibling = (struct TrieNode*)malloc(sizeof(struct TrieNode));      //haven't found a match above
        if(temp->sibling == NULL){
            printf("Malloc Error\n");
            return;
        }
        temp->sibling->val = *value;
        temp->sibling->sibling = NULL;
        temp->sibling->child = NULL;
        temp->sibling->PList = NULL;


        if(!(*value)){                //if value == '\0' nothing more to insert

            temp->sibling->PList = (struct Posting_List*)malloc(sizeof(struct Posting_List));
            if(temp->sibling->PList == NULL){
                printf("Malloc Error\n");
                return;
            }
            temp->sibling->PList->frequency = 1;
            temp->sibling->PList->line = line;
            temp->sibling->PList->next = NULL;
            return;
        }

        value++;                    //haven't reach the end of string, move to next letter
        temp = temp->sibling;

        while(*value){
            temp->child = (struct TrieNode*)malloc(sizeof(struct TrieNode));
            if(temp->child == NULL){
                printf("Malloc Error\n");
                return;
            }
            temp->child->val = *value;
            temp->child->sibling = NULL;
            temp->child->child = NULL;
            temp->child->PList = NULL;

            temp = temp->child;
            value++;
        }

        temp->child = (struct TrieNode*)malloc(sizeof(struct TrieNode));
        if(temp->child == NULL){
            printf("Malloc Error\n");
            return;
        }
        temp->child->val = '\0';
        temp->child->sibling = NULL;
        temp->child->child = NULL;

        temp->child->PList = (struct Posting_List*)malloc(sizeof(struct Posting_List));
        if(temp->child->PList == NULL){
            printf("Malloc Error\n");
            return;
        }
        temp->child->PList->frequency = 1;
        temp->child->PList->line = line;
        temp->child->PList->next = NULL;
    }
}

/* Searching for a specific string
 * If found 2nd time at the same doc
 * we increase frequency
 * else we construct a new node at PList
 */
int Search(struct TrieNode *root, const char* value, int line){

    struct TrieNode *curr = root->child;
    int flag = 0;
    while(flag == 0){
        struct TrieNode *temp = curr;
        struct TrieNode *prev = NULL;

        while(temp != NULL){
            if(temp->val == *value){
                prev = temp;
                break;
            }
            temp = temp->sibling;
        }

        if(prev == NULL)
            return 0;

        if(*value == '\0'){
            flag =  1;

            if(prev->PList->line == line)
                prev->PList->frequency++;

            else {
                struct Posting_List *first = prev->PList;
                while(prev->PList->next!=NULL){
                    if(prev->PList->next->line == line){
                        prev->PList->next->frequency++;
                        prev->PList = first;
                        return 1;
                    }
                    prev->PList = prev->PList->next;
                }

                prev->PList->next = (struct Posting_List*)malloc(sizeof(struct Posting_List));
                prev->PList->next->frequency = 1;
                prev->PList->next->line = line;
                prev->PList->next->next = NULL;
                prev->PList = first;
            }
        }
        curr = prev->child;
        value++;
    }
    if(flag == 1)
        return 1;
    else
        return 0;
}

/* Searching for term and document frequency
 * of a specific word
 */
struct Score TrieSearch(struct TrieNode *root, const char* value, int line){

    struct TrieNode *curr = root->child;
    struct Score NewScore;

    int flag = 0;
    NewScore.w_fr=0;
    NewScore.doc_fr=0;

    while(flag == 0){

        struct TrieNode *temp = curr;
        struct TrieNode *prev = NULL;

        while(temp != NULL){
            if(temp->val == *value){
                prev = temp;
                break;
            }
            temp = temp->sibling;
        }

        if(prev == NULL)
            return NewScore;

        if(*value == '\0'){
            flag = 1;

            struct Posting_List *first = prev->PList;
            while(prev->PList!=NULL){
                NewScore.doc_fr++;
                if(prev->PList->line == line){
                    NewScore.w_fr = prev->PList->frequency;
                }
                prev->PList = prev->PList->next;
            }
            prev->PList = first;
        }

        curr = prev->child;
        value++;
    }
    return NewScore;
}


void TrieDestroy (struct TrieNode* root){

    if(root->sibling!=NULL)
        TrieDestroy(root->sibling);

    if(root->child!=NULL)
        TrieDestroy(root->child);

    if(root->val == '\0'){
        struct Posting_List* next;
        struct Posting_List* curr = root->PList;
        while (curr != NULL){
            next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(root);
}

/*Prints word's document frequency*/
void printNode(struct TrieNode *p){

    int count=0;
    struct Posting_List *first = p->PList;
    while (p->PList != NULL){
        count++;
        p->PList = p->PList->next;
    }
    p->PList = first;
    printf(" %d\n", count);
}

/* Traversing Trie
  *First we push every child node into a Stack
  *If we reach a '\0' we print the items of the stack
  *Every time we visit a sibling node or we return from recursion
  *we  pop an item from Stack
 */
void printSubTrie(struct TrieNode *p, Stack** head, Stack** rear){

    Push(head,rear,p->val);
    if(p->child!=NULL)
        printSubTrie(p->child,head,rear);

    if(p->val == '\0'){
        PrintStack(head);
        printNode(p);
    }

    if(p->sibling!=NULL){
        Pop(head,rear);
        printSubTrie(p->sibling,head,rear);
    }
    else
        Pop(head,rear);
}

void Print(struct TrieNode *root){
   printf("\n");
   struct TrieNode* p = root;
   Stack *head = NULL;
   Stack *rear = NULL;
   printSubTrie(root->child,&head,&rear);
   root = p;
   printf("\n");
}
