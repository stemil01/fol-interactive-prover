#include "nd.hpp"
#include <cstdlib>
#include <vector>

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
        std::cerr << "Unknown rule (" << rule << ") found in ND class. Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<Goal> ND::notI(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::notE(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::conjI(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::conjunct1(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::conjunct2(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::disjI1(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::disjI2(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::disjE(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::impI(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::impE(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::iffI(const Goal &goal) {
    return {goal};
}

std::vector<Goal> ND::iffE(const Goal &goal) {
    return {goal};
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
    return {goal};
}
