#pragma once

#include "RandomGenerator.h"

#include <cstdint>
#include <limits>
#include <random>
#include <string>
#include <string_view>

namespace gen {

class MersenneTwisterGenerator {
private:
    std::mt19937_64 _engine;
    std::string _name;

public:
    explicit MersenneTwisterGenerator(SEED seed = 5489ULL, std::string name = "Mersenne Twister 64")
        : _engine(seed),
          _name(std::move(name)) {}

    uint64_t next() {
        return _engine();
    }

    void seed(SEED value) {
        _engine.seed(value);
    }

    std::string_view name() const {
        return _name;
    }

    uint64_t min() const {
        return std::mt19937_64::min();
    }

    uint64_t max() const {
        return std::mt19937_64::max();
    }

    int outputBits() const {
        return 64;
    }
};

static_assert(RandomGenerator<MersenneTwisterGenerator>);

}
