uint32_t next() {
    if (_index >= _state.size()) {
        twist();
    }

    uint32_t y = _state[_index++];

    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680U;
    y ^= (y << 15) & 0xEFC60000U;
    y ^= (y >> 18);

    return y;
}
