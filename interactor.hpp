#ifndef INTERACTOR_INCLUDED
#define INTERACTOR_INCLUDED

#include <chrono>
#include <istream>
#include <ostream>

struct interactor {
    std::istream& in;
    std::ostream& out;

    struct configuration {
        std::chrono::milliseconds
            short_delay = std::chrono::milliseconds(125),
            long_delay  = std::chrono::milliseconds(350);
    } config;

    void print_slow(std::string const&) const;
};

template <class T>
interactor& operator<<(interactor& io, T const& x) {
    io.out << x;
    return io;
}

#endif
