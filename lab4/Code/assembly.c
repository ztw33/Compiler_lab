#include "assembly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reg.h"

#define debug 0

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
    
    int offset = 0;
    VarAddrTable addrTable = initAddrTable();

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
                offset = 0;
            } else {
                offset = -8;
            }
            break;
        }
        case ASSIGN: {
            if (ir->assign.left->kind == VARIABLE) {
                char* leftReg = getReg();
                int addr_x = getOffset(addrTable, ir->assign.left->var);
                if (addr_x == 0) {
                    offset -= 4;
                    insertVar(addrTable, ir->assign.left->var, offset);
                    addr_x = offset;
                    addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
                }
                handleOperand(ir->assign.right, leftReg, addrTable);
                addStoreCode(leftReg, addr_x);
                freeReg(leftReg);
            } else if (ir->assign.left->kind == DEREF) {
                if (ir->assign.left->derefObj->kind != VARIABLE)
                    printAsmError("ERROR in generateAsm! Wrong operand kind in case ASSIGN *x := y. Deref object is not variable.");

                char* reg_x = getReg();
                handleOperand(ir->assign.left->derefObj, reg_x, addrTable);

                char* reg_y = getReg();
                handleOperand(ir->assign.right, reg_y, addrTable);

                char* code = (char*)malloc(strlen(reg_x) + strlen(reg_y) + 8);
                sprintf(code, "sw %s, 0(%s)", reg_y, reg_x);
                addAsmCode(createAsmCode(code, true));

                freeReg(reg_x);
                freeReg(reg_y);
            } else {
                printAsmError("ERROR in generateAsm! Wrong operand kind in case ASSIGN.");
            }
            break;
        }
        case ADD: {
            if (ir->binOp.result->kind != VARIABLE)
                printAsmError("ERROR in generateAsm! Wrong operand kind in case ADD. Left operand is not variable");

            char* resultReg = getReg();
            int addr_x = getOffset(addrTable, ir->binOp.result->var);
            if (addr_x == 0) {
                offset -= 4;
                insertVar(addrTable, ir->binOp.result->var, offset);
                addr_x = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }

            char* op1Reg = getReg();
            char* op2Reg = getReg();
            handleOperand(ir->binOp.op1, op1Reg, addrTable);
            handleOperand(ir->binOp.op2, op2Reg, addrTable);

            char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
            sprintf(code, "add %s, %s, %s", resultReg, op1Reg, op2Reg);
            addAsmCode(createAsmCode(code, true));

            addStoreCode(resultReg, addr_x);
            
            freeReg(resultReg);
            freeReg(op1Reg);
            freeReg(op2Reg);
            break;
        }
        case SUB: {
            if (ir->binOp.result->kind != VARIABLE)
                printAsmError("ERROR in generateAsm! Wrong operand kind in case SUB. Result is not variable.");

            char* resultReg = getReg();
            int addr_x = getOffset(addrTable, ir->binOp.result->var);
            if (addr_x == 0) {
                offset -= 4;
                insertVar(addrTable, ir->binOp.result->var, offset);
                addr_x = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }

            char* op1Reg = getReg();
            char* op2Reg = getReg();
            handleOperand(ir->binOp.op1, op1Reg, addrTable);
            handleOperand(ir->binOp.op2, op2Reg, addrTable);

            char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
            sprintf(code, "sub %s, %s, %s", resultReg, op1Reg, op2Reg);
            addAsmCode(createAsmCode(code, true));

            addStoreCode(resultReg, addr_x);
            
            freeReg(resultReg);
            freeReg(op1Reg);
            freeReg(op2Reg);
            break;
        }
        case MUL: {
            if (ir->binOp.result->kind != VARIABLE)
                printAsmError("ERROR in generateAsm! Wrong operand kind in case MUL. Left operand is not variable.");

            char* resultReg = getReg();
            int addr_x = getOffset(addrTable, ir->binOp.result->var);
            if (addr_x == 0) {
                offset -= 4;
                insertVar(addrTable, ir->binOp.result->var, offset);
                addr_x = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }

            char* op1Reg = getReg();
            char* op2Reg = getReg();
            handleOperand(ir->binOp.op1, op1Reg, addrTable);
            handleOperand(ir->binOp.op2, op2Reg, addrTable);

            char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 8);
            sprintf(code, "mul %s, %s, %s", resultReg, op1Reg, op2Reg);
            addAsmCode(createAsmCode(code, true));

            addStoreCode(resultReg, addr_x);
            
            freeReg(resultReg);
            freeReg(op1Reg);
            freeReg(op2Reg);
            break;
        }
        case DIV:{
            if (ir->binOp.result->kind != VARIABLE)
                printAsmError("ERROR in generateAsm! Wrong operand kind in case DIV. Left operand is not variable.");

            char* resultReg = getReg();
            int addr_x = getOffset(addrTable, ir->binOp.result->var);
            if (addr_x == 0) {
                offset -= 4;
                insertVar(addrTable, ir->binOp.result->var, offset);
                addr_x = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }

            char* op1Reg = getReg();
            char* op2Reg = getReg();
            handleOperand(ir->binOp.op1, op1Reg, addrTable);
            handleOperand(ir->binOp.op2, op2Reg, addrTable);

            char* code = (char*)malloc(strlen(resultReg) + strlen(op1Reg) + strlen(op2Reg) + 13);
            sprintf(code, "div %s, %s\n\tmflo %s", op1Reg, op2Reg, resultReg);
            addAsmCode(createAsmCode(code, true));

            addStoreCode(resultReg, addr_x);
            
            freeReg(resultReg);
            freeReg(op1Reg);
            freeReg(op2Reg);
            break;
        }
        case GOTO: {
            char* code = (char*)malloc(18);
            sprintf(code, "j label%d", ir->gotoLabelID);
            addAsmCode(createAsmCode(code, true));
            break;
        }
        case IF_GOTO: {
            char* op1Reg = getReg();
            char* op2Reg = getReg();
            handleOperand(ir->if_goto.cond->op1, op1Reg, addrTable);
            handleOperand(ir->if_goto.cond->op2, op2Reg, addrTable);

            switch (ir->if_goto.cond->relop) {
            case EQ: {
                char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                sprintf(code, "beq %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                addAsmCode(createAsmCode(code, true));
                break;
            }
            case NEQ: {
                char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                sprintf(code, "bne %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                addAsmCode(createAsmCode(code, true));
                break;
            }
            case LT: {
                char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                sprintf(code, "blt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                addAsmCode(createAsmCode(code, true));
                break;
            }
            case GT: {
                char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                sprintf(code, "bgt %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                addAsmCode(createAsmCode(code, true));
                break;
            }
            case LE: {
                char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                sprintf(code, "ble %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                addAsmCode(createAsmCode(code, true));
                break;
            }
            case GE: {
                char* code = (char*)malloc(strlen(op1Reg) + strlen(op2Reg) + 24);
                sprintf(code, "bge %s, %s, label%d", op1Reg, op2Reg, ir->if_goto.gotoLabelID);
                addAsmCode(createAsmCode(code, true));
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
            handleOperand(ir->retVal, "$v0", addrTable);
            addAsmCode(createAsmCode("jr $ra", true));
            break;
        }
        case DEC: {
            if (getOffset(addrTable, ir->dec.var) != 0)
                printAsmError("ERROR in generateAsm in case DEC! Variable is already in the addrTable.");
            
            if (ir->dec.size % 4 != 0)
                printAsmError("ERROR in generateAsm in case DEC! Dec size不是4的倍数.");
            
            offset -= ir->dec.size;
            insertVar(addrTable, ir->dec.var, offset);
            char* code = (char*)malloc(26);
            sprintf(code, "addi $sp, $sp, -%d", ir->dec.size);
            addAsmCode(createAsmCode(code, true));
            break;
        }
        case ARG: {
            InterCodes* p_arg = p;
            InterCodes* last_arg = p;  // 指向最后一个参数
            int arg_num = 0;
            while (p_arg->code->kind == ARG) {
                arg_num++;
                p_arg = p_arg->next;
            }
            p_arg = p_arg->prev;  // 指向第一个参数
            p = p_arg;  // 所有的arg在这次处理完，p直接指向最后一条arg语句(第一个参数)
            
            // 超过4个参数，溢出到栈中
            if (arg_num > 4) {
                InterCodes* pp = last_arg;
                for (int i = 0; i < arg_num - 4; i++) {
                    ir = pp->code;
                    // offset -= 4;  把offset当做记录目前函数的活动记录大小的变量，参数在调用返回后占用的空间就消失了，因此此处不再-4
                    addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
                    char* reg = getReg();
                    handleOperand(ir->arg, reg, addrTable);
                    
                    char* code = (char*)malloc(strlen(reg) + 11);
                    sprintf(code, "sw %s, 0($sp)", reg);
                    addAsmCode(createAsmCode(code, true));
                    freeReg(reg);

                    pp = pp->next;
                }
            }

            int reg_num = 0;
            while (p_arg->code->kind == ARG && reg_num < 4) {
                ir = p_arg->code;
                char* regName = (char*)malloc(4);
                sprintf(regName, "$a%d", reg_num);

                handleOperand(ir->arg, regName, addrTable);
                p_arg = p_arg->prev;
                reg_num++;
            }
            break;
        }
        case CALL: {
            addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            addAsmCode(createAsmCode("sw $ra, 0($sp)", true));  // 保存返回地址
            addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            addAsmCode(createAsmCode("sw $s8, 0($sp)", true));  // 保存帧指针
            addAsmCode(createAsmCode("addi $s8, $sp, 8", true));  // 移动$s8

            char* jmpCode = (char*)malloc(strlen(ir->call.funcName) + 4);
            sprintf(jmpCode, "jal %s", ir->call.funcName);
            addAsmCode(createAsmCode(jmpCode, true));

            addAsmCode(createAsmCode("lw $ra, -4($s8)", true));  // 恢复返回地址
            addAsmCode(createAsmCode("lw $s8, -8($s8)", true));  // 恢复$s8
            char* rs_sp = (char*)malloc(26);
            sprintf(rs_sp, "addi $sp, $s8, %d", offset);
            addAsmCode(createAsmCode(rs_sp, true));  // 恢复$sp

            int addr_ret = getOffset(addrTable, ir->call.ret);
            if (addr_ret == 0) {
                offset -= 4;
                insertVar(addrTable, ir->call.ret, offset);
                addr_ret = offset;
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            }
            addStoreCode("$v0", addr_ret);
            break;
        }
        case PARAM: {
            InterCodes* p_param = p;
            int param_count = 0;
            Operand* tmp = createOperand(VARIABLE);
            while (p_param->code->kind == PARAM) {
                addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
                offset -= 4;
                insertVar(addrTable, p_param->code->param, offset);
                if (param_count < 4) {
                    char* code = (char*)malloc(14);
                    sprintf(code, "sw $a%d, 0($sp)", param_count);
                    addAsmCode(createAsmCode(code, true));
                } else {
                    char* tmpReg = getReg();
                    int param_offset = 4 * (param_count - 4);
                    addLoadCode(tmpReg, param_offset);
                    char* code = (char*)malloc(strlen(tmpReg) + 11);
                    sprintf(code, "sw %s, 0($sp)", tmpReg);
                    addAsmCode(createAsmCode(code, true));
                    freeReg(tmpReg);
                }
                param_count++;
                p_param = p_param->next;
            }
            free(tmp);
            p = p_param->prev;  // 所有的param在这次处理完，p直接指向最后一条param语句
            break;
        }
        case READ: {
            addAsmCode(createAsmCode("addi $sp, $sp, -4", true));
            addAsmCode(createAsmCode("sw $ra, 0($sp)", true));
            addAsmCode(createAsmCode("jal read", true));
            addAsmCode(createAsmCode("lw $ra, 0($sp)", true));
            addAsmCode(createAsmCode("addi $sp, $sp, 4", true));
            if (ir->rwOperand->kind != VARIABLE)
                printAsmError("ERROR in generateAsm in case READ! Read object is not variable.");
            
            char* reg = getReg();
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
            handleOperand(ir->rwOperand, "$a0", addrTable);
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

// 把操作数load到寄存器里
void handleOperand(Operand* op, char* reg, VarAddrTable addrTable) {
    if (op->kind == CONSTANT) {
        addLoadImmCode(reg, op->constVal);
    } else if (op->kind == VARIABLE) {
        int addr = getOffset(addrTable, op->var);
        if (addr == 0)
            printAsmError("ERROR in handleOperand! op->var is not in addrTable.");

        addLoadCode(reg, addr);
    } else if (op->kind == REF) {
        if (op->refObj->kind != VARIABLE)
            printAsmError("ERROR in handleOperand! Ref object is not variable.");
        
        int addr = getOffset(addrTable, op->refObj->var);
        if (addr == 0)
            printAsmError("ERROR in handleOperand! op->refObj->var is not in addrTable.");
        
        char* code = (char*)malloc(strlen(reg) + 23);
        sprintf(code, "addi %s, $s8, %d", reg, addr);
        addAsmCode(createAsmCode(code, true));
    } else if (op->kind == DEREF) {
        if (op->derefObj->kind != VARIABLE)
            printAsmError("ERROR in handleOperand! Deref object is not variable.");

        int addr = getOffset(addrTable, op->derefObj->var);
        if (addr == 0)
            printAsmError("ERROR in handleOperand! op->derefObj->var is not in addrTable.");

        char* rightReg = getReg();
        addLoadCode(rightReg, addr);

        char* code = (char*)malloc(strlen(reg) + strlen(rightReg) + 8);
        sprintf(code, "lw %s, 0(%s)", reg, rightReg);
        addAsmCode(createAsmCode(code, true));

        freeReg(rightReg);
    } else {
        printAsmError("ERROR in handleOperand! Unknown operand kind.");
    }
}