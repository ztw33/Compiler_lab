#include "InterCode.h"
#include <stdio.h>

int getVarID(VarKind kind) {
    if (kind == T) {
        tCount++;
        return tCount;
    } else if (kind == V) {
        vCount++;
        return vCount;
    } else {
        fprintf(stderr, "\033[31mERROR in getVarID! Unknown var kind.\033[0m\n");
        return -1;
    }
}

Variable* createVar(VarKind kind) {
    Variable* var = (Variable*)malloc(sizeof(Variable));
    var->kind = kind;
    var->id = getVarID(kind);
    return var;
}

Operand* createOperand(OperandKind kind) {
    Operand* opr = (Operand*)malloc(sizeof(Operand));
    opr->kind = kind;
    return opr;
}

/* create IR */
InterCode* createLABEL(int id) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = LABEL;
    code->labelID = id;
    return code;
}

InterCode* createFUNCTION(char* funcName) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = FUNCTION;
    code->funcName = funcName;
    return code;
}

InterCode* createASSIGN(Operand* left, Operand* right) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = ASSIGN;
    code->assign.left = left;
    code->assign.right = right;
    return code;
}

InterCode* createBinOp(Operand* result, CodeKind op, Operand* op1, Operand* op2) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    switch (op) {
    case ADD:
        code->kind = ADD;
        break;
    case SUB:
        code->kind = SUB;
        break;
    case MUL:
        code->kind = MUL;
        break;
    case DIV:
        code->kind = DIV;
        break;
    default:
        fprintf(stderr, "\033[31mERROR in createBinOp! Wrong op.\033[0m\n");
        return NULL;
    }
    code->binOp.result = result;
    code->binOp.op1 = op1;
    code->binOp.op2 = op2;
    return code;
}

InterCode* createGOTO(int labelID) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = GOTO;
    code->gotoLabelID = labelID;
    return code;
}

InterCode* createIFGOTO(CondExp* cond, int labelID) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = IF_GOTO;
    code->if_goto.cond = cond;
    code->if_goto.gotoLabelID = labelID;
    return code;
}

InterCode* createRETURN(Operand* retVal) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = RETURN;
    code->retVal = retVal;
    return code;
}

InterCode* createDEC(Variable* var, int size) {

}

InterCode* createARG(Operand* arg) {

}

InterCode* createCALL(Variable* ret, char* funcName) {

}

InterCode* createPARAM(Variable* param) {

}

InterCode* createREAD(Variable* var) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = READ;
    code->rwVar = var;
    return code;
}

InterCode* createWRITE(Variable* var) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = WRITE;
    code->rwVar = var;
    return code;
}