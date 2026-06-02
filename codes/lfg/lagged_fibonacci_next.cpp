uint64_t next() {
    std::size_t first = (_index + _longLag - _shortLag) % _longLag;
    std::size_t second = _index;
    uint64_t value = _state[first] + _state[second];
    _state[_index] = value;
    _index = (_index + 1) % _longLag;
    return value;
}
