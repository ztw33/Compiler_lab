#ifndef __IR__
#define __IR__

#include "InterCode.h"
#include "SyntaxTree.h"
#include <stdio.h>

InterCodes* codesHead;
InterCodes* codesTail;

void addCode(InterCode* code);
void addCodes(CodesListHT codes);

void outputIR(FILE* file, InterCodes* codes);
char* getVarName(Variable* var);
char* getOperandName(Operand* op);
char* getRelop(CondExp* cond);

InterCodes* generateIR(const Node* syntaxTreeRootNode);
void translateProgram(const Node* Program);
void translateExtDefList(const Node* ExtDefList);
void translateExtDef(const Node* ExtDef);
void translateFunDec(const Node* FunDec);
void translateCompSt(const Node* CompSt);
void translateVarList(const Node* VarList);
void translateStmtList(const Node* StmtList);
void translateStmt(const Node* Stmt);
Operand* translateExp(const Node* Exp);
void translateCondExp(const Node* Exp, int trueLabelID, int falseLabelID);

#endif