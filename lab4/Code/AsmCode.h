#ifndef __ASMCODE__
#define __ASMCODE__

#include <stdbool.h>

typedef struct AsmCode_ AsmCode;

struct AsmCode_ {
    char* code;
    bool tab;  // 打印时是否需要缩进
    AsmCode* next;
};

AsmCode* createAsmCode(char* code, bool tab);

#endif