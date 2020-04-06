#include "Symbol.h"
#include <stdio.h>
#include <stdlib.h>

Symbol* createSymbol(char* name, SymbolKind kind) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    symbol->name = name;
    symbol->symbolKind = kind;
    switch (kind) {
    case VAR:
    case FIELD:
    case STRUCT:
        symbol->type = NULL;
        break;
    case FUNC:
        symbol->funcSign = (FuncSign*)malloc(sizeof(FuncSign));
        break;
    default:
        fprintf(stderr, "\033[31mERROR when creating symbol! Wrong symbol kind.\033[0m\n");
        return NULL;
    }
}