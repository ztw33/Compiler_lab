#ifndef __ASSEMBLY__
#define __ASSEMBLY__

#include "InterCode.h"
#include "AsmCode.h"
#include <stdio.h>

AsmCode* asmCodesHead;
AsmCode* asmCodesTail;

AsmCode* generateAsm(InterCodes* irs);
void outputAsm(FILE* file, AsmCode* codes);

#endif