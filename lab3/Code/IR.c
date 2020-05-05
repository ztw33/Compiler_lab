#include "IR.h"
#include "semantic.h"
#include <assert.h>

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
    if (debug) {
        switch (code->kind) {
        case LABEL:
            printf("LABEL label%d :\n", code->labelID);
            break;
        case FUNCTION:
            printf("FUNCTION %s :\n", code->funcName);
            break;
        case ASSIGN:
            printf("%s := %s\n", getOperandName(code->assign.left), getOperandName(code->assign.right));
            break;
        case ADD:
            printf("%s := %s + %s\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case SUB:
            printf("%s := %s - %s\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case MUL:
            printf("%s := %s * %s\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case DIV:
            printf("%s := %s / %s\n", getOperandName(code->binOp.result), getOperandName(code->binOp.op1), getOperandName(code->binOp.op2));
            break;
        case GOTO:
            printf("GOTO label%d\n", code->gotoLabelID);
            break;
        case IF_GOTO:
            printf("IF %s %s %s GOTO label%d\n", getOperandName(code->if_goto.cond->op1), getRelop(code->if_goto.cond), getOperandName(code->if_goto.cond->op2), code->if_goto.gotoLabelID);
            break;
        case RETURN:
            printf("RETURN %s\n", getOperandName(code->retVal));
            break;
        case DEC:
            printf("DEC %s %d\n", getVarName(code->dec.var), code->dec.size);
            break;
        case ARG:
            printf("ARG %s\n", getOperandName(code->arg));
            break;
        case CALL:
            printf("%s := CALL %s\n", getVarName(code->call.ret), code->call.funcName);
            break;
        case PARAM:
            printf("PARAM %s\n", getVarName(code->param));
            break;
        case READ:
            printf("READ %s\n", getVarName(code->rwVar));
            break;
        case WRITE:
            printf("WRITE %s\n", getVarName(code->rwVar));
            break; 
        default:
            fprintf(stderr, "\033[31mERROR in addCode! Unknown code kind.\033[0m\n");
            break;
        }
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
            fprintf(file, "IF %s %s %s GOTO label%d\n", getOperandName(code->if_goto.cond->op1), getRelop(code->if_goto.cond), getOperandName(code->if_goto.cond->op2), code->if_goto.gotoLabelID);
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

Operand* getConstant(int val) {
    if(debug) printf("getConstant:\tval=%d\n", val);
    char* s = (char*)malloc(16*sizeof(char));
    sprintf(s, "%d", val);
    Symbol* symbol = get(symbolTable, s, CONST);
    if (symbol != NULL) {
        Variable* t = symbol->alias;
        Operand* op = createOperand(VARIABLE);
        op->var = t;
        return op;
    } else {
        Variable* t = createVar(T);
        
        Symbol* symbol = createSymbol(s, CONST);
        symbol->type = TYPE_INT;
        symbol->alias = t;
        insert(symbolTable, symbol);

        Operand* op_t = createOperand(VARIABLE);
        op_t->var = t;
        Operand* op_i = createOperand(CONSTANT);
        op_i->constVal = val;
        addCode(createASSIGN(op_t, op_i));
        return op_t;
    }
}

void IRInit() {
    codesHead = NULL;
    codesTail = NULL;
}

InterCodes* generateIR(const Node* syntaxTreeRootNode) {
    IRInit();
    /* 首次尝试
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
    addCode(createIFGOTO(createCondExp(op_v1, GT, op_t2), 1));
    addCode(createIFGOTO(createCondExp(op_v1, LT, op_t2), 2));
    addCode(createWRITE(t2));
    addCode(createGOTO(3));
    addCode(createLABEL(1));
    Variable* t3 = createVar(T);
    Operand* op_t3 = createOperand(VARIABLE);
    op_t3->var = t3;
    Operand* op_1 = createOperand(CONSTANT);
    op_1->constVal = 1;
    addCode(createASSIGN(op_t3, op_1));
    addCode(createWRITE(t3));
    addCode(createGOTO(3));
    addCode(createLABEL(2));
    Variable* t4 = createVar(T);
    Operand* op_t4 = createOperand(VARIABLE);
    op_t4->var = t4;
    Operand* op_m1 = createOperand(CONSTANT);
    op_m1->constVal = -1;
    addCode(createASSIGN(op_t4, op_m1));
    addCode(createWRITE(t4));
    addCode(createLABEL(3));
    addCode(createRETURN(op_t2));
    */
    translateProgram(syntaxTreeRootNode);
    return codesHead;
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

void translateExtDefList(const Node* ExtDefList) {
    if(debug) printf("translateExtDefList:\t");
    if (ExtDefList == NULL) return;

    if (usedThisProd(ExtDefList, 1, "ExtDef")) {
        // ExtDefList := ExtDef ExtDefList(== NULL)
        if(debug) printf("ExtDefList := ExtDef ExtDefList(== NULL)\n");
        translateExtDef(ExtDefList->firstChild);
    } else if (usedThisProd(ExtDefList, 2, "ExtDef", "ExtDefList")) {
        // ExtDefList := ExtDef ExtDefList(!= NULL)
        if(debug) printf("ExtDefList := ExtDef ExtDefList(!= NULL)\n");
        translateExtDef(ExtDefList->firstChild);
        translateExtDefList(ExtDefList->firstChild->nextSibling);
    } else {
        fprintf(stderr, "\033[31mERROR in translateExtDefList! No matched production.\033[0m\n");
    }
}

void translateExtDef(const Node* ExtDef) {
    if(debug) printf("translateExtDef:\t");

    if (usedThisProd(ExtDef, 2, "Specifier", "SEMI")) {
        //ExtDef := Specifier SEMI
        return;

    } else if (usedThisProd(ExtDef, 3, "Specifier", "FunDec", "CompSt")) {
        //ExtDef := Specifier FunDec CompSt
        if(debug) printf("ExtDef := Specifier FunDec CompSt\n");
        
        translateFunDec(ExtDef->firstChild->nextSibling);
        translateCompSt(ExtDef->firstChild->nextSibling->nextSibling);
    } else if (usedThisProd(ExtDef, 3, "Specifier", "ExtDecList", "SEMI")) {
        //ExtDef := Specifier ExtDecList SEMI
        if(debug) printf("ExtDef := Specifier ExtDecList SEMI\n");
        fprintf(stderr, "\033[31mERROR in translateExtDef! 不应该有全局变量的使用.\033[0m\n");
    } else {
        fprintf(stderr, "\033[31mERROR in translateExtDef! No matched production.\033[0m\n");
    }
}

void translateFunDec(const Node* FunDec) {
    if(debug) printf("translateFunDec:\t");

    if (usedThisProd(FunDec, 3, "ID", "LP", "RP")) {
        // FunDec := ID LP RP
        if(debug) printf("FunDec := ID LP RP\n");
        addCode(createFUNCTION(FunDec->firstChild->stringVal));
    } else if (usedThisProd(FunDec, 4, "ID", "LP", "VarList", "RP")) {
        // FunDec := ID LP VarList RP
        if(debug) printf("FunDec := ID LP VarList RP\n");
        Node* ID = FunDec->firstChild;
        addCode(createFUNCTION(ID->stringVal));
        translateVarList(ID->nextSibling->nextSibling);
    } else {
        fprintf(stderr, "\033[31mERROR in translateFunDec! No matched production.\033[0m\n");
    }
}

void translateCompSt(const Node* CompSt) {
    if(debug) printf("translateCompSt:\t");

    if (usedThisProd(CompSt, 4, "LC", "DefList", "StmtList", "RC")) {
        // CompSt := LC DefList(!= NULL) StmtList(!= NULL) RC
        if(debug) printf("CompSt := LC DefList(!= NULL) StmtList(!= NULL) RC\n");
        translateStmtList(CompSt->firstChild->nextSibling->nextSibling);
    } else if (usedThisProd(CompSt, 3, "LC", "DefList", "RC")) {
        // CompSt := LC DefList(!= NULL) StmtList(== NULL) RC
        if(debug) printf("CompSt := LC DefList(!= NULL) StmtList(== NULL) RC\n");
        return;
    } else if (usedThisProd(CompSt, 3, "LC", "StmtList", "RC")) {
        // CompSt := LC DefList(== NULL) StmtList(!= NULL) RC
        if(debug) printf("CompSt := LC DefList(== NULL) StmtList(!= NULL) RC\n");
        translateStmtList(CompSt->firstChild->nextSibling);
    } else if (usedThisProd(CompSt, 2, "LC", "RC")) {
        // CompSt := LC DefList(== NULL) StmtList(== NULL) RC
        if(debug) printf("CompSt := LC DefList(== NULL) StmtList(== NULL) RC\n");
        return;
    } else {
        fprintf(stderr, "\033[31mERROR in translateCompSt! No matched production.\033[0m\n");
    }
}

void translateVarList(const Node* VarList) {
    if(debug) printf("translateVarList:\t");

    if (usedThisProd(VarList, 1, "ParamDec")) {
        // VarList := ParamDec
        if(debug) printf("VarList := ParamDec\n");
        Variable* var = createVar(V);
        addCode(createPARAM(var));
    } else if (usedThisProd(VarList, 3, "ParamDec", "COMMA", "VarList")) {
        // VarList := ParamDec COMMA VarList
        if(debug) printf("VarList := ParamDec COMMA VarList\n");
        Variable* var = createVar(V);
        addCode(createPARAM(var));
        translateVarList(VarList->firstChild->nextSibling->nextSibling);
    } else {
        fprintf(stderr, "\033[31mERROR in translateVarList! No matched production.\033[0m\n");
    }
}

void translateStmtList(const Node* StmtList) {
    if(debug) printf("translateStmtList:\t");

    if (usedThisProd(StmtList, 2, "Stmt", "StmtList")) {
        // StmtList := Stmt StmtList(!= NULL)
        if(debug) printf("StmtList := Stmt StmtList(!= NULL)\n");
        translateStmt(StmtList->firstChild);
        translateStmtList(StmtList->firstChild->nextSibling);
    } else if (usedThisProd(StmtList, 1, "Stmt")) {
        // StmtList : Stmt StmtList(== NULL)
        if(debug) printf("StmtList := Stmt StmtList(== NULL)\n");
        translateStmt(StmtList->firstChild);
    } else {
        fprintf(stderr, "\033[31mERROR in translateStmtList! No matched production.\033[0m\n");
    }
}

void translateStmt(const Node* Stmt) {
    if(debug) printf("translateStmt:\t");

    if (usedThisProd(Stmt, 2, "Exp", "SEMI")) {
        // Stmt := Exp SEMI
        if(debug) printf("Stmt := Exp SEMI\n");
        Operand* t = translateExp(Stmt->firstChild);
    } else if(usedThisProd(Stmt, 3, "RETURN", "Exp", "SEMI")) {
        // Stmt := RETURN Exp SEMI
        if(debug) printf("Stmt := RETURN Exp SEMI\n");
        Operand* ret = translateExp(Stmt->firstChild->nextSibling);
        addCode(createRETURN(ret));
    } else if (usedThisProd(Stmt, 5, "IF", "LP", "Exp", "RP", "Stmt")) {
        // Stmt := IF LP Exp RP Stmt
        if(debug) printf("Stmt := IF LP Exp RP Stmt\n");

        Node* nodeExp = Stmt->firstChild->nextSibling->nextSibling;
        int trueLabelID = createNewLabel();
        int nextLabelID = createNewLabel();
        translateCondExp(nodeExp, trueLabelID, nextLabelID);
        addCode(createLABEL(trueLabelID));
        translateStmt(nodeExp->nextSibling->nextSibling);
        addCode(createLABEL(nextLabelID));
    } else if (usedThisProd(Stmt, 7, "IF", "LP", "Exp", "RP", "Stmt", "ELSE", "Stmt")) {
        // Stmt := IF LP Exp RP Stmt ELSE Stmt
        if(debug) printf("Stmt := IF LP Exp RP Stmt ELSE Stmt\n");

        Node* nodeExp = Stmt->firstChild->nextSibling->nextSibling;
        int trueLabelID = createNewLabel();
        int falseLabelID = createNewLabel();
        int nextLabelID = createNewLabel();
        translateCondExp(nodeExp, trueLabelID, falseLabelID);
        addCode(createLABEL(trueLabelID));
        translateStmt(nodeExp->nextSibling->nextSibling);
        addCode(createGOTO(nextLabelID));
        addCode(createLABEL(falseLabelID));
        translateStmt(nodeExp->nextSibling->nextSibling->nextSibling->nextSibling);
        addCode(createLABEL(nextLabelID));
    } else if (usedThisProd(Stmt, 1, "CompSt")) {
        // Stmt := CompSt
        if(debug) printf("Stmt := CompSt\n");
        translateCompSt(Stmt->firstChild);
    } else if (usedThisProd(Stmt, 5, "WHILE", "LP", "Exp", "RP", "Stmt")) {
        // Stmt := WHILE LP Exp RP Stmt
        if(debug) printf("Stmt := WHILE LP Exp RP Stmt\n");
        
        int beginLabelID = createNewLabel();
        int trueLabelID = createNewLabel();
        int nextLabelID = createNewLabel();
        Node* nodeExp = Stmt->firstChild->nextSibling->nextSibling;
        addCode(createLABEL(beginLabelID));
        translateCondExp(nodeExp, trueLabelID, nextLabelID);
        addCode(createLABEL(trueLabelID));
        translateStmt(nodeExp->nextSibling->nextSibling);
        addCode(createGOTO(beginLabelID));
        addCode(createLABEL(nextLabelID));
    } else {
        fprintf(stderr, "\033[31mERROR in translateStmt! No matched production.\033[0m\n");
    }
}

// 算术表达式
Operand* translateExp(const Node* Exp) {
    if(debug) printf("translateExp:\t");

    if (usedThisProd(Exp, 3, "Exp", "ASSIGNOP", "Exp")) {
        // Exp := Exp ASSIGNOP Exp
        if(debug) printf("Exp := Exp ASSIGNOP Exp\n");
        Operand* left = translateExp(Exp->firstChild);
        Operand* right = translateExp(Exp->firstChild->nextSibling->nextSibling);
        addCode(createASSIGN(left, right));
        return left;
    } else if (usedThisProd(Exp, 3, "Exp", "AND", "Exp")) {
        // Exp := Exp AND Exp
        if(debug) printf("Exp := Exp AND Exp\n");

        Operand* op1 = translateExp(Exp->firstChild);
        Operand* op2 = translateExp(Exp->firstChild->nextSibling->nextSibling);
        
        Operand* op_0 = getConstant(0);
        CondExp* cond1 = createCondExp(op1, EQ, op_0);
        CondExp* cond2 = createCondExp(op2, EQ, op_0);
        Variable* ret = createVar(T);
        Operand* op_ret = createOperand(VARIABLE);
        op_ret->var = ret;
        int falseLabelID = createNewLabel();
        int nextLabelID = createNewLabel();
        addCode(createIFGOTO(cond1, falseLabelID));
        addCode(createIFGOTO(cond2, falseLabelID));
        addCode(createASSIGN(op_ret, getConstant(1)));
        addCode(createGOTO(nextLabelID));
        addCode(createLABEL(falseLabelID));
        addCode(createASSIGN(op_ret, op_0));
        addCode(createLABEL(nextLabelID));
        return op_ret;
    } else if (usedThisProd(Exp, 3, "Exp", "OR", "Exp")) {
        // Exp := Exp OR Exp
        if(debug) printf("Exp := Exp OR Exp\n");

        Operand* op1 = translateExp(Exp->firstChild);
        Operand* op2 = translateExp(Exp->firstChild->nextSibling->nextSibling);
        
        Operand* op_1 = getConstant(1);
        CondExp* cond1 = createCondExp(op1, EQ, op_1);
        CondExp* cond2 = createCondExp(op2, EQ, op_1);
        Variable* ret = createVar(T);
        Operand* op_ret = createOperand(VARIABLE);
        op_ret->var = ret;
        int trueLabelID = createNewLabel();
        int nextLabelID = createNewLabel();
        addCode(createIFGOTO(cond1, trueLabelID));
        addCode(createIFGOTO(cond2, trueLabelID));
        addCode(createASSIGN(op_ret, getConstant(0)));
        addCode(createGOTO(nextLabelID));
        addCode(createLABEL(trueLabelID));
        addCode(createASSIGN(op_ret, op_1));
        addCode(createLABEL(nextLabelID));
        return op_ret;
    } else if (usedThisProd(Exp, 3, "Exp", "RELOP", "Exp")) {
        // Exp := Exp RELOP Exp
        if(debug) printf("Exp := Exp RELOP Exp\n");

    } else if (usedThisProd(Exp, 2, "NOT", "Exp")) {
        /* 逻辑运算
        // Exp := NOT Exp
        */
        if(debug) printf("Exp := NOT Exp\n");

    } else if (usedThisProd(Exp, 3, "Exp", "PLUS", "Exp") || usedThisProd(Exp, 3, "Exp", "MINUS", "Exp") || usedThisProd(Exp, 3, "Exp", "STAR", "Exp") || usedThisProd(Exp, 3, "Exp", "DIV", "Exp")) {
        /* 算术运算
        // Exp := Exp PLUS Exp
        // Exp := Exp MINUS Exp
        // Exp := Exp STAR Exp
        // Exp := Exp DIV Exp
        */
        if(debug) printf("Exp := Exp PLUS/MINUS/STAR/DIV Exp\n");
        
    } else if (usedThisProd(Exp, 2, "MINUS", "Exp")) {
        // Exp := MINUS Exp
        if(debug) printf("Exp := MINUS Exp\n");

        // 负数
        if (usedThisProd(Exp->firstChild->nextSibling, 1, "INT")) {
            return getConstant(0 - Exp->firstChild->nextSibling->firstChild->intVal);
        } else {
            Operand* op_0 = getConstant(0);
            Operand* op = translateExp(Exp->firstChild->nextSibling);

            Variable* res = createVar(T);
            Operand* op_res = createOperand(VARIABLE);
            op_res->var = res;
            addCode(createBinOp(op_res, SUB, op_0, op));
            return op_res;
        }
    } else if (usedThisProd(Exp, 3, "LP", "Exp", "RP")) {
        // Exp := LP Exp RP
        if(debug) printf("Exp := LP Exp RP\n");

    } else if (usedThisProd(Exp, 4, "ID", "LP", "Args", "RP")) {
        // Exp := ID LP Args RP
        if(debug) printf("Exp := ID LP Args RP\n");

        if (equalString(Exp->firstChild->stringVal, "write")) {
            Node* nodeArgs = Exp->firstChild->nextSibling->nextSibling;
            Operand* op = translateExp(nodeArgs->firstChild);
            if (op->kind != VARIABLE) {
                fprintf(stderr, "\033[31mERROR in translateExp! Write object is not a variable.\033[0m\n");
                assert(0);
            }
            addCode(createWRITE(op->var));
        } else {
            
        }
        
    } else if (usedThisProd(Exp, 3, "ID", "LP", "RP")) {
        // Exp := ID LP RP
        if(debug) printf("Exp := ID LP RP\n");

        if (equalString(Exp->firstChild->stringVal, "read")) {
            Variable* t = createVar(T);
            addCode(createREAD(t));
            Operand* op_t = createOperand(VARIABLE);
            op_t->var = t;
            return op_t;
        } else {

        }
    } else if (usedThisProd(Exp, 4, "Exp", "LB", "Exp", "RB")) {
        // Exp := Exp LB Exp RB
        if(debug) printf("Exp := Exp LB Exp RB\n");

    } else if (usedThisProd(Exp, 3, "Exp", "DOT", "ID")) {
        // Exp := Exp DOT ID
        if(debug) printf("Exp := Exp DOT ID\n");

        // TODO
    } else if (usedThisProd(Exp, 1, "ID")) {
        // Exp := ID
        if(debug) printf("Exp := ID\n");

        Symbol* symbol = get(symbolTable, Exp->firstChild->stringVal, VAR);
        if (symbol == NULL) {
            fprintf(stderr, "\033[31mERROR in translateExp! Cannot get ID from symbolTable.\033[0m\n");
            return NULL;
        }
        if (symbol->alias == NULL) {
            Variable* v = createVar(V);
            symbol->alias = v;
            Operand* op_v = createOperand(VARIABLE);
            op_v->var = v;
            return op_v;
        } else {
            Operand* op = createOperand(VARIABLE);
            op->var = symbol->alias;
            return op;
        }
    } else if (usedThisProd(Exp, 1, "INT")) {
        // Exp := INT
        if(debug) printf("Exp := INT\n");

        return getConstant(Exp->firstChild->intVal);
    } else if (usedThisProd(Exp, 1, "FLOAT")) {
        // Exp := FLOAT
        if(debug) printf("Exp := FLOAT\n");

        fprintf(stderr, "\033[31mERROR in translateExp! 不应存在浮点数.\033[0m\n");
    } else {
        fprintf(stderr, "\033[31mERROR in translateExp! No matched production.\033[0m\n");
    }
}

// if和while语句中的条件表达式
void translateCondExp(const Node* Exp, int trueLabelID, int falseLabelID) {
    if(debug) printf("translateCondExp:\t");

    if (usedThisProd(Exp, 3, "Exp", "ASSIGNOP", "Exp")) {
        // Exp := Exp ASSIGNOP Exp
        if(debug) printf("Exp := Exp ASSIGNOP Exp\n");
        
        
    } else if (usedThisProd(Exp, 3, "Exp", "AND", "Exp")) {
        // Exp := Exp AND Exp
        if(debug) printf("Exp := Exp AND Exp\n");

    } else if (usedThisProd(Exp, 3, "Exp", "OR", "Exp")) {
        // Exp := Exp OR Exp
        if(debug) printf("Exp := Exp OR Exp\n");

    } else if (usedThisProd(Exp, 3, "Exp", "RELOP", "Exp")) {
        // Exp := Exp RELOP Exp
        if(debug) printf("Exp := Exp RELOP Exp\n");

        Operand* op1 = translateExp(Exp->firstChild);
        Node* nodeRELOP = Exp->firstChild->nextSibling;
        RelopKind rk;
        if (equalString(nodeRELOP->stringVal, "==")) {
            rk = EQ;
        } else if (equalString(nodeRELOP->stringVal, "!=")) {
            rk = NEQ;
        } else if (equalString(nodeRELOP->stringVal, "<")) {
            rk = LT;
        } else if (equalString(nodeRELOP->stringVal, ">")) {
            rk = GT;
        } else if (equalString(nodeRELOP->stringVal, "<=")) {
            rk = LE;
        } else if (equalString(nodeRELOP->stringVal, ">=")) {
            rk = GE;
        } else {
            fprintf(stderr, "\033[31mERROR in translateCondExp! Unknown relop.\033[0m\n");
            assert(0);
        }
        Operand* op2 = translateExp(nodeRELOP->nextSibling);
        CondExp* cond = createCondExp(op1, rk, op2);
        addCode(createIFGOTO(cond, trueLabelID));
        addCode(createGOTO(falseLabelID));
    } else if (usedThisProd(Exp, 2, "NOT", "Exp")) {
        // Exp := NOT Exp
        if(debug) printf("Exp := NOT Exp\n");

    } else if (usedThisProd(Exp, 3, "Exp", "PLUS", "Exp") || usedThisProd(Exp, 3, "Exp", "MINUS", "Exp") || usedThisProd(Exp, 3, "Exp", "STAR", "Exp") || usedThisProd(Exp, 3, "Exp", "DIV", "Exp")) {
        /* 算术运算
        // Exp := Exp PLUS Exp
        // Exp := Exp MINUS Exp
        // Exp := Exp STAR Exp
        // Exp := Exp DIV Exp
        */
        if(debug) printf("Exp := Exp PLUS/MINUS/STAR/DIV Exp\n");
        
    } else if (usedThisProd(Exp, 2, "MINUS", "Exp")) {
        /* 算术运算
        // Exp := MINUS Exp
        */
        if(debug) printf("Exp := MINUS Exp\n");

    } else if (usedThisProd(Exp, 3, "LP", "Exp", "RP")) {
        // Exp := LP Exp RP
        if(debug) printf("Exp := LP Exp RP\n");

    } else if (usedThisProd(Exp, 4, "ID", "LP", "Args", "RP")) {
        // Exp := ID LP Args RP
        if(debug) printf("Exp := ID LP Args RP\n");

    } else if (usedThisProd(Exp, 3, "ID", "LP", "RP")) {
        // Exp := ID LP RP
        if(debug) printf("Exp := ID LP RP\n");

    } else if (usedThisProd(Exp, 4, "Exp", "LB", "Exp", "RB")) {
        // Exp := Exp LB Exp RB
        if(debug) printf("Exp := Exp LB Exp RB\n");

    } else if (usedThisProd(Exp, 3, "Exp", "DOT", "ID")) {
        // Exp := Exp DOT ID
        if(debug) printf("Exp := Exp DOT ID\n");

    } else if (usedThisProd(Exp, 1, "ID")) {
        // Exp := ID
        if(debug) printf("Exp := ID\n");

    } else if (usedThisProd(Exp, 1, "INT")) {
        // Exp := INT
        if(debug) printf("Exp := INT\n");

    } else if (usedThisProd(Exp, 1, "FLOAT")) {
        // Exp := FLOAT
        if(debug) printf("Exp := FLOAT\n");

        fprintf(stderr, "\033[31mERROR in translateCondExp! 不存在浮点数.\033[0m\n");
    } else {
        fprintf(stderr, "\033[31mERROR in translateCondExp! No matched production.\033[0m\n");
    }
}