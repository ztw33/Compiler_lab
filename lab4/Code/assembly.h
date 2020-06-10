#ifndef __ASSEMBLY__
#define __ASSEMBLY__

#include "InterCode.h"
#include "AsmCode.h"
#include <stdio.h>
#include "VarAddr.h"

AsmCode* asmCodesHead;
AsmCode* asmCodesTail;

AsmCode* generateAsm(InterCodes* irs);
void outputAsm(FILE* file, AsmCode* codes);
void printAsmError(char* msg);
void addLoadCode(char* regName, int offset);
void addStoreCode(char* regName, int offset);
void addLoadImmCode(char* regName, int imm);
void handleOperand(Operand* op, char* reg, VarAddrTable addrTable);

#endif