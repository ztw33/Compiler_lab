#ifndef __IR__
#define __IR__

#include "InterCode.h"
#include "SyntaxTree.h"
#include <stdio.h>
#include "Symbol.h"

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
void translateParamDec(const Node* ParamDec);
void translateStmtList(const Node* StmtList);
void translateStmt(const Node* Stmt);
Operand* translateExp(const Node* Exp);
void translateCondExp(const Node* Exp, int trueLabelID, int falseLabelID);
void translateArgs(const Node* Args);
void translateDefList(const Node* DefList);
void translateDef(const Node* Def);
void translateDecList(const Node* DecList, Type* type);
void translateDec(const Node* Dec, Type* type);
Operand* translateVarDec(const Node* VarDec, Type* type);

#endif