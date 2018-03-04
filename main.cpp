#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

enum class faction_id {
	bandits, blacksmiths, builders, craftsmen, farmers, guards, traders, prostitutes
};

constexpr std::array<faction_id, 8> faction_ids{{faction_id::bandits, faction_id::blacksmiths, 
	faction_id::builders, faction_id::craftsmen, faction_id::farmers, faction_id::guards, faction_id::traders, faction_id::prostitutes}};


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
	food, water, wood, stone, marble
};

constexpr std::array<trade_good_id, 5> trade_good_ids{{trade_good_id::food, trade_good_id::water, trade_good_id::stone,
 trade_good_id::marble, trade_good_id::wood}};


struct trade_good {
	trade_good_id id;
	double price_const;
	int amount;
};

std::ostream& operator<<(std::ostream& out, trade_good const& value) {
	return out << "{ Price constant = " << value.price_const
			   << ", Amount = "         << value.amount << " }";
}

enum class infrastructure_id {
	farms
};

constexpr std::array<infrastructure_id, 1> infrastructure_ids{{infrastructure_id::farms}};

struct infrastructure {
	infrastructure_id id;
	trade_good_id product;
	int base_production;
	double production_modifier;
	int maintenance;
	trade_good build_cost;
};

std::ostream& operator<<(std::ostream& out, infrastructure const& value) {
	return out << "{ Base production = " << value.base_production
			   << ", Production modifier = " << value.production_modifier << " }";
}

infrastructure const farms{ infrastructure_id::farms, trade_good_id::food, 15, 1.05, 0 };

struct region {
	//pop = population
	int provincial_production_value;

	double goods_produced_mod;

	population pop;

	trade_good food;

	std::vector<infrastructure> infras;
};

std::ostream& operator<<(std::ostream& out, region const& value) {
	return out
		<<   "Provincial production value: " << value.provincial_production_value
		<< "\nGoods produced mod:          " << value.goods_produced_mod
		<< "\nPopulation:                  " << value.pop.total()
		<< "\nFood:                        " << value.food;
}

int region_trade_prod(
		int provincial_production_value,
		double goods_produced_mod,
		int pop,
		std::vector<infrastructure> const& infras) {
	auto goods_produced = provincial_production_value
		* goods_produced_mod
		* farms.production_modifier
		+ farms.base_production;
	auto local_demand = pop;
	auto net_supply = static_cast<int>(std::round(goods_produced - local_demand));
	//std::cout<<net_supply;
	return net_supply;
}

// TODO: Don't always round fractional populations up.
population kill_people(population pop, int count) {
	double survival_rate = static_cast<double>(pop.total() - count) / pop.total();
	int killed = 0;
	for (auto pos = begin(pop.factions);
			pos != end(pop.factions) && killed < count;
			++pos) {
		auto& fac = pos->second;
		int new_pop = static_cast<int>(std::floor(fac.pop * survival_rate));
		killed += fac.pop - new_pop;
		fac.pop = new_pop;
	}
	return pop;
}

region simulate_turn(region reg) {
	reg.food.amount += region_trade_prod(reg.provincial_production_value, reg.goods_produced_mod, reg.pop.total(), reg.infras);
	if (reg.food.amount < 0) {
		int old_total = reg.pop.total();
		reg.pop = kill_people(reg.pop, -reg.food.amount);
		int const starved = old_total - reg.pop.total();
		std::cout << "The people starve!\n" << starved << " people have died.\n";
	}
	return reg;
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
		{ trade_good_id::food, 10, 10 },	// goods: price constant, amount
		{		// infrastructures
			{ infrastructure_id::farms, trade_good_id::food, 15, 1},
		}
	};
	std::cout << reg1 << "\n";
	
	reg1 = simulate_turn(reg1);

	std::cout << reg1 << "\n";

}