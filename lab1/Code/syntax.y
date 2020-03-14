%locations
%{
    #include <stdio.h>
    #include <string.h>
    #include "lex.yy.c"
    void yyerror(const char* s);
    int print_syn = 1;
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
            struct Node* nodeProgram = createNewNode("Program", NonTerm, @$.first_line);
            nodeProgram->firstChild = $1;
            $$ = nodeProgram;
            syntaxTreeRootNode = nodeProgram;
            printSyntaxTree(syntaxTreeRootNode);
        }
    ;
ExtDefList : ExtDef ExtDefList {
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
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @3.first_line);
            $1->nextSibling = $2;
            $2->nextSibling = nodeSEMI;
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, @$.first_line);
            nodeExtDef->firstChild = $1;
            $$ = nodeExtDef;
        }
    | Specifier SEMI {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @2.first_line);
            $1->nextSibling = nodeSEMI;
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, @$.first_line);
            nodeExtDef->firstChild = $1;
            $$ = nodeExtDef;
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
            struct Node* nodeExtDecList = createNewNode("ExtDecList", NonTerm, @$.first_line);
            nodeExtDecList->firstChild = $1;
            $$ = nodeExtDecList;
        }
    | VarDec COMMA ExtDecList {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, @2.first_line);
            $1->nextSibling = nodeCOMMA;
            nodeCOMMA->nextSibling = $3;
            struct Node* nodeExtDecList = createNewNode("ExtDecList", NonTerm, @$.first_line);
            nodeExtDecList->firstChild = $1;
            $$ = nodeExtDecList;
        }
    ;

/* Specifiers */
Specifier : TYPE {
            struct Node* nodeTYPE = createNewNode("TYPE", ValToken, @1.first_line);
            nodeTYPE->stringVal = $1;
            struct Node* nodeSpecifier = createNewNode("Specifier", NonTerm, @$.first_line);
            nodeSpecifier->firstChild = nodeTYPE;
            $$ = nodeSpecifier;
        }
    | StructSpecifier {
            struct Node* nodeSpecifier = createNewNode("Specifier", NonTerm, @$.first_line);
            nodeSpecifier->firstChild = $1;
            $$ = nodeSpecifier;
        }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {
            struct Node* nodeSTRUCT = createNewNode("STRUCT", NonValToken, @1.first_line);
            struct Node* nodeLC = createNewNode("LC", NonValToken, @3.first_line);
            struct Node* nodeRC = createNewNode("RC", NonValToken, @5.first_line);
            if ($2 == NULL) {
                nodeSTRUCT->nextSibling = nodeLC;
            } else {
                nodeSTRUCT->nextSibling = $2;
                $2->nextSibling = nodeLC;
            }
            if ($4 == NULL) {
                nodeLC->nextSibling = nodeRC;
            } else {
                nodeLC->nextSibling = $4;
                $4->nextSibling = nodeRC;
            }
            struct Node* nodeStructSpecifier = createNewNode("StructSpecifier", NonTerm, @$.first_line);           
            nodeStructSpecifier->firstChild = nodeSTRUCT;           
            $$ = nodeStructSpecifier;
        }
    | STRUCT Tag {
            struct Node* nodeSTRUCT = createNewNode("STRUCT", NonValToken, @1.first_line);           
            nodeSTRUCT->nextSibling = $2;
            struct Node* nodeStructSpecifier = createNewNode("StructSpecifier", NonTerm, @$.first_line);
            nodeStructSpecifier->firstChild = nodeSTRUCT;
            $$ = nodeStructSpecifier;
        }
    ;
OptTag : ID {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeOptTag = createNewNode("OptTag", NonTerm, @$.first_line);
            nodeOptTag->firstChild = nodeID;
            $$ = nodeOptTag;
        }
    | /* empty */ {
            $$ = NULL;
        }
    ;
Tag : ID {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeTag = createNewNode("Tag", NonTerm, @$.first_line);
            nodeTag->firstChild = nodeID;
            $$ = nodeTag;
        }
    ;

