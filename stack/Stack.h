#pragma once

typedef struct StackNode {
    char letter;
    struct StackNode *next;
    struct StackNode* prev;
}Stack;

/*Pushing an Item to Stack*/
void Push(Stack**,Stack**,char);

/*Popping an Item from Stack*/
void Pop(Stack**,Stack**);

/*Prints Stack*/
void PrintStack(Stack**);
