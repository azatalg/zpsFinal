uint64_t next() {
    uint64_t feedback = 0;

    for (int tap : _taps) {
        feedback ^= (_state >> tap) & 1ULL;
    }

    _state = (_state >> 1) | (feedback << (_width - 1));
    _state &= _mask;
    return _state;
}