/* Declarators */
VarDec : ID {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeVarDec = createNewNode("VarDec", NonTerm, @$.first_line);
            nodeVarDec->firstChild = nodeID;
            $$ = nodeVarDec;
        }
    | VarDec LB INT RB {
            struct Node* nodeLB = createNewNode("LB", NonValToken, @2.first_line);
            struct Node* nodeINT = createNewNode("INT", ValToken, @3.first_line);
            nodeINT->intVal = $3;
            struct Node* nodeRB = createNewNode("RB", NonValToken, @4.first_line);
            $1->nextSibling = nodeLB;
            nodeLB->nextSibling = nodeINT;
            nodeINT->nextSibling = nodeRB;
            struct Node* nodeVarDec = createNewNode("VarDec", NonTerm, @$.first_line);
            nodeVarDec->firstChild = $1;
            $$ = nodeVarDec;
        }
    ;
FunDec : ID LP VarList RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            nodeID->nextSibling = nodeLP;
            nodeLP->nextSibling = $3;
            $3->nextSibling = nodeRP;
            struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, @$.first_line);
            nodeFunDec->firstChild = nodeID;
            $$ = nodeFunDec;
        }
    | ID LP RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @3.first_line);
            nodeID->nextSibling = nodeLP;
            nodeLP->nextSibling = nodeRP;
            struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, @$.first_line);
            nodeFunDec->firstChild = nodeID;
            $$ = nodeFunDec;
        }
    ;
