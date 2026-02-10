#include <iostream>

extern int yyparse();

int main() {
    if (yyparse() == 0) {
        std::cout << "well-formed formula" << std::endl;
    }
    return 0;
}
