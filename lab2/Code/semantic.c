#include "semantic.h"
#include "HashSet.h"

bool equalString(char* s1, char* s2) {
    if (strcmp(s1, s2) == 0)
        return true;
    else
        return false;
}

bool equalType(Type* t1, Type* t2) {
    if (t1 == NULL || t2 == NULL) return false;
    if (t1->kind != t2->kind) return false;
    if (t1->kind == BASIC && t2->kind == BASIC) {
        return t1->basic == t2->basic;
    } else if (t1->kind == ARRAY && t2->kind == ARRAY) {

    } else if (t1->kind == STRUCTURE && t2->kind == STRUCTURE) {

    } else {
        fprintf(stderr, "\033[31mERROR when compare Type! Unknown Type kind.\033[0m\n");
        return false;
    }
}

void printType(Type* type) {
    char *k[] = {"BASIC", "ARRAY", "STRUCTURE"};
    printf("Kind: %s\n", k[type->kind]);
    if (type->kind == BASIC) {
        char *b[] = {"INT", "FLOAT"};
        printf("Basic: %s\n", b[type->basic]);
    } else if (type->kind == ARRAY) {

    } else {

    }
}
void printSymbolTable(HashSet table) {
    printf("\n-----------Symbol Table-----------\n");
    printf("table addr: %p\n", table);
    int count = 0;
    for (int i = 0; i < table->size; i++) {
        SymbolList *p = table->buckets[i].symbolList;
        while (p != NULL) {
            printf("ztw2, i=%d\n", i);
            count++;
            printf("[Symbol %d]\n", count);
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
                    printf("ParamName: %s\nParamType: \n", pa->name);
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

void sementicAnalysis(const Node* syntaxTreeRoot) {
    printf("\n---------Sementic Analysis---------\n");
    symbolTable = initHashSet(HASH_SIZE);
    lastErrorLineno = 0;
    analyseProgram(syntaxTreeRoot);
    printSymbolTable(symbolTable);
}

void analyseProgram(const Node* Program) {
    printf("analyseProgram\n");
    if (Program == NULL) return;
    analyseExtDefList(Program->firstChild);
}

void analyseExtDefList(const Node* ExtDefList) {
    printf("analyseExtDefList\n");
    if (ExtDefList == NULL) return;
    Node* ExtDef = ExtDefList->firstChild;
    analyseExtDef(ExtDef);
    if (ExtDef->nextSibling != NULL) {
        analyseExtDefList(ExtDef->nextSibling);
    }
}

void analyseExtDef(const Node* ExtDef) {
    printf("analyseExtDef\n");
    Node* Specifier = ExtDef->firstChild;
    Type* type = analyseSpecifier(Specifier);
    if (type == NULL) {
        fprintf(stderr, "\033[31mERROR in analyseExtDef->analyseSpecifier!\033[0m\n");
        return;
    }

    if (Specifier->nextSibling != NULL && equalString(Specifier->nextSibling->nodeName, "ExtDecList") && Specifier->nextSibling->nextSibling != NULL && equalString(Specifier->nextSibling->nextSibling->nodeName, "SEMI")) {
        //ExtDef := Specifier ExtDecList SEMI

    } else if (Specifier->nextSibling != NULL && equalString(Specifier->nextSibling->nodeName, "SEMI")) {
        //ExtDef := Specifier SEMI
    } else if (Specifier->nextSibling != NULL && equalString(Specifier->nextSibling->nodeName, "FunDec") && Specifier->nextSibling->nextSibling != NULL && equalString(Specifier->nextSibling->nextSibling->nodeName, "CompSt")) {
        //ExtDef := Specifier FunDec CompSt
        Symbol* funDec = analyseFunDec(Specifier->nextSibling, type);
        analyseCompSt(Specifier->nextSibling->nextSibling, funDec);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseExtDef!\033[0m\n");
    }
}

Type* analyseSpecifier(const Node* Specifier) {
    printf("analyseSpecifier\n");
    if (Specifier->firstChild != NULL && equalString(Specifier->firstChild->nodeName, "TYPE")) {
        //Specifier := TYPE
        Node* TYPE = Specifier->firstChild;
        return analyseTYPE(TYPE);
    } else if (Specifier->firstChild != NULL && equalString(Specifier->firstChild->nodeName, "StructSpecifier")) {
        //Specifier := StructSpecifier
        Node* StructSpecifier = Specifier->firstChild;
        return analyseStructSpecifier(StructSpecifier);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseSpecifier! No matched production.\033[0m\n");
        return NULL;
    }
}

Type* analyseTYPE(const Node* TYPE) {
    printf("analyseTYPE\n");
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
        fprintf(stderr, "\033[31mERROR in analyseExtDef! Specifier := TYPE, unknown TYPE!\033[0m\n");
        return NULL;
    }
}

Type* analyseStructSpecifier(const Node* StructSpecifier) {
    Node* Struct = StructSpecifier->firstChild;
    if (Struct == NULL) {
        fprintf(stderr, "\033[31mERROR in analyseStructSpecifier! Missing Node STRUCT.\033[0m\n");
        return NULL;
    }

    if (Struct->nextSibling != NULL && equalString(Struct->nextSibling->nodeName, "Tag")) {
        //StructSpecifier := STRUCT Tag
        Node* ID = Struct->nextSibling->firstChild;
        if (contains(symbolTable, ID->stringVal, STRUCT)) {

        } else {

        }
    } else if (Struct->nextSibling != NULL && equalString(Struct->nextSibling->nodeName, "LC") && \
               Struct->nextSibling->nextSibling != NULL && equalString(Struct->nextSibling->nextSibling->nodeName, "DefList") && \
               Struct->nextSibling->nextSibling->nextSibling != NULL && equalString(Struct->nextSibling->nextSibling->nextSibling->nodeName, "RC")) {
        //StructSpecifier := STRUCT OptTag(== NULL) LC DefList RC

    } else if (Struct->nextSibling != NULL && equalString(Struct->nextSibling->nodeName, "OptTag") && \
               Struct->nextSibling->nextSibling != NULL && equalString(Struct->nextSibling->nextSibling->nodeName, "LC") && \
               Struct->nextSibling->nextSibling->nextSibling != NULL && equalString(Struct->nextSibling->nextSibling->nextSibling->nodeName, "DefList") && \
               Struct->nextSibling->nextSibling->nextSibling->nextSibling != NULL && equalString(Struct->nextSibling->nextSibling->nextSibling->nextSibling->nodeName, "RC")) {
        //StructSpecifier := STRUCT OptTag(!= NULL) LC DefList RC
        
    } else {
        fprintf(stderr, "\033[31mERROR in analyseStructSpecifier! No matched production.\033[0m\n");
        return NULL;
    }
}

Symbol* analyseFunDec(const Node* FunDec, Type* retType) {
    printf("analyseFunDec\n");
    Node* ID = FunDec->firstChild;
    bool insertFlag = true;
    if (contains(symbolTable, ID->stringVal, FUNC)) {
        printSemanticError(4, ID->lineNum, "Redefined function");
        insertFlag = false;
    }
    Symbol* func = createSymbol(ID->stringVal, FUNC);
    if (equalString(ID->nextSibling->nodeName, "LP") && equalString(ID->nextSibling->nextSibling->nodeName, "RP")) {
        // FunDec := ID LP RP
        func->funcSign->retType = retType;
        func->funcSign->paramNum = 0;
        func->funcSign->paramList = NULL;
    } else if (equalString(ID->nextSibling->nodeName, "LP") && equalString(ID->nextSibling->nextSibling->nodeName, "VarList") && equalString(ID->nextSibling->nextSibling->nextSibling->nodeName, "RP")) {
        // FunDec := ID LP VarList RP
    } else {
        fprintf(stderr, "\033[31mERROR in analyseFunDec! No matched production.\033[0m\n");
        return NULL;
    }
    if (insertFlag) { // 若出现重名情况则不再往符号表中插入符号
        insert(symbolTable, func);
    }
    return func; // 不管是否重名都正常分析并返回结果
}

void analyseCompSt(const Node* CompSt, Symbol* func) {
    printf("analyseCompSt\n");
    if (usedThisProd(CompSt, 4, "LC", "DefList", "StmtList", "RC")) {
        // CompSt := LC DefList(!= NULL) StmtList(!= NULL) RC
        analyseDefList(CompSt->firstChild->nextSibling, VAR);
        analyseStmtList(CompSt->firstChild->nextSibling->nextSibling);
    } else if (usedThisProd(CompSt, 3, "LC", "DefList", "RC")) {
        // CompSt := LC DefList(!= NULL) StmtList(== NULL) RC
    } else if (usedThisProd(CompSt, 3, "LC", "StmtList", "RC")) {
        // CompSt := LC DefList(== NULL) StmtList(!= NULL) RC
    } else if (usedThisProd(CompSt, 2, "LC", "RC")) {
        // CompSt := LC DefList(== NULL) StmtList(== NULL) RC
        return;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseCompSt! No matched production.\033[0m\n");
    }
}

void analyseDefList(const Node* DefList, SymbolKind kind) {
    printf("analyseDefList\n");
    if (usedThisProd(DefList, 2, "Def", "DefList")) {
        // DefList := Def DefList(!=NULL)
    } else if (usedThisProd(DefList, 1, "Def")) {
        // DefList := Def DefList(==NULL)
        analyseDef(DefList->firstChild, kind);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDefList! No matched production.\033[0m\n");
    }
}

void analyseDef(const Node* Def, SymbolKind kind) {
    printf("analyseDef\n");
    if (usedThisProd(Def, 3, "Specifier", "DecList", "SEMI")) {
        // Def := Specifier DecList SEMI
        Node* Specifier = Def->firstChild;
        Type* type = analyseSpecifier(Specifier);
        SymbolList* decList = analyseDecList(Specifier->nextSibling, kind, type);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDef! No matched production.\033[0m\n");
    }
}

SymbolList* analyseDecList(const Node* DecList, SymbolKind kind, Type* type) {
    printf("analyseDecList\n");
    if (usedThisProd(DecList, 1, "Dec")) {
        // DecList := Dec
        Symbol* dec = analyseDec(DecList->firstChild, kind, type);
        SymbolList* decList = (SymbolList*)malloc(sizeof(SymbolList));
        decList->symbol = dec;
        decList->next = NULL;
        return decList;
    } else if (usedThisProd(DecList, 3, "Dec", "COMMA", "DecList")) {
        // DecList := Dec COMMA DecList
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDecList! No matched production.\033[0m\n");
        return NULL;
    }
}

Symbol* analyseDec(const Node* Dec, SymbolKind kind, Type* type) {
    printf("analyseDec\n");
    if (usedThisProd(Dec, 1, "VarDec")) {
        // Dec := VarDec
    } else if (usedThisProd(Dec, 3, "VarDec", "ASSIGNOP", "Exp")) {
        // Dec := VarDec ASSIGNOP Exp
        Node* VarDec = Dec->firstChild;
        Symbol* varDec = analyseVarDec(VarDec, kind, type);
        Type* expType = analyseExp(VarDec->nextSibling->nextSibling);
        if (!equalType(type, expType)) {
            printSemanticError(5, VarDec->nextSibling->lineNum, "Type mismatched for assignment");
        }
        return varDec;
    }
}

Symbol* analyseVarDec(const Node* VarDec, SymbolKind kind, Type* type) {
    printf("analyseVarDec\n");
    if(usedThisProd(VarDec, 1, "ID")) {
        // VarDec := ID
        Node* ID = VarDec->firstChild;
        Symbol* id = createSymbol(ID->stringVal, kind);
        printType(type);
        id->type = type;
        bool insertFlag = true;
        if (contains(symbolTable, ID->stringVal, kind)) {
            insertFlag = false;
            switch (kind) {
            case VAR:
                printSemanticError(3, ID->lineNum, "Redefined variable");
                break;
            case FIELD:
                printSemanticError(3, ID->lineNum, "Redefined field");
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
    }
}

Type* analyseExp(const Node* Exp) {
    printf("analyseExp\n");
    if (usedThisProd(Exp, 3, "Exp", "ASSIGNOP", "Exp")) {
        // Exp := Exp ASSIGNOP Exp
        Type* left = analyseExp(Exp->firstChild);
        Type* right = analyseExp(Exp->firstChild->nextSibling->nextSibling);
        if (!equalType(left, right)) {
            printSemanticError(5, Exp->firstChild->nextSibling->lineNum, "Type mismatched for assignment");
        }
        if (left != NULL && !left->Lvalue) {
            printSemanticError(6, Exp->firstChild->lineNum, "The left-hand side of an assignment must be a variable");
        }
        return left;
    } else if (usedThisProd(Exp, 1, "ID")) {
        // Exp := ID
        Node* ID = Exp->firstChild;
        if (contains(symbolTable, ID->stringVal, VAR)) {
            Symbol* symbol = get(symbolTable, ID->stringVal, VAR);
            if (symbol != NULL) {
                return symbol->type;
            } else {
                fprintf(stderr, "\033[31mERROR in analyseExp when get symbol from table! .\033[0m\n");
            }
        } else {
            printSemanticError(1, ID->lineNum, "Undefined variable");
            return NULL;
        }
    } else if (usedThisProd(Exp, 1, "INT")) {
        // Exp := INT
        Type* type = (Type*)malloc(sizeof(Type));
        type->kind = BASIC;
        type->basic = INT;
        type->Lvalue = false;
        return type;
    }
}

void analyseStmtList(const Node* StmtList) {
    printf("analyseStmtList\n");
    if (usedThisProd(StmtList, 2, "Stmt", "StmtList")) {
        // StmtList := Stmt StmtList(!= NULL)
    } else if (usedThisProd(StmtList, 1, "Stmt")) {
        // StmtList : Stmt StmtList(== NULL)
        analyseStmt(StmtList->firstChild);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseStmtList! No matched production.\033[0m\n");
    }
}

void analyseStmt(const Node* Stmt) {
    if (usedThisProd(Stmt, 2, "Exp", "SEMI")) {
        // Stmt := Exp SEMI
        Type* type = analyseExp(Stmt->firstChild);
    }
}


void printSemanticError(int errorType, int lineNum, char* msg) {
    if (lastErrorLineno != lineNum) {
        fprintf(stderr, "Error type %d at Line %d: %s.\n", errorType, lineNum, msg);
        lastErrorLineno = lineNum;
    }     
}