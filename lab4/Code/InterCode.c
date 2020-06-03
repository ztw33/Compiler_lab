#include "InterCode.h"
#include <stdio.h>
#include <stdlib.h>

int tCount = 0;
int vCount = 0;
int labelID = 0;

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

int createNewLabel() {
    labelID++;
    return labelID;
}

Variable* createVar(VarKind kind) {
    Variable* var = (Variable*)malloc(sizeof(Variable));
    var->kind = kind;
    var->id = getVarID(kind);
    var->isAddr = false;
    return var;
}

Operand* createOperand(OperandKind kind) {
    Operand* opr = (Operand*)malloc(sizeof(Operand));
    opr->kind = kind;
    return opr;
}

CondExp* createCondExp(Operand* op1, RelopKind relop, Operand* op2) {
    CondExp* exp = (CondExp*)malloc(sizeof(CondExp));
    exp->op1 = op1;
    exp->relop = relop;
    exp->op2 = op2;
    return exp;
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
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = DEC;
    code->dec.var = var;
    code->dec.size = size;
    return code;
}

InterCode* createARG(Operand* arg) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = ARG;
    code->arg = arg;
    return code;
}

InterCode* createCALL(Variable* ret, char* funcName) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = CALL;
    code->call.ret = ret;
    code->call.funcName = funcName;
    return code;
}

InterCode* createPARAM(Variable* param) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = PARAM;
    code->param = param;
    return code;
}

InterCode* createREAD(Operand* op) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = READ;
    code->rwOperand = op;
    return code;
}

InterCode* createWRITE(Operand* op) {
    InterCode* code = (InterCode*)malloc(sizeof(InterCode));
    code->kind = WRITE;
    code->rwOperand = op;
    return code;
}

char* getVarName(Variable* var) {
    char* name = (char*)malloc(16*sizeof(char));
    sprintf(name, "%c%d", var->kind == T ? 't' : 'v', var->id);
    return name;
}

char* getOperandName(Operand* op) {
    if (op->kind == VARIABLE) {
        return getVarName(op->var);
    } else if (op->kind == CONSTANT) {
        char* name = (char*)malloc(16*sizeof(char));
        sprintf(name, "#%d", op->constVal);
        return name;
    } else if (op->kind == REF) {
        char* name = (char*)malloc(16*sizeof(char));
        sprintf(name, "&%s", getOperandName(op->refObj));
        return name;
    } else if (op->kind == DEREF) {
        char* name = (char*)malloc(16*sizeof(char));
        sprintf(name, "*%s", getOperandName(op->derefObj));
        return name;
    } else {
        fprintf(stderr, "\033[31mERROR in getOperandName! Unknown operand kind.\033[0m\n");
        return NULL;
    }
}

char* getRelop(CondExp* cond) {
    switch (cond->relop) {
    case EQ:
        return "==";
    case NEQ:
        return "!=";
    case LT:
        return "<";
    case GT:
        return ">";
    case LE:
        return "<=";
    case GE:
        return ">=";
    default:
        fprintf(stderr, "\033[31mERROR in getRelop! Unknown relop kind.\033[0m\n");
        return NULL;
    }
}

void printCode(InterCode* code) {
    switch (code->kind) {
        case LABEL:
            printf("\033[34mLABEL label%d :\033[0m\n", code->labelID);
            break;
        case FUNCTION:
            printf("\033[34mFUNCTION %s :\033[0m\n", code->funcName);
            break;
        case ASSIGN:
            printf("\033[34m%s := %s\033[0m\n", getOperandName(code->assign.left), getOperandName(code->assign.right));
            break;
        case ADD:
            printf("\033[34m%s := %s + %s\033[0m\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case SUB:
            printf("\033[34m%s := %s - %s\033[0m\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case MUL:
            printf("\033[34m%s := %s * %s\033[0m\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case DIV:
            printf("\033[34m%s := %s / %s\033[0m\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case GOTO:
            printf("\033[34mGOTO label%d\033[0m\n", code->gotoLabelID);
            break;
        case IF_GOTO:
            printf("\033[34mIF %s %s %s GOTO label%d\033[0m\n", getOperandName(code->if_goto.cond->op1), getRelop(code->if_goto.cond), getOperandName(code->if_goto.cond->op2), code->if_goto.gotoLabelID);
            break;
        case RETURN:
            printf("\033[34mRETURN %s\033[0m\n", getOperandName(code->retVal));
            break;
        case DEC:
            printf("\033[34mDEC %s %d\033[0m\n", getVarName(code->dec.var), code->dec.size);
            break;
        case ARG:
            printf("\033[34mARG %s\033[0m\n", getOperandName(code->arg));
            break;
        case CALL:
            printf("\033[34m%s := CALL %s\033[0m\n", getVarName(code->call.ret), code->call.funcName);
            break;
        case PARAM:
            printf("\033[34mPARAM %s\033[0m\n", getVarName(code->param));
            break;
        case READ:
            printf("\033[34mREAD %s\033[0m\n", getOperandName(code->rwOperand));
            break;
        case WRITE:
            printf("\033[34mWRITE %s\033[0m\n", getOperandName(code->rwOperand));
            break; 
        default:
            fprintf(stderr, "\033[31mERROR in addCode! Unknown code kind.\033[0m\n");
            break;
    }
}

void removeCode(InterCodes* code) {
    if (code == NULL || code->code == NULL) return;
    InterCodes* prev = code->prev;
    InterCodes* next = code->next;
    if (prev != NULL && next != NULL) {
        prev->next = next;
        next->prev = prev;
    } else if (prev != NULL && next == NULL) {
        prev->next = next;
    } else if (prev == NULL && next != NULL) {
        printf("\033[33m删除的代码位于链表头部\033[0m\n");
        next->prev = prev;
    }
    free(code->code);
    free(code);
    code = NULL;
}