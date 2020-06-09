#ifndef __STACK__
#define __STACK__

#include <stdbool.h>

typedef struct stack_node_ StackNode;
typedef StackNode* Stack;

struct stack_node_ {
    int data;
    StackNode* next;
};

Stack create_stack();
void push_stack(Stack s, int data);
int pop_stack(Stack s);
bool stack_is_empty(Stack s);

#endif