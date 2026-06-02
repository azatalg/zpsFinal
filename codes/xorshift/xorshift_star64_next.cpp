uint64_t next() {
    _state ^= _state >> 12;
    _state ^= _state << 25;
    _state ^= _state >> 27;
    return _state * _multiplier;
}
