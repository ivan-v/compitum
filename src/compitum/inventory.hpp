#ifndef COMPITUM_INVENTORY_INCLUDED
#define COMPITUM_INVENTORY_INCLUDED

#include "compitum/apparel.hpp"

#include <iostream>
#include <vector>

namespace compitum {

struct inventory {
    int weight;
    int weight_limit;
    int value;
    std::vector<apparel> apparel_list;
};

}

#endif