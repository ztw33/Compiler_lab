#include "SyntaxTree.h"

typedef struct Node Node;
typedef struct Type_ Type;
typedef struct Field_ Field;

struct Type_ {
    enum { BASIC, ARRAY, STRUCTURE } kind;
    union {
        enum { INT, FLOAT } basic; // 基本类型
        struct { Type* elem; int size; } array; // 数组类型信息包括元素类型与数组大小构成
        Field* structure; // 结构体类型信息是一个链表
    };
};

struct Field_ {
    char* name; // 域的名字
    Type* type; // 域的类型
    Field* next; // 下一个域
};


void sementicAnalysis(const Node* syntaxTreeRootNode);

void analyseProgram(const Node* Program);
void analyseExtDefList(const Node* ExtDefList);
void analyseExtDef(const Node* ExtDef);
Type* analyseSpecifier(const Node* Specifier);
Type* analyseTYPE(const Node* TYPE);