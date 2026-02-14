#include <stdexcept>
#include <vector>

#include "nd.hpp"
#include "fol.hpp"

std::vector<Goal> ND::apply_rule(const std::string rule, const Goal &goal) {
    if (rule == "notI") {
        return notI(goal);
    }
    else if (rule == "notE") {
        return notE(goal);
    }
    else if (rule == "conjI") {
        return conjI(goal);
    }
    else if (rule == "conjunct1") {
        return conjunct1(goal);
    }
    else if (rule == "conjunct2") {
        return conjunct2(goal);
    }
    else if (rule == "disjI1") {
        return disjI1(goal);
    }
    else if (rule == "disjI2") {
        return disjI2(goal);
    }
    else if (rule == "disjE") {
        return disjE(goal);
    }
    else if (rule == "impI") {
        return impI(goal);
    }
    else if (rule == "impE") {
        return impE(goal);
    }
    else if (rule == "iffI") {
        return iffI(goal);
    }
    else if (rule == "iffE") {
        return iffE(goal);
    }
    else if (rule == "allI") {
        return allI(goal);
    }
    else if (rule == "allE") {
        return allE(goal);
    }
    else if (rule == "exI") {
        return exI(goal);
    }
    else if (rule == "exE") {
        return exE(goal);
    }
    else if (rule == "assumption") {
        return assumption(goal);
    }
    else {
        throw std::runtime_error("\tUnknown rule. Run 'rules' to see the available rules.");
    }
}

