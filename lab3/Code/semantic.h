#ifndef __SEMANTIC__
#define __SEMANTIC__

#include "SyntaxTree.h"
#include "Symbol.h"
#include "HashSet.h"

// 全局变量
HashSet symbolTable;
Type typeInt;
Type typeFloat;
#define TYPE_INT &typeInt
#define TYPE_FLOAT &typeFloat
int unnamedCount;

void sementicAnalysis(const Node* syntaxTreeRootNode);
void analyseProgram(const Node* Program);
void analyseExtDefList(const Node* ExtDefList);
void analyseExtDef(const Node* ExtDef);
void analyseExtDecList(const Node* ExtDecList, Type* type);
Type* analyseSpecifier(const Node* Specifier);
Type* analyseTYPE(const Node* TYPE);
Type* analyseStructSpecifier(const Node* StructSpecifier);
Symbol* analyseFunDec(const Node* FunDec, Type* retType);
void analyseCompSt(const Node* CompSt, Symbol* func);
Field* analyseDefList(const Node* DefList, SymbolKind kind, int offset);
Field* analyseDef(const Node* Def, SymbolKind kind, int offset);
SymbolList* analyseDecList(const Node* DecList, SymbolKind kind, Type* type);
Symbol* analyseDec(const Node* Dec, SymbolKind kind, Type* type);
Symbol* analyseVarDec(const Node* VarDec, SymbolKind kind, Type* type);
Type* analyseExp(const Node* Exp);
void analyseStmtList(const Node* StmtList, Symbol* func);
void analyseStmt(const Node* Stmt, Symbol* func);
Param* analyseArgs(const Node* Args);
Param* analyseVarList(const Node* VarList);
Param* analyseParamDec(const Node* ParamDec);

bool equalString(char* s1, char* s2);
bool usedThisProd(const Node* father, int nodeNum, ...);

void printSemanticError(int errorType, int lineNum, char* msg);

#endif