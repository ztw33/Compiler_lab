#include "SyntaxTree.h"
#include "semantic.h"
#include "IR.h"
#include "assembly.h"

extern FILE* yyin;
// extern int yydebug;
extern int yyparse (void);
extern void yyrestart (FILE *input_file  );
extern int errorNum;
extern struct Node* syntaxTreeRootNode;

int main(int argc, char** argv) {
    if (argc <= 2) 
        return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    // Lab1: 词法&语法分析
    yyrestart(f);
    // yydebug = 1;
    yyparse();
    if (errorNum == 0) {
        // printSyntaxTree(syntaxTreeRootNode);
        // Lab2: 语义分析
        sementicAnalysis(syntaxTreeRootNode);
        // Lab3: 生成中间代码
        InterCodes* codes = generateIR(syntaxTreeRootNode);
        // Lab4: 生成目标代码
        AsmCode* asmCodes = generateAsm(codes);
        FILE* ff = fopen(argv[2], "w");
        if (!ff) {
            perror(argv[2]);
            return 1;
        }
        // outputIR(ff, codes);
        outputAsm(ff, asmCodes);
        fclose(ff);
    }
    destroySyntaxTree(syntaxTreeRootNode);
    fclose(f);
    return 0;
}
