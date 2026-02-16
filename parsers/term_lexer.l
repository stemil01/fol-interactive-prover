%option prefix="term"
%option noyywrap
%option noinput
%option nounput

%{
#include "fol.hpp"
#include "term_parser.hpp"
%}

%%

[a-z][a-zA-Z_0-9]*       termlval.str_attr = new std::string(termtext); return SYMBOL;
[A-Z][a-zA-Z_0-9]*       termlval.str_attr = new std::string(termtext); return VARIABLE;
\(                       return *termtext;
\)                       return *termtext;
,			             return *termtext;
\"                       return *termtext;
[ \t\n]

%%

extern int termparse();

int parse_term_string(const std::string &input) {
    YY_BUFFER_STATE buffer = term_scan_string(input.c_str());
    int val = termparse();
    term_delete_buffer(buffer);
    termlex_destroy();
    return val;
}
