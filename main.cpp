#include <iostream>
#include "fol.hpp"

extern int yyparse();
extern int yylex_destroy();

FormulaPtr input_formula = nullptr;

int main() {
    if (yyparse() == 0) {
        print(input_formula);
        std::cout << std::endl;
    }

    yylex_destroy();

    return 0;
}
