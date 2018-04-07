#ifndef TRADE_GOOD_INCLUDED
#define TRADE_GOOD_INCLUDED

#include <array>
#include <map>
#include <ostream>
#include <string>

enum class trade_good_id { food, water, wood, stone, marble };

std::string to_string(trade_good_id);

int get_price_constant(trade_good_id);

using trade_good_amount_map = std::map<trade_good_id, int>;

std::ostream& operator<<(
        std::ostream&,
        trade_good_amount_map::value_type const&);

#endif
