#ifndef COMPITUM_INFRASTRUCTURE_INCLUDED
#define COMPITUM_INFRASTRUCTURE_INCLUDED

#include "trade_good.hpp"

#include <ostream>

namespace compitum {

enum class infrastructure_id { farm, well };

struct infrastructure {
    infrastructure_id id;
    trade_good_id product;
    int base_production;
    double production_modifier;
    int maintenance;
    // trade_good build_cost;  TODO
};

std::ostream& operator<<(std::ostream&, infrastructure const&);

constexpr infrastructure
    farm{ infrastructure_id::farm, trade_good_id::food, 15, 1.05, 0 },
    well{ infrastructure_id::well, trade_good_id::water, 40, 1.00, 0 };

}

#endif
