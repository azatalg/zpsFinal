#pragma once

#include "RandomGenerator.h"

#include <bit>
#include <cstdint>
#include <string>
#include <string_view>

namespace gen {

template <RandomGenerator A, RandomGenerator B>
class CombinedGenerator {
private:
    A _first;
    B _second;
    uint64_t _modulus;
    std::string _name;

public:
    CombinedGenerator(A first, B second, uint64_t modulus, std::string name = "Combined Generator")
        : _first(std::move(first)),
          _second(std::move(second)),
          _modulus(modulus),
          _name(std::move(name)) {}

    uint64_t next() {
        uint64_t x = _first.next() % _modulus;
        uint64_t y = _second.next() % _modulus;
        return (x + _modulus - y) % _modulus;
    }

    void seed(SEED value) {
        _first.seed(value);
        _second.seed(value + 0x9e3779b97f4a7c15ULL);
    }

    std::string_view name() const {
        return _name;
    }

    uint64_t min() const {
        return 0;
    }

    uint64_t max() const {
        return _modulus - 1;
    }

    int outputBits() const {
        return static_cast<int>(std::bit_width(_modulus - 1));
    }
};

}
