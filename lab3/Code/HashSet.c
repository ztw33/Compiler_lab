#include "HashSet.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HashSet initHashSet(int size) {
    HashSet hs = (HashSet)malloc(sizeof(struct HashSet_));
    if (hs == NULL) return NULL;
    hs->size = size;
    hs->buckets = (Bucket*)malloc(sizeof(Bucket) * size);
    for (int i = 0; i < size; i++) {
        hs->buckets[i].symbolList = NULL;
    }
    return hs;
}

bool contains(HashSet set, char* name, SymbolKind kind) {
    unsigned int hash = hash_pjw(name);
    SymbolList* listhead = set->buckets[hash].symbolList;
    if (listhead == NULL) {
        return false;
    } else {
        for (SymbolList* p = listhead; p != NULL; p = p->next) {
            if (strcmp(p->symbol->name, name) == 0) {
                SymbolKind pkind = p->symbol->symbolKind;
                switch (kind) {
                case VAR:
                    if (pkind == VAR || pkind == FIELD || pkind == STRUCT) return true;
                    break;
                case FIELD:
                    if (pkind == VAR || pkind == FIELD) return true;
                    break;
                case STRUCT:
                    if (pkind == VAR || pkind == STRUCT) return true;
                    break;
                case FUNC:
                    if (pkind == FUNC) return true;
                    break;
                default:
                    break;
                }
            }
        }
        return false;
    }
}

void insert(HashSet set, Symbol* symbol) {
    unsigned int hash = hash_pjw(symbol->name);
    SymbolList* listhead = set->buckets[hash].symbolList;
    if (listhead == NULL) {
        listhead = (SymbolList*)malloc(sizeof(SymbolList));
        listhead->symbol = symbol;
        listhead->next = NULL;
        set->buckets[hash].symbolList = listhead; 
    } else {
        SymbolList* p = listhead;
        for (; p->next != NULL; p = p->next) {
            if (strcmp(p->symbol->name, symbol->name) == 0 && p->symbol->symbolKind == symbol->symbolKind) {
                fprintf(stderr, "\033[31mERROR when insert into HashSet! Symbol is already in the table.\033[0m\n");
            }
        }
        if (strcmp(p->symbol->name, symbol->name) == 0 && p->symbol->symbolKind == symbol->symbolKind) {
            fprintf(stderr, "\033[31mERROR when insert into HashSet! Symbol is already in the table.\033[0m\n");
        }
        SymbolList* newSymbol = (SymbolList*)malloc(sizeof(SymbolList));
        newSymbol->symbol = symbol;
        newSymbol->next = NULL;
        p->next = newSymbol;
    }
}

Symbol* get(HashSet set, char* name, SymbolKind kind) {
    unsigned int hash = hash_pjw(name);
    SymbolList* listhead = set->buckets[hash].symbolList;
    if (listhead == NULL) {
        return NULL;
    } else {
        for (SymbolList* p = listhead; p != NULL; p = p->next) {
            if (strcmp(p->symbol->name, name) == 0 && p->symbol->symbolKind == kind) {
                return p->symbol;
            }
        }
        return NULL;
    }
}


unsigned int hash_pjw(char* name) {
    unsigned int val = 0, i;
    for (; *name; ++name) {
        val = (val << 2) + *name;
        if (i = val & ~(HASH_SIZE-1)) val = (val ^ (i >> 12)) & (HASH_SIZE-1);
    }
    return val;
}
