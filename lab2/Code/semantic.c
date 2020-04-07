#include "semantic.h"
#include "HashSet.h"

#define debug 1

bool equalString(char* s1, char* s2) {
    if (strcmp(s1, s2) == 0)
        return true;
    else
        return false;
}

// 是否为相同类型
bool equalType(Type* t1, Type* t2) {
    if (t1 == NULL || t2 == NULL) return false;
    if (t1->kind != t2->kind) return false;
    if (t1->kind == BASIC && t2->kind == BASIC) {
        return t1->basic == t2->basic;
    } else if (t1->kind == ARRAY && t2->kind == ARRAY) {
        // TODO
    } else if (t1->kind == STRUCTURE && t2->kind == STRUCTURE) {
        // TODO
    } else {
        fprintf(stderr, "\033[31mERROR when compare Type! Unknown Type kind.\033[0m\n");
        return false;
    }
}

// 函数调用时实参与形参对应关系检查
bool argsCheck(Param* args, Param* params, int paramNum) {
    Param* pa = args;
    Param* pp = params;
    for (int i = 0; i < paramNum; i++) {
        if (pa == NULL) return false;
        if (pp == NULL) {
            fprintf(stderr, "\033[31mERROR when checking args! paramList does not match paramNum.\033[0m\n");
            return false;
        }
        if (!equalType(pa->type, pp->type)) return false;
        pa = pa->next;
        pp = pp->next;
    }
    if (pp != NULL) {
        fprintf(stderr, "\033[31mERROR when checking args! paramList does not match paramNum.\033[0m\n");
        return false;
    }
    if (pa != NULL) return false;
    return true;
}

void printSymbolTable(HashSet table) {
    printf("\n-----------Symbol Table-----------\n");
    int count = 0;
    for (int i = 0; i < table->size; i++) {
        SymbolList *p = table->buckets[i].symbolList;
        while (p != NULL) {
            count++;
            printf("【Symbol %d】\n", count);
            Symbol* s = p->symbol;
            char *sk[] = {"VAR", "FIELD", "STRUCT", "FUNC"};
            printf("Name: %s\tSymbolKind: %s\n", s->name, sk[s->symbolKind]);
            if (s->symbolKind == FUNC) {
                printf("RetType: \n");
                printType(s->funcSign->retType);
                printf("ParamNum: %d\n", s->funcSign->paramNum);
                printf("ParamList: \n");
                Param* pl = s->funcSign->paramList;
                for (Param* pa = pl; pa != NULL; pa = pa->next) {
                    printf("ParamType: \n");
                    printType(pa->type);
                }
            } else {
                printf("Type: \n");
                printType(s->type);
            }
            p = p->next;
        }
    }
    printf("\n");
}

// 判断是否使用了指定的产生式
bool usedThisProd(const Node* father, int nodeNum, ...) {
    va_list valist;
    va_start(valist, nodeNum);
    bool usedFlag = true;
    Node* child = father->firstChild;
    for (int i = 0; i < nodeNum; i++, child = child->nextSibling) {
        if (child == NULL) {
            usedFlag = false;
            break;
        }
        char* nodeName = va_arg(valist, char*);
        if (!equalString(nodeName, child->nodeName)) {
            usedFlag = false;
            break;
        }
    }
    va_end(valist);
    if (child != NULL)
        usedFlag = false;
    return usedFlag;
}

void initGlobalVar() {
    symbolTable = initHashSet(HASH_SIZE);
    lastErrorLineno = 0;
    typeInt.kind = BASIC; typeInt.basic = INT;
    typeFloat.kind = BASIC; typeFloat.basic = FLOAT;
}

void sementicAnalysis(const Node* syntaxTreeRoot) {
    if(debug) printf("\n---------Sementic Analysis---------\n");
    initGlobalVar();
    analyseProgram(syntaxTreeRoot);
    if(debug) printSymbolTable(symbolTable);
}

