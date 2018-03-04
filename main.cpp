#include <iostream>

struct trade_good {
	//string name;
	double price_const;
	int amount;
};

struct region {
	//pop = population
	int provincial_production_value, goods_produced_mod,
		pop;

	trade_good food;
};

int region_trade_prod(int provincial_production_value, int goods_produced_mod, int pop) {
	auto goods_produced = provincial_production_value * 2 * goods_produced_mod;
	auto local_demand = pop;
	auto net_supply = goods_produced - local_demand;
	return net_supply;
}

void simulate_turn_region(region r) {
	r.food.amount = region_trade_prod(r.provincial_production_value, r.goods_produced_mod, r.pop);
	if(r.food.amount < 0){
		std::cout << "The people starve!\n";
		r.pop -= r.food.amount;
	}
}

int main() {
//	std::cout << "Hello, world.\n";
	region reg1;
	reg1.provincial_production_value = 3;



}