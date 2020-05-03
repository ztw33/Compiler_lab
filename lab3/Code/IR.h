#ifndef __IR__
#define __IR__

#include "InterCode.h"
#include <stddef.h>

InterCodes* codesHead = NULL;
InterCodes* codesTail = NULL;

void addCode(InterCode* code);
void addCodes(CodesListHT codes);

void outputIR(FILE* file, InterCodes* codes);
char* getVarName(Variable* var);
char* getOperandName(Operand* op);
char* getRelop(CondExp* cond);

InterCodes* generateIR(const Node* syntaxTreeRootNode);

#endif