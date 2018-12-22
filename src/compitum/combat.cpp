#include "compitum/combat.hpp"
#include "compitum/dice.hpp"
#include "compitum/interactor.hpp"

#include <cassert>
// using std::chrono::milliseconds;
// using std::this_thread::sleep_for;

//TODO: make armor and defense checks
int compitum::attempt_block(character& self) {
    int stamina_cost = 10; //TODO: make dynamic
    //if (incoming_attack.damage_type == "")
    if (self.stamina >= stamina_cost) {
        return 1;
    }
    else
        return 0;
}

int compitum::attempt_strike(character& self, character& target,
                               int distance_between_characters) {
    if (self.attack_kind.stamina_cost <= self.stamina) {
        drain_stamina(self, self.attack_kind.stamina_cost);
        int damage_dealt = roll_dice(
            self.attack_kind.roll_dice_faces,
            self.attack_kind.roll_dice_times, 
            self.attack_kind.roll_dice_modifier);
        drain_hp(target, damage_dealt);
        return damage_dealt;
    }
    return 0;
}

//player must type in attack quickly enough
int compitum::player_action(character& self, int milliseconds_allowed) {
    std::string input;
    auto start = std::chrono::steady_clock::now();
    getline(std::cin, input);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now( ) - start);
    if (elapsed.count() < milliseconds_allowed) {
        if (input.compare(self.attack_kind.name)==0)
            return 1;
        else if (input == "heal")
            return 2;
        else if (input == "flee")
            return 3;
        else if (input == "block")
            return 4;
        else 
            return -1; 
    }
    else 
        return 0;
}

//TODO: make enemy action functions, such as different attacks
void compitum::enemy_action(character& self, character& enemy, 
                  int distance_between_characters, int block) {
    interactor io{std::cin, std::cout};
    if (self.hp < 10) {
        replenish_hp(self, 5);
        io.print_fast(self.character_name + " healed themselves to "
                  + std::to_string(self.hp) + " health!");
    } else if (self.stamina >= 5) {
        if (block == 0) {
            io.print_fast(self.character_name + " strikes " + enemy.character_name 
                      + ", dealing " + 
                      std::to_string(attempt_strike(self, enemy, distance_between_characters))
                      + " damage!");
            io.print_fast(enemy.character_name + " has " + std::to_string(enemy.hp) 
                      + " health left.");
        } else {
            io.print_fast(enemy.character_name + " successfully blocks "
                      + self.character_name + "'s attack!"); 
        } 
    } else {
        replenish_stamina(self, 3);
        io.print_fast(self.character_name + " repleninshes their stamina.");
    }
}