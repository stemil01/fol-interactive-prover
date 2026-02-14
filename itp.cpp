#include <cstdlib>
#include <ostream>
#include <sstream>
#include <string>

#include "itp.hpp"
#include "fol.hpp"
#include "nd.hpp"

std::set<std::string> ITP::m_available_rules = {
    "notI", "notE", "conjI", "conjunct1", "conjunct2", "disjI1", "disjI2", "disjE",
    "impI", "impE", "iffI", "iffE", "allI", "allE", "exI", "exE", "assumption",
};

void ITP::interactive_proof(FormulaPtr formula) {
    std::stack<Goal> goals;
    goals.push(Goal(std::set<Variable>(), std::set<FormulaPtr>(), formula));

    while (!goals.empty()) {
        print_goals(goals);
        Goal current_goal = goals.top();
    
        std::string rule = get_rule_from_user();
        if (rule == "EOF") {
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
        else if (rule == "REVERT") {
            std::cout << "Reverting to the previous state" << std::endl;
        }
        else {
            std::vector<Goal> generated_goals = ND::apply_rule(rule, current_goal);
            goals.pop();
            for (auto rit = generated_goals.rbegin(); rit != generated_goals.rend(); rit++) {
                goals.push(*rit);
            }
        }
    }
}

void ITP::print_goals(std::stack<Goal> goals) {
    int order = 1;
    while (!goals.empty()) {
        std::cout << order++ << ". " << goals.top();
        goals.pop();
    }
}

std::string ITP::get_rule_from_user() {
    std::string line;
    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, line)) {
            // End of input reached
            return "EOF";
        }

        std::istringstream iss(line);

        std::string command;
        iss >> command;

        if (command == "help") {
            std::cout << "available options:\n";
            std::cout << "\thelp\t\t - prints this help\n";
            std::cout << "\tapply RULE\t - apply the rule RULE on the first goal\n";
            std::cout << "\trules\t\t - prints all the available rules\n";
            std::cout << "\tdone\t\t - exit if all goals are met\n";
            std::cout << "\tclear\t\t - clear the screen\n";
            std::cout << "\trevert\t\t - revert the last appled rule" << std::endl;
        }
        else if (command == "rules") {
            std::cout << "\tnotI\t\t - negation introduction\n";
            std::cout << "\tnotE\t\t - negation elimination\n\n";

            std::cout << "\tconjI\t\t - conjunction introduction\n";
            std::cout << "\tconjunct1\t - conjunction elimination on the first conjunct\n";
            std::cout << "\tconjunct2\t - conjunction elimination on the second conjunct\n\n";

            std::cout << "\tdisjI1\t\t - disjunction introduction on the first disjunct\n";
            std::cout << "\tdisjI2\t\t - disjunction introduction on the second disjunct\n";
            std::cout << "\tdisjE\t\t - disjunction elimination\n\n";

            std::cout << "\timpI\t\t - implication introduction\n";
            std::cout << "\timpE\t\t - implication elimination\n\n";

            std::cout << "\tiffI\t\t - equivalence introduction\n";
            std::cout << "\tiffE\t\t - equivalence elimination\n\n";

            std::cout << "\tallI\t\t - universal quantifier introduction\n";
            std::cout << "\tallE\t\t - universal quantifier elimination\n\n";

            std::cout << "\texI\t\t - existential quantifier introduction\n";
            std::cout << "\texE\t\t - existential quantifier elimination\n\n";

            std::cout << "\tassumption\t - use the assumption on the left-hand side" << std::endl;
        }
        else if (command == "revert") {
            return "REVERT";
        }
        else if (command == "done") {
            std::cout << "done case" << std::endl;
        }
        else if (command == "clear") {
            clear_screen();
        }
        else if (command == "apply") {
            std::string rule;
            iss >> rule;
            if (!m_available_rules.contains(rule)) {
                std::cout << "\tUnknown rule. Run 'rules' to see the available rules." << std::endl;
            }
            else {
                return rule;
            }
        }
        else {
            std::cout << "\tCommand unknown. Run 'help' for instructions." << std::endl;
        }
    }
}

void ITP::clear_screen() {
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}

Goal::Goal(const std::set<Variable> &free_variables, const std::set<FormulaPtr> &lhs, FormulaPtr rhs)
    : m_free_variables(free_variables), m_lhs(lhs), m_rhs(rhs) {}

std::set<Variable> Goal::get_free_variables() const {
    return m_free_variables;
}

std::set<FormulaPtr> Goal::get_lhs() const {
    return m_lhs;
}

FormulaPtr Goal::get_rhs() const {
    return m_rhs;
}

void Goal::print(std::ostream &os) const {
    for (auto it = m_free_variables.begin(); it != m_free_variables.end(); it++) {
        os << it->name;
        it++;
        if (it != m_free_variables.end()) {
            os << " ";
        }
    }
    os << ": ";

    for (auto it = m_lhs.begin(); it != m_lhs.end(); it++) {
        os << *it;
        it++;
        if (it != m_lhs.end()) {
            os << ", ";
        }
    }
    os << " |- " << m_rhs << "\n";
}

std::ostream &operator<<(std::ostream &os, const Goal &goal) {
    goal.print(os);
    return os;
}