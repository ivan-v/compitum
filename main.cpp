#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>

enum class faction_id {
	bandits, blacksmiths, builders, craftsmen, guards, traders, prostitutes
};

constexpr std::array<faction_id, 7> faction_ids{{faction_id::bandits, faction_id::blacksmiths, 
	faction_id::builders, faction_id::craftsmen, faction_id::guards, faction_id::traders, faction_id::prostitutes}};


using opinion_map = std::map<faction_id, int>;

struct faction {
	int wealth; 	//TODO
	int pop;
	opinion_map opinions;
};

struct population {
	std::map<faction_id, faction> factions;

	int total() const {
		return std::accumulate(
			begin(factions), end(factions),
			0,
			[](int acc, auto const& item) {
				faction const& fac = item.second;
				return acc + fac.pop;
			});
	}
};

enum class trade_good_id {
	food
};

constexpr std::array<trade_good_id, 1> trade_good_ids{{trade_good_id::food}};


struct trade_good {
	trade_good_id id;
	double price_const;
	int amount;
};

std::ostream& operator<<(std::ostream& out, trade_good const& value) {
	return out << "{ Price constant = " << value.price_const
			   << ", Amount = "         << value.amount << " }";
}

struct region {
	//pop = population
	int provincial_production_value;

	double goods_produced_mod;

	population pop;

	trade_good food;
};

std::ostream& operator<<(std::ostream& out, region const& value) {
	return out
		<<   "Provincial production value: " << value.provincial_production_value
		<< "\nGoods produced mod:          " << value.goods_produced_mod
		<< "\nPopulation:                  " << value.pop.total()
		<< "\nFood:                        " << value.food;
}

int region_trade_prod(int provincial_production_value, double goods_produced_mod, int pop) {
	auto goods_produced = provincial_production_value * goods_produced_mod;
	auto local_demand = pop;
	auto net_supply = static_cast<int>(std::round(goods_produced - local_demand));
	//std::cout<<net_supply;
	return net_supply;
}

region simulate_turn(region r) {
	r.food.amount = region_trade_prod(r.provincial_production_value, r.goods_produced_mod, r.pop.total());
	if(r.food.amount < 0) {

		//TODO: Don't allow negative population
		int starved = static_cast<int>(-r.food.amount / static_cast<double>(faction_ids.size())); //TODO: not all people starve
		std::cout << "The people starve!\n" << starved << " people have died.\n";
		for (auto& item : r.pop.factions) {
			auto& fac = item.second;
			fac.pop -= starved;
		}
	}
	return r;
}

int main() {
//	std::cout << "Hello, world.\n";
	region reg1 {
		30,		// provincial production value
		1.00,   // "goods produced" modifier
		{		// population
			{	//   factions:  wealth, population, opinions
				{ faction_id::bandits,     { 0, 10, {} } },
				{ faction_id::blacksmiths, { 0, 20, {} } },
				{ faction_id::builders,    { 0,  0, {} } },
				{ faction_id::craftsmen,   { 0,  0, {} } },
				{ faction_id::guards,      { 0,  0, {} } },
				{ faction_id::traders,     { 0, 10, {} } },
				{ faction_id::prostitutes, { 0, 10, {} } },
			}
		},
		{ trade_good_id::food, 10, 1 }	// goods: amount, cost
	};
//	std::cout << reg1 << "\n";
	
	reg1 = simulate_turn(reg1);

	std::cout << reg1 << "\n";

}