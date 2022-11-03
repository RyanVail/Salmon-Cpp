/* This handles all of the optimization passess during the intermediate phase. */

#ifndef INTERMEDIATE_OPTIMZATION_H
#define INTERMEDIATE_OPTIMZATION_H

#include<vector>
#include<intermediate/intermediate.hpp>
#include<typedefs.hpp>

/* ==== Current optimiztions ====
 * Constant evuations
 * 
 *
 *
 */


 /* ==== To add ====
  * Peephole shifting in place of multiplication
  */

namespace intermediate_optimization
{
    // This takes in a pointer to the static intermediate file and optimizes it.
    void optimzize_file(std::vector<inter> file);
}

#endif