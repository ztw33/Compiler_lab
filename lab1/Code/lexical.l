%{
    #include "syntax.tab.h"
%}
letter  [A-Za-z]
digit   [0-9]
INT     ("+"|"-")?(0|([1-9]{digit}*))
FLOAT   {INT}"."{digit}+
ID      ({letter}|_)({letter}|{digit}|_)*
RELOP   ">"|"<"|">="|"<="|"=="|"!="
delimeter   [ \t\n\r]
%option yylineno
%%
{RELOP} { printf("RELOP, %s\n", yytext); }
";"     { printf("SEMI\n"); }
","     { printf("COMMA\n"); }
"="     { printf("ASSIGNOP\n"); }
"+"     { printf("PLUS\n"); }
"-"     { printf("MINUS\n"); }
"*"     { printf("STAR\n"); }
"/"     { printf("DIV\n"); }
"&&"    { printf("AND\n"); }
"||"    { printf("OR\n"); }
"."     { printf("DOT\n"); }
"!"     { printf("NOT\n"); }
"("     { printf("LP\n"); }
")"     { printf("RP\n"); }
"["     { printf("LB\n"); }
"]"     { printf("RB\n"); }
"{"     { printf("LC\n"); }
"}"     { printf("RC\n"); }
{INT}   { printf("int, %d\n", atoi(yytext)); }
{FLOAT} { printf("float, %f\n", atof(yytext)); }
"if"    { printf("IF\n"); }
"else"  { printf("ELSE\n"); }
"while" { printf("WHILE\n"); }
"int"|"float"   { printf("TYPE, %s\n", yytext); }
"struct"    { printf("STRUCT\n"); }
"return"    { printf("RETURN\n"); }
"//"    { 
            char c = input();
            while (c != '\n') c = input();
        }
"/*"    { 
            printf("block comment start\n"); 
            char c = input();
            while (1) {
                if (c == '*') {
                    c = input();
                    if (c == '/')
                        break;
                }
                c = input();
            }
        }
{ID}    { printf("id, %s\n", yytext); }
{delimeter}    { }
.       { printf("Error type A at Line %d: Mysterious character \"%s\".\n", yylineno, yytext); }
%%