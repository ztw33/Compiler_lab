#include "optimizeIR.h"
#include <stddef.h>
#include <stdio.h>

#define debug 1
void optimizeIR(InterCodes* codes) {
    printf("optimize\n");
    BlockList* blockList = divToBlocks(codes);
    if(debug) printBlocks(blockList);
    for (BlockList* p = blockList; p != NULL; p = p->nextBlock) {
        optimizeBlock(p->beginCode, p->endCode);
    }
}

void optimizeBlock(InterCodes* begin, InterCodes* end) {

}
