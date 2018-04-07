#include "trade_good.hpp"

std::ostream& operator<<(std::ostream& out, trade_good const& value) {
    return out
        << "{ Price constant = " << value.price_const
        << ", Amount = "         << value.amount
        << " }";
}
