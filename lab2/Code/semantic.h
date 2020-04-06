#ifndef __SEMANTIC__
#define __SEMANTIC__

#include "SyntaxTree.h"
#include "Symbol.h"
#include "HashSet.h"

HashSet symbolTable;
int lastErrorLineno;


typedef struct Node Node;


void sementicAnalysis(const Node* syntaxTreeRootNode);
void analyseProgram(const Node* Program);
void analyseExtDefList(const Node* ExtDefList);
void analyseExtDef(const Node* ExtDef);
Type* analyseSpecifier(const Node* Specifier);
Type* analyseTYPE(const Node* TYPE);
Type* analyseStructSpecifier(const Node* StructSpecifier);
Symbol* analyseFunDec(const Node* FunDec, Type* retType);
void analyseCompSt(const Node* CompSt, Symbol* func);
void analyseDefList(const Node* DefList, SymbolKind kind);
void analyseDef(const Node* Def, SymbolKind kind);
SymbolList* analyseDecList(const Node* DecList, SymbolKind kind, Type* type);
Symbol* analyseDec(const Node* Dec, SymbolKind kind, Type* type);
Symbol* analyseVarDec(const Node* VarDec, SymbolKind kind, Type* type);
Type* analyseExp(const Node* Exp);
void analyseStmtList(const Node* StmtList);
void analyseStmt(const Node* Stmt);

bool equalString(char* s1, char* s2);

void printSemanticError(int errorType, int lineNum, char* msg);

#endif