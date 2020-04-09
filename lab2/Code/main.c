#include "SyntaxTree.h"
#include "semantic.h"

extern FILE* yyin;
// extern int yydebug;
extern int yyparse (void);
extern void yyrestart (FILE *input_file  );
extern int errorNum;
extern struct Node* syntaxTreeRootNode;

int main(int argc, char** argv) {
    if (argc <= 1) 
        return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    // yydebug = 1;
    yyparse();
    if (errorNum == 0) {
        // printSyntaxTree(syntaxTreeRootNode);
        sementicAnalysis(syntaxTreeRootNode);
    }
    destroySyntaxTree(syntaxTreeRootNode);
    return 0;
}
