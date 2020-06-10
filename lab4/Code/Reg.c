#include "Reg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 代表$t0-$t7共8个寄存器，true代表正在被使用，false代表空闲
bool regs[8];

char* getReg() {
    for (int i = 0; i < 7; i++) {
        if (regs[i] == false) {
            regs[i] = true;
            char* reg = (char*)malloc(4);
            sprintf(reg, "$t%d", i);
            return reg;
        }
    }
    fprintf(stderr, "\033[31mERROR in getReg! No spare reg.\033[0m\n");
    exit(1);
}

void freeReg(char* regName) {
    if (strcmp(regName, "$t0") == 0) {
        regs[0] = false;
    } else if (strcmp(regName, "$t1") == 0) {
        regs[1] = false;
    } else if (strcmp(regName, "$t2") == 0) {
        regs[2] = false;
    } else if (strcmp(regName, "$t3") == 0) {
        regs[3] = false;
    } else if (strcmp(regName, "$t4") == 0) {
        regs[4] = false;
    } else if (strcmp(regName, "$t5") == 0) {
        regs[5] = false;
    } else if (strcmp(regName, "$t6") == 0) {
        regs[6] = false;
    } else if (strcmp(regName, "$t7") == 0) {
        regs[7] = false;
    } else {
        fprintf(stderr, "\033[31mERROR in freeReg! Wrong reg name.\033[0m\n");
        exit(1);
    }
}