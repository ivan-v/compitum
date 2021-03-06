#ifndef COMPITUM_REGION_INCLUDED
#define COMPITUM_REGION_INCLUDED

#include "compitum/infrastructure.hpp"
#include "compitum/population.hpp"

#include <vector>

namespace compitum {

struct region {
    // pop = population
    int provincial_production_value;
    double goods_produced_mod;
    population pop;
    trade_good_amount_map trade_good_amounts;
    std::vector<infrastructure> infras;
};

std::ostream& operator<<(std::ostream&, region const&);

int gross_production(region const&, trade_good_id);

int net_production(region const&, trade_good_id);

int get_price(region const&, trade_good_id);

}

#endif
