#ifndef __OPTIMIZE_IR__
#define __OPTIMIZE_IR__

#include "InterCode.h"
#include "Block.h"

void optimizeIR(InterCodes* codes);
void optimizeBlock(InterCodes* begin, InterCodes* end);
void optimizeGOTO(BlockList* blockList, LabelInfo* labels);

#endif