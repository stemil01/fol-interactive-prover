#include "itp.hpp"
#include "fol.hpp"
#include <cstdlib>
#include <sstream>
#include <string>

void ITP::interactive_proof(FormulaPtr formula) {
    std::stack<Goal> goals;
    goals.push(Goal(std::set<Variable>(), std::set<FormulaPtr>(), formula));

    while (!goals.empty()) {
        print_goals(goals);
        Goal current_goal = goals.top();
    
        process_user_input();
    }
}

void ITP::print_goals(std::stack<Goal> goals) {
    int order = 1;
    while (!goals.empty()) {
        std::cout << order++ << ". " << goals.top();
        goals.pop();
    }
}

void ITP::process_user_input() {
    std::string line;
    bool goals_changed = false;
    while (!goals_changed) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            // End of input reached
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }

        std::istringstream iss(line);

        std::string command;
        iss >> command;

        if (command == "help") {
            std::cout << "available options:\n";
            std::cout << "\thelp - prints this help\n";
            std::cout << "\tapply rule - prints all the available rules\n";
            std::cout << "\trules - prints all the available rules\n";
            std::cout << "\tdone - exit if all goals are met\n";
            std::cout << "\tclear - clear the screen\n";
            std::cout << "\trevert - revert the last appled rule" << std::endl;
        }
        else if (command == "rules") {
            std::cout << "printing rules" << std::endl;
        }
        else if (command == "revert") {
            std::cout << "reverting to a previous state" << std::endl;
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
            std::cout << "Applying " << rule << std::endl;
            goals_changed = true;
        }
        else {
            std::cout << "Command unknown. Run 'help' for instructions." << std::endl;
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