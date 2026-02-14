#ifndef ITP_HPP
#define ITP_HPP

#include <stack>
#include <set>

#include "fol.hpp"

class Goal;

// ITP - Interactive Theorem Prover
// Class representing the core behavior of the interactive prover.
class ITP {
public:
    static void interactive_proof(FormulaPtr formula);
    static void print_goals(std::stack<Goal> goals);

private:
    static std::string get_rule_from_user();
    static void clear_screen();
};

// Class representing a goal of an interactive proof
class Goal {
public:
    Goal();
    Goal(const std::set<Variable> &free_variables, const std::set<FormulaPtr> &lhs, FormulaPtr rhs);

    std::set<Variable> get_free_variables() const;
    std::set<FormulaPtr> get_lhs() const;
    FormulaPtr get_rhs() const;

    void print(std::ostream &os) const;

private:
    std::set<Variable> m_free_variables;
    std::set<FormulaPtr> m_lhs;
    FormulaPtr m_rhs;
};

std::ostream &operator<<(std::ostream &os, const Goal &goal);

// Class representing a change in the goals stack after an ND rule is applied
class GoalDiff {
public:
    GoalDiff(const std::vector<Goal> &added);

    void apply(std::stack<Goal> &goals);
    void revert(std::stack<Goal> &goals) const;

private:
    Goal m_removed;
    std::vector<Goal> m_added;
};

#endif
