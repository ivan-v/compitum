#ifndef COMPITUM_REGION_INCLUDED
#define COMPITUM_REGION_INCLUDED

#include "compitum/infrastructure.hpp"

#include <vector>

namespace compitum {

struct region {
    std::vector<infrastructure> infras;
};

}

#endif
