#pragma once

#include "RandomGenerator.h"

#include <bit>
#include <cstdint>
#include <string>
#include <string_view>

namespace gen {

class LCG {
private:
    uint64_t _state;
    uint64_t _multiplier;
    uint64_t _increment;
    uint64_t _modulus;
    std::string _name;

public:
    LCG()
        : _state(1),
          _multiplier(1103515245ULL),
          _increment(12345ULL),
          _modulus(1ULL << 31),
          _name("Linear Congruential Generator") {}

    explicit LCG(SEED seed)
        : _state(seed),
          _multiplier(1103515245ULL),
          _increment(12345ULL),
          _modulus(1ULL << 31),
          _name("Linear Congruential Generator") {}

    LCG(SEED seed, uint64_t multiplier, uint64_t increment, uint64_t modulus, std::string name = "Linear Congruential Generator")
        : _state(seed % modulus),
          _multiplier(multiplier),
          _increment(increment),
          _modulus(modulus),
          _name(std::move(name)) {}

    uint64_t next() {
        __uint128_t value = static_cast<__uint128_t>(_multiplier) * _state + _increment;
        _state = static_cast<uint64_t>(value % _modulus);
        return _state;
    }

    void seed(SEED value) {
        _state = value % _modulus;
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

static_assert(RandomGenerator<LCG>);

}
