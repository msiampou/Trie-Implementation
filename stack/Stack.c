#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Stack.h"

/* Head points to the bottom of the Stack
 * and rear on top
 */
void Push(Stack** head, Stack** rear, char val){

    if(*head==NULL){
        (*head) = (Stack*)malloc(sizeof(Stack));
        (*head)->letter = val;
        (*head)->next = NULL;
        (*rear) = (*head);
        (*head)->prev = NULL;
    }
    else{
        Stack *temp = (Stack*)malloc(sizeof(Stack));
        temp->letter = val;
        temp->next = NULL;

        (*rear)->next = temp;
        temp->prev = (*rear);
        (*rear) = temp;
    }
}

void Pop(Stack** head, Stack** rear){

    if(head!=NULL){
        Stack* temp = (*rear);
        if((*rear)==(*head))
            (*head) = NULL;

        (*rear) = temp->prev;
        free(temp);

        if((*rear)!=NULL)
            (*rear)->next=NULL;
    }
}

void PrintStack(Stack** head){
    Stack* ptr = (*head);
    while(ptr!=NULL){
        printf("%c",ptr->letter);
        ptr = ptr->next;
    }
}
