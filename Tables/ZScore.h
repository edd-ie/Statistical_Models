//
// Created by _edd.ie_ on 01/05/2025.
//

#ifndef ZSCORE_H
#define ZSCORE_H

#include <array>
#include <cstdint>
#include <string>
#include <algorithm>


class ZScore {
    // Store Z-scores with 1 decimal place precision from -3.7 to 3.6
    static constexpr double Z_MIN = -3.7;
    static constexpr double Z_MAX = 3.6;
    static constexpr double Z_STEP = 0.1;
    static constexpr size_t Z_ROWS = static_cast<size_t>((Z_MAX - Z_MIN) / Z_STEP) + 1;

    // Store probabilities with 4 decimal places precision from 0.00 to 0.09
    static constexpr double P_STEP = 0.01;
    static constexpr size_t P_COLS = 10;  // 0.00 to 0.09

    // Store values as 16-bit integers to save memory (multiply actual values by 10000)
    std::array<std::array<uint16_t, P_COLS>, Z_ROWS> table;

public:
    ZScore();
    [[nodiscard]] double getProbability(double z, double decimal) const;
    [[nodiscard]] double getScore(double target_prob) const;
};

#endif //ZSCORE_H
