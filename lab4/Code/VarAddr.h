#ifndef __VARADDR__
#define __VARADDR__

#include "HashSet.h"
#include "InterCode.h"

typedef struct VarHashSet_* VarAddrTable;
typedef struct VarNode_ VarNode;

struct VarNode_ {
    Variable* var;
    int stackOffset;  // 在栈中相对于$fp的偏移量
    VarNode* next;
};

typedef struct VarBucket_ {
    VarNode* varList;
} VarBucket;

struct VarHashSet_ {
    int size;
    VarBucket* buckets;
};

VarAddrTable initAddrTable();
int getOffset(VarAddrTable table, Variable* var);
void insertVar(VarAddrTable table, Variable* var, int offset);

#endif