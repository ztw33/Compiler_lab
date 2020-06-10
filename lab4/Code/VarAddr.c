#include "VarAddr.h"
#include <stdlib.h>
#include <stdio.h>

VarAddrTable initAddrTable() {
    int size = HASH_SIZE;
    VarAddrTable hs = (VarAddrTable)malloc(sizeof(VarAddrTable));
    if (hs == NULL) return NULL;
    hs->size = size;
    hs->buckets = (VarBucket*)malloc(sizeof(VarBucket) * size);
    for (int i = 0; i < size; i++) {
        hs->buckets[i].varList = NULL;
    }
    return hs;
}

int getOffset(VarAddrTable table, Variable* var) {
    unsigned int hash = var->id % table->size;
    VarNode* listhead = table->buckets[hash].varList;
    if (listhead == NULL) {
        return 0;  // 不在其中
    } else {
        for (VarNode* p = listhead; p != NULL; p = p->next) {
            if (p->var->id == var->id && p->var->kind == var->kind) {
                return p->stackOffset;
            }
        }
        return 0;
    }
}

void insertVar(VarAddrTable table, Variable* var, int offset) {
    unsigned int hash = var->id % table->size;
    VarNode* listhead = table->buckets[hash].varList;
    if (listhead == NULL) {
        listhead = (VarNode*)malloc(sizeof(VarNode));
        listhead->var = var;
        listhead->stackOffset = offset;
        listhead->next = NULL;
        table->buckets[hash].varList = listhead; 
    } else {
        VarNode* p = listhead;
        for (; p->next != NULL; p = p->next) {
            if (p->var->id == var->id && p->var->kind == var->kind) {
                fprintf(stderr, "\033[31mERROR when insert into VarAddrTable! Var is already in the table.\033[0m\n");
                exit(1);
            }
        }
        if (p->var->id == var->id && p->var->kind == var->kind) {
            fprintf(stderr, "\033[31mERROR when insert into VarAddrTable! Var is already in the table.\033[0m\n");
            exit(1);
        }
        VarNode* newNode = (VarNode*)malloc(sizeof(VarNode));
        newNode->var = var;
        newNode->stackOffset = offset;
        newNode->next = NULL;
        p->next = newNode;
    }
}