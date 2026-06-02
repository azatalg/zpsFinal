uint32_t next() {
    if (_index >= _state.size()) {
        twistWithoutMatrixA();
    }

    return _state[_index++];
}
