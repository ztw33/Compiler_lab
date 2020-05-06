#include "semantic.h"
#include "HashSet.h"

#define debug 0

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
        return equalType(t1->array.elem, t2->array.elem);
    } else if (t1->kind == STRUCTURE && t2->kind == STRUCTURE) {
        Field* f1 = t1->structure.fields;
        Field* f2 = t2->structure.fields;
        while (f1 != NULL && f2 != NULL) {
            if (!equalType(f1->type, f2->type)) {
                return false;
            }
            f1 = f1->next;
            f2 = f2->next;
        }
        if (f1 != NULL || f2 != NULL)
            return false;
        else
            return true;
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

Field* concatField(SymbolList* symbols, int offset) {
    if (symbols == NULL) return NULL;
    SymbolList* head = symbols;
    if (head->symbol == NULL) {
        if (head->next == NULL)
            return NULL;
        else
            return concatField(head->next, offset);       
    }
    Field* field = (Field*)malloc(sizeof(Field));
    field->name = head->symbol->name;
    field->type = head->symbol->type;
    field->offset = offset;
    field->next = concatField(head->next, offset + getSizeofType(field->type));
    return field;
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

void semanticInit() {
    symbolTable = initHashSet(HASH_SIZE);
    typeInt.kind = BASIC; typeInt.basic = INT; typeInt.Rvalue = false;
    typeFloat.kind = BASIC; typeFloat.basic = FLOAT; typeInt.Rvalue = false;
    unnamedCount = 0;

    // 添加read和write
    Symbol* read = createSymbol("read", FUNC);
    read->funcSign->retType = TYPE_INT;
    read->funcSign->paramNum = 0;
    read->funcSign->paramList = NULL;
    insert(symbolTable, read);
    Symbol* write = createSymbol("write", FUNC);
    write->funcSign->retType = TYPE_INT;
    write->funcSign->paramNum = 1;
    Param* param = (Param*)malloc(sizeof(Param));
    param->type = TYPE_INT;
    param->next = NULL;
    write->funcSign->paramList = param;
    insert(symbolTable, write);
}

void sementicAnalysis(const Node* syntaxTreeRoot) {
    if(debug) 
        printf("\n---------Sementic Analysis---------\n");
    semanticInit();
    analyseProgram(syntaxTreeRoot);
    if(debug) 
        printSymbolTable(symbolTable);
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
    if (ExtDefList == NULL) return;

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
        analyseExtDecList(ExtDef->firstChild->nextSibling, type);

    } else if (usedThisProd(ExtDef, 2, "Specifier", "SEMI")) {
        //ExtDef := Specifier SEMI
        if(debug) printf("ExtDef := Specifier SEMI\n");
        Type* type = analyseSpecifier(ExtDef->firstChild);

    } else if (usedThisProd(ExtDef, 3, "Specifier", "FunDec", "CompSt")) {
        //ExtDef := Specifier FunDec CompSt
        if(debug) printf("ExtDef := Specifier FunDec CompSt\n");
        Type* type = analyseSpecifier(ExtDef->firstChild);
        
        /* 写错了的地方 */
        // if (type != NULL) {
        //     type->Rvalue = true; // 函数返回值都是右值
        // }
        /* 修改 */
        // 函数返回值都是右值, 但是要新建一个结构体啊！
        Type* newType = (Type*)malloc(sizeof(Type));
        newType->kind = type->kind;
        if (newType->kind == BASIC) {
            newType->basic = type->basic;
        } else if (newType->kind == ARRAY) {
            newType->array = type->array;
        } else {
            newType->structure = type->structure;
        }
        newType->Rvalue = true;

        Symbol* funDec = analyseFunDec(ExtDef->firstChild->nextSibling, newType);
        analyseCompSt(ExtDef->firstChild->nextSibling->nextSibling, funDec);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseExtDef! No matched production.\033[0m\n");
    }
}

void analyseExtDecList(const Node* ExtDecList, Type* type) {
    if(debug) printf("analyseExtDecList:\t");

    if (usedThisProd(ExtDecList, 1, "VarDec")) {
        // ExtDecList := VarDec
        if(debug) printf("ExtDecList := VarDec\n");
        Symbol* var = analyseVarDec(ExtDecList->firstChild, VAR, type);
    } else if (usedThisProd(ExtDecList, 3, "VarDec", "COMMA", "ExtDecList")) {
        // ExtDecList := VarDec COMMA ExtDecList
        if(debug) printf("ExtDecList := VarDec COMMA ExtDecList\n");
        Symbol* var = analyseVarDec(ExtDecList->firstChild, VAR, type);
        analyseExtDecList(ExtDecList->firstChild->nextSibling->nextSibling, type);
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
        newType->Rvalue = false;
        return newType;
    } else if (equalString(TYPE->stringVal, "float")) {
        Type* newType = (Type*)malloc(sizeof(Type));
        newType->kind = BASIC;
        newType->basic = FLOAT;
        newType->Rvalue = false;
        return newType;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseTYPE! Unknown TYPE!\033[0m\n");
        return NULL;
    }
}

Type* analyseStructSpecifier(const Node* StructSpecifier) {
    if(debug) printf("analyseStructSpecifier:\t");

    if (usedThisProd(StructSpecifier, 2, "STRUCT", "Tag")) {
        //StructSpecifier := STRUCT Tag
        if(debug) printf("StructSpecifier := STRUCT Tag\nTag := ID\n");
        
        Node* ID = StructSpecifier->firstChild->nextSibling->firstChild;
        if (contains(symbolTable, ID->stringVal, STRUCT)) {
            Symbol* struct_ = get(symbolTable, ID->stringVal, STRUCT);
            if (struct_ == NULL) {
                printSemanticError(17, ID->lineNum, "Undefined structure");
                return NULL;
            } else {
                return struct_->type;
            }
        } else {
            printSemanticError(17, ID->lineNum, "Undefined structure");
            return NULL;
        }
    } else if (usedThisProd(StructSpecifier, 4, "STRUCT", "LC", "DefList", "RC")) {
        //StructSpecifier := STRUCT OptTag(== NULL) LC DefList(!=NULL) RC
        if(debug) printf("StructSpecifier := STRUCT OptTag(== NULL) LC DefList(!=NULL) RC\n");

        unnamedCount++;
        char name[32]; // 为每个匿名结构体生成一个独一无二的名字: {id}_unnamed
        sprintf(name, "%d_unnamed", unnamedCount);
        char* structName = (char*)malloc(32*sizeof(char));
        strcpy(structName, name);
        
        Type* newStruct = (Type*)malloc(sizeof(Type));
        newStruct->kind = STRUCTURE;
        newStruct->Rvalue = false;
        newStruct->structure.name = structName;
        newStruct->structure.fields = analyseDefList(StructSpecifier->firstChild->nextSibling->nextSibling, FIELD, 0);
        
        Symbol* symbol = createSymbol(structName, STRUCT);
        symbol->type = newStruct;
        insert(symbolTable, symbol);
        return newStruct;
    } else if (usedThisProd(StructSpecifier, 5, "STRUCT", "OptTag", "LC", "DefList", "RC")) {
        //StructSpecifier := STRUCT OptTag(!= NULL) LC DefList(!=NULL) RC
        if(debug) printf("StructSpecifier := STRUCT OptTag(!= NULL) LC DefList(!=NULL) RC\nOptTag := ID\n");
        
        Node* ID = StructSpecifier->firstChild->nextSibling->firstChild;
        bool insertFlag = true;
        if (contains(symbolTable, ID->stringVal, STRUCT)) {
            printSemanticError(16, ID->lineNum, "Duplicated name");
            insertFlag = false;
        }
        Type* newStruct = (Type*)malloc(sizeof(Type));
        newStruct->kind = STRUCTURE;
        newStruct->Rvalue = false;
        newStruct->structure.name = ID->stringVal;
        newStruct->structure.fields = analyseDefList(StructSpecifier->firstChild->nextSibling->nextSibling->nextSibling, FIELD, 0);
        if (insertFlag) {
            Symbol* symbol = createSymbol(ID->stringVal, STRUCT);
            symbol->type = newStruct;
            insert(symbolTable, symbol);
        }
        return newStruct;
    } else if (usedThisProd(StructSpecifier, 3, "STRUCT", "LC", "RC")) {
        //StructSpecifier := STRUCT OptTag(== NULL) LC DefList(==NULL) RC
        if(debug) printf("StructSpecifier := STRUCT OptTag(== NULL) LC DefList(==NULL) RC\n");

        unnamedCount++;
        char name[32]; // 为每个匿名结构体生成一个独一无二的名字: {id}_unnamed
        sprintf(name, "%d_unnamed", unnamedCount);
        char* structName = (char*)malloc(32*sizeof(char));
        strcpy(structName, name);

        Type* newStruct = (Type*)malloc(sizeof(Type));
        newStruct->kind = STRUCTURE;
        newStruct->Rvalue = false;
        newStruct->structure.name = structName;
        newStruct->structure.fields = NULL;
        
        Symbol* symbol = createSymbol(structName, STRUCT);
        symbol->type = newStruct;
        insert(symbolTable, symbol);
        return newStruct;
    } else if(usedThisProd(StructSpecifier, 4, "STRUCT", "OptTag", "LC", "RC")) {
        //StructSpecifier := STRUCT OptTag(!= NULL) LC DefList(==NULL) RC
        if(debug) printf("StructSpecifier := STRUCT OptTag(!= NULL) LC DefList(==NULL) RC\n");

        Node* ID = StructSpecifier->firstChild->nextSibling->firstChild;
        bool insertFlag = true;
        if (contains(symbolTable, ID->stringVal, STRUCT)) {
            printSemanticError(16, ID->lineNum, "Duplicated name");
            insertFlag = false;
        }
        Type* newStruct = (Type*)malloc(sizeof(Type));
        newStruct->kind = STRUCTURE;
        newStruct->Rvalue = false;
        newStruct->structure.name = ID->stringVal;
        newStruct->structure.fields = NULL;
        if (insertFlag) {
            Symbol* symbol = createSymbol(ID->stringVal, STRUCT);
            symbol->type = newStruct;
            insert(symbolTable, symbol);
        }
        return newStruct;
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
        analyseDefList(CompSt->firstChild->nextSibling, VAR, -1);
        analyseStmtList(CompSt->firstChild->nextSibling->nextSibling, func);
    } else if (usedThisProd(CompSt, 3, "LC", "DefList", "RC")) {
        // CompSt := LC DefList(!= NULL) StmtList(== NULL) RC
        if(debug) printf("CompSt := LC DefList(!= NULL) StmtList(== NULL) RC\n");
        analyseDefList(CompSt->firstChild->nextSibling, VAR, -1);
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

/*
 * if kind == VAR, return NULL; if kind == FIELD, return fieldlist.
 * offset: Lab3 for kind == FIELD, if kind == VAR, offset = -1.
 */
Field* analyseDefList(const Node* DefList, SymbolKind kind, int offset) {
    if(debug) printf("analyseDefList:\t");

    if (usedThisProd(DefList, 2, "Def", "DefList")) {
        // DefList := Def DefList(!=NULL)
        if(debug) printf("DefList := Def DefList(!=NULL)\n");
        if (kind == VAR) {
            analyseDef(DefList->firstChild, kind, -1);
            analyseDefList(DefList->firstChild->nextSibling, kind, -1);
            return NULL;
        } else if (kind == FIELD) {
            Field* field = analyseDef(DefList->firstChild, kind, offset);
            if (field == NULL) {
                return analyseDefList(DefList->firstChild->nextSibling, kind, offset);
            } else {
                Field* tail = field;
                for (; tail->next != NULL; tail = tail->next);
                tail->next = analyseDefList(DefList->firstChild->nextSibling, kind, tail->offset + getSizeofType(tail->type));
                return field;
            }
        } else {
            fprintf(stderr, "\033[31mERROR in analyseDefList! Wrong symbol kind.\033[0m\n");
            return NULL;
        }
        
    } else if (usedThisProd(DefList, 1, "Def")) {
        // DefList := Def DefList(==NULL)
        if(debug) printf("DefList := Def DefList(==NULL)\n");
        if (kind == VAR) {
            analyseDef(DefList->firstChild, kind, -1);
            return NULL;
        } else if (kind == FIELD) {
            return analyseDef(DefList->firstChild, kind, offset);
        } else {
            fprintf(stderr, "\033[31mERROR in analyseDefList! Wrong symbol kind.\033[0m\n");
            return NULL;
        }
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDefList! No matched production.\033[0m\n");
        return NULL;
    }
}

/*
 * if kind == VAR, return NULL; if kind == FIELD, return field.
 */
Field* analyseDef(const Node* Def, SymbolKind kind, int offset) {
    if(debug) printf("analyseDef:\t");
    if (usedThisProd(Def, 3, "Specifier", "DecList", "SEMI")) {
        // Def := Specifier DecList SEMI
        if(debug) printf("Def := Specifier DecList SEMI\n");
        Node* Specifier = Def->firstChild;
        Type* type = analyseSpecifier(Specifier);
        SymbolList* decList = analyseDecList(Specifier->nextSibling, kind, type);

        if (kind == VAR) {
            return NULL;
        } else if (kind == FIELD) {
            return concatField(decList, offset);
        } else {
            fprintf(stderr, "\033[31mERROR in analyseDef! Wrong symbol kind.\033[0m\n");
            return NULL;
        }
    } else {
        fprintf(stderr, "\033[31mERROR in analyseDef! No matched production.\033[0m\n");
        return NULL;
    }
}

SymbolList* analyseDecList(const Node* DecList, SymbolKind kind, Type* type) {
    if(debug) printf("analyseDecList:\t");
    if (usedThisProd(DecList, 1, "Dec")) {
        // DecList := Dec
        if(debug) printf("DecList := Dec\n");

        Symbol* dec = analyseDec(DecList->firstChild, kind, type);
        if (dec == NULL) return NULL;
        SymbolList* decList = (SymbolList*)malloc(sizeof(SymbolList));
        decList->symbol = dec;
        decList->next = NULL;
        return decList;
    } else if (usedThisProd(DecList, 3, "Dec", "COMMA", "DecList")) {
        // DecList := Dec COMMA DecList
        if(debug) printf("DecList := Dec COMMA DecList\n");

        Symbol* dec = analyseDec(DecList->firstChild, kind, type);
        if (dec == NULL) {
            return analyseDecList(DecList->firstChild->nextSibling->nextSibling, kind, type);
        } else {
            SymbolList* decList = (SymbolList*)malloc(sizeof(SymbolList));
            decList->symbol = dec;
            decList->next = analyseDecList(DecList->firstChild->nextSibling->nextSibling, kind, type);
            return decList;
        }
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
        if (kind == VAR) {
            Type* expType = analyseExp(VarDec->nextSibling->nextSibling);
            if (!equalType(type, expType)) {
                printSemanticError(5, VarDec->nextSibling->lineNum, "Type mismatched for assignment");
            }
            return dec;
        } else if (kind == FIELD) {
            printSemanticError(15, VarDec->nextSibling->lineNum, "Initialize field at definition time");
            return dec;
        } else {
            fprintf(stderr, "\033[31mERROR in analyseDec! Wrong symbol kind.\033[0m\n");
            return NULL;
        }
        
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
                return NULL;
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
        Symbol* elem = analyseVarDec(VarDec->firstChild, kind, type);
        if (elem != NULL) {
            Type* arrayType = (Type*)malloc(sizeof(Type));
            arrayType->kind = ARRAY;
            arrayType->Rvalue = false;
            arrayType->array.elem = elem->type;
            arrayType->array.size = VarDec->firstChild->nextSibling->nextSibling->intVal;
            elem->type = arrayType;
        }
        return elem;
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
        
        if (left != NULL && left->Rvalue) {
            printSemanticError(6, Exp->firstChild->lineNum, "The left-hand side of an assignment must be a variable");
            if (!equalType(left, right)) {
                printSemanticError(5, Exp->firstChild->nextSibling->lineNum, "Type mismatched for assignment");
            }
        }
        if (!equalType(left, right)) {
            printSemanticError(5, Exp->firstChild->nextSibling->lineNum, "Type mismatched for assignment");
        }
    
        if (left != NULL)
            return left;
        else
            return right;
    } else if (usedThisProd(Exp, 3, "Exp", "AND", "Exp") || usedThisProd(Exp, 3, "Exp", "OR", "Exp") || usedThisProd(Exp, 3, "Exp", "RELOP", "Exp")) {
        /* 逻辑运算
        // Exp := Exp AND Exp
        // Exp := Exp OR Exp
        // Exp := Exp RELOP Exp
        */
        if(debug) printf("Exp := Exp AND/OR/RELOP Exp\n");

        Type* left = analyseExp(Exp->firstChild);
        Type* right = analyseExp(Exp->firstChild->nextSibling->nextSibling);
        if (!(equalType(left, TYPE_INT) && equalType(right, TYPE_INT))) {
            printSemanticError(7, Exp->firstChild->nextSibling->lineNum, "Type mismatched for operands");
        }
        Type* intType = (Type*)malloc(sizeof(Type));
        intType->kind = BASIC;
        intType->basic = INT;
        intType->Rvalue = true;
        return intType; // 逻辑运算返回新构建的int类型
    } else if (usedThisProd(Exp, 2, "NOT", "Exp")) {
        /* 逻辑运算
        // Exp := NOT Exp
        */
        if(debug) printf("Exp := NOT Exp\n");

        Type* type = analyseExp(Exp->firstChild->nextSibling);
        if (!equalType(type, TYPE_INT)) {
            printSemanticError(7, Exp->firstChild->lineNum, "Type mismatched for operands");
        }
        Type* intType = (Type*)malloc(sizeof(Type));
        intType->kind = BASIC;
        intType->basic = INT;
        intType->Rvalue = true;
        return intType; // 逻辑运算返回新构建的int类型
    } else if (usedThisProd(Exp, 3, "Exp", "PLUS", "Exp") || usedThisProd(Exp, 3, "Exp", "MINUS", "Exp") || usedThisProd(Exp, 3, "Exp", "STAR", "Exp") || usedThisProd(Exp, 3, "Exp", "DIV", "Exp")) {
        /* 算术运算
        // Exp := Exp PLUS Exp
        // Exp := Exp MINUS Exp
        // Exp := Exp STAR Exp
        // Exp := Exp DIV Exp
        */
        if(debug) printf("Exp := Exp PLUS/MINUS/STAR/DIV Exp\n");
        Type* left = analyseExp(Exp->firstChild);
        Type* right = analyseExp(Exp->firstChild->nextSibling->nextSibling);
        if (!(equalType(left, TYPE_INT) && equalType(right, TYPE_INT) || equalType(left, TYPE_FLOAT) && equalType(right, TYPE_FLOAT))) {
            printSemanticError(7, Exp->firstChild->nextSibling->lineNum, "Type mismatched for operands");
        }
        if (left != NULL)
            return left;
        else
            return right;
    } else if (usedThisProd(Exp, 2, "MINUS", "Exp")) {
        /* 算术运算
        // Exp := MINUS Exp
        */
        if(debug) printf("Exp := MINUS Exp\n");

        Type* type = analyseExp(Exp->firstChild->nextSibling);
        if (!(equalType(type, TYPE_INT) || equalType(type, TYPE_FLOAT))) {
            printSemanticError(7, Exp->firstChild->lineNum, "Type mismatched for the operand");
        }
        return type;
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
                return func->funcSign->retType;
            } else {
                fprintf(stderr, "\033[31mERROR in analyseExp when get func symbol from table! .\033[0m\n");
                return NULL;
            }
        } else if (contains(symbolTable, ID->stringVal, VAR)) {
            printSemanticError(11, ID->lineNum, "This variable is not a function");
            return NULL;
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
                return func->funcSign->retType;
            } else {
                fprintf(stderr, "\033[31mERROR in analyseExp when get func symbol from table! .\033[0m\n");
                return NULL;
            }
        } else if (contains(symbolTable, ID->stringVal, VAR)) {
            printSemanticError(11, ID->lineNum, "This variable is not a function");
            return NULL;
        }  else {
            printSemanticError(2, ID->lineNum, "Undefined function");
            return NULL;
        }
    } else if (usedThisProd(Exp, 4, "Exp", "LB", "Exp", "RB")) {
        // Exp := Exp LB Exp RB
        if(debug) printf("Exp := Exp LB Exp RB\n");
        Type* varType = analyseExp(Exp->firstChild);
        if (varType == NULL) return NULL;
        Type* indexType = analyseExp(Exp->firstChild->nextSibling->nextSibling);
        if (varType->kind != ARRAY) {
            printSemanticError(10, Exp->firstChild->lineNum, "This variable is not an array");
            if (!equalType(indexType, TYPE_INT)) {
                printSemanticError(12, Exp->firstChild->nextSibling->nextSibling->lineNum, "Expression between \"[]\" is not an integer");
            }
            return NULL;
        }
        if (!equalType(indexType, TYPE_INT)) {
            printSemanticError(12, Exp->firstChild->nextSibling->nextSibling->lineNum, "Expression between \"[]\" is not an integer");
        }
        return varType->array.elem;
    } else if (usedThisProd(Exp, 3, "Exp", "DOT", "ID")) {
        // Exp := Exp DOT ID
        if(debug) printf("Exp := Exp DOT ID\n");
        Type* varType = analyseExp(Exp->firstChild);
        if (varType == NULL) return NULL;
        if (varType->kind != STRUCTURE) {
            printSemanticError(13, Exp->firstChild->lineNum, "Illegal use of \".\"");
            return NULL;
        }
        Symbol* struct_ = get(symbolTable, varType->structure.name, STRUCT);
        if (struct_ == NULL) {
            fprintf(stderr, "\033[31mERROR in analyseExp when get struct symbol from table! .\033[0m\n");
            return NULL;
        }
        Node* ID = Exp->firstChild->nextSibling->nextSibling;
        Field* fields = varType->structure.fields;
        for (Field* f = fields; f != NULL; f = f->next) {
            if (equalString(f->name, ID->stringVal)) {
                return f->type;
            }
        }
        printSemanticError(14, ID->lineNum, "Non-existent field");
        return NULL;
    } else if (usedThisProd(Exp, 1, "ID")) {
        // Exp := ID
        if(debug) printf("Exp := ID\n");
        Node* ID = Exp->firstChild;
        if (contains(symbolTable, ID->stringVal, VAR)) {
            Symbol* symbol = get(symbolTable, ID->stringVal, VAR);
            if (symbol != NULL) {
                return symbol->type;
            } else { // 由于contains函数实现的原因，可能有重名但类型不是VAR的元素也被认为被包含了
                Symbol* wrongSymbol = get(symbolTable, ID->stringVal, FIELD);
                if (wrongSymbol == NULL) {
                    wrongSymbol = get(symbolTable, ID->stringVal, STRUCT);
                }
                if (wrongSymbol == NULL) {
                    fprintf(stderr, "\033[31mERROR in analyseExp when get var symbol from table! ID_name:%s\033[0m\n", ID->stringVal);
                }
                return wrongSymbol->type;
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
        type->Rvalue = true;
        return type;
    } else if (usedThisProd(Exp, 1, "FLOAT")) {
        // Exp := FLOAT
        if(debug) printf("Exp := FLOAT\n");
        Type* type = (Type*)malloc(sizeof(Type));
        type->kind = BASIC;
        type->basic = FLOAT;
        type->Rvalue = true;
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
    } else if (usedThisProd(Stmt, 5, "IF", "LP", "Exp", "RP", "Stmt")) {
        // Stmt := IF LP Exp RP Stmt
        if(debug) printf("Stmt := IF LP Exp RP Stmt\n");

        Node* exp = Stmt->firstChild->nextSibling->nextSibling;
        Type* cond = analyseExp(exp);
        if (!equalType(cond, TYPE_INT)) {
            printSemanticError(7, exp->lineNum, "Type mismatched for \"if\"(int only)");
        }
        analyseStmt(exp->nextSibling->nextSibling, func);
    } else if (usedThisProd(Stmt, 7, "IF", "LP", "Exp", "RP", "Stmt", "ELSE", "Stmt")) {
        // Stmt := IF LP Exp RP Stmt ELSE Stmt
        if(debug) printf("Stmt := IF LP Exp RP Stmt ELSE Stmt\n");

        Node* exp = Stmt->firstChild->nextSibling->nextSibling;
        Type* cond = analyseExp(exp);
        if (!equalType(cond, TYPE_INT)) {
            printSemanticError(7, exp->lineNum, "Type mismatched for \"if\"(int only)");
        }
        analyseStmt(exp->nextSibling->nextSibling, func);
        analyseStmt(exp->nextSibling->nextSibling->nextSibling->nextSibling, func);
    } else if (usedThisProd(Stmt, 1, "CompSt")) {
        // Stmt := CompSt
        if(debug) printf("Stmt := CompSt\n");
        analyseCompSt(Stmt->firstChild, func);
    } else if (usedThisProd(Stmt, 5, "WHILE", "LP", "Exp", "RP", "Stmt")) {
        // Stmt := WHILE LP Exp RP Stmt
        if(debug) printf("Stmt := WHILE LP Exp RP Stmt\n");
        Node* exp = Stmt->firstChild->nextSibling->nextSibling;
        Type* cond = analyseExp(exp);
        if (!equalType(cond, TYPE_INT)) {
            printSemanticError(7, exp->lineNum, "Type mismatched for \"while\"(int only)");
        }
        analyseStmt(exp->nextSibling->nextSibling, func);
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
        param->type = symbol->type;
        param->next = NULL;
        return param;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseParamDec! No matched production.\033[0m\n");
        return NULL;
    }
}

void printSemanticError(int errorType, int lineNum, char* msg) {
    fprintf(stderr, "Error type %d at Line %d: %s.\n", errorType, lineNum, msg);
}