#include "compitum/region.hpp"

#include <cmath>
#include <iomanip>

using namespace compitum;

namespace {

    struct collect_infra_result {
        int base_production;
        double production_modifier;
    };

}