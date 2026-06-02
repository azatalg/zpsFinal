uint32_t next() {
    _state ^= _state << _shiftA;
    _state ^= _state >> _shiftB;
    _state ^= _state << _shiftC;
    return _state;
}
