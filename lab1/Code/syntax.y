%locations
%{
    #include <stdio.h>
    #include <string.h>
    #include "lex.yy.c"
    void yyerror(const char* s);
    int print_syn = 0;

    /* 
     * Type of the node:
     *  - NonTerm: Non-Terminal, line number should be printed
     *  - NonValToken: Token without value, print node name only
     *  - ValToken: Token with value, value should be printed
     */
    enum NodeType { NonTerm, NonValToken, ValToken };
    struct Node {
        char* nodeName;
        enum NodeType nodeType;
        int lineNum;
        union {
            int intVal;
            float floatVal;
            char* stringVal;
        };
        struct Node* firstChild;
        struct Node* nextSibling;
    };

    struct Node* syntaxTreeRootNode = NULL;

    struct Node* createNewNode(char* nodeName, enum NodeType nodeType, int lineNum);
    void printSyntaxTree(struct Node* rootNode);
%}

/* declared types */
%union {
    int type_int;
    float type_float;
    char* type_string;
    struct Node* type_pnode;
}

/* declared tokens */
%token RELOP ASSIGNOP
%token SEMI COMMA
%token PLUS MINUS STAR DIV AND OR NOT
%token DOT
%token LP RP LB RB LC RC
%token <type_int> INT
%token <type_float> FLOAT
%token <type_string> ID TYPE
%token IF ELSE WHILE STRUCT RETURN

/* declared non-terminals */
%type <type_pnode> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier
%type <type_pnode> OptTag Tag VarDec FunDec VarList ParamDec CompSt
%type <type_pnode> StmtList Stmt DefList Def DecList Dec Exp Args

/* precedence and associativity */
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT // 取负符号怎么与减号区分？
%left LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
/* High-level Definitions */
Program : ExtDefList {
            if (print_syn) printf("[Program->ExtDefList]\n");
            struct Node* nodeProgram = createNewNode("Program", NonTerm, @$.first_line);
            nodeProgram->firstChild = $1;
            $$ = nodeProgram;
            syntaxTreeRootNode = nodeProgram;
            printSyntaxTree(syntaxTreeRootNode);
        }
    ;
ExtDefList : ExtDef ExtDefList {
            if (print_syn) printf("[ExtDefList->ExtDef ExtDefList]\n");
            $1->nextSibling = $2;
            struct Node* nodeExtDefList = createNewNode("ExtDefList", NonTerm, @$.first_line);
            nodeExtDefList->firstChild = $1;
            $$ = nodeExtDefList;
        }
    | /* empty */ {
            $$ = NULL;
        }
    ;
ExtDef : Specifier ExtDecList SEMI {

        }
    | Specifier SEMI {

        }
    | Specifier FunDec CompSt {
            $1->nextSibling = $2;
            $2->nextSibling = $3;
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, @$.first_line);
            nodeExtDef->firstChild = $1;
            $$ = nodeExtDef;
        }
    ;
ExtDecList : VarDec {

        }
    | VarDec COMMA ExtDecList {

        }
    ;

/* Specifiers */
Specifier : TYPE {
            struct Node* nodeTYPE = createNewNode("TYPE", ValToken, @1.first_line);
            //printf("%s\n", $1);
            nodeTYPE->stringVal = $1;
            struct Node* nodeSpecifier = createNewNode("Specifier", NonTerm, @$.first_line);
            nodeSpecifier->firstChild = nodeTYPE;
            $$ = nodeSpecifier;
        }
    | StructSpecifier {

        }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {

        }
    | STRUCT Tag {

        }
    ;
OptTag : ID {

        }
    | /* empty */ {

        }
    ;
Tag : ID {

        }
    ;

/* Declarators */
VarDec : ID {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            //printf("%s\n", $1);
            struct Node* nodeVarDec = createNewNode("VarDec", NonTerm, @$.first_line);
            nodeVarDec->firstChild = nodeID;
            $$ = nodeVarDec;
        }
    | VarDec LB INT RB {

        }
    ;
FunDec : ID LP VarList RP {

        }
    | ID LP RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            printf("%s\n", $1);
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            nodeID->nextSibling = nodeLP;
            struct Node* nodeRP = createNewNode("RP", NonValToken, @3.first_line);
            nodeLP->nextSibling = nodeRP;
            struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, @$.first_line);
            nodeFunDec->firstChild = nodeID;
            $$ = nodeFunDec;
        }
    ;
VarList : ParamDec COMMA VarList {

        }
    | ParamDec {
            struct Node* nodeVarList = createNewNode("VarList", NonTerm, @$.first_line);
            nodeVarList->firstChild = $1;
            $$ = nodeVarList;
        }
    ;
ParamDec : Specifier VarDec {
            $1->nextSibling = $2;
            struct Node* nodeParamDec = createNewNode("ParamDec", NonTerm, @$.first_line);
            nodeParamDec->firstChild = $1;
            $$ = nodeParamDec;
        }
    ;

/* Statements */
CompSt : LC DefList StmtList RC {
            struct Node* nodeLC = createNewNode("LC", NonValToken, @1.first_line);
            nodeLC->nextSibling = $2;
            $2->nextSibling = $3;
            struct Node* nodeRC = createNewNode("RC", NonValToken, @4.first_line);
            $3->nextSibling = nodeRC;
            struct Node* nodeCompSt = createNewNode("CompSt", NonTerm, @$.first_line);
            nodeCompSt->firstChild = nodeLC;
            $$ = nodeCompSt;
        }
    ;
