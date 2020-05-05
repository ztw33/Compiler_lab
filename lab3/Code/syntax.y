%locations
%{
    #include "lex.yy.c"
    #include "SyntaxTree.h"

    void yyerror(const char* s);

    int errorNum = 0;
    int lastErrorLineno = 0;

    struct Node* syntaxTreeRootNode = NULL;

    void printError(char errorType, int lineno, char* msg);
    int isNewError(int errorLineno);
%}

/* declared types */
%union {
    int type_int;
    float type_float;
    char* type_string;
    struct Node* type_pnode;
}

/* declared tokens */
%token ASSIGNOP
%token SEMI COMMA
%token PLUS MINUS STAR DIV AND OR NOT
%token DOT
%token LP RP LB RB LC RC
%token <type_int> INT
%token <type_float> FLOAT
%token <type_string> ID TYPE RELOP
%token IF ELSE WHILE STRUCT RETURN

/* declared non-terminals */
%type <type_pnode> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier
%type <type_pnode> OptTag Tag VarDec FunDec VarList ParamDec CompSt
%type <type_pnode> StmtList Stmt DefList Def DecList Dec Exp Args

/* precedence and associativity */
%nonassoc error
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
/* High-level Definitions */
Program : ExtDefList {
            struct Node* nodeProgram = createNewNode("Program", NonTerm, @$.first_line);
            buildRel(nodeProgram, 1, $1);
            $$ = nodeProgram;
            syntaxTreeRootNode = nodeProgram;
        }
    | ExtDefList error {
            if (isNewError(@2.first_line)) {
                printError('B', @2.first_line, "Unexpected character");
                struct Node* nodeError = createNewNode("error", NonValToken, @2.first_line);
                struct Node* nodeProgram = createNewNode("Program", NonTerm, @$.first_line);
                buildRel(nodeProgram, 2, $1, nodeError);
                $$ = nodeProgram;
                syntaxTreeRootNode = nodeProgram;
            } else {
            	$$ = NULL;
            }
        }
    ;
ExtDefList : ExtDef ExtDefList {
            struct Node* nodeExtDefList = createNewNode("ExtDefList", NonTerm, @$.first_line);
            buildRel(nodeExtDefList, 2, $1, $2);
            $$ = nodeExtDefList;
        }
    | /* empty */ {
            $$ = NULL;
        }
    ;
ExtDef : Specifier ExtDecList SEMI {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @3.first_line);
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, @$.first_line);
            buildRel(nodeExtDef, 3, $1, $2, nodeSEMI);
            $$ = nodeExtDef;
        }
    | Specifier SEMI {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @2.first_line);
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, @$.first_line);
            buildRel(nodeExtDef, 2, $1, nodeSEMI);
            $$ = nodeExtDef;
        }
    | Specifier FunDec CompSt {
            struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, @$.first_line);
            buildRel(nodeExtDef, 3, $1, $2, $3);
            $$ = nodeExtDef;
        }
    | Specifier error {
            if (isNewError(@2.first_line)) {
                printError('B', @2.first_line, "Missing \";\"");
                struct Node* nodeError = createNewNode("error", NonValToken, @2.first_line);
                struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, @$.first_line);
                buildRel(nodeExtDef, 2, $1, nodeError);
                $$ = nodeExtDef;
            } else {
            	$$ = NULL;
            }
        }
    | Specifier FunDec SEMI {
            if (isNewError(@2.first_line)) {
                printError('B', @2.first_line, "Incomplete definition of function");
                struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @2.first_line);
                struct Node* nodeExtDef = createNewNode("ExtDef", NonTerm, @$.first_line);
                buildRel(nodeExtDef, 3, $1, $2, nodeSEMI);
                $$ = nodeExtDef;
            } else {
            	$$ = NULL;
            }
        }
    ;
ExtDecList : VarDec {
            struct Node* nodeExtDecList = createNewNode("ExtDecList", NonTerm, @$.first_line);
            buildRel(nodeExtDecList, 1, $1);
            $$ = nodeExtDecList;
        }
    | VarDec COMMA ExtDecList {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, @2.first_line);
            struct Node* nodeExtDecList = createNewNode("ExtDecList", NonTerm, @$.first_line);
            buildRel(nodeExtDecList, 3, $1, nodeCOMMA, $3);
            $$ = nodeExtDecList;
        }
    ;

