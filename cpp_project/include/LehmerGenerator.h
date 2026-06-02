#pragma once

#include "RandomGenerator.h"

#include <bit>
#include <cstdint>
#include <string>
#include <string_view>

namespace gen {

class LehmerGenerator {
private:
    uint64_t _state;
    uint64_t _multiplier;
    uint64_t _modulus;
    std::string _name;

public:
    LehmerGenerator(SEED seed = 1, uint64_t multiplier = 48271ULL, uint64_t modulus = 2147483647ULL, std::string name = "Lehmer Generator")
        : _state(seed % modulus),
          _multiplier(multiplier),
          _modulus(modulus),
          _name(std::move(name)) {
        if (_state == 0) {
            _state = 1;
        }
    }

    uint64_t next() {
        __uint128_t value = static_cast<__uint128_t>(_multiplier) * _state;
        _state = static_cast<uint64_t>(value % _modulus);
        return _state;
    }

    void seed(SEED value) {
        _state = value % _modulus;
        if (_state == 0) {
            _state = 1;
        }
    }

    std::string_view name() const {
        return _name;
    }

    uint64_t min() const {
        return 1;
    }

    uint64_t max() const {
        return _modulus - 1;
    }

    int outputBits() const {
        return static_cast<int>(std::bit_width(_modulus - 1));
    }
};

static_assert(RandomGenerator<LehmerGenerator>);

}
