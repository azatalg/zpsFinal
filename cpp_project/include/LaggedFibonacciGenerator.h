#pragma once

#include "RandomGenerator.h"

#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

namespace gen {

class LaggedFibonacciGenerator {
private:
    std::vector<uint64_t> _state;
    std::size_t _shortLag;
    std::size_t _longLag;
    std::size_t _index;
    std::string _name;

    static uint64_t splitmix64(uint64_t& value) {
        value += 0x9e3779b97f4a7c15ULL;
        uint64_t z = value;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }

public:
    LaggedFibonacciGenerator(SEED seed = 42, std::size_t shortLag = 24, std::size_t longLag = 55, std::string name = "Lagged Fibonacci Generator")
        : _state(longLag),
          _shortLag(shortLag),
          _longLag(longLag),
          _index(0),
          _name(std::move(name)) {
        this->seed(seed);
    }

    uint64_t next() {
        std::size_t i = (_index + _longLag - _shortLag) % _longLag;
        std::size_t j = _index;
        uint64_t value = _state[i] + _state[j];
        _state[_index] = value;
        _index = (_index + 1) % _longLag;
        return value;
    }

    void seed(SEED value) {
        uint64_t x = value;
        for (auto& element : _state) {
            element = splitmix64(x);
        }
        _index = 0;
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

static_assert(RandomGenerator<LaggedFibonacciGenerator>);

}
