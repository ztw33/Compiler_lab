#ifndef __INTERCODE__
#define __INTERCODE__
#include <stdbool.h>

extern int tCount;
extern int vCount;
extern int labelID;

typedef struct InterCode_ InterCode;
typedef struct InterCodes_ InterCodes;
typedef struct Operand_ Operand;
typedef struct Variable_ Variable;
typedef struct CondExp_ CondExp;

typedef enum CodeKind { LABEL, FUNCTION, ASSIGN, ADD, SUB, MUL, DIV, GOTO, IF_GOTO, RETURN, DEC, ARG, CALL, PARAM, READ, WRITE } CodeKind;
struct InterCode_ {
    CodeKind kind;
    union {
        int labelID;  // LABEL
        char* funcName;  // FUNCTION
        struct { Operand* left; Operand* right; } assign;  // ASSIGN
        struct { Operand* result; Operand* op1; Operand* op2; } binOp;  // ADD, SUB, MUL, DIV
        int gotoLabelID;  // GOTO
        struct { CondExp* cond; int gotoLabelID; } if_goto;  // IF_GOTO
        Operand* retVal;  // RETURN
        struct { Variable* var; int size; } dec;  // DEC
        Operand* arg;  // ARG
        struct { Variable* ret; char* funcName; } call;  // CALL
        Variable* param;  // PARAM
        Operand* rwOperand;  // READ/WRITE
    };
};

struct InterCodes_ {
    InterCode* code;
    InterCodes* prev;
    InterCodes* next;
};

typedef enum OperandKind { VARIABLE, CONSTANT, REF, DEREF } OperandKind;
struct Operand_ {
    OperandKind kind;  // 变量/立即数/取址/取值
    union {
        Variable* var;  // VARIABLE
        int constVal;  // CONSTANT
        Operand* refObj;  // REF
        Operand* derefObj;  // DEREF
    };
};

typedef enum VarKind { T, V } VarKind;
struct Variable_ {
    VarKind kind;  // 临时变量/变量
    int id;
    bool isAddr;  // 是否表示一个地址，函数参数为结构体类型的形参此域即为true
};

typedef enum RelopKind { EQ, NEQ, LT, GT, LE, GE } RelopKind;
struct CondExp_ {
    Operand* op1;
    RelopKind relop;
    Operand* op2;
};

// 中间代码链表头尾
typedef struct CodesListHT {
    InterCodes* head;
    InterCodes* tail;
} CodesListHT;

int getVarID(VarKind kind);
int createNewLabel();
Variable* createVar(VarKind kind);
Operand* createOperand(OperandKind kind);
CondExp* createCondExp(Operand* op1, RelopKind relop, Operand* op2);
char* getVarName(Variable* var);
char* getOperandName(Operand* op);
char* getRelop(CondExp* cond);
void removeCode(InterCodes* code);

InterCode* createLABEL(int id);
InterCode* createFUNCTION(char* funcName);
InterCode* createASSIGN(Operand* left, Operand* right);
InterCode* createBinOp(Operand* result, CodeKind op, Operand* op1, Operand* op2);
InterCode* createGOTO(int labelID);
InterCode* createIFGOTO(CondExp* cond, int labelID);
InterCode* createRETURN(Operand* retVal);
InterCode* createDEC(Variable* var, int size);
InterCode* createARG(Operand* arg);
InterCode* createCALL(Variable* ret, char* funcName);
InterCode* createPARAM(Variable* param);
InterCode* createREAD(Operand* op);
InterCode* createWRITE(Operand* op);

void printCode(InterCode* code);

#endif