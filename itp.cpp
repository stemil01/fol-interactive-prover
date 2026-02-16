#include <cstdlib>
#include <exception>
#include <ostream>
#include <sstream>
#include <string>

#include "itp.hpp"
#include "fol.hpp"
#include "nd.hpp"

void ITP::interactive_proof(FormulaPtr formula) {
    std::stack<Goal> goals;
    std::stack<GoalDiff> diffs;
    goals.push(Goal(std::set<Variable>(), std::set<FormulaPtr>(), formula));
    print_goals(goals);

    while (!goals.empty()) {
        Goal current_goal = goals.top();
    
        std::string rule = get_rule_from_user();
        if (rule == "EOF") {
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
        else if (rule == "GOALS") {
            print_goals(goals);
        }
        else if (rule == "REVERT") {
            if (diffs.empty()) {
                std::cout << "No previous state exists." << std::endl;
            }
            else {
                GoalDiff last_diff = diffs.top();
                diffs.pop();
                last_diff.revert(goals);
            }
        }
        else {
            try {
                GoalDiff goal_diff(ND::apply_rule(rule, current_goal));
                goal_diff.apply(goals);
                diffs.push(goal_diff);
            }
            catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    std::cout << "All goals resolved.\nProof complete." << std::endl;
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
            std::cout << "\trules\t\t - prints all the available rules\n";
            std::cout << "\tgoals\t\t - print the remaining goals\n";
            std::cout << "\trevert\t\t - revert the last appled rule\n";
            std::cout << "\tclear\t\t - clear the screen\n";
            std::cout << "\tapply RULE\t - apply the rule RULE on the first goal" << std::endl;
        }
        else if (command == "rules") {
            std::cout << "\tnotI\t\t - negation introduction\n";
            std::cout << "\tnotE\t\t - negation elimination\n\n";

            std::cout << "\tconjI\t\t - conjunction introduction\n";
            std::cout << "\tconjE\t\t - conjunction elimination\n";
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
        else if (command == "goals") {
            return "GOALS";
        }
        else if (command == "revert") {
            return "REVERT";
        }
        else if (command == "clear") {
            clear_screen();
        }
        else if (command == "apply") {
            std::string rule;
            iss >> rule;
            return rule;
        }
        else if (command != "") {
            std::cout << "\tCommand unknown. Run 'help' for instructions." << std::endl;
        }
    }
}

void ITP::clear_screen() {
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}

Goal::Goal() {}

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
    auto fv_it = m_free_variables.begin();
    while (fv_it != m_free_variables.end()) {
        os << fv_it->name;
        fv_it++;
        if (fv_it != m_free_variables.end()) {
            os << " ";
        }
    }
    os << ": ";

    auto l_it = m_lhs.begin();
    while (l_it != m_lhs.end()) {
        os << *l_it;
        l_it++;
        if (l_it != m_lhs.end()) {
            os << ", ";
        }
    }
    os << " |- " << m_rhs << "\n";
}

std::ostream &operator<<(std::ostream &os, const Goal &goal) {
    goal.print(os);
    return os;
}

GoalDiff::GoalDiff(const std::vector<Goal> &added) : m_added(added) {}

void GoalDiff::apply(std::stack<Goal> &goals) {
    m_removed = goals.top();
    goals.pop();

    for (auto rit = m_added.rbegin(); rit != m_added.rend(); rit++) {
        goals.push(*rit);
    }

    ITP::print_goals(goals);
}

void GoalDiff::revert(std::stack<Goal> &goals) const {
    for (const Goal &_ : m_added) {
        goals.pop();
    }

    goals.push(m_removed);
    ITP::print_goals(goals);
}
