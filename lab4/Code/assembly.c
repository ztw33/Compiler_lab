#include "assembly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reg.h"

#define debug 1

void addAsmCode(AsmCode* code) {
    if (asmCodesHead == NULL) {
        asmCodesHead = code;
        asmCodesTail = asmCodesHead;
    } else {
        asmCodesTail->next = code;
        asmCodesTail = code;
    }
}

AsmCode* generateAsm(InterCodes* irs) {
    addAsmCode(createAsmCode(".data\n"
                             "_prompt: .asciiz \"Enter an integer:\"\n"
                             "_ret: .asciiz \"\\n\"\n"
                             ".globl main\n"
                             ".text\n"
                             "read:\n"
                             "\tli $v0, 4\n"
                             "\tla $a0, _prompt\n"
                             "\tsyscall\n"
                             "\tli $v0, 5\n"
                             "\tsyscall\n"
                             "\tjr $ra\n"
                             "\nwrite:\n"
                             "\tli $v0, 1\n"
                             "\tsyscall\n"
                             "\tli $v0, 4\n"
                             "\tla $a0, _ret\n"
                             "\tsyscall\n"
                             "\tmove $v0, $0\n"
                             "\tjr $ra", false));
    
    InterCodes* p = irs;
    while (p != NULL) {
        InterCode* ir = p->code;
        switch (ir->kind) {
        case LABEL: {
            // LABEL x :
            if (debug) printf("LABEL\n");
            char* code = (char*)malloc(17);
            sprintf(code, "label%d:", ir->labelID);
            addAsmCode(createAsmCode(code, false));
            break;
        }
        case FUNCTION: {
            // FUNCTION f :
            if (debug) printf("FUNCTION\n");
            char* code = (char*)malloc(strlen(ir->funcName) + 2);
            sprintf(code, "\n%s:", ir->funcName);
            addAsmCode(createAsmCode(code, false));
            break;
        }
        case ASSIGN: {
            if (ir->assign.left->kind == VARIABLE) {
                char* leftReg = getReg(ir->assign.left);
                if (ir->assign.right->kind == VARIABLE) {
                    // x := y
                    char* rightReg = getReg(ir->assign.right);
                    char* code = (char*)malloc(strlen(leftReg) + strlen(rightReg) + 7);
                    sprintf(code, "move %s, %s", leftReg, rightReg);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->assign.right->kind == CONSTANT) {
                    // x := #k
                    char* code = (char*)malloc(strlen(leftReg) + 16);
                    sprintf(code, "li %s, %d", leftReg, ir->assign.right->constVal);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->assign.right->kind == REF) {
                    // x := &y
                    // TODO

                } else if (ir->assign.right->kind == DEREF) {
                    // x := *y
                    if (ir->assign.right->derefObj->kind != VARIABLE) {
                        fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case ASSIGN x := *y. Deref object is not variable.\033[0m\n");
                        exit(1);
                    }
                    char* rightReg = getReg(ir->assign.right->derefObj);
                    char* code = (char*)malloc(strlen(leftReg) + strlen(rightReg) + 8);
                    sprintf(code, "lw %s, 0(%s)", leftReg, rightReg);
                    addAsmCode(createAsmCode(code, true));
                } else {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Unknown operand kind.\033[0m\n");
                    exit(1);
                }
            } else if (ir->assign.left->kind == DEREF) {
                if (ir->assign.left->derefObj->kind != VARIABLE) {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case ASSIGN *x := y. Deref object is not variable.\033[0m\n");
                    exit(1);
                }
                char* leftReg = getReg(ir->assign.left->derefObj);
                if (ir->assign.right->kind == VARIABLE) {
                    // *x := y
                    char* rightReg = getReg(ir->assign.right);
                    char* code = (char*)malloc(strlen(leftReg) + strlen(rightReg) + 8);
                    sprintf(code, "sw %s, 0(%s)", rightReg, leftReg);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->assign.right->kind == CONSTANT) {
                    // *x := #k
                    char* code = (char*)malloc(strlen(leftReg) + 19);
                    sprintf(code, "sw %d, 0(%s)", ir->assign.right->constVal, leftReg);
                    addAsmCode(createAsmCode(code, true));
                } else {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case ASSIGN *x := y. y is not variable or constant.\033[0m\n");
                    exit(1);
                }
            } else {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case ASSIGN.\033[0m\n");
                exit(1);
            }
            break;
        }
        case ADD: {
            if (ir->binOp.result->kind != VARIABLE) {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case ADD.\033[0m\n");
                exit(1);
            }
            char* resultReg = getReg(ir->binOp.result);
            if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == VARIABLE) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code, "add %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code, true));
            } else if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == CONSTANT) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + 20);
                sprintf(code, "addi %s, %s, %d", resultReg, op1Reg, ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code, true));
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == VARIABLE) {
                char* op2Reg = getReg(ir->binOp.op2);
                char* code = (char*)malloc(strlen(resultReg) + strlen(op2Reg) + 20);
                sprintf(code, "addi %s, %d, %s", resultReg, ir->binOp.op1->constVal, op2Reg);
                addAsmCode(createAsmCode(code, true));
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == CONSTANT) {
                char* code = (char*)malloc(strlen(resultReg) + 31);
                sprintf(code, "addi %s, %d, %d", resultReg, ir->binOp.op1->constVal, ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code, true));
            } else {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case ADD x := y + z. Wrong operand kind of y or z.\033[0m\n");
                exit(1);
            }
            break;
        }
        case SUB: {
            if (ir->binOp.result->kind != VARIABLE) {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case SUB.\033[0m\n");
                exit(1);
            }
            char* resultReg = getReg(ir->binOp.result);
            if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == VARIABLE) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code, "sub %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code, true));
            } else if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == CONSTANT) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + 20);
                sprintf(code, "addi %s, %s, %d", resultReg, op1Reg, 0-ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code, true));
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == VARIABLE) {
                char* op2Reg = getReg(ir->binOp.op2);
                char* code = (char*)malloc(strlen(resultReg) + strlen(op2Reg) + 18);
                sprintf(code, "sub %s, %d, %s", resultReg, ir->binOp.op1->constVal, op2Reg);
                addAsmCode(createAsmCode(code, true));
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == CONSTANT) {
                char* code = (char*)malloc(strlen(resultReg) + 31);
                sprintf(code, "addi %s, %d, %d", resultReg, ir->binOp.op1->constVal, 0-ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code, true));
            } else {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case SUB x := y - z. Wrong operand kind of y or z.\033[0m\n");
                exit(1);
            }
            break;
        }
        case MUL: {
            if (ir->binOp.result->kind != VARIABLE) {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case MUL.\033[0m\n");
                exit(1);
            }
            char* resultReg = getReg(ir->binOp.result);
            if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == VARIABLE) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code, "mul %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code, true));
            } else if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == CONSTANT) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code1 = (char*)malloc(strlen(op2Reg) + 16);
                sprintf(code1, "li %s, %d", op2Reg, ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code1, true));
                char* code2 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code2, "mul %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code2, true));
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == VARIABLE) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                sprintf(code1, "li %s, %d", op1Reg, ir->binOp.op1->constVal);
                addAsmCode(createAsmCode(code1, true));
                char* code2 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code2, "mul %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code2, true));
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == CONSTANT) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                sprintf(code1, "li %s, %d", op1Reg, ir->binOp.op1->constVal);
                addAsmCode(createAsmCode(code1, true));
                char* code2 = (char*)malloc(strlen(op2Reg) + 16);
                sprintf(code2, "li %s, %d", op2Reg, ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code2, true));
                char* code3 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code3, "mul %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code3, true));
            } else {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case MUL x := y * z. Wrong operand kind of y or z.\033[0m\n");
                exit(1);
            }
            break;
        }
        case DIV:{
            if (ir->binOp.result->kind != VARIABLE) {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case DIV.\033[0m\n");
                exit(1);
            }
            char* resultReg = getReg(ir->binOp.result);
            if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == VARIABLE) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 13);
                sprintf(code, "div %s, %s\n\tmflo %s", op1Reg, op2Reg, resultReg);
                addAsmCode(createAsmCode(code, true));
            } else if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == CONSTANT) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code1 = (char*)malloc(strlen(op2Reg) + 16);
                sprintf(code1, "li %s, %d", op2Reg, ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code1, true));
                char* code2 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 13);
                sprintf(code2, "div %s, %s\n\tmflo %s", op1Reg, op2Reg, resultReg);
                addAsmCode(createAsmCode(code2, true));
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == VARIABLE) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                sprintf(code1, "li %s, %d", op1Reg, ir->binOp.op1->constVal);
                addAsmCode(createAsmCode(code1, true));
                char* code2 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 13);
                sprintf(code2, "div %s, %s\n\tmflo %s", op1Reg, op2Reg, resultReg);
                addAsmCode(createAsmCode(code2, true));
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == CONSTANT) {
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                sprintf(code1, "li %s, %d", op1Reg, ir->binOp.op1->constVal);
                addAsmCode(createAsmCode(code1, true));
                char* code2 = (char*)malloc(strlen(op2Reg) + 16);
                sprintf(code2, "li %s, %d", op2Reg, ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code2, true));
                char* code3 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 13);
                sprintf(code3, "div %s, %s\n\tmflo %s", op1Reg, op2Reg, resultReg);
                addAsmCode(createAsmCode(code3, true));
            } else {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case DIV x := y / z. Wrong operand kind of y or z.\033[0m\n");
                exit(1);
            }
            break;
        }
        case GOTO: {
            char* code = (char*)malloc(18);
            sprintf(code, "j label%d", ir->gotoLabelID);
            addAsmCode(createAsmCode(code, true));
            break;
        }
        case IF_GOTO: {
            char* op1Reg = getReg(ir->if_goto.cond->op1);
            char* op2Reg = getReg(ir->if_goto.cond->op2);
            switch (ir->if_goto.cond->relop) {
            case EQ: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code1, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code2, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code2, true));
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case IF_GOTO EQ. Wrong operand kind of op1 or op2.\033[0m\n");
                    exit(1);
                }
                break;
            }
            case NEQ: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code1, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code2, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code2, true));
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case IF_GOTO NEQ. Wrong operand kind of op1 or op2.\033[0m\n");
                    exit(1);
                }
                break;
            }
            case LT: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code1, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code2, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code2, true));
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case IF_GOTO LT. Wrong operand kind of op1 or op2.\033[0m\n");
                    exit(1);
                }
                break;
            }
            case GT: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code1, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code2, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code2, true));
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case IF_GOTO GT. Wrong operand kind of op1 or op2.\033[0m\n");
                    exit(1);
                }
                break;
            }
            case LE: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code1, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code2, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code2, true));
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case IF_GOTO LE. Wrong operand kind of op1 or op2.\033[0m\n");
                    exit(1);
                }
                break;
            }
            case GE: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code1, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code1 = (char*)malloc(strlen(op1Reg) + 16);
                    sprintf(code1, "li %s, %d", op1Reg, ir->if_goto.cond->op1->constVal);
                    addAsmCode(createAsmCode(code1, true));
                    char* code2 = (char*)malloc(strlen(op2Reg) + 16);
                    sprintf(code2, "li %s, %d", op2Reg, ir->if_goto.cond->op2->constVal);
                    addAsmCode(createAsmCode(code2, true));
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    fprintf(stderr, "\033[31mERROR in generateAsm! Wrong operand kind in case IF_GOTO GE. Wrong operand kind of op1 or op2.\033[0m\n");
                    exit(1);
                }
                break;
            }
            default:
                fprintf(stderr, "\033[31mERROR in generateAsm! Unknown relop in case IF_GOTO.\033[0m\n");
                exit(1);
                break;
            }
            break;
        }
        case RETURN: {
            if (ir->retVal->kind == VARIABLE) {
                char* reg = getReg(ir->retVal);
                char* code1 = (char*)malloc(strlen(reg) + 10);
                sprintf(code1, "move $v0, %s", reg);
                addAsmCode(createAsmCode(code1, true));
                addAsmCode(createAsmCode("jr $ra", true));
            } else if (ir->retVal->kind == CONSTANT) {
                char* reg = getReg(ir->retVal);
                char* code1 = (char*)malloc(strlen(reg) + 16);
                sprintf(code1, "li %s, %d", reg, ir->retVal->constVal);
                addAsmCode(createAsmCode(code1, true));
                char* code2 = (char*)malloc(strlen(reg) + 10);
                sprintf(code2, "move $v0, %s", reg);
                addAsmCode(createAsmCode(code2, true));
                addAsmCode(createAsmCode("jr $ra", true));
            } else {
                fprintf(stderr, "\033[31mERROR in generateAsm! Wrong retVal kind in case RETURN.\033[0m\n");
                exit(1);
            }
            break;
        }
        case DEC:
            break;
        case ARG:
            break;
        case CALL:
            break;
        case PARAM:
            break;
        case READ:
            break;
        case WRITE:
            break;
        default:
            fprintf(stderr, "\033[31mERROR in generateAsm! Unknown code kind.\033[0m\n");
            exit(1);
        }
        p = p->next;
    }
    return asmCodesHead;
}

void outputAsm(FILE* file, AsmCode* codes) {
    AsmCode* p = codes;
    while (p != NULL) {
        if (p->tab) {
            fprintf(file, "\t%s\n", p->code);
            if (debug) printf("\t%s\n", p->code);
        } else {
            fprintf(file, "%s\n", p->code);
            if (debug) printf("%s\n", p->code);
        }
        p = p->next;
    }
}