#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

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
	std::array<trade_good_id, 2> food_and_water{{trade_good_id::food, trade_good_id::water}};
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

enum class infrastructure_id {
	farm, well
};

constexpr std::array<infrastructure_id, 2> infrastructure_ids{{infrastructure_id::farm, infrastructure_id::well}};

struct infrastructure {
	infrastructure_id id;
	trade_good_id product;
	int base_production;
	double production_modifier;
	int maintenance;
	//trade_good build_cost;  TODO
};

std::ostream& operator<<(std::ostream& out, infrastructure const& value) {
	return out << "{ Base production = " << value.base_production
			   << ", Production modifier = " << value.production_modifier << " }";
}

infrastructure const farm{ infrastructure_id::farm, trade_good_id::food, 15, 1.05, 0 };
infrastructure const well{ infrastructure_id::well, trade_good_id::water, 40, 1.00, 0 };

struct region {
	//pop = population
	int provincial_production_value;

	double goods_produced_mod;

	population pop;

	trade_good food, water;

	std::vector<infrastructure> infras;
};

std::ostream& operator<<(std::ostream& out, region const& value) {
	return out
		<<   "Provincial production value: " << value.provincial_production_value
		<< "\nGoods produced mod:          " << value.goods_produced_mod
		<< "\nPopulation:                  " << value.pop.total()
		<< "\nFood:                        " << value.food
		<< "\nWater:                       " << value.water;
//		<< "\nBuildings:                   " << value.infras;
}

struct collect_infra_result {
	int base_production;
	double production_modifier;
};
collect_infra_result collect_infra(std::vector<infrastructure> const& infras, trade_good_id product) {
	collect_infra_result result{0, 1.0};
	for (auto const& infra : infras) {
		if (infra.product == product) {
			result.base_production += infra.base_production;
			result.production_modifier *= infra.production_modifier;
		}
	}
	return result;
}

int region_trade_prod(region const& reg, trade_good_id product) {
	auto [base_production, production_modifier] = collect_infra(reg.infras, product);
	auto goods_produced = reg.provincial_production_value 
						* reg.goods_produced_mod
						* production_modifier 
						+ base_production;
	auto local_demand = reg.pop.total();
	auto net_supply = static_cast<int>(std::round(goods_produced - local_demand));
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

region starve_turn_tick(region reg) {
	int food_produced = region_trade_prod(reg, trade_good_id::food);
	reg.food.amount += food_produced;
	int old_total = reg.pop.total();
	if (reg.food.amount < 0) {
		reg.pop = kill_people(reg.pop, -reg.food.amount);
		int const starved = old_total - reg.pop.total();
		std::cout << "The people starve!\n" << starved << " people have died.\n";
	}
	if (food_produced > 0) {
		std::cout << "A surplus of " << food_produced << " food has been produced.\n";	
	}
	reg.water.amount += region_trade_prod(reg, trade_good_id::water);
	if (reg.water.amount < 0) {
		int old_total = reg.pop.total();
		reg.pop = kill_people(reg.pop, -reg.water.amount);
		int const starved = old_total - reg.pop.total();
		std::cout << "The people die of thirst!\n" << starved << " people have died.\n";
	}
	return reg;
}

region simulate_turn(region reg) {
	reg = starve_turn_tick(reg);
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
		{ trade_good_id::water, 10, 10 },
		{		// infrastructures
			{ farm, well },
		}
	};
	std::cout << reg1 << "\n";
	
	reg1 = simulate_turn(reg1);

	std::cout << reg1 << "\n";

}