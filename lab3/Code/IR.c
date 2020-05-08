#include "IR.h"
#include "semantic.h"
#include <assert.h>
#include "optimizeIR.h"

#define debug 0

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
        printCode(code);
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
    if (debug) {
        InterCodes* p = codes.head;
        while (p != codes.tail) {
            printCode(p->code);
        }
        printCode(codes.tail->code);
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
            fprintf(file, "READ %s\n", getOperandName(code->rwOperand));
            break;
        case WRITE:
            fprintf(file, "WRITE %s\n", getOperandName(code->rwOperand));
            break; 
        default:
            fprintf(stderr, "\033[31mERROR in outputIR! Unknown code kind.\033[0m\n");
            break;
        }
        p = p->next;
    } 
}

Operand* getConstant(int val) {
    Operand* op_i = createOperand(CONSTANT);
    op_i->constVal = val;
    return op_i;
}

void IRInit() {
    codesHead = NULL;
    codesTail = NULL;
}

InterCodes* generateIR(const Node* syntaxTreeRootNode) {
    IRInit();
    translateProgram(syntaxTreeRootNode);
    optimizeIR(codesHead);
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
        translateDefList(CompSt->firstChild->nextSibling);
        translateStmtList(CompSt->firstChild->nextSibling->nextSibling);
    } else if (usedThisProd(CompSt, 3, "LC", "DefList", "RC")) {
        // CompSt := LC DefList(!= NULL) StmtList(== NULL) RC
        if(debug) printf("CompSt := LC DefList(!= NULL) StmtList(== NULL) RC\n");
        translateDefList(CompSt->firstChild->nextSibling);
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
        translateParamDec(VarList->firstChild);
    } else if (usedThisProd(VarList, 3, "ParamDec", "COMMA", "VarList")) {
        // VarList := ParamDec COMMA VarList
        if(debug) printf("VarList := ParamDec COMMA VarList\n");
        translateParamDec(VarList->firstChild);
        translateVarList(VarList->firstChild->nextSibling->nextSibling);
    } else {
        fprintf(stderr, "\033[31mERROR in translateVarList! No matched production.\033[0m\n");
    }
}

