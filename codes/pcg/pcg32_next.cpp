uint32_t next() {
    uint64_t oldState = _state;
    _state = oldState * _multiplier + _increment;

    uint32_t xorshifted = static_cast<uint32_t>(
        ((oldState >> 18U) ^ oldState) >> 27U
    );

    uint32_t rotation = static_cast<uint32_t>(oldState >> 59U);

    return (xorshifted >> rotation) |
           (xorshifted << ((-rotation) & 31));
}
