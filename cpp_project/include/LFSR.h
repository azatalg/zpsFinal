#pragma once

#include "RandomGenerator.h"

#include <bit>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace gen {

class LFSR {
private:
    uint64_t _state;
    int _width;
    std::vector<int> _taps;
    std::string _name;

    uint64_t mask() const {
        if (_width >= 64) {
            return std::numeric_limits<uint64_t>::max();
        }
        return (1ULL << _width) - 1ULL;
    }

public:
    LFSR(SEED seed = 0xACE1ULL, int width = 16, std::vector<int> taps = {0, 2, 3, 5}, std::string name = "Linear Feedback Shift Register")
        : _state(seed),
          _width(width),
          _taps(std::move(taps)),
          _name(std::move(name)) {
        _state &= mask();
    }

    uint64_t next() {
        uint64_t feedback = 0;
        for (int tap : _taps) {
            feedback ^= (_state >> tap) & 1ULL;
        }
        _state = ((_state >> 1) | (feedback << (_width - 1))) & mask();
        return _state;
    }

    void seed(SEED value) {
        _state = value & mask();
    }

    std::string_view name() const {
        return _name;
    }

    uint64_t min() const {
        return 0;
    }

    uint64_t max() const {
        return mask();
    }

    int outputBits() const {
        return _width;
    }
};

static_assert(RandomGenerator<LFSR>);

}
