#ifndef __HASHSET__
#define __HASHSET__

#include "Symbol.h"
#include <stdbool.h>

#define HASH_SIZE 16384

typedef struct HashSet_* HashSet;

typedef struct Bucket_ {
    SymbolList* symbolList;
} Bucket;

struct HashSet_ {
    int size;
    Bucket* buckets; // 数组
};

HashSet initHashSet(int size);
bool contains(HashSet set, char* name, SymbolKind kind);
void insert(HashSet set, Symbol* symbol);
Symbol* get(HashSet set, char* name, SymbolKind kind);

unsigned int hash_pjw(char* name);

#endif