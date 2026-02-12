#include <iostream>
#include "fol.hpp"

extern int yyparse();

FormulaPtr input_formula = nullptr;

int main() {
    if (yyparse() == 0) {
        print(input_formula);
        std::cout << std::endl;
    }

    return 0;
}
