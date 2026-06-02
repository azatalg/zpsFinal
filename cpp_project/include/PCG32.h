#pragma once

#include "RandomGenerator.h"

#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

namespace gen {

class PCG32 {
private:
    uint64_t _state;
    uint64_t _multiplier;
    uint64_t _increment;
    std::string _name;

    static uint32_t rotateRight(uint32_t value, unsigned rotation) {
        return (value >> rotation) | (value << ((-rotation) & 31));
    }

public:
    PCG32(SEED seed = 42, uint64_t stream = 54, std::string name = "PCG32")
        : _state(0),
          _multiplier(6364136223846793005ULL),
          _increment((stream << 1U) | 1U),
          _name(std::move(name)) {
        this->seed(seed);
    }

    PCG32(SEED seed, uint64_t multiplier, uint64_t increment, std::string name)
        : _state(seed),
          _multiplier(multiplier),
          _increment(increment),
          _name(std::move(name)) {}

    uint64_t next() {
        uint64_t oldState = _state;
        _state = oldState * _multiplier + _increment;
        uint32_t xorshifted = static_cast<uint32_t>(((oldState >> 18U) ^ oldState) >> 27U);
        uint32_t rotation = static_cast<uint32_t>(oldState >> 59U);
        return rotateRight(xorshifted, rotation);
    }

    void seed(SEED value) {
        _state = 0U;
        next();
        _state += value;
        next();
    }

    std::string_view name() const {
        return _name;
    }

    uint64_t min() const {
        return 0;
    }

    uint64_t max() const {
        return std::numeric_limits<uint32_t>::max();
    }

    int outputBits() const {
        return 32;
    }
};

static_assert(RandomGenerator<PCG32>);

}
