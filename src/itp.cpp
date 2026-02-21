#include <cstdlib>
#include <exception>
#include <ostream>
#include <sstream>
#include <string>

#include "itp.hpp"
#include "fol.hpp"
#include "nd.hpp"

TermPtr input_term = nullptr;

void ITP::interactive_proof(FormulaPtr formula) {
    std::stack<Goal> goals;
    std::stack<GoalDiff> diffs;
    goals.push(Goal(std::set<std::string>(), std::set<FormulaPtr>(), formula));
    print_goals(goals);

    while (!goals.empty()) {
        Goal current_goal = goals.top();
    
        std::string command = get_command_from_user();
        if (command == "EOF" || command == "EXIT") {
            std::cout << std::endl;
            break;
        }
        else if (command == "GOALS") {
            print_goals(goals);
        }
        else if (command == "REVERT") {
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
                GoalDiff goal_diff(ND::apply_rule(command, current_goal, input_term));
                goal_diff.apply(goals);
                diffs.push(goal_diff);
            }
            catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    if (goals.empty()) {
        std::cout << "All goals resolved.\nProof complete." << std::endl;
    }
}

void ITP::print_goals(std::stack<Goal> goals) {
    int order = 1;
    while (!goals.empty()) {
        std::cout << order++ << ". " << goals.top();
        goals.pop();
    }
}

std::string ITP::get_command_from_user() {
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
            std::cout << "\thelp\t\t\t - print this help\n";
            std::cout << "\texit\t\t\t - exit the interactive console\n";
            std::cout << "\trules\t\t\t - print all the available rules\n";
            std::cout << "\tgoals\t\t\t - print the remaining goals\n";
            std::cout << "\trevert\t\t\t - revert to the state before the last rule was applied\n";
            std::cout << "\tclear\t\t\t - clear the screen\n";
            std::cout << "\tapply RULE [\"TERM\"]\t - apply the rule RULE with an optional TERM substitution\n";
            std::cout << "\t\t\t\t   on the first goal" << std::endl;
        }
        else if (command == "exit") {
            return "EXIT";
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
            std::cout << "\tallE\t\t - universal quantifier elimination, requires a term for substitution\n\n";

            std::cout << "\texI\t\t - existential quantifier introduction, requires a term for substitution\n";
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
            std::string rule, term;
            iss >> rule;

            if (rule != "allE" && rule != "exI") {
                return rule;
            }

            std::getline(iss >> std::ws, term);
            if (parse_term_string(term) == 0) {
                return rule;
            }

            std::cout << "\nFaileld to parse the required term for the rule " << rule << ".\n";
            std::cout << "\tUsage: apply " << rule << " \"TERM\"" << std::endl;
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

Goal::Goal(const std::set<std::string> &arbitrary_vars, const std::set<FormulaPtr> &lhs, FormulaPtr rhs)
    : m_arbitrary_vars(arbitrary_vars), m_lhs(lhs), m_rhs(rhs) {}

std::set<std::string> Goal::get_arbitrary_vars() const {
    return m_arbitrary_vars;
}

std::set<FormulaPtr> Goal::get_lhs() const {
    return m_lhs;
}

FormulaPtr Goal::get_rhs() const {
    return m_rhs;
}

std::string Goal::unused_variable(const std::string &base_var) const {
    std::set<std::string> used_variables = m_arbitrary_vars;
    for (const FormulaPtr &f : m_lhs) {
        getVariables(f, used_variables, false);
    }
    getVariables(m_rhs, used_variables, false);

    std::string var = base_var;
    int num = 1;
    while (used_variables.contains(var)) {
        var = base_var + std::to_string(num);
        num++;
    }
    return var;
}

void Goal::print(std::ostream &os) const {
    auto fv_it = m_arbitrary_vars.begin();
    while (fv_it != m_arbitrary_vars.end()) {
        os << *fv_it;
        fv_it++;
        if (fv_it != m_arbitrary_vars.end()) {
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
