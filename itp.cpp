#include "itp.hpp"
#include "fol.hpp"
#include <string>

void ITP::interactive_proof(FormulaPtr formula) {
    std::stack<Goal> goals;
    goals.push(Goal(std::set<Variable>(), std::set<FormulaPtr>(), formula));

    while (!goals.empty()) {
        print_goals(goals);
        Goal current_goal = goals.top();
        goals.pop();
    
        std::string rule_to_apply = get_user_input();
        std::cout << rule_to_apply << std::endl;
    }
}

void ITP::print_goals(std::stack<Goal> goals) {
    int order = 1;
    while (!goals.empty()) {
        std::cout << order++ << ". " << goals.top();
        goals.pop();
    }
}

std::string ITP::get_user_input() {
    std::string line;
    std::getline(std::cin, line);
    return line.substr(line.find(' ') + 1, line.size());
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