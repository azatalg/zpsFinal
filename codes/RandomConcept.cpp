using SEED = uint64_t;

template <typename G>
concept RandomGenerator =
requires(G gen, SEED seed) {
    { gen.next() } -> std::convertible_to<uint64_t>;
    { gen.seed(seed) } -> std::same_as<void>;
    { gen.name() } -> std::convertible_to<std::string_view>;
    { gen.min() } -> std::convertible_to<uint64_t>;
    { gen.max() } -> std::convertible_to<uint64_t>;
    { gen.outputBits() } -> std::convertible_to<int>;
};
