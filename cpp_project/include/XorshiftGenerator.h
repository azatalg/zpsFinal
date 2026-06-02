#pragma once

#include "RandomGenerator.h"

#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

namespace gen {

class Xorshift32 {
private:
    uint32_t _state;
    int _leftA;
    int _rightB;
    int _leftC;
    std::string _name;

public:
    Xorshift32(SEED seed = 1, int leftA = 13, int rightB = 17, int leftC = 5, std::string name = "Xorshift32")
        : _state(static_cast<uint32_t>(seed)),
          _leftA(leftA),
          _rightB(rightB),
          _leftC(leftC),
          _name(std::move(name)) {}

    uint64_t next() {
        uint32_t x = _state;
        x ^= x << _leftA;
        x ^= x >> _rightB;
        x ^= x << _leftC;
        _state = x;
        return _state;
    }

    void seed(SEED value) {
        _state = static_cast<uint32_t>(value);
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

class XorshiftStar64 {
private:
    uint64_t _state;
    uint64_t _multiplier;
    std::string _name;

public:
    XorshiftStar64(SEED seed = 1, uint64_t multiplier = 2685821657736338717ULL, std::string name = "XorshiftStar64")
        : _state(seed),
          _multiplier(multiplier),
          _name(std::move(name)) {
        if (_state == 0) {
            _state = 1;
        }
    }

    uint64_t next() {
        uint64_t x = _state;
        x ^= x >> 12;
        x ^= x << 25;
        x ^= x >> 27;
        _state = x;
        return x * _multiplier;
    }

    void seed(SEED value) {
        _state = value == 0 ? 1 : value;
    }

    std::string_view name() const {
        return _name;
    }

    uint64_t min() const {
        return 0;
    }

    uint64_t max() const {
        return std::numeric_limits<uint64_t>::max();
    }

    int outputBits() const {
        return 64;
    }
};

static_assert(RandomGenerator<Xorshift32>);
static_assert(RandomGenerator<XorshiftStar64>);

}
