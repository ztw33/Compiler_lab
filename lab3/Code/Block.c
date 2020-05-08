#include "Block.h"
#include <stddef.h>
#include <stdlib.h>

BlockList* divToBlocks(InterCodes* codesListHead) {
    InterCodes* p = codesListHead;
    BlockList* blocksHead = NULL;
    BlockList* currentBlock = NULL;
    while (p != NULL) {
        InterCode* code = p->code;
        if (code->kind == FUNCTION) { // 新块
            if (blocksHead == NULL) {
                currentBlock = (BlockList*)malloc(sizeof(BlockList));
                currentBlock->beginCode = p;
                currentBlock->endCode = p;
                currentBlock->nextBlock = NULL;
                blocksHead = currentBlock;
            } else if (currentBlock->beginCode == NULL) {
                currentBlock->beginCode = p;
                currentBlock->endCode = p;
            } else {
                currentBlock->endCode = p->prev;
                BlockList* newBlock = (BlockList*)malloc(sizeof(BlockList));
                currentBlock->nextBlock = newBlock;
                newBlock->beginCode = p;
                newBlock->endCode = p;
                newBlock->nextBlock = NULL;
                currentBlock = newBlock;
            }
        } else if (code->kind == GOTO || code->kind == RETURN) {
            // 块结尾
            if (currentBlock->beginCode == NULL) {
                currentBlock->beginCode = p;
            }
            currentBlock->endCode = p;
            if (p->next != NULL) {
                BlockList* newBlock = (BlockList*)malloc(sizeof(BlockList));
                currentBlock->nextBlock = newBlock;
                newBlock->beginCode = NULL;
                newBlock->endCode = NULL;
                newBlock->nextBlock = NULL;
                currentBlock = newBlock;
            }
        } else {
            if (currentBlock->beginCode == NULL) {
                currentBlock->beginCode = p;
            }
            currentBlock->endCode = p;
        }
        p = p->next;
    }
    return blocksHead;
}

void printBlocks(BlockList* blocks) {
    BlockList* p = blocks;
    while (p != NULL) {
        printf("[BLOCK]\n");
        InterCodes* pp = p->beginCode;
        while (pp != p->endCode) {
            printCode(pp->code);
            pp = pp->next;
        }
        printCode(p->endCode->code);
        p = p->nextBlock;
    }
}