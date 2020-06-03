#include "AsmCode.h"
#include <stdlib.h>

AsmCode* createAsmCode(char* code, bool tab) {
    AsmCode* asmCode = (AsmCode*)malloc(sizeof(AsmCode));
    asmCode->code = code;
    asmCode->tab = tab;
    asmCode->next = NULL;
    return asmCode;
}