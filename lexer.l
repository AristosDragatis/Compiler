%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *outfile;
FILE *input_;
char *yytext;

#define T_num 1
#define T_id 2
#define T_plus 3
#define T_minus 4
#define T_mul 5
#define T_div 6
#define T_asign 7
#define T_neq 8
#define T_pos_inf 9
#define T_neg_inf 10
#define T_empty 11
#define T_pi 12
#define T_two_pi 13
#define T_half_pi 14
#define T_all_reals 15
#define T_pos_reals 16
#define T_neg_reals 17
#define T_contains 18
#define T_intersects 19
#define T_disjoint 20
#define T_isempty 21
#define T_is_subset 22
#define T_is_strict_subset 23
#define T_is_superset 24
#define T_is_strict_superset 25
#define T_lparen 26
#define T_rparen 27
#define T_lbrace 28
#define T_rbrace 29
#define T_semi 30
#define T_comma 31
#define T_lbracket 32
#define T_rbracket 33
#define T_eq 34
#define T_interval 35
#define T_IntervalVector 36
#define T_double 37
#define T_float 38
#define T_int 39

%}
%option noyywrap

DIGIT      [0-9]
ID         [a-zA-Z_][a-zA-Z0-9_]*
PLUS       \+
MINUS      -
MULTIPLY   \*
DIVIDE     \/
ASIGN      =
EQUALS     (==)
NEQUAL     !=
POS_INF    POS_INFINITY
NEG_INF    NEG_INFINITY
EMPTY_SET  EMPTY_SET
PI         PI
ZERO       ZERO
ONE        ONE
TWO_PI     TWO_PI
HALF_PI    HALF_PI
ALL_REALS  ALL_REALS
POS_REALS  POS_REALS
NEG_REALS  NEG_REALS
CONTAINS   contains
INTERSECTS intersects
DISJOINT   is_disjoint
ISEMPTY    is_empty
ISUBSET    is_subset
ISSUBSET   is_strict_subset
ISUPERSET  is_superset
ISSUPERSET is_strict_superset
LPAREN     \(
RPAREN     \)
LBRACE     \{
RBRACE     \}
LBRACKET   \[
RBRACKET   \]
SEMI       ;
COMMA      ,
INTERVAL   Interval
INTERVALVECTOR      IntervalVector
DOUBLE     double
INT        int
FLOAT      float


%%
{DOUBLE}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_double "); return T_double; }
{INT}                    { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_int "); return T_int; }
{FLOAT}                  { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_float "); return T_float; }
{INTERVAL}               { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_interval "); return T_interval; }
{INTERVALVECTOR}         { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_IntervalVector "); return T_IntervalVector;}
{DIGIT}+(\.({DIGIT}+))?   { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_num "); return T_num; }
{PLUS}                   { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_plus "); return T_plus; }
{MINUS}                  { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_minus "); return T_minus; }
{MULTIPLY}               { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_mul "); return T_mul; }
{DIVIDE}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_div "); return T_div; }
{ASIGN}                  { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_asign "); return T_asign; }
{EQUALS}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_eq "); return T_eq; }
{NEQUAL}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_neq "); return T_neq; }
{POS_INF}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_pos_inf "); return T_pos_inf; }
{NEG_INF}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_neg_inf "); return T_neg_inf; }
{EMPTY_SET}             { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_empty "); return T_empty; }
{PI}                     { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_pi "); return T_pi; }
{TWO_PI}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_two_pi "); return T_two_pi; }
{HALF_PI}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_half_pi "); return T_half_pi; }
{ALL_REALS}             { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_all_reals "); return T_all_reals; }
{POS_REALS}             { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_pos_reals "); return T_pos_reals; }
{NEG_REALS}             { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_neg_reals "); return T_neg_reals; }
{CONTAINS}               { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_contains "); return T_contains; }
{INTERSECTS}             { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_intersects "); return T_intersects; }
{DISJOINT}               { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_disjoint "); return T_disjoint; }
{ISEMPTY}               { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_isempty "); return T_isempty; }
{ISUBSET}               { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_is_subset "); return T_is_subset; }
{ISSUBSET}              { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_is_strict_subset "); return T_is_strict_subset; }
{ISUPERSET}             { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_is_superset "); return T_is_superset; }
{ISSUPERSET}            { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_is_strict_superset "); return T_is_strict_superset; }
{LPAREN}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_lparen "); return T_lparen; }
{RPAREN}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_rparen "); return T_rparen; }
{LBRACE}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_lbrace "); return T_lbrace; }
{RBRACE}                 { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_rbrace "); return T_rbrace; }
{LBRACKET}               { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_lbracket "); return T_lbracket; }
{RBRACKET}               { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_rbracket "); return T_rbracket; }
{SEMI}                   { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_semi "); return T_semi; }
{COMMA}                  { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_comma "); return T_comma; }
{ID}                     { fprintf(input_, "%s ", yytext); fprintf(outfile, "T_id "); return T_id; }
[ \t\n]+                 {  }
.                        { fprintf(stderr, "Unknown character: %s\n", yytext); }
%%


int main() {
    outfile = fopen("tokens.txt", "w");
    if (!outfile) {
        fprintf(stderr, "Could not open tokens.txt for writing\n");
        return 1;
    }
    input_ = fopen("input_.txt", "w");
    if(!input){
        fprintf(stderr, "Could not open input.txt for writing\n");
        return 1;
    }

    while (1) {
        int token = yylex();
        if (token == 0) break;
        printf("Input: %s , Token: %d\n",yytext,  token);
    }

    fclose(outfile);
    fclose(input_);
    return 0;
}
