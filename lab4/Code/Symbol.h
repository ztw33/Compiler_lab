#ifndef __SYMBOL__
#define __SYMBOL__

#include <stdbool.h>
#include "InterCode.h"

typedef struct Type_ Type;
typedef struct Field_ Field;
typedef struct Symbol_ Symbol;
typedef struct Param_ Param;
typedef struct FuncSign_ FuncSign;
typedef struct SymbolList SymbolList;


struct Type_ {
    enum { BASIC, ARRAY, STRUCTURE } kind;
    bool Rvalue; // 是否为右值
    union {
        enum { INT, FLOAT } basic; // 基本类型
        struct { Type* elem; int size; } array; // 数组类型信息包括元素类型与数组大小构成
        struct { char* name; Field* fields; } structure; // 结构体类型信息包括结构体名和结构体域
    };
};

struct Field_ {
    char* name; // 域的名字
    Type* type; // 域的类型
    Field* next; // 下一个域
    int offset; // Lab3: 域在结构体中的偏移量
};

// 符号类型：变量(程序变量&函数形参)；结构体域；结构体定义；函数定义
// Lab3新增CONST: 常数, 避免同一个常数被重复赋值
typedef enum SymbolKind { VAR, FIELD, STRUCT, FUNC, CONST } SymbolKind;
struct Symbol_ {
    char* name;
    SymbolKind symbolKind;
    union {
        Type* type; // VAR & STRUCT & FIEID
        FuncSign* funcSign; // func
    };
    Variable* alias;  // just for VAR & CONST, 别名
};

struct Param_ {
    Type* type; // 参数的类型
    Param* next; // 下一个参数
};

struct FuncSign_ {
    Type* retType;
    int paramNum;
    Param* paramList; // 函数参数列表
};

struct SymbolList {
    Symbol* symbol;
    SymbolList* next;
};

Symbol* createSymbol(char* name, SymbolKind kind);

int getSizeofType(Type* type);
int getFieldOffset(Type* type, char* fieldName);

void printType(Type* type);
#endif