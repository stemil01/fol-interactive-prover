#include "fol.hpp"
#include "itp.hpp"

extern int formulaparse();
extern int formulalex_destroy();

FormulaPtr input_formula = nullptr;

int main() {
    std::cout << "Enter the goal formula in quotes." << std::endl;
    std::cout << "Formula: ";
    if (formulaparse() == 0) {
        ITP::interactive_proof(input_formula);
    }

    formulalex_destroy();

    return 0;
}
