#ifndef COMPITUM_INTERACTOR_INCLUDED
#define COMPITUM_INTERACTOR_INCLUDED

#include <chrono>
#include <istream>
#include <ostream>

namespace compitum {

using namespace std::literals;

using std::chrono::milliseconds;

struct interactor {
    std::istream& in;
    std::ostream& out;

    struct configuration {
       milliseconds
            short_delay = 125ms,
            long_delay  = 350ms;
    } config;

    void print_slow(std::string const&) const;
};

template <class T>
interactor& operator<<(interactor& io, T const& x) {
    io.out << x;
    return io;
}

}

#endif