/* Specifiers */
Specifier : TYPE {
            struct Node* nodeTYPE = createNewNode("TYPE", ValToken, @1.first_line);
            nodeTYPE->stringVal = $1;
            struct Node* nodeSpecifier = createNewNode("Specifier", NonTerm, @$.first_line);
            buildRel(nodeSpecifier, 1, nodeTYPE);
            $$ = nodeSpecifier;
        }
    | StructSpecifier {
            struct Node* nodeSpecifier = createNewNode("Specifier", NonTerm, @$.first_line);
            buildRel(nodeSpecifier, 1, $1);
            $$ = nodeSpecifier;
        }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {
            struct Node* nodeSTRUCT = createNewNode("STRUCT", NonValToken, @1.first_line);
            struct Node* nodeLC = createNewNode("LC", NonValToken, @3.first_line);
            struct Node* nodeRC = createNewNode("RC", NonValToken, @5.first_line);
            struct Node* nodeStructSpecifier = createNewNode("StructSpecifier", NonTerm, @$.first_line);           
            buildRel(nodeStructSpecifier, 5, nodeSTRUCT, $2, nodeLC, $4, nodeRC);          
            $$ = nodeStructSpecifier;
        }
    | STRUCT Tag {
            struct Node* nodeSTRUCT = createNewNode("STRUCT", NonValToken, @1.first_line);           
            struct Node* nodeStructSpecifier = createNewNode("StructSpecifier", NonTerm, @$.first_line);
            buildRel(nodeStructSpecifier, 2, nodeSTRUCT, $2);
            $$ = nodeStructSpecifier;
        }
    | STRUCT OptTag LC DefList error {
            if (isNewError(@5.first_line)) {
                printError('B', @5.first_line, "Missing \"}\"");
                struct Node* nodeSTRUCT = createNewNode("STRUCT", NonValToken, @1.first_line);
                struct Node* nodeLC = createNewNode("LC", NonValToken, @3.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @5.first_line);
                struct Node* nodeStructSpecifier = createNewNode("StructSpecifier", NonTerm, @$.first_line);           
                buildRel(nodeStructSpecifier, 5, nodeSTRUCT, $2, nodeLC, $4, nodeError);          
                $$ = nodeStructSpecifier;
            } else {
            	$$ = NULL;
            }
        }
    ;
OptTag : ID {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeOptTag = createNewNode("OptTag", NonTerm, @$.first_line);
            buildRel(nodeOptTag, 1, nodeID);
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
            buildRel(nodeTag, 1, nodeID);
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
            struct Node* nodeVarDec = createNewNode("VarDec", NonTerm, @$.first_line);
            buildRel(nodeVarDec, 4, $1, nodeLB, nodeINT, nodeRB);
            $$ = nodeVarDec;
        }
    | VarDec LB error RB {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error between \"[]\"");
                struct Node* nodeLB = createNewNode("LB", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);
                struct Node* nodeRB = createNewNode("RB", NonValToken, @4.first_line);
                struct Node* nodeVarDec = createNewNode("VarDec", NonTerm, @$.first_line);
                buildRel(nodeVarDec, 4, $1, nodeLB, nodeError, nodeRB);
                $$ = nodeVarDec;
            } else {
            	$$ = NULL;
            }
        }
    ;
