#include "interactor.hpp"

#include <thread>

using std::this_thread::sleep_for;

void interactor::print_slow(std::string const& text) const {
    for (char c : text){
        out << c << std::flush;
        sleep_for(c == '.' ? config.long_delay : config.short_delay);
    }
    out << "\n";
}
