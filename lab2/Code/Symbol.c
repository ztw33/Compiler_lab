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

void printType(Type* type) {
    char *k[] = {"BASIC", "ARRAY", "STRUCTURE"};
    printf("kind: %s\n", k[type->kind]);
    if (type->kind == BASIC) {
        char *b[] = {"INT", "FLOAT"};
        printf("basic: %s\n", b[type->basic]);
    } else if (type->kind == ARRAY) {

    } else {

    }
}