FunDec : ID LP VarList RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, @$.first_line);
            buildRel(nodeFunDec, 4, nodeID, nodeLP, $3, nodeRP);
            $$ = nodeFunDec;
        }
    | ID LP RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @3.first_line);
            struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, @$.first_line);
            buildRel(nodeFunDec, 3, nodeID, nodeLP, nodeRP);
            $$ = nodeFunDec;
        }
    | ID LP error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Missing \")\"");
                struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
                nodeID->stringVal = $1;
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);
                struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, @$.first_line);
                buildRel(nodeFunDec, 3, nodeID, nodeLP, nodeError);
                $$ = nodeFunDec;
            } else {
            	$$ = NULL;
            } 
        }
    | ID LP error RP {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error between ()");
                struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
                nodeID->stringVal = $1;
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);
                struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
                struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, @$.first_line);
                buildRel(nodeFunDec, 4, nodeID, nodeLP, nodeError, nodeRP);
                $$ = nodeFunDec;
            } else {
            	$$ = NULL;
            }
        }
    | ID error RP {
            if (isNewError(@2.first_line)) {
                printError('B', @2.first_line, "Missing \"(\"");
                struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
                nodeID->stringVal = $1;
                struct Node* nodeError = createNewNode("error", NonValToken, @2.first_line);
                struct Node* nodeRP = createNewNode("RP", NonValToken, @3.first_line);
                struct Node* nodeFunDec = createNewNode("FunDec", NonTerm, @$.first_line);
                buildRel(nodeFunDec, 3, nodeID, nodeError, nodeRP);
                $$ = nodeFunDec;
            } else {
            	$$ = NULL;
            }
        }
    ;
VarList : ParamDec COMMA VarList {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, @2.first_line);
            struct Node* nodeVarList = createNewNode("VarList", NonTerm, @$.first_line);
            buildRel(nodeVarList, 3, $1, nodeCOMMA, $3);
            $$ = nodeVarList;
        }
    | ParamDec {
            struct Node* nodeVarList = createNewNode("VarList", NonTerm, @$.first_line);
            nodeVarList->firstChild = $1;
            $$ = nodeVarList;
        }
    ;
ParamDec : Specifier VarDec {
            struct Node* nodeParamDec = createNewNode("ParamDec", NonTerm, @$.first_line);
            buildRel(nodeParamDec, 2, $1, $2);
            $$ = nodeParamDec;
        }
    ;

/* Statements */
CompSt : LC DefList StmtList RC {
            struct Node* nodeLC = createNewNode("LC", NonValToken, @1.first_line);
            struct Node* nodeRC = createNewNode("RC", NonValToken, @4.first_line);
            struct Node* nodeCompSt = createNewNode("CompSt", NonTerm, @$.first_line);
            buildRel(nodeCompSt, 4, nodeLC, $2, $3, nodeRC);
            $$ = nodeCompSt;
        }
    | error DefList StmtList RC {
            if (isNewError(@1.first_line)) {
                printError('B', @1.first_line, "Missing \"{\"");
                struct Node* nodeError = createNewNode("error", NonValToken, @1.first_line);
                struct Node* nodeRC = createNewNode("LC", NonValToken, @4.first_line);
                struct Node* nodeCompSt = createNewNode("CompSt", NonTerm, @$.first_line);
                buildRel(nodeCompSt, 4, nodeError, $2, $3, nodeRC);
                $$ = nodeCompSt;
            } else {
            	$$ = NULL;
            }
        }
    ;
StmtList : Stmt StmtList {
            struct Node* nodeStmtList = createNewNode("StmtList", NonTerm, @$.first_line);
            buildRel(nodeStmtList, 2, $1, $2);
            $$ = nodeStmtList;
        }
    | /* empty */ {
            $$ = NULL;
        }
    ;
