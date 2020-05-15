#ifndef __BLOCK__
#define __BLOCK__

#include "InterCode.h"

typedef struct BlockList_ BlockList;
typedef struct CodeUsage_ CodeUsage;
typedef struct VarNode_ VarNode;
typedef struct VarList_ VarList;

struct BlockList_ {
    InterCodes* beginCode;
    InterCodes* endCode;
    BlockList* nextBlock;
    BlockList* prevBlock;
};

typedef enum NodeAttr { CONST_NODE, ADD_NODE, SUB_NODE, MUL_NODE, DIV_NODE } NodeAttr;
struct VarNode_ {
    VarList* varList;  // 此结点标识的变量列表
    bool isAbandoned;  // 是否被废弃了
    CodeUsage* relatedCodes;  // 与此结点相关的中间代码
    NodeAttr attr;
};

// 代码使用情况，表示一行代码是否会被使用到
struct CodeUsage_ {
    InterCode* code;
    bool isUsed;
    InterCode* nextCode;
};

struct VarList_ {
    Variable* var;
    Variable* nextVar;
};

typedef struct LabelInfo_ {
    int labelID;
    BlockList* block;  // 此label属于哪一个Block
    int refCount;  // goto到此label的引用次数
} LabelInfo;

BlockList* divToBlocks(InterCodes* codesListHead, LabelInfo* labelsInfo);

void printBlocks(BlockList* blocks);
#endif