#ifndef __SYNTAXTREE__
#define __SYNTAXTREE__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

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

struct Node* createNewNode(char* nodeName, enum NodeType nodeType, int lineNum);
void buildRel(struct Node* fatherNode, int childNodeNum, ...);
void printSyntaxTree(struct Node* rootNode);
void destroySyntaxTree(struct Node* rootNode);

#endif