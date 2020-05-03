#include "IR.h"
#include "semantic.h"

#define debug 1

void addCode(InterCode* code) {
    if (codesHead == NULL) {
        InterCodes* head = (InterCodes*)malloc(sizeof(InterCodes));
        head->code = code;
        head->prev = NULL;
        head->next = NULL;
        codesHead = head;
        codesTail = codesHead;
    } else {
        InterCodes* tail = (InterCodes*)malloc(sizeof(InterCodes));
        tail->code = code;
        tail->prev = codesTail;
        tail->next = NULL;
        codesTail->next = tail;
        codesTail = tail;
    }
}

void addCodes(CodesListHT codes) {
    if (codesHead == NULL) {
        codesHead = codes.head;
        codesTail = codes.tail;
    } else {
        codesTail->next = codes.head;
        codes.head->prev = codesTail;
        codesTail = codes.tail;
    }
}

void outputIR(FILE* file, InterCodes* codes) {
    InterCodes* p = codes;
    while (p != NULL) {
        InterCode* code = p->code;
        switch (code->kind) {
        case LABEL:
            fprintf(file, "LABEL label%d :\n", code->labelID);
            break;
        case FUNCTION:
            fprintf(file, "FUNCTION %s :\n", code->funcName);
            break;
        case ASSIGN:
            fprintf(file, "%s := %s\n", getOperandName(code->assign.left), getOperandName(code->assign.right));
            break;
        case ADD:
            fprintf(file, "%s := %s + %s\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case SUB:
            fprintf(file, "%s := %s - %s\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case MUL:
            fprintf(file, "%s := %s * %s\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case DIV:
            fprintf(file, "%s := %s / %s\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case GOTO:
            fprintf(file, "GOTO label%d\n", code->gotoLabelID);
            break;
        case IF_GOTO:
            fprintf(file, "IF %s %s %s GOTO label%d\n", code->if_goto.cond->op1, getRelop(code->if_goto.cond), code->if_goto.cond->op2, code->if_goto.gotoLabelID);
            break;
        case RETURN:
            fprintf(file, "RETURN %s\n", getOperandName(code->retVal));
            break;
        case DEC:
            fprintf(file, "DEC %s %d\n", getVarName(code->dec.var), code->dec.size);
            break;
        case ARG:
            fprintf(file, "ARG %s\n", getOperandName(code->arg));
            break;
        case CALL:
            fprintf(file, "%s := CALL %s\n", getVarName(code->call.ret), code->call.funcName);
            break;
        case PARAM:
            fprintf(file, "PARAM %s\n", getVarName(code->param));
            break;
        case READ:
            fprintf(file, "READ %s\n", getVarName(code->rwVar));
            break;
        case WRITE:
            fprintf(file, "WRITE %s\n", getVarName(code->rwVar));
            break; 
        default:
            fprintf(stderr, "\033[31mERROR in outputIR! Unknown code kind.\033[0m\n");
            break;
        }
        p = p->next;
    } 
}

char* getVarName(Variable* var) {
    char name[16];
    sprintf(name, "%c%d", var->kind == T ? 't' : 'v', var->id);
    return name;
}

char* getOperandName(Operand* op) {
    if (op->kind == VARIABLE) {
        return getVarName(op->var);
    } else if (op->kind == CONSTANT) {
        char name[16];
        sprintf(name, "#%d", op->constVal);
        return name;
    } else if (op->kind == REF) {
        char name[16];
        sprintf(name, "&%s", getOperandName(op->refObj));
        return name;
    } else if (op->kind == DEREF) {
        char name[16];
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

InterCodes* generateIR(const Node* syntaxTreeRootNode) {
    addCode(createFUNCTION("main"));
    Variable* t1 = createVar(T);
    addCode(createREAD(t1));
    Variable* v1 = createVar(V);
    Operand* op_v1 = createOperand(VARIABLE);
    op_v1->var = v1;
    Operand* op_t1 = createOperand(VARIABLE);
    op_t1->var = t1;
    Operand* op_0 = createOperand(CONSTANT);
    op_0->constVal = 0;
    addCode(createASSIGN(op_v1, op_t1));
    Variable* t2 = createVar(T);
    Operand* op_t2 = createOperand(VARIABLE);
    op_t2->var = t2;
    addCode(createASSIGN(op_t2, op_0));

}

void translateProgram(const Node* Program) {
    if(debug) printf("translateProgram:\t");

    if (Program == NULL) return;
    if (usedThisProd(Program, 1, "ExtDefList")) {
        // Program := ExtDefList
        if(debug) printf("Program := ExtDefList\n");
        translateExtDefList(Program->firstChild);
    } else {
        fprintf(stderr, "\033[31mERROR in translateProgram! No matched production.\033[0m\n");
    }
}