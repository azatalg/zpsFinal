uint32_t next() {
    if (_index >= _state.size()) {
        twist();
    }

    return _state[_index++];
}
