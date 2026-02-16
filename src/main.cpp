#include "fol.hpp"
#include "itp.hpp"

extern int formulaparse();
extern int formulalex_destroy();

FormulaPtr input_formula = nullptr;

int main() {
    std::cout << "Formula: ";
    if (formulaparse() == 0) {
        ITP::interactive_proof(input_formula);
    }

    formulalex_destroy();

    return 0;
}