void analyseProgram(const Node* Program) {
    if(debug) printf("analyseProgram:\t");
    
    if (Program == NULL) return;
    if (usedThisProd(Program, 1, "ExtDefList")) {
        // Program := ExtDefList
        if(debug) printf("Program := ExtDefList\n");
        analyseExtDefList(Program->firstChild);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseProgram! No matched production.\033[0m\n");
    }
}

void analyseExtDefList(const Node* ExtDefList) {
    if(debug) printf("analyseExtDefList:\t");

    if (usedThisProd(ExtDefList, 1, "ExtDef")) {
        // ExtDefList := ExtDef ExtDefList(== NULL)
        if(debug) printf("ExtDefList := ExtDef ExtDefList(== NULL)\n");
        analyseExtDef(ExtDefList->firstChild);
    } else if (usedThisProd(ExtDefList, 2, "ExtDef", "ExtDefList")) {
        // ExtDefList := ExtDef ExtDefList(!= NULL)
        if(debug) printf("ExtDefList := ExtDef ExtDefList(!= NULL)\n");
        analyseExtDef(ExtDefList->firstChild);
        analyseExtDefList(ExtDefList->firstChild->nextSibling);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseExtDefList! No matched production.\033[0m\n");
    }
}

void analyseExtDef(const Node* ExtDef) {
    if(debug) printf("analyseExtDef:\t");

    if (usedThisProd(ExtDef, 3, "Specifier", "ExtDecList", "SEMI")) {
        //ExtDef := Specifier ExtDecList SEMI
        if(debug) printf("ExtDef := Specifier ExtDecList SEMI\n");
        Type* type = analyseSpecifier(ExtDef->firstChild);
        if (type == NULL) {
            fprintf(stderr, "\033[31mERROR in analyseExtDef->analyseSpecifier!\033[0m\n");
            return;
        }

        // TODO

    } else if (usedThisProd(ExtDef, 2, "Specifier", "SEMI")) {
        //ExtDef := Specifier SEMI
        if(debug) printf("ExtDef := Specifier SEMI\n");
        Type* type = analyseSpecifier(ExtDef->firstChild);
        if (type == NULL) {
            fprintf(stderr, "\033[31mERROR in analyseExtDef->analyseSpecifier!\033[0m\n");
            return;
        }

        // TODO

    } else if (usedThisProd(ExtDef, 3, "Specifier", "FunDec", "CompSt")) {
        //ExtDef := Specifier FunDec CompSt
        if(debug) printf("ExtDef := Specifier FunDec CompSt\n");
        Type* type = analyseSpecifier(ExtDef->firstChild);
        if (type == NULL) {
            fprintf(stderr, "\033[31mERROR in analyseExtDef->analyseSpecifier!\033[0m\n");
            return;
        }

        Symbol* funDec = analyseFunDec(ExtDef->firstChild->nextSibling, type);
        analyseCompSt(ExtDef->firstChild->nextSibling->nextSibling, funDec);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseExtDef! No matched production.\033[0m\n");
    }
}

Type* analyseSpecifier(const Node* Specifier) {
    if(debug) printf("analyseSpecifier:\t");

    if (usedThisProd(Specifier, 1, "TYPE")) {
        //Specifier := TYPE
        if(debug) printf("Specifier := TYPE\n");
        return analyseTYPE(Specifier->firstChild);
    } else if (usedThisProd(Specifier, 1, "StructSpecifier")) {
        //Specifier := StructSpecifier
        if(debug) printf("Specifier := StructSpecifier\n");
        return analyseStructSpecifier(Specifier->firstChild);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseSpecifier! No matched production.\033[0m\n");
        return NULL;
    }
}

Type* analyseTYPE(const Node* TYPE) {
    if(debug) printf("analyseTYPE\n");

    if (equalString(TYPE->stringVal, "int")) {
        Type* newType = (Type*)malloc(sizeof(Type));
        newType->kind = BASIC;
        newType->basic = INT;
        return newType;
    } else if (equalString(TYPE->stringVal, "float")) {
        Type* newType = (Type*)malloc(sizeof(Type));
        newType->kind = BASIC;
        newType->basic = FLOAT;
        return newType;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseTYPE! Unknown TYPE!\033[0m\n");
        return NULL;
    }
}

Type* analyseStructSpecifier(const Node* StructSpecifier) {
    if(debug) printf("analyseStructSpecifier\n");

    if (usedThisProd(StructSpecifier, 2, "STRUCT", "Tag")) {
        //StructSpecifier := STRUCT Tag
        
        // TODO

    } else if (usedThisProd(StructSpecifier, 4, "STRUCT", "LC", "DefList", "RC")) {
        //StructSpecifier := STRUCT OptTag(== NULL) LC DefList RC

        // TODO

    } else if (usedThisProd(StructSpecifier, 5, "STRUCT", "OptTag", "LC", "DefList", "RC")) {
        //StructSpecifier := STRUCT OptTag(!= NULL) LC DefList RC
        
        // TODO

    } else {
        fprintf(stderr, "\033[31mERROR in analyseStructSpecifier! No matched production.\033[0m\n");
        return NULL;
    }
}

Symbol* analyseFunDec(const Node* FunDec, Type* retType) {
    if(debug) printf("analyseFunDec:\t");

    if (usedThisProd(FunDec, 3, "ID", "LP", "RP")) {
        // FunDec := ID LP RP
        if(debug) printf("FunDec := ID LP RP\n");
        Node* ID = FunDec->firstChild;
        bool insertFlag = true;
        if (contains(symbolTable, ID->stringVal, FUNC)) {
            printSemanticError(4, ID->lineNum, "Redefined function");
            insertFlag = false;
        }
        Symbol* func = createSymbol(ID->stringVal, FUNC);
        func->funcSign->retType = retType;
        func->funcSign->paramNum = 0;
        func->funcSign->paramList = NULL;

        if (insertFlag) { // 若出现重名情况则不再往符号表中插入符号
            insert(symbolTable, func);
        }
        return func; // 不管是否重名都正常分析并返回结果
    } else if (usedThisProd(FunDec, 4, "ID", "LP", "VarList", "RP")) {
        // FunDec := ID LP VarList RP
        if(debug) printf("FunDec := ID LP VarList RP\n");
        Node* ID = FunDec->firstChild;
        bool insertFlag = true;
        if (contains(symbolTable, ID->stringVal, FUNC)) {
            printSemanticError(4, ID->lineNum, "Redefined function");
            insertFlag = false;
        }
        Symbol* func = createSymbol(ID->stringVal, FUNC);
        func->funcSign->retType = retType;
        Param* params = analyseVarList(ID->nextSibling->nextSibling);
        func->funcSign->paramList = params;
        int paramNum = 0;
        for (Param* p = params; p != NULL; p = p->next) {
            paramNum++;
        }
        func->funcSign->paramNum = paramNum;
        
        if (insertFlag) { // 若出现重名情况则不再往符号表中插入符号
            insert(symbolTable, func);
        }
        return func; // 不管是否重名都正常分析并返回结果
    } else {
        fprintf(stderr, "\033[31mERROR in analyseFunDec! No matched production.\033[0m\n");
        return NULL;
    }
}

void analyseCompSt(const Node* CompSt, Symbol* func) {
    if(debug) printf("analyseCompSt:\t");

    if (usedThisProd(CompSt, 4, "LC", "DefList", "StmtList", "RC")) {
        // CompSt := LC DefList(!= NULL) StmtList(!= NULL) RC
        if(debug) printf("CompSt := LC DefList(!= NULL) StmtList(!= NULL) RC\n");
        analyseDefList(CompSt->firstChild->nextSibling, VAR);
        analyseStmtList(CompSt->firstChild->nextSibling->nextSibling, func);
    } else if (usedThisProd(CompSt, 3, "LC", "DefList", "RC")) {
        // CompSt := LC DefList(!= NULL) StmtList(== NULL) RC
        if(debug) printf("CompSt := LC DefList(!= NULL) StmtList(== NULL) RC\n");
        analyseDefList(CompSt->firstChild->nextSibling, VAR);
    } else if (usedThisProd(CompSt, 3, "LC", "StmtList", "RC")) {
        // CompSt := LC DefList(== NULL) StmtList(!= NULL) RC
        if(debug) printf("CompSt := LC DefList(== NULL) StmtList(!= NULL) RC\n");
        analyseStmtList(CompSt->firstChild->nextSibling, func);
    } else if (usedThisProd(CompSt, 2, "LC", "RC")) {
        // CompSt := LC DefList(== NULL) StmtList(== NULL) RC
        if(debug) printf("CompSt := LC DefList(== NULL) StmtList(== NULL) RC\n");
        return;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseCompSt! No matched production.\033[0m\n");
    }
}

void analyseDefList(const Node* DefList, SymbolKind kind) {
    if(debug) printf("analyseDefList:\t");

    if (usedThisProd(DefList, 2, "Def", "DefList")) {
        // DefList := Def DefList(!=NULL)
        if(debug) printf("DefList := Def DefList(!=NULL)\n");
        analyseDef(DefList->firstChild, kind);
        analyseDefList(DefList->firstChild->nextSibling, kind);
    } else if (usedThisProd(DefList, 1, "Def")) {
        // DefList := Def DefList(==NULL)
        if(debug) printf("DefList := Def DefList(==NULL)\n");
        analyseDef(DefList->firstChild, kind);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDefList! No matched production.\033[0m\n");
    }
}

void analyseDef(const Node* Def, SymbolKind kind) {
    if(debug) printf("analyseDef:\t");
    if (usedThisProd(Def, 3, "Specifier", "DecList", "SEMI")) {
        // Def := Specifier DecList SEMI
        if(debug) printf("Def := Specifier DecList SEMI\n");
        Node* Specifier = Def->firstChild;
        Type* type = analyseSpecifier(Specifier);
        SymbolList* decList = analyseDecList(Specifier->nextSibling, kind, type);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDef! No matched production.\033[0m\n");
    }
}

SymbolList* analyseDecList(const Node* DecList, SymbolKind kind, Type* type) {
    if(debug) printf("analyseDecList:\t");
    if (usedThisProd(DecList, 1, "Dec")) {
        // DecList := Dec
        if(debug) printf("DecList := Dec\n");
        Symbol* dec = analyseDec(DecList->firstChild, kind, type);
        SymbolList* decList = (SymbolList*)malloc(sizeof(SymbolList));
        decList->symbol = dec;
        decList->next = NULL;
        return decList;
    } else if (usedThisProd(DecList, 3, "Dec", "COMMA", "DecList")) {
        // DecList := Dec COMMA DecList
        if(debug) printf("DecList := Dec COMMA DecList\n");
        Symbol* dec = analyseDec(DecList->firstChild, kind, type);
        SymbolList* decList = (SymbolList*)malloc(sizeof(SymbolList));
        decList->symbol = dec;
        decList->next = analyseDecList(DecList->firstChild->nextSibling->nextSibling, kind, type);
        return decList;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDecList! No matched production.\033[0m\n");
        return NULL;
    }
}

Symbol* analyseDec(const Node* Dec, SymbolKind kind, Type* type) {
    if(debug) printf("analyseDec:\t");

    if (usedThisProd(Dec, 1, "VarDec")) {
        // Dec := VarDec
        if(debug) printf("Dec := VarDec\n");
        return analyseVarDec(Dec->firstChild, kind, type);
    } else if (usedThisProd(Dec, 3, "VarDec", "ASSIGNOP", "Exp")) {
        // Dec := VarDec ASSIGNOP Exp
        if(debug) printf("Dec := VarDec ASSIGNOP Exp\n");
        Node* VarDec = Dec->firstChild;
        Symbol* dec = analyseVarDec(VarDec, kind, type);
        Type* expType = analyseExp(VarDec->nextSibling->nextSibling);
        if (!equalType(type, expType)) {
            printSemanticError(5, VarDec->nextSibling->lineNum, "Type mismatched for assignment");
        }
        return dec;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDec! No matched production.\033[0m\n");
        return NULL;
    }
}

Symbol* analyseVarDec(const Node* VarDec, SymbolKind kind, Type* type) {
    if(debug) printf("analyseVarDec:\t");

    if(usedThisProd(VarDec, 1, "ID")) {
        // VarDec := ID
        if(debug) printf("VarDec := ID\n");
        Node* ID = VarDec->firstChild;
        Symbol* id = createSymbol(ID->stringVal, kind);
        id->type = type;
        bool insertFlag = true;
        if (contains(symbolTable, ID->stringVal, kind)) {
            insertFlag = false;
            switch (kind) {
            case VAR:
                printSemanticError(3, ID->lineNum, "Redefined variable");
                break;
            case FIELD:
                printSemanticError(15, ID->lineNum, "Redefined field");
                break;
            default:
                fprintf(stderr, "\033[31mERROR in analyseVarDec! Wrong symbol kind.\033[0m\n");
                break;
            }
        }
        if (insertFlag) {
            insert(symbolTable, id);
        }
        return id;
    } else if (usedThisProd(VarDec, 4, "VarDec", "LB", "INT", "RB")) {
        // VarDec := VarDec LB INT RB
        if(debug) printf("VarDec := VarDec LB INT RB\n");

        // TODO
    } else {
        fprintf(stderr, "\033[31mERROR in analyseVarDec! No matched production.\033[0m\n");
        return NULL;
    }
}

Type* analyseExp(const Node* Exp) {
    if(debug) printf("analyseExp:\t");

    if (usedThisProd(Exp, 3, "Exp", "ASSIGNOP", "Exp")) {
        // Exp := Exp ASSIGNOP Exp
        if(debug) printf("Exp := Exp ASSIGNOP Exp\n");
        Type* left = analyseExp(Exp->firstChild);
        Type* right = analyseExp(Exp->firstChild->nextSibling->nextSibling);
        if (!equalType(left, right)) {
            printSemanticError(5, Exp->firstChild->nextSibling->lineNum, "Type mismatched for assignment");
        }
        if (left != NULL && !left->Lvalue) {
            printSemanticError(6, Exp->firstChild->lineNum, "The left-hand side of an assignment must be a variable");
        }
        return left;
    } else if (usedThisProd(Exp, 3, "Exp", "AND", "Exp") || usedThisProd(Exp, 3, "Exp", "OR", "Exp")) {
        /* 逻辑运算
        // Exp := Exp AND Exp
        // Exp := Exp OR Exp
        */
        if(debug) printf("Exp := Exp AND/OR Exp\n");
        // TODO

    } else if (usedThisProd(Exp, 2, "NOT", "Exp")) {
        /* 逻辑运算
        // Exp := NOT Exp
        */
        if(debug) printf("Exp := NOT Exp\n");
        // TODO

    } else if (usedThisProd(Exp, 3, "Exp", "RELOP", "Exp") || usedThisProd(Exp, 3, "Exp", "PLUS", "Exp") || usedThisProd(Exp, 3, "Exp", "MINUS", "Exp") || usedThisProd(Exp, 3, "Exp", "STAR", "Exp") || usedThisProd(Exp, 3, "Exp", "DIV", "Exp")) {
        /* 算术运算
        // Exp := Exp RELOP Exp
        // Exp := Exp PLUS Exp
        // Exp := Exp MINUS Exp
        // Exp := Exp STAR Exp
        // Exp := Exp DIV Exp
        */
        if(debug) printf("Exp := Exp RELOP/PLUS/MINUS/STAR/DIV Exp\n");
        Type* left = analyseExp(Exp->firstChild);
        Type* right = analyseExp(Exp->firstChild->nextSibling->nextSibling);
        if (!equalType(left, right)) {
            printSemanticError(7, Exp->firstChild->nextSibling->lineNum, "Type mismatched for operands");
        }
        if (!(equalType(left, TYPE_INT) && equalType(right, TYPE_INT) || equalType(left, TYPE_FLOAT) && equalType(right, TYPE_FLOAT))) {
            printSemanticError(7, Exp->firstChild->nextSibling->lineNum, "Type mismatched for operands");
        }
        return left;
    } else if (usedThisProd(Exp, 2, "MINUS", "Exp")) {
        /* 算术运算
        // Exp := MINUS Exp
        */
        if(debug) printf("Exp := MINUS Exp\n");
        //TODO

    } else if (usedThisProd(Exp, 3, "LP", "Exp", "RP")) {
        // Exp := LP Exp RP
        if(debug) printf("Exp := LP Exp RP\n");
        return analyseExp(Exp->firstChild->nextSibling);
    } else if (usedThisProd(Exp, 4, "ID", "LP", "Args", "RP")) {
        // Exp := ID LP Args RP
        if(debug) printf("Exp := ID LP Args RP\n");
        Node* ID = Exp->firstChild;
        if (contains(symbolTable, ID->stringVal, FUNC)) {
            Symbol* func = get(symbolTable, ID->stringVal, FUNC);
            Param* args = analyseArgs(ID->nextSibling->nextSibling);
            if (func != NULL) {
                if (!argsCheck(args, func->funcSign->paramList, func->funcSign->paramNum)) {
                    printSemanticError(9, ID->lineNum, "Function is not applicable for arguments");
                }
                return func->type;
            } else {
                fprintf(stderr, "\033[31mERROR in analyseExp when get func symbol from table! .\033[0m\n");
                return NULL;
            }
        } else {
            printSemanticError(2, ID->lineNum, "Undefined function");
            return NULL;
        }
    } else if (usedThisProd(Exp, 3, "ID", "LP", "RP")) {
        // Exp := ID LP RP
        if(debug) printf("Exp := ID LP RP\n");
        Node* ID = Exp->firstChild;
        if (contains(symbolTable, ID->stringVal, FUNC)) {
            Symbol* func = get(symbolTable, ID->stringVal, FUNC);
            if (func != NULL) {
                if (func->funcSign->paramNum != 0) {
                    printSemanticError(9, ID->lineNum, "Function is not applicable for arguments");
                }
                return func->type;
            } else {
                fprintf(stderr, "\033[31mERROR in analyseExp when get func symbol from table! .\033[0m\n");
                return NULL;
            }
        } else {
            printSemanticError(2, ID->lineNum, "Undefined function");
            return NULL;
        }
    } else if (usedThisProd(Exp, 1, "ID")) {
        // Exp := ID
        if(debug) printf("Exp := ID\n");
        Node* ID = Exp->firstChild;
        if (contains(symbolTable, ID->stringVal, VAR)) {
            Symbol* symbol = get(symbolTable, ID->stringVal, VAR);
            if (symbol != NULL) {
                return symbol->type;
            } else {
                fprintf(stderr, "\033[31mERROR in analyseExp when get var symbol from table! .\033[0m\n");
                return NULL;
            }
        } else {
            printSemanticError(1, ID->lineNum, "Undefined variable");
            return NULL;
        }
    } else if (usedThisProd(Exp, 1, "INT")) {
        // Exp := INT
        if(debug) printf("Exp := INT\n");
        Type* type = (Type*)malloc(sizeof(Type));
        type->kind = BASIC;
        type->basic = INT;
        type->Lvalue = false;
        return type;
    } else if (usedThisProd(Exp, 1, "FLOAT")) {
        // Exp := FLOAT
        if(debug) printf("Exp := FLOAT\n");
        Type* type = (Type*)malloc(sizeof(Type));
        type->kind = BASIC;
        type->basic = FLOAT;
        type->Lvalue = false;
        return type;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseExp! No matched production.\033[0m\n");
        return NULL;
    }
}

void analyseStmtList(const Node* StmtList, Symbol* func) {
    if(debug) printf("analyseStmtList:\t");

    if (usedThisProd(StmtList, 2, "Stmt", "StmtList")) {
        // StmtList := Stmt StmtList(!= NULL)
        if(debug) printf("StmtList := Stmt StmtList(!= NULL)\n");
        analyseStmt(StmtList->firstChild, func);
        analyseStmtList(StmtList->firstChild->nextSibling, func);
    } else if (usedThisProd(StmtList, 1, "Stmt")) {
        // StmtList : Stmt StmtList(== NULL)
        if(debug) printf("StmtList := Stmt StmtList(== NULL)\n");
        analyseStmt(StmtList->firstChild, func);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseStmtList! No matched production.\033[0m\n");
    }
}

void analyseStmt(const Node* Stmt, Symbol* func) {
    if(debug) printf("analyseStmt:\t");

    if (usedThisProd(Stmt, 2, "Exp", "SEMI")) {
        // Stmt := Exp SEMI
        if(debug) printf("Stmt := Exp SEMI\n");
        Type* type = analyseExp(Stmt->firstChild);
    } else if(usedThisProd(Stmt, 3, "RETURN", "Exp", "SEMI")) {
        // Stmt := RETURN Exp SEMI
        if(debug) printf("Stmt := RETURN Exp SEMI\n");
        if (!equalType(func->funcSign->retType, analyseExp(Stmt->firstChild->nextSibling))) {
            printSemanticError(8, Stmt->firstChild->nextSibling->lineNum, "Type mismatched for return");
        }
    } else {
        fprintf(stderr, "\033[31mERROR in analyseStmt! No matched production.\033[0m\n");
    }
}

Param* analyseArgs(const Node* Args) {
    if(debug) printf("analyseArgs:\t");

    if (usedThisProd(Args, 1, "Exp")) {
        // Args := Exp
        if(debug) printf("Args := Exp\n");
        Param* arg = (Param*)malloc(sizeof(Param));
        arg->next = NULL;
        arg->type = analyseExp(Args->firstChild);
        return arg;
    } else if (usedThisProd(Args, 3, "Exp", "COMMA", "Args")) {
        // Args := Exp COMMA Args
        if(debug) printf("Args := Exp COMMA Args\n");
        Param* arg = (Param*)malloc(sizeof(Param));
        arg->type = analyseExp(Args->firstChild);
        arg->next = analyseArgs(Args->firstChild->nextSibling->nextSibling);
        return arg;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseArgs! No matched production.\033[0m\n");
        return NULL;
    }
}

Param* analyseVarList(const Node* VarList) {
    if(debug) printf("analyseVarList:\t");

    if (usedThisProd(VarList, 1, "ParamDec")) {
        // VarList := ParamDec
        if(debug) printf("VarList := ParamDec\n");
        return analyseParamDec(VarList->firstChild);
    } else if (usedThisProd(VarList, 3, "ParamDec", "COMMA", "VarList")) {
        // VarList := ParamDec COMMA VarList
        if(debug) printf("VarList := ParamDec COMMA VarList\n");
        Param* params = analyseParamDec(VarList->firstChild);
        params->next = analyseVarList(VarList->firstChild->nextSibling->nextSibling);
        return params;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseVarList! No matched production.\033[0m\n");
        return NULL;
    }
}

Param* analyseParamDec(const Node* ParamDec) {
    if(debug) printf("analyseParamDec:\t");

    if(usedThisProd(ParamDec, 2, "Specifier", "VarDec")) {
        // ParamDec := Specifier VarDec
        if(debug) printf("ParamDec := Specifier VarDec\n");
        Type* type = analyseSpecifier(ParamDec->firstChild);
        Symbol* symbol = analyseVarDec(ParamDec->firstChild->nextSibling, VAR, type);
        Param* param = (Param*)malloc(sizeof(Param));
        param->type = type;
        param->next = NULL;
        return param;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseParamDec! No matched production.\033[0m\n");
        return NULL;
    }
}

void printSemanticError(int errorType, int lineNum, char* msg) {
    if (lastErrorLineno != lineNum) {
        fprintf(stderr, "Error type %d at Line %d: %s.\n", errorType, lineNum, msg);
        lastErrorLineno = lineNum;
    }     
}