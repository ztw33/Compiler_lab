%{
    #include "syntax.tab.h"

    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
	    yylloc.first_column = yycolumn; \
	    yylloc.last_column = yycolumn + yyleng - 1; \
	    yycolumn += yyleng;
    
    int print_lex = 0;
    extern void printError(char errorType, int lineno, char* msg);
    extern int isNewError(int errorLineno);
%}
/* 对于ID，先不设置其必须为下划线或者字母开头，识别出来后再去判断 */
letter  [A-Za-z]
digit   [0-9]
INT     0|([1-9]{digit}*)
FLOAT   {INT}"."{digit}+
ID      ({letter}|{digit}|_)+
RELOP   ">"|"<"|">="|"<="|"=="|"!="
blank   [ \t\r]
%option yylineno
%%
{RELOP} { 
            if (print_lex) printf("RELOP, %s\n", yytext);
            yylval.type_string = strdup(yytext);
            return RELOP; }
";"     { if (print_lex) printf("SEMI\n"); return SEMI; }
","     { if (print_lex) printf("COMMA\n"); return COMMA; }
"="     { if (print_lex) printf("ASSIGNOP\n"); return ASSIGNOP; }
"+"     { if (print_lex) printf("PLUS\n"); return PLUS; }
"-"     { if (print_lex) printf("MINUS\n"); return MINUS; }
"*"     { if (print_lex) printf("STAR\n"); return STAR; }
"/"     { if (print_lex) printf("DIV\n"); return DIV; }
"&&"    { if (print_lex) printf("AND\n"); return AND; }
"||"    { if (print_lex) printf("OR\n"); return OR; }
"."     { if (print_lex) printf("DOT\n"); return DOT; }
"!"     { if (print_lex) printf("NOT\n"); return NOT; }
"("     { if (print_lex) printf("LP\n"); return LP; }
")"     { if (print_lex) printf("RP\n"); return RP; }
"["     { if (print_lex) printf("LB\n"); return LB; }
"]"     { if (print_lex) printf("RB\n"); return RB; }
"{"     { if (print_lex) printf("LC\n"); return LC; }
"}"     { if (print_lex) printf("RC\n"); return RC; }
{INT}   { 
            if (print_lex) printf("int, %d\n", atoi(yytext)); 
            yylval.type_int = atoi(yytext);
            return INT;
        }
{FLOAT} { 
            if (print_lex) printf("float, %f\n", atof(yytext)); 
            yylval.type_float = atof(yytext);
            return FLOAT;
        }
"if"    { if (print_lex) printf("IF\n"); return IF; }
"else"  { if (print_lex) printf("ELSE\n"); return ELSE; }
"while" { if (print_lex) printf("WHILE\n"); return WHILE; }
"int"|"float"   { if (print_lex) printf("TYPE, %s\n", yytext); 
                    yylval.type_string = strdup(yytext);
                    return TYPE; 
                }
"struct"    { if (print_lex) printf("STRUCT\n"); return STRUCT; }
"return"    { if (print_lex) printf("RETURN\n"); return RETURN; }
"//"    { 
            char c = input();
            while (c != '\n') c = input();
        }
"/*"    {
            char c = input();
            while (1) {
                if (c == '*') {
                    c = input();
                    if (c == '/')
                        break;
                    else
                    	unput(c);
                } else if (c == EOF) {
                    if (isNewError(yylineno)) {
                        printError('A', yylineno, "Unmatched \"/*\"");
                    }
                    break;
                }
                c = input();
            }
        }
"*/"    {
            if (isNewError(yylineno)) {
                printError('A', yylineno, "Unmatched \"*/\"");
            }
        }
{ID}    { if (print_lex) printf("id, %s\n", yytext);
            if (yytext[0] >= '0' && yytext[0] <= '9') {
                if (isNewError(yylineno)) {
                    printError('A', yylineno, "Illegal ID");
                }
            }
            yylval.type_string = strdup(yytext);
            return ID; 
        }
\n      { yycolumn = 1; }
{blank}    { }
.       { 
            if (isNewError(yylineno)) {
                char msg[32];
                sprintf(msg, "Mysterious character \"%s\"", yytext);
                printError('A', yylineno, msg);
            }
        }
%%
