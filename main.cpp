#include "fol.hpp"
#include "itp.hpp"

extern int yyparse();
extern int yylex_destroy();

FormulaPtr input_formula = nullptr;

int main() {
    if (yyparse() == 0) {
        ITP::interactive_proof(input_formula);
    }

    yylex_destroy();

    return 0;
}
