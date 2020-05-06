#include "Symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Symbol* createSymbol(char* name, SymbolKind kind) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    symbol->name = name;
    symbol->symbolKind = kind;
    symbol->alias = NULL;
    switch (kind) {
    case VAR:
    case FIELD:
    case STRUCT:
    case CONST:
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
    if (type == NULL) {
        printf("(null)\n");
        return;
    }
    char *k[] = {"BASIC", "ARRAY", "STRUCTURE"};
    printf("type kind: %s\n", k[type->kind]);
    printf("Rvalue: %s\n", type->Rvalue ? "yes" : "no");
    if (type->kind == BASIC) {
        char *b[] = {"INT", "FLOAT"};
        printf("basic: %s\n", b[type->basic]);
    } else if (type->kind == ARRAY) {
        printf("array size: %d\n", type->array.size);
        printf("array type: %s\n", k[type->array.elem->kind]);
        printType(type->array.elem);
    } else if (type->kind == STRUCTURE) {
        printf("struct name: %s\n", type->structure.name);
        printf("struct fields: \n");
        int i = 0;
        for (Field* f = type->structure.fields; f != NULL; f = f->next) {
            i++;
            printf("field %d name: %s\n", i, f->name);
            printf("field %d type: \n", i);
            printType(f->type);
        }
    } else {
        fprintf(stderr, "\033[31mERROR when print Type! Wrong type.\033[0m\n");
    }
}

int getSizeofType(Type* type) {
    if (type == NULL) {
        return 0;
    }
    if (type->kind == BASIC) {
        return 4;
    } else if (type->kind == ARRAY) {
        return type->array.size * getSizeofType(type->array.elem);
    } else if (type->kind == STRUCTURE) {
        int size = 0;
        for (Field* f = type->structure.fields; f != NULL; f = f->next) {
            size += getSizeofType(f->type);
        }
        return size;
    } else {
        fprintf(stderr, "\033[31mERROR in getSizeofType! Wrong type.\033[0m\n");
        return 0;
    }
}

int getFieldOffset(Type* type, char* fieldName) {
    if (type->kind != STRUCTURE) {
        fprintf(stderr, "\033[31mERROR in getFieldOffset! Wrong type.\033[0m\n");
        return 0;
    }
    for (Field* f = type->structure.fields; f != NULL; f = f->next) {
        if (strcmp(f->name, fieldName) == 0) {
            return f->offset;
        }
    }
    fprintf(stderr, "\033[31mERROR in getFieldOffset! No matched field.\033[0m\n");
    return 0;
}