#include <filesystem>
#include <iostream>

#include "CombinedGenerator.h"
#include "LCG.h"
#include "LFSR.h"
#include "LaggedFibonacciGenerator.h"
#include "LehmerGenerator.h"
#include "MersenneTwisterGenerator.h"
#include "PCG32.h"
#include "RandomGenerator.h"
#include "XorshiftGenerator.h"

using gen::CombinedGenerator;
using gen::LCG;
using gen::LFSR;
using gen::LaggedFibonacciGenerator;
using gen::LehmerGenerator;
using gen::MersenneTwisterGenerator;
using gen::PCG32;
using gen::Xorshift32;
using gen::XorshiftStar64;

int main() {
    std::filesystem::create_directories("plots/classical/lcg/good");
    std::filesystem::create_directories("plots/classical/lcg/bad_large_range");
    std::filesystem::create_directories("plots/classical/lcg/single_bad_parameter");
    std::filesystem::create_directories("plots/classical/lehmer/good");
    std::filesystem::create_directories("plots/classical/lehmer/bad_large_range");
    std::filesystem::create_directories("plots/classical/lehmer/single_bad_parameter");
    std::filesystem::create_directories("plots/recursive/lfsr/good");
    std::filesystem::create_directories("plots/recursive/lfsr/bad_large_range");
    std::filesystem::create_directories("plots/recursive/lfsr/single_bad_parameter");
    std::filesystem::create_directories("plots/recursive/lfg/good");
    std::filesystem::create_directories("plots/recursive/lfg/bad_large_range");
    std::filesystem::create_directories("plots/recursive/lfg/single_bad_parameter");
    std::filesystem::create_directories("plots/modern/xorshift/good");
    std::filesystem::create_directories("plots/modern/xorshift/bad_large_range");
    std::filesystem::create_directories("plots/modern/xorshift/single_bad_parameter");
    std::filesystem::create_directories("plots/modern/pcg/good");
    std::filesystem::create_directories("plots/modern/pcg/bad_large_range");
    std::filesystem::create_directories("plots/modern/pcg/single_bad_parameter");
    std::filesystem::create_directories("plots/showcase/mersenne_twister");
    std::filesystem::create_directories("plots/showcase/xorshift_star");
    std::filesystem::create_directories("plots/showcase/combined");

    constexpr int statsSamples = 100000;
    constexpr int plotSamples = 5000;
    constexpr int buckets = 20;

    LCG goodLcg(123456789ULL, 1103515245ULL, 12345ULL, 1ULL << 31, "Good LCG");
    LCG badLargeRangeLcg(123456789ULL, 65539ULL, 0ULL, 1ULL << 31, "Bad large-range LCG");
    LCG singleBadLcg(123456789ULL, 1103515245ULL, 12344ULL, 1ULL << 31, "LCG with even increment");

    LehmerGenerator goodLehmer(123456789ULL, 48271ULL, 2147483647ULL, "Good Lehmer");
    LehmerGenerator badLargeRangeLehmer(123456789ULL, 2147483646ULL, 2147483647ULL, "Bad large-range Lehmer");
    LehmerGenerator singleBadLehmer(123456789ULL, 48270ULL, 2147483647ULL, "Lehmer with non-primitive multiplier");

    LFSR goodLfsr(0xACE1ULL, 16, {0, 2, 3, 5}, "Good LFSR");
    LFSR badLargeRangeLfsr(0xDEADBEEFULL, 32, {0, 1}, "Bad large-range LFSR");
    LFSR singleBadLfsr(0xACE1ULL, 16, {0, 2, 3}, "LFSR with one missing tap");

    LaggedFibonacciGenerator goodLfg(42ULL, 24, 55, "Good Lagged Fibonacci");
    LaggedFibonacciGenerator badLargeRangeLfg(42ULL, 1, 2, "Bad large-range Lagged Fibonacci");
    LaggedFibonacciGenerator singleBadLfg(1ULL, 24, 25, "Lagged Fibonacci with close lags");

    Xorshift32 goodXorshift(123456789ULL, 13, 17, 5, "Good Xorshift32");
    Xorshift32 badLargeRangeXorshift(123456789ULL, 31, 1, 31, "Bad large-range Xorshift32");
    Xorshift32 singleBadXorshift(123456789ULL, 13, 17, 1, "Xorshift32 with one weak shift");

    PCG32 goodPcg(42ULL, 54ULL, "Good PCG32");
    PCG32 badLargeRangePcg(42ULL, 6364136223846793005ULL, 0ULL, "Bad large-range PCG32");
    PCG32 singleBadPcg(42ULL, 6364136223846793005ULL, 108ULL, "PCG32 with even increment");

    MersenneTwisterGenerator mersenneTwister(42ULL, "Mersenne Twister 64");
    XorshiftStar64 xorshiftStar(42ULL, 2685821657736338717ULL, "XorshiftStar64");

    LCG combinedLcg(17ULL, 1103515245ULL, 12345ULL, 1ULL << 31, "LCG component");
    LehmerGenerator combinedLehmer(19ULL, 48271ULL, 2147483647ULL, "Lehmer component");
    CombinedGenerator goodCombined(combinedLcg, combinedLehmer, 2147483647ULL, "Good Combined Generator");

    gen::printSequence(goodLcg, 10);
    goodLcg.seed(123456789ULL);

    gen::generateAllPlots(goodLcg, statsSamples, plotSamples, buckets, "plots/classical/lcg/good/lcg_good");
    gen::generateAllPlots(badLargeRangeLcg, statsSamples, plotSamples, buckets, "plots/classical/lcg/bad_large_range/lcg_bad_large_range");
    gen::generateAllPlots(singleBadLcg, statsSamples, plotSamples, buckets, "plots/classical/lcg/single_bad_parameter/lcg_single_bad_parameter");

    gen::generateAllPlots(goodLehmer, statsSamples, plotSamples, buckets, "plots/classical/lehmer/good/lehmer_good");
    gen::generateAllPlots(badLargeRangeLehmer, statsSamples, plotSamples, buckets, "plots/classical/lehmer/bad_large_range/lehmer_bad_large_range");
    gen::generateAllPlots(singleBadLehmer, statsSamples, plotSamples, buckets, "plots/classical/lehmer/single_bad_parameter/lehmer_single_bad_parameter");

    gen::generateAllPlots(goodLfsr, statsSamples, plotSamples, buckets, "plots/recursive/lfsr/good/lfsr_good");
    gen::generateAllPlots(badLargeRangeLfsr, statsSamples, plotSamples, buckets, "plots/recursive/lfsr/bad_large_range/lfsr_bad_large_range");
    gen::generateAllPlots(singleBadLfsr, statsSamples, plotSamples, buckets, "plots/recursive/lfsr/single_bad_parameter/lfsr_single_bad_parameter");

    gen::generateAllPlots(goodLfg, statsSamples, plotSamples, buckets, "plots/recursive/lfg/good/lfg_good");
    gen::generateAllPlots(badLargeRangeLfg, statsSamples, plotSamples, buckets, "plots/recursive/lfg/bad_large_range/lfg_bad_large_range");
    gen::generateAllPlots(singleBadLfg, statsSamples, plotSamples, buckets, "plots/recursive/lfg/single_bad_parameter/lfg_single_bad_parameter");

    gen::generateAllPlots(goodXorshift, statsSamples, plotSamples, buckets, "plots/modern/xorshift/good/xorshift_good");
    gen::generateAllPlots(badLargeRangeXorshift, statsSamples, plotSamples, buckets, "plots/modern/xorshift/bad_large_range/xorshift_bad_large_range");
    gen::generateAllPlots(singleBadXorshift, statsSamples, plotSamples, buckets, "plots/modern/xorshift/single_bad_parameter/xorshift_single_bad_parameter");

    gen::generateAllPlots(goodPcg, statsSamples, plotSamples, buckets, "plots/modern/pcg/good/pcg_good");
    gen::generateAllPlots(badLargeRangePcg, statsSamples, plotSamples, buckets, "plots/modern/pcg/bad_large_range/pcg_bad_large_range");
    gen::generateAllPlots(singleBadPcg, statsSamples, plotSamples, buckets, "plots/modern/pcg/single_bad_parameter/pcg_single_bad_parameter");

    gen::generateAllPlots(mersenneTwister, statsSamples, plotSamples, buckets, "plots/showcase/mersenne_twister/mersenne_twister_64");
    gen::generateAllPlots(xorshiftStar, statsSamples, plotSamples, buckets, "plots/showcase/xorshift_star/xorshift_star_64");
    gen::generateAllPlots(goodCombined, statsSamples, plotSamples, buckets, "plots/showcase/combined/combined_good");

    return 0;
}
