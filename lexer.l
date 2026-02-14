%option noyywrap
%option noinput
%option nounput

%{
#include "fol.hpp"
#include "parser.hpp"
%}

%%

true                     return TRUE;
false                    return FALSE;
[a-z][a-zA-Z_0-9]*       yylval.str_attr = new std::string(yytext); return SYMBOL;
[A-Z][a-zA-Z_0-9]*       yylval.str_attr = new std::string(yytext); return VARIABLE;
\(                       return *yytext;
\)                       return *yytext;
,			 return *yytext;
&                        return AND;
\|			 return OR;
=\>                      return IMP;
\<=\>                    return IFF;
~			 return NOT;
!			 return FORALL;
\?			 return EXISTS;
\.                       return *yytext;
;                        return *yytext;
\n                        return *yytext;
[ \t]

%%