/* This handles all of the optimization passess during the intermediate phase. */

#ifndef INTERMEDIATE_OPTIMZATION_H
#define INTERMEDIATE_OPTIMZATION_H

#include<vector>
#include<intermediate/intermediate.hpp>
#include<typedefs.hpp>
#include<error.hpp>

/* ==== Current optimiztions ====
 * Constant evuations.
 * Dead if and while statment removal.
 * Always do constant if.
 */

/* ==== To add ====
 * Dead code removal.
 * Peephole shifting in place of multiplication.
 * Register substitution.
 * Const variables.
 */

namespace intermediate_optimization
{
    // This takes in a pointer to the static intermediate file and optimizes it.
    void optimzize_file(std::vector<inter> file);
}

#endif