#include "optimizeIR.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define debug 0

void optimizeIR(InterCodes* codes) {
    if (debug) printf("optimize\n");
    LabelInfo* labelsInfo = (LabelInfo*)calloc(labelID + 1, sizeof(LabelInfo)); //label信息，一维数组
    for (int i = 0; i < labelID + 1; i++) {
        labelsInfo[i].block = NULL;
        labelsInfo[i].labelID = i;
        labelsInfo[i].refCount = 0;
    }
    BlockList* blockList = divToBlocks(codes, labelsInfo);
    //if(debug) printBlocks(blockList);
    for (BlockList* p = blockList; p != NULL; p = p->nextBlock) {
        optimizeBlock(p->beginCode, p->endCode);
    }
    if (debug) {
        printf("optimizeBlock完成\n");
        printBlocks(blockList);
        printf("optimizeGOTO\n");
    }
    // optimizeGOTO(blockList, labelsInfo);
    if(debug) printBlocks(blockList);
}

void optimizeBlock(InterCodes* begin, InterCodes* end) {
    for (InterCodes* p = begin; p != end; p = p->next) {
        InterCode* code = p->code;
        if ((code->kind == ADD || code->kind == SUB || code->kind == MUL || code->kind == DIV) && code->binOp.result->kind == VARIABLE && code->binOp.result->var->kind == T) {
            InterCode* nextCode = p->next->code;
            if (nextCode->kind == ASSIGN && nextCode->assign.right->kind == VARIABLE && nextCode->assign.right->var->kind == T && nextCode->assign.right->var->id == code->binOp.result->var->id && nextCode->assign.left->kind == VARIABLE) {
                code->binOp.result = nextCode->assign.left;
                removeCode(p->next);
            }
        }
    }
}

// 有bug，未完成
void optimizeGOTO(BlockList* blockList, LabelInfo* labelsInfo) {
    if (blockList == NULL) return;
    BlockList* currentBlock = blockList;
    if (debug) {
        printf("current Block begin code:");
        printCode(currentBlock->beginCode->code);
        printf("current Block end code:");
        printCode(currentBlock->endCode->code);
    }
    if (debug) printf("ztw0\n");
    InterCodes* endCodes = currentBlock->endCode;
    if (debug) printf("ztw1\n");
    if (endCodes->code->kind == GOTO) {
        if (debug) printf("ztw2\n");
        int gotoLabelID = endCodes->code->gotoLabelID;
        if (debug) printf("ztw3, gotoLabelID=%d\n", gotoLabelID);
        BlockList* gotoBlock = labelsInfo[gotoLabelID].block;
        if (gotoBlock->beginCode->code->kind == LABEL && gotoBlock->beginCode->code->labelID == gotoLabelID && gotoBlock->nextBlock != NULL) {
            if (debug) {
                printf("gotoblock begin code:");
                printCode(gotoBlock->beginCode->code);
            } 
            InterCodes* code1 = endCodes->prev;
            InterCodes* code2 = endCodes->next;
            InterCodes* code3 = gotoBlock->beginCode->prev;
            InterCodes* code4 = gotoBlock->endCode->next;
            if (debug) printf("ztw4\n");
            code1->next = gotoBlock->beginCode;
            gotoBlock->beginCode->prev = code1;
            gotoBlock->endCode->next = code2;
            code2->prev = gotoBlock->endCode;
            code3->next = code4;
            code4->prev = code3;
            currentBlock->endCode = gotoBlock->endCode;
            gotoBlock->prevBlock->nextBlock = gotoBlock->nextBlock;
            labelsInfo[gotoLabelID].block = currentBlock;
            if (debug) printf("ztw5\n");
            free(endCodes->code);
            free(endCodes);
            labelsInfo[gotoLabelID].refCount -= 1;
            if (debug) printf("ztw6\n");
            if (labelsInfo[gotoLabelID].refCount == 0) {
                free(gotoBlock);
            }
            if (debug) {
                printf("-----------------------\n");
                printBlocks(blockList);
            }
            optimizeGOTO(currentBlock, labelsInfo);
        }
        if (debug) printf("ztw8\n");
    }
    optimizeGOTO(currentBlock->nextBlock, labelsInfo);
}
