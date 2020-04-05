#include "semantic.h"

int stringEqual(char* s1, char* s2) {
    if (strcmp(s1, s2) == 0)
        return 1;
    else
        return 0;
}

void sementicAnalysis(const Node* syntaxTreeRoot) {
    printf("\n---------Sementic Analysis---------\n");
    analyseProgram(syntaxTreeRoot);
}

void analyseProgram(const Node* Program) {
    if (Program == NULL) return;
    analyseExtDefList(Program->firstChild);
}

void analyseExtDefList(const Node* ExtDefList) {
    if (ExtDefList == NULL) return;
    struct Node* ExtDef = ExtDefList->firstChild;
    analyseExtDef(ExtDef);
    analyseExtDefList(ExtDef->nextSibling);
}

void analyseExtDef(const Node* ExtDef) {
    struct Node* Specifier = ExtDef->firstChild;
    Type* type = analyseSpecifier(Specifier);
    if (type == NULL) {
        fprintf(stderr, "\033[31mERROR in analyseExtDef->analyseSpecifier!\033[0m\n");
        return;
    }

    if (Specifier->nextSibling != NULL && stringEqual(Specifier->nextSibling->nodeName, "ExtDecList") && Specifier->nextSibling->nextSibling != NULL && stringEqual(Specifier->nextSibling->nextSibling->nodeName, "SEMI")) {
        //ExtDef := Specifier ExtDecList SEMI

    } else if (Specifier->nextSibling != NULL && stringEqual(Specifier->nextSibling->nodeName, "SEMI")) {
        //ExtDef := Specifier SEMI
    } else if (Specifier->nextSibling != NULL && stringEqual(Specifier->nextSibling->nodeName, "FunDec") && Specifier->nextSibling->nextSibling != NULL && stringEqual(Specifier->nextSibling->nextSibling->nodeName, "CompSt")) {
        //ExtDef := Specifier FunDec CompSt
    } else {
        fprintf(stderr, "\033[31mERROR in analyseExtDef!\033[0m\n");
    }
}

Type* analyseSpecifier(const Node* Specifier) {
    if (Specifier->firstChild != NULL && stringEqual(Specifier->firstChild->nodeName, "TYPE")) {
        //Specifier := TYPE
        Node* TYPE = Specifier->firstChild;
        return analyseTYPE(TYPE);
    } else if (Specifier->firstChild != NULL && stringEqual(Specifier->firstChild->nodeName, "StructSpecifier")) {
        //Specifier := StructSpecifier
        Node* StructSpecifier = Specifier->firstChild;
        return analyseStructSpecifier(StructSpecifier);
    } else {
        fprintf(stderr, "\033[31mERROR in analyseSpecifier! No matched production.\033[0m\n");
        return NULL;
    }
}

Type* analyseTYPE(const Node* TYPE) {
    if (stringEqual(TYPE->stringVal, "int")) {
        Type* newType = (Type*)malloc(sizeof(Type));
        newType->kind = BASIC;
        newType->basic = INT;
    } else if (stringEqual(TYPE->stringVal, "float")) {
        Type* newType = (Type*)malloc(sizeof(Type));
        newType->kind = BASIC;
        newType->basic = FLOAT;
    } else {
        fprintf(stderr, "\033[31mERROR in analyseExtDef! Specifier := TYPE, unknown TYPE!\033[0m\n");
        return NULL;
    }
}

Type* analyseStructSpecifier(const Node* StructSpecifier) {
    Node* STRUCT = StructSpecifier->firstChild;
    if (STRUCT == NULL) {
        fprintf(stderr, "\033[31mERROR in analyseStructSpecifier! Missing Node STRUCT.\033[0m\n");
        return NULL;
    }

    if (STRUCT->nextSibling != NULL && stringEqual(STRUCT->nextSibling->nodeName, "Tag")) {
        //StructSpecifier := STRUCT Tag

    } else if (STRUCT->nextSibling != NULL && stringEqual(STRUCT->nextSibling->nodeName, "LC") && \
               STRUCT->nextSibling->nextSibling != NULL && stringEqual(STRUCT->nextSibling->nextSibling->nodeName, "DefList") && \
               STRUCT->nextSibling->nextSibling->nextSibling != NULL && stringEqual(STRUCT->nextSibling->nextSibling->nextSibling->nodeName, "RC")) {
        //StructSpecifier := STRUCT OptTag(== NULL) LC DefList RC

    } else if (STRUCT->nextSibling != NULL && stringEqual(STRUCT->nextSibling->nodeName, "OptTag") && \
               STRUCT->nextSibling->nextSibling != NULL && stringEqual(STRUCT->nextSibling->nextSibling->nodeName, "LC") && \
               STRUCT->nextSibling->nextSibling->nextSibling != NULL && stringEqual(STRUCT->nextSibling->nextSibling->nextSibling->nodeName, "DefList") && \
               STRUCT->nextSibling->nextSibling->nextSibling->nextSibling != NULL && stringEqual(STRUCT->nextSibling->nextSibling->nextSibling->nextSibling->nodeName, "RC")) {
        //StructSpecifier := STRUCT OptTag(!= NULL) LC DefList RC
        
    } else {
        fprintf(stderr, "\033[31mERROR in analyseStructSpecifier! No matched production.\033[0m\n");
        return NULL;
    }
}