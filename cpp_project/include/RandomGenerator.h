#pragma once

#include <algorithm>
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

namespace gen {

using SEED = uint64_t;

inline constexpr std::string_view PlotBlue = "#2563eb";
inline constexpr std::string_view PlotBlueDark = "#1e3a8a";
inline constexpr std::string_view PlotBlueLight = "#93c5fd";
inline constexpr std::string_view PlotGrid = "#dbeafe";
inline constexpr std::string_view PlotAxis = "#111827";

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

template <RandomGenerator R>
void printSequence(R& generator, int count) {
    std::cout << generator.name() << ":\n";
    for (int i = 0; i < count; ++i) {
        std::cout << generator.next() << '\n';
    }
    std::cout << '\n';
}

struct GeneratorStats {
    uint64_t min = std::numeric_limits<uint64_t>::max();
    uint64_t max = std::numeric_limits<uint64_t>::min();
    long double mean = 0.0L;
    long double variance = 0.0L;
    long double stddev = 0.0L;
    uint64_t zeroCount = 0;
    uint64_t oneBitCount = 0;
    uint64_t totalBitCount = 0;
    long double oneBitRatio = 0.0L;
    std::vector<uint64_t> histogram;
    long double chiSquare = 0.0L;
};

template <RandomGenerator R>
GeneratorStats computeStats(R& generator, int sampleCount, int bucketCount = 20) {
    GeneratorStats stats;
    stats.histogram.assign(bucketCount, 0);
    std::vector<long double> values;
    values.reserve(sampleCount);
    long double sum = 0.0L;
    long double minOutput = static_cast<long double>(generator.min());
    long double maxOutput = static_cast<long double>(generator.max());
    long double outputRange = maxOutput - minOutput + 1.0L;
    if (outputRange <= 0.0L) {
        outputRange = static_cast<long double>(std::numeric_limits<uint64_t>::max()) + 1.0L;
    }

    for (int i = 0; i < sampleCount; ++i) {
        uint64_t x = generator.next();
        stats.min = std::min(stats.min, x);
        stats.max = std::max(stats.max, x);
        if (x == 0) {
            ++stats.zeroCount;
        }
        stats.oneBitCount += static_cast<uint64_t>(std::popcount(x));
        stats.totalBitCount += static_cast<uint64_t>(generator.outputBits());
        long double value = static_cast<long double>(x);
        values.push_back(value);
        sum += value;
        int bucket = static_cast<int>(((value - minOutput) / outputRange) * bucketCount);
        bucket = std::clamp(bucket, 0, bucketCount - 1);
        ++stats.histogram[bucket];
    }

    stats.mean = sum / static_cast<long double>(sampleCount);
    long double squaredSum = 0.0L;
    for (long double value : values) {
        long double diff = value - stats.mean;
        squaredSum += diff * diff;
    }
    stats.variance = squaredSum / static_cast<long double>(sampleCount);
    stats.stddev = std::sqrt(stats.variance);
    if (stats.totalBitCount != 0) {
        stats.oneBitRatio = static_cast<long double>(stats.oneBitCount) / static_cast<long double>(stats.totalBitCount);
    }

    long double expected = static_cast<long double>(sampleCount) / static_cast<long double>(bucketCount);
    for (uint64_t observed : stats.histogram) {
        long double diff = static_cast<long double>(observed) - expected;
        stats.chiSquare += (diff * diff) / expected;
    }
    return stats;
}

inline void printStats(const GeneratorStats& stats) {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "min:           " << stats.min << '\n';
    std::cout << "max:           " << stats.max << '\n';
    std::cout << "mean:          " << static_cast<double>(stats.mean) << '\n';
    std::cout << "variance:      " << static_cast<double>(stats.variance) << '\n';
    std::cout << "stddev:        " << static_cast<double>(stats.stddev) << '\n';
    std::cout << "zero count:    " << stats.zeroCount << '\n';
    std::cout << "one bits:      " << stats.oneBitCount << '\n';
    std::cout << "total bits:    " << stats.totalBitCount << '\n';
    std::cout << "one bit ratio: " << static_cast<double>(stats.oneBitRatio) << '\n';
    std::cout << "chi-square:    " << static_cast<double>(stats.chiSquare) << '\n';
    std::cout << "histogram:\n";
    for (std::size_t i = 0; i < stats.histogram.size(); ++i) {
        std::cout << "  bucket " << i << ": " << stats.histogram[i] << '\n';
    }
    std::cout << '\n';
}

inline void saveStatsText(const GeneratorStats& stats, std::string_view filename, std::string_view title) {
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    std::ofstream out{std::string(filename)};
    out << std::fixed << std::setprecision(6);
    out << title << '\n';
    out << "min: " << stats.min << '\n';
    out << "max: " << stats.max << '\n';
    out << "mean: " << static_cast<double>(stats.mean) << '\n';
    out << "variance: " << static_cast<double>(stats.variance) << '\n';
    out << "stddev: " << static_cast<double>(stats.stddev) << '\n';
    out << "zero count: " << stats.zeroCount << '\n';
    out << "one bits: " << stats.oneBitCount << '\n';
    out << "total bits: " << stats.totalBitCount << '\n';
    out << "one bit ratio: " << static_cast<double>(stats.oneBitRatio) << '\n';
    out << "chi-square: " << static_cast<double>(stats.chiSquare) << '\n';
}

inline void beginSvg(std::ofstream& out, int width, int height, std::string_view title) {
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height << "\">\n";
    out << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    out << "<text x=\"" << width / 2 << "\" y=\"34\" font-size=\"22\" text-anchor=\"middle\" font-family=\"Arial\" fill=\"" << PlotAxis << "\">" << title << "</text>\n";
}

inline void drawAxes(std::ofstream& out, int left, int top, int plotWidth, int plotHeight) {
    int bottom = top + plotHeight;
    int right = left + plotWidth;
    for (int i = 1; i <= 4; ++i) {
        int y = top + i * plotHeight / 5;
        out << "<line x1=\"" << left << "\" y1=\"" << y << "\" x2=\"" << right << "\" y2=\"" << y << "\" stroke=\"" << PlotGrid << "\"/>\n";
    }
    out << "<line x1=\"" << left << "\" y1=\"" << bottom << "\" x2=\"" << right << "\" y2=\"" << bottom << "\" stroke=\"" << PlotAxis << "\"/>\n";
    out << "<line x1=\"" << left << "\" y1=\"" << top << "\" x2=\"" << left << "\" y2=\"" << bottom << "\" stroke=\"" << PlotAxis << "\"/>\n";
}

inline void saveHistogramSvg(const GeneratorStats& stats, std::string_view filename, std::string_view title = "Histogram") {
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    const int width = 820;
    const int height = 520;
    const int left = 72;
    const int rightMargin = 32;
    const int top = 62;
    const int bottomMargin = 72;
    const int plotWidth = width - left - rightMargin;
    const int plotHeight = height - top - bottomMargin;
    uint64_t maxCount = 1;
    for (uint64_t value : stats.histogram) {
        maxCount = std::max(maxCount, value);
    }
    std::ofstream out{std::string(filename)};
    beginSvg(out, width, height, title);
    drawAxes(out, left, top, plotWidth, plotHeight);
    int bucketCount = static_cast<int>(stats.histogram.size());
    double gap = 6.0;
    double barWidth = (static_cast<double>(plotWidth) - gap * (bucketCount - 1)) / bucketCount;
    for (int i = 0; i < bucketCount; ++i) {
        double normalized = static_cast<double>(stats.histogram[i]) / static_cast<double>(maxCount);
        double barHeight = normalized * plotHeight;
        double x = left + i * (barWidth + gap);
        double y = top + plotHeight - barHeight;
        out << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << barWidth << "\" height=\"" << barHeight << "\" fill=\"" << PlotBlue << "\"/>\n";
        out << "<text x=\"" << x + barWidth / 2 << "\" y=\"" << top + plotHeight + 20 << "\" font-size=\"11\" text-anchor=\"middle\" font-family=\"Arial\" fill=\"" << PlotAxis << "\">" << i << "</text>\n";
    }
    out << "<text x=\"" << width / 2 << "\" y=\"" << height - 20 << "\" font-size=\"16\" text-anchor=\"middle\" font-family=\"Arial\" fill=\"" << PlotAxis << "\">Bucket</text>\n";
    out << "<text x=\"22\" y=\"" << height / 2 << "\" font-size=\"16\" text-anchor=\"middle\" font-family=\"Arial\" transform=\"rotate(-90 22 " << height / 2 << ")\" fill=\"" << PlotAxis << "\">Count</text>\n";
    out << "</svg>\n";
}

inline void saveBitRatioSvg(const GeneratorStats& stats, std::string_view filename, std::string_view title = "Bit ratio") {
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    const int width = 520;
    const int height = 420;
    const int margin = 74;
    const int plotHeight = height - 2 * margin;
    const int baseY = height - margin;
    long double zeroRatio = 1.0L - stats.oneBitRatio;
    long double oneRatio = stats.oneBitRatio;
    std::ofstream out{std::string(filename)};
    beginSvg(out, width, height, title);
    drawAxes(out, margin, margin, width - 2 * margin, plotHeight);
    int idealY = baseY - static_cast<int>(0.5 * plotHeight);
    out << "<line x1=\"" << margin << "\" y1=\"" << idealY << "\" x2=\"" << width - margin << "\" y2=\"" << idealY << "\" stroke=\"" << PlotBlueDark << "\" stroke-dasharray=\"6,6\"/>\n";
    auto drawBar = [&](int index, std::string_view label, long double ratio) {
        int x = 148 + index * 150;
        int barWidth = 100;
        int barHeight = static_cast<int>(ratio * plotHeight);
        int y = baseY - barHeight;
        out << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << barWidth << "\" height=\"" << barHeight << "\" fill=\"" << PlotBlue << "\"/>\n";
        out << "<text x=\"" << x + barWidth / 2 << "\" y=\"" << baseY + 25 << "\" font-size=\"15\" text-anchor=\"middle\" font-family=\"Arial\" fill=\"" << PlotAxis << "\">" << label << "</text>\n";
        out << "<text x=\"" << x + barWidth / 2 << "\" y=\"" << y - 8 << "\" font-size=\"13\" text-anchor=\"middle\" font-family=\"Arial\" fill=\"" << PlotAxis << "\">" << std::fixed << std::setprecision(6) << static_cast<double>(ratio) << "</text>\n";
    };
    drawBar(0, "0 bits", zeroRatio);
    drawBar(1, "1 bits", oneRatio);
    out << "<text x=\"" << width - margin << "\" y=\"" << idealY - 7 << "\" font-size=\"12\" text-anchor=\"end\" font-family=\"Arial\" fill=\"" << PlotBlueDark << "\">ideal 0.5</text>\n";
    out << "</svg>\n";
}

template <RandomGenerator R>
void saveSequenceSvg(R& generator, int sampleCount, std::string_view filename, std::string_view title = "Sequence plot") {
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    const int width = 920;
    const int height = 520;
    const int left = 72;
    const int rightMargin = 32;
    const int top = 62;
    const int bottomMargin = 72;
    const int plotWidth = width - left - rightMargin;
    const int plotHeight = height - top - bottomMargin;
    std::vector<uint64_t> values;
    values.reserve(sampleCount);
    uint64_t minValue = std::numeric_limits<uint64_t>::max();
    uint64_t maxValue = std::numeric_limits<uint64_t>::min();
    for (int i = 0; i < sampleCount; ++i) {
        uint64_t value = generator.next();
        values.push_back(value);
        minValue = std::min(minValue, value);
        maxValue = std::max(maxValue, value);
    }
    long double range = static_cast<long double>(maxValue) - static_cast<long double>(minValue);
    if (range == 0.0L) {
        range = 1.0L;
    }
    std::ofstream out{std::string(filename)};
    beginSvg(out, width, height, title);
    drawAxes(out, left, top, plotWidth, plotHeight);
    for (int i = 0; i < sampleCount; ++i) {
        long double normalized = (static_cast<long double>(values[i]) - static_cast<long double>(minValue)) / range;
        double x = left + (static_cast<double>(i) / static_cast<double>(std::max(1, sampleCount - 1))) * plotWidth;
        double y = top + plotHeight - static_cast<double>(normalized) * plotHeight;
        out << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"1.45\" fill=\"" << PlotBlue << "\" fill-opacity=\"0.72\"/>\n";
    }
    out << "<text x=\"" << width / 2 << "\" y=\"" << height - 20 << "\" font-size=\"16\" text-anchor=\"middle\" font-family=\"Arial\" fill=\"" << PlotAxis << "\">n</text>\n";
    out << "<text x=\"22\" y=\"" << height / 2 << "\" font-size=\"16\" text-anchor=\"middle\" font-family=\"Arial\" transform=\"rotate(-90 22 " << height / 2 << ")\" fill=\"" << PlotAxis << "\">x_n</text>\n";
    out << "</svg>\n";
}

template <RandomGenerator R>
void saveScatterSvg(R& generator, int sampleCount, std::string_view filename, std::string_view title = "Successive values") {
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    const int width = 720;
    const int height = 720;
    const int margin = 76;
    const int plotWidth = width - 2 * margin;
    const int plotHeight = height - 2 * margin;
    std::vector<uint64_t> values;
    values.reserve(sampleCount + 1);
    for (int i = 0; i < sampleCount + 1; ++i) {
        values.push_back(generator.next());
    }
    long double minOutput = static_cast<long double>(generator.min());
    long double maxOutput = static_cast<long double>(generator.max());
    long double range = maxOutput - minOutput;
    if (range <= 0.0L) {
        range = static_cast<long double>(std::numeric_limits<uint64_t>::max());
    }
    std::ofstream out{std::string(filename)};
    beginSvg(out, width, height, title);
    drawAxes(out, margin, margin, plotWidth, plotHeight);
    for (int i = 0; i < sampleCount; ++i) {
        long double xNorm = (static_cast<long double>(values[i]) - minOutput) / range;
        long double yNorm = (static_cast<long double>(values[i + 1]) - minOutput) / range;
        double x = margin + static_cast<double>(xNorm) * plotWidth;
        double y = height - margin - static_cast<double>(yNorm) * plotHeight;
        out << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"1.25\" fill=\"" << PlotBlue << "\" fill-opacity=\"0.58\"/>\n";
    }
    out << "<text x=\"" << width / 2 << "\" y=\"" << height - 22 << "\" font-size=\"16\" text-anchor=\"middle\" font-family=\"Arial\" fill=\"" << PlotAxis << "\">x_n</text>\n";
    out << "<text x=\"27\" y=\"" << height / 2 << "\" font-size=\"16\" text-anchor=\"middle\" font-family=\"Arial\" transform=\"rotate(-90 27 " << height / 2 << ")\" fill=\"" << PlotAxis << "\">x_{n+1}</text>\n";
    out << "</svg>\n";
}

template <RandomGenerator R>
void generateAllPlots(R& generator, int statsSampleCount, int plotSampleCount, int bucketCount, std::string_view outputPrefix) {
    std::string prefix(outputPrefix);
    GeneratorStats stats = computeStats(generator, statsSampleCount, bucketCount);
    std::cout << "Analysis of " << generator.name() << "\n";
    printStats(stats);
    saveStatsText(stats, prefix + "_stats.txt", generator.name());
    saveHistogramSvg(stats, prefix + "_histogram.svg", std::string(generator.name()) + " histogram");
    saveBitRatioSvg(stats, prefix + "_bit_ratio.svg", std::string(generator.name()) + " bit ratio");
    saveSequenceSvg(generator, plotSampleCount, prefix + "_sequence.svg", std::string(generator.name()) + " sequence");
    saveScatterSvg(generator, plotSampleCount, prefix + "_successive_values.svg", std::string(generator.name()) + " successive values");
}

}
