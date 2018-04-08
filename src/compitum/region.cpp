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

static collect_infra_result collect_infra(
        std::vector<infrastructure> const& infras,
        trade_good_id product) {
    collect_infra_result result{0, 1.0};
    for (auto const& infra : infras) {
        if (infra.product == product) {
            result.base_production += infra.base_production;
            result.production_modifier *= infra.production_modifier;
        }
    }
    return result;
}

std::ostream& compitum::operator<<(std::ostream& out, region const& value) {
    out << "Provincial production value: " << value.provincial_production_value
        << "\nGoods produced mod:          " << value.goods_produced_mod
        << "\nPopulation:                  " << value.pop.total();
    for (auto [id, amount] : value.trade_good_amounts)
        out << "\n"
            << std::setw(29) << std::left
            << (to_string(id) + ":") << amount;
    // out << "\nBuildings:                   " << value.infras;
    return out;
}

int compitum::gross_production(region const& reg, trade_good_id product) {
    auto infra = collect_infra(reg.infras, product);
    auto goods_produced = reg.provincial_production_value
                        * reg.goods_produced_mod
                        * infra.production_modifier
                        + infra.base_production;
    return static_cast<int>(std::round(goods_produced));
}

int compitum::net_production(region const& reg, trade_good_id product) {
    auto goods_produced = gross_production(reg, product);
    auto local_demand = reg.pop.total();
    auto net_supply =
        static_cast<int>(std::round(goods_produced - local_demand));
    return net_supply;
}

int compitum::get_price(region const& reg, trade_good_id product){
    auto goods_produced = gross_production(reg, product);
    auto local_demand = reg.pop.total();
    auto constant = get_price_constant(product);
    auto price = std::max(
        static_cast<double>(local_demand) / goods_produced * constant,
        1.0);
    return static_cast<int>(std::round(price));
}
