#include "assembly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reg.h"
#include "Stack.h"
#include "VarAddr.h"

#define debug 1

void addAsmCode(AsmCode* code) {
    if (asmCodesHead == NULL) {
        asmCodesHead = code;
        asmCodesTail = asmCodesHead;
    } else {
        asmCodesTail->next = code;
        asmCodesTail = code;
    }
    if (debug) printf("%s\n", code->code);
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
    
    Stack s = create_stack();  // 用于存放每次函数调用时相对上次$s8的偏移量
    int offset = 0;
    VarAddrTable addrTable = initAddrTabel();

    InterCodes* p = irs;
    while (p != NULL) {
        InterCode* ir = p->code;
        switch (ir->kind) {
        case LABEL: {
            // LABEL x :
            char* code = (char*)malloc(17);
            sprintf(code, "label%d:", ir->labelID);
            addAsmCode(createAsmCode(code, false));
            break;
        }
        case FUNCTION: {
            // FUNCTION f :
            char* code = (char*)malloc(strlen(ir->funcName) + 2);
            sprintf(code, "\n%s:", ir->funcName);
            addAsmCode(createAsmCode(code, false));

            if (strcmp(ir->funcName, "main") == 0) {
                addAsmCode(createAsmCode("move $s8, $sp", true));
            }
            break;
        }
        case ASSIGN: {
            if (ir->assign.left->kind == VARIABLE) {
                char* leftReg = getReg(ir->assign.left);
                int addr_x = getOffset(addrTable, ir->assign.left->var);
                if (addr_x == 0) {
                    offset -= 4;
                    insertVar(addrTable, ir->assign.left->var, offset);
                    addr_x = offset;
                    addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
                }
                if (ir->assign.right->kind == VARIABLE) {
                    // x := y
                    int addr_y = getOffset(addrTable, ir->assign.right->var);
                    if (addr_y == 0)
                        printAsmError("ERROR in generateAsm! Right operand is not in addrTable in case ASSIGN x := y.");

                    char* rightReg = getReg(ir->assign.right);
                    addLoadCode(rightReg, addr_y);

                    char* code = (char*)malloc(strlen(leftReg) + strlen(rightReg) + 7);
                    sprintf(code, "move %s, %s", leftReg, rightReg);
                    addAsmCode(createAsmCode(code, true));

                    addStoreCode(leftReg, addr_x);

                    freeReg(leftReg);
                    freeReg(rightReg);
                } else if (ir->assign.right->kind == CONSTANT) {
                    // x := #k
                    addLoadImmCode(leftReg, ir->assign.right->constVal);
                    addStoreCode(leftReg, addr_x);

                    freeReg(leftReg);
                } else if (ir->assign.right->kind == REF) {
                    // x := &y
                    if (ir->assign.right->refObj->kind != VARIABLE)
                        printAsmError("ERROR in generateAsm! Wrong operand kind in case ASSIGN x := &y. Ref object is not variable.");
                    
                    int addr_y = getOffset(addrTable, ir->assign.right->refObj->var);
                    if (addr_y == 0)
                        printAsmError("ERROR in generateAsm! Right operand is not in addrTable in case ASSIGN x := &y.");
                    
                    char* code = (char*)malloc(strlen(leftReg) + 23);
                    sprintf(code, "addi %s, $s8, %d", leftReg, addr_y);
                    addAsmCode(createAsmCode(code, true));

                    addStoreCode(leftReg, addr_x);

                    freeReg(leftReg);
                } else if (ir->assign.right->kind == DEREF) {
                    // x := *y
                    if (ir->assign.right->derefObj->kind != VARIABLE)
                        printAsmError("ERROR in generateAsm! Wrong operand kind in case ASSIGN x := *y. Deref object is not variable.");
            
                    int addr_y = getOffset(addrTable, ir->assign.right->derefObj->var);
                    if (addr_y == 0)
                        printAsmError("ERROR in generateAsm! Right operand is not in addrTable in case ASSIGN x := *y.");

                    char* rightReg = getReg(ir->assign.right->derefObj);
                    addLoadCode(rightReg, addr_y);

                    char* code = (char*)malloc(strlen(leftReg) + strlen(rightReg) + 8);
                    sprintf(code, "lw %s, 0(%s)", leftReg, rightReg);
                    addAsmCode(createAsmCode(code, true));

                    addStoreCode(leftReg, addr_x);

                    freeReg(leftReg);
                    freeReg(rightReg);
                } else {
                    printAsmError("ERROR in generateAsm! Unknown operand kind in case ASSIGN.");
                }
            } else if (ir->assign.left->kind == DEREF) {
                if (ir->assign.left->derefObj->kind != VARIABLE)
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case ASSIGN *x := y. Deref object is not variable.");

                char* leftReg = getReg(ir->assign.left->derefObj);
                int addr_x = getOffset(addrTable, ir->assign.left->derefObj->var);
                if (addr_x == 0)
                    printAsmError("ERROR in generateAsm in case ASSIGN *x := y. x is not in the addrTable.");

                addLoadCode(leftReg, addr_x);

                if (ir->assign.right->kind == VARIABLE) {
                    // *x := y
                    int addr_y = getOffset(addrTable, ir->assign.right->var);
                    if (addr_y == 0)
                        printAsmError("ERROR in generateAsm! Right operand is not in addrTable in case ASSIGN *x := y.");

                    char* rightReg = getReg(ir->assign.right);
                    addLoadCode(rightReg, addr_y);

                    char* code = (char*)malloc(strlen(leftReg) + strlen(rightReg) + 8);
                    sprintf(code, "sw %s, 0(%s)", rightReg, leftReg);
                    addAsmCode(createAsmCode(code, true));

                    freeReg(leftReg);
                    freeReg(rightReg);
                } else if (ir->assign.right->kind == CONSTANT) {
                    // *x := #k
                    char* immReg = getReg(ir->assign.right);
                    addLoadImmCode(immReg, ir->assign.right->constVal);

                    char* code = (char*)malloc(strlen(leftReg) + strlen(immReg) + 8);
                    sprintf(code, "sw %s, 0(%s)", immReg, leftReg);
                    addAsmCode(createAsmCode(code, true));

                    freeReg(leftReg);
                    freeReg(immReg);
                } else {
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case ASSIGN *x := y. y is not variable or constant.");
                }
            } else {
                printAsmError("ERROR in generateAsm! Wrong operand kind in case ASSIGN.");
            }
            break;
        }
        case ADD: {
            if (ir->binOp.result->kind != VARIABLE)
                printAsmError("ERROR in generateAsm! Wrong operand kind in case ADD. Left operand is not variable");

            char* resultReg = getReg(ir->binOp.result);
            int addr_x = getOffset(addrTable, ir->binOp.result->var);
            if (addr_x == 0) {
                offset -= 4;
                insertVar(addrTable, ir->binOp.result->var, offset);
                addr_x = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }

            if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == VARIABLE) {
                // x := y + z
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_y = getOffset(addrTable, ir->binOp.op1->var);
                int addr_z = getOffset(addrTable, ir->binOp.op2->var);
                if (addr_y == 0 || addr_z == 0)
                    printAsmError("ERROR in generateAsm in case ADD x := y + z. y or z is not in addrTable.");
                
                addLoadCode(op1Reg, addr_y);
                addLoadCode(op2Reg, addr_z);

                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code, "add %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == CONSTANT) {
                // x := y + #k
                char* op1Reg = getReg(ir->binOp.op1);
                int addr_y = getOffset(addrTable, ir->binOp.op1->var);
                if (addr_y == 0)
                    printAsmError("ERROR in generateAsm in case ADD x := y + #k. y is not in addrTable.");
                
                addLoadCode(op1Reg, addr_y);
                
                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + 20);
                sprintf(code, "addi %s, %s, %d", resultReg, op1Reg, ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == VARIABLE) {
                // x := #k + z
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_z = getOffset(addrTable, ir->binOp.op2->var);
                if (addr_z == 0)
                    printAsmError("ERROR in generateAsm in case ADD x := #k + z. z is not in addrTable.");
                
                addLoadCode(op2Reg, addr_z);

                char* code = (char*)malloc(strlen(resultReg) + strlen(op2Reg) + 20);
                sprintf(code, "addi %s, %s, %d", resultReg, op2Reg, ir->binOp.op1->constVal);
                addAsmCode(createAsmCode(code, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == CONSTANT) {
                // x := #k1 + #k2
                int result = ir->binOp.op1->constVal + ir->binOp.op2->constVal;
                addLoadImmCode(resultReg, result);
                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
            } else {
                printAsmError("ERROR in generateAsm! Wrong operand kind in case ADD x := y + z. Wrong operand kind of y or z.");
            }
            break;
        }
        case SUB: {
            if (ir->binOp.result->kind != VARIABLE)
                printAsmError("ERROR in generateAsm! Wrong operand kind in case SUB. Result is not variable.");

            char* resultReg = getReg(ir->binOp.result);
            int addr_x = getOffset(addrTable, ir->binOp.result->var);
            if (addr_x == 0) {
                offset -= 4;
                insertVar(addrTable, ir->binOp.result->var, offset);
                addr_x = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }

            if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == VARIABLE) {
                // x := y - z
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_y = getOffset(addrTable, ir->binOp.op1->var);
                int addr_z = getOffset(addrTable, ir->binOp.op2->var);
                if (addr_y == 0 || addr_z == 0)
                    printAsmError("ERROR in generateAsm in case SUB x := y - z. y or z is not in addrTable.");
                
                addLoadCode(op1Reg, addr_y);
                addLoadCode(op2Reg, addr_z);

                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code, "sub %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == CONSTANT) {
                // x := y - #k
                char* op1Reg = getReg(ir->binOp.op1);
                int addr_y = getOffset(addrTable, ir->binOp.op1->var);
                if (addr_y == 0)
                    printAsmError("ERROR in generateAsm in case SUB x := y - #k. y is not in addrTable.");
                
                addLoadCode(op1Reg, addr_y);

                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + 20);
                sprintf(code, "addi %s, %s, %d", resultReg, op1Reg, 0-ir->binOp.op2->constVal);
                addAsmCode(createAsmCode(code, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == VARIABLE) {
                // x := #k - z
                char* immReg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_z = getOffset(addrTable, ir->binOp.op2->var);
                if (addr_z == 0)
                    printAsmError("ERROR in generateAsm in case SUB x := #k - z. z is not in addrTable.");
                
                addLoadImmCode(immReg, ir->binOp.op1->constVal);
                addLoadCode(op2Reg, addr_z);

                char* code = (char*)malloc(strlen(resultReg) + strlen(op2Reg) + 18);
                sprintf(code, "sub %s, %s, %s", resultReg, immReg, op2Reg);
                addAsmCode(createAsmCode(code, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(immReg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == CONSTANT) {
                // x := #k1 - #k2
                int result = ir->binOp.op1->constVal - ir->binOp.op2->constVal;
                addLoadImmCode(resultReg, result);
                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
            } else {
                printAsmError("ERROR in generateAsm! Wrong operand kind in case SUB x := y - z. Wrong operand kind of y or z.");
            }
            break;
        }
        case MUL: {
            if (ir->binOp.result->kind != VARIABLE)
                printAsmError("ERROR in generateAsm! Wrong operand kind in case MUL. Left operand is not variable.");

            char* resultReg = getReg(ir->binOp.result);
            int addr_x = getOffset(addrTable, ir->binOp.result->var);
            if (addr_x == 0) {
                offset -= 4;
                insertVar(addrTable, ir->binOp.result->var, offset);
                addr_x = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }

            if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == VARIABLE) {
                // x := y * z
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_y = getOffset(addrTable, ir->binOp.op1->var);
                int addr_z = getOffset(addrTable, ir->binOp.op2->var);
                if (addr_y == 0 || addr_z == 0)
                    printAsmError("ERROR in generateAsm in case MUL x := y * z. y or z is not in addrTable.");
                
                addLoadCode(op1Reg, addr_y);
                addLoadCode(op2Reg, addr_z);

                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code, "mul %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == CONSTANT) {
                // x := y * #k
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_y = getOffset(addrTable, ir->binOp.op1->var);
                if (addr_y == 0)
                    printAsmError("ERROR in generateAsm in case MUL x := y * #k. y is not in addrTable.");

                addLoadCode(op1Reg, addr_y);
                addLoadImmCode(op2Reg, ir->binOp.op2->constVal);

                char* code2 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code2, "mul %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code2, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == VARIABLE) {
                // x := #k * z
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_z = getOffset(addrTable, ir->binOp.op2->var);
                if (addr_z == 0)
                    printAsmError("ERROR in generateAsm in case MUL x := #k * z. z is not in addrTable.");

                addLoadCode(op2Reg, addr_z);
                addLoadImmCode(op1Reg, ir->binOp.op1->constVal);

                char* code2 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
                sprintf(code2, "mul %s, %s, %s", resultReg, op1Reg, op2Reg);
                addAsmCode(createAsmCode(code2, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == CONSTANT) {
                // x := #k1 * #k2
                int result = ir->binOp.op1->constVal * ir->binOp.op1->constVal;
                addLoadImmCode(resultReg, result);
                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
            } else {
                printAsmError("ERROR in generateAsm! Wrong operand kind in case MUL x := y * z. Wrong operand kind of y or z.");
            }
            break;
        }
        case DIV:{
            if (ir->binOp.result->kind != VARIABLE)
                printAsmError("ERROR in generateAsm! Wrong operand kind in case DIV. Left operand is not variable.");

            char* resultReg = getReg(ir->binOp.result);
            int addr_x = getOffset(addrTable, ir->binOp.result->var);
            if (addr_x == 0) {
                offset -= 4;
                insertVar(addrTable, ir->binOp.result->var, offset);
                addr_x = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }

            if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == VARIABLE) {
                // x := y / z
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_y = getOffset(addrTable, ir->binOp.op1->var);
                int addr_z = getOffset(addrTable, ir->binOp.op2->var);
                if (addr_y == 0 || addr_z == 0)
                    printAsmError("ERROR in generateAsm in case DIV x := y / z. y or z is not in addrTable.");
                
                addLoadCode(op1Reg, addr_y);
                addLoadCode(op2Reg, addr_z);

                char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 13);
                sprintf(code, "div %s, %s\n\tmflo %s", op1Reg, op2Reg, resultReg);
                addAsmCode(createAsmCode(code, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == VARIABLE && ir->binOp.op2->kind == CONSTANT) {
                // x := y / #k
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_y = getOffset(addrTable, ir->binOp.op1->var);
                if (addr_y == 0)
                    printAsmError("ERROR in generateAsm in case DIV x := y / #k. y is not in addrTable.");

                addLoadCode(op1Reg, addr_y);
                addLoadImmCode(op2Reg, ir->binOp.op2->constVal);
                
                char* code2 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 13);
                sprintf(code2, "div %s, %s\n\tmflo %s", op1Reg, op2Reg, resultReg);
                addAsmCode(createAsmCode(code2, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == VARIABLE) {
                // x := #k / z
                char* op1Reg = getReg(ir->binOp.op1);
                char* op2Reg = getReg(ir->binOp.op2);
                int addr_z = getOffset(addrTable, ir->binOp.op2->var);
                if (addr_z == 0)
                    printAsmError("ERROR in generateAsm in case DIV x := #k / z. z is not in addrTable.");

                addLoadCode(op2Reg, addr_z);
                addLoadImmCode(op1Reg, ir->binOp.op1->constVal);

                char* code2 = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 13);
                sprintf(code2, "div %s, %s\n\tmflo %s", op1Reg, op2Reg, resultReg);
                addAsmCode(createAsmCode(code2, true));

                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
                freeReg(op1Reg);
                freeReg(op2Reg);
            } else if (ir->binOp.op1->kind == CONSTANT && ir->binOp.op2->kind == CONSTANT) {
                // x := #k1 / #k2
                int result = ir->binOp.op1->constVal / ir->binOp.op1->constVal;
                addLoadImmCode(resultReg, result);
                addStoreCode(resultReg, addr_x);

                freeReg(resultReg);
            } else {
                printAsmError("ERROR in generateAsm! Wrong operand kind in case DIV x := y / z. Wrong operand kind of y or z.");
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

            if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                int addr_y = getOffset(addrTable, ir->if_goto.cond->op1->var);
                int addr_z = getOffset(addrTable, ir->if_goto.cond->op2->var);
                if (addr_y == 0 || addr_z == 0)
                    printAsmError("ERROR in generateAsm in case IF_GOTO y op z. y or z is not in addrTable.");
                
                addLoadCode(op1Reg, addr_y);
                addLoadCode(op2Reg, addr_z);
            } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                int addr_y = getOffset(addrTable, ir->if_goto.cond->op1->var);
                if (addr_y == 0)
                    printAsmError("ERROR in generateAsm in case DIV x := y / #k. y is not in addrTable.");

                addLoadCode(op1Reg, addr_y);
                addLoadImmCode(op2Reg, ir->if_goto.cond->op2->constVal);
            } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                int addr_z = getOffset(addrTable, ir->if_goto.cond->op2->var);
                if (addr_z == 0)
                    printAsmError("ERROR in generateAsm in case DIV x := #k / z. z is not in addrTable.");

                addLoadImmCode(op1Reg, ir->if_goto.cond->op1->constVal);
                addLoadCode(op2Reg, addr_z);
            } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                addLoadImmCode(op1Reg, ir->if_goto.cond->op1->constVal);
                addLoadImmCode(op2Reg, ir->if_goto.cond->op2->constVal);
            } else {
                printAsmError("ERROR in generateAsm! Wrong operand kind in case IF_GOTO. Wrong operand kind of op1 or op2.");
            }

            switch (ir->if_goto.cond->relop) {
            case EQ: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case IF_GOTO EQ. Wrong operand kind of op1 or op2.");
                }
                break;
            }
            case NEQ: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case IF_GOTO NEQ. Wrong operand kind of op1 or op2.");
                }
                break;
            }
            case LT: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case IF_GOTO LT. Wrong operand kind of op1 or op2.");
                }
                break;
            }
            case GT: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case IF_GOTO GT. Wrong operand kind of op1 or op2.");
                }
                break;
            }
            case LE: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case IF_GOTO LE. Wrong operand kind of op1 or op2.");
                }
                break;
            }
            case GE: {
                if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code, true));
                } else if (ir->if_goto.cond->op1->kind == VARIABLE && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == VARIABLE) {
                    char* code2 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code2, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code2, true));
                } else if (ir->if_goto.cond->op1->kind == CONSTANT && ir->if_goto.cond->op2->kind == CONSTANT) {
                    char* code3 = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                    sprintf(code3, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                    addAsmCode(createAsmCode(code3, true));
                } else {
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case IF_GOTO GE. Wrong operand kind of op1 or op2.");
                }
                break;
            }
            default:
                printAsmError("ERROR in generateAsm! Unknown relop in case IF_GOTO.");
            }
            freeReg(op1Reg);
            freeReg(op2Reg);
            break;
        }
        case RETURN: {
            if (ir->retVal->kind == VARIABLE) {
                char* reg = getReg(ir->retVal);
                int addr = getOffset(addrTable, ir->retVal->var);
                if (addr == 0)
                    printAsmError("ERROR in generateAsm in case RETURN. Return variable is not in addrTable.");

                addLoadCode(reg, addr);

                char* code1 = (char*)malloc(strlen(reg) + 10);
                sprintf(code1, "move $v0, %s", reg);
                addAsmCode(createAsmCode(code1, true));
                addAsmCode(createAsmCode("jr $ra", true));
                freeReg(reg);
            } else if (ir->retVal->kind == CONSTANT) {
                addLoadImmCode("$v0", ir->retVal->constVal);
                addAsmCode(createAsmCode("jr $ra", true));
            } else {
                printAsmError("ERROR in generateAsm! Wrong retVal kind in case RETURN.");
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
        case READ: {
            addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            addAsmCode(createAsmCode("sw $ra, 0($sp)", true));
            addAsmCode(createAsmCode("jal read", true));
            addAsmCode(createAsmCode("lw $ra, 0($sp)", true));
            addAsmCode(createAsmCode("addi $sp, $sp, 4", true));
            if (ir->rwOperand->kind != VARIABLE)
                printAsmError("ERROR in generateAsm in case READ! Read object is not variable.");
            
            char* reg = getReg(ir->rwOperand);
            int addr = getOffset(addrTable, ir->rwOperand->var);
            if (addr == 0) {
                offset -= 4;
                insertVar(addrTable, ir->rwOperand->var, offset);
                addr = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }
            addStoreCode("$v0", addr);
            freeReg(reg);
            break;
        }
        case WRITE: {
            if (ir->rwOperand->kind == CONSTANT) {
                addLoadImmCode("$a0", ir->rwOperand->constVal);
            } else if (ir->rwOperand->kind == VARIABLE) {
                int addr = getOffset(addrTable, ir->rwOperand->var);
                if (addr == 0)
                    printAsmError("ERROR in generateAsm in case WRITE! Write variable is not in the addrTable.");
                char* reg = getReg(ir->rwOperand);
                addLoadCode(reg, addr);
                char* code = (char*)malloc(strlen(reg) + 10);
                sprintf(code, "move $a0, %s", reg);
                addAsmCode(createAsmCode(code, true));
                freeReg(reg);
            } else if (ir->rwOperand->kind == REF) {
                if (ir->rwOperand->refObj->kind != VARIABLE)
                    printAsmError("ERROR in generateAsm in case WRITE ref! Ref object is not variable.");
                
                int addr = getOffset(addrTable, ir->rwOperand->refObj->var);
                if (addr == 0)
                    printAsmError("ERROR in generateAsm in case WRITE ref! Write operand is not in addrTable.");
                
                char* reg = getReg(ir->rwOperand);
                char* code1 = (char*)malloc(strlen(reg) + 23);
                sprintf(code1, "addi %s, $s8, %d", reg, addr);
                addAsmCode(createAsmCode(code1, true));
                char* code2 = (char*)malloc(strlen(reg) + 10);
                sprintf(code2, "move $a0, %s", reg);
                addAsmCode(createAsmCode(code2, true));

                freeReg(reg);
            } else if (ir->rwOperand->kind == DEREF) {
                 if (ir->rwOperand->derefObj->kind != VARIABLE)
                    printAsmError("ERROR in generateAsm in case WRITE deref! Deref object is not variable.");
        
                int addr = getOffset(addrTable, ir->rwOperand->derefObj->var);
                if (addr == 0)
                    printAsmError("ERROR in generateAsm in case WRITE deref! Write deref operand is not in addrTable.");

                char* reg = getReg(ir->rwOperand->derefObj);
                addLoadCode(reg, addr);

                char* code = (char*)malloc(strlen(reg) + 11);
                sprintf(code, "lw $a0, 0(%s)", reg);
                addAsmCode(createAsmCode(code, true));

                freeReg(reg);
            } else {
                printAsmError("ERROR in generateAsm in case WRITE! Wrong write operand kind.");
            }
            addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            addAsmCode(createAsmCode("sw $ra, 0($sp)", true));
            addAsmCode(createAsmCode("jal write", true));
            addAsmCode(createAsmCode("lw $ra, 0($sp)", true));
            addAsmCode(createAsmCode("addi $sp, $sp, 4", true));
            break;
        }
        default:
            printAsmError("ERROR in generateAsm! Unknown code kind.");
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
        } else {
            fprintf(file, "%s\n", p->code);
        }
        p = p->next;
    }
}

void printAsmError(char* msg) {
    fprintf(stderr, "\033[31m%s\033[0m\n", msg);
    exit(1);
}

void addLoadCode(char* regName, int offset) {
    char* load = (char*)malloc(strlen(regName) + 21);
    sprintf(load, "lw %s, %d($s8)", regName, offset);
    addAsmCode(createAsmCode(load, true));
}

void addStoreCode(char* regName, int offset) {
    char* store = (char*)malloc(strlen(regName) + 21);
    sprintf(store, "sw %s, %d($s8)", regName, offset);
    addAsmCode(createAsmCode(store, true));
}

void addLoadImmCode(char* regName, int imm) {
    char* code = (char*)malloc(strlen(regName) + 16);
    sprintf(code, "li %s, %d", regName, imm);
    addAsmCode(createAsmCode(code, true));
}