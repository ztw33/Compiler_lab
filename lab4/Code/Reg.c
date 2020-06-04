#include "Reg.h"
#include <stdio.h>
#include <stdlib.h>

char* getReg(Operand* op) {
    if (op->kind == VARIABLE) {
        return "$t1";
    } else if (op->kind == CONSTANT) {
        return "$t1";
    } else {
        fprintf(stderr, "\033[31mERROR in getReg! Wrong operand kind.\033[0m\n");
        exit(1);
    }
}