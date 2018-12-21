#include "compitum/interactor.hpp"

#include "compitum/config.hpp"

#include <thread>

using std::this_thread::sleep_for;

namespace compitum {

void interactor::print_slow(std::string const& text) const {
    for (char c : text){
        out << c << std::flush;
        sleep_for(c == '.' ? config.long_delay : config.short_delay);
    }
    out << "\n";
    sleep_for(config.long_delay*2);
}

void interactor::print_fast(std::string const& text) const {
    for (char c : text){
        out << c << std::flush;
        sleep_for(c == '.' ? config.short_delay : config.quick_delay);
    }
    out << "\n";    
    sleep_for(config.long_delay*1);
}

}
