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
	faction_id id;
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

struct trade_good {
	//string name;
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
	if(r.food.amount < 0){
		std::cout << "The people starve!\n" << -r.food.amount << " people have died.\n";
		//r.pop.total = std::max(0, r.pop.total() + r.food.amount);
		int starved = static_cast<int>(-r.food.amount / static_cast<double>(faction_ids.size()));
		for (auto& [id, fac] : r.pop.factions) {
			fac.pop -= starved;
		}
	}
	return r;
}

int main() {
//	std::cout << "Hello, world.\n";
	region reg1 {
		80,		// provincial production value
//		100,		// population
		1.00,   	// "goods produced" modifier
		{},		//total population
		{ 10, 1 }	// food: amount, cost
	};
//	std::cout << reg1 << "\n";
	
	reg1 = simulate_turn(reg1);


	std::cout << reg1 << "\n";




}