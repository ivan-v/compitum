#ifndef COMPITUM_INTERACTOR_INCLUDED
#define COMPITUM_INTERACTOR_INCLUDED

#include <chrono>
#include <istream>
#include <ostream>

namespace compitum {

struct interactor {
    std::istream& in;
    std::ostream& out;

    void print_slow(std::string const&) const;
    void print_fast(std::string const&) const;
};

template <class T>
interactor& operator<<(interactor& io, T const& x) {
    io.out << x;
    return io;
}

}

#endif