VarList : ParamDec COMMA VarList {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, @2.first_line);
            $1->nextSibling = nodeCOMMA;
            nodeCOMMA->nextSibling = $3;
            struct Node* nodeVarList = createNewNode("VarList", NonTerm, @$.first_line);
            nodeVarList->firstChild = $1;
            $$ = nodeVarList;
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
            struct Node* nodeRC = createNewNode("RC", NonValToken, @4.first_line);
            if ($2 == NULL) {
                if ($3 == NULL) {
                    nodeLC->nextSibling = nodeRC;
                } else {
                    nodeLC->nextSibling = $3;
                    $3->nextSibling = nodeRC;
                }
            } else {
                if ($3 == NULL) {
                    nodeLC->nextSibling = $2;
                    $2->nextSibling = nodeRC;
                } else {
                    nodeLC->nextSibling = $2;
                    $2->nextSibling = $3;
                    $3->nextSibling = nodeRC;
                }
            }
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
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            nodeStmt->firstChild = $1;
            $$ = nodeStmt;
        }
    | RETURN Exp SEMI {
            struct Node* nodeRETURN = createNewNode("RETURN", NonValToken, @1.first_line);
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @3.first_line);
            nodeRETURN->nextSibling = $2;
            $2->nextSibling = nodeSEMI;
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            nodeStmt->firstChild = nodeRETURN;
            $$ = nodeStmt;
        }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
            struct Node* nodeIF = createNewNode("IF", NonValToken, @1.first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            nodeIF->nextSibling = nodeLP;
            nodeLP->nextSibling = $3;
            $3->nextSibling = nodeRP;
            nodeRP->nextSibling = $5;
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            nodeStmt->firstChild = nodeIF;
            $$ = nodeStmt;
        }
    | IF LP Exp RP Stmt ELSE Stmt {
            struct Node* nodeIF = createNewNode("IF", NonValToken, @1.first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            struct Node* nodeELSE = createNewNode("ELSE", NonValToken, @6.first_line);
            nodeIF->nextSibling = nodeLP;
            nodeLP->nextSibling = $3;
            $3->nextSibling = nodeRP;
            nodeRP->nextSibling = $5;
            $5->nextSibling = nodeELSE;
            nodeELSE->nextSibling = $7;
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            nodeStmt->firstChild = nodeIF;
            $$ = nodeStmt;
        }
    | WHILE LP Exp RP Stmt {
            struct Node* nodeWHILE = createNewNode("WHILE", NonValToken, @1.first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            nodeWHILE->nextSibling = nodeLP;
            nodeLP->nextSibling = $3;
            $3->nextSibling = nodeRP;
            nodeRP->nextSibling = $5;
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            nodeStmt->firstChild = nodeWHILE;
            $$ = nodeStmt;
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
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, @2.first_line);
            $1->nextSibling = nodeCOMMA;
            nodeCOMMA->nextSibling = $3;
            struct Node* nodeDecList = createNewNode("DecList", NonTerm, @$.first_line);
            nodeDecList->firstChild = $1;
            $$ = nodeDecList;
        }
    ;
Dec : VarDec {
            struct Node* nodeDec = createNewNode("Dec", NonTerm, @$.first_line);
            nodeDec->firstChild = $1;
            $$ = nodeDec;
        }
    | VarDec ASSIGNOP Exp {
            struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, @2.first_line);
            $1->nextSibling = nodeASSIGNOP;
            nodeASSIGNOP->nextSibling = $3;
            struct Node* nodeDec = createNewNode("Dec", NonTerm, @$.first_line);
            nodeDec->firstChild = $1;
            $$ = nodeDec;
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
            struct Node* nodeAND = createNewNode("AND", NonValToken, @2.first_line);
            $1->nextSibling = nodeAND;
            nodeAND->nextSibling = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
        }
    | Exp OR Exp {
            struct Node* nodeOR = createNewNode("OR", NonValToken, @2.first_line);
            $1->nextSibling = nodeOR;
            nodeOR->nextSibling = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
        }
    | Exp RELOP Exp {
            struct Node* nodeRELOP = createNewNode("RELOP", NonValToken, @2.first_line);
            $1->nextSibling = nodeRELOP;
            nodeRELOP->nextSibling = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
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
            struct Node* nodeSTAR = createNewNode("STAR", NonValToken, @2.first_line);
            $1->nextSibling = nodeSTAR;
            nodeSTAR->nextSibling = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
        }
    | Exp DIV Exp {
            struct Node* nodeDIV = createNewNode("DIV", NonValToken, @2.first_line);
            $1->nextSibling = nodeDIV;
            nodeDIV->nextSibling = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
        }
    | LP Exp RP {
            struct Node* nodeLP = createNewNode("LP", NonValToken, @1.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @3.first_line);
            nodeLP->nextSibling = $2;
            $2->nextSibling = nodeRP;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = nodeLP;
            $$ = nodeExp;
        }
    | MINUS Exp {
            struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, @1.first_line);
            nodeMINUS->nextSibling = $2;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = nodeMINUS;
            $$ = nodeExp;
        }
    | NOT Exp {
            struct Node* nodeNOT = createNewNode("NOT", NonValToken, @1.first_line);
            nodeNOT->nextSibling = $2;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = nodeNOT;
            $$ = nodeExp;
        }
    | ID LP Args RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            nodeID->nextSibling = nodeLP;
            nodeLP->nextSibling = $3;
            $3->nextSibling = nodeRP;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = nodeID;
            $$ = nodeExp;
        }
    | ID LP RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @3.first_line);
            nodeID->nextSibling = nodeLP;
            nodeLP->nextSibling = nodeRP;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = nodeID;
            $$ = nodeExp;
        }
    | Exp LB Exp RB {
            struct Node* nodeLB = createNewNode("LB", NonValToken, @2.first_line);
            struct Node* nodeRB = createNewNode("RB", NonValToken, @4.first_line);
            $1->nextSibling = nodeLB;
            nodeLB->nextSibling = $3;
            $3->nextSibling = nodeRB;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
        }
    | Exp DOT ID {
            struct Node* nodeDOT = createNewNode("DOT", NonValToken, @2.first_line);
            struct Node* nodeID = createNewNode("ID", ValToken, @3.first_line);
            nodeID->stringVal = $3;
            $1->nextSibling = nodeDOT;
            nodeDOT->nextSibling = nodeID;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            nodeExp->firstChild = $1;
            $$ = nodeExp;
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
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, @2.first_line);
            $1->nextSibling = nodeCOMMA;
            nodeCOMMA->nextSibling = $3;
            struct Node* nodeArgs = createNewNode("Args", NonTerm, @$.first_line);
            nodeArgs->firstChild = $1;
            $$ = nodeArgs;
        }
    | Exp {
            struct Node* nodeArgs = createNewNode("Args", NonTerm, @$.first_line);
            nodeArgs->firstChild = $1;
            $$ = nodeArgs;
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
