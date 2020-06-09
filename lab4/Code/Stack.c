#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>

/* 创建一个空栈 */
Stack create_stack() {
    Stack S;
    S = (Stack)malloc(sizeof(StackNode));
    S->next = NULL;
    return S;
}

/* PUSH 操作 */
void push_stack(Stack s, int data) {
    // 新建一个结点，用于存放压入栈内的元素，即新的栈顶
    StackNode* head_node = (StackNode*)malloc(sizeof(StackNode));
    head_node->data = data;            // 添加数据
    head_node->next = s->next;        // 新的栈顶 head_node 的 next 指针指向原来的栈顶 s->next
    s->next = head_node;            // s->next 现在指向新的栈顶
}

/* POP 操作 */
int pop_stack(Stack s) {
    StackNode* head_node = (StackNode*)malloc(sizeof(StackNode));

    // 先判断栈是否为空，若栈为空，则不能再进行出栈操作，报错
    if (stack_is_empty(s)) {
        fprintf(stderr, "\033[31mERROR in pop_stack! Stack is empty.\033[0m\n");
        exit(1);
    } else {
        head_node = s->next;            // head_node 为栈顶
        int data = head_node->data;
        s->next = head_node->next;        // s->next 指向 head_node->next ，即新的栈顶
        free(head_node);                // 释放原来栈顶元素所占的内存
        return data;
    }
}

/* 判断栈是否为空 */
bool stack_is_empty(Stack s) {
    return s->next == NULL;
}