void translateParamDec(const Node* ParamDec) {
    if(debug) printf("translateParamDec:\t");

    if(usedThisProd(ParamDec, 2, "Specifier", "VarDec")) {
        // ParamDec := Specifier VarDec
        if(debug) printf("ParamDec := Specifier VarDec\n");
        Node* nodeVarDec = ParamDec->firstChild->nextSibling;
        if(usedThisProd(nodeVarDec, 1, "ID")) {
            // VarDec := ID
            if(debug) printf("VarDec := ID\n");

            Symbol* symbol = get(symbolTable, nodeVarDec->firstChild->stringVal, VAR);
            if (symbol == NULL) {
                fprintf(stderr, "\033[31mERROR in translateParamDec! Cannot get ID from symbolTable.\033[0m\n");
            }
            if (symbol->alias == NULL) {
                Type* type = analyseSpecifier(ParamDec->firstChild);
                Variable* v = createVar(V);
                if (type->kind == STRUCTURE) {
                    v->isAddr = true;
                }
                symbol->alias = v;
                Operand* op_v = createOperand(VARIABLE);
                op_v->var = v;
                addCode(createPARAM(v));
            } else {
                fprintf(stderr, "\033[31mERROR in translateParamDec! 定义时符号已经有别名了.\033[0m\n");
                addCode(createPARAM(symbol->alias));
            }
        } else if (usedThisProd(nodeVarDec, 4, "VarDec", "LB", "INT", "RB")) {
            // VarDec := VarDec LB INT RB
            if(debug) printf("VarDec := VarDec LB INT RB\n");
            fprintf(stderr, "\033[31m无法翻译：数组不应作为参数传递.\033[0m\n");
            exit(0);           
        } else {
            fprintf(stderr, "\033[31mERROR in translateParamDec when analyse VarDec! No matched production.\033[0m\n");
        }
    } else {
        fprintf(stderr, "\033[31mERROR in translateParamDec! No matched production.\033[0m\n");
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
    } else if (usedThisProd(Exp, 3, "Exp", "AND", "Exp") || usedThisProd(Exp, 3, "Exp", "OR", "Exp") || usedThisProd(Exp, 3, "Exp", "RELOP", "Exp") || usedThisProd(Exp, 2, "NOT", "Exp")) {
        // Exp := Exp AND/OR/RELOP Exp / NOT Exp
        if(debug) printf("Exp := Exp AND/OR/RELOP Exp / NOT Exp\n");

        Variable* ret = createVar(T);
        Operand* op_ret = createOperand(VARIABLE);
        op_ret->var = ret;
        int label1 = createNewLabel();
        int label2 = createNewLabel();

        addCode(createASSIGN(op_ret, getConstant(0)));
        translateCondExp(Exp, label1, label2);
        addCode(createLABEL(label1));
        addCode(createASSIGN(op_ret, getConstant(1)));
        addCode(createLABEL(label2));
        return op_ret;
    } else if (usedThisProd(Exp, 3, "Exp", "PLUS", "Exp")) {
        // Exp := Exp PLUS Exp
        if(debug) printf("Exp := Exp PLUS Exp\n");

        Operand* op1 = translateExp(Exp->firstChild);
        Operand* op2 = translateExp(Exp->firstChild->nextSibling->nextSibling);        
        // 优化：若算术表达式的操作数为立即数，则可以直接把结果计算出来
        if (op1->kind == CONSTANT && op2->kind == CONSTANT) {
            return getConstant(op1->constVal + op2->constVal);
        } else {
            if (op1->kind == CONSTANT && op1->constVal == 0){
                return op2;
            } else if (op2->kind == CONSTANT && op2->constVal == 0) {
                return op1;
            } else {
                Variable* ret = createVar(T);
                Operand* op_ret = createOperand(VARIABLE);
                op_ret->var = ret;
                addCode(createBinOp(op_ret, ADD, op1, op2));
                return op_ret;
            }
        }
    } else if (usedThisProd(Exp, 3, "Exp", "MINUS", "Exp")) {
        // Exp := Exp MINUS Exp
        if(debug) printf("Exp := Exp MINUS Exp\n");
        
        Operand* op1 = translateExp(Exp->firstChild);
        Operand* op2 = translateExp(Exp->firstChild->nextSibling->nextSibling);
        // 优化：若算术表达式的操作数为立即数，则可以直接把结果计算出来
        if (op1->kind == CONSTANT && op2->kind == CONSTANT) {
            return getConstant(op1->constVal - op2->constVal);
        } else {
            if (op2->kind == CONSTANT && op2->constVal == 0) {
                return op1;
            } else {
                Variable* ret = createVar(T);
                Operand* op_ret = createOperand(VARIABLE);
                op_ret->var = ret;
                addCode(createBinOp(op_ret, SUB, op1, op2));
                return op_ret;
            }
        }
    } else if (usedThisProd(Exp, 3, "Exp", "STAR", "Exp")) {
        // Exp := Exp STAR Exp
        if(debug) printf("Exp := Exp STAR Exp\n");
        
        Operand* op1 = translateExp(Exp->firstChild);
        Operand* op2 = translateExp(Exp->firstChild->nextSibling->nextSibling);
        // 优化：若算术表达式的操作数为立即数，则可以直接把结果计算出来
        if (op1->kind == CONSTANT && op2->kind == CONSTANT) {
            return getConstant(op1->constVal * op2->constVal);
        } else {
            if (op1->kind == CONSTANT && op1->constVal == 0) {
                return getConstant(0);
            } else if (op1->kind == CONSTANT && op1->constVal == 1) {
                return op2;
            } else if (op2->kind == CONSTANT && op2->constVal == 0) {
                return getConstant(0);
            } else if (op2->kind == CONSTANT && op2->constVal == 1) {
                return op1;
            } else {
                Variable* ret = createVar(T);
                Operand* op_ret = createOperand(VARIABLE);
                op_ret->var = ret;
                addCode(createBinOp(op_ret, MUL, op1, op2));
                return op_ret;
            }
        }
    } else if (usedThisProd(Exp, 3, "Exp", "DIV", "Exp")) {
        // Exp := Exp DIV Exp
        if(debug) printf("Exp := Exp DIV Exp\n");
        
        Operand* op1 = translateExp(Exp->firstChild);
        Operand* op2 = translateExp(Exp->firstChild->nextSibling->nextSibling);
        
        if (op1->kind == CONSTANT && op2->kind == CONSTANT) {
            return getConstant(op1->constVal / op2->constVal);
        } else {
            if (op1->kind == CONSTANT && op1->constVal == 0) {
                return getConstant(0);
            } else if (op2->kind == CONSTANT && op2->constVal == 1) {
                return op1;
            } else {
                Variable* ret = createVar(T);
                Operand* op_ret = createOperand(VARIABLE);
                op_ret->var = ret;
                addCode(createBinOp(op_ret, DIV, op1, op2));
                return op_ret;
            }
        }
    } else if (usedThisProd(Exp, 2, "MINUS", "Exp")) {\
        // Exp := MINUS Exp
        if(debug) printf("Exp := MINUS Exp\n");

        Operand* op = translateExp(Exp->firstChild->nextSibling);
        // 优化：若算术表达式的操作数为立即数，则可以直接把结果计算出来
        if (op->kind == CONSTANT) {
            return getConstant(0 - op->constVal);
        } else {
            Variable* res = createVar(T);
            Operand* op_res = createOperand(VARIABLE);
            op_res->var = res;
            addCode(createBinOp(op_res, SUB, getConstant(0), op));
            return op_res;
        }
    } else if (usedThisProd(Exp, 3, "LP", "Exp", "RP")) {
        // Exp := LP Exp RP
        if(debug) printf("Exp := LP Exp RP\n");
        return translateExp(Exp->firstChild->nextSibling);
    } else if (usedThisProd(Exp, 4, "ID", "LP", "Args", "RP")) {
        // Exp := ID LP Args RP
        if(debug) printf("Exp := ID LP Args RP\n");

        if (equalString(Exp->firstChild->stringVal, "write")) {
            Node* nodeArgs = Exp->firstChild->nextSibling->nextSibling;
            Operand* op = translateExp(nodeArgs->firstChild);
            addCode(createWRITE(op));
        } else {
            CodesListHT argCodes = translateArgs(Exp->firstChild->nextSibling->nextSibling);
            addCodes(argCodes);
            Variable* ret = createVar(T);
            Operand* op_ret = createOperand(VARIABLE);
            op_ret->var = ret;
            addCode(createCALL(ret, Exp->firstChild->stringVal));
            return op_ret;
        }
    } else if (usedThisProd(Exp, 3, "ID", "LP", "RP")) {
        // Exp := ID LP RP
        if(debug) printf("Exp := ID LP RP\n");

        if (equalString(Exp->firstChild->stringVal, "read")) {
            Variable* t = createVar(T);
            Operand* op_t = createOperand(VARIABLE);
            op_t->var = t;
            addCode(createREAD(op_t));
            return op_t;
        } else {
            Variable* ret = createVar(T);
            Operand* op_ret = createOperand(VARIABLE);
            op_ret->var = ret;
            addCode(createCALL(ret, Exp->firstChild->stringVal));
            return op_ret;
        }
    } else if (usedThisProd(Exp, 4, "Exp", "LB", "Exp", "RB")) {
        // Exp := Exp LB Exp RB
        if(debug) printf("Exp := Exp LB Exp RB\n");

        Type* type = analyseExp(Exp->firstChild);
        if (type->kind != ARRAY) {
            fprintf(stderr, "\033[31mERROR in translateExp! Not array.\033[0m\n");
        }
        Operand* op_array = translateExp(Exp->firstChild);
        Operand* array_addr = NULL;
        if (op_array->kind == DEREF) {
            array_addr = op_array->derefObj;
        } else {
            array_addr = createOperand(REF);
            array_addr->refObj = op_array;
        }
        
        Operand* index = translateExp(Exp->firstChild->nextSibling->nextSibling);
        Variable* addr = createVar(T);
        Operand* op_addr = createOperand(VARIABLE);
        op_addr->var = addr;
        Operand* elem = NULL;
        if (op_addr->kind == REF) {
            elem = op_addr->refObj;
        } else {
            elem = createOperand(DEREF);
            elem->derefObj = op_addr;
        }
        int elem_size = getSizeofType(type->array.elem);
        if (index->kind == CONSTANT) {
            int offset = index->constVal * elem_size;
            if (offset == 0) {
                addCode(createASSIGN(op_addr, array_addr));
            } else {
                addCode(createBinOp(op_addr, ADD, array_addr, getConstant(offset)));  // addr = &array + offset
            }
        } else {
            Variable* offset = createVar(T);
            Operand* op_offset = createOperand(VARIABLE);
            op_offset->var = offset;
            addCode(createBinOp(op_offset, MUL, index, getConstant(elem_size)));  // offset := index * #[size]
            addCode(createBinOp(op_addr, ADD, array_addr, op_offset));  // addr = &array + offset
        }
        return elem;  // *addr
    } else if (usedThisProd(Exp, 3, "Exp", "DOT", "ID")) {
        // Exp := Exp DOT ID
        if(debug) printf("Exp := Exp DOT ID\n");

        Operand* op_struct = translateExp(Exp->firstChild);
        Type* type = analyseExp(Exp->firstChild);
        if (type->kind != STRUCTURE) {
            fprintf(stderr, "\033[31mERROR in translateExp! Type is not structure.\033[0m\n");
            return NULL;
        }
        Operand* struct_addr = NULL;
        if (op_struct->kind == VARIABLE && op_struct->var->isAddr) {
            struct_addr = op_struct;
        } else {
            if (op_struct->kind == DEREF) {
                struct_addr = op_struct->derefObj;
            } else {
                struct_addr = createOperand(REF);
                struct_addr->refObj = op_struct;
            }
        }
        int offset = getFieldOffset(type, Exp->firstChild->nextSibling->nextSibling->stringVal);
        Variable* addr = createVar(T);
        Operand* op_addr = createOperand(VARIABLE);
        op_addr->var = addr;
        if (offset == 0) {
            addCode(createASSIGN(op_addr, struct_addr));
        } else {
            addCode(createBinOp(op_addr, ADD, struct_addr, getConstant(offset)));
        }
        Operand* field = createOperand(DEREF);
        field->derefObj = op_addr;
        return field;
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

    if (usedThisProd(Exp, 3, "Exp", "AND", "Exp")) {
        // Exp := Exp AND Exp
        if(debug) printf("Exp := Exp AND Exp\n");

        int label = createNewLabel();
        translateCondExp(Exp->firstChild, label, falseLabelID);
        addCode(createLABEL(label));
        translateCondExp(Exp->firstChild->nextSibling->nextSibling, trueLabelID, falseLabelID);
    } else if (usedThisProd(Exp, 3, "Exp", "OR", "Exp")) {
        // Exp := Exp OR Exp
        if(debug) printf("Exp := Exp OR Exp\n");

        int label = createNewLabel();
        translateCondExp(Exp->firstChild, trueLabelID, label);
        addCode(createLABEL(label));
        translateCondExp(Exp->firstChild->nextSibling->nextSibling, trueLabelID, falseLabelID);
    } else if (usedThisProd(Exp, 3, "Exp", "RELOP", "Exp")) {
        // Exp := Exp RELOP Exp
        if(debug) printf("Exp := Exp RELOP Exp\n");
     
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
        Operand* op1 = translateExp(Exp->firstChild);
        Operand* op2 = translateExp(nodeRELOP->nextSibling);
        // 优化：若操作数为常数则可以直接判断true还是false
        if (op1->kind == CONSTANT && op2->kind == CONSTANT) {
            switch (rk) {
            case EQ:
                if (op1->constVal == op2->constVal)
                    createGOTO(trueLabelID);
                else
                    createGOTO(falseLabelID);
                break;
            case NEQ:
                if (op1->constVal != op2->constVal)
                    createGOTO(trueLabelID);
                else
                    createGOTO(falseLabelID);
                break;
            case LT:
                if (op1->constVal < op2->constVal)
                    createGOTO(trueLabelID);
                else
                    createGOTO(falseLabelID);
                break;
            case GT:
                if (op1->constVal > op2->constVal)
                    createGOTO(trueLabelID);
                else
                    createGOTO(falseLabelID);
                break;
            case LE:
                if (op1->constVal <= op2->constVal)
                    createGOTO(trueLabelID);
                else
                    createGOTO(falseLabelID);
                break;
            case GE:
                if (op1->constVal >= op2->constVal)
                    createGOTO(trueLabelID);
                else
                    createGOTO(falseLabelID);
                break;
            default:
                break;
            }
        } else {
            CondExp* cond = createCondExp(op1, rk, op2);
            addCode(createIFGOTO(cond, trueLabelID));
            addCode(createGOTO(falseLabelID));
        }
    } else if (usedThisProd(Exp, 2, "NOT", "Exp")) {
        // Exp := NOT Exp
        if(debug) printf("Exp := NOT Exp\n");
        translateCondExp(Exp->firstChild->nextSibling, falseLabelID, trueLabelID);
    } else if (usedThisProd(Exp, 3, "LP", "Exp", "RP")) {
        if(debug) printf("Exp := LP Exp RP\n");
        translateCondExp(Exp->firstChild->nextSibling, trueLabelID, falseLabelID);
    } else if (usedThisProd(Exp, 1, "INT")) {
        if(debug) printf("Exp := INT\n");
        Operand* op = translateExp(Exp);
        if (op->constVal == 0) {
            addCode(createGOTO(falseLabelID));
        } else {
            addCode(createGOTO(trueLabelID));
        }
    } else if (usedThisProd(Exp, 3, "Exp", "ASSIGNOP", "Exp") ||
               usedThisProd(Exp, 3, "Exp", "PLUS", "Exp") || usedThisProd(Exp, 3, "Exp", "MINUS", "Exp") || usedThisProd(Exp, 3, "Exp", "STAR", "Exp") || usedThisProd(Exp, 3, "Exp", "DIV", "Exp") || usedThisProd(Exp, 2, "MINUS", "Exp") || 
               usedThisProd(Exp, 4, "ID", "LP", "Args", "RP") || usedThisProd(Exp, 3, "ID", "LP", "RP") || 
               usedThisProd(Exp, 4, "Exp", "LB", "Exp", "RB") || usedThisProd(Exp, 3, "Exp", "DOT", "ID") || usedThisProd(Exp, 1, "ID")) {
        // other condition
        if(debug) printf("other condition\n");
        Operand* op = translateExp(Exp);
        if (op->kind == CONSTANT) {
            if (op->constVal == 0) {
                addCode(createGOTO(falseLabelID));
            } else {
                addCode(createGOTO(trueLabelID));
            }
        } else {
            CondExp* cond = createCondExp(op, NEQ, getConstant(0));
            addCode(createIFGOTO(cond, trueLabelID));
            addCode(createGOTO(falseLabelID));
        }
    } else if (usedThisProd(Exp, 1, "FLOAT")) {
        // Exp := FLOAT
        if(debug) printf("Exp := FLOAT\n");
        fprintf(stderr, "\033[31mERROR in translateCondExp! 不存在浮点数.\033[0m\n");
    } else {
        fprintf(stderr, "\033[31mERROR in translateCondExp! No matched production.\033[0m\n");
    }
}

CodesListHT translateArgs(const Node* Args) {
    if(debug) printf("translateArgs:\t");

    if (usedThisProd(Args, 1, "Exp")) {
        // Args := Exp
        if(debug) printf("Args := Exp\n");
        Type* type = analyseExp(Args->firstChild);
        if (type->kind == BASIC) {
            InterCode* code = createARG(translateExp(Args->firstChild));
            InterCodes* codes = (InterCodes*)malloc(sizeof(InterCodes));
            codes->code = code;
            codes->next = NULL;
            codes->prev = NULL;
            CodesListHT argCode;
            argCode.head = codes;
            argCode.tail = codes;
            return argCode;
        } else if (type->kind == STRUCTURE) {
            Operand* op = translateExp(Args->firstChild);
            Operand* op_ref = NULL;
            if (op->kind == DEREF) {
                op_ref = op->derefObj;
            } else {
                op_ref = createOperand(REF);
                op_ref->refObj = op;
            }
            InterCode* code = createARG(op_ref);
            InterCodes* codes = (InterCodes*)malloc(sizeof(InterCodes));
            codes->code = code;
            codes->next = NULL;
            codes->prev = NULL;
            CodesListHT argCode;
            argCode.head = codes;
            argCode.tail = codes;
            return argCode;
        } else {
            fprintf(stderr, "\033[31mERROR in translateArgs! Wrong arg type.\033[0m\n");
        }
    } else if (usedThisProd(Args, 3, "Exp", "COMMA", "Args")) {
        // Args := Exp COMMA Args
        if(debug) printf("Args := Exp COMMA Args\n");
        
        CodesListHT prevCodes = translateArgs(Args->firstChild->nextSibling->nextSibling);
        InterCode* code = createARG(translateExp(Args->firstChild));
        InterCodes* codes = (InterCodes*)malloc(sizeof(InterCodes));
        codes->code = code;
        codes->next = NULL;
        codes->prev = prevCodes.tail;
        prevCodes.tail->next = codes;
        prevCodes.tail = codes;
        return prevCodes;
    } else {
        fprintf(stderr, "\033[31mERROR in translateArgs! No matched production.\033[0m\n");
    }
}

void translateDefList(const Node* DefList) {
    if(debug) printf("translateDefList:\t");

    if (usedThisProd(DefList, 2, "Def", "DefList")) {
        // DefList := Def DefList(!=NULL)
        if(debug) printf("DefList := Def DefList(!=NULL)\n");
        translateDef(DefList->firstChild);
        translateDefList(DefList->firstChild->nextSibling);
    } else if (usedThisProd(DefList, 1, "Def")) {
        // DefList := Def DefList(==NULL)
        if(debug) printf("DefList := Def DefList(==NULL)\n");
        translateDef(DefList->firstChild);
    } else {
        fprintf(stderr, "\033[31mERROR in translateDefList! No matched production.\033[0m\n");
    }
}

void translateDef(const Node* Def) {
    if(debug) printf("translateDef:\t");
    if (usedThisProd(Def, 3, "Specifier", "DecList", "SEMI")) {
        // Def := Specifier DecList SEMI
        if(debug) printf("Def := Specifier DecList SEMI\n");
        Type* type = analyseSpecifier(Def->firstChild);
        translateDecList(Def->firstChild->nextSibling, type);
    } else {
        fprintf(stderr, "\033[31mERROR in translateDef! No matched production.\033[0m\n");
    }
}

void translateDecList(const Node* DecList, Type* type) {
    if(debug) printf("translateDecList:\t");
    if (usedThisProd(DecList, 1, "Dec")) {
        // DecList := Dec
        if(debug) printf("DecList := Dec\n");
        translateDec(DecList->firstChild, type);
    } else if (usedThisProd(DecList, 3, "Dec", "COMMA", "DecList")) {
        // DecList := Dec COMMA DecList
        if(debug) printf("DecList := Dec COMMA DecList\n");
        translateDec(DecList->firstChild, type);
        translateDecList(DecList->firstChild->nextSibling->nextSibling, type);
    } else {
        fprintf(stderr, "\033[31mERROR in translateDecList! No matched production.\033[0m\n");
    }
}

void translateDec(const Node* Dec, Type* type) {
    if(debug) printf("translateDec:\t");

    if (usedThisProd(Dec, 1, "VarDec")) {
        // Dec := VarDec
        if(debug) printf("Dec := VarDec\n");
        translateVarDec(Dec->firstChild, type);
    } else if (usedThisProd(Dec, 3, "VarDec", "ASSIGNOP", "Exp")) {
        // Dec := VarDec ASSIGNOP Exp
        if(debug) printf("Dec := VarDec ASSIGNOP Exp\n");
        Operand* var = translateVarDec(Dec->firstChild, type);
        Operand* val = translateExp(Dec->firstChild->nextSibling->nextSibling);
        if (type->kind != BASIC) {
            fprintf(stderr, "\033[31mERROR in translateDec! 赋值时出现基本类型以外的类型.\033[0m\n");
        } else {
            addCode(createASSIGN(var, val));
        }
    } else {
        fprintf(stderr, "\033[31mERROR in translateDec! No matched production.\033[0m\n");
    }
}

Operand* translateVarDec(const Node* VarDec, Type* type) {
    if(debug) printf("translateVarDec:\t");

    if(usedThisProd(VarDec, 1, "ID")) {
        // VarDec := ID
        if(debug) printf("VarDec := ID\n");

        Symbol* symbol = get(symbolTable, VarDec->firstChild->stringVal, VAR);
        if (symbol == NULL) {
            fprintf(stderr, "\033[31mERROR in translateVarDec! Cannot get ID from symbolTable.\033[0m\n");
            return NULL;
        }
        if (symbol->alias == NULL) {
            Variable* v = createVar(V);
            symbol->alias = v;
            Operand* op_v = createOperand(VARIABLE);
            op_v->var = v;
            if (type->kind == STRUCTURE) {
                addCode(createDEC(v, getSizeofType(type)));
            }
            return op_v;
        } else {
            fprintf(stderr, "\033[31mERROR in translateVarDec! 定义时符号已经有别名了.\033[0m\n");
            Operand* op = createOperand(VARIABLE);
            op->var = symbol->alias;
            return op;
        }
    } else if (usedThisProd(VarDec, 4, "VarDec", "LB", "INT", "RB")) {
        // VarDec := VarDec LB INT RB
        if(debug) printf("VarDec := VarDec LB INT RB\n");
        
        Node* array = VarDec->firstChild;
        if (usedThisProd(array, 1, "ID")) {
            Symbol* symbol = get(symbolTable, array->firstChild->stringVal, VAR);
            if (symbol == NULL) {
                fprintf(stderr, "\033[31mERROR in translateVarDec! Cannot get ID from symbolTable.\033[0m\n");
                return NULL;
            }
            if (symbol->alias == NULL) {
                Variable* v = createVar(V);
                symbol->alias = v;
                Operand* op_v = createOperand(VARIABLE);
                op_v->var = v;
                addCode(createDEC(v, array->nextSibling->nextSibling->intVal * getSizeofType(type)));
                return op_v;
            } else {
                fprintf(stderr, "\033[31mERROR in translateVarDec! 定义时符号已经有别名了.\033[0m\n");
                Operand* op = createOperand(VARIABLE);
                op->var = symbol->alias;
                addCode(createDEC(op->var, array->nextSibling->nextSibling->intVal * getSizeofType(type)));
                return op;
            }
        } else {
            fprintf(stderr, "\033[31m无法翻译：不应该出现高维数组.\033[0m\n");
            exit(0);
        }
    } else {
        fprintf(stderr, "\033[31mERROR in translateVarDec! No matched production.\033[0m\n");
    }
}