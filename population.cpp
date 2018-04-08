#include "population.hpp"

#include <numeric>

namespace compitum {

int population::total() const {
    return std::accumulate(
            begin(factions), end(factions),
            0,
            [](int acc, auto const& item) {
                faction const& fac = item.second;
                return acc + fac.pop;
            });
}

}
