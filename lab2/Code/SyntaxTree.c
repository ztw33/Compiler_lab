#include "SyntaxTree.h"

struct Node* createNewNode(char* nodeName, enum NodeType nodeType, int lineNum) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->nodeName = nodeName;
    newNode->nodeType = nodeType;
    newNode->lineNum = lineNum;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

void buildRel(struct Node* fatherNode, int childNodeNum, ...) {
    va_list valist;
    va_start(valist, childNodeNum);
    struct Node* firstChild = NULL;
    struct Node* lastChild = NULL;
    for (int i = 0; i < childNodeNum; i++) {
        struct Node* curNode = va_arg(valist, struct Node*);
        if (firstChild == NULL) {
            if (curNode != NULL) {
                firstChild = curNode;
                lastChild = firstChild;
            }
        } else {
            if (curNode != NULL) {
                lastChild->nextSibling = curNode;
                lastChild = curNode;
            }
        }
    }
    va_end(valist);
    fatherNode->firstChild = firstChild;
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

void destroySyntaxTree(struct Node* rootNode) {
    if (rootNode == NULL) {
        return;
    }
    struct Node* curNode = rootNode->firstChild;
    struct Node* nextNode = NULL;
    while (curNode != NULL) {
        nextNode = curNode->nextSibling;
        destroySyntaxTree(curNode);
        curNode = nextNode;
    }
    if (rootNode->nodeName != NULL && ((strcmp(rootNode->nodeName, "TYPE") == 0) || (strcmp(rootNode->nodeName, "ID") == 0))) {
        free(rootNode->stringVal);
        rootNode->stringVal = NULL;
    }
    free(rootNode);
    rootNode = NULL;
}