%option prefix="formula"
%option noyywrap
%option noinput
%option nounput

%{
#include "fol.hpp"
#include "formula_parser.hpp"
%}

%%

true                     return TRUE;
false                    return FALSE;
[a-z][a-zA-Z_0-9]*       formulalval.str_attr = new std::string(formulatext); return SYMBOL;
[A-Z][a-zA-Z_0-9]*       formulalval.str_attr = new std::string(formulatext); return VARIABLE;
\(                       return *formulatext;
\)                       return *formulatext;
,			             return *formulatext;
&                        return AND;
\|			             return OR;
-\>                      return IMP;
\<-\>                    return IFF;
~			             return NOT;
!			             return FORALL;
\?			             return EXISTS;
\.                       return *formulatext;
\"                       return *formulatext;
[ \t\n]

%%