StmtList : Stmt StmtList {
            $1->nextSibling = $2;
            struct Node* nodeStmtList = createNewNode("StmtList", NonTerm, @$.first_line);
            nodeStmtList->firstChild = $1;
            $$ = nodeStmtList;
        }
    | /* empty */ {
            $$ = NULL;
        }
    ;
Stmt : Exp SEMI {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @2.first_line);
            $1->nextSibling = nodeSEMI;
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            nodeStmt->firstChild = $1;
            $$ = nodeStmt;
        }
    | CompSt {

        }
    | RETURN Exp SEMI {

        }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {

        }
    | IF LP Exp RP Stmt ELSE Stmt {

        }
    | WHILE LP Exp RP Stmt {

        }
    ;

/* Local Definitions */
DefList : Def DefList {
            $1->nextSibling = $2;
            struct Node* nodeDefList = createNewNode("DefList", NonTerm, @$.first_line);
            nodeDefList->firstChild = $1;
            $$ = nodeDefList;
        }
    | /* empty */ {
            $$ = NULL;
        }
    ;
Def : Specifier DecList SEMI {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @3.first_line);
            $1->nextSibling = $2;
            $2->nextSibling = nodeSEMI;
            struct Node* nodeDef = createNewNode("Def", NonTerm, @$.first_line);
            nodeDef->firstChild = $1;
            $$ = nodeDef;
        }
    ;
DecList : Dec {
            struct Node* nodeDecList = createNewNode("DecList", NonTerm, @$.first_line);
            nodeDecList->firstChild = $1;
            $$ = nodeDecList;
        }
    | Dec COMMA DecList {

        }
    ;
Dec : VarDec {
            struct Node* nodeDec = createNewNode("Dec", NonTerm, @$.first_line);
            nodeDec->firstChild = $1;
            $$ = nodeDec;
        }
    | VarDec ASSIGNOP Exp {

        }
    ;

/* Expressions */
Exp : Exp ASSIGNOP Exp {
            struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, @2.first_line);
            $1->nextSibling = nodeASSIGNOP;
            nodeASSIGNOP->nextSibling = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
        }
    | Exp AND Exp {

        }
    | Exp OR Exp {

        }
    | Exp RELOP Exp {

        }
    | Exp PLUS Exp {
            struct Node* nodePLUS = createNewNode("PLUS", NonValToken, @2.first_line);
            $1->nextSibling = nodePLUS;
            nodePLUS->nextSibling = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
        }
    | Exp MINUS Exp {
            struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, @2.first_line);
            $1->nextSibling = nodeMINUS;
            nodeMINUS->nextSibling = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
        }
    | Exp STAR Exp {

        }
    | Exp DIV Exp {

        }
    | LP Exp RP {

        }
    | MINUS Exp {

        }
    | NOT Exp {

        }
    | ID LP Args RP {

        }
    | ID LP RP {

        }
    | Exp LB Exp RB {

        }
    | Exp DOT ID {

        }
    | ID {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = nodeID;
            $$ = nodeExp;
        }
    | INT {
            struct Node* nodeINT = createNewNode("INT", ValToken, @1.first_line);
            nodeINT->intVal = $1;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = nodeINT;
            $$ = nodeExp;
        }
    | FLOAT {
            struct Node* nodeFLOAT = createNewNode("FLOAT", ValToken, @1.first_line);
            nodeFLOAT->floatVal = $1;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = nodeFLOAT;
            $$ = nodeExp;
        }
    ;
Args : Exp COMMA Args {

        }
    | Exp {

        }
    ;

%%
struct Node* createNewNode(char* nodeName, enum NodeType nodeType, int lineNum) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->nodeName = nodeName;
    newNode->nodeType = nodeType;
    newNode->lineNum = lineNum;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

void _printSyntaxTree(struct Node* rootNode, int spaceNum) {
    if (rootNode == NULL)
        return;
    for (int i = 0; i < spaceNum; i++) {
        printf(" ");
    }
    switch (rootNode->nodeType) {
        case NonTerm:
            printf("%s (%d)\n", rootNode->nodeName, rootNode->lineNum);
            break;
        case NonValToken:
            printf("%s\n", rootNode->nodeName);
            break;
        case ValToken:
            printf("%s: ", rootNode->nodeName);
            if ((strcmp(rootNode->nodeName, "TYPE") == 0) || (strcmp(rootNode->nodeName, "ID") == 0)) {
                printf("%s\n", rootNode->stringVal);
            } else if (strcmp(rootNode->nodeName, "INT") == 0) {
                printf("%d\n", rootNode->intVal);
            } else if (strcmp(rootNode->nodeName, "FLOAT") == 0) {
                printf("%f\n", rootNode->floatVal);
            } else {
                printf("ERROR!!!");
            }
            break;
        default:
            printf("ERROR!!!");
    }
    spaceNum += 2;
    struct Node* firstChild = rootNode->firstChild;
    if (firstChild != NULL) {
        _printSyntaxTree(firstChild, spaceNum);
        struct Node* sibling = firstChild->nextSibling;
        while (sibling != NULL) {
            _printSyntaxTree(sibling, spaceNum);
            sibling = sibling->nextSibling;
        }
    }
}

void printSyntaxTree(struct Node* rootNode) {
    _printSyntaxTree(rootNode, 0);
}