std::vector<Goal> ND::notI(const Goal &goal) {
    FormulaPtr rhs = goal.get_rhs();

    if (!is<Not>(rhs)) {
        throw std::runtime_error("Failed to apply rule notI.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    std::set<FormulaPtr> lhs = goal.get_lhs();
    FormulaPtr subformula = as<Not>(rhs).subformula;
    lhs.insert(subformula);
    rhs = falseConst();

    return {Goal{free_variables, lhs, rhs}};
}

std::vector<Goal> ND::notE(const Goal &goal) {
    std::set<FormulaPtr> lhs = goal.get_lhs();
    FormulaPtr rhs = goal.get_rhs();

    if (rhs != falseConst()) {
        throw std::runtime_error("Failed to apply rule notE.");
    }

    // We search for the first occurence of a negation formula
    // on the left hand side of the current goal. However, this
    // is not always sufficient, and the user should be able to
    // specify the negated formula in the general case.
    FormulaPtr not_formula;
    bool found_not = false;
    for (auto it = lhs.begin(); !found_not && it != lhs.end(); it++) {
        if (is<Not>(*it)) {
            not_formula = *it;
            found_not = true;
        }
    }
    if (!found_not) {
        throw std::runtime_error("Failed to apply rule notE.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    lhs.erase(not_formula);
    rhs = as<Not>(not_formula).subformula;

    return {Goal{free_variables, lhs, rhs}};
}

std::vector<Goal> ND::conjI(const Goal &goal) {
    FormulaPtr rhs = goal.get_rhs();

    if (!is<Binary>(rhs)) {
        throw std::runtime_error("Failed to apply rule conjI.");
    }

    Binary bin = as<Binary>(rhs);
    if (bin.type != Binary::And) {
        throw std::runtime_error("Failed to apply rule conjI.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    std::set<FormulaPtr> lhs = goal.get_lhs();

    return {
        {Goal{free_variables, lhs, bin.l}},
        {Goal{free_variables, lhs, bin.r}}
    };
}

std::vector<Goal> ND::conjunct1(const Goal &goal) {
    std::set<FormulaPtr> lhs = goal.get_lhs();

    // We search for the first occurence of a conjunction formula
    // on the left hand side of the current goal. However, this
    // is not always sufficient, and the user should be able to
    // specify the conjunction formula in the general case.
    FormulaPtr conj_formula;
    bool found_conj = false;
    for (auto it = lhs.begin(); !found_conj && it != lhs.end(); it++) {
        if (is<Binary>(*it) && as<Binary>(*it).type == Binary::And) {
            conj_formula = *it;
            found_conj = true;
        }
    }
    if (!found_conj) {
        throw std::runtime_error("Failed to apply rule conjunct1.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    lhs.erase(conj_formula);
    lhs.insert(as<Binary>(conj_formula).l);
    FormulaPtr rhs = goal.get_rhs();

    return {Goal{free_variables, lhs, rhs}};
}

std::vector<Goal> ND::conjunct2(const Goal &goal) {
    std::set<FormulaPtr> lhs = goal.get_lhs();

    // We search for the first occurence of a conjunction formula
    // on the left hand side of the current goal. However, this
    // is not always sufficient, and the user should be able to
    // specify the conjunction formula in the general case.
    FormulaPtr conj_formula;
    bool found_conj = false;
    for (auto it = lhs.begin(); !found_conj && it != lhs.end(); it++) {
        if (is<Binary>(*it) && as<Binary>(*it).type == Binary::And) {
            conj_formula = *it;
            found_conj = true;
        }
    }
    if (!found_conj) {
        throw std::runtime_error("Failed to apply rule conjunct2.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    lhs.erase(conj_formula);
    lhs.insert(as<Binary>(conj_formula).r);
    FormulaPtr rhs = goal.get_rhs();

    return {Goal{free_variables, lhs, rhs}};
}

std::vector<Goal> ND::disjI1(const Goal &goal) {
    FormulaPtr rhs = goal.get_rhs();

    if (!is<Binary>(rhs)) {
        throw std::runtime_error("Failed to apply rule disjI1.");
    }

    Binary bin = as<Binary>(rhs);
    if (bin.type != Binary::Or) {
        throw std::runtime_error("Failed to apply rule disjI1.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    std::set<FormulaPtr> lhs = goal.get_lhs();

    return {Goal{free_variables, lhs, bin.l}};
}

std::vector<Goal> ND::disjI2(const Goal &goal) {
    FormulaPtr rhs = goal.get_rhs();

    if (!is<Binary>(rhs)) {
        throw std::runtime_error("Failed to apply rule disjI2.");
    }

    Binary bin = as<Binary>(rhs);
    if (bin.type != Binary::Or) {
        throw std::runtime_error("Failed to apply rule disjI2.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    std::set<FormulaPtr> lhs = goal.get_lhs();

    return {Goal{free_variables, lhs, bin.r}};
}

std::vector<Goal> ND::disjE(const Goal &goal) {
    std::set<FormulaPtr> lhs = goal.get_lhs();

    // We search for the first occurence of a disjunction formula
    // on the left hand side of the current goal. However, this
    // is not always sufficient, and the user should be able to
    // specify the disjunction formula in the general case.
    FormulaPtr disj_formula;
    bool found_disj = false;
    for (auto it = lhs.begin(); !found_disj && it != lhs.end(); it++) {
        if (is<Binary>(*it) && as<Binary>(*it).type == Binary::Or) {
            disj_formula = *it;
            found_disj = true;
        }
    }
    if (!found_disj) {
        throw std::runtime_error("Failed to apply rule disjE.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    lhs.erase(disj_formula);
    std::set<FormulaPtr> lhs_l = lhs;
    lhs_l.insert(as<Binary>(disj_formula).l);
    std::set<FormulaPtr> lhs_r = lhs;
    lhs_r.insert(as<Binary>(disj_formula).r);
    FormulaPtr rhs = goal.get_rhs();

    return {
        Goal{free_variables, lhs_l, rhs},
        Goal{free_variables, lhs_r, rhs}
    };
}

std::vector<Goal> ND::impI(const Goal &goal) {
    FormulaPtr rhs = goal.get_rhs();

    if (!is<Binary>(rhs)) {
        throw std::runtime_error("Failed to apply rule impI.");
    }

    Binary impl = as<Binary>(rhs);
    if (impl.type != Binary::Impl) {
        throw std::runtime_error("Failed to apply rule impI.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    std::set<FormulaPtr> lhs = goal.get_lhs();
    lhs.insert(impl.l);
    rhs = impl.r;

    return {Goal{free_variables, lhs, rhs}};
}

std::vector<Goal> ND::impE(const Goal &goal) {
    std::set<FormulaPtr> lhs = goal.get_lhs();
    FormulaPtr rhs = goal.get_rhs();

    // We search for the first occurence of a implication formula
    // on the left hand side of the current goal. However, this
    // is not always sufficient, and the user should be able to
    // specify the implication formula in the general case.
    FormulaPtr impl_formula;
    bool found_impl = false;
    for (auto it = lhs.begin(); !found_impl && it != lhs.end(); it++) {
        if (is<Binary>(*it) && as<Binary>(*it).type == Binary::Impl && as<Binary>(*it).r == rhs) {
            impl_formula = *it;
            found_impl = true;
        }
    }
    if (!found_impl) {
        throw std::runtime_error("Failed to apply rule impE.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    lhs.erase(impl_formula);
    rhs = as<Binary>(impl_formula).l;

    return {Goal{free_variables, lhs, rhs}};
}

std::vector<Goal> ND::iffI(const Goal &goal) {
    FormulaPtr rhs = goal.get_rhs();

    if (!is<Binary>(rhs)) {
        throw std::runtime_error("Failed to apply rule iffI.");
    }

    Binary equiv = as<Binary>(rhs);
    if (equiv.type != Binary::Eq) {
        throw std::runtime_error("Failed to apply rule iffI.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();
    std::set<FormulaPtr> lhs = goal.get_lhs();

    FormulaPtr rhs1 = ptr(Binary{Binary::Impl, equiv.l, equiv.r});
    FormulaPtr rhs2 = ptr(Binary{Binary::Impl, equiv.r, equiv.l});

    return {
        Goal{free_variables, lhs, rhs1},
        Goal{free_variables, lhs, rhs2}
    };
}

std::vector<Goal> ND::iffE(const Goal &goal) {
    std::set<FormulaPtr> lhs = goal.get_lhs();

    // We search for the first occurence of a implication formula
    // on the left hand side of the current goal. However, this
    // is not always sufficient, and the user should be able to
    // specify the implication formula in the general case.
    FormulaPtr equiv_formula;
    bool found_equiv = false;
    for (auto it = lhs.begin(); !found_equiv && it != lhs.end(); it++) {
        if (is<Binary>(*it) && as<Binary>(*it).type == Binary::Eq) {
            equiv_formula = *it;
            found_equiv = true;
        }
    }
    if (!found_equiv) {
        throw std::runtime_error("Failed to apply rule iffE.");
    }

    std::set<Variable> free_variables = goal.get_free_variables();

    Binary bin = as<Binary>(equiv_formula);
    lhs.erase(equiv_formula);
    lhs.insert(ptr(Binary{Binary::Impl, bin.l, bin.r}));
    lhs.insert(ptr(Binary{Binary::Impl, bin.r, bin.l}));
    FormulaPtr rhs = goal.get_rhs();

    return {Goal{free_variables, lhs, rhs}};
}

std::vector<Goal> ND::allI(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::allE(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::exI(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::exE(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::assumption(const Goal &goal) {
    std::set<FormulaPtr> lhs = goal.get_lhs();
    FormulaPtr rhs = goal.get_rhs();

    for (auto it = lhs.begin(); it != lhs.end(); it++) {
        if (*it == rhs) {
            return {};
        }
    }

    throw std::runtime_error("Failed to apply rule assumption.");
}
