#include <stdio.h>
#include "lex.yy.c"

extern FILE* yyin;
//extern int yylex (void);

int main(int argc, char** argv) {
    // if (argc > 1) {
    //     if (!(yyin = fopen(argv[1], "r"))) {
    //         perror(argv[1]);
    //         return 1;
    //     }
    // }
    // while (yylex() != 0);
    // return 0;
    if (argc <= 1) 
        return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    return 0;
}