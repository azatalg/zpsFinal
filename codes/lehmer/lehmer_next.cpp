uint64_t next() {
    __uint128_t value = static_cast<__uint128_t>(_multiplier) * _state;
    _state = static_cast<uint64_t>(value % _modulus);
    return _state;
}
