#ifndef ND_HPP
#define ND_HPP

#include "itp.hpp"
#include <vector>

// Class representing Natural Deduction (ND) rules
class ND {
public:
    static std::vector<Goal> apply_rule(const std::string rule, const Goal &goal);

private:
    static std::vector<Goal> notI(const Goal &goal);
    static std::vector<Goal> notE(const Goal &goal);

    static std::vector<Goal> conjI(const Goal &goal);
    static std::vector<Goal> conjE(const Goal &goal);
    static std::vector<Goal> conjunct1(const Goal &goal);
    static std::vector<Goal> conjunct2(const Goal &goal);

    static std::vector<Goal> disjI1(const Goal &goal);
    static std::vector<Goal> disjI2(const Goal &goal);
    static std::vector<Goal> disjE(const Goal &goal);

    static std::vector<Goal> impI(const Goal &goal);
    static std::vector<Goal> impE(const Goal &goal);

    static std::vector<Goal> iffI(const Goal &goal);
    static std::vector<Goal> iffE(const Goal &goal);

    static std::vector<Goal> allI(const Goal &goal);
    static std::vector<Goal> allE(const Goal &goal);

    static std::vector<Goal> exI(const Goal &goal);
    static std::vector<Goal> exE(const Goal &goal);

    static std::vector<Goal> assumption(const Goal &goal);
};

#endif
