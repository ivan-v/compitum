#ifndef COMPITUM_REGION_INCLUDED
#define COMPITUM_REGION_INCLUDED

#include <compitum/clock.hpp>
#include "compitum/infrastructure.hpp"

#include <vector>

namespace compitum {

struct region {
    std::vector<infrastructure> infras;
};

struct world {
    region reg1; // TODO: Make vector of regions
    world_time t1;
};

region simulate_turn(region reg);

}

#endif