Stmt : Exp SEMI {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @2.first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            buildRel(nodeStmt, 2, $1, nodeSEMI);
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
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            buildRel(nodeStmt, 3, nodeRETURN, $2, nodeSEMI);
            $$ = nodeStmt;
        }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
            struct Node* nodeIF = createNewNode("IF", NonValToken, @1.first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            buildRel(nodeStmt, 5, nodeIF, nodeLP, $3, nodeRP, $5);
            $$ = nodeStmt;
        }
    | IF LP Exp RP Stmt ELSE Stmt {
            struct Node* nodeIF = createNewNode("IF", NonValToken, @1.first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            struct Node* nodeELSE = createNewNode("ELSE", NonValToken, @6.first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            buildRel(nodeStmt, 7, nodeIF, nodeLP, $3, nodeRP, $5, nodeELSE, $7);
            $$ = nodeStmt;
        }
    | WHILE LP Exp RP Stmt {
            struct Node* nodeWHILE = createNewNode("WHILE", NonValToken, @1.first_line);
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
            buildRel(nodeStmt, 5, nodeWHILE, nodeLP, $3, nodeRP, $5);
            $$ = nodeStmt;
        }
    | Exp error {
            if (isNewError(@2.first_line)) {
                printError('B', @2.first_line, "Missing \";\"");
                struct Node* nodeError = createNewNode("error", NonValToken, @2.first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);                
                buildRel(nodeStmt, 2, $1, nodeError);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    | RETURN Exp error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Missing \";\"");
                struct Node* nodeRETURN = createNewNode("RETURN", NonValToken, @1.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);                
                buildRel(nodeStmt, 3, nodeRETURN, $2, nodeError);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    | error SEMI {
            if (isNewError(@1.first_line)) {
                printError('B', @1.first_line, "Syntax error in Exp");
                struct Node* nodeError = createNewNode("error", NonValToken, @1.first_line);                
                struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @2.first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
                buildRel(nodeStmt, 2, nodeError, nodeSEMI);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    | IF LP error RP Stmt %prec LOWER_THAN_ELSE {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeIF = createNewNode("IF", NonValToken, @1.first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);                
                struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
                buildRel(nodeStmt, 5, nodeIF, nodeLP, nodeError, nodeRP, $5);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    | IF LP Exp error Stmt %prec LOWER_THAN_ELSE {
            if (isNewError(@4.first_line)) {
                printError('B', @4.first_line, "Missing \")\"");
                struct Node* nodeIF = createNewNode("IF", NonValToken, @1.first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @4.first_line);                
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
                buildRel(nodeStmt, 5, nodeIF, nodeLP, $3, nodeError, $5);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    | IF LP error RP Stmt ELSE Stmt {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeIF = createNewNode("IF", NonValToken, @1.first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);                
                struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
                struct Node* nodeELSE = createNewNode("ELSE", NonValToken, @6.first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
                buildRel(nodeStmt, 7, nodeIF, nodeLP, nodeError, nodeRP, $5, nodeELSE, $7);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    | IF LP Exp error Stmt ELSE Stmt {
            if (isNewError(@4.first_line)) {
                printError('B', @4.first_line, "Missing \")\"");
                struct Node* nodeIF = createNewNode("IF", NonValToken, @1.first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @4.first_line);                
                struct Node* nodeELSE = createNewNode("ELSE", NonValToken, @6.first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
                buildRel(nodeStmt, 7, nodeIF, nodeLP, $3, nodeError, $5, nodeELSE, $7);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    | WHILE LP error RP Stmt {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeWHILE = createNewNode("WHILE", NonValToken, @1.first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);
                struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
                buildRel(nodeStmt, 5, nodeWHILE, nodeLP, nodeError, nodeRP, $5);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    | WHILE LP Exp error Stmt {
            if (isNewError(@4.first_line)) {
                printError('B', @4.first_line, "Missing \")\"");
                struct Node* nodeWHILE = createNewNode("WHILE", NonValToken, @1.first_line);
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @4.first_line);
                struct Node* nodeStmt = createNewNode("Stmt", NonTerm, @$.first_line);
                buildRel(nodeStmt, 5, nodeWHILE, nodeLP, $3, nodeError, $5);
                $$ = nodeStmt;
            } else {
            	$$ = NULL;
            }
        }
    ;

/* Local Definitions */
DefList : Def DefList {
            struct Node* nodeDefList = createNewNode("DefList", NonTerm, @$.first_line);
            buildRel(nodeDefList, 2, $1, $2);
            $$ = nodeDefList;
        }
    | /* empty */ {
            $$ = NULL;
        }
    ;
Def : Specifier DecList SEMI {
            struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @3.first_line);
            struct Node* nodeDef = createNewNode("Def", NonTerm, @$.first_line);
            buildRel(nodeDef, 3, $1, $2, nodeSEMI);
            $$ = nodeDef;
        }
    | Specifier error SEMI {
            if (isNewError(@2.first_line)) {
                printError('B', @2.first_line, "Syntax error in DecList");
                struct Node* nodeError = createNewNode("error", NonValToken, @2.first_line);
                struct Node* nodeSEMI = createNewNode("SEMI", NonValToken, @3.first_line);
                struct Node* nodeDef = createNewNode("Def", NonTerm, @$.first_line);
                buildRel(nodeDef, 3, $1, nodeError, nodeSEMI);
                $$ = nodeDef;
            } else {
            	$$ = NULL;
            }
        }
    ;
DecList : Dec {
            struct Node* nodeDecList = createNewNode("DecList", NonTerm, @$.first_line);
            nodeDecList->firstChild = $1;
            $$ = nodeDecList;
        }
    | Dec COMMA DecList {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, @2.first_line);
            struct Node* nodeDecList = createNewNode("DecList", NonTerm, @$.first_line);
            buildRel(nodeDecList, 3, $1, nodeCOMMA, $3);
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
            struct Node* nodeDec = createNewNode("Dec", NonTerm, @$.first_line);
            buildRel(nodeDec, 3, $1, nodeASSIGNOP, $3);
            $$ = nodeDec;
        }
    | VarDec ASSIGNOP error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);
                struct Node* nodeDec = createNewNode("Dec", NonTerm, @$.first_line);
                buildRel(nodeDec, 3, $1, nodeASSIGNOP, nodeError);
                $$ = nodeDec;
            } else {
            	$$ = NULL;
            }
        }
    ;

/* Expressions */
Exp : Exp ASSIGNOP Exp {
            struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, @2.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodeASSIGNOP, $3);
            $$ = nodeExp;
        }
    | Exp AND Exp {
            struct Node* nodeAND = createNewNode("AND", NonValToken, @2.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodeAND, $3);
            $$ = nodeExp;
        }
    | Exp OR Exp {
            struct Node* nodeOR = createNewNode("OR", NonValToken, @2.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodeOR, $3);
            $$ = nodeExp;
        }
    | Exp RELOP Exp {
            struct Node* nodeRELOP = createNewNode("RELOP", NonValToken, @2.first_line);
            nodeRELOP->stringVal = $2;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodeRELOP, $3);
            $$ = nodeExp;
        }
    | Exp PLUS Exp {
            struct Node* nodePLUS = createNewNode("PLUS", NonValToken, @2.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodePLUS, $3);
            $$ = nodeExp;
        }
    | Exp MINUS Exp {
            struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, @2.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodeMINUS, $3);
            $$ = nodeExp;
        }
    | Exp STAR Exp {
            struct Node* nodeSTAR = createNewNode("STAR", NonValToken, @2.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodeSTAR, $3);
            $$ = nodeExp;
        }
    | Exp DIV Exp {
            struct Node* nodeDIV = createNewNode("DIV", NonValToken, @2.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodeDIV, $3);
            $$ = nodeExp;
        }
    | LP Exp RP {
            struct Node* nodeLP = createNewNode("LP", NonValToken, @1.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @3.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, nodeLP, $2, nodeRP);
            $$ = nodeExp;
        }
    | MINUS Exp {
            struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, @1.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 2, nodeMINUS, $2);
            $$ = nodeExp;
        }
    | NOT Exp {
            struct Node* nodeNOT = createNewNode("NOT", NonValToken, @1.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 2, nodeNOT, $2);
            $$ = nodeExp;
        }
    | ID LP Args RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @4.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 4, nodeID, nodeLP, $3, nodeRP);
            $$ = nodeExp;
        }
    | ID LP RP {
            struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
            nodeID->stringVal = $1;
            struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
            struct Node* nodeRP = createNewNode("RP", NonValToken, @3.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, nodeID, nodeLP, nodeRP);
            $$ = nodeExp;
        }
    | Exp LB Exp RB {
            struct Node* nodeLB = createNewNode("LB", NonValToken, @2.first_line);
            struct Node* nodeRB = createNewNode("RB", NonValToken, @4.first_line);
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 4, $1, nodeLB, $3, nodeRB);
            $$ = nodeExp;
        }
    | Exp DOT ID {
            struct Node* nodeDOT = createNewNode("DOT", NonValToken, @2.first_line);
            struct Node* nodeID = createNewNode("ID", ValToken, @3.first_line);
            nodeID->stringVal = $3;
            struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
            buildRel(nodeExp, 3, $1, nodeDOT, nodeID);
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
    | Exp LB error RB {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error between \"[]\"");
                struct Node* nodeLB = createNewNode("LB", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);
                struct Node* nodeRB = createNewNode("RB", NonValToken, @4.first_line);
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 4, $1, nodeLB, nodeError, nodeRB);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | error RP {
            if (isNewError(@1.first_line)) {
                printError('B', @1.first_line, "Missing \"(\"");
                struct Node* nodeError = createNewNode("error", NonValToken, @1.first_line);
                struct Node* nodeRP = createNewNode("RP", NonValToken, @2.first_line);
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 2, nodeError, nodeRP);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | ID LP Args error {
            if (isNewError(@4.first_line)) {
                printError('B', @4.first_line, "Missing \")\"");
                struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
                nodeID->stringVal = $1;
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @4.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 4, nodeID, nodeLP, $3, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | ID LP error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Missing \")\"");
                struct Node* nodeID = createNewNode("ID", ValToken, @1.first_line);
                nodeID->stringVal = $1;
                struct Node* nodeLP = createNewNode("LP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, nodeID, nodeLP, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | Exp ASSIGNOP error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeASSIGNOP = createNewNode("ASSIGNOP", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, $1, nodeASSIGNOP, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | Exp AND error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeAND = createNewNode("AND", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, $1, nodeAND, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | Exp OR error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeOR = createNewNode("OR", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, $1, nodeOR, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | Exp RELOP error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeRELOP = createNewNode("RELOP", NonValToken, @2.first_line);
                nodeRELOP->stringVal = $2;
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, $1, nodeRELOP, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | Exp PLUS error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodePLUS = createNewNode("PLUS", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, $1, nodePLUS, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | Exp MINUS error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, $1, nodeMINUS, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | Exp STAR error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeSTAR = createNewNode("STAR", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, $1, nodeSTAR, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | Exp DIV error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Syntax error in Exp");
                struct Node* nodeDIV = createNewNode("DIV", NonValToken, @2.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, $1, nodeDIV, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | MINUS error {
            if (isNewError(@2.first_line)) {
                printError('B', @2.first_line, "Syntax error in Exp");
                struct Node* nodeMINUS = createNewNode("MINUS", NonValToken, @1.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @2.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 2, nodeMINUS, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | NOT error {
            if (isNewError(@2.first_line)) {
                printError('B', @2.first_line, "Syntax error in Exp");
                struct Node* nodeNOT = createNewNode("NOT", NonValToken, @1.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @2.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 2, nodeNOT, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    | LP Exp error {
            if (isNewError(@3.first_line)) {
                printError('B', @3.first_line, "Missing \")\"");
                struct Node* nodeLP = createNewNode("LP", NonValToken, @1.first_line);
                struct Node* nodeError = createNewNode("error", NonValToken, @3.first_line);              
                struct Node* nodeExp = createNewNode("Exp", NonTerm, @$.first_line);
                buildRel(nodeExp, 3, nodeLP, $2, nodeError);
                $$ = nodeExp;
            } else {
            	$$ = NULL;
            }
        }
    ;
Args : Exp COMMA Args {
            struct Node* nodeCOMMA = createNewNode("COMMA", NonValToken, @2.first_line);
            struct Node* nodeArgs = createNewNode("Args", NonTerm, @$.first_line);
            buildRel(nodeArgs, 3, $1, nodeCOMMA, $3);
            $$ = nodeArgs;
        }
    | Exp {
            struct Node* nodeArgs = createNewNode("Args", NonTerm, @$.first_line);
            nodeArgs->firstChild = $1;
            $$ = nodeArgs;
        }
    ;

%%
void yyerror(const char* s) { }

void printError(char errorType, int lineno, char* msg) {
    fprintf(stderr, "Error type %c at Line %d: %s.\n", errorType, lineno, msg);
}

int isNewError(int errorLineno) {
    if (lastErrorLineno != errorLineno) {
        errorNum++;
        lastErrorLineno = errorLineno;
        return 1;
    } else {
        return 0;
    }
}
